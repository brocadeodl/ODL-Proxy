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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/LocationManagement/Location.h"
#include "Framework/LocationManagement/SubLocation.h"
#include "Framework/LocationManagement/PrismNodeConnectionInformation.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Messaging/Remote/InterLocationMessageReceiverObjectManager.h"
#include <time.h>
#include "Version/PrismVersion.h"

namespace WaveNs
{

Location::Location (UI32 locationId, string &ipAddress, SI32 port)
    : LocationBase (locationId, ipAddress, port)
{
    m_locationRole = LOCATION_STAND_ALONE;
}

Location::~Location ()
{
    map<UI32, SubLocation *>::iterator element         = m_subLocations.begin ();
    map<UI32, SubLocation *>::iterator limitingElement = m_subLocations.end ();

    while (element != limitingElement)
    {
        SubLocation *pSubLocation = element->second;

        if (NULL != pSubLocation)
        {
            delete pSubLocation;
        }

        element++;
    }
}

bool Location::supportsSubLocations ()
{
    return (true);
}

void Location::addSubLocation (LocationId locationId, string &ipAddress, SI32 port)
{
    lockAccess ();

    SubLocation *pSubLocation = new SubLocation (m_locationId, m_ipAddress, m_port, locationId, ipAddress, port);

    if (NULL == pSubLocation)
    {
        trace (TRACE_LEVEL_ERROR, "Location::addSubLocation : Could not create a SubLocation.");
        prismAssert (false, __FILE__, __LINE__);
    }

    map<UI32, SubLocation *>::iterator limitingElement = m_subLocations.end ();
    map<UI32, SubLocation *>::iterator element         = m_subLocations.find (locationId);

    if (element == limitingElement)
    {
        m_subLocations[locationId] = pSubLocation;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Location::addSubLocation : This Sub Location (") + locationId + " )already exists.");
        prismAssert (false, __FILE__, __LINE__);
        delete pSubLocation;
    }

    unlockAccess ();
}

void Location::removeAllSubLocations ()
{
    lockAccess ();

    map<UI32, SubLocation *>::iterator element         = m_subLocations.begin ();
    map<UI32, SubLocation *>::iterator limitingElement = m_subLocations.end ();

    while (element != limitingElement)
    {
        SubLocation *pSubLocation = element->second;

        if (NULL != pSubLocation)
        {
            delete pSubLocation;
        }

        element++;
    }
    m_subLocations.clear ();

    unlockAccess ();
}

void Location::addKnownLocation (LocationId knownLocationId, string &ipAddress, SI32 port)
{
    lockAccess ();

    Location *pLocation = new Location (knownLocationId, ipAddress, port);

    if (NULL == pLocation)
    {
        trace (TRACE_LEVEL_ERROR, "Location::addKnownLocation : Could not create a Location.");
        prismAssert (false, __FILE__, __LINE__);
    }

    map<UI32, Location *>::iterator limitingElement = m_knownLocations.end ();
    map<UI32, Location *>::iterator element         = m_knownLocations.find (knownLocationId);

    if (element == limitingElement)
    {
        m_knownLocations[knownLocationId] = pLocation;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("Location::addKnownLocation : The Location (") + knownLocationId + " ) already exists.");
        prismAssert (false, __FILE__, __LINE__);
        delete pLocation;
    }

    unlockAccess ();
}

