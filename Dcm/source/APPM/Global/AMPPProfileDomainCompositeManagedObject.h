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

#ifndef AMPPPROFILEDOMAINCOMPOSITEMANAGEDOBJECT_H
#define AMPPPROFILEDOMAINCOMPOSITEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

/* Used to associate a port-profile-domain with an interface [physical interface/po] */

using namespace WaveNs;

namespace DcmNs {

    class NsmGlobalObjectManager;
    class NsmLocalObjectManager;
    class AMPPProfileDomainCompositeManagedObject : virtual public DcmManagedObject {

        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            AMPPProfileDomainCompositeManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
			AMPPProfileDomainCompositeManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual ~AMPPProfileDomainCompositeManagedObject ();

            static string getClassName();

            void setPortProfileDomainName(const string &portProfileDomainName);
            string getPortProfileDomainName() const;

        private:
        protected:
        public:
            string m_portProfileDomainName;
    };
}
#endif                                            //AMPPPROFILEDOMAINCOMPOSITEMANAGEDOBJECT_H
