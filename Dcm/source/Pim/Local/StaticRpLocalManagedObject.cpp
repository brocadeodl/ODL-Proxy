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

#include "Pim/Local/StaticRpLocalManagedObject.h"
#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimTypes.h"
#include "vcs.h"

namespace DcmNs
{

    StaticRpLocalManagedObject::StaticRpLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager)
        : PrismElement  (pPimLocalObjectManager),
        PrismPersistableObject (StaticRpLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPimLocalObjectManager),
        DcmManagedObject (pPimLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPimLocalObjectManager)
    {
    }

    StaticRpLocalManagedObject::~StaticRpLocalManagedObject ()
    {
    }

    string  StaticRpLocalManagedObject::getClassName()
    {
        return ("StaticRpLocalManagedObject");
    }

    void  StaticRpLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute(new AttributeIpV4Address(&m_rpAddr,"rpAddr"));
        addPersistableAttribute(new AttributeStringUC(&m_prefixList, true,
            "prefixList"));
        
        /* Adding the XML tag mappings */
        setUserTagForAttribute("rpAddr", pim_rp_ip_addr);
        setUserTagForAttribute("prefixList", pim_prefix_list);
    }

    void  StaticRpLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate(new AttributeIpV4Address(&m_rpAddr,"rpAddr"));
        addPersistableAttributeForCreate(new AttributeStringUC(&m_prefixList,
            true, "prefixList"));
    }

    void StaticRpLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("rpAddr");
        setUserDefinedKeyCombination (keyName);
    }
    
    void  StaticRpLocalManagedObject::setRpAddr(const IpV4Address &rpAddr)
    {
        m_rpAddr  =  rpAddr;
    }

    IpV4Address  StaticRpLocalManagedObject::getRpAddr() const
    {
        return (m_rpAddr);
    }

    void  StaticRpLocalManagedObject::setPrefixList(const string &prefixList,
        bool userConfig)
    {
        m_prefixList.setStringValue(prefixList);
        m_prefixList.setIsUserConfigured(userConfig);
    }

    string  StaticRpLocalManagedObject::getPrefixList() const
    {
        return (m_prefixList.getStringValue());
    }

}
