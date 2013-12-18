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

#ifndef OSPFVIRTUALLINKLOCALMANAGEDOBJECT_H
#define OSPFVIRTUALLINKLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeStringUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalObjectManager;
	class OSPFAreaMessage;
	class OSPFVirtualLinkLocalMessage;

    class OSPFVirtualLinkLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            OSPFVirtualLinkLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager);
            OSPFVirtualLinkLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const IpV4Address &virtualLinkRouterId,const UI32 &HelloInterval,const UI32 &RouterDeadInterval,const UI32 &RetransmitInterval,const UI32 &TransDelay,const OSPFEncrypType &AuthType,const OSPFEncrypType &EncryptionType,const string &AuthKey, const string &AuthNoEncryptKey, const UI32 &MD5KeyActWaitTime,const UI8 &MD5KeyId, const UI8 &MD5NoEncryptKeyId, const string &MD5Key, const string &MD5NoEncryptKey);
            virtual    ~OSPFVirtualLinkLocalManagedObject ();
            static string getClassName();
            
            void setVirtualLinkRouterId(const IpV4Address &virtualLinkRouterId);
            IpV4Address getVirtualLinkRouterId()  const;
            
            void setHelloInterval(const UI32 &HelloInterval, const bool &set);
            UI32 getHelloInterval()  const;
            
            void setRouterDeadInterval(const UI32 &RouterDeadInterval, const bool &set);
            UI32 getRouterDeadInterval()  const;
            
            void setRetransmitInterval(const UI32 &RetransmitInterval, const bool &set);
            UI32 getRetransmitInterval()  const;
            
            void setTransDelay(const UI32 &TransDelay, const bool &set);
            UI32 getTransDelay()  const;
            
            void setAuthType(const OSPFEncrypType &AuthType, const bool &set);
            OSPFEncrypType getAuthType()  const;
            bool getIsSetAuthType()  const;
            
            void setEncryptionType(const OSPFEncrypType &EncryptionType, const bool &set);
            OSPFEncrypType getEncryptionType()  const;
            bool getIsSetEncryptionType()  const;
            
            void setAuthKey(const string &AuthKey, const bool &set);
            string getAuthKey()  const;
            bool getIsSetAuthKey()  const;
            
            void setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime, const bool &set);
            UI32 getMD5KeyActWaitTime()  const;
            
            void setMD5KeyId(const UI8 &MD5KeyId, const bool &set);
            UI8 getMD5KeyId()  const;
            bool getIsSetMD5KeyId()  const;
            
            void setMD5Key(const string &MD5Key, const bool &set);
            string getMD5Key()  const;
            bool getIsSetMD5Key()  const;

			void updateMOFromLocalMessage(OSPFVirtualLinkLocalMessage *pMsg);
			void updateLocalMessageFromMO(OSPFVirtualLinkLocalMessage *pMsg);
			void updateClientMessageForReplay(OSPFAreaMessage *pMsg);

// Now the data members

        private:
        protected:
        public:
            IpV4Address m_virtualLinkRouterId;
            UI32UC  m_HelloInterval;
            UI32UC  m_RouterDeadInterval;
            UI32UC  m_RetransmitInterval;
            UI32UC  m_TransDelay;
            UI32UC  m_AuthType;
            UI32UC  m_EncryptionType;
            StringUC m_AuthKey;
            StringUC m_AuthNoEncryptKey;
            UI32UC  m_MD5KeyActWaitTime;
            //UI8  m_MD5KeyId;
            UI32UC  m_MD5KeyId;
            UI32UC  m_MD5NoEncryptKeyId;
            StringUC m_MD5Key;
            StringUC m_MD5NoEncryptKey;
    };
}
#endif                                            //OSPFVIRTUALLINKLOCALMANAGEDOBJECT_H
