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
 *   Author : Sudha Balodia                                                     *
 **************************************************************************/

#ifndef QOSCOSPROFILEANDTHRESHOLDCONFIGMESSAGE_H
#define QOSCOSPROFILEANDTHRESHOLDCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosCosProfileAndThresholdConfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosCosProfileAndThresholdConfigMessage ();
            QosCosProfileAndThresholdConfigMessage (const UI32 &cos,const UI32 &redProfile,const UI32 &tailDrop);
            virtual ~QosCosProfileAndThresholdConfigMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCos(const UI32 &cos);
            UI32 getCos()  const;
            void setRedProfile(const UI32 &redProfile);
            UI32 getRedProfile()  const;
            void setTailDrop(const UI32 &tailDrop);
            UI32 getTailDrop()  const;
            void setCmdCode(const SI32 &cmdCode);
            SI32 getCmdCode()  const;
            void setOpCode(const SI32 &OpCode);
            SI32 getOpCode()  const;
			void setIfType(const SI32 &ifType);
			SI32 getIfType() const;

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32  m_cos;
            UI32  m_redProfile;
            UI32  m_tailDrop;
            SI32  m_cmdCode;
            SI32  m_OpCode;
			SI32  m_ifType;
    };
}
#endif                                            //QosCosProfileAndThresholdConfigMessage_H
