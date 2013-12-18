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

#ifndef NSMCLIENTDELETEVLANCLASSIFIERRULESPMESSAGE_H
#define NSMCLIENTDELETEVLANCLASSIFIERRULESPMESSAGE_H

#include "ClientInterface/Nsm/NsmVlanProvisionMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmClientDeleteVlanClassifierRuleSPMessage : public NsmVlanProvisionMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmClientDeleteVlanClassifierRuleSPMessage ();
            NsmClientDeleteVlanClassifierRuleSPMessage (const UI32 &ruleId);
            virtual    ~NsmClientDeleteVlanClassifierRuleSPMessage ();
            void setRuleId(const UI32 &ruleId);
            UI32 getRuleId()  const;
	    
	    virtual const void *getCStructureForInputs    ();

// Now the data members

        private:
        protected:
        public:
            UI32  m_ruleId;
    };
}
#endif                                            //NSMCLIENTDELETEVLANCLASSIFIERRULESPMESSAGE_H
