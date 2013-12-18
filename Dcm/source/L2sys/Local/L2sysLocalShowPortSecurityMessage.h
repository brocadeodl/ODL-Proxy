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
 *   Author : skrastog                                                     *
 **************************************************************************/

#ifndef L2SYSLOCALSHOWPORTSECURITYMESSAGE_H
#define L2SYSLOCALSHOWPORTSECURITYMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/L2sys/L2sysPortSecShowMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    class L2sysLocalShowPortSecurityMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L2sysLocalShowPortSecurityMessage ();
            L2sysLocalShowPortSecurityMessage (const UI32 &cmdCode, const string &ifName, const UI32 &ifType);
			virtual    ~L2sysLocalShowPortSecurityMessage ();
            void setRbridgeId(const UI32 &rbridgeId);
            UI32 getRbridgeId()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
	    	void setIsGwRequest(bool isGwRequest);
            bool getIsGwRequest() const;
            void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
	    	void transferOutputBuffer(L2sysPortSecShowMessage *&pMsg);
	    	void copyAllBuffers (const PrismMessage &prismMessage);
        	void setLastPortSecurityRecord(const l2sys_show_mac_output_msg_t *lastMacRecord);
        	void getLastPortSecurityRecord(l2sys_show_mac_output_msg_t *lastMacRecord);
        	void setIsFirstRequest(const UI32 &isFirstRequest);
        	UI32 getIsFirstRequest() const;
	    	// Now the data members

        private:
        protected:
        public:
            UI32 m_cmdCode;
            UI32  m_rbridgeId;
            string m_ifName;
            UI32  m_ifType;
	    	bool m_isGwRequest;
//These members will be used to convey the information of lastRecord to the backend
            UI32 m_lastRbridgeId;
            string m_lastIfName;
            string m_lastMac;
            string m_lastType;
            UI32 m_lastVlanId;
            UI32 m_isLast;
            UI32 m_isFirstRequest;
    };
}
#endif                                            //L2SYSLOCALSHOWPORTSECURITYMESSAGE_H
