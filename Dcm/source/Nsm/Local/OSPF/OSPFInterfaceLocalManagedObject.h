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

#ifndef OSPFINTERFACELOCALMANAGEDOBJECT_H
#define OSPFINTERFACELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeBoolUC.h"
#include "Framework/Attributes/AttributeStringUC.h"
using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
	class OSPFInterfaceConfigMessage;
    class OSPFInterfaceLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            //virtual void setupKeys();
        public:
            OSPFInterfaceLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
			OSPFInterfaceLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const string &id,const UI32 &Type,const ObjectId &Area,const UI32 &HelloInterval,const UI32 &RouterDeadInterval,const UI32 &RetransmitInterval,const UI32 &TransDelay,const UI32 &Cost,const UI32 &Priority,const UI32 &AuthType,const OSPFEncrypType &EncryptionType,const string &AuthKey,const string &AuthNoEncryptKey, const UI32 &AuthChangeWaitTime,const UI32 &MD5KeyActWaitTime,const UI8 &MD5KeyId, const UI8 &MD5NoEncryptKeyId, const OSPFEncrypType &MD5EncryptionType,const string &MD5Key, const string &MD5NoEncryptKey, const bool &FilterAll, const OSPFIntfDatabaseFilterAllow &FilterAllExt,  const OSPFIntfDatabaseFilterAllow &FilterAllSumExt, const bool &MtuIgnore,const OSPFIntfNetworkType &NetworkType,const bool &Active,const bool &Passive);

            virtual    ~OSPFInterfaceLocalManagedObject ();
            static string getClassName();
            void setId(const string &id);
            string getId()  const;
            void setType(const UI32 &Type);
            UI32 getType()  const;
            void setArea(const ObjectId &Area);
            ObjectId getArea()  const;
            void setHelloInterval(const UI32 &HelloInterval, const bool &set);
            UI32 getHelloInterval()  const;
            void setRouterDeadInterval(const UI32 &RouterDeadInterval, const bool &set);
            UI32 getRouterDeadInterval()  const;
            void setRetransmitInterval(const UI32 &RetransmitInterval);
            UI32 getRetransmitInterval()  const;
            void setTransDelay(const UI32 &TransDelay);
            UI32 getTransDelay()  const;
            void setCost(const UI32 &Cost, const bool &set);
            UI32 getCost()  const;
            void setPriority(const UI32 &Priority);
            UI32 getPriority()  const;
            void setAuthType(const UI32 &AuthType);
            UI32 getAuthType()  const;
            void setEncryptionType(const OSPFEncrypType &EncryptionType, const bool &set);
            OSPFEncrypType getEncryptionType()  const;
            void setAuthKey(const string &AuthKey, const bool &set);
            string getAuthKey()  const;
            void setAuthChangeWaitTime(const UI32 &AuthChangeWaitTime);
            UI32 getAuthChangeWaitTime()  const;
            void setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime, const bool &set);
            UI32 getMD5KeyActWaitTime()  const;
            void setMD5KeyId(const UI8 &MD5KeyId, const bool &set);
            UI8 getMD5KeyId()  const;
            void setMD5NoEncryptKeyId(const UI8 &MD5NoEncryptKeyId, const bool &set);
            UI8 getMD5NoEncryptKeyId()  const;
			void setMD5EncryptionType(const OSPFEncrypType &MD5EncryptionType, const bool &set);
			OSPFEncrypType getMD5EncryptionType() const;
            void setMD5Key(const string &MD5Key, const bool &set);
            string getMD5Key()  const;
            void setMD5NoEncryptKey(const string &MD5NoEncryptKey, const bool &set);
            string getMD5NoEncryptKey()  const;

			void setFilterAll(const bool &FilterAll);
			bool getFilterAll() const;
			void clearDatabaseFilterOptions(void);
			void setFilterAllExt(const UI32 &FilterAllExt);
			UI32 getFilterAllExt() const;
			void setFilterAllSumExt(const UI32 &FilterAllSumExt);
			UI32 getFilterAllSumExt() const;
			
            void setMtuIgnore(const bool &MtuIgnore);
            bool getMtuIgnore()  const;
            void setNetworkType(const OSPFIntfNetworkType &NetworkType);
            OSPFIntfNetworkType getNetworkType()  const;
            void setActive(const bool &Active);
            bool getActive()  const;
            void setPassive(const bool &Passive);
            bool getPassive()  const;

			void updateClientMessageForReplay(OSPFInterfaceConfigMessage *pMsg);

// Now the data members

        private:
        protected:
        public:
            string  m_id;
            UI32  m_Type;
            ObjectId  m_Area;
            UI32UC  m_HelloInterval;
            UI32UC  m_RouterDeadInterval;
            UI32  m_RetransmitInterval;
            UI32  m_TransDelay;
            UI32UC  m_Cost;
            UI32  m_Priority;
            UI32  m_AuthType;
            UI32UC m_EncryptionType;
            StringUC  m_AuthKey;
            StringUC  m_AuthNoEncryptKey;
            UI32  m_AuthChangeWaitTime;
            UI32UC  m_MD5KeyActWaitTime;
            UI32UC  m_MD5KeyId;
            UI32UC  m_MD5NoEncryptKeyId;
			UI32UC m_MD5EncryptionType;
            StringUC  m_MD5Key;
            StringUC  m_MD5NoEncryptKey;

			// for ip ospf database-filter
            bool  m_FilterAll;       // if true, implies the 'all-out' option
            UI32  m_FilterAllExt;    // stores enum OSPFIntfDatabaseFilterAllow
            UI32  m_FilterAllSumExt; // stores enum OSPFIntfDatabaseFilterAllow

            bool  m_MtuIgnore;
            OSPFIntfNetworkType  m_NetworkType;
            bool  m_Active;
            bool  m_Passive;
    };
}
#endif                                            //OSPFINTERFACELOCALMANAGEDOBJECT_H
