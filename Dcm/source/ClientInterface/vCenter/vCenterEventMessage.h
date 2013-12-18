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
 *   Author : Ritesh Madapurath                                            *
 **************************************************************************/

#ifndef APPMPROFILEMESSAGE_H
#define APPMPROFILEMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class vCenterEventMessage : public DcmManagementInterfaceMessage
    {
        private:

        protected:
            virtual void setupAttributesForSerialization();
        public:
            vCenterEventMessage ();
            vCenterEventMessage (const UI8 &opCode,const UI8 &cmdCode);
            virtual    ~vCenterEventMessage ();
        const virtual void *getCStructureForInputs();
        virtual void loadOutputsFromCStructure(const void *pOutputCStructure);
        UI8 getOpCode() const;
        void setOpCode(UI8 opCode);

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;


    };
}
#endif //APPMPROFILEMESSAGE_H
