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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef QOSLOCALNASSHOWMESSAGE_H
#define QOSLOCALNASSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Qos/Common/QosBaseMessages.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosLocalNasShowMessage : public PrismMessage , public QosNasBaseMessage
    {
        private:
        protected:
        public:
            QosLocalNasShowMessage ();
			QosLocalNasShowMessage (const UI32 opCode, const UI32 cmdCode, const UI32 cos, const UI32 dscp,
                const IpV4AddressNetworkMask serverIpAddressPrefix, const string vrfName, const UI32 vlanNumber);
            virtual    ~QosLocalNasShowMessage ();
			virtual void  setupAttributesForSerialization();
            void setIsGwRequest (bool &isGwRequest);
            bool getIsGwRequest ()  const;            
            void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);

// Now the data members

        private:
        protected:
        public:
            bool    m_isGwRequest;
    };
}
#endif                                            //QOSLOCALNASMESSAGE_H
