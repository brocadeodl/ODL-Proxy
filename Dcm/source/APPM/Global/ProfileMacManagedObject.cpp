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
 *   Author : bgangise                                                     *
 **************************************************************************/

#include "APPM/Global/ProfileMacManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"
//#include "vcs.h"
#include "brocade-port-profile.h"
namespace DcmNs
{

    ProfileMacManagedObject::ProfileMacManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (ProfileMacManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager)
    {
    }

    ProfileMacManagedObject::ProfileMacManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const string &mac)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (ProfileMacManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        m_mac    (mac)
    {
    }

    ProfileMacManagedObject::~ProfileMacManagedObject ()
    {
    }

    string  ProfileMacManagedObject::getClassName()
    {
        return ("ProfileMacManagedObject");
    }

    void  ProfileMacManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_mac, "mac", appm_mac_address));
        addPersistableAttribute (new AttributeString(&m_profileName,"profileName"));
        addPersistableAttribute (new AttributeUI32(&m_profileId,"profileId"));
    }

    void  ProfileMacManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_mac, "mac", appm_mac_address));
        addPersistableAttributeForCreate  (new AttributeString(&m_profileName,"profileName"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_profileId,"profileId"));

        vector<string > keyName;
        keyName.push_back("mac");
        setUserDefinedKeyCombination(keyName);
    }

    void  ProfileMacManagedObject::setMac(const string &mac)
    {
        m_mac  =  mac;
    }

    string  ProfileMacManagedObject::getMac() const
    {
        return (m_mac);
    }

    void  ProfileMacManagedObject::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  ProfileMacManagedObject::getProfileName() const
    {
        return (m_profileName);
    }

    void  ProfileMacManagedObject::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  ProfileMacManagedObject::getProfileId() const
    {
        return (m_profileId);
    }

}
