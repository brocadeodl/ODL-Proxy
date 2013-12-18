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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef PRIORITYMAPMANAGEDOBJECT_H
#define PRIORITYMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class PriorityMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PriorityMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager);
            PriorityMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const ObjectId &cos0ToPriorityGrouMap,const ObjectId &cos1ToPriorityGrouMap,const ObjectId &cos2ToPriorityGrouMap,const ObjectId &cos3ToPriorityGrouMap,const ObjectId &cos4ToPriorityGrouMap,const ObjectId &cos5ToPriorityGrouMap,const ObjectId &cos6ToPriorityGrouMap,const ObjectId &cos7ToPriorityGrouMap);
            virtual    ~PriorityMapManagedObject ();
            static string getClassName();
            void setCos0ToPriorityGrouMap(const ObjectId &cos0ToPriorityGrouMap);
            ObjectId getCos0ToPriorityGrouMap()  const;
            void setCos1ToPriorityGrouMap(const ObjectId &cos1ToPriorityGrouMap);
            ObjectId getCos1ToPriorityGrouMap()  const;
            void setCos2ToPriorityGrouMap(const ObjectId &cos2ToPriorityGrouMap);
            ObjectId getCos2ToPriorityGrouMap()  const;
            void setCos3ToPriorityGrouMap(const ObjectId &cos3ToPriorityGrouMap);
            ObjectId getCos3ToPriorityGrouMap()  const;
            void setCos4ToPriorityGrouMap(const ObjectId &cos4ToPriorityGrouMap);
            ObjectId getCos4ToPriorityGrouMap()  const;
            void setCos5ToPriorityGrouMap(const ObjectId &cos5ToPriorityGrouMap);
            ObjectId getCos5ToPriorityGrouMap()  const;
            void setCos6ToPriorityGrouMap(const ObjectId &cos6ToPriorityGrouMap);
            ObjectId getCos6ToPriorityGrouMap()  const;
            void setCos7ToPriorityGrouMap(const ObjectId &cos7ToPriorityGrouMap);
            ObjectId getCos7ToPriorityGrouMap()  const;
			void setCeeMapName(const string &ceeMapName);
			string getCeeMapName()	const;

// Now the data members

        private:
        protected:
        public:
            ObjectId  m_cos0ToPriorityGrouMap;
            ObjectId  m_cos1ToPriorityGrouMap;
            ObjectId  m_cos2ToPriorityGrouMap;
            ObjectId  m_cos3ToPriorityGrouMap;
            ObjectId  m_cos4ToPriorityGrouMap;
            ObjectId  m_cos5ToPriorityGrouMap;
            ObjectId  m_cos6ToPriorityGrouMap;
            ObjectId  m_cos7ToPriorityGrouMap;
			string	  m_ceeMapName;
    };
}
#endif                                            //PRIORITYMAPMANAGEDOBJECT_H
