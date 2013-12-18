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

#include "Qos/Global/CeeGlobalCreatePriorityMapMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    CeeGlobalCreatePriorityMapMessage::CeeGlobalCreatePriorityMapMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALCREATEPRIORITYMAP)
    {
    }

    CeeGlobalCreatePriorityMapMessage::CeeGlobalCreatePriorityMapMessage (const string &ceeMapName,const string &cos0ToPriorityGrouMap,const string &cos1ToPriorityGrouMap,const string &cos2ToPriorityGrouMap,const string &cos3ToPriorityGrouMap,const string &cos4ToPriorityGrouMap,const string &cos5ToPriorityGrouMap,const string &cos6ToPriorityGrouMap,const string &cos7ToPriorityGrouMap)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALCREATEPRIORITYMAP),
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

    CeeGlobalCreatePriorityMapMessage::~CeeGlobalCreatePriorityMapMessage ()
    {
    }

    void  CeeGlobalCreatePriorityMapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeString(&m_cos0ToPriorityGrouMap,"cos0ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos1ToPriorityGrouMap,"cos1ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos2ToPriorityGrouMap,"cos2ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos3ToPriorityGrouMap,"cos3ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos4ToPriorityGrouMap,"cos4ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos5ToPriorityGrouMap,"cos5ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos6ToPriorityGrouMap,"cos6ToPriorityGrouMap"));
        addSerializableAttribute (new AttributeString(&m_cos7ToPriorityGrouMap,"cos7ToPriorityGrouMap"));
    }

    void  CeeGlobalCreatePriorityMapMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  CeeGlobalCreatePriorityMapMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  CeeGlobalCreatePriorityMapMessage::setCos0ToPriorityGrouMap(const string &cos0ToPriorityGrouMap)
    {
        m_cos0ToPriorityGrouMap  =  cos0ToPriorityGrouMap;
    }

    string  CeeGlobalCreatePriorityMapMessage::getCos0ToPriorityGrouMap() const
    {
        return (m_cos0ToPriorityGrouMap);
    }

    void  CeeGlobalCreatePriorityMapMessage::setCos1ToPriorityGrouMap(const string &cos1ToPriorityGrouMap)
    {
        m_cos1ToPriorityGrouMap  =  cos1ToPriorityGrouMap;
    }

    string  CeeGlobalCreatePriorityMapMessage::getCos1ToPriorityGrouMap() const
    {
        return (m_cos1ToPriorityGrouMap);
    }

    void  CeeGlobalCreatePriorityMapMessage::setCos2ToPriorityGrouMap(const string &cos2ToPriorityGrouMap)
    {
        m_cos2ToPriorityGrouMap  =  cos2ToPriorityGrouMap;
    }

    string  CeeGlobalCreatePriorityMapMessage::getCos2ToPriorityGrouMap() const
    {
        return (m_cos2ToPriorityGrouMap);
    }

    void  CeeGlobalCreatePriorityMapMessage::setCos3ToPriorityGrouMap(const string &cos3ToPriorityGrouMap)
    {
        m_cos3ToPriorityGrouMap  =  cos3ToPriorityGrouMap;
    }

    string  CeeGlobalCreatePriorityMapMessage::getCos3ToPriorityGrouMap() const
    {
        return (m_cos3ToPriorityGrouMap);
    }

    void  CeeGlobalCreatePriorityMapMessage::setCos4ToPriorityGrouMap(const string &cos4ToPriorityGrouMap)
    {
        m_cos4ToPriorityGrouMap  =  cos4ToPriorityGrouMap;
    }

    string  CeeGlobalCreatePriorityMapMessage::getCos4ToPriorityGrouMap() const
    {
        return (m_cos4ToPriorityGrouMap);
    }

    void  CeeGlobalCreatePriorityMapMessage::setCos5ToPriorityGrouMap(const string &cos5ToPriorityGrouMap)
    {
        m_cos5ToPriorityGrouMap  =  cos5ToPriorityGrouMap;
    }

    string  CeeGlobalCreatePriorityMapMessage::getCos5ToPriorityGrouMap() const
    {
        return (m_cos5ToPriorityGrouMap);
    }

    void  CeeGlobalCreatePriorityMapMessage::setCos6ToPriorityGrouMap(const string &cos6ToPriorityGrouMap)
    {
        m_cos6ToPriorityGrouMap  =  cos6ToPriorityGrouMap;
    }

    string  CeeGlobalCreatePriorityMapMessage::getCos6ToPriorityGrouMap() const
    {
        return (m_cos6ToPriorityGrouMap);
    }

    void  CeeGlobalCreatePriorityMapMessage::setCos7ToPriorityGrouMap(const string &cos7ToPriorityGrouMap)
    {
        m_cos7ToPriorityGrouMap  =  cos7ToPriorityGrouMap;
    }

    string  CeeGlobalCreatePriorityMapMessage::getCos7ToPriorityGrouMap() const
    {
        return (m_cos7ToPriorityGrouMap);
    }

}
