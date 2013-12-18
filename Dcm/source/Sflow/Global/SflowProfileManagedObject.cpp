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

/*
 * SflowProfileManagedObject.cpp
 *
 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Sep 17, 2012
 *      Author: sunniram
 */

#include "Sflow/Global/SflowProfileManagedObject.h"
#include "Sflow/Global/SflowGlobalObjectManager.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "vcs.h"

namespace DcmNs
{

    SflowProfileManagedObject::SflowProfileManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager)
        : PrismElement  (pSflowGlobalObjectManager),
        PrismPersistableObject (SflowProfileManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSflowGlobalObjectManager),
        DcmManagedObject (pSflowGlobalObjectManager)
    {
    }

    SflowProfileManagedObject::SflowProfileManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager,const string &name,const UI32 &samplingRate)
        : PrismElement  (pSflowGlobalObjectManager),
        PrismPersistableObject (SflowProfileManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSflowGlobalObjectManager),
        DcmManagedObject (pSflowGlobalObjectManager),
        m_name    (name),
        m_samplingRate    (samplingRate)
    {
    }

    SflowProfileManagedObject::~SflowProfileManagedObject ()
    {
    }

    string  SflowProfileManagedObject::getClassName()
    {
        return ("SflowProfileManagedObject");
    }


    void SflowProfileManagedObject::setupKeys()
    {

        vector<string> keyName;
        keyName.push_back("profileName");
        setUserDefinedKeyCombination (keyName);
    }

    void  SflowProfileManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_name,"profileName",sflow_profile_name));
        addPersistableAttribute (new AttributeUI32(&m_samplingRate, "samplingRate", sflow_profile_sampling_rate));
    }

    void  SflowProfileManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_name,"profileName", sflow_profile_name));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_samplingRate, "samplingRate", sflow_profile_sampling_rate));
        setupKeys();
    }

    void  SflowProfileManagedObject::setProfileName(const string &name)
    {
        m_name  =  name;
    }

    string  SflowProfileManagedObject::getProfileName() const
    {
        return (m_name);
    }

    void  SflowProfileManagedObject::setSamplingRate(const UI32 &samplingRate)
    {
        m_samplingRate  =  samplingRate;
    }

    UI32  SflowProfileManagedObject::getSamplingRate() const
    {
        return (m_samplingRate);
    }

}


