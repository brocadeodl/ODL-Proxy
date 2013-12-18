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

#ifndef QOSCREATEQOSPHYINTFCONFIGMESSAGE_H
#define QOSCREATEQOSPHYINTFCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosCreateQosPhyIntfConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosCreateQosPhyIntfConfigMessage ();
            QosCreateQosPhyIntfConfigMessage (const string &ifName,const UI64 &ifIndex,const string &id,const string &cosToCosMutationMapName,const string &CosToTrafficClassMapName,const string &ceeMapName,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const SI32 &flowControlPfcCosValue,const SI32 &defaultCosValue,const string &qosTrust,const SI32 &opCode);
            virtual    ~QosCreateQosPhyIntfConfigMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfIndex(const UI64 &ifIndex);
            UI64 getIfIndex()  const;
            void setId(const string &id);
            string getId()  const;
            void setCosToCosMutationMapName(const string &cosToCosMutationMapName);
            string getCosToCosMutationMapName()  const;
            void setCosToTrafficClassMapName(const string &CosToTrafficClassMapName);
            string getCosToTrafficClassMapName()  const;
            void setCeeMapName(const string &ceeMapName);
            string getCeeMapName()  const;
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
            void setOpCode(const SI32 &opCode);
            SI32 getOpCode()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI64  m_ifIndex;
            string  m_id;
            string  m_cosToCosMutationMapName;
            string  m_CosToTrafficClassMapName;
            string  m_ceeMapName;
            SI32  m_flowControlTxEnable;
            SI32  m_flowControlRxEnable;
            SI32  m_flowControlPfcCosValue;
            SI32  m_defaultCosValue;
            string  m_qosTrust;
            SI32  m_opCode;
    };
}
#endif                                            //QOSCREATEQOSPHYINTFCONFIGMESSAGE_H
