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
#include "Policer/Local/PolicerPriorityMapMessage.h"
#include "Policer/Local/PolicerTypes.h"

namespace DcmNs
{

    PolicerPriorityMapMessage::PolicerPriorityMapMessage ()
		:PrismMessage (PolicerLocalObjectManager::getPrismServiceId(),POLICERPRIORITYMAP)
    {
    }

    PolicerPriorityMapMessage::PolicerPriorityMapMessage (const string &priorityMapName,const UI8 &conformPriorityMap0,const UI8 &conformPriorityMap1,const UI8 &conformPriorityMap2,const UI8 &conformPriorityMap3,const UI8 &conformPriorityMap4,const UI8 &conformPriorityMap5,const UI8 &conformPriorityMap6,const UI8 &conformPriorityMap7,const UI8 &exceedPriorityMap0,const UI8 &exceedPriorityMap1,const UI8 &exceedPriorityMap2,const UI8 &exceedPriorityMap3,const UI8 &exceedPriorityMap4,const UI8 &exceedPriorityMap5,const UI8 &exceedPriorityMap6,const UI8 &exceedPriorityMap7, const UI16 &conformCos, const UI16 &exceedCos, const UI32 &opcode)
        :PrismMessage (PolicerLocalObjectManager::getPrismServiceId(),POLICERPRIORITYMAP),
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

    PolicerPriorityMapMessage::~PolicerPriorityMapMessage ()
    {
    }

