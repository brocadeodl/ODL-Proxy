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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef QOSPHYINTFCONFIGLOCALMANAGEDOBJECT_H
#define QOSPHYINTFCONFIGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosLocalObjectManager;
    class QosPhyIntfConfigLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            QosPhyIntfConfigLocalManagedObject (QosLocalObjectManager *pQosLocalObjectManager);
            QosPhyIntfConfigLocalManagedObject (QosLocalObjectManager *pQosLocalObjectManager,const ObjectId &physicalInterface,const ObjectId &cosMutation,const ObjectId &cosTrafficClass,const ObjectId &ceeMap,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const SI32 &flowControlPfcCosValue,const SI32 &defaultCosValue,const string &qosTrust);
            virtual    ~QosPhyIntfConfigLocalManagedObject ();
            static string getClassName();
            void setPhysicalInterface(const ObjectId &physicalInterface);
            ObjectId getPhysicalInterface()  const;
            void setCosMutation(const ObjectId &cosMutation);
            ObjectId getCosMutation()  const;
            void setCosTrafficClass(const ObjectId &cosTrafficClass);
            ObjectId getCosTrafficClass()  const;
            void setCeeMap(const ObjectId &ceeMap);
            ObjectId getCeeMap()  const;
            void setFlowControlTxEnable(const SI32 &flowControlTxEnable);
            SI32 getFlowControlTxEnable()  const;
            void setFlowControlRxEnable(const SI32 &flowControlRxEnable);
            SI32 getFlowControlRxEnable()  const;
            void setFlowControlPfcCosValue(const SI32 &flowControlPfcCosValue);
            SI32 getFlowControlPfcCosValue()  const;
            void setDefaultCosValue(const SI32 &defaultCosValue);
            SI32 getDefaultCosValue()  const;
            void setQosTrust(const string &qosTrust);
            string getQosTrust()  const;

// Now the data members

        private:
        protected:
        public:
            ObjectId  m_physicalInterface;
            ObjectId  m_cosMutation;
            ObjectId  m_cosTrafficClass;
            ObjectId  m_ceeMap;
            SI32  m_flowControlTxEnable;
            SI32  m_flowControlRxEnable;
            SI32  m_flowControlPfcCosValue;
            SI32  m_defaultCosValue;
            string  m_qosTrust;
    };
}
#endif                                            //QOSPHYINTFCONFIGLOCALMANAGEDOBJECT_H
