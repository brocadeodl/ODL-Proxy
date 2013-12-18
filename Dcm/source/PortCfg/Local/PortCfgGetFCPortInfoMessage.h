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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vbagavat                                                     *
 **************************************************************************/

#ifndef PORTCFGGETFCPORTINFOMESSAGE_H
#define PORTCFGGETFCPORTINFOMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgGetFCPortInfoMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgGetFCPortInfoMessage ();
            PortCfgGetFCPortInfoMessage (const UI32 &cmdCode,const UI32 &rbridgeId,const UI32 &slotId,const UI32 &respCode);
            virtual    ~PortCfgGetFCPortInfoMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setRbridgeId(const UI32 &rbridgeId);
            UI32 getRbridgeId()  const;
            void setSlotId(const UI32 &slotId);
            UI32 getSlotId()  const;
            void setRespCode(const UI32 &respCode);
            UI32 getRespCode()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_cmdCode;
            UI32  m_rbridgeId;
            UI32  m_slotId;
            UI32  m_respCode;
    };
}
#endif                                            //PORTCFGGETFCPORTINFOMESSAGE_H
