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
 *   Author : rmuthigi                                                     *
 **************************************************************************/

#ifndef VLANCLASSIFIERRULESMANAGEDOBJECT_H
#define VLANCLASSIFIERRULESMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class VlanClassifierRulesManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
			VlanClassifierRulesManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
			VlanClassifierRulesManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &ruleId,const string &mac,const ProtoType &protoValEnum,const string &protoValStr,const WaveUnion  &appType,const EncapType &encap);
			virtual    ~VlanClassifierRulesManagedObject ();
			virtual UI32 getCase (const UI32 &userChoiceWyserTag);
			static string getClassName();
			void setRuleId(const UI32 &ruleId);
			UI32 getRuleId()  const;
			void setMac(const string &mac);
			string getMac()  const;
			void setProtoValEnum(const ProtoType &protoValEnum);
			ProtoType getProtoValEnum()  const;
			void setProtoValStr(const string &protoValStr) ;
			string getProtoValStr() const;
			void setAppType(const WaveUnion &appType);
			WaveUnion getAppType()  const;
			void setEncap(const EncapType &encap);
			EncapType getEncap()  const;
			UI32 getConfigTypeChoice () const;
			void setConfigTypeChoice (const UI32 &configTypeChoice);
			friend bool vlanClassifierRuleMoCompare(WaveManagedObject* p1, WaveManagedObject* p2);

// Now the data members

        private:
        protected:
        public:
            UI32  m_ruleId;
            string  m_mac;
            ProtoType  m_protoValEnum;
	    string m_protoValStr;
	    WaveUnion  m_appType;
            EncapType  m_encap;
	    UI32 m_configTypeChoice;
    };
}
#endif                                            //VLANCLASSIFIERRULESMANAGEDOBJECT_H
