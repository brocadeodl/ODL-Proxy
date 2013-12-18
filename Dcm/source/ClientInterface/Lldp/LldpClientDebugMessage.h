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
 *   Author : Glenn Wenig                                           *
 **************************************************************************/

#ifndef LLDPCLIENTDEBUGMESSAGE_H
#define LLDPCLIENTDEBUGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LldpClientDebugMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LldpClientDebugMessage ();
            LldpClientDebugMessage (const string &ifName,
                                    const UI32 &cmdCode, 
                                    const UI32 &dbgFlags, 
                                    const UI32 &mappedId,
                                    const UI32 &lastIfIndex,
                                    const UI32 &numRecords);
            virtual    ~LldpClientDebugMessage ();

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

            void setDbgFlags(const UI32 &dbgFlags);
            UI32 getDbgFlags()  const;

            void setIfName(const string &ifName);
            string getIfName() const;

            void setMappedId (const UI32 &mappedId);
            UI32 getMappedId ()  const;

            void setLastIfIndex(const UI32 lastIfIndex);
            UI32 getLastIfIndex() const;

            void setNumRecords(const UI32 numRecords);
            UI32 getNumRecords() const;

            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (
                const void *pOutputCStructure);

            void getOutputBufferRef (UI32   bufferNum, 
                                    UI32 &size, void *&pBuffer);
// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32    m_cmdCode;
            UI32    m_dbgFlags;
            UI32    m_mappedId;
            UI32    m_lastIfIndex;
            UI32    m_numRecords;
    };
}
#endif                                            //LLDPCLIENTDEBUGMESSAGE_H
