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
 *   Author : jrodrigu                                                     *
 **************************************************************************/

#ifndef SWITCHCONFIGCFGCHASSISSTATEMESSAGE_H
#define SWITCHCONFIGCFGCHASSISSTATEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SwitchConfigCfgChassisStateMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SwitchConfigCfgChassisStateMessage ();
            SwitchConfigCfgChassisStateMessage (const bool &enableChassis, const bool &beacon);
            virtual    ~SwitchConfigCfgChassisStateMessage ();
            void setChassisEnableFlag(const bool &enableChassis);
            bool getChassisEnableFlag()  const;
            void setBeacon(const bool &beacon);
            bool getBeacon()  const;
            void setResult(const SI32 &result);
            SI32 getResult()  const;

// Now the data members

        private:
        protected:
        public:
            bool  m_enableChassis;
            bool  m_beacon;
            SI32  m_result;
    };
}
#endif                                            //SWITCHCONFIGCFGCHASSISSTATEMESSAGE_H
