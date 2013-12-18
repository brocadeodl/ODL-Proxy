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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Aashish Akhouri                                              *
 ***************************************************************************/

#ifndef WAVENEWNODESADDEDEVENT_H
#define WAVENEWNODESADDEDEVENT_H
#include "Framework/Messaging/Local/PrismEvent.h"

namespace WaveNs
{

class WaveNewNodesAddedEvent : public PrismEvent
{
    private :

    protected :
        virtual void setupAttributesForSerialization ();

    public :
                 WaveNewNodesAddedEvent ();
        virtual ~WaveNewNodesAddedEvent ();

                void        addNewNodeDetails           (const LocationId &locationId, const string &ipAddress, const UI32 &port);
                UI32        getNumberOfNewLocations     () const;
                LocationId  getLocationIdAtIndex        (const UI32 &index) const;
                string      getIpAddressAtIndex         (const UI32 &index) const;
                UI32        getPortAtIndex              (const UI32 &index) const;

    // Now the data members

    private :

        vector<LocationId> m_locationIdsForNewNodes;
        vector<string>     m_IpAddressesForNewNodes;
        vector<UI32>       m_portsForNewNodes;

    protected :
    public :
};

}

#endif // WAVENEWNODESADDEDEVENT_H
