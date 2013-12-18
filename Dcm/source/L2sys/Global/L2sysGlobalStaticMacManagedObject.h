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
 *   Author : aantony                                                     *
 **************************************************************************/

#ifndef LSYSGLOBALSTATICMACMANAGEDOBJECT_H
#define LSYSGLOBALSTATICMACMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveLocalManagedObjectBase.h"

using namespace WaveNs;

namespace DcmNs
{

    class L2sysGlobalObjectManager;
    class L2sysGlobalStaticMacManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            L2sysGlobalStaticMacManagedObject (L2sysGlobalObjectManager *pL2sysGlobalObjectManager);
            L2sysGlobalStaticMacManagedObject (L2sysGlobalObjectManager *pL2sysGlobalObjectManager,const string &Mac,const UI32 &forward,const UI32 &interface_type,const string &ifname,const UI32 &vlan,const UI32 &vlanid,const UI32 &slotid);
            virtual    ~L2sysGlobalStaticMacManagedObject ();
            static string getClassName();
            void setMac(const string &Mac);
            string getMac()  const;
            void setForward(const UI32 &forward);
            UI32 getForward()  const;
            void setInterface_type(const UI32 &interface_type);
            UI32 getInterface_type()  const;
            void setIfname(const string &ifname);
            string getIfname()  const;
            void setVlan(const UI32 &vlan);
            UI32 getVlan()  const;
            void setVlanid(const UI32 &vlanid);
            UI32 getVlanid()  const;
            UI32 getSlotid()  const;
            void setSlotid(const UI32 &slotid);

// Now the data members

        private:
        protected:
        public:
            string  m_Mac;
            UI32  m_forward;
            UI32  m_interface_type;
            string  m_ifname;
            UI32  m_vlan;
            UI32  m_vlanid;
            UI32  m_slotid;
    };
}
#endif                                            //LSYSGLOBALSTATICMACMANAGEDOBJECT_H
