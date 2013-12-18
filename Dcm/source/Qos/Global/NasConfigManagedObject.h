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

#ifndef NASCONFIGMANAGEDOBJECT_H
#define NASCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/Attributes.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class NasConfigManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            NasConfigManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager);
            NasConfigManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager, const UI32 cos, const UI32 dscp);
            virtual    ~NasConfigManagedObject ();
            static string getClassName();
			void setCos (const UI32 &cos);
			UI32 getCos();
			void setDscp(const UI32 &dscp);
			UI32 getDscp();


// Now the data members

        private:
        protected:
        public:
			bool m_autoNasPresenceFlag;
			UI32 m_cos;
			UI32 m_dscp;
    };
}
#endif                                            //NASCONFIGMANAGEDOBJECT_H
