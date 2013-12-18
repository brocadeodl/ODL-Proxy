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

/******************************************************************************
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.                  *
 *   All rights reserved.                                                     *
 *   Author : Mobin Mohan	                                                  *
 *****************************************************************************/

#ifndef CONFIGMGMTLOCALSHOWMESSAGE_H 
#define CONFIGMGMTLOCALSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class ConfigMgmtLocalShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ConfigMgmtLocalShowMessage ();
            ConfigMgmtLocalShowMessage (const UI32 &cmdCode,
									const UI32 &mappedId, bool isGwRequest);
            virtual ~ConfigMgmtLocalShowMessage ();
            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode () const;
            void setMappedId(const UI32 &mappedId);
            UI32 getMappedId() const;
            void setIsGwRequest (bool isGwRequest);
            bool getIsGwRequest ()  const;            
            void setResult(const string &result);
            string getResult()  const;
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);
			void copyAllResult (ConfigMgmtLocalShowMessage &pMsg);
			   
            void transferOutputBuffer(ConfigMgmtLocalShowMessage *&pMsg);

        private:
        protected:
        public:
            UI32    m_cmdCode;
            UI32    m_mappedId;
            bool    m_isGwRequest;
            string  m_result;
    };
}
#endif  //CONFIGMGMTLOCALSHOWMESSAGE_H
