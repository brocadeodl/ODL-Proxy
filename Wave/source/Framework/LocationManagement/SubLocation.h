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

#ifndef SUBLOCATION_H
#define SUBLOCATION_H

#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/Utils/ClientStreamingSocket.h"

namespace WaveNs
{

class Location;

class SubLocation : public LocationBase
{
    private :
        virtual void getKnownRemoteLocations (vector<LocationId> &remoteLocationsVector);
        virtual void getKnownLocations       (vector<LocationId> &knownLocationsVector);
        virtual void getConnectedLocations   (vector<LocationId> &connectedLocationsVector);
        virtual void getKnownSubLocations    (vector<LocationId> &knownSubLocationsVector);

    protected :
    public :
                                       SubLocation                                      (UI32 parentLocationId, string &parentIpAddress, SI32 parentPort, UI32 locationId, string &ipAddress, SI32 port);
        virtual                       ~SubLocation                                      ();
                void                   setPParentLocation                               (Location *pParentLocation);
        virtual bool                   supportsSubLocations                             ();
        virtual void                   addSubLocation                                   (LocationId locationId, string &ipAddress, SI32 port);
        virtual void                   initializeClientCommunications                   ();
        virtual ClientStreamingSocket *getClientStreamingSocketForRemoteLocation        (LocationId locationId);
        virtual ClientStreamingSocket* getClientStreamingSocketForHaPeer                ();
        virtual void                   invalidateClientStreamingSocketForRemoteLocation (LocationId locationId);
        virtual ResourceId             connectToRemoteLocation                          (LocationId locationId, UI32 numberOfRetries = 0, UI32 maximumNumberOfSecondsToTryFor = 0);
        virtual LocationId             getLocationIdForIpAddressAndPort                 (const string &ipAddress, const SI32 &port);
        virtual string                 getIpAddressForLocationId                        (const LocationId &locationId);
        virtual SI32                   getPortForLocationId                             (const LocationId &locationId);
        virtual void                   addKnownLocation                                 (LocationId knownLocationId, string &ipAddress, SI32 port);
        virtual void                   removeKnownLocation                              (LocationId knownLocationId);

                UI32                   getParentLocationId                              () const;
                bool                   isAKnownLocation                                 (const LocationId &locationId);
                bool                   isAConnectedLocation                             (const LocationId &locationId);

        virtual void                   resetLocationToStandAlone                        (const LocationId &locationId);
        virtual void                   resetLocationToPrimary                           ();
        virtual void                   removeAllKnownLocations                          ();
        virtual void                   removeAllSubLocations                            ();

    // Now the data members

    private :
        Location              *m_pParentLocation;
        ClientStreamingSocket *m_pClientSocketForParentLocation;

    protected :
    public :
};

}

#endif // SUBLOCATION_H
