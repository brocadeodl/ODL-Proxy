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

#ifndef EXTMACACLRULEMANAGEDOBJECT_H
#define EXTMACACLRULEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Ssm/Global/MacAclRuleManagedObject.h"
#include "vcs.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalObjectManager;
    class ExtMacAclRuleManagedObject : public MacAclRuleManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ExtMacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager);
            ExtMacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,
				const UI64 &ruleid, const UI32 &action, const string &srcMac, const string &srcMacMask,
				const string &srcHost, const AddrType &srcType, const string &dstMac, const string &dstMacMask,
				const AddrType &dstType, const string &dstHost, const EtherType &appType, const UI16 &appTypInt,
				const bool &iscount, const bool &islog); 
				
				
            virtual    ~ExtMacAclRuleManagedObject ();
            static string getClassName();
            void setDstMac(const string &dstMac);
            string getDstMac()  const;
            void setDstMacMask(const string &dstMacMask);
            string getDstMacMask()  const;
            void setAppType(const EtherType &appType);
            EtherType getAppType()  const;
            void setDstHost(const string &dstHost);
            string getDstHost()  const;
            AddrType getDstType()  const;
            void  setDstType(const AddrType &dstType);
			void setDstUnion(const WaveUnion &val);
            WaveUnion getDstUnion() const;
			void setAppTypeUnion(const WaveUnion &val);
            WaveUnion getAppTypeUnion() const;
            void setAppTypeInt(const UI16 &apptype);
            UI16 getAppTypeInt()  const;

// Now the data members

        private:
        protected:
        public:
            string		  m_dstMac;
            string		  m_dstMacMask;
            string		  m_dstHost;
            AddrType	  m_dstType;
            WaveUnion	  m_dstUnion;
            EtherType	  m_appType;
            UI16		  m_appTypeInt;
            WaveUnion	  m_appTypeUnion;
    };
}
#endif                                            //MACACLRULEMANAGEDOBJECT_H
