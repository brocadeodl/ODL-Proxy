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

#ifndef NSMGLOBALUPDATEVLANCLASSIFIERGROUPMESSAGE_H
#define NSMGLOBALUPDATEVLANCLASSIFIERGROUPMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalUpdateVlanClassifierGroupMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalUpdateVlanClassifierGroupMessage ();
            NsmGlobalUpdateVlanClassifierGroupMessage (const UI32 &groupId,const UI32 &operation,const UI32 &ruleId,const UI32 &dummy);
            virtual    ~NsmGlobalUpdateVlanClassifierGroupMessage ();
            void setGroupId(const UI32 &groupId);
            UI32 getGroupId()  const;
            void setOperation(const UI32 &operation);
            UI32 getOperation()  const;
            void setRuleId(const UI32 &ruleId);
            UI32 getRuleId()  const;
            void setdummy(const UI32 &dummy);
            UI32 getdummy()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_groupId;
            UI32  m_operation;
            UI32  m_ruleId;
            UI32  m_dummy;
    };
}
#endif                                            //NSMGLOBALUPDATEVLANCLASSIFIERGROUPMESSAGE_H
