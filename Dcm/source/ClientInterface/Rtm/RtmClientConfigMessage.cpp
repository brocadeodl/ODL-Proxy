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
 *   Author : hzhu                                                     *
 **************************************************************************/

#include "RtmClientConfigMessage.h"
#include "Rtm/Local/RtmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "RtmMessageDef.h"
#include "Vrf/Local/VrfLocalMessages.h"

namespace DcmNs
{

    RtmClientConfigMessage::RtmClientConfigMessage ()
        : DcmManagementInterfaceMessage (RTMRTMCONFIG)
    {
        m_cfgBitMask = 0;
    }

    RtmClientConfigMessage::RtmClientConfigMessage (RtmUpdateRtmConfigMessage& msg)
        : DcmManagementInterfaceMessage (RTMRTMCONFIG)
    {
        m_nhProto = msg.m_nhProto;
        m_nhDefault = msg.m_nhDefault;
        m_nhRecur = msg.m_nhRecur;
        m_loadSharing = msg.m_loadSharing;
        m_routerID = msg.m_routerID;
        m_opCode = msg.m_opCode;
        m_vrf = msg.getPartitionName();
    }


    RtmClientConfigMessage::~RtmClientConfigMessage ()
    {
    }

    RtmClientConfigMessage::RtmClientConfigMessage (VrfLocalAddVrfMessage *msg)
        : DcmManagementInterfaceMessage (RTMRTMCONFIG)
    {
        m_vrf = msg->m_vrfName;
        m_opCode = msg->m_cmdCode;
        if(msg->m_cmdCode == RTM_MSG_CONFIG_ROUTER_ID_SET || 
            msg->m_cmdCode == RTM_MSG_CONFIG_ROUTER_ID_UNSET) {
            m_routerID = msg->m_ip;
            m_cfgBitMask |= RTM_MSG_CONFIG_ROUTER_ID_BIT;
        }
        else if(msg->m_cmdCode == RTM_MSG_AF_IPV4_SET ||
                msg->m_cmdCode == RTM_MSG_AF_IPV4_UNSET)  {
            m_vrfafIpv4Flag = msg->m_vrfIpv4Flag;
            m_maxRoutes = msg->m_maxRoutes;
        }
        else if(msg->m_cmdCode == RTM_MSG_CONFIG_MAX_ROUTES_SET) { 
            m_maxRoutes = msg->m_maxRoutes;
            m_opCode = RTM_MSG_AF_IPV4_SET;
        }
        else if( msg->m_cmdCode == RTM_MSG_CONFIG_MAX_ROUTES_UNSET)  {
            m_maxRoutes = msg->m_maxRoutes;
        }
        else if(msg->m_cmdCode == RTM_MSG_RD_SET ||
                msg->m_cmdCode == RTM_MSG_RD_UNSET) {
            m_rd = msg->m_rd;
        }
        else if(msg->m_cmdCode == RTM_MSG_CONFIG_VRF_MODE_RT_SET ||
                msg->m_cmdCode == RTM_MSG_CONFIG_VRF_MODE_RT_UNSET ||
                msg->m_cmdCode == RTM_MSG_CONFIG_VRF_AF_IPV4_MODE_RT_SET||
                msg->m_cmdCode == RTM_MSG_CONFIG_VRF_AF_IPV4_MODE_RT_UNSET) {
            m_rt = msg->m_rt;
            m_rtType = msg->m_rtType;
            m_vrfafIpv4Flag = msg->m_vrfIpv4Flag;
        }
    }


