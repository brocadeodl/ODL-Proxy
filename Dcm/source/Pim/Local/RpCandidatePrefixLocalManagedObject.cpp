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
 *   Author : Chirag Taunk                                                 *
 **************************************************************************/

#include "Pim/Local/RpCandidatePrefixLocalManagedObject.h"
#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimTypes.h"
#include "vcs.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    RpCandidatePrefixLocalManagedObject::RpCandidatePrefixLocalManagedObject (
		PimLocalObjectManager *pPimLocalObjectManager)
        : PrismElement  (pPimLocalObjectManager),
        PrismPersistableObject (RpCandidatePrefixLocalManagedObject::
		getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPimLocalObjectManager),
        DcmManagedObject (pPimLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPimLocalObjectManager)
    {
    }

    RpCandidatePrefixLocalManagedObject::~RpCandidatePrefixLocalManagedObject()
    {
    }

    string  RpCandidatePrefixLocalManagedObject::getClassName()
    {
        return ("RpCandidatePrefixLocalManagedObject");
    }

    void  RpCandidatePrefixLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute(new AttributeEnum((UI32*)(&m_prefixAction), 
			PIM_PREFIX_ACTION_UNKWN, "prefixAction"));
        addPersistableAttribute(new AttributeIpV4Address(&m_prefixIP,"prefixIP"));
        addPersistableAttribute(new AttributeUI16(&m_prefixLen,"prefixLen"));
        
        /* Adding the XML tag mappings */
		setUserTagForAttribute("prefixAction", pim_rp_cand_grp_prefix_action);
        setUserTagForAttribute("prefixIP", pim_rp_cand_grp_prefix);
        setUserTagForAttribute("prefixLen", pim_rp_cand_grp_prefix_length);
    }

    void  RpCandidatePrefixLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate(new AttributeEnum((UI32*)
			(&m_prefixAction), PIM_PREFIX_ACTION_UNKWN, "prefixAction"));
        addPersistableAttributeForCreate(new AttributeIpV4Address(&m_prefixIP,
			"prefixIP"));
        addPersistableAttributeForCreate(new AttributeUI16(&m_prefixLen,
			"prefixLen"));
    }

    void RpCandidatePrefixLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("prefixIP");
		keyName.push_back("prefixLen");
        setUserDefinedKeyCombination (keyName);
    }
    
	void  RpCandidatePrefixLocalManagedObject::setAction(
		const PimPrefixActionType &prefixAction)
    {
        m_prefixAction  =  prefixAction;
    }

    PimPrefixActionType RpCandidatePrefixLocalManagedObject::getAction()  const
    {
        return (m_prefixAction);
    }

    PimPrefixActionType RpCandidatePrefixLocalManagedObject::
		mapPrefixActionToEnumVal(const UI8 &prefixAction)
    {
        PimPrefixActionType actionVal = PIM_PREFIX_ACTION_UNKWN;
        switch(prefixAction) {
            case PIM_PREFIX_ACTION_ADD:
                actionVal = PIM_PREFIX_ACTION_ADD;
                break;
            case PIM_PREFIX_ACTION_DELETE:
                actionVal = PIM_PREFIX_ACTION_DELETE;
                break;
        }

        return(actionVal);
    }


    void  RpCandidatePrefixLocalManagedObject::setPrefixIP(const 
		IpV4Address &prefixIP)
    {
        m_prefixIP  =  prefixIP;
    }

    IpV4Address  RpCandidatePrefixLocalManagedObject::getPrefixIP() const
    {
        return (m_prefixIP);
    }

    void  RpCandidatePrefixLocalManagedObject::setPrefixLen(const UI16 
		&prefixLen)
    {
        m_prefixLen  =  prefixLen;
    }

    UI16  RpCandidatePrefixLocalManagedObject::getPrefixLen() const
    {
        return (m_prefixLen);
    }

}
