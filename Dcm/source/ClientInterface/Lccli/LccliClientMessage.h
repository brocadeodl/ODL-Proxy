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
 *   Author : Jmaes Chen                                                     *
 **************************************************************************/

#ifndef LCCLICLIENTMESSAGE_H
#define LCCLICLIENTMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ClientInterface/Lccli/LccliClient.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

#define	LCCLI_MSG_CODE  90000

class LccliMsg : public DcmManagementInterfaceMessage
{
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LccliMsg ();
            LccliMsg (const string &Arg, const UI32 &CliCode);
            virtual ~LccliMsg ();
            void setArg(const string &Arg);
            string getArg() const;
            void setCliCode(const UI32 &CliCode);
            UI32 getCliCode() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 	*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, 										UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_Arg;
            UI32 m_CliCode;

        public:
};

}
#endif  //LCCLICLIENTMESSAGE_H
