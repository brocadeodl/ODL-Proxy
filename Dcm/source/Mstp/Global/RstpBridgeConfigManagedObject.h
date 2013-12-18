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

#ifndef RSTPBRIDGECONFIGMANAGEDOBJECT_H
#define RSTPBRIDGECONFIGMANAGEDOBJECT_H

#include "Mstp/Global/XstpBridgeConfigManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalObjectManager;
    class MstpBridgeConfigMessage;
    class RstpBridgeConfigManagedObject : virtual public XstpBridgeConfigManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            RstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager);
            RstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &helloTime,const UI32 &txHoldCount);
            virtual    ~RstpBridgeConfigManagedObject ();
            static string getClassName();
            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
            void setTxHoldCount(const UI32 &txHoldCount);
            UI32 getTxHoldCount()  const;
	    ResourceId setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg);
            void  setDefaultAttribute();
            ResourceId getAttributeToMessage(MstpBridgeConfigMessage *&pMsg);



// Now the data members

        private:
        protected:
        public:
            UI32  m_helloTime;
            UI32  m_txHoldCount;
	    bool  m_rstp;
    };
}
#endif                                            //RSTPBRIDGECONFIGMANAGEDOBJECT_H
