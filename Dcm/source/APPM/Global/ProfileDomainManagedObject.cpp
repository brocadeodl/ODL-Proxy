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

#include "APPM/Global/ProfileDomainManagedObject.h"
#include "APPM/Global/ProfileDomainCompositeManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalConfWorker.h"
#include "APPM/Global/ProfileMacManagedObject.h"
#include "APPM/Global/SecProfileCfgManagedObject.h"
#include "APPM/Global/FcoeProfileCfgManagedObject.h"
#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Utils/FrameworkToolKit.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "APPM/Global/APPMGlobalAPPMProfileMessage.h"
#include "APPM/Global/APPMGlobalAPPMSecurityMessage.h"
#include "APPM/Global/APPMGlobalAPPMFcoeMessage.h"
#include "Ssm/Global/StdMacAclPolicyManagedObject.h"
#include "Ssm/Global/ExtMacAclPolicyManagedObject.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/APPM/APPMProfileMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "brocade-port-profile.h"
#include "DcmCStatus.h"
#include <raslog/raslog.h>
#include <raslog/raslog_nsm.h>
#include <algorithm>

namespace DcmNs {

    ProfileDomainManagedObject::ProfileDomainManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (ProfileDomainManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager) {
        }

    ProfileDomainManagedObject::~ProfileDomainManagedObject () {
        m_associatedPortProfiles.clear();
    }

    string  ProfileDomainManagedObject::getClassName() {
        return ("ProfileDomainManagedObject");
    }

    void  ProfileDomainManagedObject::setupAttributesForPersistence() {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_portProfileDomainName, "portProfileDomainName", appm_port_profile_domain_name));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<ProfileDomainCompositeManagedObject>
                (&m_associatedPortProfiles,"associatedPortProfiles", getClassName(), getObjectId(), ProfileDomainCompositeManagedObject::getClassName ()));
    }

    void  ProfileDomainManagedObject::setupAttributesForCreate() {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_portProfileDomainName, "portProfileDomainName", appm_port_profile_domain_name));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<ProfileDomainCompositeManagedObject>
                (&m_associatedPortProfiles,"associatedPortProfiles", getClassName(), getObjectId(), ProfileDomainCompositeManagedObject::getClassName ()));

        vector<string > keyName;
        keyName.push_back ("portProfileDomainName");
        setUserDefinedKeyCombination (keyName);
    }

    void ProfileDomainManagedObject::addPortProfileToPortProfileDomain
        (const WaveManagedObjectPointer<ProfileDomainCompositeManagedObject> &portProfile) {
            m_associatedPortProfiles.push_back(portProfile);
        }

    void ProfileDomainManagedObject::removePortProfileFromPortProfileDomain
        (const string &portProfile) {

            for(unsigned int it = 0 ; it < m_associatedPortProfiles.size(); it++) {
                if(portProfile == (m_associatedPortProfiles[it]->getPortProfileName())){
                    m_associatedPortProfiles.erase(m_associatedPortProfiles.begin()+it);
                }
            }
        }

    vector<WaveManagedObjectPointer<ProfileDomainCompositeManagedObject> > 
        ProfileDomainManagedObject::getAssociatedPortProfiles() const {

            return m_associatedPortProfiles;
        }

    void ProfileDomainManagedObject::setAssociatedPortProfiles
        (const vector<WaveManagedObjectPointer<ProfileDomainCompositeManagedObject> > &associatedPortProfiles) {
            m_associatedPortProfiles = associatedPortProfiles;
        }

    void ProfileDomainManagedObject::setPortProfileDomainName(const string &portProfileDomainName) {
        m_portProfileDomainName = portProfileDomainName;
    }

    string ProfileDomainManagedObject::getPortProfileDomainName() const {
        return m_portProfileDomainName;
    }

}
