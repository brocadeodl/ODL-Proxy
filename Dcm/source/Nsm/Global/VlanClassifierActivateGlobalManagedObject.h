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
 *   Author : rmuthigi                                                     *
 **************************************************************************/

#ifndef VLANCLASSIFIERACTIVATEGLOBALMANAGEDOBJECT_H
#define VLANCLASSIFIERACTIVATEGLOBALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class VlanClassifierActivateGlobalManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            VlanClassifierActivateGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            VlanClassifierActivateGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &groupId,const ObjectId &vlanId);
            virtual    ~VlanClassifierActivateGlobalManagedObject ();
            static string getClassName();
            void setGroupId(const UI32 &groupId);
            UI32 getGroupId()  const;
    	    void setVlanId(const ObjectId &vlanId);
            ObjectId getVlanId()  const;
	        void setdummy(const UI32  &dummy);
            UI32  getdummy()  const;
            void setIfName(const string ifName);
            string getIfName()  const;    
			friend bool vlanClassifierPoActivateMoCompare(WaveManagedObject* p1, WaveManagedObject* p2);

// Now the data members

        private:
        protected:
        public:
            UI32  m_groupId;
	        ObjectId  m_vlanId; 
	        UI32 m_dummy;
			string m_ifName;
    };
}
#endif                                            //VLANCLASSIFIERACTIVATELOCALMANAGEDOBJECT_H
