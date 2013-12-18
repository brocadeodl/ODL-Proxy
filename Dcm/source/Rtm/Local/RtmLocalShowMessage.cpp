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
 * RtmLocalShowMessage.cpp
 *
 * Oct 8, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */


#include "Rtm/Local/RtmLocalShowMessage.h"
#include "Rtm/Local/RtmLocalObjectManager.h"
#include "Rtm/Local/RtmTypes.h"

namespace DcmNs {

    RtmLocalShowMessage::RtmLocalShowMessage ()
        : PrismMessage (RtmLocalObjectManager::getPrismServiceId(), RTMLOCALSHOWMESSAGE)
    {
        m_cmdCode = 0;
        m_vrfName = "";
    }

    RtmLocalShowMessage::~RtmLocalShowMessage ()
    {
    }

    void RtmLocalShowMessage::setupAttributesForSerialization ()
    {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute( new AttributeStringVector(&m_argv, "argv") );
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
    }

    void RtmLocalShowMessage::setArgv (const vector<string> &argv)
    {
        m_argv = argv;
    }

    vector<string> RtmLocalShowMessage::getArgv () const
    {
        return m_argv;
    }

    void RtmLocalShowMessage::setCmdCode (const UI32 &cmdCode)
    {
        m_cmdCode = cmdCode;
    }

    UI32 RtmLocalShowMessage::getCmdCode () const
    {
        return m_cmdCode;
    }

    void RtmLocalShowMessage::setVrfName (const string &vrfName)
    {
        m_vrfName = vrfName;
    }

    string RtmLocalShowMessage::getVrfName () const
    {
        return m_vrfName;
    }

}
