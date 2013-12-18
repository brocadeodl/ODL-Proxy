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
 **************************************************************************/

#ifndef RTMSHOWMESSAGE_H
#define RTMSHOWMESSAGE_H
#ifdef __cplusplus
extern "C" {
#endif

void* RtmClientWriterInit();
void RtmClientWriterWriteAP(void* handle, const char* fmt, va_list ap);
void RtmClientWriterWrite(void* handle, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

// Generic Message Class
#ifdef __cplusplus

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include <string>

using namespace WaveNs;

namespace DcmNs
{

    class RtmLocalShowMessage;

    class RtmShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RtmShowMessage (RtmLocalShowMessage *pMessage);
            RtmShowMessage ();
            virtual    ~RtmShowMessage ();

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

			void  setArgs(const vector<string> &args);
            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        void setVrfNAme (const string &vrfName);
        string getVrfName () const;

			void setSequenceNumIntVal(char *hex_str);
// Now the data members

        private:
            UI32 m_cmdCode;
      			UI32 m_rbridgeID;
			      vector<string> m_argv;
            string  m_vrfName; /*This is used for vrf show cmds*/

        protected:
        public:
    };
}
#endif
#endif                                            //RTMSHOWMESSAGE_H