void Location::removeKnownLocation (LocationId knownLocationId)
{
    lockAccess ();

    map<UI32, Location *>::iterator limitingElement = m_knownLocations.end ();
    map<UI32, Location *>::iterator element         = m_knownLocations.find (knownLocationId);

    if (element != limitingElement)
    {
        Location *pLocation = m_knownLocations[knownLocationId];

        m_knownLocations.erase (knownLocationId);
        delete pLocation;

        //  Delete the corresponding ClientStreaming Socket as well.

        map<LocationId, ClientStreamingSocket *>::iterator element1 = m_clientSocketsForKnownLocations.find (knownLocationId);
        map<LocationId, ClientStreamingSocket *>::iterator end      = m_clientSocketsForKnownLocations.end ();

        if (end != element1)
        {
            ClientStreamingSocket *pClientStreamingSocket = element1->second;

            m_clientSocketsForKnownLocations.erase (element1);

            if (NULL != pClientStreamingSocket)
            {
                delete pClientStreamingSocket;
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("Location::removeKnownLocation : The Location (") + knownLocationId + " ) is not known to us.");
        //prismAssert (false, __FILE__, __LINE__);
    }

    unlockAccess ();
}

void Location::initializeClientCommunications ()
{
    lockAccess ();

    map<UI32, SubLocation *>::iterator element         = m_subLocations.begin ();
    map<UI32, SubLocation *>::iterator limitingElement = m_subLocations.end ();

    while (element != limitingElement)
    {
        UI32         subLocationId = element->first;
        SubLocation *pSubLocation  = element->second;

        if (NULL != pSubLocation)
        {
            ClientStreamingSocket *pClientSocket;

            trace (TRACE_LEVEL_INFO, string ("Location::initializeClientCommunications : Now Connecting to a Sub Location (") + subLocationId + ").");

            while (true)
            {
                pClientSocket = new ClientStreamingSocket (pSubLocation->getIpAddress (), pSubLocation->getPort (), 1);

                if (NULL == pClientSocket)
                {
                    trace (TRACE_LEVEL_FATAL, "Location::initializeClientCommunications : Could not allocate a Client Socket");
                    prismAssert (false, __FILE__, __LINE__);
                }

                if (false == (pClientSocket->getIsConnected ()))
                {
                    delete pClientSocket;
                    prismSleep (1);
                }
                else
                {
                    break;
                }
            }

            m_clientSocketsForSubLocations[subLocationId] = pClientSocket;

            trace (TRACE_LEVEL_INFO, string ("Location::initializeClientCommunications : Now Connected to a Sub Location (") + subLocationId + ").");

            element++;
        }
    }

    unlockAccess ();
}

ClientStreamingSocket *Location::getClientStreamingSocketForRemoteLocation (LocationId locationId)
{
    if (false == (isASubLocation (locationId)))
    {
        map<UI32, Location *>::iterator element         = m_knownLocations.find (locationId);
        map<UI32, Location *>::iterator limitingElement = m_knownLocations.end ();

        if (element != limitingElement)
        {
            return (m_clientSocketsForKnownLocations[locationId]);
        }
        else
        {
            //trace (TRACE_LEVEL_DEVEL, string ("Location::getClientStreamingSocketForRemoteLocation : Requesting a Client Streaming Socket for an aunknown location : ") + locationId);
        }
    }
    else
    {
        map<UI32, SubLocation *>::iterator element         = m_subLocations.find (locationId);
        map<UI32, SubLocation *>::iterator limitingElement = m_subLocations.end ();

        if (element != limitingElement)
        {
            return (m_clientSocketsForSubLocations[locationId]);
        }
        else
        {
            //trace (TRACE_LEVEL_DEVEL, string ("Location::getClientStreamingSocketForRemoteLocation : Requesting a Client Streaming Socket for an unknown sub location : ") + locationId);
        }
    }

    return (NULL);
}

void Location::invalidateClientStreamingSocketForHaPeer ()
{

    lockAccess ();

    if (m_clientSocketForHaPeer != NULL)
    {
        delete m_clientSocketForHaPeer;
    }

    m_clientSocketForHaPeer = NULL;

    unlockAccess ();
}

void Location::invalidateClientStreamingSocketForRemoteLocation (LocationId locationId)
{
    static UI32 endOfConnectionMarker = 0xFFFFFFFF;

    lockAccess ();

    if (false == (isASubLocation (locationId)))
    {
        map<LocationId, ClientStreamingSocket *>::iterator element = m_clientSocketsForKnownLocations.find (locationId);
        map<LocationId, ClientStreamingSocket *>::iterator end     = m_clientSocketsForKnownLocations.end ();

        if (element != end)
        {
            if (NULL != (m_clientSocketsForKnownLocations[locationId]))
            {
                *(m_clientSocketsForKnownLocations[locationId]) << endOfConnectionMarker;
                delete (m_clientSocketsForKnownLocations[locationId]);
                m_clientSocketsForKnownLocations[locationId] = NULL;
            }
        }
        else
        {
            trace (TRACE_LEVEL_WARN, string ("Location::invalidateClientStreamingSocketForRemoteLocation : Trying to invalidate a client socket connection for an unknown (") + locationId + ") Location.");
        }
    }
    else
    {
        map<LocationId, ClientStreamingSocket *>::iterator element = m_clientSocketsForSubLocations.find (locationId);
        map<LocationId, ClientStreamingSocket *>::iterator end     = m_clientSocketsForSubLocations.end ();

        if (element != end)
        {
            if (NULL != (m_clientSocketsForSubLocations[locationId]))
            {
                *(m_clientSocketsForSubLocations[locationId]) << endOfConnectionMarker;
                delete (m_clientSocketsForSubLocations[locationId]);
                m_clientSocketsForSubLocations[locationId] = NULL;
            }
        }
        else
        {
            trace (TRACE_LEVEL_WARN, string ("Location::invalidateClientStreamingSocketForRemoteLocation : Trying to invalidate a client socket connection for an unknown (") + locationId + ") Sub Location.");
        }
    }

    unlockAccess ();
}

ResourceId Location::connectToRemoteLocation (LocationId locationId, UI32 numberOfRetries, UI32 maximumNumberOfSecondsToTryFor)
{
    lockConnectionEstablishment ();
    lockAccess ();

    LocationBase *pLocationBase = NULL;
    ResourceId    status        = FRAMEWORK_TIME_OUT;

    if (false == (isASubLocation (locationId)))
    {
        map<UI32, Location *>::iterator element = m_knownLocations.find (locationId);
        map<UI32, Location *>::iterator end     = m_knownLocations.end ();

        if (end != element)
        {
            pLocationBase = element->second;
        }
    }
    else
    {
        map<UI32, SubLocation *>::iterator element = m_subLocations.find (locationId);
        map<UI32, SubLocation *>::iterator end     = m_subLocations.end ();

        if (end != element)
        {
            pLocationBase = element->second;
        }
    }


    if (NULL == pLocationBase)
    {
        status = FRAMEWORK_UNKNOWN_LOCATION;

        trace (TRACE_LEVEL_WARN, string ("Location::connectToRemoteLocation : We do not know about the location : ") + locationId);
    }
    else
    {
        ClientStreamingSocket *pClientSocket = getClientStreamingSocketForRemoteLocation (locationId);

        if (NULL == pClientSocket)
        {
            trace (TRACE_LEVEL_INFO, string ("Location::connectToRemoteLocation : Now Connecting to a Sub Location/SubLocation (") + locationId + ").");

            UI32    numberOfTimesWeTriedToConnect = 0;
            UI32    numberOfSecondsElapsed        = 0;
            timeval startTime;
            timeval currentTime;

            if (0 != (gettimeofday (&startTime, NULL)))
            {
                trace (TRACE_LEVEL_FATAL, string ("Location::connectToRemoteLocation : Could not obtain start time."));
                prismAssert (false, __FILE__, __LINE__);
                status = FRAMEWORK_ERROR;
            }
            else
            {
                // During the period which we try to connect to a location we release the lock.
                // Connecting to a location can potentially take long time.  Especially in case of an invalid IpAddress
                // we essentially time out.  Since the same locks are used to be able to use remote messaging, we must
                // release the lock as soon as possible.

                unlockAccess ();

                while (true)
                {
                    lockAccess ();

                    if (!(((0 == numberOfRetries) || (numberOfTimesWeTriedToConnect < numberOfRetries)) && ((0 == maximumNumberOfSecondsToTryFor) || (numberOfSecondsElapsed <= maximumNumberOfSecondsToTryFor))))
                    {
                        break;
                    }

                    if (false == (isASubLocation (locationId)))
                    {
                        map<UI32, Location *>::iterator tempElement1 = m_knownLocations.find (locationId);
                        map<UI32, Location *>::iterator tempEnd1     = m_knownLocations.end ();

                        if (tempEnd1 == tempElement1)
                        {
                            status = FRAMEWORK_UNKNOWN_LOCATION;

                            break;
                        }

                        map<LocationId, ClientStreamingSocket *>::iterator tempElement = m_clientSocketsForKnownLocations.find (locationId);
                        map<LocationId, ClientStreamingSocket *>::iterator tempEnd     = m_clientSocketsForKnownLocations.end ();

                        if ((tempElement != tempEnd) && (NULL != tempElement->second))
                        {
                            pClientSocket = tempElement->second;
                            status = FRAMEWORK_SUCCESS;

                            break;
                        }
                    }
                    else
                    {
                        map<UI32, SubLocation *>::iterator tempElement1 = m_subLocations.find (locationId);
                        map<UI32, SubLocation *>::iterator tempEnd1     = m_subLocations.end ();

                        if (tempEnd1 == tempElement1)
                        {
                            status = FRAMEWORK_UNKNOWN_LOCATION;

                            break;
                        }

                        map<LocationId, ClientStreamingSocket *>::iterator tempElement = m_clientSocketsForSubLocations.find (locationId);
                        map<LocationId, ClientStreamingSocket *>::iterator tempEnd     = m_clientSocketsForSubLocations.end ();

                        if ((tempElement != tempEnd) && (NULL != tempElement->second))
                        {
                            pClientSocket = tempElement->second;
                            status = FRAMEWORK_SUCCESS;

                            break;
                        }
                    }

                    pClientSocket = new ClientStreamingSocket (pLocationBase->getIpAddress (), pLocationBase->getPort (), 1);

                    if (NULL == pClientSocket)
                    {
                        trace (TRACE_LEVEL_FATAL, string ("Location::connectToRemoteLocation : Could not allocate a Client Socket for Location/SubLocation : ") + locationId);
                        prismAssert (false, __FILE__, __LINE__);
                        break;
                    }

                    if (false == (pClientSocket->getIsConnected ()))
                    {
                        delete pClientSocket;
                        unlockAccess ();
                        prismSleep (1);
                    }
                    else
                    {
                        // If we are able to connect post them the header.
                        // the header currently contains The pass code for authorization, ipaddress and
                        // the server port for this location so that the remote locations knows how to contact us back.

                        bool isSuccessful = false;

                        isSuccessful = (*pClientSocket) << FrameworkToolKit::getPrismConnectionPassPhrase ();

                        if (true != isSuccessful)
                        {
                            prismAssert (false, __FILE__, __LINE__);
                        }

                        // now create the Prism Node Connection Information object and post it to the remote location.

                        PrismNodeConnectionInformation thisServerConnectionInformation (m_ipAddress, m_port, locationId, PrismVersion::getVersionString ());

                        //isSuccessful = (*pClientSocket) << (&thisServerConnectionInformation);

                        string serializedData; 
                        thisServerConnectionInformation.serialize2 (serializedData, SERIALIZE_WITH_ATTRIBUTE_NAME);
                        UI32 size = serializedData.size(); 
                        isSuccessful = (*pClientSocket) << (size); 
                        
                        if (true == isSuccessful)    
                        {
                             isSuccessful = (*pClientSocket) << (serializedData); 
                        }

                        if (true != isSuccessful)
                        {
                            prismAssert (false, __FILE__, __LINE__);
                        }

                        status = FRAMEWORK_SUCCESS;
                        break;
                    }

                    numberOfTimesWeTriedToConnect++;

                    if (0 != (gettimeofday (&currentTime, NULL)))
                    {
                        trace (TRACE_LEVEL_FATAL, string ("Location::connectToRemoteLocation : Could not obtain current time."));
                        prismAssert (false, __FILE__, __LINE__);
                        status = FRAMEWORK_ERROR;
                        break;
                    }

                    numberOfSecondsElapsed = (UI32) (currentTime.tv_sec - startTime.tv_sec);
                }
            }

            if (FRAMEWORK_SUCCESS == status)
            {
                if (false == (isASubLocation (locationId)))
                {
                    m_clientSocketsForKnownLocations[locationId] = pClientSocket;
                }
                else
                {
                    m_clientSocketsForSubLocations[locationId] = pClientSocket;
                }

                string toMsgVersion         = FrameworkToolKit::getMessageVersionForRemoteNode      (pLocationBase->getIpAddress (), pLocationBase->getPort ());
                UI8    toSerializationType  = FrameworkToolKit::getSerializationTypeForRemoteNode   (pLocationBase->getIpAddress (), pLocationBase->getPort ());

                if (SERIALIZE_WITH_UNKNOWN != toSerializationType)  
                {
                    pClientSocket->setToMessageVersionAndSerializationType (toMsgVersion, toSerializationType); 
                }
                else
                {
                    trace (TRACE_LEVEL_WARN, string("Location::connectToRemoteLocation : could not find messageVersion for node with ip:port - ") + pLocationBase->getIpAddress () + string (":") + pLocationBase->getPort ());
                }

                trace (TRACE_LEVEL_INFO, string ("Location::connectToRemoteLocation : Now Connected to a Location/SubLocation (") + locationId + ").");
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("Location::connectToRemoteLocation : We could not Connect to a Location/SubLocation (") + locationId + ").  All retries exhausted.");
            }
        }
        else
        {
            trace (TRACE_LEVEL_WARN, string ("Location::connectToRemoteLocation : We are already connected to Location/SubLocation : ") + locationId);

            string toMsgVersion         = FrameworkToolKit::getMessageVersionForRemoteNode      (pLocationBase->getIpAddress (), pLocationBase->getPort ()); 
            UI8    toSerializationType  = FrameworkToolKit::getSerializationTypeForRemoteNode   (pLocationBase->getIpAddress (), pLocationBase->getPort ());

            if (SERIALIZE_WITH_UNKNOWN != toSerializationType)   
            {
                pClientSocket->setToMessageVersionAndSerializationType (toMsgVersion, toSerializationType); 
            }
            else
            {
                trace (TRACE_LEVEL_WARN, string("Location::connectToRemoteLocation : could not find messageVersion for node with ip:port - ") + pLocationBase->getIpAddress () + string (":") + pLocationBase->getPort ());      
             }

            status = FRAMEWORK_SUCCESS;
        }
    }

    unlockAccess ();
    unlockConnectionEstablishment ();

    return (status);
}

