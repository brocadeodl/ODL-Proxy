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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef FCOEFABRICMAPMANAGEDOBJECT_H
#define FCOEFABRICMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeGlobalObjectManager;
    class FcoeFabricMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FcoeFabricMapManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager);
            FcoeFabricMapManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager,const string &fabric_map_name,const UI32 &vlan,const SI32 &priority,const string &fcmap,const SI32 &vfid,const SI32 &fka_intvl,const bool &timeout,const SI32 &mtu, const UI32 &max_enodes, const string &enodes_cfg_mode);
            virtual    ~FcoeFabricMapManagedObject ();
            static string getClassName();
            void setName(const string &fabric_map_name);
            string getName()  const;
            void setVlan(const UI32 &vlan);
            UI32 getVlan()  const;
            void setPriority(const SI32 &priority);
            SI32 getPriority()  const;
            void setFcmap(const string &fcmap);
            string getFcmap()  const;
            void setVfid(const SI32 &vfid);
            SI32 getVfid()  const;
            void setFka_intvl(const SI32 &fka_intvl);
            SI32 getFka_intvl()  const;
            void setTimeout(const bool &timeout);
            bool getTimeout()  const;
            void setMtu(const SI32 &mtu);
            SI32 getMtu() const;
            void setMaxEnodes(const UI32 &max_enodes);
            UI32 getMaxEnodes() const;
            void setEnodesCfgMode(const string &enodes_cfg_mode);
            string getEnodesCfgMode() const;

// Now the data members

        private:
        protected:
        public:
            string  m_fabric_map_name;
            UI32  m_vlan;
            SI32  m_priority;
            string  m_fcmap;
            SI32  m_vfid;
            SI32  m_fka_intvl;
            bool  m_timeout;
            SI32  m_mtu;
            UI32  m_max_enodes;
            string  m_enodes_cfg_mode;
            
    };
}
#endif                                            //FCOEFABRICMAPMANAGEDOBJECT_H
