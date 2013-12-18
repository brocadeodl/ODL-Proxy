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
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Rtm/Local/RtmTypes.h"
#include "RtmClientStaticRouteMessage.h"
#include "RtmMessageDef.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"
namespace DcmNs
{

    RtmClientStaticRouteMessage::RtmClientStaticRouteMessage ()
        : DcmManagementInterfaceMessage (RTMSTATICROUTE)
    {
    }

    RtmClientStaticRouteMessage::RtmClientStaticRouteMessage (RtmUpdateStaticRouteMessage& msg)
        : DcmManagementInterfaceMessage (RTMSTATICROUTE)
    {
        m_dest = msg.m_dest;
        m_nhIp = msg.m_nhIp;
        m_ifType = msg.m_ifType;
        m_ifName = msg.m_ifName;
        m_nextHopVrf = msg.getNextHopVrf();

        if (msg.m_cost.getIsUserConfigured()) {
            m_cost = msg.m_cost.getUI32Value();
        } else {
            m_cost = RTM_STATIC_ROUTE_DEFAULT_METRIC;
        }

        if (msg.m_distance.getIsUserConfigured()) {
            m_distance = msg.m_distance.getUI32Value();
        } else {
            m_distance = RTM_STATIC_ROUTE_DEFAULT_DISTANCE;
        }

        if (msg.m_tag.getIsUserConfigured()) {
            m_tag = msg.m_tag.getUI32Value();
        } else {
            m_tag = RTM_STATIC_ROUTE_DEFAULT_TAG;
        }

        m_opCode = msg.m_opCode;
    }

    void RtmClientStaticRouteMessage::setStaticRoute (IfStaticRouteLocalManagedObject *pMo)
    {
        m_dest = pMo->m_dest;
        m_ifType = pMo->m_ifType;
        m_ifName = pMo->m_ifName;
		m_nhIp.fromString("127.0.0.1");

        if (pMo->m_cost.getIsUserConfigured()) {
            m_cost = pMo->m_cost.getUI32Value();
        } else {
            m_cost = RTM_STATIC_ROUTE_DEFAULT_METRIC;
        }

        if (pMo->m_distance.getIsUserConfigured()) {
            m_distance = pMo->m_distance.getUI32Value();
        } else {
            m_distance = RTM_STATIC_ROUTE_DEFAULT_DISTANCE;
        }

        if (pMo->m_tag.getIsUserConfigured()) {
            m_tag = pMo->m_tag.getUI32Value();
        } else {
            m_tag = RTM_STATIC_ROUTE_DEFAULT_TAG;
        }

        m_opCode = RTM_MSG_IF_STATIC_ROUTE_SET;
    }

    void RtmClientStaticRouteMessage::setStaticRoute (NhStaticRouteLocalManagedObject *pMo)
    {
        m_dest = pMo->m_dest;
        m_nhIp = pMo->m_nhIp;
        m_ifType = 0;
        m_ifName = "";

        if (pMo->m_cost.getIsUserConfigured()) {
            m_cost = pMo->m_cost.getUI32Value();
        } else {
            m_cost = RTM_STATIC_ROUTE_DEFAULT_METRIC;
        }

        if (pMo->m_distance.getIsUserConfigured()) {
            m_distance = pMo->m_distance.getUI32Value();
        } else {
            m_distance = RTM_STATIC_ROUTE_DEFAULT_DISTANCE;
        }

        if (pMo->m_tag.getIsUserConfigured()) {
            m_tag = pMo->m_tag.getUI32Value();
        } else {
            m_tag = RTM_STATIC_ROUTE_DEFAULT_TAG;
        }

        m_opCode = RTM_MSG_NH_STATIC_ROUTE_SET;
    }

    RtmClientStaticRouteMessage::~RtmClientStaticRouteMessage ()
    {
    }

    void  RtmClientStaticRouteMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_dest,"dest"));
        addSerializableAttribute (new AttributeIpV4Address(&m_nhIp,"nhIp"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_cost,"cost",false,false));
        addSerializableAttribute (new AttributeUI32(&m_distance,"distance",false,false));
        addSerializableAttribute (new AttributeUI32(&m_tag,"tag",false,false));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeString(&m_nextHopVrf, "nextHopVrf"));
    }

    const void * RtmClientStaticRouteMessage::getCStructureForInputs() 
    {
        string twoTupleName = "";
        static const char *ifType[] = 
        {
            "",
            "tengigabitethernet",
            "gigabitethernet",
            "port-channel",
            "vlan",
            "null",
            "fortygigabitethernet"
        };
        rtm_static_route_msg_t *rtm_config = new rtm_static_route_msg_t;
        memset(rtm_config, 0, sizeof(rtm_static_route_msg_t));

		rtm_config->opCode = m_opCode;

		strncpy(rtm_config->dest, m_dest.toString().c_str(), RTM_IP_STRING_LEN);
        strncpy(rtm_config->vrfCtxt, getPartitionName().c_str(), RTM_VRF_NAME_STRING_LEN);
        rtm_config->vrfCtxt[RTM_VRF_NAME_STRING_LEN] = '\0';

		if (m_opCode == RTM_MSG_NH_STATIC_ROUTE_SET || 
			m_opCode == RTM_MSG_NH_STATIC_ROUTE_UNSET) {
			strncpy(rtm_config->nhIp, m_nhIp.toString().c_str(), RTM_IP_STRING_LEN);
		} else 
        if(m_opCode == RTM_MSG_NH_VRF_SET ||
             m_opCode == RTM_MSG_NH_VRF_UNSET) {
			      strncpy(rtm_config->nhIp, m_nhIp.toString().c_str(), RTM_IP_STRING_LEN);
            strncpy(rtm_config->nextHopVrf, m_nextHopVrf.c_str(), RTM_VRF_NAME_STRING_LEN);
            rtm_config->nextHopVrf[RTM_VRF_NAME_STRING_LEN] = '\0';
        }else {
			rtm_config->ifType = ifType[m_ifType];
            if (NsmUtils::isValidThreeTuple(m_ifName)) {
                NsmUtils::getTwoTupleIfName(m_ifName, twoTupleName);
                m_ifName = twoTupleName;
            }
			rtm_config->ifName = m_ifName.c_str();
		}

		rtm_config->cost = m_cost;
        rtm_config->distance = m_distance;
        rtm_config->tag = m_tag;

        return (rtm_config);
    }

    void RtmClientStaticRouteMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
