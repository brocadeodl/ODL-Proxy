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

#include "Policer/Global/PolicerGlobalPriorityMapMessage.h"
#include "Policer/Global/PolicerGlobalObjectManager.h"
#include "Policer/Global/PolicerGlobalTypes.h"

namespace DcmNs
{

    PolicerGlobalPriorityMapMessage::PolicerGlobalPriorityMapMessage ()
        : ManagementInterfaceMessage (PolicerGlobalObjectManager::getClassName (),POLICERGLOBALPRIORITYMAP)
    {
    }

    PolicerGlobalPriorityMapMessage::PolicerGlobalPriorityMapMessage (const string &priorityMapName,const UI8 &conformPriorityMap0,const UI8 &conformPriorityMap1,const UI8 &conformPriorityMap2,const UI8 &conformPriorityMap3,const UI8 &conformPriorityMap4,const UI8 &conformPriorityMap5,const UI8 &conformPriorityMap6,const UI8 &conformPriorityMap7,const UI8 &exceedPriorityMap0,const UI8 &exceedPriorityMap1,const UI8 &exceedPriorityMap2,const UI8 &exceedPriorityMap3,const UI8 &exceedPriorityMap4,const UI8 &exceedPriorityMap5,const UI8 &exceedPriorityMap6,const UI8 &exceedPriorityMap7, const UI16 &conformCos, const UI16 &exceedCos, const UI32 &opcode)
        : ManagementInterfaceMessage (PolicerGlobalObjectManager::getClassName (),POLICERGLOBALPRIORITYMAP),
        m_priorityMapName    (priorityMapName),
        m_conformPriorityMap0    (conformPriorityMap0),
        m_conformPriorityMap1    (conformPriorityMap1),
        m_conformPriorityMap2    (conformPriorityMap2),
        m_conformPriorityMap3    (conformPriorityMap3),
        m_conformPriorityMap4    (conformPriorityMap4),
        m_conformPriorityMap5    (conformPriorityMap5),
        m_conformPriorityMap6    (conformPriorityMap6),
        m_conformPriorityMap7    (conformPriorityMap7),
        m_exceedPriorityMap0    (exceedPriorityMap0),
        m_exceedPriorityMap1    (exceedPriorityMap1),
        m_exceedPriorityMap2    (exceedPriorityMap2),
        m_exceedPriorityMap3    (exceedPriorityMap3),
        m_exceedPriorityMap4    (exceedPriorityMap4),
        m_exceedPriorityMap5    (exceedPriorityMap5),
        m_exceedPriorityMap6    (exceedPriorityMap6),
        m_exceedPriorityMap7    (exceedPriorityMap7),
		m_set_conform_cos   (conformCos),
		m_set_exceed_cos    (exceedCos),
        m_opcode    (opcode)
    {
    }

    PolicerGlobalPriorityMapMessage::~PolicerGlobalPriorityMapMessage ()
    {
    }

    void  PolicerGlobalPriorityMapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_priorityMapName,"priorityMapName"));
        addSerializableAttribute (new AttributeUI8(&m_conformPriorityMap0,"conformPriorityMap0"));
        addSerializableAttribute (new AttributeUI8(&m_conformPriorityMap1,"conformPriorityMap1"));
        addSerializableAttribute (new AttributeUI8(&m_conformPriorityMap2,"conformPriorityMap2"));
        addSerializableAttribute (new AttributeUI8(&m_conformPriorityMap3,"conformPriorityMap3"));
        addSerializableAttribute (new AttributeUI8(&m_conformPriorityMap4,"conformPriorityMap4"));
        addSerializableAttribute (new AttributeUI8(&m_conformPriorityMap5,"conformPriorityMap5"));
        addSerializableAttribute (new AttributeUI8(&m_conformPriorityMap6,"conformPriorityMap6"));
        addSerializableAttribute (new AttributeUI8(&m_conformPriorityMap7,"conformPriorityMap7"));
        addSerializableAttribute (new AttributeUI8(&m_exceedPriorityMap0,"exceedPriorityMap0"));
        addSerializableAttribute (new AttributeUI8(&m_exceedPriorityMap1,"exceedPriorityMap1"));
        addSerializableAttribute (new AttributeUI8(&m_exceedPriorityMap2,"exceedPriorityMap2"));
        addSerializableAttribute (new AttributeUI8(&m_exceedPriorityMap3,"exceedPriorityMap3"));
        addSerializableAttribute (new AttributeUI8(&m_exceedPriorityMap4,"exceedPriorityMap4"));
        addSerializableAttribute (new AttributeUI8(&m_exceedPriorityMap5,"exceedPriorityMap5"));
        addSerializableAttribute (new AttributeUI8(&m_exceedPriorityMap6,"exceedPriorityMap6"));
        addSerializableAttribute (new AttributeUI8(&m_exceedPriorityMap7,"exceedPriorityMap7"));
		addSerializableAttribute (new AttributeUI16(&m_set_conform_cos,"conformCos"));
		addSerializableAttribute (new AttributeUI16(&m_set_exceed_cos,"exceedCos"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  PolicerGlobalPriorityMapMessage::setPriorityMapName(const string &priorityMapName)
    {
        m_priorityMapName  =  priorityMapName;
    }

    string  PolicerGlobalPriorityMapMessage::getPriorityMapName() const
    {
        return (m_priorityMapName);
    }

    void  PolicerGlobalPriorityMapMessage::setConformPriorityMap0(const UI8 &conformPriorityMap0)
    {
        m_conformPriorityMap0  =  conformPriorityMap0;
    }

    UI8  PolicerGlobalPriorityMapMessage::getConformPriorityMap0() const
    {
        return (m_conformPriorityMap0);
    }

    void  PolicerGlobalPriorityMapMessage::setConformPriorityMap1(const UI8 &conformPriorityMap1)
    {
        m_conformPriorityMap1  =  conformPriorityMap1;
    }

    UI8  PolicerGlobalPriorityMapMessage::getConformPriorityMap1() const
    {
        return (m_conformPriorityMap1);
    }

    void  PolicerGlobalPriorityMapMessage::setConformPriorityMap2(const UI8 &conformPriorityMap2)
    {
        m_conformPriorityMap2  =  conformPriorityMap2;
    }

    UI8  PolicerGlobalPriorityMapMessage::getConformPriorityMap2() const
    {
        return (m_conformPriorityMap2);
    }

    void  PolicerGlobalPriorityMapMessage::setConformPriorityMap3(const UI8 &conformPriorityMap3)
    {
        m_conformPriorityMap3  =  conformPriorityMap3;
    }

    UI8  PolicerGlobalPriorityMapMessage::getConformPriorityMap3() const
    {
        return (m_conformPriorityMap3);
    }

    void  PolicerGlobalPriorityMapMessage::setConformPriorityMap4(const UI8 &conformPriorityMap4)
    {
        m_conformPriorityMap4  =  conformPriorityMap4;
    }

    UI8  PolicerGlobalPriorityMapMessage::getConformPriorityMap4() const
    {
        return (m_conformPriorityMap4);
    }

    void  PolicerGlobalPriorityMapMessage::setConformPriorityMap5(const UI8 &conformPriorityMap5)
    {
        m_conformPriorityMap5  =  conformPriorityMap5;
    }

    UI8  PolicerGlobalPriorityMapMessage::getConformPriorityMap5() const
    {
        return (m_conformPriorityMap5);
    }

    void  PolicerGlobalPriorityMapMessage::setConformPriorityMap6(const UI8 &conformPriorityMap6)
    {
        m_conformPriorityMap6  =  conformPriorityMap6;
    }

    UI8  PolicerGlobalPriorityMapMessage::getConformPriorityMap6() const
    {
        return (m_conformPriorityMap6);
    }

    void  PolicerGlobalPriorityMapMessage::setConformPriorityMap7(const UI8 &conformPriorityMap7)
    {
        m_conformPriorityMap7  =  conformPriorityMap7;
    }

    UI8  PolicerGlobalPriorityMapMessage::getConformPriorityMap7() const
    {
        return (m_conformPriorityMap7);
    }

    void  PolicerGlobalPriorityMapMessage::setExceedPriorityMap0(const UI8 &exceedPriorityMap0)
    {
        m_exceedPriorityMap0  =  exceedPriorityMap0;
    }

    UI8  PolicerGlobalPriorityMapMessage::getExceedPriorityMap0() const
    {
        return (m_exceedPriorityMap0);
    }

    void  PolicerGlobalPriorityMapMessage::setExceedPriorityMap1(const UI8 &exceedPriorityMap1)
    {
        m_exceedPriorityMap1  =  exceedPriorityMap1;
    }

    UI8  PolicerGlobalPriorityMapMessage::getExceedPriorityMap1() const
    {
        return (m_exceedPriorityMap1);
    }

    void  PolicerGlobalPriorityMapMessage::setExceedPriorityMap2(const UI8 &exceedPriorityMap2)
    {
        m_exceedPriorityMap2  =  exceedPriorityMap2;
    }

    UI8  PolicerGlobalPriorityMapMessage::getExceedPriorityMap2() const
    {
        return (m_exceedPriorityMap2);
    }

    void  PolicerGlobalPriorityMapMessage::setExceedPriorityMap3(const UI8 &exceedPriorityMap3)
    {
        m_exceedPriorityMap3  =  exceedPriorityMap3;
    }

    UI8  PolicerGlobalPriorityMapMessage::getExceedPriorityMap3() const
    {
        return (m_exceedPriorityMap3);
    }

    void  PolicerGlobalPriorityMapMessage::setExceedPriorityMap4(const UI8 &exceedPriorityMap4)
    {
        m_exceedPriorityMap4  =  exceedPriorityMap4;
    }

    UI8  PolicerGlobalPriorityMapMessage::getExceedPriorityMap4() const
    {
        return (m_exceedPriorityMap4);
    }

    void  PolicerGlobalPriorityMapMessage::setExceedPriorityMap5(const UI8 &exceedPriorityMap5)
    {
        m_exceedPriorityMap5  =  exceedPriorityMap5;
    }

    UI8  PolicerGlobalPriorityMapMessage::getExceedPriorityMap5() const
    {
        return (m_exceedPriorityMap5);
    }

    void  PolicerGlobalPriorityMapMessage::setExceedPriorityMap6(const UI8 &exceedPriorityMap6)
    {
        m_exceedPriorityMap6  =  exceedPriorityMap6;
    }

    UI8  PolicerGlobalPriorityMapMessage::getExceedPriorityMap6() const
    {
        return (m_exceedPriorityMap6);
    }

    void  PolicerGlobalPriorityMapMessage::setExceedPriorityMap7(const UI8 &exceedPriorityMap7)
    {
        m_exceedPriorityMap7  =  exceedPriorityMap7;
    }

    UI8  PolicerGlobalPriorityMapMessage::getExceedPriorityMap7() const
    {
        return (m_exceedPriorityMap7);
    }

    void PolicerGlobalPriorityMapMessage::setConformCos(const UI16 &conformCos)
	{
		m_set_conform_cos = conformCos;
	}

	UI16 PolicerGlobalPriorityMapMessage::getConformCos() const
	{
		return m_set_conform_cos;
	}

	void PolicerGlobalPriorityMapMessage::setExceedCos(const UI16 &exceedCos)
	{
		m_set_exceed_cos = exceedCos;
	}

	UI16 PolicerGlobalPriorityMapMessage::getExceedCos() const
	{
		return m_set_exceed_cos;
	}

    void  PolicerGlobalPriorityMapMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  PolicerGlobalPriorityMapMessage::getOpcode() const
    {
        return (m_opcode);
    }

}
