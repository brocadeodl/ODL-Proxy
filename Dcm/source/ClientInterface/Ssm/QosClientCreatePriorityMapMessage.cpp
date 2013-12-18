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

#include "ClientInterface/Ssm/QosClientCreatePriorityMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreatePriorityMapMessage::QosClientCreatePriorityMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEPRIORITYMAP)
    {
    }

    QosClientCreatePriorityMapMessage::QosClientCreatePriorityMapMessage (const string &ceeMapName,const string &cos0ToPriorityGrouMap,const string &cos1ToPriorityGrouMap,const string &cos2ToPriorityGrouMap,const string &cos3ToPriorityGrouMap,const string &cos4ToPriorityGrouMap,const string &cos5ToPriorityGrouMap,const string &cos6ToPriorityGrouMap,const string &cos7ToPriorityGrouMap)
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEPRIORITYMAP),
        m_ceeMapName    (ceeMapName),
        m_cos0ToPriorityGrouMap    (cos0ToPriorityGrouMap),
        m_cos1ToPriorityGrouMap    (cos1ToPriorityGrouMap),
        m_cos2ToPriorityGrouMap    (cos2ToPriorityGrouMap),
        m_cos3ToPriorityGrouMap    (cos3ToPriorityGrouMap),
        m_cos4ToPriorityGrouMap    (cos4ToPriorityGrouMap),
        m_cos5ToPriorityGrouMap    (cos5ToPriorityGrouMap),
        m_cos6ToPriorityGrouMap    (cos6ToPriorityGrouMap),
        m_cos7ToPriorityGrouMap    (cos7ToPriorityGrouMap)
    {
    }

    QosClientCreatePriorityMapMessage::~QosClientCreatePriorityMapMessage ()
    {
    }

    void  QosClientCreatePriorityMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeString(&m_cos0ToPriorityGrouMap,"cos0ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos1ToPriorityGrouMap,"cos1ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos2ToPriorityGrouMap,"cos2ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos3ToPriorityGrouMap,"cos3ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos4ToPriorityGrouMap,"cos4ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos5ToPriorityGrouMap,"cos5ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos6ToPriorityGrouMap,"cos6ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos7ToPriorityGrouMap,"cos7ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeSI32(&m_configReplay,"configReplay"));
    }

    void  QosClientCreatePriorityMapMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosClientCreatePriorityMapMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosClientCreatePriorityMapMessage::setCos0ToPriorityGrouMap(const string &cos0ToPriorityGrouMap)
    {
        m_cos0ToPriorityGrouMap  =  cos0ToPriorityGrouMap;
    }

    string  QosClientCreatePriorityMapMessage::getCos0ToPriorityGrouMap() const
    {
        return (m_cos0ToPriorityGrouMap);
    }

    void  QosClientCreatePriorityMapMessage::setCos1ToPriorityGrouMap(const string &cos1ToPriorityGrouMap)
    {
        m_cos1ToPriorityGrouMap  =  cos1ToPriorityGrouMap;
    }

    string  QosClientCreatePriorityMapMessage::getCos1ToPriorityGrouMap() const
    {
        return (m_cos1ToPriorityGrouMap);
    }

    void  QosClientCreatePriorityMapMessage::setCos2ToPriorityGrouMap(const string &cos2ToPriorityGrouMap)
    {
        m_cos2ToPriorityGrouMap  =  cos2ToPriorityGrouMap;
    }

    string  QosClientCreatePriorityMapMessage::getCos2ToPriorityGrouMap() const
    {
        return (m_cos2ToPriorityGrouMap);
    }

    void  QosClientCreatePriorityMapMessage::setCos3ToPriorityGrouMap(const string &cos3ToPriorityGrouMap)
    {
        m_cos3ToPriorityGrouMap  =  cos3ToPriorityGrouMap;
    }

    string  QosClientCreatePriorityMapMessage::getCos3ToPriorityGrouMap() const
    {
        return (m_cos3ToPriorityGrouMap);
    }

    void  QosClientCreatePriorityMapMessage::setCos4ToPriorityGrouMap(const string &cos4ToPriorityGrouMap)
    {
        m_cos4ToPriorityGrouMap  =  cos4ToPriorityGrouMap;
    }

    string  QosClientCreatePriorityMapMessage::getCos4ToPriorityGrouMap() const
    {
        return (m_cos4ToPriorityGrouMap);
    }

    void  QosClientCreatePriorityMapMessage::setCos5ToPriorityGrouMap(const string &cos5ToPriorityGrouMap)
    {
        m_cos5ToPriorityGrouMap  =  cos5ToPriorityGrouMap;
    }

    string  QosClientCreatePriorityMapMessage::getCos5ToPriorityGrouMap() const
    {
        return (m_cos5ToPriorityGrouMap);
    }

    void  QosClientCreatePriorityMapMessage::setCos6ToPriorityGrouMap(const string &cos6ToPriorityGrouMap)
    {
        m_cos6ToPriorityGrouMap  =  cos6ToPriorityGrouMap;
    }

    string  QosClientCreatePriorityMapMessage::getCos6ToPriorityGrouMap() const
    {
        return (m_cos6ToPriorityGrouMap);
    }

    void  QosClientCreatePriorityMapMessage::setCos7ToPriorityGrouMap(const string &cos7ToPriorityGrouMap)
    {
        m_cos7ToPriorityGrouMap  =  cos7ToPriorityGrouMap;
    }

    string  QosClientCreatePriorityMapMessage::getCos7ToPriorityGrouMap() const
    {
        return (m_cos7ToPriorityGrouMap);
    }

    void  QosClientCreatePriorityMapMessage::setConfigReplay(const SI32 &configReplay)
    {
        m_configReplay = configReplay;
    }

    SI32 QosClientCreatePriorityMapMessage::getConfigReplay() const
    {
        return (m_configReplay);
    }

    const void *QosClientCreatePriorityMapMessage::getCStructureForInputs ()
    {
        QosPriorityMapMsg_t *pInput = (QosPriorityMapMsg_t *)calloc (1, sizeof (QosPriorityMapMsg_t));
        
        if ( NULL == pInput ) {
            return pInput;
        }
        strncpy (pInput->ceeMapName, m_ceeMapName.c_str (), MAP_NAME_MAX_STR-1);
        pInput->ceeMapName[MAP_NAME_MAX_STR-1] = '\0';
        strncpy (pInput->pgid0, m_cos0ToPriorityGrouMap.c_str (), 8);
		pInput->pgid0[7] = '\0';
        strncpy (pInput->pgid1, m_cos1ToPriorityGrouMap.c_str (), 8);
		pInput->pgid1[7] = '\0';
        strncpy (pInput->pgid2, m_cos2ToPriorityGrouMap.c_str (), 8);
		pInput->pgid2[7] = '\0';
        strncpy (pInput->pgid3, m_cos3ToPriorityGrouMap.c_str (), 8);
		pInput->pgid3[7] = '\0';
        strncpy (pInput->pgid4, m_cos4ToPriorityGrouMap.c_str (), 8);
		pInput->pgid4[7] = '\0';
        strncpy (pInput->pgid5, m_cos5ToPriorityGrouMap.c_str (), 8);
		pInput->pgid5[7] = '\0';
        strncpy (pInput->pgid6, m_cos6ToPriorityGrouMap.c_str (), 8);
		pInput->pgid6[7] = '\0';
        strncpy (pInput->pgid7, m_cos7ToPriorityGrouMap.c_str (), 8);
		pInput->pgid7[7] = '\0';
        pInput->configReplay = (int)m_configReplay;

        return (pInput);
    }

    void QosClientCreatePriorityMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}

