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
 *   Author : vtummala                                                     *
 **************************************************************************/

#include "FabricService/Local/FabricServiceLocalObjectManager.h"
#include "FabricService/Local/FabricServicePortLogMessage.h"
#include "FabricService/Local/FabricServiceTypes.h"

namespace DcmNs
{

    FabricServicePortLogMessage::FabricServicePortLogMessage ()
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEPORTLOG)
    {
    }

    FabricServicePortLogMessage::FabricServicePortLogMessage (const vector<string> &log_entries)
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICEPORTLOG),
        m_log_entries    (log_entries)
    {
    }

    FabricServicePortLogMessage::~FabricServicePortLogMessage ()
    {
    }

    void  FabricServicePortLogMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeStringVector(&m_log_entries,"log_entries"));
    }

    void  FabricServicePortLogMessage::setLog_entries(const vector<string> &log_entries)
    {
        m_log_entries  =  log_entries;
    }

    vector<string>  FabricServicePortLogMessage::getLog_entries() const
    {
        return (m_log_entries);
    }

}
