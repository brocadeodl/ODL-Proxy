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

#ifndef OSPFDebugMessage_H
#define OSPFDebugMessage_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFDebugMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFDebugMessage ();
			// Add the 2nd ctor if needed later
            //OSPFDebugMessage (const string &ifName,const UI32 &cmdCode, const IpV4Address &ip_addr);
            virtual    ~OSPFDebugMessage ();

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

			// Add other functions if needed
            int getArgc();
            void setArgv(char **argv, int argc);
            void setNoArgv(char **argv, int argc);
            vector<string> getArgv();
            void setVRF(const string &vrfName);
            string getVRF()  const;
 
			#if 0
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIp_addr(const IpV4Address &ip_addr);
            IpV4Address getIp_addr()  const;
			void setMac(const string &mac);
			string getMac() const;
			void setStatic_arp(const bool &static_arp);
			bool getStatic_arp() const;
			void setDynamic(const bool &dynamic);
			bool getDynamic( ) const;
			#endif

            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
// Now the data members

        private:
            UI32    m_cmd;
            vector<string> m_argv;
	    string m_vrfName;

			// Add other members as needed
#if 0
            string  m_ifName;
			string m_mac;
			bool m_static_arp;
			bool m_dynamic;
			IpV4Address	m_ip_addr;
#endif

        protected:
        public:
    };
}
#endif                                            //OSPFDebugMessage_H
