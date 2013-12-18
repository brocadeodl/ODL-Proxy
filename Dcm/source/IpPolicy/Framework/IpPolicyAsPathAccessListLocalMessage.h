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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 **************************************************************************/

#ifndef IPPOLICYASPATHACCESSLISTLOCALMESSAGES_H
#define IPPOLICYASPATHACCESSLISTLOCALMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

	class IpPolicyAsPathAccessListLocalMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			IpPolicyAsPathAccessListLocalMessage ();
			IpPolicyAsPathAccessListLocalMessage (const string &name,
				const UI16 &instance,
				const PolicyAction &action,
				const string &RegExpr);
			virtual    ~IpPolicyAsPathAccessListLocalMessage ();
			void setName(const string &name);
			string getName()  const;
			bool getNameFlag()  const;
			void setInstance(const UI16 &instance);
			UI16 getInstance()  const;
			bool getInstanceFlag()  const;
			void setAction(const PolicyAction &action);
			PolicyAction getAction()  const;
			bool getActionFlag()  const;
			void setRegExpr(const string &RegExpr);
			string getRegExpr()  const;
			bool getRegExprFlag()  const;
			IpPolicyOpCode opCode() const { return m_op_code;};
			void opCode(IpPolicyOpCode op_code) {m_op_code = op_code;};
			const string& get_backend_err() const { return m_backend_err;}
			void set_backend_err(const string& err) { m_backend_err = err;}

	// Now the data members

		private:
			IpPolicyOpCode m_op_code;
			string  m_name;
			UI16  m_instance;
			PolicyAction  m_action;
			string  m_RegExpr;
			bool m_name_flag;
			bool m_instance_flag;
			bool m_action_flag;
			bool m_RegExprFlag;
			string m_backend_err;
	};

}
#endif									//IPPOLICYASPATHACCESSLISTLOCALMESSAGES_H
