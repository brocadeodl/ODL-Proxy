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

#ifndef PROFILEDOMAINMANAGEDOBJECT_H
#define PROFILEDOMAINMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "APPM/Global/ProfileDomainCompositeManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs {

    class APPMGlobalObjectManager;
    class ProfileDomainCompositeManagedObject;
    class ProfileDomainManagedObject : virtual public DcmManagedObject {

        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ProfileDomainManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            virtual    ~ProfileDomainManagedObject ();

            static string getClassName();

            void addPortProfileToPortProfileDomain
                (const WaveManagedObjectPointer<ProfileDomainCompositeManagedObject> &portProfile);
            void removePortProfileFromPortProfileDomain
                (const string &portProfile);

            vector<WaveManagedObjectPointer<ProfileDomainCompositeManagedObject> > getAssociatedPortProfiles() const;
            void setAssociatedPortProfiles(const vector<WaveManagedObjectPointer<ProfileDomainCompositeManagedObject> > &associatedPortProfiles);

            void setPortProfileDomainName(const string &portProfileDomainName);
            string getPortProfileDomainName() const;

        private:
        protected:
        public:
            string m_portProfileDomainName;
            vector<WaveManagedObjectPointer<ProfileDomainCompositeManagedObject> >  m_associatedPortProfiles;
    };
}
#endif                                            //PROFILEDOMAINMANAGEDOBJECT_H
