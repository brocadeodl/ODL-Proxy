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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Ojasvita                                                     *
 **************************************************************************/

#include "Nsm/Local/Ssm/NsmStormControlIntfConfLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ssmBum_public.h"
#include "ClientInterface/Qos/qos_bum_dcm_common.h"

namespace DcmNs
{
	NsmStormControlIntfConfLocalManagedObject :: NsmStormControlIntfConfLocalManagedObject (WaveObjectManager *pNsmLocalObjectManager)
		: PrismElement  (pNsmLocalObjectManager),
		PrismPersistableObject (NsmStormControlIntfConfLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pNsmLocalObjectManager),
		DcmManagedObject (pNsmLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pNsmLocalObjectManager)
	{
	}

	NsmStormControlIntfConfLocalManagedObject :: NsmStormControlIntfConfLocalManagedObject (WaveObjectManager *pNsmLocalObjectManager,
		const UI32 &protocolType, const UI32 &rateFormat, const UI64 &bps, const UI32 &percentage, const UI32 &action)
		: PrismElement  (pNsmLocalObjectManager),
		PrismPersistableObject (NsmStormControlIntfConfLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pNsmLocalObjectManager),
		DcmManagedObject (pNsmLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pNsmLocalObjectManager),
		m_protocolType (protocolType),
		m_rateFormat (rateFormat),
		m_bps (bps),
		m_percentage (percentage),
		m_action (action)
	{
	}

    NsmStormControlIntfConfLocalManagedObject::~NsmStormControlIntfConfLocalManagedObject ()
    {
    }

    string  NsmStormControlIntfConfLocalManagedObject::getClassName()
    {
        return ("NsmStormControlIntfConfLocalManagedObject");
    }

    void  NsmStormControlIntfConfLocalManagedObject::setupAttributesForPersistence()
    {
		UI32 actionDefault = QOS_BUM_ACTION_LIMIT_ONLY_E;
        DcmLocalManagedObject::setupAttributesForPersistence ();
		
        addPersistableAttribute (new AttributeEnum((UI32*)&m_protocolType, "protocolType", bum_storm_control_protocol_type));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_rateFormat, "rateFormat", bum_storm_control_rate_format));
        addPersistableAttribute (new AttributeUI64((UI64*)&m_bps, "bps", bum_storm_control_rate_bps));
        addPersistableAttribute (new AttributeUI32((UI32*)&m_percentage, "percentage", bum_storm_control_rate_percent));
		  addPersistableAttribute (new AttributeEnum((UI32*)&m_action, actionDefault, "action", bum_storm_control_bum_action));
    }

    void  NsmStormControlIntfConfLocalManagedObject::setupAttributesForCreate()
    {
		UI32 actionDefault = QOS_BUM_ACTION_LIMIT_ONLY_E;
        DcmLocalManagedObject::setupAttributesForCreate ();

        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_protocolType, "protocolType", bum_storm_control_protocol_type));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_rateFormat, "rateFormat", bum_storm_control_rate_format));
        addPersistableAttributeForCreate  (new AttributeUI64((UI64*)&m_bps, "bps", bum_storm_control_rate_bps));
        addPersistableAttributeForCreate  (new AttributeUI32((UI32*)&m_percentage, "percentage", bum_storm_control_rate_percent));
		  addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_action, actionDefault, "action", bum_storm_control_bum_action));
    }

    void NsmStormControlIntfConfLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("protocolType");
        setUserDefinedKeyCombination (keyName);
    }

    void NsmStormControlIntfConfLocalManagedObject::setProtocolType(const UI32 &protocolType)
    {
        m_protocolType = protocolType;
    }

    UI32 NsmStormControlIntfConfLocalManagedObject::getProtocolType() const
    {
        return m_protocolType;
    }

    void  NsmStormControlIntfConfLocalManagedObject::setRateFormat(const UI32 &rateFormat)
    {
        m_rateFormat = rateFormat;
    }

    UI32  NsmStormControlIntfConfLocalManagedObject::getRateFormat() const
    {
        return (m_rateFormat);
    }

    void  NsmStormControlIntfConfLocalManagedObject::setBps(const UI64 &bps)
    {
        m_bps  =  bps;
    }

    UI64  NsmStormControlIntfConfLocalManagedObject::getBps() const
    {
        return (m_bps);
    }

    void  NsmStormControlIntfConfLocalManagedObject::setPercentage(const UI32 &percentage)
    {
        m_percentage  =  percentage;
    }

    UI32 NsmStormControlIntfConfLocalManagedObject::getPercentage() const
    {
        return (m_percentage);
    }
	
    void  NsmStormControlIntfConfLocalManagedObject::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  NsmStormControlIntfConfLocalManagedObject::getAction() const
    {
        return (m_action);
    }
}
