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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef PIMMULTRTRCFGMESSAGE_H
#define PIMMULTRTRCFGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimMultRtrCfgMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PimMultRtrCfgMessage ();
            PimMultRtrCfgMessage (const UI8 &msgType,const UI8 &cmdCode,const bool &enable);
            virtual    ~PimMultRtrCfgMessage ();
            void setMsgType(const UI8 &msgType);
            UI8 getMsgType()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            void setEnable(const bool &enable);
            bool getEnable()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_msgType;
            UI8  m_cmdCode;
            bool  m_enable;
    };
}
#endif                                            //PIMMULTRTRCFGMESSAGE_H