LocationId Location::getLocationIdForIpAddressAndPort (const string &ipAddress, const SI32 &port)
{
    lockAccess ();

    map<UI32, SubLocation *>::iterator element         = m_subLocations.begin ();
    map<UI32, SubLocation *>::iterator limitingElement = m_subLocations.end ();
    LocationId                         subLocationId   = 0;
    bool                               found           = false;

    while (element != limitingElement)
    {
        SubLocation *pSubLocation  = element->second;

        if (NULL != pSubLocation)
        {
            if ((ipAddress == (pSubLocation->getIpAddress ())) && (port == (pSubLocation->getPort ())))
            {
                subLocationId = element->first;
                found         = true;
                break;
            }
        }

        element++;
    }

    unlockAccess ();

    if (true == found)
    {
        return (subLocationId);
    }

    lockAccess ();

    map<UI32, Location *>::iterator element1         = m_knownLocations.begin ();
    map<UI32, Location *>::iterator limitingElement1 = m_knownLocations.end ();
    LocationId                      locationId       = 0;

    while (element1 != limitingElement1)
    {
        Location *pLocation  = element1->second;

        if (NULL != pLocation)
        {
            if ((ipAddress == (pLocation->getIpAddress ())) && (port == (pLocation->getPort ())))
            {
                locationId = element1->first;
                found      = true;
                break;
            }
        }

        element1++;
    }

    unlockAccess ();

    if (true == found)
    {
        return (locationId);
    }
    else
    {
        // FIXME : sagar replace 0 with NullLocationId;
        return (0);
    }
}

