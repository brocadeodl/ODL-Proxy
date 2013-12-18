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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skumaras                                                     *
 **************************************************************************/

#include "SnmpServer/Local/EngineIDLocalManagedObject.h"
#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "brocade-snmp.h"

namespace DcmNs
{

    EngineIDLocalManagedObject::EngineIDLocalManagedObject (SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager)
        : PrismElement  (pSnmpServerIntLocalObjectManager),
        PrismPersistableObject (EngineIDLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerIntLocalObjectManager),
       	DcmManagedObject (pSnmpServerIntLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pSnmpServerIntLocalObjectManager)
    {
    }

    EngineIDLocalManagedObject::EngineIDLocalManagedObject (SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager,const string &EngineID)
        : PrismElement  (pSnmpServerIntLocalObjectManager),
        PrismPersistableObject (EngineIDLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerIntLocalObjectManager),
        DcmManagedObject (pSnmpServerIntLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pSnmpServerIntLocalObjectManager),
        m_EngineID    (EngineID)
    {
    }

    EngineIDLocalManagedObject::~EngineIDLocalManagedObject ()
    {
    }

    string  EngineIDLocalManagedObject::getClassName()
    {
        return ("EngineIDLocalManagedObject");
    }

    void  EngineIDLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_EngineID,"EngineID", snmp_server_local));
    }

    void  EngineIDLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_EngineID,"EngineID", snmp_server_local));
    }

    void  EngineIDLocalManagedObject::setEngineID(const string &EngineID)
    {
        m_EngineID  =  EngineID;
    }

    string  EngineIDLocalManagedObject::getEngineID() const
    {
        return (m_EngineID);
    }

}
