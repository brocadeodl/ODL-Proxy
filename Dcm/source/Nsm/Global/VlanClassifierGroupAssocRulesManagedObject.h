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
#ifndef VLANCLASSIFIERGROUPASSOCRULESMANAGEDOBJECT_H
#define VLANCLASSIFIERGROUPASSOCRULESMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"


using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class VlanClassifierGroupAssocRulesManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
			VlanClassifierGroupAssocRulesManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
			VlanClassifierGroupAssocRulesManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &groupId,const vlanClassifierOperation &oper,const UI32  &dummy,const ObjectId &ruleId);
			virtual    ~VlanClassifierGroupAssocRulesManagedObject ();
			static string getClassName();
			void setGroupId(const UI32 &groupId);
			UI32 getGroupId()  const;
			void setRuleId(const ObjectId &ruleId);
			ObjectId getRuleId()  const;
			void setoper(const vlanClassifierOperation  &oper);
			vlanClassifierOperation  getoper()  const;
			void setdummy(const UI32  &dummy);
			UI32  getdummy()  const;
			friend bool vlanClassifierGroupMoCompare(WaveManagedObject* p1, WaveManagedObject* p2);

			// Now the data members

		private:
		protected:
		public:
			UI32  m_groupId;
			vlanClassifierOperation m_oper;
			UI32  m_dummy;
			ObjectId  m_ruleId;
    };
}
#endif                                            //VLANCLASSIFIERGROUPASSOCRULESMANAGEDOBJECT_H
