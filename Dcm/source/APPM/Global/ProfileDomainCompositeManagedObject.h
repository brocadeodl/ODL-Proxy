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

#ifndef PROFILEDOMAINCOMPOSITEMANAGEDOBJECT_H
#define PROFILEDOMAINCOMPOSITEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs {

    class APPMGlobalObjectManager;
    class ProfileDomainCompositeManagedObject : virtual public DcmManagedObject
    {
	
        private:

        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();

        public:
            ProfileDomainCompositeManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            virtual    ~ProfileDomainCompositeManagedObject ();
            
			static string getClassName();
			
			void setPortProfileName(const string &portProfileName);
			string getPortProfileName() const;

		private:
        protected:
        public:
			string 			m_portProfileName;
    };
}
#endif                                            //PROFILEDOMAINCOMPOSITEMANAGEDOBJECT_H
