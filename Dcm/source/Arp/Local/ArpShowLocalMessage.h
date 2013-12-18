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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vkarnati                                                     *
 **************************************************************************/

#ifndef ARPSHOWLOCALMESSAGE_H
#define ARPSHOWLOCALMESSAGE_H

#include "Framework/Types/IpV4Address.h"
#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class ArpShowLocalMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ArpShowLocalMessage ();
			ArpShowLocalMessage(ArpShowLocalMessage *m);
			ArpShowLocalMessage(const std::string&, const WaveNs::UI32&, const WaveNs::IpV4Address&, const std::string&, const bool&, const bool&, const bool&, const int&, const std::string&, const int&);
            virtual    ~ArpShowLocalMessage ();
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

			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);
            bool getisDistributionRequired() const { return m_is_distribution_required;}
            void setisDistributionRequired(const bool f) { m_is_distribution_required = f;}
            vector<UI32> getLocationIds(void) const { return m_locs ;}
            void setLocationIds(vector<UI32> locations) { m_locs = locations;}
// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
			string m_mac;
			bool m_summary;
			bool m_static_arp;
			bool m_dynamic;
            UI32    m_cmd;
			IpV4Address	m_ip_addr;
			int    m_skip_count;
            string m_ip_mask;
			int    m_last_printed;
            bool m_is_distribution_required;
            vector<UI32>  m_locs;
    };
}
#endif                                            //ARPSHOWLOCALMESSAGE_H
