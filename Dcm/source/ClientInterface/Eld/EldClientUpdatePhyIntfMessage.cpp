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
 *   Copyright (C) 2005-20l1 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#include "ClientInterface/Eld/EldClientUpdatePhyIntfMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{

    EldClientUpdatePhyIntfMessage::EldClientUpdatePhyIntfMessage ()
        : DcmManagementInterfaceMessage (MSG_ELD_INTF_CONF),
        m_prio              (128),
        m_vlanId            (1),
        m_opCode            (0)
	{
		m_vlanidList.clear();
		m_ifType = IF_TYPE_INVALID;
	//	m_vlanIdRange = UI32Range("");
    }

    EldClientUpdatePhyIntfMessage::EldClientUpdatePhyIntfMessage (const UI32 &lmqi,const UI32 &prio,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const UI32 &opCode,const bool &negation)
        : DcmManagementInterfaceMessage (MSG_ELD_INTF_CONF),
        m_lmqi    (lmqi),
        m_prio    (prio),
        m_enable    (enable),
        m_fastLeave    (fastLeave),
        m_queryInterval    (queryInterval),
        m_querierEnable    (querierEnable),
        m_mrouterTimeout    (mrouterTimeout),
        m_vlanId    (vlanId),
        m_vlanIfName    (vlanIfName),
        m_vlanIfIndex    (vlanIfIndex),
        m_opCode    (opCode),
        m_negation    (negation)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    EldClientUpdatePhyIntfMessage::~EldClientUpdatePhyIntfMessage ()
    {
    }

    void  EldClientUpdatePhyIntfMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
		addSerializableAttribute (new AttributeString(&m_IfName,"IfName"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_prio,"prio"));
		addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
		addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
	//	addSerializableAttribute (new AttributeUI32Range(&m_vlanIdRange,"vlanIdRange"));
    }
/*	
	void  EldClientUpdatePhyIntfMessage::setVlanIdRange(const UI32Range &vlanRange)
	{
		m_vlanIdRange  =  vlanRange;
	}

	UI32Range EldClientUpdatePhyIntfMessage::getVlanIdRange() const
	{
		return (m_vlanIdRange);
	} */

    void  EldClientUpdatePhyIntfMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  EldClientUpdatePhyIntfMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  EldClientUpdatePhyIntfMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  EldClientUpdatePhyIntfMessage::getOpCode() const
    {
        return (m_opCode);
    }
	void  EldClientUpdatePhyIntfMessage::setIfName(const string &ifName)
	{
		m_IfName  =  ifName;
	}

	string  EldClientUpdatePhyIntfMessage::getIfName() const
	{
		return (m_IfName);
	}

    void  EldClientUpdatePhyIntfMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  EldClientUpdatePhyIntfMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  EldClientUpdatePhyIntfMessage::setPrio(const UI32 &prio)
    {
        m_prio  =  prio;
    }

    UI32  EldClientUpdatePhyIntfMessage::getPrio() const
    {
        return (m_prio);
	}

	void  EldClientUpdatePhyIntfMessage::setVlanidList(const vector<UI32> &vlanidList)
	{
		unsigned int i;
		m_vlanidList  =  vlanidList;
		for (i = 0; i< m_vlanidList.size(); i++)
		{
			WaveNs::trace (TRACE_LEVEL_DEBUG, string ("\t TO CLIENT: setVlanidList(): i = ") + i + string("VLAN_ID = ") + m_vlanidList[i]);
		}
	} 

	vector<UI32>  EldClientUpdatePhyIntfMessage::getVlanidList() const
	{
		return (m_vlanidList);
	}

    const void * EldClientUpdatePhyIntfMessage::getCStructureForInputs()
    {
        eld_mgmt_conf_req_t *eld_intf_config = NULL;
#if 1
        unsigned int i, size = 0;
        unsigned char *ptr = NULL;

        size = sizeof(eld_mgmt_conf_req_t) + (m_vlanidList.size() * sizeof(uint16_t));

        ptr = (unsigned char *)calloc(1, size);
        if (ptr == NULL) {
			WaveNs::trace (TRACE_LEVEL_ERROR, string(" Calloc failed for  size :"));
            return (eld_intf_config);
        }
        eld_intf_config = (eld_mgmt_conf_req_t *)&ptr[0];
#else 
        eld_intf_config = (eld_mgmt_conf_req_t *)malloc (sizeof (eld_mgmt_conf_req_t));

		if (eld_intf_config == NULL) {
			WaveNs::trace (TRACE_LEVEL_ERROR, string(" Calloc failed for  size :"));
            return (eld_intf_config);
		}
#endif
        eld_intf_config->entry.intf_conf.cmd_code = m_opCode;
        eld_intf_config->entry.intf_conf.intf_type = m_ifType;
		trace(TRACE_LEVEL_DEBUG, string("OP_CODE IS ") + eld_intf_config->entry.intf_conf.cmd_code);
		strncpy (eld_intf_config->entry.intf_conf.if_name, m_IfName.c_str(), 63);

		trace(TRACE_LEVEL_DEBUG, string("IF_NAME IS ") + eld_intf_config->entry.intf_conf.if_name);
		if (IS_CMDCODE_SET(m_opCode, ELD_INTF_CONF_PRIORITY))
        	eld_intf_config->entry.intf_conf.priority = m_prio;
		trace(TRACE_LEVEL_DEBUG, string("PRIORITY IS ") + eld_intf_config->entry.intf_conf.priority);

		if ((IS_CMDCODE_SET(m_opCode, ELD_INTF_CONF_ELD_ENABLE)) 
			|| (IS_CMDCODE_SET(m_opCode, ELD_INTF_CONF_ELD_DISABLE)))
		{
			if (!m_vlanidList.empty()) {

				eld_intf_config->entry.intf_conf.vlan_num = m_vlanidList.size(); 
				trace(TRACE_LEVEL_DEBUG, string("VLAN_NUM IS ") + eld_intf_config->entry.intf_conf.vlan_num);

				if (eld_intf_config->entry.intf_conf.vlan_num) {
					for (i = 0; i< eld_intf_config->entry.intf_conf.vlan_num; i++)
					{
						eld_intf_config->entry.intf_conf.vid[i] = m_vlanidList[i];
						trace(TRACE_LEVEL_DEBUG, string("getCStructureForInputs(): I = ") + i + string(", VLAN_ID IS ") + eld_intf_config->entry.intf_conf.vid[i]);
					}

				}
			}
		}
#if 0
		/* For ELD_DISABLE, CLI spec allows only one vlan-id to be mentioend for removal. 
		   Hence, ELD_DISABLE will currently use vlan_id, instead of vid array (as done in
		   ELD_ENABLE case)*/
		if (IS_CMDCODE_SET(m_opCode, ELD_INTF_CONF_ELD_DISABLE)) {
			eld_intf_config->entry.intf_conf.vlan_id = m_vlanidList[0];
			trace(TRACE_LEVEL_DEBUG, string("VLAN-ID IS ") + eld_intf_config->entry.intf_conf.vlan_id);
		}
#endif
        return (eld_intf_config);
    }

	void EldClientUpdatePhyIntfMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
