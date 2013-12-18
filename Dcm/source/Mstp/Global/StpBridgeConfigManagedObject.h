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

#ifndef STPBRIDGECONFIGMANAGEDOBJECT_H
#define STPBRIDGECONFIGMANAGEDOBJECT_H

#include "Mstp/Global/XstpBridgeConfigManagedObject.h"
//#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalObjectManager;
    class MstpGlobalUpdateBridgeConfigMessage;
    class MstpBridgeConfigMessage;
    class StpBridgeConfigManagedObject : virtual public XstpBridgeConfigManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            StpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager);
            StpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &helloTime);
            virtual    ~StpBridgeConfigManagedObject ();
            static string getClassName();
            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
	    ResourceId setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg);
	    void  setDefaultAttribute();
	    ResourceId getAttributeToMessage(MstpBridgeConfigMessage *&pMsg);


// Now the data members

        private:
        protected:
        public:
            UI32  m_helloTime;
	    bool  m_stp;
    };
}
#endif                                            //STPBRIDGECONFIGMANAGEDOBJECT_H
