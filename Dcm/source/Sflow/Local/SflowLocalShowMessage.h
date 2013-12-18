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
 *   Author : Venkat                                                       *
 **************************************************************************/

#ifndef SFLOWLOCALSHOWMESSAGE_H
#define SFLOWLOCALSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SflowLocalShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SflowLocalShowMessage ();
            SflowLocalShowMessage (const string &ifName, const UI32 &cmdCode, bool &isGwRequest);
            virtual    ~SflowLocalShowMessage ();
            
            void setIfName (const string &ifName);
            string getIfName ()  const;

            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode ()  const;

            void setIsGwRequest (bool &isGwRequest);
            bool getIsGwRequest ()  const;

			void setMappedId(const UI32 &mappedId);
			UI32 getMappedId() const;


            void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
            
            void copyAllBuffers (const PrismMessage &prismMessage);
            void setInterface (const string &interface);
            string getInterface ()  const;
// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32    m_cmdCode;
            bool    m_isGwRequest;
			string  m_interface;
			UI32 m_mappedId;
    };
}
#endif                                            //SFLOWLOCALSHOWMESSAGE_H
