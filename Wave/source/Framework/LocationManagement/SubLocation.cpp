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
#include "Framework/LocationManagement/SubLocation.h"
#include "Framework/LocationManagement/Location.h"
#include "Framework/LocationManagement/PrismNodeConnectionInformation.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include <time.h>

namespace WaveNs
{

SubLocation::SubLocation (UI32 parentLocationId, string &parentIpAddress, SI32 parentPort, UI32 locationId, string &ipAddress, SI32 port)
    : LocationBase (locationId, ipAddress, port),
      m_pClientSocketForParentLocation (NULL)
{
    m_locationRole = LOCATION_SUB_LOCATION;

    if (parentLocationId != (locationId & (~(getSubLocationMask ()))))
    {
        // Trace facilities might not have started at this time.  So use cout.

        cout << "The parent LocationId is not in sync with this LocationId" << endl;

        prismAssert (false, __FILE__, __LINE__);
    }

    m_pParentLocation = new Location (parentLocationId, parentIpAddress, parentPort);
}

SubLocation::~SubLocation ()
{
    if (NULL != m_pParentLocation)
    {
        delete m_pParentLocation;
    }

    if (NULL != m_pClientSocketForParentLocation)
    {
        delete m_pClientSocketForParentLocation;
    }
}

void SubLocation::setPParentLocation (Location *pParentLocation)
{
    m_pParentLocation = pParentLocation;
}

bool SubLocation::supportsSubLocations ()
{
    return (false);
}

void SubLocation::addSubLocation (LocationId locationId, string &ipAddress, SI32 port)
{
    trace (TRACE_LEVEL_FATAL, "SubLocation::addSubLocation : Sub locations do not support adding Sub Locations to them.");
    prismAssert (false, __FILE__, __LINE__);
}

void SubLocation::initializeClientCommunications ()
{
    lockAccess ();

    if (NULL == m_pParentLocation)
    {
        trace (TRACE_LEVEL_FATAL, "SubLocation::initializeClientCommunications : The Parent Location for this Location is not setup.");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (NULL != m_pClientSocketForParentLocation)
    {
        trace (TRACE_LEVEL_FATAL, "SubLocation::initializeClientCommunications : This location is already connected to the Parent Location");
        prismAssert (false, __FILE__, __LINE__);
    }

    // Now connect to the Parent Location.  We cannot proceed without connecting.

    ClientStreamingSocket *pClientSocket = NULL;

    trace (TRACE_LEVEL_INFO, string ("SubLocation::initializeClientCommunications : Now Trying To Connect To Parent Location (") + m_pParentLocation->getLocationId () + ").");

    while (true)
    {
        m_pClientSocketForParentLocation = pClientSocket = new ClientStreamingSocket (m_pParentLocation->getIpAddress (), m_pParentLocation->getPort (), 1);

        if (NULL == pClientSocket)
        {
            trace (TRACE_LEVEL_FATAL, string ("SubLocation::initializeClientCommunications : Could not allocate a Client Socket for Location : ") + m_pParentLocation->getLocationId ());
            prismAssert (false, __FILE__, __LINE__);
        }

        if (false == (pClientSocket->getIsConnected ()))
        {
            delete pClientSocket;
            m_pClientSocketForParentLocation = pClientSocket = NULL;
            prismSleep (1);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("SubLocation::initializeClientCommunications : Now Connected To Parent Location (") + m_pParentLocation->getLocationId () + ").");
            break;
        }
    }

    unlockAccess ();
}

UI32 SubLocation::getParentLocationId () const
{
    return (m_pParentLocation->getLocationId ());
}

ClientStreamingSocket *SubLocation::getClientStreamingSocketForRemoteLocation (LocationId locationId)
{
    if ((getParentLocationId ()) == locationId)
    {
        if (NULL == m_pClientSocketForParentLocation)
        {
            trace (TRACE_LEVEL_WARN, "SubLocation::getClientStreamingSocketForRemoteLocation : Someone is requesting for a client socket for parent location before we connected to the parent location.");
        }

        return (m_pClientSocketForParentLocation);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("SubLocation::getClientStreamingSocketForRemoteLocation : We can only communicate to the Parent Location.  LocationId (") + locationId + ")other than parent location detected.");
        prismAssert (false, __FILE__, __LINE__);
        return (NULL);
    }
}

void SubLocation::invalidateClientStreamingSocketForRemoteLocation (LocationId locationId)
{
    static UI32 endOfConnectionMarker = 0xFFFFFFFF;

    lockAccess ();

    if ((getParentLocationId ()) == locationId)
    {
        if (NULL == m_pClientSocketForParentLocation)
        {
            trace (TRACE_LEVEL_WARN, "SubLocation::invalidateClientStreamingSocketForRemoteLocation : Someone is requesting for a client socket for parent location before we connected to the parent location.");
        }
        else
        {
            *m_pClientSocketForParentLocation << endOfConnectionMarker;
            delete m_pClientSocketForParentLocation;
            m_pClientSocketForParentLocation = NULL;
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("SubLocation::invalidateClientStreamingSocketForRemoteLocation : We can only communicate to the Parent Location.  LocationId (") + locationId + ")other than parent location detected.");
        prismAssert (false, __FILE__, __LINE__);
    }

    unlockAccess ();
}

