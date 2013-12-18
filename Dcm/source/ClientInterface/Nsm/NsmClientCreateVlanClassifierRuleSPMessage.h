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
 *   Author : rmuthigi                                                     *
 **************************************************************************/

#ifndef NSMCLIENTCREATEVLANCLASSIFIERRULESPMESSAGE_H
#define NSMCLIENTCREATEVLANCLASSIFIERRULESPMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmClientCreateVlanClassifierRuleSPMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmClientCreateVlanClassifierRuleSPMessage ();
            NsmClientCreateVlanClassifierRuleSPMessage (const UI32 &ruleId,const string &mac,const UI32 &protoVal,const UI32 &encap,const UI32 &opcode);
            virtual    ~NsmClientCreateVlanClassifierRuleSPMessage ();
            void setRuleId(const UI32 &ruleId);
            UI32 getRuleId()  const;
            void setMac(const string &mac);
            string getMac()  const;
            void setProtoVal(const UI32 &protoVal);
            UI32 getProtoVal()  const;
            void setEncap(const UI32 &encap);
            UI32 getEncap()  const;
 	    void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;	
	    
	    virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            UI32  m_ruleId;
            string  m_mac;
            UI32  m_protoVal;
            UI32  m_encap;
	    UI32  m_opcode;
    };
}
#endif                                            //NSMCLIENTCREATEVLANCLASSIFIERRULESPMESSAGE_H