string Location::getIpAddressForLocationId (const LocationId &locationId)
{
    lockAccess ();

    LocationBase *pLocationBase = NULL;
    string        ipAddress     = "";

    if (locationId == m_locationId)
    {
        ipAddress = m_ipAddress;
    }
    else
    {
        if (false == (isASubLocation (locationId)))
        {
            map<UI32, Location *>::iterator element = m_knownLocations.find (locationId);
            map<UI32, Location *>::iterator end     = m_knownLocations.end ();

            if (end != element)
            {
                pLocationBase = element->second;
            }
        }
        else
        {
            map<UI32, SubLocation *>::iterator element = m_subLocations.find (locationId);
            map<UI32, SubLocation *>::iterator end     = m_subLocations.end ();

            if (end != element)
            {
                pLocationBase = element->second;
            }
        }

        if (NULL != pLocationBase)
        {
            ipAddress = pLocationBase->getIpAddress ();
        }
        else
        {
            ipAddress == "";
        }
    }

    unlockAccess ();

    return (ipAddress);
}

SI32 Location::getPortForLocationId (const LocationId &locationId)
{
    lockAccess ();

    LocationBase *pLocationBase = NULL;
    SI32          port          = 0;

    if (locationId == m_locationId)
    {
        port = m_port;
    }
    else
    {
        if (false == (isASubLocation (locationId)))
        {
            map<UI32, Location *>::iterator element = m_knownLocations.find (locationId);
            map<UI32, Location *>::iterator end     = m_knownLocations.end ();

            if (end != element)
            {
                pLocationBase = element->second;
            }
        }
        else
        {
            map<UI32, SubLocation *>::iterator element = m_subLocations.find (locationId);
            map<UI32, SubLocation *>::iterator end     = m_subLocations.end ();

            if (end != element)
            {
                pLocationBase = element->second;
            }
        }

        if (NULL != pLocationBase)
        {
            port = pLocationBase->getPort ();
        }
        else
        {
            port = 0;
        }
    }

    unlockAccess ();

    return (port);
}

