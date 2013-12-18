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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Bhavin                                                     *
 **************************************************************************/


#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Span/NsmLocaldestinationMirrorportMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Span/SpanMessageDef.h"

namespace DcmNs
{
    NsmLocaldestinationMirrorportMessage::NsmLocaldestinationMirrorportMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMLOCALDESTINATIONMIRRORPORT)
    {
    }

	NsmLocaldestinationMirrorportMessage::NsmLocaldestinationMirrorportMessage (const string &destifName, const UI32 &opCode)
	    : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMLOCALDESTINATIONMIRRORPORT),
		 m_destifName   (destifName),
		 m_opCode    	(opCode)
	{
	}

	NsmLocaldestinationMirrorportMessage::~NsmLocaldestinationMirrorportMessage ()
	{
	}

	void  NsmLocaldestinationMirrorportMessage::setupAttributesForSerialization()
	{
	     ManagementInterfaceMessage::setupAttributesForSerialization ();
	     addSerializableAttribute (new AttributeString(&m_destifName,"destifName"));
	     addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
	}

	void  NsmLocaldestinationMirrorportMessage::setDestifName(const string &destifName)
	{
	     m_destifName  =  destifName;
	}

    string  NsmLocaldestinationMirrorportMessage::getDestifName() const
    {
         return (m_destifName);
    }

	void  NsmLocaldestinationMirrorportMessage::setOpcode(const UI32 &opCode)
	{
	     m_opCode  =  opCode;
	}

	UI32  NsmLocaldestinationMirrorportMessage::getOpcode() const
	{
	     return (m_opCode);
	}
}



