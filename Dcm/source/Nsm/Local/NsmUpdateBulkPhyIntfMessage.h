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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef NSMUPDATEBULKPHYINTFMESSAGE_H
#define NSMUPDATEBULKPHYINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Attributes/AttributeUI32Range.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmUpdateBulkPhyIntfMessage : public ManagementInterfaceMessage
    {
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			NsmUpdateBulkPhyIntfMessage ();
			virtual ~NsmUpdateBulkPhyIntfMessage ();
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode()  const;
            void setMsgId(const UI32 &msgId);
            UI32 getMsgId() const;
            void loadOutputsFromCStructure (unsigned int size, void *pOutputCStructure);
			void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
		private:
		protected:
		public:
			UI32  m_cmdCode;
            UI32 m_msgId;
	};
}
#endif                                            //NSMUPDATEPHYINTFMESSAGE_H