void Location::getKnownRemoteLocations (vector<LocationId> &remoteLocationsVector)
{
    lockAccess ();

    map<UI32, SubLocation *>::iterator element         = m_subLocations.begin ();
    map<UI32, SubLocation *>::iterator limitingElement = m_subLocations.end ();

    while (element != limitingElement)
    {
        SubLocation *pSubLocation  = element->second;

        if (NULL != pSubLocation)
        {
            remoteLocationsVector.push_back (pSubLocation->getLocationId ());
        }

        element++;
    }

    unlockAccess ();

    lockAccess ();

    map<UI32, Location *>::iterator element1         = m_knownLocations.begin ();
    map<UI32, Location *>::iterator limitingElement1 = m_knownLocations.end ();

    while (element1 != limitingElement1)
    {
        Location *pLocation  = element1->second;

        if (NULL != pLocation)
        {
            remoteLocationsVector.push_back (pLocation->getLocationId ());
        }

        element1++;
    }

    unlockAccess ();
}

void Location::getKnownLocations (vector<LocationId> &knownLocationsVector)
{
    lockAccess ();

    map<UI32, Location *>::iterator element1         = m_knownLocations.begin ();
    map<UI32, Location *>::iterator limitingElement1 = m_knownLocations.end ();

    while (element1 != limitingElement1)
    {
        Location *pLocation  = element1->second;

        if (NULL != pLocation)
        {
            knownLocationsVector.push_back (pLocation->getLocationId ());
        }

        element1++;
    }

    unlockAccess ();
}

