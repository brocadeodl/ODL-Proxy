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

#ifndef NSMLOCALPOLICYMAPMESSAGE_H
#define NSMLOCALPOLICYMAPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalPolicyMapMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmLocalPolicyMapMessage ();
            NsmLocalPolicyMapMessage (const string &ifName, const string &mapName, const UI32 &cmdCode, const UI32 &opCode);
            virtual    ~NsmLocalPolicyMapMessage ();
			void setIfName(const string &ifName);
			string getIfName() const;
			void setMapName(const string &mapName);
			string getMapName() const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode() const;
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;


// Now the data members

        private:
        protected:
        public:
			string m_ifName;
			string m_mapName;
            UI32  m_cmdCode;
            UI32  m_opCode;
    };
}
#endif                                            //NsmLocalPolicyMapMessage_H
