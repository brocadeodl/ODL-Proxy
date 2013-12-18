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

#ifndef XSTPPORTINSTANCECONFIGMANAGEDOBJECT_H
#define XSTPPORTINSTANCECONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
	class NsmUpdatePhyInstanceStpConfigMessage;
	class NsmGlobalUpdatePoInstanceStpConfigMessage;
	class NsmUpdatePoInstanceStpConfigSPMessage;
	class MstpPortConfigMessage;
    class XstpPortInstanceConfigManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            XstpPortInstanceConfigManagedObject (WaveObjectManager *pWaveObjectManager);
      //      XstpPortInstanceConfigManagedObject (WaveObjectManager *pWaveObjectManager,const ObjectId &instanceId,const string &port,const UI32 &cost,const UI32 &priority,const bool &guard,const bool &restrictedTcn,const bool &restrictedRole);
            virtual    ~XstpPortInstanceConfigManagedObject ();
            static string getClassName();
            void setInstanceId(const ObjectId &instanceId);
            ObjectId getInstanceId()  const;
            void setId(const UI8 &id);
            UI8 getId()  const;
            void setPort(const string &port);
            string getPort()  const;
            void setCost(const UI32 &cost);
            UI32 getCost()  const;
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setGuard(const bool &guard);
            bool getGuard()  const;
            void setRestrictedTcn(const bool &restrictedTcn);
            bool getRestrictedTcn()  const;
            void setRestrictedRole(const bool &restrictedRole);
            bool getRestrictedRole()  const;
	    ResourceId setAttributeByMessage(NsmGlobalUpdatePoInstanceStpConfigMessage *pMsg);
	    void setDefaultAttribute(UI8 &id, ObjectId & instanceId, string &ifName);
        //template <typename T>
        //ResourceId  setAttributeByMessage(T *pMsg);
        ResourceId  setAttributeByMessage(NsmUpdatePhyInstanceStpConfigMessage *pMsg);
        //ResourceId getAttributeToMessage(NsmUpdatePhyInstanceStpConfigMessage *&pMsg);
        //ResourceId getAttributeToMessage(NsmUpdatePoInstanceStpConfigSPMessage *&pMsg);
        ResourceId getAttributeToMessage(MstpPortConfigMessage *&pMsg);
	bool isDefault();

// Now the data members

        private:
        protected:
        public:
		UI8 m_id;
            ObjectId  m_instanceId;
            string  m_port;
            UI32  m_cost;
            UI32  m_priority;
            bool  m_guard;
            bool  m_restrictedTcn;
            bool  m_restrictedRole;
    };
}
#endif                                            //XSTPPORTINSTANCECONFIGMANAGEDOBJECT_H
