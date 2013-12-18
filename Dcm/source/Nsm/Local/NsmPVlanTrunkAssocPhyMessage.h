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
 *   Author : rprashar                                                     *
 **************************************************************************/

#ifndef NSMPVLANTRUNKASSOCPHYMESSAGE_H
#define NSMPVLANTRUNKASSOCPHYMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmPVlanTrunkAssocPhyMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmPVlanTrunkAssocPhyMessage ();
            NsmPVlanTrunkAssocPhyMessage (const string &ifName,const UI32 &primaryVlanId,const UI32 &secondaryVlanId,const UI32 &opcode);
            virtual    ~NsmPVlanTrunkAssocPhyMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;

            void setPrimaryVlanId(const UI32 &primaryVlanId);
            UI32 getPrimaryVlanId()  const;
            void setSecondaryVlanId(const UI32 &secondaryVlanId);
            UI32 getSecondaryVlanId()  const;

            void  setOpcode(const UI32 &opcode);
     	    UI32 getOpcode() const;
     	    void setIfType(const UI32 &ifType);
     	    UI32 getIfType() const;
            void setCmdCode(const SI32 &cmdCode);
            SI32 getCmdCode()  const;



        private:
        protected:
        public:
            string  m_ifName;
            UI32  m_primaryVlanId;
            UI32  m_secondaryVlanId;
	        UI32 m_opcode;
	        UI32 m_ifType;
	        SI32  m_cmdCode;
    };
}
#endif
