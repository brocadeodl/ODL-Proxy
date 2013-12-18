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
 *   Author : pasu														   *
 **************************************************************************/

#ifndef NSMEVENTMESSAGE_H
#define NSMEVENTMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmEventMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmEventMessage ();
            NsmEventMessage (const UI32 &event, const UI32 &parameter);
            virtual    ~NsmEventMessage ();
            void setEventId(const UI32 &event);
            UI32 getEventId()  const;
            void setParameter(const UI32 &parameter);
            UI32 getParameter()  const;

        private:
        protected:
        public:
            UI32  m_eventId;
            UI32  m_parameter;
    };
}
#endif
