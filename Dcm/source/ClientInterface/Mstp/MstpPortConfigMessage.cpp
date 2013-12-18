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

#include <string.h>
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "Nsm/Local/NsmUpdatePoStpConfigSPMessage.h"
#include "Nsm/Local/NsmUpdatePhyStpConfigMessage.h"
#include "Nsm/Local/NsmUpdatePoInstanceStpConfigSPMessage.h"
#include "Nsm/Local/NsmUpdatePhyInstanceStpConfigMessage.h"

namespace DcmNs
{

    MstpPortConfigMessage::MstpPortConfigMessage ()
        : DcmManagementInterfaceMessage (MSTPPORTCONFIG)
    {
        initialyzeDefaultMembers();
        m_priority = MSTP_DCM_DEFAULT_PORT_PRIORITY;
        m_cost = MSTP_DCM_INVALID_COST;
	    m_helloTime = MSTP_DCM_DEFAULT_HELLO_TIME;
        m_ifType = IF_TYPE_INVALID;
	    m_cmdCode = 0;
	    m_instanceId = 0;
	    m_portId  = 0;
        m_bpduDropDirection = BPDU_DROP_DISABLED;
    }

   MstpPortConfigMessage::MstpPortConfigMessage(NsmUpdatePoStpConfigSPMessage &msg)
        : DcmManagementInterfaceMessage (MSTPPORTCONFIG)
    {
        initialyzeDefaultMembers();
        m_priority = msg.m_priority;
        m_cost = msg.m_cost;
        m_helloTime = msg.m_helloTime;
        m_ifType = IF_TYPE_PO;
        m_ifName = msg.m_ifName;
	    m_instanceId = 0;
        m_cmdCode = msg.m_cmdCode;
        m_portId  = msg.m_portId;
        m_bpduDropDirection = BPDU_DROP_DISABLED;
    }
   
   MstpPortConfigMessage::MstpPortConfigMessage(NsmUpdatePoInstanceStpConfigSPMessage &msg)
        : DcmManagementInterfaceMessage (MSTPPORTINSTCONFIG)
    {
        initialyzeDefaultMembers();
        m_priority = msg.m_priority;
        m_cost = msg.m_cost;
        m_helloTime = MSTP_DCM_DEFAULT_HELLO_TIME;
        m_ifType = IF_TYPE_PO;
        m_ifName = msg.m_ifName;
        m_instanceId = msg.m_instanceId;
        m_cmdCode = msg.m_cmdCode;
        m_portId  = 0;
        m_bpduDropDirection = BPDU_DROP_DISABLED;
    }

   MstpPortConfigMessage::MstpPortConfigMessage(NsmUpdatePhyInstanceStpConfigMessage &msg)
        : DcmManagementInterfaceMessage (MSTPPORTINSTCONFIG)
    {
        initialyzeDefaultMembers();
        m_priority = msg.m_priority;
        m_cost = msg.m_cost;
        m_helloTime = MSTP_DCM_DEFAULT_HELLO_TIME;
        m_ifType = msg.m_ifType;
        m_ifName = msg.m_ifName;
        m_instanceId = msg.m_instanceId;
        m_cmdCode = msg.m_cmdCode;
        m_portId  = 0;
        m_bpduDropDirection = BPDU_DROP_DISABLED;
    }


   MstpPortConfigMessage::MstpPortConfigMessage(NsmUpdatePhyStpConfigMessage &msg)
        : DcmManagementInterfaceMessage (MSTPPORTCONFIG)
    {
        initialyzeDefaultMembers();
        m_priority = msg.m_priority;
        m_cost = msg.m_cost;
        m_helloTime = msg.m_helloTime;
        m_ifType = msg.m_ifType;
        m_ifName = msg.m_ifName;
        m_cmdCode = msg.m_cmdCode;
        m_instanceId = 0;
        m_portId  = 0;
        m_bpduDropDirection = msg.m_bpduDropDirection;
    }
    void MstpPortConfigMessage::initialyzeDefaultMembers()
    {
        m_restrictedRole = false;
        m_restrictedTcn = false;
        m_autoEdge = false;
        m_portFast = false;
        m_edgePort = false;
        m_bpduGuardEnable = false;
        m_bpduGuard = false;
        m_bpduFilter = false;
        m_guard = false;
        m_linkShared = MSTPPORTLINKTYPEP2P;
        m_bpduMac = MSTPPORTPVSTBPDUMACBRCD;
        m_stpEnable = false;
        m_taggedBpduEnable = false; 
    }

