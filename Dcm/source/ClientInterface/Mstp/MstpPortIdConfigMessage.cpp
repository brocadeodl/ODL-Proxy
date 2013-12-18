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

/* **
 * MstpPortIdConfigMessage.cpp
 *
 * Jul 11, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "MstpPortIdConfigMessage.h"
#include "MstpMessageDef.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/DceWaveClientMessage.h"

#define PORT_ID_BUFF_TAG 111

namespace DcmNs {

MstpPortIdConfigMessage::MstpPortIdConfigMessage()
    : DcmManagementInterfaceMessage (MSTP_PORT_ID_CONFIG)
{
    m_hasMore = false;
}

MstpPortIdConfigMessage::~MstpPortIdConfigMessage()
{
}

void MstpPortIdConfigMessage::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization();

//    addSerializableAttribute(new AttributeUI32(&m_idCount, "idCount"));
//    addSerializableAttribute(new AttributeBool(&m_hasMore, "hasMore"));
}

void MstpPortIdConfigMessage::setPortIdInfo(vector<mstp_msg_port_id_info> &portIdList)
{
    UI32 idCount = portIdList.size();
    UI32 size = sizeof(mstp_msg_port_id_config) + ((idCount-1) * sizeof(mstp_msg_port_id_info));
    void *buff = calloc(1, size);
    if (buff == NULL) {
        tracePrintf(TRACE_LEVEL_ERROR, "MstpPortIdConfigMessage::setPortIdInfo : calloc returned NULL");
        return;
    }

    mstp_msg_port_id_config *config = (mstp_msg_port_id_config*) buff;
    config->count = idCount;

    for (UI32 i = 0; i < idCount; ++i) {
        mstp_msg_port_id_info &info = portIdList[i];
        tracePrintf(TRACE_LEVEL_INFO, "MstpPortIdConfigMessage::setPortIdInfo : if_type=%u, if_name=%s, port_id=%u", info.if_type, info.if_name, info.port_id);
        memcpy(config->pid_info+i, &info, sizeof(mstp_msg_port_id_info));
    }

    addBuffer(PORT_ID_BUFF_TAG, size, buff, true);
}


bool MstpPortIdConfigMessage::hasMore() const {
    return m_hasMore;
}

void MstpPortIdConfigMessage::setHasMore(const bool b) {
    m_hasMore = b;
}

const void *MstpPortIdConfigMessage::getCStructureForInputs()
{
    UI32 size;
    return transferBufferToUser(PORT_ID_BUFF_TAG, size);
}

void MstpPortIdConfigMessage::loadOutputsFromCStructure(const void *out)
{
    // Not required..
    removeAllBuffers();
}

}

