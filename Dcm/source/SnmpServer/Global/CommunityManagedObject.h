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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef COMMUNITYMANAGEDOBJECT_H
#define COMMUNITYMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "SnmpServer/Global/enumdatatype.h"
using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerObjectManager;
    class CommunityManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            CommunityManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager);
            CommunityManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager,const string &community,const Access &access);
            virtual    ~CommunityManagedObject ();
            static string getClassName();
            void setCommunity(const string &community);
            string getCommunity()  const;
            //string getObjectId()  const;
            void setAccess(const Access &access);
            Access getAccess()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_community;
            Access  m_access;
    };
}
#endif                                            //CommunityManagedObject_H
