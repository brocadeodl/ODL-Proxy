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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef NASSERVERIPVLANMANAGEDOBJECT_H
#define NASSERVERIPVLANMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/Attributes.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class NasServerIpVlanManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            NasServerIpVlanManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager);
            NasServerIpVlanManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager, const UI32 vlanNumber);
            virtual    ~NasServerIpVlanManagedObject ();
			virtual void  setupKeys ();
            static string getClassName();
			void setVlanNumber (const UI32 &vlanNumber);
			UI32 getVlanNumber ();


// Now the data members

        private:
        protected:
        public:
			UI32 m_vlanNumber;
    };
}
#endif                                            //NASSERVERIPVLANMANAGEDOBJECT_H
