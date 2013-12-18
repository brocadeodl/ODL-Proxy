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
 *   Author : pasu                                                     *
 **************************************************************************/

#ifndef SSMASSOCRULETOPOLICYMSGPAYLOAD_H
#define SSMASSOCRULETOPOLICYMSGPAYLOAD_H 

//#include "Framework/Types/Types.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Messaging/Local/WaveMessagePayloadBase.h"
#include "Ssm/Local/SsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include <string>

using namespace std;
using namespace WaveNs;

namespace DcmNs
{

    class SsmAssocRuleToPolicyMsgPayload : public WaveMessagePayloadBase
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
			SsmAssocRuleToPolicyMsgPayload (PrismMessage *pPrismMsg);

            SsmAssocRuleToPolicyMsgPayload (PrismMessage *pPrismMsg, const string &policyName,const UI32 &policyType,const UI64 &ruleid,const UI32 &action,const string &srcMac, const bool &srcMask,const string &dstMac,const bool &dstMask, const string &appType, const bool &isCount,const UI32 &opcode);
 
			SsmAssocRuleToPolicyMsgPayload (PrismMessage *pPrismMsg, const string &policyName, 
				const UI32 &policyType, const UI64 &ruleid, const UI32 &action, const string &srcMac, 
				const string &srcMacMask, const AddrType &srcType, const string &dstMac, 
				const string &dstMacMask, const AddrType &dstType, const string &appType, 
				const EtherType &appTypeEnum, const UI16 &appTypeInt, const bool &isCount, 
				const bool &isLog, const UI32 &opcode, const UI8 &userEnteredSeqid);

			virtual    ~SsmAssocRuleToPolicyMsgPayload ();
            void setPolicyName(const string &policyName);
            string getPolicyName()  const;
            void setPolicyType(const UI32 &policyType);
            UI32 getPolicyType()  const;
            void setRuleid(const UI64 &ruleid);
            UI64 getRuleid()  const;
            void setAction(const UI32 &action);
            UI32  getAction()  const;
            void setSrcMac(const string &srcMac);
            string getSrcMac()  const;
            void setSrcMacMask(const string &srcMacMask);
            string getSrcMacMask()  const;
            void setDstMac(const string &dstMac);
            string getDstMac()  const;
            void setDstMacMask(const string &dstMacMask);
            string getDstMacMask()  const;
            void setAppType(const string &appType);
            string getAppType()  const;
            void setIsCount(const bool &isCount);
            bool getIsCount()  const;
            void setIsLog(const bool &isLog);
            bool getIsLog()  const;
            void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;
            void setAppTypeInt(const UI16 &appTypeInt);
            UI16 getAppTypeInt()  const;
            void setAppTypeEnum(const EtherType &appTypeEnum);
            EtherType getAppTypeEnum()  const;
            void setSrcType(const AddrType &srcType);
            AddrType getSrcType()  const;
            void setDstType(const AddrType &dstType);
            AddrType getDstType()  const;

            void setUserEnteredSeqid(const UI8 &userEnteredSeqid);
            UI8 getUserEnteredSeqid()  const;
            void        setPolicyObjectId   (const ObjectId policyObjectId);
            ObjectId    getPolicyObjectId   () const;
// Now the data members

        private:
        protected:
            string  		m_policyName;
            UI32			m_policyType;
            UI64			m_ruleid;
            UI32			m_action;
            string			m_srcMac;
			string			m_srcMacMask;
            bool 			m_srcMask;
			AddrType 		m_srcType;
            string			m_dstMac;
			string			m_dstMacMask;
			bool 			m_dstMask;
            AddrType 		m_dstType;
            string  		m_appType;
            EtherType 		m_appTypeEnum;
            UI16			m_appTypeInt;
            bool  			m_isCount;
            bool  			m_isLog;
            UI32  			m_opcode;
			UI8				m_userEnteredSeqid;
            ObjectId        m_policyObjectId;
        public:
    };
}
#endif                                            //SSMASSOCRULETOPOLICYMSGPAYLOAD_H
