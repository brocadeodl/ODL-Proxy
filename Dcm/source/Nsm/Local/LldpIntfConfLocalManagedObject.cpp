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
 *   Author : sanjeevj                                                     *
 **************************************************************************/

#include "Nsm/Local/LldpIntfConfLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Lldp/Global/LldpGlobalProfileConfManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "vcs.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    LldpIntfConfLocalManagedObject::LldpIntfConfLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (LldpIntfConfLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_dcbxVersion = LLDP_DCBX_VERSION_AUTO;
		m_disable = false;
		m_iscsiPriority = DEFAULT_ISCSI_PRIORITY;
		m_lldpCeeType = LLDP_CEE_OFF;
    }

    LldpIntfConfLocalManagedObject::LldpIntfConfLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const LldpDcbxVersion &dcbxVersion,const bool &disable,const ObjectId &profileId,const UI32 &iscsiPriority)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (LldpIntfConfLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_dcbxVersion    (dcbxVersion),
        m_disable    (disable),
        m_profileId    (profileId),
        m_iscsiPriority    (iscsiPriority)
    {
		m_lldpCeeType = LLDP_CEE_OFF;
    }

    LldpIntfConfLocalManagedObject::~LldpIntfConfLocalManagedObject ()
    {
    }

    string  LldpIntfConfLocalManagedObject::getClassName()
    {
        return ("LldpIntfConfLocalManagedObject");
    }

    void  LldpIntfConfLocalManagedObject::setupAttributesForPersistence()
    {
		UI32 appTypeDefault = (UI32)LLDP_DCBX_VERSION_AUTO;
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum((UI32*)&m_dcbxVersion,appTypeDefault,"dcbxVersion",lldp_dcbx_version));
        addPersistableAttribute (new AttributeBool(&m_disable,"disable",lldp_disable));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_profileId,"profileId", LldpGlobalProfileConfManagedObject::getClassName (), true));
        addPersistableAttribute (new AttributeUI32(&m_iscsiPriority,"iscsiPriority",lldp_iscsi_priority));
		addPersistableAttribute (new AttributeEnum((UI32*)(&m_lldpCeeType), "lldpCeeType", lldp_lldp_cee_on_off));

		setUserTagForAttribute ("profileId", lldp_profile);
    }

    void  LldpIntfConfLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_dcbxVersion,"dcbxVersion",lldp_dcbx_version));
        addPersistableAttributeForCreate  (new AttributeBool(&m_disable,"disable",lldp_disable));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_profileId,"profileId", LldpGlobalProfileConfManagedObject::getClassName (), true));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_iscsiPriority,"iscsiPriority",lldp_iscsi_priority));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_lldpCeeType), "lldpCeeType", lldp_lldp_cee_on_off));
    }

    void LldpIntfConfLocalManagedObject::setLldpCeeStatus(const lldp_cee_on_off_t &lldpCeeType)
    {
        m_lldpCeeType = lldpCeeType;
    }

    lldp_cee_on_off_t LldpIntfConfLocalManagedObject::getLldpCeeStatus() const
    {
        return m_lldpCeeType;
    }

    void  LldpIntfConfLocalManagedObject::setDcbxVersion(const LldpDcbxVersion &dcbxVersion)
    {
        m_dcbxVersion  =  dcbxVersion;
    }

    LldpDcbxVersion  LldpIntfConfLocalManagedObject::getDcbxVersion() const
    {
        return (m_dcbxVersion);
    }

    void  LldpIntfConfLocalManagedObject::setDisable(const bool &disable)
    {
        m_disable  =  disable;
    }

    bool  LldpIntfConfLocalManagedObject::getDisable() const
    {
        return (m_disable);
    }

    void  LldpIntfConfLocalManagedObject::setProfileId(const ObjectId &profileId)
    {
        m_profileId  =  profileId;
    }

    ObjectId  LldpIntfConfLocalManagedObject::getProfileId() const
    {
        return (m_profileId);
    }
    void  LldpIntfConfLocalManagedObject::setIscsiPriority(const UI32 &iscsiPriority)
    {
        m_iscsiPriority  =  iscsiPriority;
    }

    UI32  LldpIntfConfLocalManagedObject::getIscsiPriority() const
    {
        return (m_iscsiPriority);
    }

}
