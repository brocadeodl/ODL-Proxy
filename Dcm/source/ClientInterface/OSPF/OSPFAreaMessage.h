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
 **************************************************************************/

#ifndef OSPFAreaMessage_H
#define OSPFAreaMessage_H

#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFAreaMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFAreaMessage ();
			// Add the 2nd ctor if needed later
            //OSPFAreaMessage (const string &ifName,const UI32 &cmdCode, const IpV4Address &ip_addr);
            virtual    ~OSPFAreaMessage ();

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setAreaId(const string &id);
            string getAreaId()  const;

            void setNssa(const unsigned int &nssa);
            unsigned int getNssa()  const;
            void setStub(const unsigned int &stub);
            unsigned int getStub()  const;
            
			// Add other functions if needed
            void setOpCode(const unsigned int &opCode);
            unsigned int getOpCode()  const;
            void setVirtualLinkRouterId(const IpV4Address &virtualLinkRouterId);
            IpV4Address getVirtualLinkRouterId()  const;
            void setHelloInterval(const UI32 &HelloInterval);
            UI32 getHelloInterval()  const;
            void setRouterDeadInterval(const UI32 &RouterDeadInterval);
            UI32 getRouterDeadInterval()  const;
            void setRetransmitInterval(const UI32 &RetransmitInterval);
            UI32 getRetransmitInterval()  const;
            void setTransDelay(const UI32 &TransDelay);
            UI32 getTransDelay()  const;
            void setAuthType(const OSPFEncrypType &AuthType);
            OSPFEncrypType getAuthType()  const;
            void setEncryptionType(const OSPFEncrypType &EncryptionType);
            OSPFEncrypType getEncryptionType()  const;
            void setAuthKey(const string &AuthKey);
            string getAuthKey()  const;
            void setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime);
            UI32 getMD5KeyActWaitTime()  const;
            void setMD5KeyId(const UI8 &MD5KeyId);
            UI8 getMD5KeyId()  const;
            void setMD5Key(const string &MD5Key);
            string getMD5Key()  const;

			UI64 getReplayCmdCodeBitmap() const;
			void setReplayCmdCodeBitmap(const UI32 &cmdCode);

			string getVRF() const;
			void setVRF(const string &vrfName);

            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
// Now the data members

        private:
			string m_areaId;
            UI32 m_opCode;
            UI32 m_cmd;
            unsigned int m_nssa;
            unsigned int m_stub;

            IpV4Address  m_virtualLinkRouterId;
            UI32  m_HelloInterval;
            UI32  m_RouterDeadInterval;
            UI32  m_RetransmitInterval;
            UI32  m_TransDelay;
            OSPFEncrypType  m_AuthType;
            OSPFEncrypType  m_EncryptionType;
            string  m_AuthKey;
            UI32  m_MD5KeyActWaitTime;
            UI8  m_MD5KeyId;
            string  m_MD5Key;
            string m_vrf;

        protected:
        public:
			// Add other members as needed
			/*
			 * Config Replay specific
			 */
			UI32 m_cmdCodeBitmap;
			 
    };
}
#endif                                            //OSPFAreaMessage_H

