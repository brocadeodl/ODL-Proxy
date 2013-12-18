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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "SnmpServer/Global/CommunityManagedObject.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "SnmpServer/Global/enumdatatype.h"
//#include "snmp.h"
#include "vcs.h"
namespace DcmNs
{

    CommunityManagedObject::CommunityManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (CommunityManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager)
    {
    }

    CommunityManagedObject::CommunityManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager,const string &community,const Access &access)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (CommunityManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager),
        m_community    (community),
        m_access    (access)
    {
    }

    CommunityManagedObject::~CommunityManagedObject ()
    {
    }

    string  CommunityManagedObject::getClassName()
    {
        return ("CommunityManagedObject");
    }

    void  CommunityManagedObject::setupAttributesForPersistence()
    {
		DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_community,"community"));
        addPersistableAttribute (new AttributeEnum((UI32 * )&m_access,"access", snmp_server_access));
        setUserTagForAttribute ("community", snmp_server_community);
    }

    void  CommunityManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_community,"community", snmp_server_community));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 * )&m_access,"access", snmp_server_access));
        vector<string > keyList;
        keyList.push_back ("community");
        setUserDefinedKeyCombination (keyList);
    }

    void  CommunityManagedObject::setCommunity(const string &community)
    {
        m_community  =  community;
    }

    string  CommunityManagedObject::getCommunity() const
    {
        return (m_community);
    }

/*    string  CommunityManagedObject::getObjectId() const
    {
        return (m_community);
    }
*/
    void  CommunityManagedObject::setAccess(const Access &access)
    {
        m_access  =  access;
    }

    Access  CommunityManagedObject::getAccess() const
    {
        return (m_access);
    }

}
