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
 *   Author : Mobin Mohan	                                               *
 **************************************************************************/

#ifndef LACPLOCALSHOWMESSAGE_H 
#define LACPLOCALSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Nsm/Local/PoPaginationLocalMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LacpLocalShowMessage : public PoPaginationLocalMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LacpLocalShowMessage ();
            LacpLocalShowMessage (const string &ifName, const UI32 &poNum, const UI32 &cmdCode, bool isGwRequest);
            virtual ~LacpLocalShowMessage ();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setPoNum (const UI32 &poNum);
            UI32 getPoNum ()  const;            
            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode () const;
            //void setIsGwRequest (bool isGwRequest);
            //bool getIsGwRequest ()  const;            
            void setIfType (const UI32 &ifType);
            UI32 getIfType ()  const;            
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);
            void transferOutputBuffer(LacpLocalShowMessage *&pMsg);

        private:
        protected:
        public:
            string  m_ifName;
            UI32    m_poNum;
            UI32    m_cmdCode;
            //bool    m_isGwRequest;
			UI32    m_ifType;
    };
}
#endif  //LACPLOCALSHOWMESSAGE_H
