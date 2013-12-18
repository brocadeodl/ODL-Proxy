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
 *   Author : vbagavat & mlachire                                                    *
 **************************************************************************/

#ifndef PORTCFGFCPORTTRANSLATEMESSAGE_H
#define PORTCFGFCPORTTRANSLATEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgFCPortTranslateMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgFCPortTranslateMessage ();
            PortCfgFCPortTranslateMessage (const string &id,const SI32 &switchPort,const UI32 &rbridgeId);
            virtual    ~PortCfgFCPortTranslateMessage ();
            void setId(const string &id);
            string getId()  const;
            void setSwitchPort(const SI32 &switchPort);
            SI32 getSwitchPort()  const;
            void setRbridgeId(const UI32 &rbridgeId);
            UI32 getRbridgeId()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_id;
            SI32  m_switchPort;
            UI32  m_rbridgeId;
    };
}
#endif                                            //PORTCFGFCPORTTRANSLATEMESSAGE_H
