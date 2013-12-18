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
 *   Author : lhu                                                          *
 **************************************************************************/

#ifndef FIRMWARESHOWACTIONMESSAGE_H
#define FIRMWARESHOWACTIONMESSAGE_H

#include <utils/cpuinfo.h>
#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    typedef struct _ncFwInfoStruct {
        UI32    isChassis;
        UI32    switch_id;
        char    os_name[TMP_FW_MAX_STR_LEN];
        char    os_version[TMP_FW_MAX_VERSION_LEN];
        char    os_copyright[TMP_FW_MAX_STR_LEN];
        char    os_build_time[TMP_FW_MAX_STR_LEN];
        char    os_verstr[TMP_FW_MAX_STR_LEN];
        cpu_info_t  cpu_info;
        UI32    node_cnt;
        struct {
            UI32                slot_no;
            UI32                node_inst_no;
            UI32                node_type;
            bool                is_active_cp;
            fw_app_show_info_t  fw_app_info;
        } fw_nodeinfo[2 * 32];
    } ncFwInfo_t;

#define FW_SHOW_MSG_ID 0

    class FirmwareShowActionMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FirmwareShowActionMessage ();
            FirmwareShowActionMessage (const string &command);
            virtual    ~FirmwareShowActionMessage ();
            void setCommand(const string &command);
            string getCommand()  const;
            void setResult(const string &result);
            string getResult()  const;
            void setApiFlag(const bool &flag);
            bool getApiFlag()  const;
            void getOutputBuffer (UI32 bufferNum, UI32 &size, ncFwInfo_t *&pBuffer);
            void transferOutputBuffer(ncFwInfo_t *pBuffer, UI32 size, UI32 i);

// Now the data members

        private:
        protected:
        public:
            string  m_command;
            string  m_result;
            bool    m_apiFlag;
    };
}
#endif                                            //FIRMWARESHOWACTIONMESSAGE_H
