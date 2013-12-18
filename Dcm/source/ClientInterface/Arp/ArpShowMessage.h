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
 *   Author : Mobin Mohan                                                *
 **************************************************************************/

#ifndef ArpShowMessage_H
#define ArpShowMessage_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Arp/Local/ArpShowLocalMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    class ArpShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ArpShowMessage ();
            ArpShowMessage (const string &ifName,const UI32 &cmdCode, const IpV4Address &ip_addr);
            ArpShowMessage(const std::string&, const WaveNs::UI32&, const WaveNs::IpV4Address&,     const std::string&, const bool&, const bool&, const bool&, const int&, const std::string&, const     int&);
            ArpShowMessage (ArpShowLocalMessage *m);
            virtual    ~ArpShowMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setIp_addr(const IpV4Address &ip_addr);
            IpV4Address getIp_addr()  const;
			void setMac(const string &mac);
			string getMac() const;
			void setSummary (const bool &summary);
			bool getSummary () const;		
			void setStatic_arp(const bool &static_arp);
			bool getStatic_arp() const;
			void setDynamic(const bool &dynamic);
			bool getDynamic( ) const;
			void setSkipCount(const int &skip_count);
			int getSkipCount( ) const;
			void setIpMask(const string &ip_mask);
			string getIpMask() const;
			void setLastPrinted(const int &last_printed);
			int getLastPrinted() const;

            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32    m_cmd;
			IpV4Address	m_ip_addr;
			string m_mac;
			bool m_summary;
			bool m_static_arp;
			bool m_dynamic;
			int    m_skip_count;
			string m_ip_mask;
			int    m_last_printed;
            SI32 m_mappedId;
    };
}
#endif                                            //ArpShowMessage_H
