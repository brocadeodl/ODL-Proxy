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

#ifndef MACACLRULEMANAGEDOBJECT_H
#define MACACLRULEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Ssm/Local/SsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class MacAclRuleManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            MacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
           	MacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,
				const UI64 &ruleid, const UI32 &action, const string &srcMac, 
				const string &srcMacMask, const string &srcHost, const AddrType &srcType, 
				const bool &isCount, const bool &isLog); 
            virtual    ~MacAclRuleManagedObject ();
            static string getClassName();
            //void setPolicyName(const string &policyName);
            //string getPolicyName()  const;
            void setRuleid(const UI64 &ruleid);
            UI64 getRuleid()  const;
            void setAction(const UI32 &action);
            UI32 getAction()  const;
            void setSrcMac(const string &srcMac);
            string getSrcMac()  const;
            void setSrcMacMask(const string &srcMacMask);
            string getSrcMacMask()  const;
            void setSrcType(const AddrType &srcType);
            AddrType getSrcType()  const;
			void setSrcUnion(const WaveUnion &val);
            WaveUnion getSrcUnion() const;
            void setIsCount(const bool &isCount);
            bool getIsCount()  const;
            void setIsLog(const bool &isLog);
            bool getIsLog()  const;
            void setSrcHost(const string &srcHost);
            string getSrcHost()  const;

// Now the data members

        private:
        protected:
        public:
            //string    m_policyName;
            UI64		  m_ruleid;
            UI32 		  m_action;
            string		  m_srcMac;
			string		  m_srcMacMask;
            string        m_srcHost;
			AddrType	  m_srcType;
            WaveUnion	  m_srcUnion;
            bool		  m_isCount;
            bool		  m_isLog;
    };
}
#endif                                            //MACACLRULEMANAGEDOBJECT_H
