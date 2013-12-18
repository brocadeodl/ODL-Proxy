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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef SYSTEMMONITORGLOBALCHANGEPOWERALERTMESSAGE_H
#define SYSTEMMONITORGLOBALCHANGEPOWERALERTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorGlobalChangePowerAlertMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SystemMonitorGlobalChangePowerAlertMessage ();
            SystemMonitorGlobalChangePowerAlertMessage (const UI32 &action,const UI32 &state);
            virtual    ~SystemMonitorGlobalChangePowerAlertMessage ();
            void setAction(const UI32 &action);
            UI32 getAction()  const;
            void setState(const UI32 &state);
            UI32 getState()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_action;
            UI32  m_state;
    };
}
#endif                                            //SYSTEMMONITORGLOBALCHANGEPOWERALERTMESSAGE_H
