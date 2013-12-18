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
 *   Author : pasu                                                     *
 **************************************************************************/

#ifndef SSMGLOBALASSOCRULETOPOLICYMESSAGE_H
#define SSMGLOBALASSOCRULETOPOLICYMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Ssm/Global/SsmAssocRuleToPolicyMsgPayload.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalAssocRuleToPolicyMessage : public ManagementInterfaceMessage, public SsmAssocRuleToPolicyMsgPayload 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmGlobalAssocRuleToPolicyMessage ();
            SsmGlobalAssocRuleToPolicyMessage (const string &policyName, const UI32 &policyType, 
				const UI64 &ruleid,const UI32 &action, const string &srcMac, const string &srcMacMask, 
				const AddrType &srcType, const string &dstMac, const string &dstMacMask, const AddrType &dstType,
				const string &appType, const EtherType &appTypeEnum, const UI16 &appTypeInt, 
				const bool &isCount, const bool &isLog, const UI32 &opcode, const UI8 &userEnteredSeqid);
			
			SsmGlobalAssocRuleToPolicyMessage (const string &policyName, const UI32 &policyType, 
				const UI64 &ruleid, const UI32 &action, const string &srcMac, const bool &srcMask, 
				const string &dstMac, const bool &dstMask, const string &appType, const bool &isCount, 
				const UI32 &opcode);
			virtual    ~SsmGlobalAssocRuleToPolicyMessage ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            
