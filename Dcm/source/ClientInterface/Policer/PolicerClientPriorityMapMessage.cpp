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

#include "ClientInterface/Policer/PolicerClientPriorityMapMessage.h"
#include "ssm_policer_dcmd_public.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    PolicerClientPriorityMapMessage::PolicerClientPriorityMapMessage ()
        : DcmManagementInterfaceMessage (POLICERCLIENTPRIORITYMAP)
    {
    }

    PolicerClientPriorityMapMessage::PolicerClientPriorityMapMessage (const string &priorityMapName,const UI8 &conformPriorityMap0,const UI8 &conformPriorityMap1,const UI8 &conformPriorityMap2,const UI8 &conformPriorityMap3,const UI8 &conformPriorityMap4,const UI8 &conformPriorityMap5,const UI8 &conformPriorityMap6,const UI8 &conformPriorityMap7,const UI8 &exceedPriorityMap0,const UI8 &exceedPriorityMap1,const UI8 &exceedPriorityMap2,const UI8 &exceedPriorityMap3,const UI8 &exceedPriorityMap4,const UI8 &exceedPriorityMap5,const UI8 &exceedPriorityMap6,const UI8 &exceedPriorityMap7, const UI16 &conformCos, const UI16 &exceedCos, const UI32 &opcode)
        : DcmManagementInterfaceMessage (POLICERCLIENTPRIORITYMAP),
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

    PolicerClientPriorityMapMessage::~PolicerClientPriorityMapMessage ()
    {
    }

    void  PolicerClientPriorityMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
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

    void  PolicerClientPriorityMapMessage::setPriorityMapName(const string &priorityMapName)
    {
        m_priorityMapName  =  priorityMapName;
    }

    string  PolicerClientPriorityMapMessage::getPriorityMapName() const
    {
        return (m_priorityMapName);
    }

    void  PolicerClientPriorityMapMessage::setConformPriorityMap0(const UI8 &conformPriorityMap0)
    {
        m_conformPriorityMap0  =  conformPriorityMap0;
    }

    UI8  PolicerClientPriorityMapMessage::getConformPriorityMap0() const
    {
        return (m_conformPriorityMap0);
    }

    void  PolicerClientPriorityMapMessage::setConformPriorityMap1(const UI8 &conformPriorityMap1)
    {
        m_conformPriorityMap1  =  conformPriorityMap1;
    }

    UI8  PolicerClientPriorityMapMessage::getConformPriorityMap1() const
    {
        return (m_conformPriorityMap1);
    }

    void  PolicerClientPriorityMapMessage::setConformPriorityMap2(const UI8 &conformPriorityMap2)
    {
        m_conformPriorityMap2  =  conformPriorityMap2;
    }

    UI8  PolicerClientPriorityMapMessage::getConformPriorityMap2() const
    {
        return (m_conformPriorityMap2);
    }

    void  PolicerClientPriorityMapMessage::setConformPriorityMap3(const UI8 &conformPriorityMap3)
    {
        m_conformPriorityMap3  =  conformPriorityMap3;
    }

    UI8  PolicerClientPriorityMapMessage::getConformPriorityMap3() const
    {
        return (m_conformPriorityMap3);
    }

    void  PolicerClientPriorityMapMessage::setConformPriorityMap4(const UI8 &conformPriorityMap4)
    {
        m_conformPriorityMap4  =  conformPriorityMap4;
    }

    UI8  PolicerClientPriorityMapMessage::getConformPriorityMap4() const
    {
        return (m_conformPriorityMap4);
    }

    void  PolicerClientPriorityMapMessage::setConformPriorityMap5(const UI8 &conformPriorityMap5)
    {
        m_conformPriorityMap5  =  conformPriorityMap5;
    }

    UI8  PolicerClientPriorityMapMessage::getConformPriorityMap5() const
    {
        return (m_conformPriorityMap5);
    }

    void  PolicerClientPriorityMapMessage::setConformPriorityMap6(const UI8 &conformPriorityMap6)
    {
        m_conformPriorityMap6  =  conformPriorityMap6;
    }

    UI8  PolicerClientPriorityMapMessage::getConformPriorityMap6() const
    {
        return (m_conformPriorityMap6);
    }

    void  PolicerClientPriorityMapMessage::setConformPriorityMap7(const UI8 &conformPriorityMap7)
    {
        m_conformPriorityMap7  =  conformPriorityMap7;
    }

    UI8  PolicerClientPriorityMapMessage::getConformPriorityMap7() const
    {
        return (m_conformPriorityMap7);
    }

    void  PolicerClientPriorityMapMessage::setExceedPriorityMap0(const UI8 &exceedPriorityMap0)
    {
        m_exceedPriorityMap0  =  exceedPriorityMap0;
    }

    UI8  PolicerClientPriorityMapMessage::getExceedPriorityMap0() const
    {
        return (m_exceedPriorityMap0);
    }

    void  PolicerClientPriorityMapMessage::setExceedPriorityMap1(const UI8 &exceedPriorityMap1)
    {
        m_exceedPriorityMap1  =  exceedPriorityMap1;
    }

    UI8  PolicerClientPriorityMapMessage::getExceedPriorityMap1() const
    {
        return (m_exceedPriorityMap1);
    }

    void  PolicerClientPriorityMapMessage::setExceedPriorityMap2(const UI8 &exceedPriorityMap2)
    {
        m_exceedPriorityMap2  =  exceedPriorityMap2;
    }

    UI8  PolicerClientPriorityMapMessage::getExceedPriorityMap2() const
    {
        return (m_exceedPriorityMap2);
    }

    void  PolicerClientPriorityMapMessage::setExceedPriorityMap3(const UI8 &exceedPriorityMap3)
    {
        m_exceedPriorityMap3  =  exceedPriorityMap3;
    }

    UI8  PolicerClientPriorityMapMessage::getExceedPriorityMap3() const
    {
        return (m_exceedPriorityMap3);
    }

    void  PolicerClientPriorityMapMessage::setExceedPriorityMap4(const UI8 &exceedPriorityMap4)
    {
        m_exceedPriorityMap4  =  exceedPriorityMap4;
    }

    UI8  PolicerClientPriorityMapMessage::getExceedPriorityMap4() const
    {
        return (m_exceedPriorityMap4);
    }

    void  PolicerClientPriorityMapMessage::setExceedPriorityMap5(const UI8 &exceedPriorityMap5)
    {
        m_exceedPriorityMap5  =  exceedPriorityMap5;
    }

    UI8  PolicerClientPriorityMapMessage::getExceedPriorityMap5() const
    {
        return (m_exceedPriorityMap5);
    }

    void  PolicerClientPriorityMapMessage::setExceedPriorityMap6(const UI8 &exceedPriorityMap6)
    {
        m_exceedPriorityMap6  =  exceedPriorityMap6;
    }

    UI8  PolicerClientPriorityMapMessage::getExceedPriorityMap6() const
    {
        return (m_exceedPriorityMap6);
    }

    void  PolicerClientPriorityMapMessage::setExceedPriorityMap7(const UI8 &exceedPriorityMap7)
    {
        m_exceedPriorityMap7  =  exceedPriorityMap7;
    }

    UI8  PolicerClientPriorityMapMessage::getExceedPriorityMap7() const
    {
        return (m_exceedPriorityMap7);
    }

    void PolicerClientPriorityMapMessage::setConformCos(const UI16 &conformCos)
	{
		m_set_conform_cos = conformCos;
	}

	UI16 PolicerClientPriorityMapMessage::getConformCos() const
	{
		 return m_set_conform_cos;
	}

	void PolicerClientPriorityMapMessage::setExceedCos(const UI16 &exceedCos)
	{
		m_set_exceed_cos = exceedCos;
	}

	UI16 PolicerClientPriorityMapMessage::getExceedCos() const
	{
		return m_set_exceed_cos;
	}

    void  PolicerClientPriorityMapMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  PolicerClientPriorityMapMessage::getOpcode() const
    {
        return (m_opcode);
    }

	const void *PolicerClientPriorityMapMessage::getCStructureForInputs ()
    {
        ssm_pol_prio_map_msg_t *pInput = (ssm_pol_prio_map_msg_t *)malloc (sizeof (ssm_pol_prio_map_msg_t));
		
		pInput->opcode = (ssm_pol_cmd_code)m_opcode;

        strncpy ((pInput->prio_map).name, m_priorityMapName.c_str (), SSM_POL_MAX_NAME_SIZE);
        (pInput->prio_map).name[SSM_POL_MAX_NAME_SIZE] = '\0';
        (pInput->prio_map).conform_cos[0] = m_conformPriorityMap0;
        (pInput->prio_map).conform_cos[1] = m_conformPriorityMap1;
        (pInput->prio_map).conform_cos[2] = m_conformPriorityMap2;
        (pInput->prio_map).conform_cos[3] = m_conformPriorityMap3;
        (pInput->prio_map).conform_cos[4] = m_conformPriorityMap4;
        (pInput->prio_map).conform_cos[5] = m_conformPriorityMap5;
        (pInput->prio_map).conform_cos[6] = m_conformPriorityMap6;
        (pInput->prio_map).conform_cos[7] = m_conformPriorityMap7;

        (pInput->prio_map).exceed_cos[0] = m_exceedPriorityMap0;
        (pInput->prio_map).exceed_cos[1] = m_exceedPriorityMap1;
        (pInput->prio_map).exceed_cos[2] = m_exceedPriorityMap2;
        (pInput->prio_map).exceed_cos[3] = m_exceedPriorityMap3;
        (pInput->prio_map).exceed_cos[4] = m_exceedPriorityMap4;
        (pInput->prio_map).exceed_cos[5] = m_exceedPriorityMap5;
        (pInput->prio_map).exceed_cos[6] = m_exceedPriorityMap6;
        (pInput->prio_map).exceed_cos[7] = m_exceedPriorityMap7;
		(pInput->prio_map).set_conform_cos = m_set_conform_cos;
		(pInput->prio_map).set_exceed_cos = m_set_exceed_cos;

        return (pInput);
    }

    void PolicerClientPriorityMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
