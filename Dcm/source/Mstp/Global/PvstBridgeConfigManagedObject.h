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

#ifndef PVSTBRIDGECONFIGMANAGEDOBJECT_H
#define PVSTBRIDGECONFIGMANAGEDOBJECT_H

#include "Mstp/Global/XstpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalObjectManager;
	class MstpGlobalUpdateBridgeConfigMessage;
	class MstpBridgeConfigMessage;

    class PvstBridgeConfigManagedObject : virtual public XstpBridgeConfigManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PvstBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager);
            PvstBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &helloTime,const MstpMacReduction &macReduction,const vector<ObjectId> &activeVlan,const vector<ObjectId> &inactiveVlan);
            virtual    ~PvstBridgeConfigManagedObject ();
            static string getClassName();
            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
            void setMacReduction(const  MstpMacReduction &macReduction);
            MstpMacReduction getMacReduction()  const;
            void setActiveVlan(const vector<ObjectId> &activeVlan);
            vector<ObjectId> getActiveVlan()  const;
            void setInactiveVlan(const vector<ObjectId> &inactiveVlan);
            vector<ObjectId> getInactiveVlan()  const;

            ResourceId setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg);
		    void  setDefaultAttribute();

			ResourceId getAttributeToMessage(MstpBridgeConfigMessage *&pMsg);


// Now the data members

        private:
        protected:
        public:
            UI32  m_helloTime;
            MstpMacReduction  m_macReduction;
			bool  m_stp;
    };

}
#endif                                            //PVSTBRIDGECONFIGMANAGEDOBJECT_H