void Location::getConnectedLocations (vector<LocationId> &connectedLocationsVector)
{
    lockAccess ();

    map<UI32, Location *>::iterator element1         = m_knownLocations.begin ();
    map<UI32, Location *>::iterator limitingElement1 = m_knownLocations.end ();
    LocationId                      locationId;

    while (element1 != limitingElement1)
    {
        Location *pLocation  = element1->second;

        if (NULL != pLocation)
        {
            locationId = pLocation->getLocationId ();

            if ( NULL != (m_clientSocketsForKnownLocations[locationId]) )
            {
                connectedLocationsVector.push_back (locationId);
            }
        }

        element1++;
    }

    unlockAccess ();
}

void Location::getKnownSubLocations (vector<LocationId> &knownSubLocationsVector)
{
    lockAccess ();

    map<UI32, SubLocation *>::iterator element         = m_subLocations.begin ();
    map<UI32, SubLocation *>::iterator limitingElement = m_subLocations.end ();

    while (element != limitingElement)
    {
        SubLocation *pSubLocation  = element->second;

        if (NULL != pSubLocation)
        {
            knownSubLocationsVector.push_back (pSubLocation->getLocationId ());
        }

        element++;
    }

    unlockAccess ();
}

bool Location::isAKnownLocation (const LocationId &locationId)
{
    lockAccess ();

    bool isKnown = false;

    map<UI32, Location *>::iterator limitingElement = m_knownLocations.end ();
    map<UI32, Location *>::iterator element         = m_knownLocations.find (locationId);

    if (limitingElement != element)
    {
        isKnown = true;
    }
    else
    {
        isKnown = false;
    }

    unlockAccess ();

    return (isKnown);
}

bool Location::isAConnectedLocation (const LocationId &locationId)
{
    lockAccess ();

    bool isConnected = false;

    map<UI32, Location *>::iterator limitingElement = m_knownLocations.end ();
    map<UI32, Location *>::iterator element         = m_knownLocations.find (locationId);

    if (limitingElement != element)
    {
        if (NULL != (m_clientSocketsForKnownLocations[locationId]))
        {
            isConnected = true;
        }
        else
        {
            isConnected = false;
        }
    }
    else
    {
        isConnected = false;
    }

    unlockAccess ();

    return (isConnected);
}

void Location::resetLocationToStandAlone (const LocationId &locationId)
{
    setLocationId               (locationId);
    setClusterPrimaryLocationId (locationId);
    setClusterPrimaryPort       (m_port);
    setLocationRole             (LOCATION_STAND_ALONE);
}

