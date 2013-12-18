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
 * NsmDeleteMstpInstanceConfigMessage.cpp
 *
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "Nsm/Common/NsmUpdateMstpInstanceConfigMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Local/NsmLocalObjectManager.h"


namespace DcmNs {

    NsmUpdateMstpInstanceConfigMessage* NsmUpdateMstpInstanceConfigMessage::forNsmGlobal ()
    {
        return new NsmUpdateMstpInstanceConfigMessage(NsmGlobalObjectManager::getPrismServiceId(), NSMGLOBALDELETEBRIDGEINSTANCECONFIG);
    }

    NsmUpdateMstpInstanceConfigMessage* NsmUpdateMstpInstanceConfigMessage::forNsmLocal ()
    {
        return new NsmUpdateMstpInstanceConfigMessage(NsmLocalObjectManager::getPrismServiceId(), NSM_DELETE_MSTP_INST_CONFIG);
    }


    NsmUpdateMstpInstanceConfigMessage::NsmUpdateMstpInstanceConfigMessage (PrismServiceId targetSvc, UI32 opCode)
            : PrismMessage (targetSvc, opCode)
    {
        m_instanceId = 0;
    }

    void  NsmUpdateMstpInstanceConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeUI32(&m_instanceId, "instanceId"));
        addSerializableAttribute (new AttributeObjectIdVector(&m_instanceVlans, "instanceVlans"));
    }

    UI32 NsmUpdateMstpInstanceConfigMessage::getInstanceId () const
    {
        return m_instanceId;
    }

    void NsmUpdateMstpInstanceConfigMessage::setInstanceId (UI32 instanceId)
    {
        m_instanceId = instanceId;
    }

    vector<ObjectId> NsmUpdateMstpInstanceConfigMessage::getInstanceVlans () const
    {
        return m_instanceVlans;
    }

    void NsmUpdateMstpInstanceConfigMessage::setInstanceVlans (const vector<ObjectId> &instanceVlans)
    {
        m_instanceVlans = instanceVlans;
    }

    void NsmUpdateMstpInstanceConfigMessage::setInstanceInfo (UI32 id, const vector<ObjectId> &vlans)
    {
        m_instanceId = id;
        m_instanceVlans = vlans;
    }

}
