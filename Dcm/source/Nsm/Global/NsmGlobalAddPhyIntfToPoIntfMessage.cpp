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
 *   Author : skbalasu.                                                     *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalAddPhyIntfToPoIntfMessage.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmGlobalAddPhyIntfToPoIntfMessage::NsmGlobalAddPhyIntfToPoIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALADDPHYINTFTOPOINTF),
		  m_replayPoConfigs (false)
    {
		m_mode = 0;
		m_poCmdCode = 0;
		m_poIndex = 0;
		m_poLacpSystemIdDomainId = 0;
		m_swIfIndex = 0;
		m_type = 0;
        m_ifType = IF_TYPE_INVALID;
    }

    NsmGlobalAddPhyIntfToPoIntfMessage::NsmGlobalAddPhyIntfToPoIntfMessage (const string &ifName,const UI64 &swIfIndex,const string &poName,const UI32 &poIndex,const UI32 &mode,const UI32 &type, const string &poLacpSystemId, const UI32 &poLacpSystemIdDomainId, const bool &replayPoConfigs)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALADDPHYINTFTOPOINTF),
        m_ifName    (ifName),
        m_swIfIndex    (swIfIndex),
        m_poName    (poName),
        m_poIndex    (poIndex),
        m_mode    (mode),
        m_type    (type),
        m_poLacpSystemId    (poLacpSystemId),
        m_poLacpSystemIdDomainId    (poLacpSystemIdDomainId),
		m_replayPoConfigs (replayPoConfigs)
    {
		m_poCmdCode = 0;
        m_ifType = IF_TYPE_INVALID;
    }

    NsmGlobalAddPhyIntfToPoIntfMessage::~NsmGlobalAddPhyIntfToPoIntfMessage ()
    {
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeUI32(&m_poIndex,"poIndex"));
        addSerializableAttribute (new AttributeString(&m_poName,"poName"));
        addSerializableAttribute (new AttributeUI64(&m_swIfIndex,"swIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_type,"type"));
        addSerializableAttribute (new AttributeString(&m_poLacpSystemId,"poLacpSystemId"));
        addSerializableAttribute (new AttributeUI32(&m_poLacpSystemIdDomainId,"poLacpSystemIdDomainId"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }
    void NsmGlobalAddPhyIntfToPoIntfMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32 NsmGlobalAddPhyIntfToPoIntfMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmGlobalAddPhyIntfToPoIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setSwIfIndex(const UI64 &swIfIndex)
    {
        m_swIfIndex  =  swIfIndex;
    }

    UI64  NsmGlobalAddPhyIntfToPoIntfMessage::getSwIfIndex() const
    {
        return (m_swIfIndex);
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmGlobalAddPhyIntfToPoIntfMessage::getPoName() const
    {
        return (m_poName);
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setPoIndex(const UI32 &poIndex)
    {
        m_poIndex  =  poIndex;
    }

    UI32  NsmGlobalAddPhyIntfToPoIntfMessage::getPoIndex() const
    {
        return (m_poIndex);
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setPoMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  NsmGlobalAddPhyIntfToPoIntfMessage::getPoMode() const
    {
        return (m_mode);
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setPoType(const UI32 &type)
    {
        m_type  =  type;
    }

    UI32  NsmGlobalAddPhyIntfToPoIntfMessage::getPoType() const
    {
        return (m_type);
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  NsmGlobalAddPhyIntfToPoIntfMessage::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setPoCmdCode(const UI32 &poCmdCode)
    {
        m_poCmdCode  =  poCmdCode;
    }

    UI32  NsmGlobalAddPhyIntfToPoIntfMessage::getPoCmdCode() const
    {
        return (m_poCmdCode);
    }
    
    void  NsmGlobalAddPhyIntfToPoIntfMessage::setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId)
    {
        m_poLacpSystemIdDomainId  =  poLacpSystemIdDomainId;
    }

    UI32  NsmGlobalAddPhyIntfToPoIntfMessage::getPoLacpSystemIdDomainId() const
    {
        return (m_poLacpSystemIdDomainId);
    }

    void  NsmGlobalAddPhyIntfToPoIntfMessage::setReplayPoConfigs(const bool &replayPoConfigs)
    {
        m_replayPoConfigs  =  replayPoConfigs;
    }

    bool  NsmGlobalAddPhyIntfToPoIntfMessage::getReplayPoConfigs() const
    {
        return (m_replayPoConfigs);
    }

    PhyIntfLocalManagedObject *NsmGlobalAddPhyIntfToPoIntfMessage::getPhyRef()
    {
        return m_refPhyIntfLocalManagedObject;
    }
    
    void NsmGlobalAddPhyIntfToPoIntfMessage::setPhyRef(PhyIntfLocalManagedObject *refPhyIntfLocalManagedObject)
    {
        m_refPhyIntfLocalManagedObject = refPhyIntfLocalManagedObject;
    }

    PoIntfManagedObject *NsmGlobalAddPhyIntfToPoIntfMessage::getPoRef()
    {
        return m_refPoIntfManagedObject;
    } 

    void NsmGlobalAddPhyIntfToPoIntfMessage::setPoRef(PoIntfManagedObject *refPoIntfManagedObject)
    {
        m_refPoIntfManagedObject = refPoIntfManagedObject;
    }
    

}
