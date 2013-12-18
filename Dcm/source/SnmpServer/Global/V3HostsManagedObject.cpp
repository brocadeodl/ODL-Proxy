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
#include "SnmpServer/Global/V3HostsManagedObject.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpUserManagedObject.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "vcs.h"
namespace DcmNs
{

    V3HostsManagedObject::V3HostsManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (V3HostsManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager)
    {
    }

    V3HostsManagedObject::V3HostsManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager,const HostUC &ip,const NotifyType &nType,const ObjectId &username,const UI32 &udpPort, const string &engineId, const SevLevel &sevLevel)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (V3HostsManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager),
        m_ip    (ip),
        m_nType    (nType),
        m_username    (username),
        m_udpPort    (udpPort),
        m_engineId    (engineId),
		m_sevLevel (sevLevel)
    {
    }

    V3HostsManagedObject::~V3HostsManagedObject ()
    {
    }

    string  V3HostsManagedObject::getClassName()
    {
        return ("V3HostsManagedObject");
    }

    void  V3HostsManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
		HostUC defaultHostUC("0.0.0.0");
        addPersistableAttribute (new AttributeHostUC(&m_ip, true, true, defaultHostUC, "hostip", snmp_server_hostip));
        addPersistableAttribute (new AttributeEnum((UI32 * )&m_nType,"notifytype", snmp_server_notifytype));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_username,"username", SnmpUserManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeUI32(&m_udpPort,"udpPort", snmp_server_udp_port));
        addPersistableAttribute (new AttributeString(&m_engineId,"engineId", snmp_server_engineid));
		addPersistableAttribute (new AttributeEnum((UI32 *)&m_sevLevel,"sevLevel", snmp_server_severity_level));
        
		//setUserTagForAttribute ("ip", t_ip);
		setUserTagForAttribute ("username", snmp_server_username);
    }

    void  V3HostsManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
		HostUC defaultHostUC("0.0.0.0");
        addPersistableAttributeForCreate  (new AttributeHostUC(&m_ip, true, true, defaultHostUC, "hostip", snmp_server_hostip));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 * )&m_nType,"notifytype", snmp_server_notifytype));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_username,"username", SnmpUserManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_udpPort,"udpPort", snmp_server_udp_port));
        addPersistableAttributeForCreate (new AttributeString(&m_engineId,"engineId", snmp_server_engineid));
		addPersistableAttributeForCreate (new AttributeEnum((UI32 *)&m_sevLevel,"sevLevel", snmp_server_severity_level));

		vector<string > keyList1;
        keyList1.push_back ("hostip");
		keyList1.push_back ("username");
        setUserDefinedKeyCombination (keyList1);
    }

    void  V3HostsManagedObject::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC  V3HostsManagedObject::getHostUC() const
    {
        return (m_ip);
    }

    void  V3HostsManagedObject::setNotify(const  NotifyType &nType)
    {
        m_nType  =  nType;
    }

    NotifyType  V3HostsManagedObject::getNotify() const
    {
        return (m_nType);
    }

    void  V3HostsManagedObject::setUsername(const ObjectId &username)
    {
        m_username  =  username;
    }

    ObjectId  V3HostsManagedObject::getUsername() const
    {
        return (m_username);
    }

    void  V3HostsManagedObject::setUdpPort(const UI32 &udpPort)
    {
        m_udpPort  =  udpPort;
    }

    UI32  V3HostsManagedObject::getUdpPort() const
    {
        return (m_udpPort);
    }
    void  V3HostsManagedObject::setEngineid(const string &engineId)
    {
        m_engineId  =  engineId;
    }

    string  V3HostsManagedObject::getEngineid() const
    {
        return (m_engineId);
    }

    void  V3HostsManagedObject::setSevLevel(const SevLevel &sevLevel)
    {
        m_sevLevel  =  sevLevel;
    }

    SevLevel V3HostsManagedObject::getSevLevel() const
    {
        return (m_sevLevel);
    }
}
