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
 *   Author : debojyoti                                                     *
 **************************************************************************/

#ifndef PBRNEXTHOPADDRMANAGEDOBJECT_H
#define PBRNEXTHOPADDRMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Framework/Attributes/AttributeStringVectorUC.h"
#include "Framework/Attributes/AttributeUI32VectorUC.h"
#include "Framework/Attributes/AttributeIpV4AddressUC.h"
#include "Framework/Attributes/AttributeBoolUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpPolicyLocalObjectManager;
    class IpPolicyLocalWorker;

// ************* PbrNexthopAddrManagedObject ************
    class PbrNexthopAddrManagedObject : virtual public DcmLocalManagedObject
    {
        private:
            friend class IpPolicyLocalWorker;
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();

        public:
            PbrNexthopAddrManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager);
            virtual    ~PbrNexthopAddrManagedObject ();
            static string getClassName();

            void setSet_next_hop_address(const IpV4Address &set_next_hop_address);
            void unsetSet_next_hop_address();
            IpV4Address getSet_next_hop_address()  const;
            void setIndex(UI64 index);
            UI64 getIndex();

// Now the data members

        private:
            IpV4Address  m_set_next_hop_address;
			UI64    m_index;
    };
}
#endif