ResourceId SubLocation::connectToRemoteLocation (LocationId locationId, UI32 numberOfRetries, UI32 maximumNumberOfSecondsToTryFor)
{
    lockConnectionEstablishment ();
    lockAccess ();

    ResourceId status = FRAMEWORK_TIME_OUT;

    if ((getParentLocationId ()) == locationId)
    {
        if (NULL == m_pClientSocketForParentLocation)
        {
            ClientStreamingSocket *pClientSocket = NULL;

            trace (TRACE_LEVEL_INFO, string ("SubLocation::connectToRemoteLocation : Now Trying To Connect To Parent Location (") + m_pParentLocation->getLocationId () + ").");

            UI32    numberOfTimesWeTriedToConnect = 0;
            UI32    numberOfSecondsElapsed        = 0;
            timeval startTime;
            timeval currentTime;

            if (0 != (gettimeofday (&startTime, NULL)))
            {
                trace (TRACE_LEVEL_FATAL, string ("SubLocation::connectToRemoteLocation : Could not obtain start time."));
                prismAssert (false, __FILE__, __LINE__);
                status = FRAMEWORK_ERROR;
            }
            else
            {
                // During the period which we try to connect to a location we release the lock.Location.Location.
                // Connecting to a location can potentially take long time.  Especially in case of an invalid IpAddress
                // we essentially time out.  Since the same locks are uused to be able to use remote messaging, we must
                // release the lock as soon as possible.

                unlockAccess ();

                while (((0 == numberOfRetries) || (numberOfTimesWeTriedToConnect < numberOfRetries)) && ((0 == maximumNumberOfSecondsToTryFor) || (numberOfSecondsElapsed >= maximumNumberOfSecondsToTryFor)))
                {
                    m_pClientSocketForParentLocation = pClientSocket = new ClientStreamingSocket (m_pParentLocation->getIpAddress (), m_pParentLocation->getPort (), 1);

                    if (NULL == pClientSocket)
                    {
                        trace (TRACE_LEVEL_FATAL, "SubLocation::connectToRemoteLocation : Could not allocate a Client Socket.");
                        prismAssert (false, __FILE__, __LINE__);
                    }

                    if (false == (pClientSocket->getIsConnected ()))
                    {
                        delete pClientSocket;
                        m_pClientSocketForParentLocation = pClientSocket = NULL;
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

                        PrismNodeConnectionInformation thisServerConnectionInformation (m_ipAddress, m_port);

                        isSuccessful = (*pClientSocket) << (&thisServerConnectionInformation);

                        if (true != isSuccessful)
                        {
                            prismAssert (false, __FILE__, __LINE__);
                        }

                        status = FRAMEWORK_SUCCESS;
                        trace (TRACE_LEVEL_INFO, string ("SubLocation::connectToRemoteLocation : Now Connected To Parent Location (") + m_pParentLocation->getLocationId () + ").");
                        break;
                    }

                    numberOfTimesWeTriedToConnect++;

                    if (0 != (gettimeofday (&currentTime, NULL)))
                    {
                        trace (TRACE_LEVEL_FATAL, string ("SubLocation::connectToRemoteLocation : Could not obtain current time."));
                        prismAssert (false, __FILE__, __LINE__);
                        status = FRAMEWORK_ERROR;
                        break;
                    }

                    numberOfSecondsElapsed = (UI32) (currentTime.tv_sec - startTime.tv_sec);
                }

                lockAccess ();
            }

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, string ("SubLocation::connectToRemoteLocation : Could not Connect To Parent Location (") + m_pParentLocation->getLocationId () + ").  All retries exhausted.");
            }
        }
        else
        {
            trace (TRACE_LEVEL_WARN, "SubLocation::connectToRemoteLocation : We are already connected to the parent location.");
            status = FRAMEWORK_SUCCESS;
        }
    }
    else
    {
        status = FRAMEWORK_UNKNOWN_LOCATION;

        trace (TRACE_LEVEL_FATAL, "SubLocation::connectToRemoteLocation : We can only communicate to the Parent Location.  LocationId other than parent location detected.");
        prismAssert (false, __FILE__, __LINE__);
    }

    unlockAccess ();
    unlockConnectionEstablishment ();

    return (status);
}

LocationId SubLocation::getLocationIdForIpAddressAndPort (const string &ipAddress, const SI32 &port)
{
    lockAccess ();

    LocationId locationId = 0;

    if ((ipAddress == (m_pParentLocation->getIpAddress ())) && (port == (m_pParentLocation->getPort ())))
    {
        locationId = m_pParentLocation->getLocationId ();
    }
    else
    {
        locationId = 0;
    }

    unlockAccess ();

    return (locationId);
}

