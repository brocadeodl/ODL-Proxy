/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Messaging/Remote/InterLocationMulticastMessage.h"
#include <time.h>

namespace WaveNs
{

LocationBase::LocationBase (UI32 locationId, string &ipAddress, SI32 port)
    : m_locationRole(LOCATION_STAND_ALONE),
      m_locationId (locationId),
      m_ipAddress (ipAddress),
      m_ipAddressForHaInterface ("127.0.0.1"),
      m_port (port),
      m_portForHaPeer (0),
      m_clusterPrimaryLocationId(0),
      m_clusterPrimaryPort(0),
      m_pServerSocketForLocation (NULL),
      m_pServerSocketForHaPeer (NULL)
{
    m_portForHaPeer = FrameworkToolKit::getHaInterfaceReceiverPort();
}

LocationBase::~LocationBase ()
{
}

ServerStreamingSocketStatus LocationBase::initializeServerCommunications ()
{
    ServerStreamingSocketStatus status = SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_BIND;

    if (NULL == m_pServerSocketForLocation)
    {
        m_pServerSocketForLocation = new ServerStreamingSocket (m_port, 1);

        prismAssert (NULL != m_pServerSocketForLocation, __FILE__, __LINE__);

        status = m_pServerSocketForLocation->getStatus ();

        if (SERVER_STREAMING_SOCKET_SUCCESS != status)
        {
            delete m_pServerSocketForLocation;
            m_pServerSocketForLocation = NULL;
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "LocationBase::initializeServerCommunications : Server Communications are already enabled for this location.");
    }

    return (status);
}

bool LocationBase::acceptNewConnection (ServerStreamingSocket &newSocket)
{
    bool successfullyAcceptedNewConnection = false;

    if (NULL != m_pServerSocketForLocation)
    {
        successfullyAcceptedNewConnection = m_pServerSocketForLocation->accept (newSocket);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "LocationBase::acceptNewConnection : Server Communications are not initialized yet.");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (successfullyAcceptedNewConnection);
}

const string &LocationBase::getIpAddress () const
{
    return (m_ipAddress);
}

void LocationBase::resetIpAddress (const string &newIpAddress)
{
    m_ipAddress = newIpAddress;
}

const SI32 &LocationBase::getPort () const
{
    return (m_port);
}

const LocationRole &LocationBase::getRole () const
{
    return (m_locationRole);
}

const UI32 &LocationBase::getLocationId () const
{
    return (m_locationId);
}

const UI32 LocationBase::getSubLocationMask ()
{
    return (0x000000FF);
}

bool LocationBase::isASubLocation (UI32 locationId)
{
    if (0 == (locationId & (getSubLocationMask ())))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool LocationBase::isASubLocation ()
{
    return (isASubLocation (getLocationId ()));
}

ResourceId LocationBase::postToRemoteLocation (const string &bufferToPost, LocationId locationId)
{
    lockAccess ();

    ResourceId failureStatus = ((getClusterPrimaryLocationId() != locationId)? WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION:WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION_DUE_TO_PRINCIPAL_FAILOVER);
    ClientStreamingSocket *pClientStreamingSocket = getClientStreamingSocketForRemoteLocation (locationId);

    if (NULL == pClientStreamingSocket)
    {
        trace (TRACE_LEVEL_DEVEL, string ("LocationBase::postToRemoteLocation : We are not connected to this location to post the buffer: ") + locationId);
        unlockAccess ();
        return (failureStatus);
    }

    UI32   bufferSize   = bufferToPost.size ();
    bool   isSuccessful = false;

    isSuccessful = (*pClientStreamingSocket) << bufferSize;

    if (true == isSuccessful)
    {
        isSuccessful = (*pClientStreamingSocket) << bufferToPost;
    }

    unlockAccess ();
    return (true == isSuccessful ? WAVE_MESSAGE_SUCCESS : failureStatus);
}

ResourceId LocationBase::postToRemoteLocation (PrismMessage *pPrismMessage, LocationId locationId)
{
    lockAccess ();
    ResourceId failureStatus = ((getClusterPrimaryLocationId() != locationId)? WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION:WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION_DUE_TO_PRINCIPAL_FAILOVER);
    ClientStreamingSocket *pClientStreamingSocket = getClientStreamingSocketForRemoteLocation (locationId);
    bool                   isSuccessful           = false;

    if (NULL == pClientStreamingSocket)
    {
        trace (TRACE_LEVEL_DEVEL, string ("LocationBase::postToRemoteLocation : We are not connected to this location to post the message: ") + locationId);
        unlockAccess ();
        return (failureStatus);
    }

    isSuccessful = (*pClientStreamingSocket) << pPrismMessage;

    unlockAccess ();
    return (true == isSuccessful ? WAVE_MESSAGE_SUCCESS : failureStatus);
}

ResourceId LocationBase::postToRemoteLocation (InterLocationMulticastMessage *pInterLocationMulticastMessage, set<LocationId> locationsToSent)
{
    lockAccess (); 

    UI32                        noOfLocationsToSent       = locationsToSent.size ();
    UI32                        numberOfFailures          = 0;
    set<LocationId>::iterator   locationIterator; 
    string                      &serializedStringToBeSent = pInterLocationMulticastMessage->getSerializedStringToSend ();

    pInterLocationMulticastMessage->messageOperationAccess ();

    UI32 dataSize        = serializedStringToBeSent.size ();
    UI32 numberOfBuffers = 0;

    UI32  dataSizeToBeSentOverNetwork = htonl (dataSize);
    UI8  *pNetworkBuffer              = (UI8 *) (&dataSizeToBeSentOverNetwork);

    serializedStringToBeSent.insert (serializedStringToBeSent.begin (), 1, (char) (pNetworkBuffer[3]));
    serializedStringToBeSent.insert (serializedStringToBeSent.begin (), 1, (char) (pNetworkBuffer[2]));
    serializedStringToBeSent.insert (serializedStringToBeSent.begin (), 1, (char) (pNetworkBuffer[1]));
    serializedStringToBeSent.insert (serializedStringToBeSent.begin (), 1, (char) (pNetworkBuffer[0]));

    UI32 numberOfBuffersToBeSentOverNetwork = htonl (numberOfBuffers);

    pNetworkBuffer = (UI8 *) (&numberOfBuffersToBeSentOverNetwork);

    serializedStringToBeSent.insert (serializedStringToBeSent.end (), 1, (char) (pNetworkBuffer[0]));
    serializedStringToBeSent.insert (serializedStringToBeSent.end (), 1, (char) (pNetworkBuffer[1]));
    serializedStringToBeSent.insert (serializedStringToBeSent.end (), 1, (char) (pNetworkBuffer[2]));
    serializedStringToBeSent.insert (serializedStringToBeSent.end (), 1, (char) (pNetworkBuffer[3]));

    for (locationIterator = locationsToSent.begin (); locationIterator != locationsToSent.end (); locationIterator++)
    {
        LocationId locationId = *locationIterator;

        ClientStreamingSocket *pClientStreamingSocket = getClientStreamingSocketForRemoteLocation (locationId);

        ResourceId failureStatus = ((getClusterPrimaryLocationId() != locationId)? WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION : WAVE_MESSAGE_ERROR_POST_TO_REMOTE_LOCATION_DUE_TO_PRINCIPAL_FAILOVER);

        if (NULL == pClientStreamingSocket)
        {  
            pInterLocationMulticastMessage->setStatusForALocation (locationId, failureStatus);
            pInterLocationMulticastMessage->setMessageRepliedToThisLocation (locationId);

            numberOfFailures++;
            continue;
        }

        bool isSuccessful = false;

        isSuccessful = (*pClientStreamingSocket) << serializedStringToBeSent;

        if (false == isSuccessful)
        {
            pInterLocationMulticastMessage->setStatusForALocation (locationId, failureStatus);
            pInterLocationMulticastMessage->setMessageRepliedToThisLocation (locationId);

            numberOfFailures++;
        }
        else
        {
            ++(*pInterLocationMulticastMessage);
        }

#if 0
        isSuccessful = (*pClientStreamingSocket) << dataSize;

        if (true == isSuccessful)
        {
            isSuccessful = (*pClientStreamingSocket) << serializedStringToSent;
            
            if (false == isSuccessful)
            {
                pInterLocationMulticastMessage->setStatusForALocation (locationId, failureStatus);
                pInterLocationMulticastMessage->setMessageRepliedToThisLocation (locationId);

                numberOfFailures++;
            }
            else
            {
                isSuccessful = (*pClientStreamingSocket) << numberOfBuffer;

                if (false == isSuccessful)
                {
                    pInterLocationMulticastMessage->setStatusForALocation (locationId, failureStatus);
                    pInterLocationMulticastMessage->setMessageRepliedToThisLocation (locationId);

                    numberOfFailures++;
                }
                else
                {
                    ++(*pInterLocationMulticastMessage);
                }
            }
        }
        else
        {
            pInterLocationMulticastMessage->setStatusForALocation (locationId, failureStatus);
            pInterLocationMulticastMessage->setMessageRepliedToThisLocation (locationId);

            numberOfFailures++;
        }
#endif
    }

    pInterLocationMulticastMessage->messageOperationReleaseAccess ();

    unlockAccess ();

    return ((noOfLocationsToSent == numberOfFailures) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void LocationBase::lockAccess ()
{
    m_accessMutex.lock ();
}

void LocationBase::unlockAccess ()
{
    m_accessMutex.unlock ();
}

void LocationBase::setLocationId (LocationId locationId)
{
    m_locationId = locationId;
}

void LocationBase::lockConnectionEstablishment ()
{
    m_connectionEstablishmentMutex.lock ();
}

void LocationBase::unlockConnectionEstablishment ()
{
    m_connectionEstablishmentMutex.unlock ();
}

void LocationBase::setClusterPrimaryLocationId (const LocationId &locationId)
{
    m_clusterPrimaryLocationId = locationId;
}

const LocationId LocationBase::getClusterPrimaryLocationId ()
{
    return (m_clusterPrimaryLocationId);
}

void LocationBase::setClusterPrimaryPort (const SI32 &port)
{
    m_clusterPrimaryPort = port;
}

const SI32 LocationBase::getClusterPrimaryPort ()
{
    return (m_clusterPrimaryPort);
}

void LocationBase::setLocationRole (const LocationRole &locatioinRole)
{
    m_locationRole = locatioinRole;
}

const LocationRole LocationBase::getLocationRole ()
{
    return (m_locationRole);
}

void LocationBase::getFullyConnectedLocations   (vector<LocationId> &connectedLocationsVector)
{
    /* We want derived class to implement this method */
    prismAssert(false,__FILE__,__LINE__);
}

ResourceId LocationBase::connectToHaPeer (const string &ipAddress, const SI32 port, UI32 numberOfRetries, UI32 maximumNumberOfSecondsToTryFor)
{
    return (FRAMEWORK_SUCCESS);
}

void LocationBase::invalidateClientStreamingSocketForHaPeer ()
{

}

ServerStreamingSocketStatus LocationBase::initializeHaPeerServerCommunications ()
{
    ServerStreamingSocketStatus status = SERVER_STREAMING_SOCKET_ERROR_COULD_NOT_BIND;

    if (NULL == m_pServerSocketForHaPeer)
    {
        m_pServerSocketForHaPeer = new ServerStreamingSocket (m_portForHaPeer, 1);

        prismAssert (NULL != m_pServerSocketForHaPeer, __FILE__, __LINE__);

        status = m_pServerSocketForHaPeer->getStatus ();

        if (SERVER_STREAMING_SOCKET_SUCCESS != status)
        {
            delete m_pServerSocketForHaPeer;
            m_pServerSocketForHaPeer = NULL;
            trace (TRACE_LEVEL_WARN, string("LocationBase::initializeHaPeerServerCommunications : init HaPeerServer port failed:") + m_portForHaPeer);
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, "LocationBase::initializeHaPeerServerCommunications : Server Communications are already enabled for HA Peer.");
    }

    return (status);
}

bool LocationBase::acceptHaPeerConnection (ServerStreamingSocket &newSocket)
{
    bool successfullyAcceptedNewConnection = false;

    if (NULL != m_pServerSocketForHaPeer)
    {
        successfullyAcceptedNewConnection = m_pServerSocketForHaPeer->accept (newSocket);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "LocationBase::acceptHaPeerConnection : Server Communications are not initialized yet.");
        prismAssert (false, __FILE__, __LINE__);
    }

    return (successfullyAcceptedNewConnection);
}

ResourceId LocationBase::postToHaPeerLocation (const string &bufferToPost)
{
    lockAccess ();

    ClientStreamingSocket *pClientStreamingSocket = getClientStreamingSocketForHaPeer ();

    if (NULL == pClientStreamingSocket)
    {
        trace (TRACE_LEVEL_DEVEL, string ("LocationBase::postToHaPeerLocation : We are not connected to HA Peer to post the buffer."));
        unlockAccess ();
        return (WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE);
    }

    UI32   bufferSize   = bufferToPost.size ();
    bool   isSuccessful = false;

    isSuccessful = (*pClientStreamingSocket) << bufferSize;

    if (true == isSuccessful)
    {
        isSuccessful = (*pClientStreamingSocket) << bufferToPost;
    }

    unlockAccess ();
    return (true == isSuccessful ? WAVE_MESSAGE_SUCCESS : WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE);
}

ResourceId LocationBase::postToHaPeerLocation (PrismMessage *pPrismMessage)
{
    lockAccess ();

    ClientStreamingSocket *pClientStreamingSocket = getClientStreamingSocketForHaPeer ();
    bool                   isSuccessful           = false;

    if (NULL == pClientStreamingSocket)
    {
        trace (TRACE_LEVEL_DEVEL, string ("LocationBase::postToHaPeerLocation : We are not connected to HA Peer to post the buffer."));
        unlockAccess ();
        return (WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE);
    }

    isSuccessful = (*pClientStreamingSocket) << pPrismMessage;

    unlockAccess ();
    return (true == isSuccessful ? WAVE_MESSAGE_SUCCESS : WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE);
}

const SI32 &LocationBase::getHaPeerPort () const
{
    return (m_portForHaPeer);
}

const string &LocationBase::getIpAddressForHaInterface () const
{
    return (m_ipAddressForHaInterface);
}

void LocationBase::resetIpAddressForHaInterface (const string &newIpAddress)
{
    m_ipAddressForHaInterface = newIpAddress;
}

}
