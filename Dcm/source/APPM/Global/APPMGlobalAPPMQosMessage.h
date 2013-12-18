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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef APPMGLOBALAPPMQOSMESSAGE_H
#define APPMGLOBALAPPMQOSMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalAPPMQosMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMGlobalAPPMQosMessage ();
            APPMGlobalAPPMQosMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const SI32 &defaultCosValue,const bool &qosTrust,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const SI32 &pfcValue,const UI32 &txValue,const UI32 &rxValue,const string &ceeMapName,const string &cosToCosMutationMap,const string &cosToTrafficClassMap);
            virtual    ~APPMGlobalAPPMQosMessage ();
            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            void setProfileName(const string &profileName);
            string getProfileName()  const;
            void setProfileId(const UI32 &profileId);
            UI32 getProfileId()  const;
            void setDefaultCosValue(const SI32 &defaultCosValue);
            SI32 getDefaultCosValue()  const;
            void setQosTrust(const bool &qosTrust);
            bool getQosTrust()  const;
            void setFlowControlTxEnable(const SI32 &flowControlTxEnable);
            SI32 getFlowControlTxEnable()  const;
            void setFlowControlRxEnable(const SI32 &flowControlRxEnable);
            SI32 getFlowControlRxEnable()  const;
            void setPfcValue(const SI32 &pfcValue);
            SI32 getPfcValue()  const;
            void setTxValue(const UI32 &txValue);
            UI32 getTxValue()  const;
            void setRxValue(const UI32 &rxValue);
            UI32 getRxValue()  const;
            void setCeeMapName(const string &ceeMapName);
            string getCeeMapName()  const;
            void setCosToCosMutationMap(const string &cosToCosMutationMap);
            string getCosToCosMutationMap()  const;
            void setCosToTrafficClassMap(const string &cosToTrafficClassMap);
            string getCosToTrafficClassMap()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;
            string  m_profileName;
            UI32  m_profileId;
            SI32  m_defaultCosValue;
            bool  m_qosTrust;
            SI32  m_flowControlTxEnable;
            SI32  m_flowControlRxEnable;
            SI32  m_pfcValue;
            UI32  m_txValue;
            UI32  m_rxValue;
            string  m_ceeMapName;
            string  m_cosToCosMutationMap;
            string  m_cosToTrafficClassMap;
    };
}
#endif                                            //APPMGLOBALAPPMQOSMESSAGE_H
