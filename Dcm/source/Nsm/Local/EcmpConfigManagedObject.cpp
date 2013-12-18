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
 *   Author : Amulya Makam                                                     *
 **************************************************************************/

#include "Nsm/Local/EcmpConfigManagedObject.h"
#include "Nsm/Local/NsmLocalEcmpConfigMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "brocade-rbridge.h"
#include "brocade-fabric-service.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "ClientInterface/Nsm/NsmUtils.h"
namespace DcmNs
{

    EcmpConfigManagedObject::EcmpConfigManagedObject (WaveObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (EcmpConfigManagedObject::getClassName (),
        DcmLocalManagedObject::getClassName()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager)
    {
        m_loadBalanceMode = 7;
        m_rbridgeId = 0;
		m_hashSwap.setUI32Value(0);
		m_hashSwap.setIsUserConfigured(false);
    }

    EcmpConfigManagedObject::EcmpConfigManagedObject (WaveObjectManager *pWaveObjectManager,
        const UI32 &loadBalanceMode,const UI32 &rbridgeId,const UI32 &hashSwap)  : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (EcmpConfigManagedObject::getClassName (),
        DcmLocalManagedObject::getClassName()),
        WaveManagedObject (pWaveObjectManager),
    	DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager),
        m_loadBalanceMode (loadBalanceMode),
        m_rbridgeId(rbridgeId)
        {
			 m_hashSwap.setUI32Value(hashSwap);
			 m_hashSwap.setIsUserConfigured(true);
        }

    EcmpConfigManagedObject::~EcmpConfigManagedObject ()
    {
    }

    string  EcmpConfigManagedObject::getClassName()
    {
        return ("EcmpConfigManagedObject");
    }

    void  EcmpConfigManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum((UI32*)&m_loadBalanceMode,"loadBalanceMode",fabric_service_ecmp_load_balance));
        addPersistableAttribute (new AttributeUI32(&m_rbridgeId,"switchNum",rbridge_rbridge_id));
     addPersistableAttribute (new AttributeUI32UC(&m_hashSwap,true,true,0,"hashSwap",fabric_service_load_balance_hash_swap));
    }

    void  EcmpConfigManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_loadBalanceMode,"loadBalanceMode",
				fabric_service_ecmp_load_balance));
        addPersistableAttributeForCreate (new AttributeUI32(&m_rbridgeId,"switchNum",rbridge_rbridge_id));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_hashSwap,true,true,0,"hashSwap",fabric_service_load_balance_hash_swap));
        vector<string > keyName;
        keyName.push_back ("switchNum");
        setUserDefinedKeyCombination (keyName);

    }

    void  EcmpConfigManagedObject::setLoadBalanceMode(const UI32 &loadBalanceMode)
    {
        m_loadBalanceMode  =  loadBalanceMode;
    }

    UI32 EcmpConfigManagedObject::getLoadBalanceMode() const
    {
        return (m_loadBalanceMode);
    }

    void EcmpConfigManagedObject::setRbridgeId(const UI32 &rbridgeId)
    {
        m_rbridgeId = rbridgeId;
    }

    UI32 EcmpConfigManagedObject::getRbridgeId() const
    {
        return (m_rbridgeId);
    }

    void EcmpConfigManagedObject::setHashSwap(const UI32 &hashSwap, const bool &flag)
    {
		 m_hashSwap.setUI32Value(hashSwap);
		 m_hashSwap.setIsUserConfigured(flag);
    }

    UI32UC EcmpConfigManagedObject::getHashSwap() const
    {
		return (m_hashSwap);
    }
	
    Attribute *EcmpConfigManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
        Attribute *pAttribute = NULL;
        pAttribute = new AttributeBool (true, "ecmp");
        trace (TRACE_LEVEL_ERROR,
                string("EcmpConfigManagedObject::getAttributeByUserTagUserDefined: Entry "));
        return (pAttribute);
    }
}
