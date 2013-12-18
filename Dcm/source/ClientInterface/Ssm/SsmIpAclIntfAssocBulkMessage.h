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

#ifndef SSMIPACLINTFASSOCBULKMESSAGE_H
#define SSMIPACLINTFASSOCBULKMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmIpAclIntfAssocBulkMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmIpAclIntfAssocBulkMessage ();
            virtual    ~SsmIpAclIntfAssocBulkMessage ();

            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);
            vector<string> getPolicyNameVector() const;
            vector<UI32>  getOpCodeVector() const;
            vector<string> getIfNameVector() const;
            void setNumRecords(const UI64 &num);
            UI64 getNumRecords() const;
            void setIntfDetails(const string &ifname, const string &policy, const UI32 
				&opcode, const UI32 &direction);



        private:
        protected:
        public:
			UI64			m_numRecords;
			vector<string>  m_policyName;
			vector<string>  m_ifname;
			vector<UI32>	m_opcode;
			vector<UI32>	m_direction;
    };
}
#endif  //SSMIPACLINTFASSOCBULKMESSAGE_H                                          
