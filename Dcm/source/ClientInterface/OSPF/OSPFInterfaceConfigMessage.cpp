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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "ClientInterface/OSPF/OSPFInterfaceConfigMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{
			
    OSPFInterfaceConfigMessage::OSPFInterfaceConfigMessage ()
        : DcmManagementInterfaceMessage (OSPFINTERFACECONFIG)
    {
		m_replayCmdCodeBitmap = 0;
    }

    OSPFInterfaceConfigMessage::OSPFInterfaceConfigMessage (const UI8 &opCode,const UI8 &cmdCode,const string &intfName,const UI32 &intfType,const string &areaId,const UI32 &HelloInterval,const UI32 &RouterDeadInterval,const UI32 &RetransmitInterval,const UI32 &TransDelay,const UI32 &Cost,const UI32 &Priority,const UI32 &AuthType,const OSPFEncrypType &EncryptionType,const OSPFEncrypType &MD5EncryptionType,const string &AuthKey,const UI32 &AuthChangeWaitTime,const UI32 &MD5KeyActWaitTime,const UI32 &MD5KeyId,const string &MD5Key,const bool &FilterAll,const UI32 &FilterOption,const bool &mtuIgnore,const bool &Active,const bool &Passive,const UI32 &NetworkType)
        : DcmManagementInterfaceMessage (OSPFINTERFACECONFIG),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
		m_intfName  (intfName),
		m_intfType  (intfType),
        m_areaId    (areaId),
        m_HelloInterval    (HelloInterval),
        m_RouterDeadInterval    (RouterDeadInterval),
        m_RetransmitInterval    (RetransmitInterval),
        m_TransDelay    (TransDelay),
        m_Cost    (Cost),
        m_Priority    (Priority),
        m_AuthType    (AuthType),
        m_EncryptionType    (EncryptionType),
        m_MD5EncryptionType    (MD5EncryptionType),
        m_AuthKey    (AuthKey),
        m_AuthChangeWaitTime    (AuthChangeWaitTime),
        m_MD5KeyActWaitTime    (MD5KeyActWaitTime),
        m_MD5KeyId    (MD5KeyId),
        m_MD5Key    (MD5Key),
        m_FilterAll    (FilterAll),
        m_FilterOption    (FilterOption),
        m_mtuIgnore    (mtuIgnore),
        m_Active    (Active),
        m_Passive    (Passive),
        m_NetworkType    (NetworkType)
    {
    }

    OSPFInterfaceConfigMessage::~OSPFInterfaceConfigMessage ()
    {
    }

    void  OSPFInterfaceConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_intfName,"intfName"));
        addSerializableAttribute (new AttributeUI32(&m_intfType,"intfType"));
        addSerializableAttribute (new AttributeString(&m_areaId,"areaId"));
        addSerializableAttribute (new AttributeUI32(&m_HelloInterval,"HelloInterval"));
        addSerializableAttribute (new AttributeUI32(&m_RouterDeadInterval,"RouterDeadInterval"));
        addSerializableAttribute (new AttributeUI32(&m_RetransmitInterval,"RetransmitInterval"));
        addSerializableAttribute (new AttributeUI32(&m_TransDelay,"TransDelay"));
        addSerializableAttribute (new AttributeUI32(&m_Cost,"Cost"));
        addSerializableAttribute (new AttributeUI32(&m_Priority,"Priority"));
        addSerializableAttribute (new AttributeUI32(&m_AuthType,"AuthType"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_EncryptionType,"EncryptionType"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_MD5EncryptionType,"MD5EncryptionType"));
        addSerializableAttribute (new AttributeString(&m_AuthKey,"AuthKey"));
        addSerializableAttribute (new AttributeUI32(&m_AuthChangeWaitTime,"AuthChangeWaitTime"));
        addSerializableAttribute (new AttributeUI32(&m_MD5KeyActWaitTime,"MD5KeyActWaitTime"));
        addSerializableAttribute (new AttributeUI32(&m_MD5KeyId,"MD5KeyId"));
        addSerializableAttribute (new AttributeString(&m_MD5Key,"MD5Key"));
        addSerializableAttribute (new AttributeBool(&m_FilterAll,"FilterAll"));
        addSerializableAttribute (new AttributeUI32(&m_FilterOption,"FilterOption"));
        addSerializableAttribute (new AttributeBool(&m_mtuIgnore,"mtuIgnore"));
        addSerializableAttribute (new AttributeBool(&m_Active,"Active"));
        addSerializableAttribute (new AttributeBool(&m_Passive,"Passive"));
        addSerializableAttribute (new AttributeUI32(&m_NetworkType,"NetworkType"));
		addSerializableAttribute (new AttributeUI32(&m_replayCmdCodeBitmap, "replayCmdCodeBitmap"));
		addSerializableAttribute (new AttributeUI64(&m_if_index, "ifindex"));
    }

    void  OSPFInterfaceConfigMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  OSPFInterfaceConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  OSPFInterfaceConfigMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  OSPFInterfaceConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  OSPFInterfaceConfigMessage::setintfName(const string &intfName)
    {
        m_intfName  =  intfName;
    }

    string  OSPFInterfaceConfigMessage::getintfName() const
    {
        return (m_intfName);
    }

    void OSPFInterfaceConfigMessage::setIfindex (const UI64 &ifindex)
    {
        m_if_index = ifindex;
    }

    UI64 OSPFInterfaceConfigMessage::getIfindex () const
    {
        return m_if_index;
    }
	
    void  OSPFInterfaceConfigMessage::setintfType(const UI32 &intfType)
    {
        m_intfType  =  intfType;
    }

    UI32  OSPFInterfaceConfigMessage::getintfType() const
    {
        return (m_intfType);
    }
	
    void  OSPFInterfaceConfigMessage::setAreaId(const string &areaId)
    {
        m_areaId  =  areaId;
    }

    string  OSPFInterfaceConfigMessage::getAreaId() const
    {
        return (m_areaId);
    }

    void  OSPFInterfaceConfigMessage::setHelloInterval(const UI32 &HelloInterval)
    {
        m_HelloInterval  =  HelloInterval;
    }

    UI32  OSPFInterfaceConfigMessage::getHelloInterval() const
    {
        return (m_HelloInterval);
    }

    void  OSPFInterfaceConfigMessage::setRouterDeadInterval(const UI32 &RouterDeadInterval)
    {
        m_RouterDeadInterval  =  RouterDeadInterval;
    }

    UI32  OSPFInterfaceConfigMessage::getRouterDeadInterval() const
    {
        return (m_RouterDeadInterval);
    }

    void  OSPFInterfaceConfigMessage::setRetransmitInterval(const UI32 &RetransmitInterval)
    {
        m_RetransmitInterval  =  RetransmitInterval;
    }

    UI32  OSPFInterfaceConfigMessage::getRetransmitInterval() const
    {
        return (m_RetransmitInterval);
    }

    void  OSPFInterfaceConfigMessage::setTransDelay(const UI32 &TransDelay)
    {
        m_TransDelay  =  TransDelay;
    }

    UI32  OSPFInterfaceConfigMessage::getTransDelay() const
    {
        return (m_TransDelay);
    }

    void  OSPFInterfaceConfigMessage::setCost(const UI32 &Cost)
    {
        m_Cost  =  Cost;
    }

    UI32  OSPFInterfaceConfigMessage::getCost() const
    {
        return (m_Cost);
    }

    void  OSPFInterfaceConfigMessage::setPriority(const UI32 &Priority)
    {
        m_Priority  =  Priority;
    }

    UI32  OSPFInterfaceConfigMessage::getPriority() const
    {
        return (m_Priority);
    }

    void  OSPFInterfaceConfigMessage::setAuthType(const UI32 &AuthType)
    {
        m_AuthType  =  AuthType;
    }

    UI32  OSPFInterfaceConfigMessage::getAuthType() const
    {
        return (m_AuthType);
    }

    void  OSPFInterfaceConfigMessage::setEncryptionType(const OSPFEncrypType &EncryptionType)
    {
        m_EncryptionType  =  EncryptionType;
    }

    OSPFEncrypType OSPFInterfaceConfigMessage::getEncryptionType() const
    {
        return (m_EncryptionType);
    }

    void  OSPFInterfaceConfigMessage::setMD5EncryptionType(const OSPFEncrypType &MD5EncryptionType)
    {
        m_MD5EncryptionType  =  MD5EncryptionType;
    }

    OSPFEncrypType OSPFInterfaceConfigMessage::getMD5EncryptionType() const
    {
        return (m_MD5EncryptionType);
    }

    void  OSPFInterfaceConfigMessage::setAuthKey(const string &AuthKey)
    {
        m_AuthKey  =  AuthKey;
    }

    string  OSPFInterfaceConfigMessage::getAuthKey() const
    {
        return (m_AuthKey);
    }

    void  OSPFInterfaceConfigMessage::setAuthChangeWaitTime(const UI32 &AuthChangeWaitTime)
    {
        m_AuthChangeWaitTime  =  AuthChangeWaitTime;
    }

    UI32  OSPFInterfaceConfigMessage::getAuthChangeWaitTime() const
    {
        return (m_AuthChangeWaitTime);
    }

    void  OSPFInterfaceConfigMessage::setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime)
    {
        m_MD5KeyActWaitTime  =  MD5KeyActWaitTime;
    }

    UI32  OSPFInterfaceConfigMessage::getMD5KeyActWaitTime() const
    {
        return (m_MD5KeyActWaitTime);
    }

    void  OSPFInterfaceConfigMessage::setMD5KeyId(const UI32 &MD5KeyId)
    {
        m_MD5KeyId  =  MD5KeyId;
    }

    UI32  OSPFInterfaceConfigMessage::getMD5KeyId() const
    {
        return (m_MD5KeyId);
    }

    void  OSPFInterfaceConfigMessage::setMD5Key(const string &MD5Key)
    {
        m_MD5Key  =  MD5Key;
    }

    string  OSPFInterfaceConfigMessage::getMD5Key() const
    {
        return (m_MD5Key);
    }

    void  OSPFInterfaceConfigMessage::setFilterAll(const bool &FilterAll)
    {
        m_FilterAll  =  FilterAll;
    }

    bool  OSPFInterfaceConfigMessage::getFilterAll() const
    {
        return (m_FilterAll);
    }

    void  OSPFInterfaceConfigMessage::setFilterOption(const UI32 &FilterOption)
    {
        m_FilterOption  =  FilterOption;
    }

    UI32  OSPFInterfaceConfigMessage::getFilterOption() const
    {
        return (m_FilterOption);
    }

    void  OSPFInterfaceConfigMessage::setmtuIgnore(const bool &mtuIgnore)
    {
        m_mtuIgnore  =  mtuIgnore;
    }

    bool  OSPFInterfaceConfigMessage::getmtuIgnore() const
    {
        return (m_mtuIgnore);
    }
	
    void  OSPFInterfaceConfigMessage::setActive(const bool &Active)
    {
        m_Active  =  Active;
    }

    bool  OSPFInterfaceConfigMessage::getActive() const
    {
        return (m_Active);
    }

    void  OSPFInterfaceConfigMessage::setPassive(const bool &Passive)
    {
        m_Passive  =  Passive;
    }

    bool  OSPFInterfaceConfigMessage::getPassive() const
    {
        return (m_Passive);
    }

    void  OSPFInterfaceConfigMessage::setNetworkType(const UI32 &NetworkType)
    {
        m_NetworkType  =  NetworkType;
    }

    UI32 OSPFInterfaceConfigMessage::getNetworkType() const
    {
        return (m_NetworkType);
    }

    UI32 OSPFInterfaceConfigMessage::getReplayCmdCodeBitmap() const
    {
        return (m_replayCmdCodeBitmap);
    }

    void OSPFInterfaceConfigMessage::setReplayCmdCodeBitmap(const OSPFIntfCmdCode &cmdCode)
    {
        m_replayCmdCodeBitmap |= (1 << (UI32)cmdCode);
    }

	void OSPFInterfaceConfigMessage::prepareCMessage(interfaceOSPFConfigMsg_t **pInput)
	{
		switch((*pInput)->cmdCode)
		{
		case OSPF_INTF_AREA:
			{
				strcpy((*pInput)->areaId,m_areaId.c_str());
				break;
			}
		case OSPF_INTF_HELLO:
			{
				(*pInput)->HelloInterval = (int)m_HelloInterval;
				break;
			}
		case OSPF_INTF_DEAD:
			{
				(*pInput)->RouterDeadInterval = (int)m_RouterDeadInterval;
				break;
			}
		case OSPF_INTF_RETRANSMIT:
			{
				(*pInput)->RetransmitInterval = (int)m_RetransmitInterval;
				break;
			}
		case OSPF_INTF_TRANSDELAY:
			{
				(*pInput)->TransDelay = (int)m_TransDelay;
				break;
			}
		case OSPF_INTF_COST:
			{
				(*pInput)->Cost = (int)m_Cost;
				break;
			}
		case OSPF_INTF_PRIORITY:
			{
				(*pInput)->Priority = (int)m_Priority;
				break;
			}
		case OSPF_INTF_AUTH_CHANGE_WAIT_TIME:
			{
				(*pInput)->AuthChangeWaitTime= (int)m_AuthChangeWaitTime;
				break;
			}
		case OSPF_INTF_AUTH_KEY:
			{
				(*pInput)->AuthType = m_AuthType;
				if(m_EncryptionType == OSPF_INVALID_ENCR)
					(*pInput)->EncryptionType= -1;
				else
					(*pInput)->EncryptionType= m_EncryptionType;
				
				strcpy((*pInput)->AuthKey,m_AuthKey.c_str());
				break;
			}
		case OSPF_INTF_MD5_KEY_ACTV_WAIT_TIME:
			{
				(*pInput)->AuthType = m_AuthType;
				(*pInput)->MD5KeyActWaitTime= (int)m_MD5KeyActWaitTime;
				break;
			}
		case OSPF_INTF_MD5_AUTH_KEY:
			{
				(*pInput)->AuthType = m_AuthType;
				(*pInput)->MD5EncryptionType= (int)m_MD5EncryptionType;
				(*pInput)->MD5KeyId= (int)m_MD5KeyId;
				strcpy((*pInput)->MD5Key,m_MD5Key.c_str());
				break;
			}
		case OSPF_INTF_FILTER_ALL:
			{
				(*pInput)->FilterAll= (unsigned short)m_FilterAll;
				break;
			}
		case OSPF_INTF_FILTER_EXT:
		case OSPF_INTF_FILTER_SUMM_EXT:
			{
				(*pInput)->FilterOption = (unsigned short)m_FilterOption;
				break;
			}
		case OSPF_INTF_MTU_IGNORE:
			{
				(*pInput)->mtuIgnore= (unsigned short)m_mtuIgnore;
				break;
			}
		case OSPF_INTF_NETWORK:
			{
				(*pInput)->NetworkType= m_NetworkType;
				break;
			}
		case OSPF_INTF_ACTIVE:
			{
				(*pInput)->Active = m_Active;
				break;
			}
		case OSPF_INTF_PASSIVE:
			{
				(*pInput)->Passive= (unsigned short)m_Passive;
				break;
			}
		default:
				break;
		}
	}

    const void *OSPFInterfaceConfigMessage::getCStructureForInputs ()
    {
        interfaceOSPFConfigMsg_t *pInput = new interfaceOSPFConfigMsg_t;
        string twoTupleName = "";

        pInput->opCode = m_opCode;
        pInput->cmdCode = m_cmdCode;
        pInput->intfType = m_intfType;
        pInput->ifindex = m_if_index;
        
		if (NsmUtils::isValidThreeTuple(m_intfName)) {
            NsmUtils::getTwoTupleIfName(m_intfName, twoTupleName);
        } else {
            twoTupleName = m_intfName;
        }
        strncpy(pInput->intfName, twoTupleName.c_str(), OSPF_DCM_IF_NAME_LEN);

		if(m_opCode == OSPF_CREATE)
		{
			if(m_cmdCode == OSPF_INTF_CONFIG_REPLAY)
			{
				for (UI32 idx = 0; idx < OSPF_INTF_DEL; idx ++) {
					if(m_replayCmdCodeBitmap & (1<<idx))
					{
						pInput->cmdCode = idx;
						prepareCMessage(&pInput);
					}
				}
				pInput->cmdCode = OSPF_INTF_CONFIG_REPLAY;
				pInput->replayCmdCodeBitmap = m_replayCmdCodeBitmap;
			}
			else
			{
				prepareCMessage(&pInput);
				return pInput;
			}
		}
		else
		{
			prepareCMessage(&pInput);
			return pInput;
		}
		return pInput;
    }

    void OSPFInterfaceConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        //trace (TRACE_LEVEL_INFO, "Entering OSPFInterfaceConfigMessage::loadOutputsFromCStructure\n");
        if (!pOutputCStructure) 
            return;

        ospf_intf_config_out_msg_t *msg = ((ospf_intf_config_out_msg_t *)pOutputCStructure);
        addBuffer (OSPFINTERFACECONFIG, msg->size, pOutputCStructure, false);
    }

    void OSPFInterfaceConfigMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
    }

	void OSPFInterfaceConfigMessage::dumpMessage () const
	{
		trace (TRACE_LEVEL_DEVEL, string("m_cmdCode : ") + m_cmdCode);
		trace (TRACE_LEVEL_DEVEL, string("m_intfName : ") + m_intfName);
		trace (TRACE_LEVEL_DEVEL, string("m_intfType : ") + m_intfType);
		trace (TRACE_LEVEL_DEVEL, string("m_areaId : ") + m_areaId);
		trace (TRACE_LEVEL_DEVEL, string("m_HelloInterval : ") + m_HelloInterval);
		trace (TRACE_LEVEL_DEVEL, string("m_RouterDeadInterval : ") + m_RouterDeadInterval);
		trace (TRACE_LEVEL_DEVEL, string("m_RetransmitInterval : ") + m_RetransmitInterval);
		trace (TRACE_LEVEL_DEVEL, string("m_TransDelay : ") + m_TransDelay);
		trace (TRACE_LEVEL_DEVEL, string("m_Cost : ") + m_Cost);
		trace (TRACE_LEVEL_DEVEL, string("m_Priority : ") + m_Priority);
		trace (TRACE_LEVEL_DEVEL, string("m_AuthType : ") + m_AuthType);
		trace (TRACE_LEVEL_DEVEL, string("m_EncryptionType : ") + m_EncryptionType);
		trace (TRACE_LEVEL_DEVEL, string("m_MD5EncryptionType : ") + m_MD5EncryptionType);
		trace (TRACE_LEVEL_DEVEL, string("m_AuthKey : ") + m_AuthKey);
		trace (TRACE_LEVEL_DEVEL, string("m_AuthChangeWaitTime : ") + m_AuthChangeWaitTime);
		trace (TRACE_LEVEL_DEVEL, string("m_MD5KeyActWaitTime : ") + m_MD5KeyActWaitTime);
		trace (TRACE_LEVEL_DEVEL, string("m_MD5KeyId : ") + m_MD5KeyId);
		trace (TRACE_LEVEL_DEVEL, string("m_MD5Key : ") + m_MD5Key);
		trace (TRACE_LEVEL_DEVEL, string("m_FilterAll : ") + m_FilterAll);
		trace (TRACE_LEVEL_DEVEL, string("m_FilterOption : ") + m_FilterOption);
		trace (TRACE_LEVEL_DEVEL, string("m_mtuIgnore : ") + m_mtuIgnore);
		trace (TRACE_LEVEL_DEVEL, string("m_Active : ") + m_Active);
		trace (TRACE_LEVEL_DEVEL, string("m_Passive : ") + m_Passive);
		trace (TRACE_LEVEL_DEVEL, string("m_NetworkType : ") + m_NetworkType);
	}
}
