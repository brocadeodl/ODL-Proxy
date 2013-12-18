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
 *   .Author : Mobin Mohan                                                     *
 *****************************************************************************/

#include "Nsm/Local/NativeVlanIntfConfLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Lldp/Global/LldpGlobalProfileConfManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "vcs.h"

namespace DcmNs
{

    NativeVlanIntfConfLocalManagedObject::NativeVlanIntfConfLocalManagedObject 
			(WaveObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (
						NativeVlanIntfConfLocalManagedObject::getClassName (), 
						DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager)
    {
		m_nativevlanId = 1;
		m_nativevlanDisable = 1;
		m_nativevlanPresence = 0;
    }

    NativeVlanIntfConfLocalManagedObject::NativeVlanIntfConfLocalManagedObject 
			(WaveObjectManager *pWaveObjectManager, 
			 const UI32 &nativevlanId)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (
			NativeVlanIntfConfLocalManagedObject::getClassName (), 
			DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pWaveObjectManager),
        m_nativevlanId    (nativevlanId)
    {
    }

    NativeVlanIntfConfLocalManagedObject::
			~NativeVlanIntfConfLocalManagedObject ()
    {
    }

    string  NativeVlanIntfConfLocalManagedObject::getClassName()
    {
        return ("NativeVlanIntfConfLocalManagedObject");
    }

    void  NativeVlanIntfConfLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_nativevlanDisable,
                                                            "nativevlanDisable", brocade_interface_native_vlan));
        addPersistableAttribute (new AttributeUI32(&m_nativevlanId, 1,
								"nativevlanId",brocade_interface_nativevlanid, false));
        addPersistableAttribute (new AttributeBool(&m_nativevlanPresence,
								"nativevlanPresence",brocade_interface_tag));

    }

    void  NativeVlanIntfConfLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeBool(&m_nativevlanDisable,
                                                            "nativevlanDisable"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_nativevlanId,
															"nativevlanId"));
        addPersistableAttributeForCreate (new AttributeBool(&m_nativevlanPresence,
                                                            "nativevlanPresence"));
    }
	
	void  NativeVlanIntfConfLocalManagedObject::setNativevlanDisable(const bool &nativevlanDisable)
    {
        m_nativevlanDisable  =  nativevlanDisable;
    }

    bool  NativeVlanIntfConfLocalManagedObject::getNativevlanDisable() const
    {
        return (m_nativevlanDisable);
    }
    
	void  NativeVlanIntfConfLocalManagedObject::setNativeVlanId(
													const UI32 &nativevlanId)
    {
        m_nativevlanId = nativevlanId; 
    }

    UI32  NativeVlanIntfConfLocalManagedObject::getNativeVlanId() const
    {
        return (m_nativevlanId);
    }
	
	void  NativeVlanIntfConfLocalManagedObject::setNativevlanPresence(const bool &nativevlanPresence)
    {
        m_nativevlanPresence  =  nativevlanPresence;
    }

    bool  NativeVlanIntfConfLocalManagedObject::getNativevlanPresence() const
    {
        return (m_nativevlanPresence);
    }

}