    void RtmClientConfigMessage::setRtmConfig (RtmConfigLocalManagedObject* pMo, string vrfName)
    {
        m_vrf = vrfName; 
        if (pMo && pMo->m_nhDefault) {
            m_nhDefault = pMo->m_nhDefault;
            m_cfgBitMask |= RTM_MSG_CONFIG_ENABLEDEFAULT_BIT;
        } 

        if (pMo && pMo->m_nhRecur.getIsUserConfigured()) {
            m_nhRecur = pMo->m_nhRecur.getUI32Value();   
            m_cfgBitMask |= RTM_MSG_CONFIG_NEXTHOPRECURSION_BIT;
        } 

        if (pMo && pMo->m_loadSharing.getIsUserConfigured()) {
            m_loadSharing = pMo->m_loadSharing.getUI32Value();
            m_cfgBitMask |= RTM_MSG_CONFIG_LOADSHARING_BIT;
        }

        if (pMo && pMo->m_routerID.getIsUserConfigured())
		{
			m_routerID = pMo->m_routerID.getIpV4AddressValue();
            m_cfgBitMask |= RTM_MSG_CONFIG_ROUTER_ID_BIT;
		}

        m_opCode = RTM_MSG_ALL_CONFIG_SET;
    }

    void RtmClientConfigMessage::setRtmConfig (RtmNhProtoLocalManagedObject* pMo, string vrfName)
    {
        m_vrf = vrfName;
        if (pMo) {
            m_cfgBitMask |= RTM_MSG_CONFIG_NEXTHOP_BIT;
            m_nhProto = pMo->m_nhProto;
        }
        m_opCode = RTM_MSG_ALL_CONFIG_SET;
    }

    void  RtmClientConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_nhProto,"nhProto"));
        addSerializableAttribute (new AttributeBool(&m_nhDefault,"nhDefault"));
        addSerializableAttribute (new AttributeUI32(&m_nhRecur,"nhRecurr"));
        addSerializableAttribute (new AttributeUI32(&m_loadSharing,"loadSharing"));
        addSerializableAttribute (new AttributeIpV4Address(&m_routerID,"routerID"));
        addSerializableAttribute (new AttributeUI32(&m_cfgBitMask,"cfgBitMask"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeString(&m_rd,"rd"));
        addSerializableAttribute (new AttributeString(&m_rt,"rt"));
        addSerializableAttribute (new AttributeUI32(&m_rtType,"rfType"));
        addSerializableAttribute (new AttributeBool(&m_vrfafIpv4Flag, "afIpv4Flag"));
        addSerializableAttribute (new AttributeUI32(&m_maxRoutes,"maxRoutes"));
        addSerializableAttribute (new AttributeString(&m_vrf,"currentVrf")); /*current vrf mode*/
    }

    const void * RtmClientConfigMessage::getCStructureForInputs() 
    {
        rtm_config_msg_t *rtm_config = new rtm_config_msg_t;
        memset(rtm_config, 0, sizeof(rtm_config_msg_t));

        rtm_config->opCode = m_opCode;
        rtm_config->cfgBitMask = m_cfgBitMask;

        rtm_config->nhDefault = m_nhDefault;
		rtm_config->nhRecur = m_nhRecur;
        strncpy(rtm_config->routerID, m_routerID.toString().c_str(), RTM_IP_STRING_LEN);
        rtm_config->loadSharing = m_loadSharing;
		rtm_config->nhProto = m_nhProto;
        if(m_opCode == RTM_MSG_AF_IPV4_SET ||
            m_opCode == RTM_MSG_AF_IPV4_UNSET ||
            m_opCode == RTM_MSG_CONFIG_MAX_ROUTES_SET ||
            m_opCode == RTM_MSG_CONFIG_MAX_ROUTES_UNSET)
        {
            rtm_config->afIpv4Mode = true;

        }
        snprintf(rtm_config->vrfName, RTM_VRF_NAME_STRING_LEN, "%s", m_vrf.c_str());
        snprintf(rtm_config->vrfRdRt, RTM_RD_RT_MAX_LEN, "%s", m_rd.c_str());
        snprintf(rtm_config->vrfRt, RTM_RD_RT_MAX_LEN, "%s", m_rt.c_str());
        rtm_config->vrfRtType = m_rtType;
        rtm_config->maxRoutes = m_maxRoutes;

        return (rtm_config);
    }

    void RtmClientConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}

