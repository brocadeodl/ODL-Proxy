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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "ClientInterface/Igmps/IgmpsClientUpdateVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"

namespace DcmNs
{

    IgmpsClientUpdateVlanConfigMessage::IgmpsClientUpdateVlanConfigMessage ()
        : DcmManagementInterfaceMessage (IGMPSCLIENTUPDATEVLANCONFIG),
        m_lmqi              (0),
        m_qmrt              (0),
        m_enable            (false),
        m_fastLeave         (false),
        m_queryInterval     (0),
        m_querierEnable     (false),
        m_mrouterTimeout    (0),
        m_vlanId            (0),
        m_vlanIfIndex       (0),
        m_opCode            (IGMPS_DEFAULT_CMD_CODE),
        m_negation          (false)
    {
    }

    IgmpsClientUpdateVlanConfigMessage::IgmpsClientUpdateVlanConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const UI32 &opCode,const bool &negation)
        : DcmManagementInterfaceMessage (IGMPSCLIENTUPDATEVLANCONFIG),
        m_lmqi    (lmqi),
        m_qmrt    (qmrt),
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
    }

    IgmpsClientUpdateVlanConfigMessage::~IgmpsClientUpdateVlanConfigMessage ()
    {
    }

    void  IgmpsClientUpdateVlanConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_lmqi,"lmqi"));
        addSerializableAttribute (new AttributeUI32(&m_qmrt,"qmrt"));
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeBool(&m_fastLeave,"fastLeave"));
        addSerializableAttribute (new AttributeUI32(&m_queryInterval,"queryInterval"));
        addSerializableAttribute (new AttributeBool(&m_querierEnable,"querierEnable"));
        addSerializableAttribute (new AttributeUI32(&m_mrouterTimeout,"mrouterTimeout"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeString(&m_vlanIfName,"vlanIfName"));
        addSerializableAttribute (new AttributeUI64(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
    }

    void  IgmpsClientUpdateVlanConfigMessage::setLmqi(const UI32 &lmqi)
    {
        m_lmqi  =  lmqi;
    }

    UI32  IgmpsClientUpdateVlanConfigMessage::getLmqi() const
    {
        return (m_lmqi);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setQmrt(const UI32 &qmrt)
    {
        m_qmrt  =  qmrt;
    }

    UI32  IgmpsClientUpdateVlanConfigMessage::getQmrt() const
    {
        return (m_qmrt);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  IgmpsClientUpdateVlanConfigMessage::getEnable() const
    {
        return (m_enable);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setFastLeave(const bool &fastLeave)
    {
        m_fastLeave  =  fastLeave;
    }

    bool  IgmpsClientUpdateVlanConfigMessage::getFastLeave() const
    {
        return (m_fastLeave);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setQueryInterval(const UI32 &queryInterval)
    {
        m_queryInterval  =  queryInterval;
    }

    UI32  IgmpsClientUpdateVlanConfigMessage::getQueryInterval() const
    {
        return (m_queryInterval);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setQuerierEnable(const bool &querierEnable)
    {
        m_querierEnable  =  querierEnable;
    }

    bool  IgmpsClientUpdateVlanConfigMessage::getQuerierEnable() const
    {
        return (m_querierEnable);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setMrouterTimeout(const UI32 &mrouterTimeout)
    {
        m_mrouterTimeout  =  mrouterTimeout;
    }

    UI32  IgmpsClientUpdateVlanConfigMessage::getMrouterTimeout() const
    {
        return (m_mrouterTimeout);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  IgmpsClientUpdateVlanConfigMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setVlanIfName(const string &vlanIfName)
    {
        m_vlanIfName  =  vlanIfName;
    }

    string  IgmpsClientUpdateVlanConfigMessage::getVlanIfName() const
    {
        return (m_vlanIfName);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setVlanIfIndex(const UI64 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI64  IgmpsClientUpdateVlanConfigMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  IgmpsClientUpdateVlanConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  IgmpsClientUpdateVlanConfigMessage::setNegation(const bool &negation)
    {
        m_negation  =  negation;
    }

    bool  IgmpsClientUpdateVlanConfigMessage::getNegation() const
    {
        return (m_negation);
    }

	const void *IgmpsClientUpdateVlanConfigMessage::getCStructureForInputs ()
    {
        igmps_vlan_config_msg_t *pInput = (igmps_vlan_config_msg_t *)malloc (sizeof (struct igmps_vlan_config_msg_));

        pInput->lmqi            = m_lmqi;
        pInput->qmrt            = m_qmrt;
        pInput->enable          = m_enable;
        pInput->fast_leave      = m_fastLeave;
        pInput->query_interval  = m_queryInterval;
        pInput->mrouter_timeout = m_mrouterTimeout;
        pInput->qenable         = m_querierEnable;
        pInput->opcode          = m_opCode;

		pInput->vlan_id         = m_vlanId;
		pInput->vlan_if_index   = m_vlanIfIndex;
        strncpy (pInput->vlan_if_name, m_vlanIfName.c_str(), IGMPS_MAX_STR);

		return (pInput);
	}

	void IgmpsClientUpdateVlanConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
