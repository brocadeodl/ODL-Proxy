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

/******************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.             *
 *   All rights reserved.                                                     *
 *   Author : Raghuprem M                                                     *   
 *****************************************************************************/

#include "APPM/Global/APPMNativeVlanManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
//#include "vcs.h"
#include "brocade-port-profile.h"
namespace DcmNs
{

    APPMNativeVlanManagedObject::APPMNativeVlanManagedObject 
			(WaveObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (
						APPMNativeVlanManagedObject::getClassName (), 
						DcmManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
		m_nativevlanId    (APPM_NSM_NATIVE_VLAN_DEFAULT),
        m_ctag            (APPM_NSM_NATIVE_VLAN_DEFAULT)
    {
    }

    APPMNativeVlanManagedObject::APPMNativeVlanManagedObject 
			(WaveObjectManager *pWaveObjectManager, 
			 const UI32 &nativevlanId)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (
			APPMNativeVlanManagedObject::getClassName (), 
			DcmManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        m_nativevlanId    (nativevlanId)
    {
    }

    APPMNativeVlanManagedObject::
			~APPMNativeVlanManagedObject ()
    {
    }

    string  APPMNativeVlanManagedObject::getClassName()
    {
        return ("APPMNativeVlanManagedObject");
    }

    void  APPMNativeVlanManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_nativevlanId,APPM_NSM_NATIVE_VLAN_DEFAULT,
								"nativevlanId",appm_native_vlan_id));
        addPersistableAttribute (new AttributeUI32 (&m_ctag, APPM_NSM_NATIVE_VLAN_DEFAULT,
                                "ctag", appm_trunk_native_vlan_ctag_id));

    }

    void  APPMNativeVlanManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_nativevlanId,
								"nativevlanId",appm_native_vlan_id));
        addPersistableAttributeForCreate (new AttributeUI32 (&m_ctag,
                                "ctag", appm_trunk_native_vlan_ctag_id));
    }
    
	void  APPMNativeVlanManagedObject::setNativeVlanId(const UI32 &nativevlanId)
    {
        m_nativevlanId = nativevlanId; 
    }

    UI32  APPMNativeVlanManagedObject::getNativeVlanId() const
    {
        return (m_nativevlanId);
    }
    void APPMNativeVlanManagedObject::setCtag(const UI32 &ctag) {
        m_ctag = ctag;
    }
    UI32 APPMNativeVlanManagedObject::getCtag() const {
        return m_ctag;
    }

}
