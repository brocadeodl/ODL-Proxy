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

#include "Pim/Local/RpCandidateLocalManagedObject.h"
#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    RpCandidateLocalManagedObject::RpCandidateLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager)
        : PrismElement  (pPimLocalObjectManager),
        PrismPersistableObject (RpCandidateLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPimLocalObjectManager),
        DcmManagedObject (pPimLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPimLocalObjectManager)
    {
    }

    RpCandidateLocalManagedObject::~RpCandidateLocalManagedObject ()
    {
    }

    string  RpCandidateLocalManagedObject::getClassName()
    {
        return ("RpCandidateLocalManagedObject");
    }

    void  RpCandidateLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum((UI32*)(&m_intfType),
            PIM_INTF_TYPE_UNKWN, "intfType"));
        addPersistableAttribute (new AttributeString(&m_intfName,"intfName"));
        addPersistableAttribute (new AttributeStringUC(&m_prefixList, true, 
            "prefixList"));
        
        /* Adding the XML tag mappings */
        setUserTagForAttribute("intfType", pim_rp_cand_intf_type);
        setUserTagForAttribute("intfName", pim_rp_cand_intf_id);
        /* Commenting this code till prefix lists are supported */
        /*setUserTagForAttribute("prefixList", pim_rp_cand_prefix_list); */
    }

    void  RpCandidateLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate(new AttributeEnum((UI32*)(&m_intfType),
            PIM_INTF_TYPE_UNKWN, "intfType"));
        addPersistableAttributeForCreate(new AttributeString(&m_intfName,"intfName"));
        addPersistableAttributeForCreate(new AttributeStringUC(&m_prefixList,
            true, "prefixList"));
    }

    void RpCandidateLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("intfType");
        keyName.push_back("intfName");
        setUserDefinedKeyCombination (keyName);
    }
    
    void  RpCandidateLocalManagedObject::setIntfType(const PimIntfType &intfType)
    {
        m_intfType  =  intfType;
    }
    
    PimIntfType RpCandidateLocalManagedObject::getIntfType()  const
    {
        return (m_intfType);
    }
    
    PimIntfType RpCandidateLocalManagedObject::mapIntfTypeToEnumVal(const UI32 &intfType)
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

    void  RpCandidateLocalManagedObject::setIntfName(const string &intfName)
    {
        m_intfName  =  intfName;
    }

    string  RpCandidateLocalManagedObject::getIntfName() const
    {
        return (m_intfName);
    }

    void  RpCandidateLocalManagedObject::setPrefixList(const string &prefixList,
        bool userConfig)
    {
        m_prefixList.setStringValue(prefixList);
        m_prefixList.setIsUserConfigured(userConfig);
    }

    string  RpCandidateLocalManagedObject::getPrefixList() const
    {
        return (m_prefixList.getStringValue());
    }

}
