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

#ifndef NSMBEACONMESSAGE_H
#define NSMBEACONMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmBeaconMessage : public DcmManagementInterfaceMessage
	    {
			private:
	        protected:
	            virtual void setupAttributesForSerialization();
	        public:
				NsmBeaconMessage();
	           NsmBeaconMessage(const UI32 cliCode, const UI32 cmdId, const string &ifName, const string &msgStatus);
	         virtual    ~NsmBeaconMessage();
	         void setCmdId(const UI32 cmdId);
	         UI32 getCmdId() const;
	         void setCliCode(const UI32 cliCode);
             UI32 getCliCode() const;
	         void setMsgStatus(const string &msgStatus);
	         string getMsgStatus()  const;
	         void setIf(const string &ifName);
	         string getIf()  const;
			virtual const void *getCStructureForInputs ();
	        virtual void loadOutputsFromCStructure (const void
					*pOutputCStructure);
            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
				UI32 &size, void *&pBuffer);

			private:
	        protected:
	        public:
			    UI32	m_cliCode;
		        UI32    m_cmdId;
		        string  m_if;
	            string  m_msgStatus;
		};
}

#endif
