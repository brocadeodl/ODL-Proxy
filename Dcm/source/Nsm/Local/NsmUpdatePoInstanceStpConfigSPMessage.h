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
 *   Author : sudheend                                                     *
 **************************************************************************/

#ifndef NSMUPDATEPOINSTANCESTPCONFIGSPMESSAGE_H
#define NSMUPDATEPOINSTANCESTPCONFIGSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Mstp/Global/MstpGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmGlobalUpdatePoInstanceStpConfigMessage;

    class NsmUpdatePoInstanceStpConfigSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmUpdatePoInstanceStpConfigSPMessage ();
            NsmUpdatePoInstanceStpConfigSPMessage(NsmGlobalUpdatePoInstanceStpConfigMessage &msg);
            NsmUpdatePoInstanceStpConfigSPMessage (const string &ifName,const UI8 &instanceId,const UI32 &priority,const UI32 &cost,const UI32 &cmdCode);
            virtual    ~NsmUpdatePoInstanceStpConfigSPMessage ();

            void setIfName(const string &ifName);
            string getIfName()  const;
            void setInstanceId(const UI8 &instanceId);
            UI8 getInstanceId()  const;
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setCost(const UI32 &cost);
            UI32 getCost()  const;
            void setRestrictedRole(const bool &restrictedRole);
            bool getRestrictedRole()  const;
            void setRestrictedTcn(const bool &restrictedTcn);
            bool getRestrictedTcn()  const;
            void setGuard(const MstpPortGuardRootEnum &guard);
            MstpPortGuardRootEnum getGuard()  const;
 
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

	    void printMsg();
// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32  m_ifIndex;
            UI8  m_instanceId;
            UI32  m_priority;
            UI32  m_cost;
            bool  m_restrictedRole;
            bool  m_restrictedTcn;
            MstpPortGuardRootEnum  m_guard;
            UI32  m_cmdCode;
    };
}
#endif                                            //NSMUPDATEPOINSTANCESTPCONFIGSPMESSAGE_H
