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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#ifndef PBRShowMessage_h
#define PBRShowMessage_h

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class PBRShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PBRShowMessage ();
            PBRShowMessage (const UI32 &opCode, const string &routeMapName, const string &ifName,
                    const string &rbridgeId, const UI32 &ifType, bool isGwRequest);
            virtual     ~PBRShowMessage ();

            void   setOpCode      (const UI32 &opCode);
            UI32   getOpCode      ()  const;

            void   setRouteMapName (const string &routeMapName);
            string getRouteMapName ()  const;

            void   setIfName      (const string &ifName);
            string getIfName      () const;

            void setRbridgeId(const string &rbridgeId);
            string getRbridgeId() const;

            void   setIfType      (const UI8 &ifType);
            UI8   getIfType   () const;

            void   setIsGwRequest (bool isGwRequest);
            bool   getIsGwRequest ()  const;

            void   setLastStanza (const UI16 &lastStannza);
            UI16   getLastStanza ()  const;

            void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);

        // Now the data members

        private:
        protected:
        public:
            UI32    m_opCode;
            string  m_routeMapName;
            string  m_ifName;
            string  m_rbridgeId;
            UI8     m_ifType;
            bool    m_isGwRequest;
            UI16    m_lastStanza;
    };
}

#endif                                            //PBRShowMessage_h

