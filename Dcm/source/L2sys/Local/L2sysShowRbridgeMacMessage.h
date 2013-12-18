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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Eugene Gee
 **************************************************************************/

#ifndef L2SYSSHOWRBRIDGEMACMESSAGE_H
#define L2SYSSHOWRBRIDGEMACMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class L2sysShowRbridgeMacMessage : public PrismMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L2sysShowRbridgeMacMessage ();
            L2sysShowRbridgeMacMessage (const string &ifName,const UI32 &cmdCode,const UI32 &ifindex, const UI32 &type, const bool isGwRequest, const UI32 &fromRbridgeId, const UI32 &forRbridgeId);
            virtual    ~L2sysShowRbridgeMacMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setIfindex(const UI32 &ifindex);
            UI32 getIfindex() const;
            void setType(const UI32 &type);
            UI32 getType() const;
            void setIsGwRequest (bool isGwRequest);
            bool getIsGwRequest ()  const;
            void setFromRbridgeId(const UI32 &fromRbridgeId);
            UI32 getFromRbridgeId() const;
            void setForRbridgeId(const UI32 &forRbridgeId);
            UI32 getForRbridgeId() const;
            void getOutputBufferRef (UI32   bufferNum,UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32  m_cmdCode;
            UI32 m_ifindex;
            UI32 m_type;
            bool m_isGwRequest;
            UI32 m_from_rbridge_id;	/* Get MAC address table from this RBridge */
            UI32 m_for_rbridge_id;	/* Get MAC address table information for this RBridge */
    };
}
#endif  //L2SYSSHOWRBRIDGEMACMESSAGE_H
