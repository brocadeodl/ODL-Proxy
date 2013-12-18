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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "ClientInterface/Ssm/QosClientDeleteCeeMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"


namespace DcmNs
{

    QosClientDeleteCeeMapMessage::QosClientDeleteCeeMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTDELETECEEMAP)
    {
    }

    QosClientDeleteCeeMapMessage::QosClientDeleteCeeMapMessage (const string &ceeMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTDELETECEEMAP),
        m_ceeMapName    (ceeMapName)
    {
    }

    QosClientDeleteCeeMapMessage::~QosClientDeleteCeeMapMessage ()
    {
    }

    void  QosClientDeleteCeeMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
    }

    void  QosClientDeleteCeeMapMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosClientDeleteCeeMapMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    const void *QosClientDeleteCeeMapMessage::getCStructureForInputs ()
    {
        char *pInput = (char*)calloc(1, MAP_NAME_MAX_STR);
        if ( NULL == pInput ) {
            return pInput;
        }
        strncpy (pInput, m_ceeMapName.c_str (), MAP_NAME_MAX_STR);
        return (pInput);
    }

    void QosClientDeleteCeeMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
