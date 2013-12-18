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

#ifndef  SSMIPACLRESEQMESSAGE_H
#define  SSMIPACLRESEQMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmIpAclReseqMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmIpAclReseqMessage ();
            SsmIpAclReseqMessage (const string &policyName,const UI64 &seqId, const UI64 &incSeqId);
            virtual    ~SsmIpAclReseqMessage ();

            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
			void setSeqId(const  UI64   &SeqId);
		    UI64	 getSeqId() const;
			void  setIncSeqId(const  UI64 &IncSeqId);
			UI64  getIncSeqId() const;

        private:
        protected:
        public:
            string   m_policyName;
            UI64     m_seqId;
            UI64     m_incSeqId;
    };
}
#endif                                            
