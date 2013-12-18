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
 *   Author : Senthil kumar                                                *
 **************************************************************************/

#ifndef NSMDEFAULTCONFIGURATIONCMDSMESSAGE_H
#define NSMDEFAULTCONFIGURATIONCMDSMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmDefaultConfigurationCmdsMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmDefaultConfigurationCmdsMessage ();
            NsmDefaultConfigurationCmdsMessage (const UI32 &cmdCode);
            virtual ~NsmDefaultConfigurationCmdsMessage ();
            void setIfName(const string &ifName);
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 
													*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, 
												UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            UI32 m_cmdCode;
    };
}
#endif  //NSMDEFAULTCONFIGURATIONCMDSMESSAGE_H