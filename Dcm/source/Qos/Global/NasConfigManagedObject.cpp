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
 *   Author : pbalacha                                                     *
 **************************************************************************/
#include "Qos/Global/NasConfigManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "brocade-qos.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"

namespace DcmNs
{

    NasConfigManagedObject::NasConfigManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (NasConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
		m_autoNasPresenceFlag(true),
		m_cos (DEFAULT_NAS_COS),
		m_dscp (DEFAULT_NAS_DSCP)
    {
    }

    NasConfigManagedObject::NasConfigManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager, const UI32 cos, const UI32 dscp)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (NasConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
		m_cos (cos),
		m_dscp (dscp)
		
    {
    }


    NasConfigManagedObject::~NasConfigManagedObject ()
    {
    }

    string  NasConfigManagedObject::getClassName()
    {
        return ("NasConfigManagedObject");
    }

    void  NasConfigManagedObject::setupAttributesForPersistence()
    {
		DcmManagedObject::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeBool(&m_autoNasPresenceFlag, "autoNasPresenceFlag", qos_auto_qos)); 
		addPersistableAttribute (new AttributeUI32(&m_cos, DEFAULT_NAS_COS, "cos", qos_cos)); 
		addPersistableAttribute (new AttributeUI32(&m_dscp, DEFAULT_NAS_DSCP, "dscp", qos_dscp)); 
    }

    void  NasConfigManagedObject::setupAttributesForCreate()
    {
		DcmManagedObject::setupAttributesForCreate();
		addPersistableAttributeForCreate (new AttributeBool(&m_autoNasPresenceFlag, "autoNasPresenceFlag", qos_auto_qos)); 
		addPersistableAttributeForCreate (new AttributeUI32(&m_cos, DEFAULT_NAS_COS, "cos", qos_cos)); 
		addPersistableAttributeForCreate (new AttributeUI32(&m_dscp, DEFAULT_NAS_DSCP, "dscp", qos_dscp)); 
    }

	void NasConfigManagedObject::setCos (const UI32 &cos)
	{
    	m_cos = cos;
	}

	UI32 NasConfigManagedObject::getCos ()
	{
    	return (m_cos);
	}

	void NasConfigManagedObject::setDscp(const UI32 &dscp)
	{
    	m_dscp = dscp;
	}

	UI32 NasConfigManagedObject::getDscp ()
	{
    	return (m_dscp);
	}
}
