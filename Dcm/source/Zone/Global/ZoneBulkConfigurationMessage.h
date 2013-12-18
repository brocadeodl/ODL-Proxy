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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef ZONEBULKCONFIGURATIONMESSAGE_H
#define ZONEBULKCONFIGURATIONMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class ZoneBulkConfigurationMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                                         ZoneBulkConfigurationMessage ();
        virtual                         ~ZoneBulkConfigurationMessage ();

                void                     addCfg                       (const string &cfg,  const vector<string> cfgMembers);
                void                     addZone                      (const string &zone, const vector<string> zoneMembers);

                vector<string>          &getCfgs                      ();
                vector<vector<string> > &getCfgMembers                ();
                vector<string>          &getZones                     ();
                vector<vector<string> > &getZoneMembers               ();

    // Now the Data Members

    private :
        vector<string>          m_cfgs;
        vector<vector<string> > m_cfgMembers;
        vector<string>          m_zones;
        vector<vector<string> > m_zoneMembers;
        vector<string>          m_cfgActions;

    protected :
    public :
};

}

#endif // ZONEBULKCONFIGURATIONMESSAGE_H
