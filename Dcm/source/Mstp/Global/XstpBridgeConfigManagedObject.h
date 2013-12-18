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

#ifndef XSTPBRIDGECONFIGMANAGEDOBJECT_H
#define XSTPBRIDGECONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Mstp/Global/MstpGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

	class MstpGlobalUpdateBridgeConfigMessage;
    class MstpGlobalObjectManager;
    class MstpBridgeConfigMessage;
    class XstpBridgeConfigManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            XstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager);
            XstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &priority,const string &description,const UI32 &errDisableInterval,const bool &errDisableEnable,const UI32 &forwardDelay,const UI32 &maxAge,const MstpPoPathcost &poPathcost,const bool &stpDisable);
            virtual    ~XstpBridgeConfigManagedObject ();
            static string getClassName();
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setDescription(const string &description);
            string getDescription()  const;
            void setErrDisableInterval(const UI32 &errDisableInterval);
            UI32 getErrDisableInterval()  const;
            void setErrDisableEnable(const bool &errDisableEnable);
            bool getErrDisableEnable()  const;
            void setForwardDelay(const UI32 &forwardDelay);
            UI32 getForwardDelay()  const;
            void setMaxAge(const UI32 &maxAge);
            UI32 getMaxAge()  const;
            void setPoPathcost(const MstpPoPathcost  &poPathcost);
            MstpPoPathcost  getPoPathcost()  const;
            void setStpDisable(const bool &stpDisable);
            bool getStpDisable()  const;
            void setBridgeMacByte (UI8 value);
            UI8  getBridgeMacByte () const;

            virtual void setDefaultAttribute();
            virtual ResourceId setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg);
			virtual ResourceId getAttributeToMessage(MstpBridgeConfigMessage *&pMsg);

        private:
        protected:
        public:
            UI32  m_priority;
            string  m_description;
            UI32  m_errDisableInterval;
            bool  m_errDisableEnable;
            UI32  m_forwardDelay;
            UI32  m_maxAge;
            MstpPoPathcost  m_poPathcost;
            bool  m_stpDisable;
            UI8   m_brMacByte;
    };
}
#endif                                            //XSTPBRIDGECONFIGMANAGEDOBJECT_H
