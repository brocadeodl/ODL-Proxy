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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/

#ifndef DHCPRELLOCALCLEARMESSAGE_H 
#define DHCPRELLOCALCLEARMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class DhcpRelLocalClearMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            DhcpRelLocalClearMessage ();
            DhcpRelLocalClearMessage (const UI32 &cmdCode);
            virtual ~DhcpRelLocalClearMessage ();
            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode () const;
			void setServerAddress(const IpV4Address &serverAddress);
        	IpV4Address getServerAddress()  const;
            void setIsDistributionReqd (const bool &isDistributionReqd);
            bool getIsDistributionReqd () const;
            void setLocationIds (const vector<LocationId> &locationIds);
            vector<LocationId> getLocationIds () const;
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);

        private:
        protected:
        public:
            UI32    m_cmdCode;
			IpV4Address  m_serverAddress;
            bool    m_isDistributionReqd;
            vector<LocationId>  m_locationIds;
    };
}
#endif  //DHCPRELLOCALCLEARMESSAGE_H
