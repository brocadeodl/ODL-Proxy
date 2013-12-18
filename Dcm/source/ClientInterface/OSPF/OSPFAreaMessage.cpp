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
 **************************************************************************/
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/OSPF/OSPFAreaMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"

namespace DcmNs
{

    OSPFAreaMessage::OSPFAreaMessage ()
        : DcmManagementInterfaceMessage (OSPFAREA)
    {
		m_cmdCodeBitmap = 0;
		m_vrf = DEFAULT_VRF_NAME;
    }

    OSPFAreaMessage::~OSPFAreaMessage ()
    {
    }

    void  OSPFAreaMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmd, "cmd"));
        addSerializableAttribute (new AttributeString(&m_areaId, "areaId"));
        addSerializableAttribute (new AttributeUI32(&m_nssa,"nssa"));
        addSerializableAttribute (new AttributeUI32(&m_stub,"stub"));

        addSerializableAttribute (new AttributeString(&m_vrf, "vrf"));

        //Attrubutes for Virtual Link cmd
        addSerializableAttribute (new AttributeIpV4Address(&m_virtualLinkRouterId, "virtualLinkRouterId"));
        addSerializableAttribute (new AttributeUI32(&m_opCode, "opCode"));
        addSerializableAttribute (new AttributeUI32(&m_HelloInterval, "HelloInterval"));
        addSerializableAttribute (new AttributeUI32(&m_RouterDeadInterval, "RouterDeadInterval"));
        addSerializableAttribute (new AttributeUI32(&m_RetransmitInterval, "RetransmitInterval"));
        addSerializableAttribute (new AttributeUI32(&m_TransDelay, "TransDelay"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_AuthType), "AuthType"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_EncryptionType), "EncryptionType"));
        addSerializableAttribute (new AttributeString(&m_AuthKey, "AuthKey"));
        addSerializableAttribute (new AttributeUI32(&m_MD5KeyActWaitTime, "MD5KeyActWaitTime"));
        addSerializableAttribute (new AttributeUI8(&m_MD5KeyId, "MD5KeyId"));
        addSerializableAttribute (new AttributeString(&m_MD5Key, "MD5Key"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCodeBitmap, "cmdCodeBitmap"));
    }

    void  OSPFAreaMessage::setCmdCode(const UI32 &cmd)
    {
        m_cmd  =  cmd;
    }

    UI32  OSPFAreaMessage::getCmdCode() const
    {
        return (m_cmd);
    }

	void OSPFAreaMessage::setVRF(const string &vrf)
	{
		m_vrf = vrf;
	}

	string OSPFAreaMessage::getVRF() const
	{
		return (m_vrf);
	}

    void  OSPFAreaMessage::setAreaId(const string &areaId)
    {
        m_areaId  =  areaId;
    }

    string  OSPFAreaMessage::getAreaId() const
    {
        return (m_areaId);
    }

    void OSPFAreaMessage::setNssa(const unsigned int &nssa)
    {
        m_nssa = nssa;
    }

    unsigned int OSPFAreaMessage::getNssa()  const
    {
        return (m_nssa);
    }

    void OSPFAreaMessage::setStub(const unsigned int &stub)
    {
        m_stub = stub;
    }

    unsigned int OSPFAreaMessage::getStub()  const
    {
        return (m_stub);
    }

    void OSPFAreaMessage::setVirtualLinkRouterId(const IpV4Address &virtualLinkRouterId)
    {
        m_virtualLinkRouterId = virtualLinkRouterId;
    }

    IpV4Address OSPFAreaMessage::getVirtualLinkRouterId()  const
    {
        return (m_virtualLinkRouterId);
    }

    void  OSPFAreaMessage::setOpCode(const unsigned int &opCode)
    {
        m_opCode  =  opCode;
    }

    unsigned int OSPFAreaMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  OSPFAreaMessage::setHelloInterval(const UI32 &HelloInterval)
    {
        m_HelloInterval  =  HelloInterval;
    }

    UI32  OSPFAreaMessage::getHelloInterval() const
    {
        return (m_HelloInterval);
    }

    void  OSPFAreaMessage::setRouterDeadInterval(const UI32 &RouterDeadInterval)
    {
        m_RouterDeadInterval  =  RouterDeadInterval;
    }

    UI32  OSPFAreaMessage::getRouterDeadInterval() const
    {
        return (m_RouterDeadInterval);
    }

    void  OSPFAreaMessage::setRetransmitInterval(const UI32 &RetransmitInterval)
    {
        m_RetransmitInterval  =  RetransmitInterval;
    }

    UI32  OSPFAreaMessage::getRetransmitInterval() const
    {
        return (m_RetransmitInterval);
    }

    void  OSPFAreaMessage::setTransDelay(const UI32 &TransDelay)
    {
        m_TransDelay  =  TransDelay;
    }

    UI32  OSPFAreaMessage::getTransDelay() const
    {
        return (m_TransDelay);
    }

    void  OSPFAreaMessage::setAuthType(const OSPFEncrypType &AuthType)
    {
        m_AuthType  =  AuthType;
    }

    OSPFEncrypType OSPFAreaMessage::getAuthType() const
    {
        return (m_AuthType);
    }

    void  OSPFAreaMessage::setEncryptionType(const OSPFEncrypType &EncryptionType)
    {
        m_EncryptionType  =  EncryptionType;
    }

    OSPFEncrypType  OSPFAreaMessage::getEncryptionType() const
    {
        return (m_EncryptionType);
    }

    void  OSPFAreaMessage::setAuthKey(const string &AuthKey)
    {
        m_AuthKey  =  AuthKey;
    }

    string  OSPFAreaMessage::getAuthKey() const
    {
        return (m_AuthKey);
    }

    void  OSPFAreaMessage::setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime)
    {
        m_MD5KeyActWaitTime  =  MD5KeyActWaitTime;
    }

    UI32  OSPFAreaMessage::getMD5KeyActWaitTime() const
    {
        return (m_MD5KeyActWaitTime);
    }

    void  OSPFAreaMessage::setMD5KeyId(const UI8 &MD5KeyId)
    {
        m_MD5KeyId  =  MD5KeyId;
    }

    UI8  OSPFAreaMessage::getMD5KeyId() const
    {
        return (m_MD5KeyId);
    }

    void  OSPFAreaMessage::setMD5Key(const string &MD5Key)
    {
        m_MD5Key  =  MD5Key;
    }

    string OSPFAreaMessage::getMD5Key() const
    {
        return (m_MD5Key);
    }

	UI64 OSPFAreaMessage::getReplayCmdCodeBitmap() const
	{
		return (m_cmdCodeBitmap);
	}

	void OSPFAreaMessage::setReplayCmdCodeBitmap(const UI32 &cmdCode)
	{
		m_cmdCodeBitmap |= (1L << cmdCode);
	}

    void OSPFAreaMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
    }

    // Convert info from C++ struct OSPFAreaMessage populated in the APWorker to a C struct for backend
    const void *OSPFAreaMessage::getCStructureForInputs ()
    {
        ospf_area_config_in_msg_t  *msg = new ospf_area_config_in_msg_t;
		
        strncpy(msg->area_id, m_areaId.c_str(), OSPF_IPV4_ADDR_STR_SIZE);
		msg->cmdCode= (ospf_area_config_cmd_code_t)m_cmd;
		msg->opCode = m_opCode;
		msg->nssa   = m_nssa;
		msg->stub   = m_stub;
		strcpy(msg->m_vrf ,m_vrf.c_str());

		if(m_virtualLinkRouterId.toString() != string("0.0.0.0"))
            strcpy(msg->vlink_routerid ,(m_virtualLinkRouterId.toString()).c_str());

		msg->vlink_hello_interval = m_HelloInterval;
		msg->vlink_dead_interval  = m_RouterDeadInterval;
		msg->vlink_retransmit_interval = m_RetransmitInterval;
		msg->vlink_transmit_delay = m_TransDelay;

		if (m_AuthType == OSPF_INVALID_ENCR) {
			msg->vlink_auth_type = -1;
		} else {
			msg->vlink_auth_type = (int)m_AuthType;
		}

		if (m_EncryptionType == OSPF_INVALID_ENCR) {
			msg->vlink_encr_type = -1;
		} else {
			msg->vlink_encr_type = (int) m_EncryptionType;
		}

		strncpy(msg->vlink_auth_key, m_AuthKey.c_str(), OSPF_AUTH_KEY_SIZE);
		msg->vlink_md5_key_act_wait_time = m_MD5KeyActWaitTime;
		msg->vlink_md5_key_id = m_MD5KeyId;
		strncpy(msg->vlink_md5_key, m_MD5Key.c_str(), OSPF_AUTH_KEY_SIZE);
		msg->cmd_code_bitmap = m_cmdCodeBitmap;

        return msg;
    }

    void OSPFAreaMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        if (!pOutputCStructure)
            return;

        ospf_area_config_out_msg_t *msg = ((ospf_area_config_out_msg_t *)pOutputCStructure);
        addBuffer (OSPFAREA, msg->size, pOutputCStructure, false);
    }
}

