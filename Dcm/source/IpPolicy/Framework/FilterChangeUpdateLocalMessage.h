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

#ifndef FILTERCHANGEUPDATELOCALMESSAGE_H
#define FILTERCHANGEUPDATELOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

	class FilterChangeUpdateLocalMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			FilterChangeUpdateLocalMessage ();
			FilterChangeUpdateLocalMessage (const UI32 &filterUpdateDelay);
			virtual    ~FilterChangeUpdateLocalMessage ();
			void setFilterUpdateDelay(const UI32 &filterUpdateDelay);
			UI32 getFilterUpdateDelay()  const;
			bool getFilterUpdateDelayFlag()  const;

			FilterChangeUpdateOpCode opCode() const { return m_op_code;};
			void opCode(FilterChangeUpdateOpCode op_code) {m_op_code = op_code;};
			const string& get_backend_err() const { return m_backend_err;}
			void set_backend_err(const string& err) { m_backend_err = err;}

	// Now the data members

		private:
			FilterChangeUpdateOpCode m_op_code;
			UI32  m_filterUpdateDelay;
			bool m_filterUpdateDelayFlag;
			string m_backend_err;
	};

}
#endif									//FILTERCHANGEUPDATELOCALMESSAGE_H