string SubLocation::getIpAddressForLocationId (const LocationId &locationId)
{
    lockAccess ();

    string ipAddress = "";

    if (locationId == m_locationId)
    {
        ipAddress = m_ipAddress;
    }
    else if (locationId == (m_pParentLocation->getLocationId ()))
    {
        ipAddress = m_pParentLocation->getIpAddress ();
    }
    else
    {
        ipAddress = "";
    }

    unlockAccess ();

    return (ipAddress);
}

SI32 SubLocation::getPortForLocationId (const LocationId &locationId)
{
    lockAccess ();

    SI32 port = 0;

    if (locationId == m_locationId)
    {
        port = m_port;
    }
    else if (locationId == (m_pParentLocation->getLocationId ()))
    {
        port = m_pParentLocation->getPort ();
    }
    else
    {
        port = 0;
    }

    unlockAccess ();

    return (port);
}

void SubLocation::addKnownLocation (LocationId knownLocationId, string &ipAddress, SI32 port)
{
    trace (TRACE_LEVEL_FATAL, "SubLocation::addKnownLocation : Sub locations do not support adding Locations to them.");
    prismAssert (false, __FILE__, __LINE__);
}

void SubLocation::removeKnownLocation (LocationId knownLocationId)
{
    trace (TRACE_LEVEL_FATAL, "SubLocation::removeKnownLocation : Sub locations do not support removing locations since it does not support adding them in the first place.");
    prismAssert (false, __FILE__, __LINE__);
}

void SubLocation::getKnownRemoteLocations (vector<LocationId> &remoteLocationsVector)
{
    lockAccess ();

    LocationId parentLocationId = getParentLocationId ();

    if (0 != parentLocationId)
    {
        remoteLocationsVector.push_back (parentLocationId);
    }

    unlockAccess ();
}

void SubLocation::getKnownLocations (vector<LocationId> &knownLocationsVector)
{
    lockAccess ();

    LocationId parentLocationId = getParentLocationId ();

    if (0 != parentLocationId)
    {
        knownLocationsVector.push_back (parentLocationId);
    }

    unlockAccess ();
}

void SubLocation::getConnectedLocations (vector<LocationId> &connectedLocationsVector)
{
    lockAccess ();

    LocationId parentLocationId = getParentLocationId ();

    if (0 != parentLocationId)
    {
        if (NULL != m_pClientSocketForParentLocation)
        {
            connectedLocationsVector.push_back (parentLocationId);
        }
    }

    unlockAccess ();
}

void SubLocation::getKnownSubLocations (vector<LocationId> &remoteLocationsVector)
{
    lockAccess ();
    // There is nothing to be done here.  Sub locations do not own sub locations.
    unlockAccess ();
}

bool SubLocation::isAKnownLocation (const LocationId &locationId)
{
    lockAccess ();

    bool isKnown = false;

    LocationId parentLocationId = getParentLocationId ();

    if (0 != parentLocationId)
    {
        if (locationId == parentLocationId)
        {
            isKnown = true;
        }
        else
        {
            isKnown = false;
        }
    }
    else
    {
        isKnown = false;
    }

    unlockAccess ();

    return (isKnown);
}

bool SubLocation::isAConnectedLocation (const LocationId &locationId)
{
    lockAccess ();

    bool isConnected = false;

    LocationId parentLocationId = getParentLocationId ();

    if (0 != parentLocationId)
    {
        if (locationId == parentLocationId)
        {
            if (NULL != m_pClientSocketForParentLocation)
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
    }
    else
    {
        isConnected = false;
    }

    unlockAccess ();

    return (isConnected);
}

void SubLocation::resetLocationToStandAlone (const LocationId &locationId)
{
    trace (TRACE_LEVEL_FATAL, "SubLocation::resetLocationToStandAlone : Sub locations cannot be reset to stand alone at this time.");
    prismAssert (false, __FILE__, __LINE__);
}

void SubLocation::resetLocationToPrimary ()
{
    trace (TRACE_LEVEL_FATAL, "SubLocation::resetLocationToPrimary : Sub locations cannot be reset to Primary at this time.");
    prismAssert (false, __FILE__, __LINE__);
}

void SubLocation::removeAllKnownLocations ()
{
    trace (TRACE_LEVEL_FATAL, "SubLocation::removeAllKnownLocations : Sub locations do not support removing locations since it does not support adding them in the first place.");

    prismAssert (false, __FILE__, __LINE__);
}

void SubLocation::removeAllSubLocations ()
{
    trace (TRACE_LEVEL_FATAL, "SubLocation::removeAllSubLocations : Sub locations do not support removing sub locations.");

    prismAssert (false, __FILE__, __LINE__);
}

ClientStreamingSocket* SubLocation::getClientStreamingSocketForHaPeer ()
{
    return NULL;
}

}
