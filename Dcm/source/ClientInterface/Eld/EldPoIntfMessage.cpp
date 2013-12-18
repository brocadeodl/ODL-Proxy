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

#include "ClientInterface/Eld/EldPoIntfMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{

    EldPoIntfMessage::EldPoIntfMessage ()
        : DcmManagementInterfaceMessage (MSG_ELD_INTF_PO_CONF),
//        m_lmqi              (0),
//        m_prio              (0),
 //       m_enable            (false),
  //      m_fastLeave         (false),
   //     m_queryInterval     (0),
    //    m_querierEnable     (false),
     //   m_mrouterTimeout    (0),
 //       m_vlanId            (1),
      //  m_vlanIfIndex       (0),
        m_cmdCode            (0)
       // m_negation          (false)
	{
		m_vlanidList.clear();
	//	m_vlanIdRange = UI32Range("");
    }

/*    EldPoIntfMessage::EldPoIntfMessage (const UI32 &lmqi,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const UI32 &cmdCode,const bool &negation)
        : DcmManagementInterfaceMessage (MSG_ELD_INTF_PO_CONF),
        m_lmqi    (lmqi),
        m_enable    (enable),
        m_fastLeave    (fastLeave),
        m_queryInterval    (queryInterval),
        m_querierEnable    (querierEnable),
        m_mrouterTimeout    (mrouterTimeout),
        m_vlanId    (vlanId),
        m_vlanIfName    (vlanIfName),
        m_vlanIfIndex    (vlanIfIndex),
        m_cmdCode    (cmdCode),
        m_negation    (negation)
    {
    } */

    EldPoIntfMessage::~EldPoIntfMessage ()
    {
    }

    void  EldPoIntfMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeString(&m_poId,"poId"));
		addSerializableAttribute (new AttributeString(&m_poName,"poName"));
//		addSerializableAttribute (new AttributeString(&m_IfName,"IfName"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeUI32(&m_prio,"prio"));
		addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
	//	addSerializableAttribute (new AttributeUI32Range(&m_vlanIdRange,"vlanIdRange"));
    }
/*	
	void  EldPoIntfMessage::setVlanIdRange(const UI32Range &vlanRange)
	{
		m_vlanIdRange  =  vlanRange;
	}

	UI32Range EldPoIntfMessage::getVlanIdRange() const
	{
		return (m_vlanIdRange);
	} */

    void  EldPoIntfMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
    }

    void  EldPoIntfMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  EldPoIntfMessage::getPoName() const
    {
        return (m_poName);
    }
    string  EldPoIntfMessage::getPoId() const
    {
        return (m_poId);
    }

    void  EldPoIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  EldPoIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

/*    void  EldPoIntfMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  EldPoIntfMessage::getVlanId() const
    {
        return (m_vlanId);
    } */

    void  EldPoIntfMessage::setPrio(const UI32 &prio)
    {
        m_prio  =  prio;
    }

    UI32  EldPoIntfMessage::getPrio() const
    {
        return (m_prio);
	}

	void  EldPoIntfMessage::setVlanidList(const vector<UI32> &vlanidList)
	{
		unsigned int i;
		m_vlanidList  =  vlanidList;
		for (i = 0; i< m_vlanidList.size(); i++)
		{
			WaveNs::trace (TRACE_LEVEL_DEBUG, string ("\t TO CLIENT: setVlanidList(): i = ") + i + string(", VLAN_ID = ") + m_vlanidList[i]);
		}
	} 

	vector<UI32>  EldPoIntfMessage::getVlanidList() const
	{
		return (m_vlanidList);
	}

    void  EldPoIntfMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  EldPoIntfMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    const void * EldPoIntfMessage::getCStructureForInputs()
	{
		unsigned int i, size = 0;
		unsigned char *ptr = NULL;
		eld_mgmt_conf_req_t *eld_intf_config = NULL;

		size = sizeof(eld_mgmt_conf_req_t) + (m_vlanidList.size() * sizeof(uint16_t));
		ptr = (unsigned char *)calloc(1, size);
		if (ptr == NULL) {
			WaveNs::trace (TRACE_LEVEL_ERROR, string(" Calloc failed for  size :"));
			return (eld_intf_config);
		}
		eld_intf_config = (eld_mgmt_conf_req_t *)&ptr[0];

		eld_intf_config->entry.intf_conf.cmd_code = m_cmdCode;
		trace(TRACE_LEVEL_DEBUG, string("Op_code is ") + eld_intf_config->entry.intf_conf.cmd_code);
		eld_intf_config->entry.intf_conf.po_info.poIfIndex = m_poIfIndex;
		trace(TRACE_LEVEL_DEBUG, string("Ifindex is ") + eld_intf_config->entry.intf_conf.po_info.poIfIndex);
		strncpy (eld_intf_config->entry.intf_conf.po_info.poId, m_poId.c_str (), 64);
		trace(TRACE_LEVEL_DEBUG, string("Po-id is ") +  eld_intf_config->entry.intf_conf.po_info.poId);

		if (IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_PRIORITY)) {
			eld_intf_config->entry.intf_conf.priority = m_prio;
			trace(TRACE_LEVEL_DEBUG, string("Priority is ") +  eld_intf_config->entry.intf_conf.priority);
		}

		if ((IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_ELD_ENABLE))
			||
			(IS_CMDCODE_SET(m_cmdCode, ELD_INTF_PO_CONF_ELD_DISABLE)))
		{
			if (!m_vlanidList.empty()) {
#if 0
				// vlan_id is used for ELD_DISABLE only, although it's populated for both 
				// ELD_ENABLE and ELD_DISABLE cases
				eld_intf_config->entry.intf_conf.vlan_id = m_vlanidList[0];
				trace(TRACE_LEVEL_DEBUG, string("Vlan-id is ") + eld_intf_config->entry.intf_conf.vlan_id);
#endif
				// vid and vlan_num are used for ELD_ENABLE only
				eld_intf_config->entry.intf_conf.vlan_num = m_vlanidList.size(); 
				trace(TRACE_LEVEL_DEBUG, string("No. of Vlans is ") + eld_intf_config->entry.intf_conf.vlan_num);

				if (eld_intf_config->entry.intf_conf.vlan_num) {
					for (i = 0; i < eld_intf_config->entry.intf_conf.vlan_num; i++)
					{
						eld_intf_config->entry.intf_conf.vid[i] = m_vlanidList[i];
						trace(TRACE_LEVEL_DEBUG, string("getCStructureForInputs(): I = ") + i + string(", Vlan_id is ") + eld_intf_config->entry.intf_conf.vid[i]);
					}
				}
			}
		}
		return (eld_intf_config);
	}

	void EldPoIntfMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
