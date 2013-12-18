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

#ifndef APPMUTILS_H
#define APPMUTILS_H

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"

using namespace WaveNs;

namespace DcmNs {

    class APPMUtils : public WaveObjectManagerToolKit {
        private:
        protected:
        public:
            static ResourceId isPortProfileDomainActiveOnAnInterface(string &portProfileDomainName);
            static ResourceId isPortProfileActiveOnAnInterface(string &portProfileName);
            static ResourceId isPortProfileActiveOnADomain(string &portProfileName);
            static ResourceId isPortProfileConflictingWithOtherProfilesOfTheDomain
                (string &portProfileName, string &portProfileDomainName);
            static ResourceId doesPortProfileExist(string &portProfileName);
            static ResourceId doesPortProfileDomainExist(string &portProfileDomainName);
            static ResourceId validateMutuallyExclusivePortProfilePortModes
                (PoPhyIntfManagedObject* pPoPhyIntfManagedObject, UI32 cmdCode);
            static ResourceId isInterfaceAlreadyAssociatedWithAPortProfileDomain
                (PoPhyIntfManagedObject* pPoPhyIntfManagedObject);
            static ResourceId isPortProfilePort
                (PoPhyIntfManagedObject* pPoPhyIntfManagedObject, bool &isProfilePort);
            static ResourceId setShutdownStateOfInterface
                (PoPhyIntfManagedObject* pPoPhyIntfManagedObject, bool &shutStateOfInterface);
            static ResourceId doesPortProfileBelongToPortProfileDomain
                (string &portProfileName, string &portProfileDomainName);
            static ResourceId isVlanPresent(UI32 &vlanId);
            static ResourceId isVlanValidForClassification(UI32 &vlanId);
            static ResourceId getPortProfilesOfAPortProfileDomain
                (string &portProfileDomainName, vector<string> &portProfiles);
            static ResourceId getProfilesAndDomainsofPoPhy
                (PoPhyIntfManagedObject* poPhyMo, vector <string> &portProfiles, string &portProfileDomainName);
            static ResourceId isMacGroupPresent(UI32 &macGroupId);
            static ResourceId isGvlanSpecificAPPMConfigPresent();
            static ResourceId doesCtagOrTrunkVlanAlreadyExist 
                (string &profileName, UI32 &trunkVlanId, UI32 &ctagId);

    };
}
#endif //APPMUTILS_H