       MstpPortConfigMessage::~MstpPortConfigMessage ()
    {
    }

    void  MstpPortConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_ifType),"ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI8(&m_instanceId,"instanceId"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32(&m_helloTime,"helloTime"));
        addSerializableAttribute (new AttributeUI32(&m_cost,"cost"));
        addSerializableAttribute (new AttributeBool(&m_restrictedRole,"restrictedRole"));
        addSerializableAttribute (new AttributeBool(&m_restrictedTcn,"restrictedTcn"));
        addSerializableAttribute (new AttributeBool(&m_autoEdge,"autoEdge"));
        addSerializableAttribute (new AttributeBool(&m_portFast,"portFast"));
        addSerializableAttribute (new AttributeBool(&m_edgePort,"edgePort"));
        addSerializableAttribute (new AttributeBool(&m_bpduGuardEnable,"bpduGuardEnable"));
        addSerializableAttribute (new AttributeBool(&m_bpduGuard,"bpduGuard"));
        addSerializableAttribute (new AttributeBool(&m_bpduFilter,"bpduFilter"));
        addSerializableAttribute (new AttributeBool(&m_guard,"guard"));
        addSerializableAttribute (new AttributeBool(&m_linkShared,"linkShared"));
        addSerializableAttribute (new AttributeBool(&m_bpduMac,"bpduMac"));
        addSerializableAttribute (new AttributeBool(&m_taggedBpduEnable, "taggedBpduEnable"));
        addSerializableAttribute (new AttributeBool(&m_stpEnable,"stpEnable"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_portId,"portId"));
        addSerializableAttribute (new AttributeUI8(&m_bpduDropDirection,"bpduDropDirection"));
    }

    void  MstpPortConfigMessage::setInterfaceInfo(const InterfaceType ifType, const string &ifName)
    {
        m_ifType = ifType;
        m_ifName  =  ifName;
    }

    InterfaceType MstpPortConfigMessage::getIfType() const
    {
        return m_ifType;
    }

    string  MstpPortConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  MstpPortConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpPortConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void  MstpPortConfigMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  MstpPortConfigMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  MstpPortConfigMessage::setCost(const UI32 &cost)
    {
        m_cost  =  cost;
    }

    UI32  MstpPortConfigMessage::getCost() const
    {
        return (m_cost);
    }

    void  MstpPortConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }
 
    UI32  MstpPortConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpPortConfigMessage::setInstanceId(const UI8 &instanceId)
    {
        m_instanceId  =  instanceId;
    }

    UI8  MstpPortConfigMessage::getInstanceId() const
    {
        return (m_instanceId);
    }


	void MstpPortConfigMessage::setRestrictedRole(const bool &restrictedRole)
	{
		m_restrictedRole = restrictedRole;
	}

	void MstpPortConfigMessage::setRestrictedTcn(const bool &restrictedTcn)
	{
		m_restrictedTcn = restrictedTcn;
	}

	void MstpPortConfigMessage::setAutoEdge(const bool &autoEdge)
	{
		m_autoEdge = autoEdge;
	}

	void MstpPortConfigMessage::setPortFast(const bool &portFast)
	{
		m_portFast = portFast;
	}

	void MstpPortConfigMessage::setEdgePort(const bool &edgePort)
	{
		m_edgePort = edgePort;
	}

	void MstpPortConfigMessage::setBpduGuardEnable(const bool &bpduGuardEnable)
	{
		m_bpduGuardEnable = bpduGuardEnable;
	}

	void MstpPortConfigMessage::setBpduGuard(const bool &bpduGuard)
	{
		m_bpduGuard = bpduGuard;
	}

	void MstpPortConfigMessage::setBpduDropDirection(const UI8 &direction)
	{
		m_bpduDropDirection = direction;
	}

	void MstpPortConfigMessage::setBpduFilter(const bool &bpduFilter)
	{
		m_bpduFilter = bpduFilter;
	}

	void MstpPortConfigMessage::setGuard(const bool &guard)
	{
		m_guard = guard;
	}

	void MstpPortConfigMessage::setLinkShared(const bool &linkShared)
	{
		m_linkShared = linkShared;
	}

	void MstpPortConfigMessage::setStpEnable(const bool &stpEnable)
	{
		m_stpEnable = stpEnable;
	}

	void MstpPortConfigMessage::setBpduMac(const bool &bpduMac)
	{
		m_bpduMac = bpduMac;
	}

	void MstpPortConfigMessage::setPortId(const UI32 portid)
	{
	    m_portId = portid;
	}

	UI32 MstpPortConfigMessage::getPortId() const
	{
	    return m_portId;
	}

    bool MstpPortConfigMessage::getTaggedBpduEnable () const
    {
        return m_taggedBpduEnable;
    }

    void MstpPortConfigMessage::setTaggedBpduEnable (bool taggedBpduEnable)
    {
        m_taggedBpduEnable = taggedBpduEnable;
    }


    const void * MstpPortConfigMessage::getCStructureForInputs() 
    {
	    mstp_msg_port_config *br_port_config = (mstp_msg_port_config*) calloc(1, sizeof(mstp_msg_port_config));
	    if (br_port_config == NULL) {
	        trace(TRACE_LEVEL_ERROR, "MstpPortConfigMessage::getCStructureForInputs : calloc returned NULL");
	        return NULL;
	    }

	    br_port_config->cmd = m_cmdCode;
	    br_port_config->priority = m_priority;
	    br_port_config->cost = m_cost;
	    br_port_config->hello_time = m_helloTime;
	    br_port_config->restricted_role = m_restrictedRole;
	    br_port_config->restricted_tcn = m_restrictedTcn;
	    br_port_config->auto_edge= m_autoEdge;
	    br_port_config->port_fast = m_portFast;
	    br_port_config->edge_port = m_edgePort;
	    //br_port_config->bpdu_guard_enable = m_bpduGuardEnable;
	    br_port_config->bpdu_guard_enable = m_bpduDropDirection;
	    br_port_config->bpdu_guard = m_bpduGuard;
	    br_port_config->bpdu_filter = m_bpduFilter;
	    br_port_config->guard_root = m_guard;
	    br_port_config->link_shared = m_linkShared;
	    br_port_config->bpdu_mac = m_bpduMac;
	    br_port_config->stp_disable = !m_stpEnable;
	    //br_port_config->if_index = m_ifIndex;
	    br_port_config->inst = m_instanceId;
        br_port_config->tagged_bpdu_enable = m_taggedBpduEnable;
	    br_port_config->port_id = m_portId;

	    writeIfName(br_port_config->name, m_ifType, m_ifName);
        br_port_config->if_type = m_ifType;

	    return (br_port_config);
    }

    void MstpPortConfigMessage::writeIfName(unsigned char *toBuff, const InterfaceType &ifType, const string &ifName)
    {
        const char *name = ifName.c_str();
        if (ifType != IF_TYPE_PO) {
            const char *p = strchr(name, '/');
            if (p && strchr(p+1, '/')) name = p+1;
        }
        strncpy((char *)toBuff, name, MSTP_DCM_IF_NAME_LEN);
    }

  void  MstpPortConfigMessage::printMsg()
  {
        tracePrintf(TRACE_LEVEL_INFO, "MstpPortConfigMessage::printMsg()");
        tracePrintf(TRACE_LEVEL_INFO, "m_cmdCode     : %u", m_cmdCode);
        tracePrintf(TRACE_LEVEL_INFO, "m_ifType      : %u", m_ifType);
        tracePrintf(TRACE_LEVEL_INFO, "m_ifName      : %s", m_ifName.c_str());
        tracePrintf(TRACE_LEVEL_INFO, "m_instanceId  : %u", (UI32)m_instanceId);
        tracePrintf(TRACE_LEVEL_INFO, "m_priority    : %u", m_priority);
        tracePrintf(TRACE_LEVEL_INFO, "m_cost        : %u", m_cost);
        tracePrintf(TRACE_LEVEL_INFO, "m_helloTime   : %u", m_helloTime);
        tracePrintf(TRACE_LEVEL_INFO, "m_portId      : %u", m_portId);
        tracePrintf(TRACE_LEVEL_INFO, "m_bpduDropDirection      : %u", m_bpduDropDirection);
    }

}
