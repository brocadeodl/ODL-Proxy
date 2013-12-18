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
 *   Author : nsong                                                     *
 **************************************************************************/

#ifndef NSMUPDATEPHYINSTANCESTPCONFIGMESSAGE_H
#define NSMUPDATEPHYINSTANCESTPCONFIGMESSAGE_H

#include "Nsm/Common/NsmPortStpConfigMessage.h"
#include "Mstp/Global/MstpGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmUpdatePhyInstanceStpConfigMessage : public NsmPortStpConfigMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();

        public:
            NsmUpdatePhyInstanceStpConfigMessage ();
            NsmUpdatePhyInstanceStpConfigMessage (const InterfaceType &ifType, const string &ifName,const UI8 &instanceid,const UI32 &priority,const UI32 &cost,const UI32 &cmdCode);
            virtual    ~NsmUpdatePhyInstanceStpConfigMessage ();
            void setInstanceId(const UI8 &instanceid);
            UI8 getInstanceId()  const;
            void setRestrictedRole(const bool &restrictedRole);
            bool getRestrictedRole()  const;
            void setRestrictedTcn(const bool &restrictedTcn);
            bool getRestrictedTcn()  const;
            void setGuard(const MstpPortGuardRootEnum &guard);
            MstpPortGuardRootEnum getGuard()  const;
 
            void  printMsg();

// Now the data members

        private:
        protected:
        public:
            UI8  m_instanceId;
            bool  m_restrictedRole;
            bool  m_restrictedTcn;
            MstpPortGuardRootEnum  m_guard;
    };
}
#endif                                            //NSMUPDATEPHYINSTANCESTPCONFIGMESSAGE_H
