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

#include "Framework/Types/HostUC.h"
#include "SnmpServer/Global/HostsManagedObject.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/CommunityManagedObject.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "SnmpServer/Global/enumdatatype.h"
//#include "snmp.h"
#include "vcs.h"
namespace DcmNs
{

    HostsManagedObject::HostsManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (HostsManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager)
    {
    }

    HostsManagedObject::HostsManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager,const HostUC &ip,const Version &version,const ObjectId &community,const UI32 &udpPort, const SevLevel &sevLevel)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (HostsManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager),
        m_ip    (ip),
        m_version    (version),
        m_community    (community),
        m_udpPort    (udpPort),
        m_sevLevel    (sevLevel)
    {
    }

    HostsManagedObject::~HostsManagedObject ()
    {
    }

    string  HostsManagedObject::getClassName()
    {
        return ("HostsManagedObject");
    }

    void  HostsManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
		HostUC defaultHostUC("0.0.0.0");
        addPersistableAttribute (new AttributeHostUC(&m_ip, true, true, defaultHostUC, "ip", snmp_server_ip));
        addPersistableAttribute (new AttributeEnum((UI32 * )&m_version,"version", snmp_server_version));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_community,"community", CommunityManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeUI32(&m_udpPort,"udpPort", snmp_server_udp_port));
		//setUserTagForAttribute ("ip", t_ip);
		setUserTagForAttribute ("community", snmp_server_community);
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_sevLevel, "sevLevel", snmp_server_severity_level));
    }

    void  HostsManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
		HostUC defaultHostUC("0.0.0.0");
        addPersistableAttributeForCreate  (new AttributeHostUC(&m_ip, true, true, defaultHostUC, "ip", snmp_server_ip));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 * )&m_version,"version", snmp_server_version));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_community,"community", CommunityManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_udpPort,"udpPort", snmp_server_udp_port));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_sevLevel,"sevLevel",snmp_server_severity_level));

		vector<string > keyList1;
        keyList1.push_back ("ip");
		keyList1.push_back ("community");
        setUserDefinedKeyCombination (keyList1);
    }

    void  HostsManagedObject::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC HostsManagedObject::getHostUC() const
    {
        return (m_ip);
    }

    void  HostsManagedObject::setVersion(const Version &version)
    {
        m_version  =  version;
    }

    Version  HostsManagedObject::getVersion() const
    {
        return (m_version);
    }

    void  HostsManagedObject::setCommunity(const ObjectId &community)
    {
        m_community  =  community;
    }

    ObjectId  HostsManagedObject::getCommunity() const
    {
        return (m_community);
    }

    void  HostsManagedObject::setUdpPort(const UI32 &udpPort)
    {
        m_udpPort  =  udpPort;
    }

    UI32  HostsManagedObject::getUdpPort() const
    {
        return (m_udpPort);
    }

    void  HostsManagedObject::setSevLevel(const SevLevel &sevLevel)
    {
        m_sevLevel  =  sevLevel;
    }

    SevLevel HostsManagedObject::getSevLevel() const
    {
        return (m_sevLevel);
    }
}
