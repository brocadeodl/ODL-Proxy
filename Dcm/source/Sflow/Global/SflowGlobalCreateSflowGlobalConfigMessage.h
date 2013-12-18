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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#ifndef SFLOWGLOBALCREATESFLOWGLOBALCONFIGMESSAGE_H
#define SFLOWGLOBALCREATESFLOWGLOBALCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SflowGlobalCreateSflowGlobalConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SflowGlobalCreateSflowGlobalConfigMessage ();
            SflowGlobalCreateSflowGlobalConfigMessage (const bool &enable,const UI32 &pollingInterval,const UI32 &sampleRate,const UI32 &opCode);
            virtual    ~SflowGlobalCreateSflowGlobalConfigMessage ();
            void setEnable(const bool &enable);
            bool getEnable()  const;
            void setPollingInterval(const UI32 &pollingInterval);
            UI32 getPollingInterval()  const;
            void setSampleRate(const UI32 &sampleRate);
            UI32 getSampleRate()  const;
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;

// Now the data members

        private:
        protected:
        public:
            bool  m_enable;
            UI32  m_pollingInterval;
            UI32  m_sampleRate;
            UI32  m_opCode;
    };
}
#endif                                            //SFLOWGLOBALCREATESFLOWGLOBALCONFIGMESSAGE_H
