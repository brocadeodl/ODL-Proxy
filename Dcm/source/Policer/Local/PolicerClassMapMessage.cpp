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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#include "Policer/Local/PolicerLocalObjectManager.h"
#include "Policer/Local/PolicerClassMapMessage.h"
#include "Policer/Local/PolicerTypes.h"

namespace DcmNs
{

    PolicerClassMapMessage::PolicerClassMapMessage ()
        : PrismMessage (PolicerLocalObjectManager::getPrismServiceId(),POLICERCLASSMAP)
    {
    }

    PolicerClassMapMessage::PolicerClassMapMessage (const string &classMapName,const bool &any,const UI32 &opcode, const UI32 &filterCriteria, const string &accessListName)
        : PrismMessage (PolicerLocalObjectManager::getPrismServiceId(),POLICERCLASSMAP),
        m_classMapName    (classMapName),
        m_any    (any),
        m_opcode    (opcode),
        m_filterCriteria (filterCriteria),
        m_accessListName (accessListName)
    {
    }

    PolicerClassMapMessage::~PolicerClassMapMessage ()
    {
    }

    void  PolicerClassMapMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_classMapName,"classMapName"));
        addSerializableAttribute (new AttributeBool(&m_any,"any"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_filterCriteria,"filterCriteria"));
        addSerializableAttribute (new AttributeString(&m_accessListName, "accessListName"));
    }

    void  PolicerClassMapMessage::setClassMapName(const string &classMapName)
    {
        m_classMapName  =  classMapName;
    }

    string  PolicerClassMapMessage::getClassMapName() const
    {
        return (m_classMapName);
    }

    void  PolicerClassMapMessage::setAny(const bool &any)
    {
        m_any  =  any;
    }

    bool  PolicerClassMapMessage::getAny() const
    {
        return (m_any);
    }

    void  PolicerClassMapMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  PolicerClassMapMessage::getOpcode() const
    {
        return (m_opcode);
    }

    string PolicerClassMapMessage::getAccessListName() const {
    	return m_accessListName;
    }
    void PolicerClassMapMessage::setAccessListName(const string &accessListName) {
    	m_accessListName = accessListName;
    }

    void PolicerClassMapMessage::setFilterCriteria(const UI32 &filterCriteria) {
    	m_filterCriteria = filterCriteria;
    }

    UI32 PolicerClassMapMessage::getFilterCriteria() const {
    	return m_filterCriteria;
    }
}
