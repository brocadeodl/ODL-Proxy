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

#ifndef XSTPPORTVLANCONFIGMANAGEDOBJECT_H
#define XSTPPORTVLANCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeBoolUC.h"


using namespace WaveNs;

namespace DcmNs
{

	class NsmPortStpConfigMessage;
	class MstpPortVlanConfigMessage;
    class XstpPortVlanConfigManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            XstpPortVlanConfigManagedObject (WaveObjectManager *pWaveObjectManager);
#if 0
            XstpPortVlanConfigManagedObject (WaveObjectManager *pWaveObjectManager,const ObjectId &vlanObjectId,const string &port,const UI32 &cost,const UI32 &priority,const bool &guard,const bool &restrictedTcn,const bool &restrictedRole);
#endif
            virtual    ~XstpPortVlanConfigManagedObject ();
            static string getClassName();
            void setVlanObjectId(const ObjectId &vlanObjectId);
            ObjectId getVlanObjectId()  const;
            void setPort(const string &port);
            string getPort()  const;
            void setCost(const UI32 &cost, bool config);
            UI32UC getCost()  const;
            void setPriority(const UI32 &priority, bool config);
            UI32UC getPriority()  const;
            void setId(const UI32 &Id);
            UI32 getId()  const;
            void setGuard(const bool &guard, bool config);
            BoolUC getGuard()  const;
            void setRestrictedTcn(const bool &restrictedTcn, bool config);
            BoolUC getRestrictedTcn()  const;
            void setRestrictedRole(const bool &restrictedRole, bool config);
            BoolUC getRestrictedRole()  const;
			ResourceId setAttributeByMessage(NsmPortStpConfigMessage *pMsg);
			void setDefaultAttribute(int id, ObjectId & vlanObjectId, string &ifName);
			Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);
            ResourceId getAttributeToMessage(MstpPortVlanConfigMessage *&pMsg);

            bool isDefault();
            static bool isConfigDefault(NsmPortStpConfigMessage *pMsg);


// Now the data members

        private:
        protected:
        public:
            ObjectId  m_vlanObjectId;
            string  m_port;
            UI32UC  m_cost;
            UI32  m_id;
            UI32UC  m_priority;
            BoolUC  m_guard;
            BoolUC  m_restrictedTcn;
            BoolUC  m_restrictedRole;
    };
}
#endif                                            //XSTPPORTVLANCONFIGMANAGEDOBJECT_H
