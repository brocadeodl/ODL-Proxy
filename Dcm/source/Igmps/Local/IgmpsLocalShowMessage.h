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
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#ifndef IGMPSLOCALSHOWMESSAGE_H 
#define IGMPSLOCALSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class IgmpsLocalShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IgmpsLocalShowMessage ();
            IgmpsLocalShowMessage (const string &grpName, const string &ifName, const UI32 &cmdCode, bool isGwRequest, bool isMgmtCluster);
            virtual ~IgmpsLocalShowMessage ();
            void setGrpName(const string &grpName);
            string getGrpName() const;
            void setIfName(const string &ifName);
            string getIfName() const;
            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode () const;
            void setIsGwRequest (bool isGwRequest);
            bool getIsGwRequest ()  const;            
            void setIsMgmtCluster (bool isMgmtCluster);
            bool getIsMgmtCluster ()  const;
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);
            void transferOutputBuffer(IgmpsLocalShowMessage *&pMsg);
            void setIfType(const UI32 &ifType);
            UI32 getIfType() const;

        private:
        protected:
        public:
            string  m_grpName;
            string  m_ifName;
            UI32    m_cmdCode;
            bool    m_isGwRequest;
            bool    m_isMgmtCluster;
            UI32    m_ifType;
    };
}
#endif  //IGMPSLOCALSHOWMESSAGE_H
