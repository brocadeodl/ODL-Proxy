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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : skrastog
 **************************************************************************/

#ifndef L2SYSPORTSECSHOWMESSAGE_H
#define L2SYSPORTSECSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{

    class L2sysPortSecShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
			L2sysPortSecShowMessage ();
			L2sysPortSecShowMessage (const UI32 &cmdCode, const string &ifName, const UI32 &ifType);
			virtual ~L2sysPortSecShowMessage ();
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode() const;
			void setIfName(const string &ifName);
			string getIfName() const;
			void setIfType(const UI32 &ifType);
			UI32 getIfType() const;

			void setLastPortSecurityAddressRecord(const l2sys_show_mac_output_msg_t *lastRecord);
			void setIsFirstRequest(const UI32 &isFirstRequest);
			UI32 getIsFirstRequest() const;

			virtual const void *getCStructureForInputs ();
			virtual void loadOutputsFromCStructure (const void 	*pOutputCStructure);

			void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            UI32 m_cmdCode;
			string m_ifName;
			UI32 m_ifType;
			string m_lastIfName;
			string m_lastType;
			string m_lastMac;
			UI32 m_lastVlanId;
			UI32 m_isLast;
			UI32 m_isFirstRequest;
    };
}
#endif  //L2SYSPORTSECSHOWMESSAGE_H
