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
 *   Copyright (C) 2012      Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pasu                                                         *
 **************************************************************************/

#ifndef NSMPHYINTFVRFCONFMESSAGE_H
#define NSMPHYINTFVRFCONFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmPhyIntfVrfConfigMessage : public ManagementInterfaceMessage
    {
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			NsmPhyIntfVrfConfigMessage ();
            NsmPhyIntfVrfConfigMessage (const string &ifName, const UI32 & ifType,
                                            const UI32 &cmdCode,const string &vrfName);
            virtual    ~NsmPhyIntfVrfConfigMessage ();
			void setIfName(const string &ifName);
			string getIfName()  const;
			void setVrfName(const string &vrfName);
			string getVrfName()  const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;


		private:
		protected:
		public:
			string  m_ifName;
			string  m_vrfName;
			UI32  m_cmdCode;
            UI32  m_ifType;
	};
}
#endif                                            //NSMUPDATEPHYINTFMESSAGE_H