void Location::resetLocationToPrimary ()
{
    setClusterPrimaryLocationId (m_locationId);
    setClusterPrimaryPort       (m_port);
    setLocationRole             (LOCATION_PRIMARY);
}


/**
 * getFullyConnectedLocations
 * Instead of connected known locations, this methods fetches locations that have two way connections.
 * Nodes which are in cluster will have two connectection established.
 */

void Location::getFullyConnectedLocations (vector<LocationId> &connectedLocationsVector)
{
    trace(TRACE_LEVEL_DEBUG,"Location::getFullyConnectedLocations: Entering ... ") ;
    lockAccess ();

    map<UI32, Location *>::iterator element1         = m_knownLocations.begin ();
    map<UI32, Location *>::iterator limitingElement1 = m_knownLocations.end ();
    LocationId                      locationId;
    vector<LocationId>              tmpConnectedLocationIds;
    vector<string>                  ipAddresses;
    vector<SI32>                    ports;            

    while (element1 != limitingElement1)
    {
        Location *pLocation  = element1->second;

        if (NULL != pLocation)
        {
            locationId = pLocation->getLocationId ();

            if ( NULL != (m_clientSocketsForKnownLocations[locationId]))
            {
                tracePrintf(TRACE_LEVEL_DEBUG,"Location::getFullyConnectedLocations: location in cluster %d",locationId);
                tmpConnectedLocationIds.push_back (locationId);
                ipAddresses.push_back(pLocation->getIpAddress());
                ports.push_back (pLocation->getPort());        
            }
        }

        element1++;
    }
    unlockAccess ();

    // check if location is currently connected.

    UI32 numOfLocations = tmpConnectedLocationIds.size ();
    for (UI32 pos = 0; pos < numOfLocations; pos++)
    {
        if (true == InterLocationMessageReceiverObjectManager::isACurrentlyConnectedLocation (ipAddresses [pos], ports [pos]))
        {
            connectedLocationsVector.push_back(tmpConnectedLocationIds [pos]);
        }
    }
}

