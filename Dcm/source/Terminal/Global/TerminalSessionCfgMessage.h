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
 *   Author : lhu                                                     *
 **************************************************************************/

#ifndef TERMINALSESSIONCFGMESSAGE_H
#define TERMINALSESSIONCFGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "TerminalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class TerminalSessionCfgMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            TerminalSessionCfgMessage ();
            TerminalSessionCfgMessage (const Session_Id &sessionid, const Terminal_cfg &operationtype, const LeafValue_flag &exectimeout_flag, const UI32 &exectimeout);
            virtual    ~TerminalSessionCfgMessage ();
            void setSessionid(const Session_Id &sessionid);
            Session_Id getSessionid()  const;
            void setOperationtype(const Terminal_cfg &operationtype);
            Terminal_cfg getOperationtype()  const;
            void setExectimeout_flag(const LeafValue_flag &exectimeout_flag);
            LeafValue_flag getExectimeout_flag()  const;
            void setExectimeout(const UI32 &exectimeout);
            UI32 getExectimeout()  const;

// Now the data members

        private:
			Session_Id  m_sessionid;
            Terminal_cfg  m_operationtype;
            LeafValue_flag  m_exectimeout_flag;
            UI32  m_exectimeout;
        protected:
        public:
    };
}
#endif                                            //TERMINALSESSIONCFGMESSAGE_H
