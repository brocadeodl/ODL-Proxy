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

#include "Policer/Global/PolicerPriorityMapManagedObject.h"
#include "Policer/Global/PolicerGlobalObjectManager.h"
#include "Policer/Global/PolicerGlobalTypes.h"
#include "ssm_policer_public.h"
#include "vcs.h"

namespace DcmNs
{

    PolicerPriorityMapManagedObject::PolicerPriorityMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager)
        : PrismElement  (pPolicerGlobalObjectManager),
        PrismPersistableObject (PolicerPriorityMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pPolicerGlobalObjectManager),
        DcmManagedObject (pPolicerGlobalObjectManager)
    {
    }

    PolicerPriorityMapManagedObject::PolicerPriorityMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager,const string &priorityMapName,const UI32UC &conformPriorityMap0,const UI32UC &conformPriorityMap1,const UI32UC &conformPriorityMap2,const UI32UC &conformPriorityMap3,const UI32UC &conformPriorityMap4,const UI32UC &conformPriorityMap5,const UI32UC &conformPriorityMap6,const UI32UC &conformPriorityMap7,const UI32UC &exceedPriorityMap0,const UI32UC &exceedPriorityMap1,const UI32UC &exceedPriorityMap2,const UI32UC &exceedPriorityMap3,const UI32UC &exceedPriorityMap4,const UI32UC &exceedPriorityMap5,const UI32UC &exceedPriorityMap6,const UI32UC &exceedPriorityMap7)
        : PrismElement  (pPolicerGlobalObjectManager),
        PrismPersistableObject (PolicerPriorityMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pPolicerGlobalObjectManager),
        DcmManagedObject (pPolicerGlobalObjectManager),
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
        m_exceedPriorityMap7    (exceedPriorityMap7)
    {
    }

    PolicerPriorityMapManagedObject::~PolicerPriorityMapManagedObject ()
    {
    }

    string  PolicerPriorityMapManagedObject::getClassName()
    {
        return ("PolicerPriorityMapManagedObject");
    }

    void  PolicerPriorityMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_priorityMapName,"priorityMapName"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformPriorityMap0, true, "conformPriorityMap0"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformPriorityMap1, true, "conformPriorityMap1"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformPriorityMap2, true, "conformPriorityMap2"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformPriorityMap3, true, "conformPriorityMap3"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformPriorityMap4, true, "conformPriorityMap4"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformPriorityMap5, true, "conformPriorityMap5"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformPriorityMap6, true, "conformPriorityMap6"));
        addPersistableAttribute (new AttributeUI32UC(&m_conformPriorityMap7, true, "conformPriorityMap7"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedPriorityMap0, true, "exceedPriorityMap0"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedPriorityMap1, true, "exceedPriorityMap1"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedPriorityMap2, true, "exceedPriorityMap2"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedPriorityMap3, true, "exceedPriorityMap3"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedPriorityMap4, true, "exceedPriorityMap4"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedPriorityMap5, true, "exceedPriorityMap5"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedPriorityMap6, true, "exceedPriorityMap6"));
        addPersistableAttribute (new AttributeUI32UC(&m_exceedPriorityMap7, true, "exceedPriorityMap7"));
		setUserTagForAttribute("priorityMapName", policer_name);
		setUserTagForAttribute("conformPriorityMap0", policer_map_pri0_conform);
		setUserTagForAttribute("conformPriorityMap1", policer_map_pri1_conform);
		setUserTagForAttribute("conformPriorityMap2", policer_map_pri2_conform);
		setUserTagForAttribute("conformPriorityMap3", policer_map_pri3_conform);
		setUserTagForAttribute("conformPriorityMap4", policer_map_pri4_conform);
		setUserTagForAttribute("conformPriorityMap5", policer_map_pri5_conform);
		setUserTagForAttribute("conformPriorityMap6", policer_map_pri6_conform);
		setUserTagForAttribute("conformPriorityMap7", policer_map_pri7_conform);
		setUserTagForAttribute("exceedPriorityMap0", policer_map_pri0_exceed);
        setUserTagForAttribute("exceedPriorityMap1", policer_map_pri1_exceed);
        setUserTagForAttribute("exceedPriorityMap2", policer_map_pri2_exceed);
        setUserTagForAttribute("exceedPriorityMap3", policer_map_pri3_exceed);
        setUserTagForAttribute("exceedPriorityMap4", policer_map_pri4_exceed);
        setUserTagForAttribute("exceedPriorityMap5", policer_map_pri5_exceed);
        setUserTagForAttribute("exceedPriorityMap6", policer_map_pri6_exceed);
        setUserTagForAttribute("exceedPriorityMap7", policer_map_pri7_exceed);
    }

    void  PolicerPriorityMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_priorityMapName,"priorityMapName"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformPriorityMap0,true,"conformPriorityMap0"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformPriorityMap1,true,"conformPriorityMap1"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformPriorityMap2,true,"conformPriorityMap2"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformPriorityMap3,true,"conformPriorityMap3"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformPriorityMap4,true,"conformPriorityMap4"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformPriorityMap5,true,"conformPriorityMap5"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformPriorityMap6,true,"conformPriorityMap6"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_conformPriorityMap7,true,"conformPriorityMap7"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedPriorityMap0,true,"exceedPriorityMap0"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedPriorityMap1,true,"exceedPriorityMap1"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedPriorityMap2,true,"exceedPriorityMap2"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedPriorityMap3,true,"exceedPriorityMap3"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedPriorityMap4,true,"exceedPriorityMap4"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedPriorityMap5,true,"exceedPriorityMap5"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedPriorityMap6,true,"exceedPriorityMap6"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_exceedPriorityMap7,true,"exceedPriorityMap7"));
		vector<string > keyName;
        keyName.push_back ("priorityMapName");
        setUserDefinedKeyCombination (keyName);

    }

    void  PolicerPriorityMapManagedObject::setPriorityMapName(const string &priorityMapName)
    {
        m_priorityMapName  =  priorityMapName;
    }

    string  PolicerPriorityMapManagedObject::getPriorityMapName() const
    {
        return (m_priorityMapName);
    }

	void PolicerPriorityMapManagedObject::setUserConfiguredConform()
	{
		if((m_conformPriorityMap0.getUI32Value()==0)&&(m_conformPriorityMap1.getUI32Value()==1)&&(m_conformPriorityMap2.getUI32Value()==2)&&(m_conformPriorityMap3.getUI32Value()==3)&&(m_conformPriorityMap4.getUI32Value()==4)&&(m_conformPriorityMap5.getUI32Value()==5)&&(m_conformPriorityMap6.getUI32Value()==6)&&(m_conformPriorityMap7.getUI32Value()==7))
	    {
			trace (TRACE_LEVEL_DEVEL, "conform not set");
			
			m_conformPriorityMap0.setIsUserConfigured(false);
			m_conformPriorityMap1.setIsUserConfigured(false);
			m_conformPriorityMap2.setIsUserConfigured(false);
			m_conformPriorityMap3.setIsUserConfigured(false);
			m_conformPriorityMap4.setIsUserConfigured(false);
			m_conformPriorityMap5.setIsUserConfigured(false);
			m_conformPriorityMap6.setIsUserConfigured(false);
			m_conformPriorityMap7.setIsUserConfigured(false);
		}
		else
		{
			trace (TRACE_LEVEL_DEVEL, "conform set");
			m_conformPriorityMap0.setIsUserConfigured(true);
            m_conformPriorityMap1.setIsUserConfigured(true);
            m_conformPriorityMap2.setIsUserConfigured(true);
            m_conformPriorityMap3.setIsUserConfigured(true);
            m_conformPriorityMap4.setIsUserConfigured(true);
            m_conformPriorityMap5.setIsUserConfigured(true);
            m_conformPriorityMap6.setIsUserConfigured(true);
            m_conformPriorityMap7.setIsUserConfigured(true);
		}
	}

	void PolicerPriorityMapManagedObject::setUserConfiguredExceed()
    {
		if((m_exceedPriorityMap0.getUI32Value()==0)&&(m_exceedPriorityMap1.getUI32Value()==1)&&(m_exceedPriorityMap2.getUI32Value()==2)&&(m_exceedPriorityMap3.getUI32Value()==3)&&(m_exceedPriorityMap4.getUI32Value()==4)&&(m_exceedPriorityMap5.getUI32Value()==5)&&(m_exceedPriorityMap6.getUI32Value()==6)&&(m_exceedPriorityMap7.getUI32Value()==7))
        {
		    trace (TRACE_LEVEL_DEVEL, "exceed not set");	
			m_exceedPriorityMap0.setIsUserConfigured(false);
            m_exceedPriorityMap1.setIsUserConfigured(false);
            m_exceedPriorityMap2.setIsUserConfigured(false);
            m_exceedPriorityMap3.setIsUserConfigured(false);
            m_exceedPriorityMap4.setIsUserConfigured(false);
            m_exceedPriorityMap5.setIsUserConfigured(false);
            m_exceedPriorityMap6.setIsUserConfigured(false);
            m_exceedPriorityMap7.setIsUserConfigured(false);
		}
        else
        {
			trace (TRACE_LEVEL_DEVEL, "exceed set");
		    m_exceedPriorityMap0.setIsUserConfigured(true);
            m_exceedPriorityMap1.setIsUserConfigured(true);
            m_exceedPriorityMap2.setIsUserConfigured(true);
            m_exceedPriorityMap3.setIsUserConfigured(true);
            m_exceedPriorityMap4.setIsUserConfigured(true);
            m_exceedPriorityMap5.setIsUserConfigured(true);
            m_exceedPriorityMap6.setIsUserConfigured(true);
            m_exceedPriorityMap7.setIsUserConfigured(true);	
		}
    }

    void  PolicerPriorityMapManagedObject::setConformPriorityMap0(const UI32 &conformPriorityMap0)
    {
        if(conformPriorityMap0 != POLICER_NOT_USR_CFGED_VAL)
			m_conformPriorityMap0.setIsUserConfigured(true);
		else
			m_conformPriorityMap0.setIsUserConfigured(false);
		
		m_conformPriorityMap0.setUI32Value(conformPriorityMap0);
    }

    UI32  PolicerPriorityMapManagedObject::getConformPriorityMap0() const
    {
        return (m_conformPriorityMap0.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setConformPriorityMap1(const UI32 &conformPriorityMap1)
    {
		if(conformPriorityMap1 != POLICER_NOT_USR_CFGED_VAL)
            m_conformPriorityMap1.setIsUserConfigured(true);
        else
            m_conformPriorityMap1.setIsUserConfigured(false);

        m_conformPriorityMap1.setUI32Value(conformPriorityMap1);
    }

    UI32  PolicerPriorityMapManagedObject::getConformPriorityMap1() const
    {
        return (m_conformPriorityMap1.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setConformPriorityMap2(const UI32 &conformPriorityMap2)
    {
		if(conformPriorityMap2 != POLICER_NOT_USR_CFGED_VAL)
            m_conformPriorityMap2.setIsUserConfigured(true);
        else
            m_conformPriorityMap2.setIsUserConfigured(false);

        m_conformPriorityMap2.setUI32Value(conformPriorityMap2);
    }

    UI32  PolicerPriorityMapManagedObject::getConformPriorityMap2() const
    {
        return (m_conformPriorityMap2.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setConformPriorityMap3(const UI32 &conformPriorityMap3)
    {
		if(conformPriorityMap3 != POLICER_NOT_USR_CFGED_VAL)
            m_conformPriorityMap3.setIsUserConfigured(true);
        else
            m_conformPriorityMap3.setIsUserConfigured(false);

        m_conformPriorityMap3.setUI32Value(conformPriorityMap3);
    }

    UI32  PolicerPriorityMapManagedObject::getConformPriorityMap3() const
    {
        return (m_conformPriorityMap3.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setConformPriorityMap4(const UI32 &conformPriorityMap4)
    {
		if(conformPriorityMap4 != POLICER_NOT_USR_CFGED_VAL)
            m_conformPriorityMap4.setIsUserConfigured(true);
        else
            m_conformPriorityMap4.setIsUserConfigured(false);

        m_conformPriorityMap4.setUI32Value(conformPriorityMap4);
    }

    UI32  PolicerPriorityMapManagedObject::getConformPriorityMap4() const
    {
        return (m_conformPriorityMap4.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setConformPriorityMap5(const UI32 &conformPriorityMap5)
    {
		if(conformPriorityMap5 != POLICER_NOT_USR_CFGED_VAL)
            m_conformPriorityMap5.setIsUserConfigured(true);
        else
            m_conformPriorityMap5.setIsUserConfigured(false);

        m_conformPriorityMap5.setUI32Value(conformPriorityMap5);
    }

    UI32  PolicerPriorityMapManagedObject::getConformPriorityMap5() const
    {
        return (m_conformPriorityMap5.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setConformPriorityMap6(const UI32 &conformPriorityMap6)
    {
		if(conformPriorityMap6 != POLICER_NOT_USR_CFGED_VAL)
            m_conformPriorityMap6.setIsUserConfigured(true);
        else
            m_conformPriorityMap6.setIsUserConfigured(false);

        m_conformPriorityMap6.setUI32Value(conformPriorityMap6);
    }

    UI32  PolicerPriorityMapManagedObject::getConformPriorityMap6() const
    {
        return (m_conformPriorityMap6.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setConformPriorityMap7(const UI32 &conformPriorityMap7)
    {
		if(conformPriorityMap7 != POLICER_NOT_USR_CFGED_VAL)
            m_conformPriorityMap7.setIsUserConfigured(true);
        else
            m_conformPriorityMap7.setIsUserConfigured(false);

        m_conformPriorityMap7.setUI32Value(conformPriorityMap7);
    }

    UI32  PolicerPriorityMapManagedObject::getConformPriorityMap7() const
    {
        return (m_conformPriorityMap7.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setExceedPriorityMap0(const UI32 &exceedPriorityMap0)
    {
		if(exceedPriorityMap0 != POLICER_NOT_USR_CFGED_VAL)
            m_exceedPriorityMap0.setIsUserConfigured(true);
        else
          	m_exceedPriorityMap0.setIsUserConfigured(false);

        m_exceedPriorityMap0.setUI32Value(exceedPriorityMap0);
    }

    UI32  PolicerPriorityMapManagedObject::getExceedPriorityMap0() const
    {
        return (m_exceedPriorityMap0.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setExceedPriorityMap1(const UI32 &exceedPriorityMap1)
    {
		if(exceedPriorityMap1 != POLICER_NOT_USR_CFGED_VAL)
            m_exceedPriorityMap1.setIsUserConfigured(true);
        else
            m_exceedPriorityMap1.setIsUserConfigured(false);

        m_exceedPriorityMap1.setUI32Value(exceedPriorityMap1);
    }

    UI32  PolicerPriorityMapManagedObject::getExceedPriorityMap1() const
    {
        return (m_exceedPriorityMap1.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setExceedPriorityMap2(const UI32 &exceedPriorityMap2)
    {
		if(exceedPriorityMap2 != POLICER_NOT_USR_CFGED_VAL)
            m_exceedPriorityMap2.setIsUserConfigured(true);
        else
           	m_exceedPriorityMap2.setIsUserConfigured(false);

        m_exceedPriorityMap2.setUI32Value(exceedPriorityMap2);
    }

    UI32  PolicerPriorityMapManagedObject::getExceedPriorityMap2() const
    {
        return (m_exceedPriorityMap2.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setExceedPriorityMap3(const UI32 &exceedPriorityMap3)
    {
		if(exceedPriorityMap3 != POLICER_NOT_USR_CFGED_VAL)
            m_exceedPriorityMap3.setIsUserConfigured(true);
        else
            m_exceedPriorityMap3.setIsUserConfigured(false);

        m_exceedPriorityMap3.setUI32Value(exceedPriorityMap3);
    }

    UI32  PolicerPriorityMapManagedObject::getExceedPriorityMap3() const
    {
        return (m_exceedPriorityMap3.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setExceedPriorityMap4(const UI32 &exceedPriorityMap4)
    {
		if(exceedPriorityMap4 != POLICER_NOT_USR_CFGED_VAL)
           	m_exceedPriorityMap4.setIsUserConfigured(true);
        else
            m_exceedPriorityMap4.setIsUserConfigured(false);

        m_exceedPriorityMap4.setUI32Value(exceedPriorityMap4);
    }

    UI32  PolicerPriorityMapManagedObject::getExceedPriorityMap4() const
    {
        return (m_exceedPriorityMap4.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setExceedPriorityMap5(const UI32 &exceedPriorityMap5)
    {
		if(exceedPriorityMap5 != POLICER_NOT_USR_CFGED_VAL)
           	m_exceedPriorityMap5.setIsUserConfigured(true);
        else
           	m_exceedPriorityMap5.setIsUserConfigured(false);

        m_exceedPriorityMap5.setUI32Value(exceedPriorityMap5);
    }

    UI32  PolicerPriorityMapManagedObject::getExceedPriorityMap5() const
    {
        return (m_exceedPriorityMap5.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setExceedPriorityMap6(const UI32 &exceedPriorityMap6)
    {
		if(exceedPriorityMap6 != POLICER_NOT_USR_CFGED_VAL)
           	m_exceedPriorityMap6.setIsUserConfigured(true);
        else
            m_exceedPriorityMap6.setIsUserConfigured(false);

        m_exceedPriorityMap6.setUI32Value(exceedPriorityMap6);
    }

    UI32  PolicerPriorityMapManagedObject::getExceedPriorityMap6() const
    {
        return (m_exceedPriorityMap6.getUI32Value());
    }

    void  PolicerPriorityMapManagedObject::setExceedPriorityMap7(const UI32 &exceedPriorityMap7)
    {
		if(exceedPriorityMap7 != POLICER_NOT_USR_CFGED_VAL)
            m_exceedPriorityMap7.setIsUserConfigured(true);
        else
            m_exceedPriorityMap7.setIsUserConfigured(false);

        m_exceedPriorityMap7.setUI32Value(exceedPriorityMap7);
    }

    UI32  PolicerPriorityMapManagedObject::getExceedPriorityMap7() const
    {
        return (m_exceedPriorityMap7.getUI32Value());
    }

}
