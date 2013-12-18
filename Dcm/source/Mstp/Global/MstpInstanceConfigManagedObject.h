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

#ifndef MSTPINSTANCECONFIGMANAGEDOBJECT_H
#define MSTPINSTANCECONFIGMANAGEDOBJECT_H

#include "Mstp/Global/InstanceBaseManagedObject.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "Mstp/Global/MstpGlobalUpdateMstpInstanceConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalObjectManager;
    class MstpInstanceConfigMessage;
    class MstpInstanceConfigManagedObject : virtual public InstanceBaseManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            MstpInstanceConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager);
            MstpInstanceConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &priority);
            virtual    ~MstpInstanceConfigManagedObject ();
            static string getClassName();
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;

	    void setDefaultAttribute();
	    void setDefaultAttribute(const UI8 instanceId);
	    ResourceId setAttributeByMessage(const MstpGlobalUpdateMstpInstanceConfigMessage *pMsg);
            ResourceId getAttributeToMessage(MstpInstanceConfigMessage *&pMsg);


    Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);

// Now the data members

        private:
        protected:
        public:
            UI32  m_priority;
    };
}
#endif                                            //MSTPINSTANCECONFIGMANAGEDOBJECT_H
