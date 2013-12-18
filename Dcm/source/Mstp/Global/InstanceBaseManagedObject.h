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
 *   Author : sudheend                                                     *
 **************************************************************************/

#ifndef INSTANCEBASEMANAGEDOBJECT_H
#define INSTANCEBASEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"


using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalObjectManager;
    class MstpGlobalUpdateMstpInstanceConfigMessage;
    class MstpInstanceConfigMessage;

    class InstanceBaseManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys ();

        public:
            InstanceBaseManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager);
            InstanceBaseManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI8 &instanceId,const vector<ObjectId> &vlanid);
            virtual    ~InstanceBaseManagedObject ();
            static string getClassName();
            void setInstanceId(const UI8 &instanceId);
            UI8 getInstanceId()  const;
            bool addInstanceVlan (const ObjectId &vlanOid);
            bool removeInstanceVlan (const ObjectId &vlanOid);
            void setInstanceVlans (const vector<ObjectId> &instVlans);
            vector<ObjectId> getInstanceVlans()  const;

            void setDefaultAttribute();
            void setDefaultAttribute(const UI8 instanceId);

            ResourceId setAttributeByMessage (const MstpGlobalUpdateMstpInstanceConfigMessage *pMsg);
            ResourceId getAttributeToMessage (MstpInstanceConfigMessage *&pMsg);

            Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);

// Now the data members

        private:
        protected:
        public:
            UI8  m_instanceId;
            vector<ObjectId>  m_instanceVlans;
    };
}
#endif                                            //INSTANCEBASEMANAGEDOBJECT_H
