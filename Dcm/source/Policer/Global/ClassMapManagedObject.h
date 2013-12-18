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

#ifndef CLASSMAPMANAGEDOBJECT_H
#define CLASSMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerGlobalObjectManager;
    class ClassMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager);
            ClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager,const string &classMapName, const UI32 &filterCriteria, const string &accessListName);
            virtual    ~ClassMapManagedObject ();
            static string getClassName();
            void setClassMapName(const string &classMapName);
            string getClassMapName()  const;

            void setFilterCriteria(const UI32 &filterCriteria);
            UI32 getFilterCriteria() const;

            void setAccessListName(const string &accessListName);
            string getAccessListName() const;

// Now the data members

        private:
        protected:
        public:
            string  m_classMapName;
            UI32 	m_filterCriteria;
            string  m_accessListName;
    };
}
#endif                                            //CLASSMAPMANAGEDOBJECT_H
