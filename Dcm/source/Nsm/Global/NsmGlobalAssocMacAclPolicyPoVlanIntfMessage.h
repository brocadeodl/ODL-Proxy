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
 *   Author : pasu														   *
 **************************************************************************/

#ifndef NSMGLOBALASSOCMACACLPOLICYPOVLANINTFMESSAGE_H
#define NSMGLOBALASSOCMACACLPOLICYPOVLANINTFMESSAGE_H 

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ssmAcl_dcmd_public.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalAssocMacAclPolicyPoVlanIntfMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalAssocMacAclPolicyPoVlanIntfMessage ();
            NsmGlobalAssocMacAclPolicyPoVlanIntfMessage (const UI32 &ifId, const UI8 &ifType, const string &aclName, const UI32 &cmdCode, const ssmAcl_binding_direction &direction);
            virtual    ~NsmGlobalAssocMacAclPolicyPoVlanIntfMessage ();
            void setIfId(const UI32 &ifId);
            UI32 getIfId()  const;
            void setIfType(const UI8 &ifType);
            UI8 getIfType()  const;
            void setAclName(const string &aclName);
            string getAclName()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setDirection(ssmAcl_binding_direction &direction);
            ssmAcl_binding_direction getDirection () const;

// Now the data members

        private:
        protected:
        public:
            UI32                        m_ifId;
            UI8                         m_ifType;
            string                      m_aclName;
            UI32                        m_cmdCode;
            ssmAcl_binding_direction    m_direction;
    };
}
#endif                                            //NSMGLOBALASSOCMACACLPOLICYPOVLANINTFMESSAGE_H
