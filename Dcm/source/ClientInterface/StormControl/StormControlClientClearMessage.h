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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Ojasvita                                                     *
 **************************************************************************/

#ifndef STORM_CONTROL_CLIENT_CLEAR_MESSAGE_H
#define STORM_CONTROL_CLIENT_CLEAR_MESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Ssm/Local/SsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Core/Wave.h"

using namespace WaveNs;

namespace DcmNs
{

    class StormControlClientClearMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            StormControlClientClearMessage();
            StormControlClientClearMessage(const string &ifName, const UI32 &opCode,
				const UI32 &protocol, const UI32 &mappedId);
            virtual ~StormControlClientClearMessage();
			
			void   setIfName      (const string &ifName);
			string getIfName      () const;

			void   setOpCode      (const UI32 &opCode);
			UI32   getOpCode      ()  const;

			void   setProtocol   (const UI32 &protocol);
			UI32   getProtocol   () const;

			void   setMappedId 	  (const UI32 &mappedId);
			UI32   getMappedId 	  ()  const;
			
            virtual const void *getCStructureForInputs ();
            void loadOutputsFromCStructure (const void *pOutputCStructure) {};
        
		private:
        protected:
        public:
			string  m_ifName;
			UI32    m_opCode;
			UI32    m_protocol;
			UI32 	m_mappedId;
    };
}
#endif  //STORM_CONTROL_CLIENT_CLEAR_MESSAGE_H
