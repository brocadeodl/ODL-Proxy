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
 * UidToolKit.cpp
 *
 * Jul 09, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "UidToolKit.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "DcmCore/DcmToolKit.h"
#include "UidService/Global/UidServiceMessage.h"
#include "DcmResourceIds.h"

namespace DcmNs {

ResourceId UidToolKit::invoke(UidServiceMessage &msg)
{
    ValClientSynchronousConnection *conn = DcmToolKit::getInterDcmCcmClient();
    ResourceId status = conn->sendSynchronouslyToWaveServer(&msg);
    if (status != WAVE_MESSAGE_SUCCESS) {
        tracePrintf(TRACE_LEVEL_ERROR, "UidToolKit::invoke : Sending message to UidService failed..");
        return status;
    }

    if ((status = msg.getCompletionStatus()) != WAVE_MESSAGE_SUCCESS) {
        tracePrintf(TRACE_LEVEL_ERROR, "UidToolKit::invoke : UidService returned error");
        return status;
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId UidToolKit::invoke(const UI32 opcode, const UI32 idType, const UI32 id, const string &name)
{
    UidServiceMessage msg (opcode, name, idType, id);
    return invoke(msg);
}


ResourceId UidToolKit::alloc(UI32 &o_uid, UI32 idType, const string &name)
{
    UidServiceMessage message (UID_ALLOC, name, idType, 0);
    ResourceId status = invoke(message);
    if (status == WAVE_MESSAGE_SUCCESS) {
        o_uid = message.getId();
        tracePrintf(TRACE_LEVEL_INFO, "UidToolKit::alloc : uid=%u, idType=%u, name:%s" , o_uid, idType, name.c_str());
    }
    return status;
}

ResourceId UidToolKit::set(const UI32 idType, const UI32 id, const string& name)
{
    return invoke(UID_SET, idType, id, name);
}

ResourceId UidToolKit::free(UI32 idType, const string &name)
{
    return invoke(UID_FREE, idType, 0, name);
}

ResourceId UidToolKit::freeAll(UI32 idType)
{
    return invoke(UID_FREE, idType, 0, "");
}

ResourceId UidToolKit::freeAll(const string &name)
{
    return invoke(UID_FREE, 0, 0, name);
}

ResourceId UidToolKit::freeAll()
{
    return invoke(UID_FREE, 0, 0, "");
}

ResourceId UidToolKit::remove(UI32 idType, const string &name)
{
    return invoke(UID_REMOVE, idType, 0, name);
}

ResourceId UidToolKit::removeAll(UI32 idType)
{
    return invoke(UID_REMOVE, idType, 0, "");
}

ResourceId UidToolKit::removeAll(const string &name)
{
    return invoke(UID_REMOVE, 0, 0, name);
}

ResourceId UidToolKit::removeAll()
{
    return invoke(UID_REMOVE, 0, 0, "");
}

}
