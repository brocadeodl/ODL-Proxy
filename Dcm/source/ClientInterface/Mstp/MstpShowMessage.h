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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Navin
 **************************************************************************/

#ifndef MSTPSHOWMESSAGE_H
#define MSTPSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpLocalShowSTPMessage;

    class MstpShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MstpShowMessage ();
            MstpShowMessage (MstpLocalShowSTPMessage *pMessage);
            virtual ~MstpShowMessage ();

            void setInterfaceName(InterfaceType ifType, const string &ifName);
            InterfaceType getIfType() const;
            string getIfName() const;
            void setInstance(const UI32 &instance);
            UI32 getInstance() const;
            void setType(const UI32 &type);
            UI32 getType() const;
            void setMappedId (const UI32 &mappedId);
            UI32 getMappedId ()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;
            void setDebugSeverity(const UI8 &severity);
            UI8  getDebugSeverity() const;

            void setLastIndex(const UI32 lastIndex);
            UI32 getLastIndex() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 
                                                    *pOutputCStructure);


        private:
        protected:
        public:
            InterfaceType m_ifType;
            string m_ifName;
            UI32 m_inst;
            UI32 m_type; //Brief or Detail
            UI32 m_cmdCode;
            UI32  m_mappedId;
            UI8 m_debugSeverity;

            // Last msti id or vlan id
            UI32 m_lastIndex;

    };
}
#endif  //MstpShowMessage_H