    void  PolicerPriorityMapMessage::setupAttributesForSerialization()
    {
		PrismMessage::setupAttributesForSerialization ();
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

    void  PolicerPriorityMapMessage::setPriorityMapName(const string &priorityMapName)
    {
        m_priorityMapName  =  priorityMapName;
    }

    string  PolicerPriorityMapMessage::getPriorityMapName() const
    {
        return (m_priorityMapName);
    }

    void  PolicerPriorityMapMessage::setConformPriorityMap0(const UI8 &conformPriorityMap0)
    {
        m_conformPriorityMap0  =  conformPriorityMap0;
    }

    UI8  PolicerPriorityMapMessage::getConformPriorityMap0() const
    {
        return (m_conformPriorityMap0);
    }

    void  PolicerPriorityMapMessage::setConformPriorityMap1(const UI8 &conformPriorityMap1)
    {
        m_conformPriorityMap1  =  conformPriorityMap1;
    }

    UI8  PolicerPriorityMapMessage::getConformPriorityMap1() const
    {
        return (m_conformPriorityMap1);
    }

    void  PolicerPriorityMapMessage::setConformPriorityMap2(const UI8 &conformPriorityMap2)
    {
        m_conformPriorityMap2  =  conformPriorityMap2;
    }

    UI8  PolicerPriorityMapMessage::getConformPriorityMap2() const
    {
        return (m_conformPriorityMap2);
    }

    void  PolicerPriorityMapMessage::setConformPriorityMap3(const UI8 &conformPriorityMap3)
    {
        m_conformPriorityMap3  =  conformPriorityMap3;
    }

    UI8  PolicerPriorityMapMessage::getConformPriorityMap3() const
    {
        return (m_conformPriorityMap3);
    }

    void  PolicerPriorityMapMessage::setConformPriorityMap4(const UI8 &conformPriorityMap4)
    {
        m_conformPriorityMap4  =  conformPriorityMap4;
    }

    UI8  PolicerPriorityMapMessage::getConformPriorityMap4() const
    {
        return (m_conformPriorityMap4);
    }

    void  PolicerPriorityMapMessage::setConformPriorityMap5(const UI8 &conformPriorityMap5)
    {
        m_conformPriorityMap5  =  conformPriorityMap5;
    }

    UI8  PolicerPriorityMapMessage::getConformPriorityMap5() const
    {
        return (m_conformPriorityMap5);
    }

    void  PolicerPriorityMapMessage::setConformPriorityMap6(const UI8 &conformPriorityMap6)
    {
        m_conformPriorityMap6  =  conformPriorityMap6;
    }

    UI8  PolicerPriorityMapMessage::getConformPriorityMap6() const
    {
        return (m_conformPriorityMap6);
    }

    void  PolicerPriorityMapMessage::setConformPriorityMap7(const UI8 &conformPriorityMap7)
    {
        m_conformPriorityMap7  =  conformPriorityMap7;
    }

    UI8  PolicerPriorityMapMessage::getConformPriorityMap7() const
    {
        return (m_conformPriorityMap7);
    }

    void  PolicerPriorityMapMessage::setExceedPriorityMap0(const UI8 &exceedPriorityMap0)
    {
        m_exceedPriorityMap0  =  exceedPriorityMap0;
    }

    UI8  PolicerPriorityMapMessage::getExceedPriorityMap0() const
    {
        return (m_exceedPriorityMap0);
    }

    void  PolicerPriorityMapMessage::setExceedPriorityMap1(const UI8 &exceedPriorityMap1)
    {
        m_exceedPriorityMap1  =  exceedPriorityMap1;
    }

    UI8  PolicerPriorityMapMessage::getExceedPriorityMap1() const
    {
        return (m_exceedPriorityMap1);
    }

    void  PolicerPriorityMapMessage::setExceedPriorityMap2(const UI8 &exceedPriorityMap2)
    {
        m_exceedPriorityMap2  =  exceedPriorityMap2;
    }

    UI8  PolicerPriorityMapMessage::getExceedPriorityMap2() const
    {
        return (m_exceedPriorityMap2);
    }

    void  PolicerPriorityMapMessage::setExceedPriorityMap3(const UI8 &exceedPriorityMap3)
    {
        m_exceedPriorityMap3  =  exceedPriorityMap3;
    }

    UI8  PolicerPriorityMapMessage::getExceedPriorityMap3() const
    {
        return (m_exceedPriorityMap3);
    }

    void  PolicerPriorityMapMessage::setExceedPriorityMap4(const UI8 &exceedPriorityMap4)
    {
        m_exceedPriorityMap4  =  exceedPriorityMap4;
    }

    UI8  PolicerPriorityMapMessage::getExceedPriorityMap4() const
    {
        return (m_exceedPriorityMap4);
    }

    void  PolicerPriorityMapMessage::setExceedPriorityMap5(const UI8 &exceedPriorityMap5)
    {
        m_exceedPriorityMap5  =  exceedPriorityMap5;
    }

    UI8  PolicerPriorityMapMessage::getExceedPriorityMap5() const
    {
        return (m_exceedPriorityMap5);
    }

    void  PolicerPriorityMapMessage::setExceedPriorityMap6(const UI8 &exceedPriorityMap6)
    {
        m_exceedPriorityMap6  =  exceedPriorityMap6;
    }

    UI8  PolicerPriorityMapMessage::getExceedPriorityMap6() const
    {
        return (m_exceedPriorityMap6);
    }

    void  PolicerPriorityMapMessage::setExceedPriorityMap7(const UI8 &exceedPriorityMap7)
    {
        m_exceedPriorityMap7  =  exceedPriorityMap7;
    }

    UI8  PolicerPriorityMapMessage::getExceedPriorityMap7() const
    {
        return (m_exceedPriorityMap7);
    }

	void PolicerPriorityMapMessage::setConformCos(const UI16 &conformCos)
	{
		 m_set_conform_cos = conformCos;
	}

	UI16 PolicerPriorityMapMessage::getConformCos() const
	{
		return m_set_conform_cos;
	}

	void PolicerPriorityMapMessage::setExceedCos(const UI16 &exceedCos)
	{
		m_set_exceed_cos = exceedCos;
	}

	UI16 PolicerPriorityMapMessage::getExceedCos() const
	{
		return m_set_exceed_cos;
	}

    void  PolicerPriorityMapMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  PolicerPriorityMapMessage::getOpcode() const
    {
        return (m_opcode);
    }

}
