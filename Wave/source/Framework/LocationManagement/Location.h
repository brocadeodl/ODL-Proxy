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

#ifndef LOCATION_H
#define LOCATION_H

#include <map>
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/Utils/ClientStreamingSocket.h"

using namespace std;

namespace WaveNs
{

class SubLocation;

class Location : public LocationBase
{
    private :
        virtual void getKnownRemoteLocations (vector<LocationId> &remoteLocationsVector);
        virtual void getKnownLocations       (vector<LocationId> &knownLocationsVector);
        virtual void getConnectedLocations   (vector<LocationId> &connectedLocationsVector);
        virtual void getKnownSubLocations    (vector<LocationId> &knownSubLocationsVector);

    protected :
    public :
                                       Location                                         (LocationId locationId, string &ipAddress, SI32 port);
        virtual                       ~Location                                         ();
        virtual bool                   supportsSubLocations                             ();
        virtual void                   addSubLocation                                   (LocationId locationId, string &ipAddress, SI32 port);
        virtual void                   addKnownLocation                                 (LocationId knownLocationId, string &ipAddress, SI32 port);
        virtual void                   removeKnownLocation                              (LocationId knownLocationId);
        virtual void                   initializeClientCommunications                   ();
        virtual ClientStreamingSocket *getClientStreamingSocketForRemoteLocation        (LocationId locationId);
        virtual ClientStreamingSocket *getClientStreamingSocketForHaPeer                ();
        virtual void                   invalidateClientStreamingSocketForRemoteLocation (LocationId locationId);
        virtual void                   invalidateClientStreamingSocketForHaPeer         ();
        virtual ResourceId             connectToRemoteLocation                          (LocationId locationId, UI32 numberOfRetries = 0, UI32 maximumNumberOfSecondsToTryFor = 0);
        virtual ResourceId             connectToHaPeer                                  (const string &ipAddress, const SI32 port, UI32 numberOfRetries, UI32 maximumNumberOfSecondsToTryFor);
        virtual LocationId             getLocationIdForIpAddressAndPort                 (const string &ipAddress, const SI32 &port);
        virtual string                 getIpAddressForLocationId                        (const LocationId &locationId);
        virtual SI32                   getPortForLocationId                             (const LocationId &locationId);
        virtual bool                   isAKnownLocation                                 (const LocationId &locationId);
        virtual bool                   isAConnectedLocation                             (const LocationId &locationId);
        virtual void                   resetLocationToStandAlone                        (const LocationId &locationId);
        virtual void                   resetLocationToPrimary                           ();
        virtual void                   getFullyConnectedLocations                       (vector<LocationId> &connectedLocationsVector);
        virtual void                   removeAllKnownLocations                          ();
                void                   removeAllSubLocations                            ();

    // Now the data members

    private :
        // Datastructures to hold the sub locations

        map<LocationId, SubLocation *>           m_subLocations;
        map<LocationId, ClientStreamingSocket *> m_clientSocketsForSubLocations;

        // Datastructures to hold the other kown locations.

        map<LocationId, Location*>               m_knownLocations;
        map<LocationId, ClientStreamingSocket *> m_clientSocketsForKnownLocations;

        ClientStreamingSocket *                  m_clientSocketForHaPeer;

    protected :
    public :
};

}

#endif // LOCATION_H
