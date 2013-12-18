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

#include "APPM/Global/AMPPProfileCompositeManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Utils/FrameworkToolKit.h"
#include "brocade-port-profile.h"
#include "DcmCStatus.h"
#include <raslog/raslog.h>
#include <raslog/raslog_nsm.h>
#include <algorithm>

namespace DcmNs {

    AMPPProfileCompositeManagedObject::AMPPProfileCompositeManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (AMPPProfileCompositeManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager) {
            m_portProfileName = "";
    }

	AMPPProfileCompositeManagedObject::AMPPProfileCompositeManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
		: PrismElement  (pNsmLocalObjectManager),
		PrismPersistableObject (AMPPProfileCompositeManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pNsmLocalObjectManager),
		DcmManagedObject (pNsmLocalObjectManager) {
            m_portProfileName = "";
	}
	
    AMPPProfileCompositeManagedObject::~AMPPProfileCompositeManagedObject () {
    }

    string  AMPPProfileCompositeManagedObject::getClassName() {
        return ("AMPPProfileCompositeManagedObject");
    }

    void  AMPPProfileCompositeManagedObject::setupAttributesForPersistence() {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_portProfileName, "portProfileName", 
		appm_port_to_port_profile_association));
    }

    void  AMPPProfileCompositeManagedObject::setupAttributesForCreate() {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_portProfileName, "portProfileName", appm_port_to_port_profile_association));

        vector<string > keyName;
        keyName.push_back ("portProfileName");
        setUserDefinedKeyCombination (keyName);
    }

    void AMPPProfileCompositeManagedObject::setPortProfileName(const string &portProfileName) {
        m_portProfileName = portProfileName;
    }

    string AMPPProfileCompositeManagedObject::getPortProfileName() const {
        return m_portProfileName;
    }

}
