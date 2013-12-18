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

#ifndef MSTPBRIDGECONFIGMANAGEDOBJECT_H
#define MSTPBRIDGECONFIGMANAGEDOBJECT_H

#include "Mstp/Global/XstpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalObjectManager;
    class MstpBridgeConfigMessage;

//    class InstanceBaseManagedObject;
    //class MstpBridgeConfigManagedObject : virtual public XstpBridgeConfigManagedObject, virtual public InstanceBaseManagedObject
    class MstpBridgeConfigManagedObject : virtual public XstpBridgeConfigManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            MstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager);
            MstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const string &region,const UI16 &revision,const MstpCiscoInterOp &ciscoInterop,const UI32 &txHoldCount,const UI32 &maxHop);
            virtual    ~MstpBridgeConfigManagedObject ();
            static string getClassName();
            void setRegion(const string &region);
            string getRegion()  const;
            void setRevision(const UI16 &revision);
            UI16 getRevision()  const;
            void setCiscoInterop(const MstpCiscoInterOp &ciscoInterop);
            MstpCiscoInterOp getCiscoInterop()  const;
            void setTxHoldCount(const UI32 &txHoldCount);
            UI32 getTxHoldCount()  const;
            void setMaxHop(const UI32 &maxHop);
            UI32 getMaxHop()  const;
	    ResourceId setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg);
            void  setDefaultAttribute();
            ResourceId getAttributeToMessage(MstpBridgeConfigMessage *&pMsg);

    Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);


// Now the data members

        private:
        protected:
        public:
            string  m_region;
            UI16  m_revision;
            MstpCiscoInterOp  m_ciscoInterop;
            UI32  m_txHoldCount;
            UI32  m_maxHop;
	    bool  m_mstp;
	    UI32  m_id;
    };
}
#endif                                            //MSTPBRIDGECONFIGMANAGEDOBJECT_H
