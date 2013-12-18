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
 *   Author : bvreddy	                                                   *
 **************************************************************************/

#ifndef NSMPORTMEDIASHOWINTFMESSAGE_H
#define NSMPORTMEDIASHOWINTFMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmPortMediaShowIntfMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmPortMediaShowIntfMessage ();
            NsmPortMediaShowIntfMessage (const string &ifTypeName,const string &ifName,const UI32 &slotId, const UI32 &cmdCode,const UI32 &mappedId);
            virtual    ~NsmPortMediaShowIntfMessage ();
			void setIfName(const string &ifName);
			string getIfName()  const;
			void setIfTypeName(const string &ifTypeName);
			string getIfTypeName()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setCmdSlotId(const UI32 &slotId);
            UI32 getCmdSlotId()  const;
			void setMappedId (const UI32 &mappedId);
			UI32 getMappedId ()  const;
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);
            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
// Now the data members

        private:
        protected:
        public:
			string  m_ifTypeName;
			string  m_ifName;
            UI32  m_slotId;
            UI32  m_cmdCode;
			UI32  m_mappedId;
    };
}
#endif                                            //NSMSHOWINTFMESSAGE_H
