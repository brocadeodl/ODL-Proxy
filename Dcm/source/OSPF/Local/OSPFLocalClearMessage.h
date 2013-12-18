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
 **************************************************************************/

#ifndef OSPFLocalClearMessage_H
#define OSPFLocalClearMessage_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalClearMessage : public PrismMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFLocalClearMessage ();
            OSPFLocalClearMessage (OSPFLocalClearMessage *pMsg);
			// Add the 2nd ctor if needed later
            //OSPFLocalClearMessage (const string &ifName,const UI32 &cmdCode, const IpV4Address &ip_addr);
            virtual    ~OSPFLocalClearMessage ();

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

			// Add other functions if needed
            int getArgc();
            void setArgv(char **argv, int argc);
            vector<string> getArgv();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setVRF(const string &ifName);
            string getVRF()  const;
		/*	
            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);*/
            bool getisDistributionRequired() const;
            void setisDistributionRequired(const bool& isDistributed);
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
// Now the data members

        public:
            UI32    m_cmd;
            vector<string> m_argv;

			// Add other members as needed
            string  m_ifName;
            string m_vrfName;
            bool m_isDistributed;
#if 0
			string m_mac;
			bool m_static_arp;
			bool m_dynamic;
			IpV4Address	m_ip_addr;
#endif

        protected:
        private:
    };
}
#endif                                            //OSPFLocalClearMessage_H
