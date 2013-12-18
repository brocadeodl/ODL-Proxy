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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sholla                                                     *
 **************************************************************************/

#ifndef UIDMANAGEDOBJECT_H
#define UIDMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

#define UID_ATTR_ENTITY_NAME   "entityName"
#define UID_ATTR_ID_TYPE       "idType"
#define UID_ATTR_ID            "id"
#define UID_ATTR_ACTIVE        "active"

    class UidServiceObjectManager;

    class UidManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            UidManagedObject (UidServiceObjectManager *pUidServiceObjectManager);
            UidManagedObject (UidServiceObjectManager *pUidServiceObjectManager,const string &entityName,const UI32 &idType,const UI32 &id,const bool &active);
            virtual    ~UidManagedObject ();
            static string getClassName();
            void setEntityName(const string &entityName);
            string getEntityName()  const;
            void setIdType(const UI32 &idType);
            UI32 getIdType()  const;
            void setId(const UI32 &id);
            UI32 getId()  const;
            void setActive(const bool &active);
            bool getActive()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_entityName;
            UI32  m_idType;
            UI32  m_id;
            bool  m_active;
    };
}
#endif                                            //UIDMANAGEDOBJECT_H
