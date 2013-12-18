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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef FCOEMAPMANAGEDOBJECT_H
#define FCOEMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeGlobalObjectManager;
    class FcoeMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FcoeMapManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager);
            FcoeMapManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager,const string &map_name,const ObjectId &cee_map,const vector<ObjectId> &fabric_map_list);
            virtual    ~FcoeMapManagedObject ();
            static string getClassName();
            void setMap_name(const string &map_name);
            string getMap_name()  const;
            void setCee_map(const ObjectId &cee_map);
            ObjectId getCee_map()  const;
            void setFabric_map_list(const vector<ObjectId> &fabric_map_list);
            vector<ObjectId> getFabric_map_list()  const;
            void addFabricMap (const ObjectId &object_id);
            void remFabricMap (const ObjectId &object_id);
            void clearCeeMap ();


// Now the data members

        private:
        protected:
        public:
            string  m_map_name;
            ObjectId  m_cee_map;
            vector<ObjectId>  m_fabric_map_list;
    };
}
#endif                                            //FCOEMAPMANAGEDOBJECT_H
