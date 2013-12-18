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
 *   Author : mmohan                                                       *
 **************************************************************************/

#ifndef DISCOVERMESSAGES_H
#define DISCOVERMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Attributes/AttributeStringVectorVector.h"

using namespace WaveNs;

namespace DcmNs {

    // ************* DiscoverStatusMessage ************
    class DiscoverStatusMessage: public PrismMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            DiscoverStatusMessage();
            DiscoverStatusMessage(const UI32 &cmdcode, const UI32 &msgType,
                    const string &vcenter);
            virtual ~DiscoverStatusMessage();
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode() const;
            void setMsgType(const UI32 &msgType);
            UI32 getMsgType() const;
            void setVcenter(const string &vcenter);
            string getVcenter() const;

            void getOutputBufferRef(UI32 bufferNum, UI32 &size, void *&pBuffer);

            // Now the data members
        private:
        protected:
        public:
            UI32 m_cmdcode;
            UI32 m_msgType;
            string m_vcenter;
    };

}
#endif  // DISCOVERMESSAGES_H
