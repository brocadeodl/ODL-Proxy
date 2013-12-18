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
 *   Author : Bhavin                                                     *
 **************************************************************************/
#ifndef SSMDELETEASSOCIATIONWITHDISCONNECTEDMESSAGE_H
#define SSMDELETEASSOCIATIONWITHDISCONNECTEDMESSAGE_H

//#include "Framework/Types/Types.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Messaging/Local/WaveMessagePayloadBase.h"
#include "Ssm/Local/SsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include <string>
#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class SsmDeleteAssociationwithDisconnectedMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmDeleteAssociationwithDisconnectedMessage ();
            SsmDeleteAssociationwithDisconnectedMessage (const string &policyName);
            virtual    ~SsmDeleteAssociationwithDisconnectedMessage ();
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
            void setPolicyId(const ObjectId &policyId);
            ObjectId getPolicyId() const;
			void setAclType (const UI32 &aclType);
			UI32 getAclType () const;

// Now the data members

        private:
        protected:
        public:
		    string   m_policyName;
            ObjectId m_policyId;
            UI32 m_aclType;
    };
}
#endif

