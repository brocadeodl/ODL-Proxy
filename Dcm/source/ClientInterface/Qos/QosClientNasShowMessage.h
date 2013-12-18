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

#ifndef QOSCLIENTNASSHOWMESSAGE_H
#define QOSCLIENTNASSHOWMESSAGE_H 

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Qos/Common/QosBaseMessages.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosClientNasShowMessage : public DcmManagementInterfaceMessage, public QosNasBaseMessage
    {
        private:
        protected:
        public:
            QosClientNasShowMessage ();
			QosClientNasShowMessage (const UI32 opCode, const UI32 cmdCode, const UI32 cos, const UI32 dscp,
                const IpV4AddressNetworkMask serverIpAddressPrefix, const string vrfName, const UI32 vlanNumber);
            virtual    ~QosClientNasShowMessage ();
			virtual void  setupAttributesForSerialization();
			virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);
            void setMappedId(const UI32 &mappedId);
            UI32 getMappedId() const;


// Now the data members

        private:
        protected:
        public:
		    UI32 m_mappedId;
    };
}
#endif                                            //QOSCLIENTNASSHOWMESSAGE_H
