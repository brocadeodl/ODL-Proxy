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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "APPM/Global/SubProfileManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"

#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    SubProfileManagedObject::SubProfileManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (SubProfileManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager)
    {
    }

    SubProfileManagedObject::SubProfileManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const string &description)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (SubProfileManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        m_description    (description)
    {
    }

    SubProfileManagedObject::~SubProfileManagedObject ()
    {
    }

    string  SubProfileManagedObject::getClassName()
    {
        return ("SubProfileManagedObject");
    }

    void  SubProfileManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_description,"description"));
        //setUserTagForAttribute ("description", logical_switch_name);
    }

    void  SubProfileManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_description,"description"));
    }

	void SubProfileManagedObject::setupKeys()
	{
        vector<string> keyName;
        keyName.push_back("description");
        setUserDefinedKeyCombination (keyName);
    }

    void  SubProfileManagedObject::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  SubProfileManagedObject::getDescription() const
    {
        return (m_description);
    }

    UI32 SubProfileManagedObject::activationValidate()
    {
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        return WAVE_MESSAGE_SUCCESS;
    }

}
