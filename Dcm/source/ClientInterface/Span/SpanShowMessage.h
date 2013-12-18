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

#ifndef SPANSHOWMESSAGE_H
#define SPANSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class SpanShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SpanShowMessage ();
            SpanShowMessage (const UI32 &session_id, const UI32 &messagetype,const UI32 &cmdCode , const UI32 &mappedId);
            virtual    ~SpanShowMessage();

            void setSession_id(const UI32 &session_id);
            UI32 getSession_id()  const;

            void setMessageType(const UI32 &messageType);
            UI32 getMessageType() const;

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;

            void setMappedId(const UI32 &mappedId);
            UI32 getMappedId() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

// Now the data members
        private:
        protected:
        public:
            UI32  m_session_id;
            UI32 m_messageType;
            UI32 m_cmdCode;
			UI32 m_mappedId;
    };
}
#endif      //SPANSHOWMESSAGE_H

