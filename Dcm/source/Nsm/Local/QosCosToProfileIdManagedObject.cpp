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
 *   Copyright (C) 2011-2015 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Sudha Balodia                                                     *
 **************************************************************************/

#include "Nsm/Local/QosCosToProfileIdManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    QosCosToProfileIdManagedObject::QosCosToProfileIdManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (QosCosToProfileIdManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_cos = 0;
		m_red_profile = 0;
    }

    QosCosToProfileIdManagedObject::QosCosToProfileIdManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (QosCosToProfileIdManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager)
    {
		m_cos = 0;
		m_red_profile = 0;
    }

    QosCosToProfileIdManagedObject::QosCosToProfileIdManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &cos, const UI32 &red_profile)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (QosCosToProfileIdManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_cos    (cos),
		m_red_profile (red_profile)
    {
    }

    QosCosToProfileIdManagedObject::QosCosToProfileIdManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &cos, const UI32 &red_profile)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (QosCosToProfileIdManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager),
        m_cos    (cos),
		m_red_profile (red_profile)
    {
    }


    QosCosToProfileIdManagedObject::~QosCosToProfileIdManagedObject ()
    {
    }

    string  QosCosToProfileIdManagedObject::getClassName()
    {
        return ("QosCosToProfileIdManagedObject");
    }

    void  QosCosToProfileIdManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_red_profile,"redprofile", qos_red_profile_id));
        addPersistableAttribute (new AttributeUI32(&m_cos,"cos"));
		setUserTagForAttribute("cos", qos_red_cos_value);
    }

    void  QosCosToProfileIdManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_red_profile,"redprofile", qos_red_profile_id));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_cos,"cos"));

		vector<string > keyName;
		keyName.push_back ("cos");
		setUserDefinedKeyCombination (keyName);
    }


    void  QosCosToProfileIdManagedObject::setCos(const UI32 &cos)
    {
        m_cos  =  cos;
    }

    UI32  QosCosToProfileIdManagedObject::getCos() const
    {
        return (m_cos);
    }
	
	void  QosCosToProfileIdManagedObject::setRedProfile(const UI32 &red_profile)
    {
        m_red_profile = red_profile;
    }

    UI32  QosCosToProfileIdManagedObject::getRedProfile() const
    {
        return (m_red_profile);
    }

}
