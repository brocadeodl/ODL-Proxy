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
 *   .Author : sanjeevj                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmLldpIntfConfSetMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/InterfaceCommon.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    NsmLldpIntfConfSetMessage::NsmLldpIntfConfSetMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMLLDPINTFCONFSET)
    {
		m_cmdcode = 0;
		m_dcbxVersion = LLDP_DCBX_VERSION_AUTO;
		m_disable = false;
		m_ifIndex = 0;
		m_iscsiPriority = DEFAULT_ISCSI_PRIORITY;
		m_mgmtFlags = 0;
		m_msgType = 0;
		m_ifType = IF_TYPE_INVALID;
		m_lldpCeeType = LLDP_CEE_OFF;
    }

    NsmLldpIntfConfSetMessage::NsmLldpIntfConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &id,const string &ifName,const UI64 &ifIndex,const UI8 &dcbxVersion,const bool &disable,const string &profile,const UI32 &iscsiPriority)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMLLDPINTFCONFSET),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_id    (id),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_dcbxVersion    (dcbxVersion),
        m_disable    (disable),
        m_profile    (profile),
        m_iscsiPriority    (iscsiPriority),
		m_lldpCeeType (LLDP_CEE_OFF)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    NsmLldpIntfConfSetMessage::~NsmLldpIntfConfSetMessage ()
    {
    }

    void  NsmLldpIntfConfSetMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI8(&m_dcbxVersion,"dcbxVersion"));
        addSerializableAttribute (new AttributeBool(&m_disable,"disable"));
        addSerializableAttribute (new AttributeString(&m_profile,"profile"));
        addSerializableAttribute (new AttributeUI32(&m_iscsiPriority,"iscsiPriority"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
 		addSerializableAttribute (new AttributeEnum((UI32*)(&m_lldpCeeType),"lldpCeeType"));
    }

    void  NsmLldpIntfConfSetMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmLldpIntfConfSetMessage::getIfType() const
    {
        return (m_ifType);
    }

    void NsmLldpIntfConfSetMessage::setLldpCeeStatus(const lldp_cee_on_off_t &lldpCeeType)
    {
        m_lldpCeeType = lldpCeeType;
    }

    lldp_cee_on_off_t NsmLldpIntfConfSetMessage::getLldpCeeStatus() const
    {
        return m_lldpCeeType;
    }
    

    void  NsmLldpIntfConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  NsmLldpIntfConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  NsmLldpIntfConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  NsmLldpIntfConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  NsmLldpIntfConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  NsmLldpIntfConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  NsmLldpIntfConfSetMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  NsmLldpIntfConfSetMessage::getId() const
    {
        return (m_id);
    }

    void  NsmLldpIntfConfSetMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmLldpIntfConfSetMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmLldpIntfConfSetMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  NsmLldpIntfConfSetMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  NsmLldpIntfConfSetMessage::setDcbxVersion(const UI8 &dcbxVersion)
    {
        m_dcbxVersion  =  dcbxVersion;
    }

    UI8  NsmLldpIntfConfSetMessage::getDcbxVersion() const
    {
        return (m_dcbxVersion);
    }

    void  NsmLldpIntfConfSetMessage::setDisable(const bool &disable)
    {
        m_disable  =  disable;
    }

    bool  NsmLldpIntfConfSetMessage::getDisable() const
    {
        return (m_disable);
    }

    void  NsmLldpIntfConfSetMessage::setProfile(const string &profile)
    {
        m_profile  =  profile;
    }

    string  NsmLldpIntfConfSetMessage::getProfile() const
    {
        return (m_profile);
    }
    
    void  NsmLldpIntfConfSetMessage::setIscsiPriority(const UI32 &iscsiPriority)
    {
        m_iscsiPriority  =  iscsiPriority;
    }

    UI32  NsmLldpIntfConfSetMessage::getIscsiPriority() const
    {
        return (m_iscsiPriority);
    }

}
