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
 *   Author : nbellari                                                     *
 **************************************************************************/

#include "Qos/Global/RedProfileManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    RedProfileManagedObject::RedProfileManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (RedProfileManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    RedProfileManagedObject::RedProfileManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const SI32 &profileId,const SI32 &minThreshold,const SI32 &maxThreshold,const SI32 &dropProbability)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (RedProfileManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_profileId    (profileId),
        m_minThreshold    (minThreshold),
        m_maxThreshold    (maxThreshold),
        m_dropProbability    (dropProbability)
    {
    }

    RedProfileManagedObject::~RedProfileManagedObject ()
    {
    }

    string  RedProfileManagedObject::getClassName()
    {
        return ("RedProfileManagedObject");
    }

    void  RedProfileManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_profileId,"profileId"));
        addPersistableAttribute (new AttributeSI32(&m_minThreshold,"minThreshold"));
        addPersistableAttribute (new AttributeSI32(&m_maxThreshold,"maxThreshold"));
        addPersistableAttribute (new AttributeSI32(&m_dropProbability,"dropProbability"));

		setUserTagForAttribute("profileId", qos_profile_id);
		setUserTagForAttribute("minThreshold", qos_min_threshold);
		setUserTagForAttribute("maxThreshold", qos_max_threshold);
		setUserTagForAttribute("dropProbability", qos_drop_probability);
    }

    void  RedProfileManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_profileId,"profileId"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_minThreshold,"minThreshold"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_maxThreshold,"maxThreshold"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dropProbability,"dropProbability"));

        vector<string> keyName;
        keyName.push_back ("profileId");
        setUserDefinedKeyCombination (keyName);
    }

    void  RedProfileManagedObject::setProfileId(const SI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    SI32  RedProfileManagedObject::getProfileId() const
    {
        return (m_profileId);
    }

    void  RedProfileManagedObject::setMinThreshold(const SI32 &minThreshold)
    {
        m_minThreshold  =  minThreshold;
    }

    SI32  RedProfileManagedObject::getMinThreshold() const
    {
        return (m_minThreshold);
    }

    void  RedProfileManagedObject::setMaxThreshold(const SI32 &maxThreshold)
    {
        m_maxThreshold  =  maxThreshold;
    }

    SI32  RedProfileManagedObject::getMaxThreshold() const
    {
        return (m_maxThreshold);
    }

    void  RedProfileManagedObject::setDropProbability(const SI32 &dropProbability)
    {
        m_dropProbability  =  dropProbability;
    }

    SI32  RedProfileManagedObject::getDropProbability() const
    {
        return (m_dropProbability);
    }

}
