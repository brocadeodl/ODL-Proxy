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
 *   Author : Sudha Balodia                                                    *
 **************************************************************************/

#ifndef QOSGLOBALCOSPROFILEANDTHRESHOLDCONFIGMESSAGE_H
#define QOSGLOBALCOSPROFILEANDTHRESHOLDCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalCosProfileAndThresholdConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosGlobalCosProfileAndThresholdConfigMessage ();
            QosGlobalCosProfileAndThresholdConfigMessage (const UI32 &cos,const UI32 &red_profile,const UI32 &tail_drop);
            virtual    ~QosGlobalCosProfileAndThresholdConfigMessage ();
            void setCos(const UI32 &cos);
            UI32 getCos()  const;
            void setRedProfile(const UI32 &red_profile);
            UI32 getRedProfile()  const;
            void setTailDrop(const UI32 &tail_drop);
            UI32 getTailDrop()  const;
            void setCmdCode(const SI32 &cmdCode);
            SI32 getCmdCode()  const;
            void setOpCode(const SI32 &opCode);
            SI32 getOpCode()  const;
			void setPoId(const UI32 &poId);
            UI32 getPoId()  const;


// Now the data members

        private:
        protected:
        public:
			UI32  m_poId;
            UI32  m_cos;
            UI32  m_redProfile;
            UI32  m_tailDrop;
            SI32  m_cmdCode;
            SI32  m_opCode;
    };
}
#endif                                            //NSMLOCALQOSPFCCONFIGMESSAGE_H
