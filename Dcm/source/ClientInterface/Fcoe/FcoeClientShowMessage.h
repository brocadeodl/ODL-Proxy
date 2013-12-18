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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vikas B Dharwadkar                                           *
 **************************************************************************/

#ifndef FCOECLIENTSHOWMESSAGE_H
#define FCOECLIENTSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeClientShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeClientShowMessage ();
            FcoeClientShowMessage (const UI32 &cmdCode,
                                   const UI32 &vlan,
                                   const UI32 &vfid,
                                   const UI32 &fc_uport,
                                   const string &fab_map_name,
                                   const string &map_name,
                                   const string &phy_intf_name,
                                   const bool &stats_enable);

            virtual    ~FcoeClientShowMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

            UI32 getVlan() const;
            void setVlan(const UI32 &vlan);
            UI32 getVfid() const;
            void setVfid(const UI32 &vfid);
            UI32 getFcUport() const;
            void setFcUport(const UI32 &fc_uport);
            string getFabMapName() const;
            void setFabMapName(const string &fab_map_name);
            string getMapName() const;
            void setMapName(const string &map_name);
            string getPhyIfName() const;
            void setPhyIfName(const string &if_name);
            bool getStatsEnable() const;
            void setStatsEnable(const bool &stats_enable);

            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);

            void getOutputBufferRef (UI32   bufferNum, 
                                    UI32 &size, void *&pBuffer);
// Now the data members

        private:
        protected:
        public:
            UI32    m_cmdCode;
            UI32    m_vlan;
            UI32    m_vfid;
            UI32    m_fc_uport;
            string  m_fab_map_name;
            string  m_map_name;
            string  m_phy_intf_name;
            bool    m_stats_enable;
    };
}
#endif                                            //FCOECLIENTSHOWMESSAGE_H
