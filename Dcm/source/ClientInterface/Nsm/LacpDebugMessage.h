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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#ifndef LACPDEBUGMESSAGE_H
#define LACPDEBUGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

using namespace WaveNs;

namespace DcmNs
{

    class LacpDebugMessage : public DcmManagementInterfaceMessage
    {
    	string ifName;
		UI32 traceLevel;
		UI32 txOrRx;
		LacpDebug_cmd_t cmdCode;
		bool negate;

        protected:
			void setupAttributesForSerialization();

        public:
            LacpDebugMessage ():
            	DcmManagementInterfaceMessage(LACP_DEBUG_DCMD) {traceLevel = txOrRx = 0; negate = false;}
            ~LacpDebugMessage () {}

            void setIfName(const string &iIfName) {ifName = iIfName;}
            void setTraceLevel(const UI32 iLevel) {traceLevel = iLevel;}
            void setNegate(bool iNegate) {negate = iNegate; }
            void setTxOrRx(UI32 iTxOrRx) {txOrRx = iTxOrRx; }
            void setCmdCode(LacpDebug_cmd_t iCmd) { cmdCode = iCmd;}

            const void *getCStructureForInputs ();
            void loadOutputsFromCStructure (const void *pOutputCStructure) {}

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, 
												UI32 &size, void *&pBuffer) {
            	pBuffer = findBuffer(bufferNum, size);
            }
    };
}
#endif  //LACPDEBUGMESSAGE_H
