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
 *   Author : nbellari													   *
 **************************************************************************/

#ifndef NSMQOSCOSTAILDROPCONFIGMESSAGE_H
#define NSMQOSCOSTAILDROPCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmQosCosTailDropConfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmQosCosTailDropConfigMessage ();
            NsmQosCosTailDropConfigMessage (const UI32 &cos0Th, const UI32 &cos1Th, const UI32 &cos2Th, const UI32 &cos3Th,
					const UI32 &cos4Th, const UI32 &cos5Th, const UI32 &cos6Th, const UI32 &cos7Th);
            virtual    ~NsmQosCosTailDropConfigMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCmdCode(const SI32 &cmdCode);
            SI32 getCmdCode()  const;
            void setOpCode(const SI32 &OpCode);
            SI32 getOpCode()  const;
			void setIfType(const UI32 &ifType);
			UI32 getIfType() const;
			void setCos0Threshold(const UI32 &cos0Th);
			UI32 getCos0Threshold() const;
			void setCos1Threshold(const UI32 &cos1Th);
			UI32 getCos1Threshold() const;
			void setCos2Threshold(const UI32 &cos2Th);
			UI32 getCos2Threshold() const;
			void setCos3Threshold(const UI32 &cos3Th);
			UI32 getCos3Threshold() const;
			void setCos4Threshold(const UI32 &cos4Th);
			UI32 getCos4Threshold() const;
			void setCos5Threshold(const UI32 &cos5Th);
			UI32 getCos5Threshold() const;
			void setCos6Threshold(const UI32 &cos6Th);
			UI32 getCos6Threshold() const;
			void setCos7Threshold(const UI32 &cos7Th);
			UI32 getCos7Threshold() const;

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            SI32  m_cmdCode;
            SI32  m_OpCode;
			UI32  m_ifType;
			UI32  m_cos0_threshold;
			UI32  m_cos1_threshold;
			UI32  m_cos2_threshold;
			UI32  m_cos3_threshold;
			UI32  m_cos4_threshold;
			UI32  m_cos5_threshold;
			UI32  m_cos6_threshold;
			UI32  m_cos7_threshold;
    };
}
#endif                                            //NSMQOSCOSTAILDROPCONFIGMESSAGE_H
