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
 *   Author : Ramachandran Anantharamn/Rohit Kumar H                       *
 **************************************************************************/

#ifndef AGPGDELMESSAGE_H
#define AGPGDELMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    class AgPGDelMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            AgPGDelMessage ();
            AgPGDelMessage (const UI32 &pgId, const vector<string> &nPorts);

            virtual    ~AgPGDelMessage ();


            void setpgId(const UI32 &pgId);
            UI32 getpgId() const;

            void setnPorts(const vector<string> &nPorts);
            vector<string> getnPorts()  const;

            void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);

// Now the data members

        private:
        protected:
        public:
            UI32 m_pgId;
            vector<string> m_nPorts;


    };
}
#endif                                            //AGPGDELMESSAGE_H
