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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include "LicenseService/Local/LicensePodLocalManagedObject.h"
#include "LicenseService/Local/LicenseServiceLocalObjectManager.h"
#include "LicenseService/Local/LicenseServiceTypes.h"
#include "vcs.h"
//#include "brocade-license.h"

namespace DcmNs
{

    LicensePodLocalManagedObject::LicensePodLocalManagedObject (LicenseServiceLocalObjectManager *pLicenseServiceLocalObjectManager)
        : PrismElement  (pLicenseServiceLocalObjectManager),
        PrismPersistableObject (LicensePodLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pLicenseServiceLocalObjectManager),
        DcmManagedObject (pLicenseServiceLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pLicenseServiceLocalObjectManager)
    {
    }

    LicensePodLocalManagedObject::LicensePodLocalManagedObject (LicenseServiceLocalObjectManager *pLicenseServiceLocalObjectManager,const string &portId,const string &assignedPodset,const string &operation)
        : PrismElement  (pLicenseServiceLocalObjectManager),
        PrismPersistableObject (LicensePodLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pLicenseServiceLocalObjectManager),
        DcmManagedObject (pLicenseServiceLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pLicenseServiceLocalObjectManager),
        m_portId    (portId),
        m_assignedPodset    (assignedPodset),
        m_operation    (operation)
    {
    }

    LicensePodLocalManagedObject::~LicensePodLocalManagedObject ()
    {
    }

    string  LicensePodLocalManagedObject::getClassName()
    {
        return ("LicensePodLocalManagedObject");
    }

    void  LicensePodLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_portId,"portId"));
        addPersistableAttribute (new AttributeString(&m_assignedPodset,"assignedPodset"));
        /* Default Value of Operation is Release, which is hidden */
        addPersistableAttribute (new AttributeString(&m_operation,true, "release", "operation", brocade_license_operation));

        setUserTagForAttribute ("portId", brocade_license_port_id);
    }

    void  LicensePodLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_portId,"portId"));
        addPersistableAttributeForCreate  (new AttributeString(&m_assignedPodset,"assignedPodset"));
        /* Default Value of Operation is Release, which is hidden */
        addPersistableAttributeForCreate  (new AttributeString(&m_operation,true, "release", "operation", brocade_license_operation));

        vector<string> keyName;
        keyName.push_back ("portId");
        setUserDefinedKeyCombination (keyName);
    }

    void  LicensePodLocalManagedObject::setPortId(const string &portId)
    {
        m_portId  =  portId;
    }

    string  LicensePodLocalManagedObject::getPortId() const
    {
        return (m_portId);
    }

    void  LicensePodLocalManagedObject::setAssignedPodset(const string &assignedPodset)
    {
        m_assignedPodset  =  assignedPodset;
    }

    string  LicensePodLocalManagedObject::getAssignedPodset() const
    {
        return (m_assignedPodset);
    }

    void  LicensePodLocalManagedObject::setOperation(const string &operation)
    {
        m_operation  =  operation;
    }

    string  LicensePodLocalManagedObject::getOperation() const
    {
        return (m_operation);
    }

}
