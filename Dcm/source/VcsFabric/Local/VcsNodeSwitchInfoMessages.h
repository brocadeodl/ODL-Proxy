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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#ifndef VCSNODESWITCHINFOMESSAGES_H
#define VCSNODESWITCHINFOMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeUpdateSwitchInfoMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeUpdateSwitchInfoMessage ();
                        VcsNodeUpdateSwitchInfoMessage (const string &chassisString, const MacAddress &switchMac, const SI32 domainId, const WorldWideName &switchWWN, const SI32 SWBDModel, const string &fwVersion, const string &swmodelName);
        virtual        ~VcsNodeUpdateSwitchInfoMessage ();

        string          getChassisString               ();
        SI32            getDomainId                    ();
        MacAddress      getSwitchMac                   ();
        WorldWideName   getSwitchWWN                   ();
        SI32            getSWBDModel                   ();
        string          getFwVersion                   ();
        string          getSwmodelName                 () const;

    // Now the data members

    private :
        string          m_chassisString;
        MacAddress      m_switchMac;
        SI32            m_domainId;
        WorldWideName   m_switchWWN;
        SI32            m_SWBDModel;
        string          m_fwVersion;        
        string          m_swmodelName;

    protected :
    public :
};
}

#endif // VCSNODESWITCHINFOMESSAGES_H

