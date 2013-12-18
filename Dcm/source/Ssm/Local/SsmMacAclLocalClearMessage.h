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
 *   Author : Bhavin                                                  *
 **************************************************************************/

#ifndef SSMMACACLLOCALCLEARMESSAGE_H
#define SSMMACACLLOCALCLEARMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmMacAclLocalClearMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmMacAclLocalClearMessage ();
            SsmMacAclLocalClearMessage (const string &l2aclPolicyName, const string &ifName, 
				const UI32 &cmdCode, bool isGwRequest, const UI32 & type);
			virtual   	~SsmMacAclLocalClearMessage ();
            
            void setL2aclPolicyName (const string &l2aclPolicyName);
            string getL2aclPolicyName ()  const;

            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode ()  const;

            void setIsGwRequest (bool isGwRequest);
            bool getIsGwRequest ()  const;            

   		    void  setIfName(const string &ifName);
		    string  getIfName() const;

    		UI32  getIfType() const;
    		void setIfType(const UI32 &type);
 
// Now the data members

        private:
        protected:
        public:
            string	m_l2aclPolicyName;
			string	m_if_name;
			UI32    m_cmdCode;
			bool    m_isGwRequest;
			UI32    m_ifType;
    };
}
#endif                                            //SSMMACACLLOCALCLEARMESSAGE_H
