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
 *   Author : Hari Om                                                      *
 **************************************************************************/

#ifndef IP6ACLRULEMANAGEDOBJECT_H
#define IP6ACLRULEMANAGEDOBJECT_H


#include "DcmObjectModel/DcmManagedObject.h"
#include "Ssm/Local/SsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Types/IpV6Address.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class Ip6AclRuleManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            Ip6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
            Ip6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, const UI64 &ruleid,
				const UI32 &action, const WaveUnion  &srcUnion, const IpV6Address &srcHostIpAddress, 
				const IpV6AddressNetworkMask &srcIpAddress,
// const IpV6Address &srcIpAddressMask,
				 const IpAddrType &srcType, 
				const bool &iscount, const bool &islog);
            virtual    ~Ip6AclRuleManagedObject ();
            static string getClassName();
            void setRuleid(const UI64 &ruleid);
            UI64 getRuleid()  const;
            void setAction(const UI32 &action);
            UI32  getAction()  const;
            void setSrcHostIpAddress(const IpV6Address &srcHostIpAddress);
			IpV6Address getSrcHostIpAddress()  const;
            void setSrcIpAddress(const IpV6AddressNetworkMask &srcIpAddress);
			IpV6AddressNetworkMask getSrcIpAddress()  const;
//            void setSrcIpAddressMask(const IpV6Address &srcIpAddressMask);
//            IpV6Address getSrcIpAddressMask()  const;
            void setSrcType(const IpAddrType &srcType);
            IpAddrType getSrcType()  const;
            void setIsCount(const bool &isCount);
            bool getIsCount()  const;
    		void setSrcUnion(const WaveUnion &srcUnion);
    		WaveUnion  getSrcUnion() const;
			void setIsLog (const bool &isLog);
			bool getIsLog() const;
			
// Now the data members

        private:
        protected:
        public:
            UI64      	m_ruleid;
            UI32			m_action;
			WaveUnion  	m_srcUnion; 
            IpV6Address m_srcHostIpAddress;
			IpV6AddressNetworkMask m_srcIpAddress;
//			IpV6Address m_srcIpAddressMask;
            IpAddrType  m_srcType; 
            bool 		m_isCount;
			bool      	m_isLog;
    };
}
#endif                                            //IP6ACLRULEMANAGEDOBJECT_H
