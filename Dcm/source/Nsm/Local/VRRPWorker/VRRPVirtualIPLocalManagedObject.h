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
 *   Author : krao                                                     *
 **************************************************************************/

#ifndef VRRPVIRTUALIPLOCALMANAGEDOBJECT_H
#define VRRPVIRTUALIPLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class VRRPVirtualIPLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            VRRPVirtualIPLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            VRRPVirtualIPLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const IpV4Address &virtual_ip);
            virtual    ~VRRPVirtualIPLocalManagedObject ();
            static string getClassName();
            void setVirtual_ip(const IpV4Address &virtual_ip);
            IpV4Address getVirtual_ip()  const;

// Now the data members

        private:
        protected:
        public:
            IpV4Address  m_virtual_ip;
    };
}
#endif                                            //VRRPVIRTUALIPLOCALMANAGEDOBJECT_H
