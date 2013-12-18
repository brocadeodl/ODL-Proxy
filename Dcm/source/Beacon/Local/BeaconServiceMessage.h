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

#ifndef BEACONSERVICEMESSAGE_H
#define BEACONSERVICEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

#define BEACON_INTERFACE_ON 1
#define BEACON_INTERFACE_OFF 2
#define BEACON_INTERFACE_SHOW 3
#define BEACON_CHASSIS_ON 4
#define BEACON_CHASSIS_OFF 5
#define BEACON_CHASSIS_SHOW 6

#define BEACON_MSG_SUCCESS 0
#define BEACON_MSG_SEND_FAIL 1
#define BEACON_MSG_GEN_ERROR 2
#define BEACON_UNKNOWN_CMD 3
#define BEACON_INVALID_INPUT 4
#define BEACON_INVALID_LINECARD 5
#define BEACON_INVALID_INTERFACE 6

    class BeaconServiceMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            BeaconServiceMessage ();
            BeaconServiceMessage (const UI32 cmdId, const string &ifName, const
	                    SI32 msgStatus, const UI32 ifType);
            virtual    ~BeaconServiceMessage ();
	        void setCmdId(const UI32 cmdId);
	        UI32 getCmdId() const;
            void setIfType(const UI32 ifType);
            UI32 getIfType() const;
            void setStatus(const SI32  msgStatus);
            SI32 getStatus()  const;
	        void setIf(const string &ifName);
            string getIf()  const;

// Now the data members

        private:
        protected:
        public:
	    UI32    m_cmdId;
	    string  m_if;
        SI32    m_status;
        UI32	m_ifType;
    };
}
#endif       
