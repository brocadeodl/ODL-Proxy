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

#ifndef NSMCLIENTDELETEVLANCLASSIFIERTOPOSPMESSAGE_H
#define NSMCLIENTDELETEVLANCLASSIFIERTOPOSPMESSAGE_H

#include "ClientInterface/Nsm/NsmVlanProvisionMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmClientDeleteVlanClassifierToPoSPMessage : public NsmVlanProvisionMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmClientDeleteVlanClassifierToPoSPMessage ();
            NsmClientDeleteVlanClassifierToPoSPMessage (const UI32 &id,const string &ifName,const UI64 &ifIndex,const UI32 &groupId,const UI32 &opcode);
            virtual    ~NsmClientDeleteVlanClassifierToPoSPMessage ();
            void setId(const UI32 &id);
            UI32 getId()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfIndex(const UI64 &ifIndex);
            UI64 getIfIndex()  const;
            void setGroupId(const UI32 &groupId);
            UI32 getGroupId()  const;
            void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;
            virtual const void *getCStructureForInputs    ();

	    

// Now the data members

        private:
        protected:
        public:
            UI32  m_id;
            string  m_ifName;
            UI64  m_ifIndex;
            UI32  m_groupId;
            UI32  m_opcode;
    };
}
#endif                                            //NSMDELETEVLANCLASSIFIERTOPOSPMESSAGE_H
