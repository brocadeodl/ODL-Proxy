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
 *   Author : rjain                                                     *
 **************************************************************************/

#include "Pim/Local/BsrCandidateLocalManagedObject.h"
#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    BsrCandidateLocalManagedObject::BsrCandidateLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager)
        : PrismElement  (pPimLocalObjectManager),
        PrismPersistableObject (BsrCandidateLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPimLocalObjectManager),
        DcmManagedObject (pPimLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPimLocalObjectManager)
    {
    }

    BsrCandidateLocalManagedObject::~BsrCandidateLocalManagedObject ()
    {
    }

    string  BsrCandidateLocalManagedObject::getClassName()
    {
        return ("BsrCandidateLocalManagedObject");
    }

    void  BsrCandidateLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum((UI32*)(&m_intfType),
            PIM_INTF_TYPE_UNKWN, "intfType"));
        addPersistableAttribute (new AttributeString(&m_intfName,"intfName"));
        addPersistableAttribute (new AttributeUI16(&m_mask,"mask"));
        addPersistableAttribute (new AttributeUI32UC(&m_priority, true, 
            "priority"));
        
        /* Adding the XML tag mappings */
        setUserTagForAttribute("intfType", pim_bsr_cand_intf_type);
        setUserTagForAttribute("intfName", pim_bsr_cand_intf_id);
        setUserTagForAttribute("mask", pim_hash_mask_length);
        setUserTagForAttribute("priority", pim_bsr_cand_priority);
    }

    void  BsrCandidateLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate(new AttributeEnum((UI32*)(&m_intfType),
            PIM_INTF_TYPE_UNKWN, "intfType"));
        addPersistableAttributeForCreate(new AttributeString(&m_intfName,"intfName"));
        addPersistableAttributeForCreate(new AttributeUI16(&m_mask,"mask"));
        addPersistableAttributeForCreate(new AttributeUI32UC(&m_priority, true, 
            "priority"));
    }

    void BsrCandidateLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("intfType");
        keyName.push_back("intfName");
        setUserDefinedKeyCombination (keyName);
    }
    
    void  BsrCandidateLocalManagedObject::setIntfType(const PimIntfType &intfType)
    {
        m_intfType  =  intfType;
    }
    
    PimIntfType BsrCandidateLocalManagedObject::getIntfType()  const
    {
        return (m_intfType);
    }
    
    PimIntfType BsrCandidateLocalManagedObject::mapIntfTypeToEnumVal(const UI32 &intfType)
    {
        PimIntfType intfTypeVal = PIM_INTF_TYPE_UNKWN;
        switch(intfType) {
            case PIM_INTF_TYPE_TE:
                intfTypeVal = PIM_INTF_TYPE_TE;
                break;
            case PIM_INTF_TYPE_PO:
                intfTypeVal = PIM_INTF_TYPE_PO;
                break;
            case PIM_INTF_TYPE_VE:
                intfTypeVal = PIM_INTF_TYPE_VE;
                break;
        }
        
        return(intfTypeVal);
    }

    void  BsrCandidateLocalManagedObject::setIntfName(const string &intfName)
    {
        m_intfName  =  intfName;
    }

    string  BsrCandidateLocalManagedObject::getIntfName() const
    {
        return (m_intfName);
    }

    void  BsrCandidateLocalManagedObject::setMask(const UI16 &mask)
    {
        m_mask  =  mask;
    }

    UI16  BsrCandidateLocalManagedObject::getMask() const
    {
        return (m_mask);
    }
    
    void  BsrCandidateLocalManagedObject::setPriority(const UI32 &priority,
        bool userConfig)
    {
        m_priority.setIsUserConfigured(userConfig);
        m_priority.setUI32Value(priority);
    }

    UI32  BsrCandidateLocalManagedObject::getPriority() const
    {
        return (m_priority.getUI32Value());
    }
}
