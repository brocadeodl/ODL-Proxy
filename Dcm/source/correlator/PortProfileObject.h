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

/*   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Created on: Oct 9, 2012                                               *
 *   Author : Ankit Jindal                                                 *
 **************************************************************************/

#ifndef PORTPROFILEOBJECT_H_
#define PORTPROFILEOBJECT_H_

#include "Framework/Types/Types.h"
#include <string>
#include "Framework/Types/UI32Range.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs {

class PortProfileObject : public ManagementInterfaceMessage {
		private:
		  	void setupAttributesForSerialization ();

        public:
			PortProfileObject();
		    PortProfileObject(const string &profileName,const bool &swMode,const bool &isAccessMode,const UI32 &accessVlan, const bool &isAllowedVlanAll,const bool &isAllowedVlanNone,const UI32Range &vlanRange);

     		void setProfileName(const string &profileName);
            string getProfileName()  const;
      		void setSwMode(const bool &swMode);
            bool getSwMode()  const;
			void setisAccessMode(const bool &isAccessMode);
			bool getisAccessMode()  const;
            void setIsAllowedVlanAll(const bool &isAllowedVlanAll);
            bool getIsAllowedVlanAll()  const;
            void setIsAllowedVlanNone(const bool &isAllowedVlanNone);
            bool getIsAllowedVlanNone()  const;
            void setAccessVlan(const UI32 &accessVlan);
            UI32 getAccessVlan()  const;
            void setVlanRange(const UI32Range &vlanRange);
            UI32Range getVlanRange()  const;
			void setNativeVlanId(const UI32 &nativeVlan);
			UI32 getNativeVlanId()const;
			static bool compare(PortProfileObject* lhs, PortProfileObject* rhs);
			bool fncompid(PortProfileObject* rhs) ;
			static bool compareMac(string lhs, string rhs);
			void isConfigMismatch(PortProfileObject* const &d, bool& updated) ;
			void setMacCount(const UI32 &macCount);
            UI32 getMacCount()  const;
			void setQos(const string &qos);
			string getQos() const;
            void setProfileMacs(const vector<string> &profileMacs);
            vector<string> getProfileMacs()  const;
            void setRemovedProfileMacs(const vector<string> &removedProfileMacs);
			vector<string> getRemovedProfileMacs()  const;
			void setRemovedMacCount(const UI32 &macCount);
			UI32 getRemovedMacCount()  const;
		
		// Now the data members

		private:
        protected:
        public:
            string  	m_profileName;		
			bool    	m_swMode;
			bool    	m_isAccessMode;
			UI32    	m_accessVlan;
			UI32Range  	m_vlanRange;
			bool  		m_isAllowedVlanAll;
            bool  		m_isAllowedVlanNone;
			UI32  		m_nativeVlan;
			UI32  		m_macCount;
			string 		m_qos;
			vector<string> m_profileMacs;
			vector<string> m_removedProfileMacs;
			UI32        m_removedMacCount;
	};

}
#endif /* PORTPROFILEOBJECT_H_ */
