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
 ***************************************************************************/

#ifndef CONNECTORSHOWMESSAGE_H
#define CONNECTORSHOWMESSAGE_H

#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class ConnectorShowMessage : public DcmManagementInterfaceMessage
    {
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			ConnectorShowMessage ();
			ConnectorShowMessage (const UI32 &cmdCode, const UI32 &linecard, const UI32 &port);
			virtual ~ConnectorShowMessage ();
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode() const;
			void setLinecard(const UI32 &linecard);
			UI32 getLinecard() const;
			void setPort(const UI32 &port);
			UI32 getPort() const;

			virtual const void *getCStructureForInputs    ();
			virtual void loadOutputsFromCStructure (const void *pOutputCStructure);
			void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

		private:
		protected:
		public:
			UI32 m_cmdCode;
			UI32 m_linecard;
			UI32 m_port;
	};
}
#endif                                            //CONNECTORSHOWMESSAGE_H

