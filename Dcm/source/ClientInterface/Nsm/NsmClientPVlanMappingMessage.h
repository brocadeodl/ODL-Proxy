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

#ifndef NSMCLIENTPVLANMAPPINGMESSAGE_H
#define NSMCLIENTPVLANMAPPINGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Nsm/Local/NsmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmClientPVlanMappingMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmClientPVlanMappingMessage ();
            NsmClientPVlanMappingMessage (const string &ifName,const UI32 &primaryVlanId,const UI32Range &secVlanRange, const  UI32 &oper);//,const UI32 &opcode);
            virtual    ~NsmClientPVlanMappingMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;

            void setPrimaryVlanId(const UI32 &primaryVlanId);
            UI32 getPrimaryVlanId()  const;
            void setSecVlanRange(const UI32Range &secVlanRange);
            UI32Range getSecVlanRange()  const;
            void setoper(const UI32  &oper);
            UI32 getoper() const;

            void  setOpcode(const UI32 &opcode);
     	    UI32 getOpcode() const;
     	    void setIfType(const UI32 &ifType);
     	    UI32 getIfType() const;
            void setCmdCode(const SI32 &cmdCode);
            SI32 getCmdCode()  const;
    	    virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);


        private:
        protected:
        public:
            string  m_ifName;
            UI32  m_primaryVlanId;
            UI32Range  m_secVlanRange;
            UI32  m_oper;
	        UI32 m_opcode;
	        UI32 m_ifType;
	        SI32  m_cmdCode;
    };
}
#endif
