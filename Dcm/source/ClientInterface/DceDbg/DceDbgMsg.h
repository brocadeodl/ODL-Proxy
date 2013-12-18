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
 *   Author : Navin
 **************************************************************************/

#ifndef DCE_DBG_MESSAGE_H
#define DCE_DBG_MESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class DceDbgMsg : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            DceDbgMsg ();
            DceDbgMsg (const string &dbgArg, const UI32 &dbgCliCode);
            virtual ~DceDbgMsg ();
            void setDbgArg(const string &dbgArg);
            string getDbgArg() const;
            void setDbgCliCode(const UI32 &dbgCliCode);
            UI32 getDbgCliCode() const;
            void setCookieFlag(const UI8 &cookieFlag);
            UI8 getCookieFlag() const;


            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 
													*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, 
												UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_dbgArg;
            UI32 m_dbgCliCode;
            UI8  m_cookieFlag;

        public:
    };
}
#endif  //DCE_DBG_MESSAGE_H
