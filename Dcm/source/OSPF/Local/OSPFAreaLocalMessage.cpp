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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "OSPF/Local/OSPFAreaLocalMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"

namespace DcmNs
{

    OSPFAreaLocalMessage::OSPFAreaLocalMessage ()
        //: ManagementInterfaceMessage (OSPFLocalObjectManager::getClassName(), OSPFAREALOCAL)
        : PrismMessage (OSPFLocalObjectManager::getPrismServiceId (), OSPFAREALOCAL)
    {
		m_areaId = "";
		m_opCode = 0;
		m_cmd    = 0;
		m_nssa   = 0;
		m_is_nssa_no_summary = 0;
		m_is_nssa_def_info_orig = 0;
		m_is_stub_no_summary = 0;
		m_stub = 0;
    }


    OSPFAreaLocalMessage::~OSPFAreaLocalMessage ()
    {
    }

    void  OSPFAreaLocalMessage::setupAttributesForSerialization()
    {
        //ManagementInterfaceMessage::setupAttributesForSerialization ();
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI32(&m_cmd,"cmd"));
        addSerializableAttribute (new AttributeString(&m_areaId,"areaId"));
        addSerializableAttribute (new AttributeUI32(&m_nssa,"nssa"));
        addSerializableAttribute (new AttributeBool(&m_is_nssa,"is_nssa"));
        addSerializableAttribute (new AttributeBool(&m_is_nssa_no_summary,"is_nssa_no_summary"));
        addSerializableAttribute (new AttributeBool(&m_is_nssa_def_info_orig,"is_nssa_def_info_orig"));
        addSerializableAttribute (new AttributeBool(&m_is_stub_no_summary,"is_stub_no_summary"));
        addSerializableAttribute (new AttributeUI32(&m_stub,"stub"));
        
    }


    void  OSPFAreaLocalMessage::setAreaId(const string &areaId)
    {
        m_areaId  =  areaId;
    }

    string  OSPFAreaLocalMessage::getAreaId() const
    {
        return (m_areaId);
    }

	void  OSPFAreaLocalMessage::setOpCode(const unsigned int &opCode)
    {
        m_opCode  =  opCode;
    }

    unsigned int OSPFAreaLocalMessage::getOpCode() const
    {
        return (m_opCode);
    }

	void  OSPFAreaLocalMessage::setCmd(const unsigned int &cmd)
    {
        m_cmd  =  cmd;
    }

    unsigned int OSPFAreaLocalMessage::getCmd() const
    {
        return (m_cmd);
    }

    void OSPFAreaLocalMessage::setNssa(const unsigned int &nssa)
    {
        m_nssa = nssa;
    }

    unsigned int OSPFAreaLocalMessage::getNssa()  const
    {
        return (m_nssa);
    }

    void OSPFAreaLocalMessage::setIsNssa(const bool &is_nssa)
    {
        m_is_nssa = is_nssa;
    }

    bool OSPFAreaLocalMessage::getIsNssa() const
    {
        return (m_is_nssa);
    }    

    void OSPFAreaLocalMessage::setIsNssaNoSummary(const bool &is_nssa_no_summary)
    {
        m_is_nssa_no_summary = is_nssa_no_summary;
    }

    bool OSPFAreaLocalMessage::getIsNssaNoSummary() const
    {
        return (m_is_nssa_no_summary);
    }    

    void OSPFAreaLocalMessage::setIsNssaDefInfoOrig(const bool &is_nssa_def_info_orig)
    {
        m_is_nssa_def_info_orig = is_nssa_def_info_orig;
    }

    bool OSPFAreaLocalMessage::getIsNssaDefInfoOrig() const
    {
        return (m_is_nssa_def_info_orig);
    }    

    void OSPFAreaLocalMessage::setStub(const unsigned int &stub)
    {
        m_stub = stub;
    }

    unsigned int OSPFAreaLocalMessage::getStub()  const
    {
        return (m_stub);
    }

    void OSPFAreaLocalMessage::setIsStubNoSummary(const bool &is_stub_no_summary)
    {
        m_is_stub_no_summary = is_stub_no_summary;
    }

    bool OSPFAreaLocalMessage::getIsStubNoSummary() const
    {
        return (m_is_stub_no_summary);
    }    

	void 
    OSPFAreaLocalMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		return;
	}
}
