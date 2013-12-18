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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef OSPFINTERFACECONFIGMESSAGE_H
#define OSPFINTERFACECONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "ClientInterface/OSPF/OSPFRouterConfigMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    class OSPFInterfaceConfigMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFInterfaceConfigMessage ();
            OSPFInterfaceConfigMessage (const UI8 &opCode,const UI8 &cmdCode,const string &intfName,const UI32 &intfType,const string &areaId,const UI32 &HelloInterval,const UI32 &RouterDeadInterval,const UI32 &RetransmitInterval,const UI32 &TransDelay,const UI32 &Cost,const UI32 &Priority,const UI32 &AuthType,const OSPFEncrypType &EncryptionType,const OSPFEncrypType &MD5EncryptionType,const string &AuthKey,const UI32 &AuthChangeWaitTime,const UI32 &MD5KeyActWaitTime,const UI32 &MD5KeyId,const string &MD5Key,const bool &FilterAll,const UI32 &FilterOption,const bool &mtuIgnore,const bool &Active,const bool &Passive,const UI32 &NetworkType);
            virtual    ~OSPFInterfaceConfigMessage ();
            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
			void setintfName(const string &intfName);
			string getintfName() const;
            UI64 getIfindex() const;
            void setIfindex(const UI64 &ifindex);
			void setintfType(const UI32 &intfType);
			UI32 getintfType() const;
            void setAreaId(const string &areaId);
            string getAreaId()  const;
            void setHelloInterval(const UI32 &HelloInterval);
            UI32 getHelloInterval()  const;
            void setRouterDeadInterval(const UI32 &RouterDeadInterval);
            UI32 getRouterDeadInterval()  const;
            void setRetransmitInterval(const UI32 &RetransmitInterval);
            UI32 getRetransmitInterval()  const;
            void setTransDelay(const UI32 &TransDelay);
            UI32 getTransDelay()  const;
            void setCost(const UI32 &Cost);
            UI32 getCost()  const;
            void setPriority(const UI32 &Priority);
            UI32 getPriority()  const;
            void setAuthType(const UI32 &AuthType);
            UI32 getAuthType()  const;
            void setEncryptionType(const OSPFEncrypType &EncryptionType);
            OSPFEncrypType getEncryptionType()  const;
            void setMD5EncryptionType(const OSPFEncrypType &MD5EncryptionType);
            OSPFEncrypType getMD5EncryptionType()  const;
            void setAuthKey(const string &AuthKey);
            string getAuthKey()  const;
            void setAuthChangeWaitTime(const UI32 &AuthChangeWaitTime);
            UI32 getAuthChangeWaitTime()  const;
            void setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime);
            UI32 getMD5KeyActWaitTime()  const;
            void setMD5KeyId(const UI32 &MD5KeyId);
            UI32 getMD5KeyId()  const;
            void setMD5Key(const string &MD5Key);
            string getMD5Key()  const;
            void setFilterAll(const bool &FilterAll);
            bool getFilterAll()  const;
            void setFilterOption(const UI32 &FilterOption);
            UI32 getFilterOption()  const;
            void setmtuIgnore(const bool &mtuIgnore);
            bool getmtuIgnore()  const;
            void setActive(const bool &Active);
            bool getActive()  const;
            void setPassive(const bool &Passive);
            bool getPassive()  const;
            void setNetworkType(const UI32 &NetworkType);
            UI32 getNetworkType()  const;
            UI32 getReplayCmdCodeBitmap() const;
            void setReplayCmdCodeBitmap(const OSPFIntfCmdCode &cmdCode);
			void prepareCMessage(interfaceOSPFConfigMsg_t **pInput);
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);
            void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
			void dumpMessage() const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;
			string m_intfName;
			UI32 m_intfType;
            string  m_areaId;
            UI32  m_HelloInterval;
            UI32  m_RouterDeadInterval;
            UI32  m_RetransmitInterval;
            UI32  m_TransDelay;
            UI32  m_Cost;
            UI32  m_Priority;
            UI32  m_AuthType;
            OSPFEncrypType m_EncryptionType;
            OSPFEncrypType m_MD5EncryptionType;
            string  m_AuthKey;
            UI32  m_AuthChangeWaitTime;
            UI32  m_MD5KeyActWaitTime;
            UI32  m_MD5KeyId;
            string  m_MD5Key;
            bool  m_FilterAll;
            UI32  m_FilterOption;
            bool  m_mtuIgnore;
            bool  m_Active;
            bool  m_Passive;
            UI32  m_NetworkType;
			UI32 m_replayCmdCodeBitmap;
			UI64 m_if_index;
    };
}
#endif                                            //OSPFINTERFACECONFIGMESSAGE_H