ResourceId Location::connectToHaPeer (const string &ipAddress, const SI32 port, UI32 numberOfRetries, UI32 maximumNumberOfSecondsToTryFor)
{
    lockConnectionEstablishment ();
    lockAccess ();

    ClientStreamingSocket   *pClientSocket  = NULL;
    ResourceId              status          = FRAMEWORK_TIME_OUT;

    pClientSocket = getClientStreamingSocketForHaPeer ();

    if (NULL == pClientSocket)
    {

            trace (TRACE_LEVEL_INFO, string ("Location::connectToHaPeer : Now Connecting to Ha Peer"));

            UI32    numberOfTimesWeTriedToConnect = 0;
            UI32    numberOfSecondsElapsed        = 0;
            timeval startTime;
            timeval currentTime;

            if (0 != (gettimeofday (&startTime, NULL)))
            {
                trace (TRACE_LEVEL_FATAL, string ("Location::connectToRemoteLocation : Could not obtain start time."));
                prismAssert (false, __FILE__, __LINE__);
                status = FRAMEWORK_ERROR;
            }
            else
            {
                // During the period which we try to connect to a location we release the lock.
                // Connecting to a location can potentially take long time.  Especially in case of an invalid IpAddress
                // we essentially time out.  Since the same locks are used to be able to use remote messaging, we must
                // release the lock as soon as possible.

                unlockAccess ();

                while (true)
                {
                    lockAccess ();

                    if (!(((0 == numberOfRetries) || (numberOfTimesWeTriedToConnect < numberOfRetries)) && ((0 == maximumNumberOfSecondsToTryFor) || (numberOfSecondsElapsed <= maximumNumberOfSecondsToTryFor))))
                    {
                        break;
                    }

                    // TBD - use well-known internal IP address of other CP
                    pClientSocket = new ClientStreamingSocket (ipAddress, port, 1);

                    if (NULL == pClientSocket)
                    {
                        trace (TRACE_LEVEL_FATAL, string ("Location::connectToHaPeer : Could not allocate a Client Socket "));
                        prismAssert (false, __FILE__, __LINE__);
                        break;
                    }

                    if (false == (pClientSocket->getIsConnected ()))
                    {
                        delete pClientSocket;
                        unlockAccess ();
                        prismSleep (1);
                    }
                    else
                    {
                        // If we are able to connect post them the header.
                        // the header currently contains The pass code for authorization, ipaddress and
                        // the server port for this location so that the remote locations knows how to contact us back.

                        bool isSuccessful = false;

                        isSuccessful = (*pClientSocket) << FrameworkToolKit::getPrismHaConnectionPassPhrase ();

                        if (true != isSuccessful)
                        {
                            prismAssert (false, __FILE__, __LINE__);
                        }

                        // now create the Prism Node Connection Information object and post it to the remote location.

                        // TBD - get local slot IP address for HA Peer.
                        PrismHaNodeConnectionInformation thisServerConnectionInformation (m_ipAddressForHaInterface, m_portForHaPeer, 0, PrismVersion::getVersionString());
                        //PrismNodeConnectionInformation thisServerConnectionInformation (m_ipAddress, m_portForHaPeer);

                        //isSuccessful = (*pClientSocket) << (&thisServerConnectionInformation);

                        //Currently, PrismNodeConnectionInformation must be sent in pre-leo format only. This can be removed in Leo++ as it will not need to support Hydra builds.

                        string serializedData;
                        //thisServerConnectionInformation.serialize2 (serializedData, "0.0.0"); //PrismVersion::getVersionString(), true);
                        thisServerConnectionInformation.serialize2 (serializedData, SERIALIZE_WITH_ATTRIBUTE_ORDER); 

                        UI32 size = serializedData.size();
                        isSuccessful = (*pClientSocket) << (size);
                        if (true == isSuccessful)
                        {
                            isSuccessful = (*pClientSocket) << (serializedData);
                        }
                        if (true != isSuccessful)
                        {
                            prismAssert (false, __FILE__, __LINE__);
                        }

                        status = FRAMEWORK_SUCCESS;
                        break;
                    }

                    numberOfTimesWeTriedToConnect++;

                    if (0 != (gettimeofday (&currentTime, NULL)))
                    {
                        trace (TRACE_LEVEL_FATAL, string ("Location::connectToHaPeer : Could not obtain current time."));
                        prismAssert (false, __FILE__, __LINE__);
                        status = FRAMEWORK_ERROR;
                        break;
                    }

                    numberOfSecondsElapsed = (UI32) (currentTime.tv_sec - startTime.tv_sec);
                }
            }

            if (FRAMEWORK_SUCCESS == status)
            {
                m_clientSocketForHaPeer = pClientSocket;

                string toMsgVersion         = FrameworkToolKit::getMessageVersionForHaPeer (ipAddress, port);
                UI8    toSerializationType  = FrameworkToolKit::getSerializationTypeForHaPeer (ipAddress, port);

                if (SERIALIZE_WITH_UNKNOWN != toSerializationType)
                {
                    m_clientSocketForHaPeer->setToMessageVersionAndSerializationType (toMsgVersion, toSerializationType);
                }
                else
                {
                    trace (TRACE_LEVEL_WARN, string("Location::connectToHaPeer : could not find messageVersion for HaPeer with ip:port - ") + ipAddress + string (":") + port);
                } 

                trace (TRACE_LEVEL_INFO, string ("Location::connectToHaPeer : Now Connected to Ha Peer"));
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("Location::connectToHaPeer : We could not Connect to Ha Peer.  All retries exhausted."));
            }
    }
    else
    {
            string toMsgVersion         = FrameworkToolKit::getMessageVersionForHaPeer (ipAddress, port);
            UI8    toSerializationType  = FrameworkToolKit::getSerializationTypeForHaPeer (ipAddress, port);

            if (SERIALIZE_WITH_UNKNOWN != toSerializationType)
            {
                pClientSocket->setToMessageVersionAndSerializationType (toMsgVersion, toSerializationType);
            }
            else
            {
                trace (TRACE_LEVEL_WARN, string("Location::connectToHaPeer : could not find messageVersion for HaPeer with ip:port - ") + ipAddress + string (":") + port);
            }

            trace (TRACE_LEVEL_WARN, "Location::connectToHaPeer : We are already connected to Ha peer");
            status = FRAMEWORK_SUCCESS;
    }

    unlockAccess ();
    unlockConnectionEstablishment ();

    return (status);
}

void Location::removeAllKnownLocations ()
{
    lockAccess ();

    m_knownLocations.clear ();

    unlockAccess ();
}

ClientStreamingSocket *Location::getClientStreamingSocketForHaPeer ()
{
    return m_clientSocketForHaPeer;
}

}
