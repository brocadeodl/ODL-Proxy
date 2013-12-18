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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#ifndef POLICYCLASSMAPMANAGEDOBJECT_H
#define POLICYCLASSMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerGlobalObjectManager;
    class PoClassMapManagedObject;
	class PolicyClassMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PolicyClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager);
            PolicyClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager,const string &policyClassMapName);
            virtual    ~PolicyClassMapManagedObject ();
            static string getClassName();
            void setPolicyClassMapName(const string &policyClassMapName);
            string getPolicyClassMapName() const;
			void setPoManObj(const vector<WaveManagedObjectPointer<PoClassMapManagedObject> > &poManObj);
		    vector<WaveManagedObjectPointer<PoClassMapManagedObject> > getPoManObj() const;
			PoClassMapManagedObject *getPoManObjByObjectId(const ObjectId &classMapId);
			void addPoManObj(PoClassMapManagedObject *poManObj);
			SI32 deletePoManObj(const ObjectId &classMapId);	
// Now the data members

        private:
        protected:
        public:
            string  m_policyClassMapName;
    		vector<WaveManagedObjectPointer<PoClassMapManagedObject> > m_poManObj;
	};
}
#endif                                            //POLICYCLASSMAPMANAGEDOBJECT_H
