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
 *   Author : nsong                                                     *
 **************************************************************************/

#ifndef VLANBRIDGESTPCONFIGMANAGEDOBJECT_H
#define VLANBRIDGESTPCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalObjectManager;
    class MstpVlanBridgeConfigMessage;
	class MstpGlobalUpdateVlanBridgeConfigMessage;
    class VlanBridgeStpConfigManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            VlanBridgeStpConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager);
#if 0
            VlanBridgeStpConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const ObjectId &vlanObjectId,const UI32UC &priority,const UI32UC &fwdDelay,const UI32UC &maxAge, const UI32UC &agingTime, const UI32UC &helloTime);
#endif
            virtual    ~VlanBridgeStpConfigManagedObject ();
            static string getClassName();
            void setVlanObjectId(const ObjectId &vlanObjectId);
            ObjectId getVlanObjectId()  const;
            void setPriority(const UI32 &priority, bool userConfig = false);
            UI32UC getPriority()  const;
            void setForwardDelay(const UI32 &fwdDelay, bool userConfig = false);
            UI32UC getForwardDelay()  const;
            void setMaxAge(const UI32 &maxAge, bool userConfig = false);
            UI32UC getMaxAge()  const;
            void setAgingTime(const UI32 &agingTime, bool userConfig = false);
            UI32UC getAgingTime()  const;
            void setHelloTime(const UI32 &helloTime, bool userConfig = false);
            UI32UC getHelloTime()  const;
            void setId(const UI32 &id);
            UI32 getId()  const;

			void setDefaultAttribute(int id, ObjectId vlanObjectId);
		    ResourceId setAttributeByMessage(MstpGlobalUpdateVlanBridgeConfigMessage *pMsg);
			ResourceId getAttributeToMessage(MstpVlanBridgeConfigMessage *&pMsg);
			bool isDefault();
			static bool isConfigDefault(MstpGlobalUpdateVlanBridgeConfigMessage *pMsg);


// Now the data members

        private:
        protected:
        public:
            ObjectId  m_vlanObjectId;
            UI32UC  m_priority;
            UI32UC  m_fwdDelay;
            UI32UC  m_maxAge;
            UI32UC  m_agingTime;
            UI32UC  m_helloTime;
			UI32	m_id;
    };
}
#endif                                            //VLANBRIDGESTPCONFIGMANAGEDOBJECT_H
