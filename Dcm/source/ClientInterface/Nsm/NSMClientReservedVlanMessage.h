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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : rgandhi                                                *
 **************************************************************************/

#ifndef NSMCLIENTRESERVEDVLANMESSAGE_H
#define NSMCLIENTRESERVEDVLANMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"


using namespace WaveNs;

namespace DcmNs
{

    class NsmClientReservedVlanMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmClientReservedVlanMessage ();
            NsmClientReservedVlanMessage (const UI32 &cmdcode, const UI32 &start, const UI32 &end);
            virtual    ~NsmClientReservedVlanMessage ();
            void setCmdCode(const UI32 &cmdcode);
            UI32 getCmdCode() const;
            void setStart(const UI32 &start);
            UI32 getStart()  const;
            void setEnd(const UI32 &end);
            UI32 getEnd()  const;
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);


        private:
        protected:
        public:
            UI32  m_cmdcode;
            UI32  m_start;
            UI32  m_end;
    };
}
#endif       //NSMCLIENTRESERVEDVLANINTFMESSAGE_H

