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
 *   Author : rjain                                                        *
 **************************************************************************/

#include "PimIntfCfgLocalManagedObject.h"
#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimTypes.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "vcs.h"

namespace DcmNs
{

    PimIntfCfgLocalManagedObject::PimIntfCfgLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement (pNsmLocalObjectManager),
        PrismPersistableObject(PimIntfCfgLocalManagedObject::getClassName (), 
            DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    PimIntfCfgLocalManagedObject::~PimIntfCfgLocalManagedObject ()
    {
    }

    string  PimIntfCfgLocalManagedObject::getClassName()
    {
        return ("PimIntfCfgLocalManagedObject");
    }

    void  PimIntfCfgLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_enabled,"enabled"));
		/* commenting : Changing the type as we dont accept acl name now */
#if 0
        addPersistableAttribute (new AttributeStringUC(&m_multBoundaryAcl, true,
			"multBoundaryAcl"));
#endif
		addPersistableAttribute (new AttributeBoolUC(&m_multBoundaryAcl, true,
			"multBoundaryAcl"));

        addPersistableAttribute (new AttributeUI32UC(&m_drPriority, true, 
            "drPriority"));
        addPersistableAttribute (new AttributeBoolUC(&m_pimBorder, true, 
            "pimBorder"));
        addPersistableAttribute (new AttributeUI32UC(&m_ttlThold, true, 
            "ttlThold"));
        
        /* Adding the XML tag mappings */
        setUserTagForAttribute("enabled", pim_pim_sparse);
        setUserTagForAttribute("drPriority", pim_dr_priority);
		/* commenting : Changing the type as we dont accept acl name now */
#if 0
        setUserTagForAttribute("multBoundaryAcl", pim_mcast_bdry_prefix_list);
#endif
		setUserTagForAttribute("multBoundaryAcl", pim_multicast_boundary);

        setUserTagForAttribute("ttlThold", pim_ttl_threshold);
        
        /* Commenting this code till the border command is supported */
        /*setUserTagForAttribute("pimBorder", pim_border); */
    }

    void  PimIntfCfgLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate(new AttributeBool(&m_enabled,"enabled"));
		/* commenting : Changing the type as  we are not accepting acl name now */
#if 0
        addPersistableAttributeForCreate(new AttributeStringUC(&m_multBoundaryAcl,
			true, "multBoundaryAcl"));
#endif
		addPersistableAttributeForCreate(new AttributeBoolUC(&m_multBoundaryAcl,
			true, "multBoundaryAcl"));

        addPersistableAttributeForCreate(new AttributeUI32UC(&m_drPriority, true, 
            "drPriority"));
        addPersistableAttributeForCreate(new AttributeUI32UC(&m_ttlThold, true, 
            "ttlThold"));
        addPersistableAttributeForCreate(new AttributeBoolUC(&m_pimBorder, true, 
            "pimBorder"));
    }

    void  PimIntfCfgLocalManagedObject::setEnabled(const bool &enabled)
    {
        m_enabled  =  enabled;
    }

    bool  PimIntfCfgLocalManagedObject::getEnabled() const
    {
        return (m_enabled);
    }

    void  PimIntfCfgLocalManagedObject::setDrPriority(const UI32 &drPriority, 
        bool userConfig)
    {
        m_drPriority.setIsUserConfigured(userConfig);
        m_drPriority.setUI32Value(drPriority);
    }

    UI32  PimIntfCfgLocalManagedObject::getDrPriority() const
    {
        if(m_drPriority.getIsUserConfigured())
            return (m_drPriority.getUI32Value());
        else
            return(PIM_DFLT_DR_PRIO);
    }
	
	/* Commenting : Changing the type, now we dont accept the acl name */
#if 0
    void  PimIntfCfgLocalManagedObject::setMultBoundaryAcl(const string 
			&multBoundaryAcl, bool userConfig)
    {
		m_multBoundaryAcl.setIsUserConfigured(userConfig);
        m_multBoundaryAcl.setStringValue(multBoundaryAcl);
    }

    string  PimIntfCfgLocalManagedObject::getMultBoundaryAcl() const
    {
		return(m_multBoundaryAcl.getStringValue());
    }
#endif
	void  PimIntfCfgLocalManagedObject::setMultBoundaryAcl(const bool
		&multBoundaryAcl, bool userConfig)
	{
		m_multBoundaryAcl.setIsUserConfigured(userConfig);
		m_multBoundaryAcl.setBoolValue(multBoundaryAcl);
	}
	bool PimIntfCfgLocalManagedObject::getMultBoundaryAcl() const
	{
		if(m_multBoundaryAcl.getIsUserConfigured())
			return(m_multBoundaryAcl.getBoolValue());
		else
			return(PIM_NOT_USR_CFGED_VAL);
	}

    void  PimIntfCfgLocalManagedObject::setPimBorder(const bool &pimBorder,
        bool userConfig)
    {
        m_pimBorder.setIsUserConfigured(userConfig);
        m_pimBorder.setBoolValue(pimBorder);
    }

    bool  PimIntfCfgLocalManagedObject::getPimBorder() const
    {
        if(m_pimBorder.getIsUserConfigured())
            return(m_pimBorder.getBoolValue());
        else
            return(PIM_NOT_USR_CFGED_VAL);
    }
    
    void  PimIntfCfgLocalManagedObject::setTtlThold(const UI32 &ttlThold, 
        bool userConfig)
    {
        m_ttlThold.setIsUserConfigured(userConfig);
        m_ttlThold.setUI32Value(ttlThold);
    }

    UI32  PimIntfCfgLocalManagedObject::getTtlThold() const
    {
        if(m_ttlThold.getIsUserConfigured())
            return(m_ttlThold.getUI32Value());
        else
            return(PIM_DFLT_TTL_THRESH);
    }

}
