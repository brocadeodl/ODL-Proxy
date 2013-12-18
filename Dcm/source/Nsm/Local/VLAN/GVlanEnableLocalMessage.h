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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : Venkat														*
 **************************************************************************/

#ifndef GVLANENABLELOCALMESSAGE_H
#define GVLANENABLELOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

using namespace WaveNs;

namespace DcmNs
{
	class GVlanEnableLocalMessage : public PrismMessage
	{
		private :
			void setupAttributesForSerialization ();

		protected :
		public :
			GVlanEnableLocalMessage ();
			virtual ~GVlanEnableLocalMessage ();
			bool getEnable() const;
			void setEnable(const bool &enable);
			bool getUndo() const;
			void setUndo(const bool &undo);
			gvlan_enable_stage_t getCmdCode() const;
			void setCmdCode(const gvlan_enable_stage_t &cmdCode);
		private:

		// Now the data members
		private :
			bool m_enable;
			bool m_undo;
			gvlan_enable_stage_t m_cmdCode;

		protected :
		public :
	};
}
#endif	// GVLANENABLELOCALMESSAGE_H


