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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalManagedObject.h"
#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgTypes.h"
#include "vcs.h"

namespace DcmNs
{

    PortCfgLocalManagedObject::PortCfgLocalManagedObject (PortCfgLocalObjectManager *pPortCfgLocalObjectManager)
        : PrismElement  (pPortCfgLocalObjectManager),
        PrismPersistableObject (PortCfgLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPortCfgLocalObjectManager),
        DcmManagedObject (pPortCfgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPortCfgLocalObjectManager),
		m_switchPort (-1),
		m_portType ("Te"),
		m_rbridgeId (-1),
		m_switchPortType ("TE"),
		m_slot (-1),
		m_phyPort (-1),
		m_fcoePort (-1),
		m_vlan (-1)

    {
    }

    PortCfgLocalManagedObject::PortCfgLocalManagedObject (PortCfgLocalObjectManager *pPortCfgLocalObjectManager,const string &id,const bool &portStateEnable,const bool &trunkEnable,const UI32 &areaNumber,const bool &longDistance,const bool &vcLinkInit,const bool &disabledEPort,const bool &lockedEPort,const bool &npivCapability,const bool &portAutoDisable,const bool &rateLimit,const bool &mirrorPort,const bool &creditRecovery,const bool &fportBuffers,const UI32 &npivPPLimit,const bool &csctlMode, const int &switchPort, const string &portType, const int &rbridgeId, const string &switchPortType, const int &slot, const int &phyPort, const int &fcoePort, const int &vlan)
        : PrismElement  (pPortCfgLocalObjectManager),
        PrismPersistableObject (PortCfgLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPortCfgLocalObjectManager),
        DcmManagedObject (pPortCfgLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPortCfgLocalObjectManager),
        m_id    (id),
        m_portStateEnable    (portStateEnable),
        m_trunkEnable    (trunkEnable),
        m_areaNumber    (areaNumber),
        m_longDistance    (longDistance),
        m_vcLinkInit    (vcLinkInit),
        m_disabledEPort    (disabledEPort),
        m_lockedEPort    (lockedEPort),
        m_npivCapability    (npivCapability),
        m_portAutoDisable    (portAutoDisable),
        m_rateLimit    (rateLimit),
        m_mirrorPort    (mirrorPort),
        m_creditRecovery    (creditRecovery),
        m_fportBuffers    (fportBuffers),
        m_npivPPLimit    (npivPPLimit),
        m_csctlMode    (csctlMode),
		m_switchPort	(switchPort),
		m_portType      (portType),
		m_rbridgeId     (rbridgeId),
		m_switchPortType      (switchPortType),
		m_slot (slot),
		m_phyPort (phyPort),
		m_fcoePort (fcoePort),
		m_vlan (vlan)
    {
    }

    PortCfgLocalManagedObject::~PortCfgLocalManagedObject ()
    {
    }

    string  PortCfgLocalManagedObject::getClassName()
    {
        return ("PortCfgLocalManagedObject");
    }

    void  PortCfgLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_id,"id"));

	    setUserTagForAttribute ("id", brocade_interface_name);

        addPersistableAttribute (new AttributeBool(&m_portStateEnable,"portStateEnable"));

	    //setUserTagForAttribute ("portStateEnable", fcoe_fabric_isl_enable);

        addPersistableAttribute (new AttributeBool(&m_trunkEnable,"trunkEnable"));

	    //setUserTagForAttribute ("trunkEnable", fcoe_fabric_isl_enable);

        addPersistableAttribute (new AttributeUI32(&m_areaNumber,"areaNumber"));
        addPersistableAttribute (new AttributeBool(&m_longDistance,"longDistance"));
        addPersistableAttribute (new AttributeBool(&m_vcLinkInit,"vcLinkInit"));
        addPersistableAttribute (new AttributeBool(&m_disabledEPort,"disabledEPort"));
        addPersistableAttribute (new AttributeBool(&m_lockedEPort,"lockedEPort"));
        addPersistableAttribute (new AttributeBool(&m_npivCapability,"npivCapability"));
        addPersistableAttribute (new AttributeBool(&m_portAutoDisable,"portAutoDisable"));
        addPersistableAttribute (new AttributeBool(&m_rateLimit,"rateLimit"));
        addPersistableAttribute (new AttributeBool(&m_mirrorPort,"mirrorPort"));
        addPersistableAttribute (new AttributeBool(&m_creditRecovery,"creditRecovery"));
        addPersistableAttribute (new AttributeBool(&m_fportBuffers,"fportBuffers"));
        addPersistableAttribute (new AttributeUI32(&m_npivPPLimit,"npivPPLimit"));
        addPersistableAttribute (new AttributeBool(&m_csctlMode,"csctlMode"));
        addPersistableAttribute (new AttributeSI32(&m_switchPort,"switchPort"));
		addPersistableAttribute (new AttributeString(&m_portType,"portType"));
		addPersistableAttribute (new AttributeSI32(&m_rbridgeId,"rbridgeId"));
		addPersistableAttribute (new AttributeString(&m_switchPortType,"switchPortType"));
		addPersistableAttribute (new AttributeSI32(&m_slot,"slot"));
		addPersistableAttribute (new AttributeSI32(&m_phyPort,"phyPort"));
		addPersistableAttribute (new AttributeSI32(&m_fcoePort,"fcoePort"));
		addPersistableAttribute (new AttributeSI32(&m_vlan,"vlan"));
    }

    void  PortCfgLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_id,"id"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_portStateEnable,"portStateEnable"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_trunkEnable,"trunkEnable"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_areaNumber,"areaNumber"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_longDistance,"longDistance"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_vcLinkInit,"vcLinkInit"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_disabledEPort,"disabledEPort"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_lockedEPort,"lockedEPort"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_npivCapability,"npivCapability"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_portAutoDisable,"portAutoDisable"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_rateLimit,"rateLimit"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_mirrorPort,"mirrorPort"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_creditRecovery,"creditRecovery"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_fportBuffers,"fportBuffers"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_npivPPLimit,"npivPPLimit"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_csctlMode,"csctlMode"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_switchPort,"switchPort"));
		addPersistableAttributeForCreate  (new AttributeString(&m_portType,"portType"));
		addPersistableAttributeForCreate  (new AttributeSI32(&m_rbridgeId,"rbridgeId"));
		addPersistableAttributeForCreate  (new AttributeString(&m_switchPortType,"switchPortType"));
		addPersistableAttributeForCreate  (new AttributeSI32(&m_slot,"slot"));
		addPersistableAttributeForCreate  (new AttributeSI32(&m_phyPort,"phyPort"));
		addPersistableAttributeForCreate  (new AttributeSI32(&m_fcoePort,"fcoePort"));
		addPersistableAttributeForCreate  (new AttributeSI32(&m_vlan,"vlan"));
	    vector<string> keyName;
	    keyName.push_back ("id");
	    setUserDefinedKeyCombination (keyName);
    }

    void  PortCfgLocalManagedObject::setId(const string &id)
    {
        m_id  =  id;
    }

    string  PortCfgLocalManagedObject::getId() const
    {
        return (m_id);
    }

    void  PortCfgLocalManagedObject::setPortStateEnable(const bool &portStateEnable)
    {
        m_portStateEnable  =  portStateEnable;
    }

    bool  PortCfgLocalManagedObject::getPortStateEnable() const
    {
        return (m_portStateEnable);
    }

    void  PortCfgLocalManagedObject::setTrunkEnable(const bool &trunkEnable)
    {
        m_trunkEnable  =  trunkEnable;
    }

    bool  PortCfgLocalManagedObject::getTrunkEnable() const
    {
        return (m_trunkEnable);
    }

    void  PortCfgLocalManagedObject::setAreaNumber(const UI32 &areaNumber)
    {
        m_areaNumber  =  areaNumber;
    }

    UI32  PortCfgLocalManagedObject::getAreaNumber() const
    {
        return (m_areaNumber);
    }

    void  PortCfgLocalManagedObject::setLongDistance(const bool &longDistance)
    {
        m_longDistance  =  longDistance;
    }

    bool  PortCfgLocalManagedObject::getLongDistance() const
    {
        return (m_longDistance);
    }

    void  PortCfgLocalManagedObject::setVcLinkInit(const bool &vcLinkInit)
    {
        m_vcLinkInit  =  vcLinkInit;
    }

    bool  PortCfgLocalManagedObject::getVcLinkInit() const
    {
        return (m_vcLinkInit);
    }

    void  PortCfgLocalManagedObject::setDisabledEPort(const bool &disabledEPort)
    {
        m_disabledEPort  =  disabledEPort;
    }

    bool  PortCfgLocalManagedObject::getDisabledEPort() const
    {
        return (m_disabledEPort);
    }

    void  PortCfgLocalManagedObject::setLockedEPort(const bool &lockedEPort)
    {
        m_lockedEPort  =  lockedEPort;
    }

    bool  PortCfgLocalManagedObject::getLockedEPort() const
    {
        return (m_lockedEPort);
    }

    void  PortCfgLocalManagedObject::setNpivCapability(const bool &npivCapability)
    {
        m_npivCapability  =  npivCapability;
    }

    bool  PortCfgLocalManagedObject::getNpivCapability() const
    {
        return (m_npivCapability);
    }

    void  PortCfgLocalManagedObject::setPortAutoDisable(const bool &portAutoDisable)
    {
        m_portAutoDisable  =  portAutoDisable;
    }

    bool  PortCfgLocalManagedObject::getPortAutoDisable() const
    {
        return (m_portAutoDisable);
    }

    void  PortCfgLocalManagedObject::setRateLimit(const bool &rateLimit)
    {
        m_rateLimit  =  rateLimit;
    }

    bool  PortCfgLocalManagedObject::getRateLimit() const
    {
        return (m_rateLimit);
    }

    void  PortCfgLocalManagedObject::setMirrorPort(const bool &mirrorPort)
    {
        m_mirrorPort  =  mirrorPort;
    }

    bool  PortCfgLocalManagedObject::getMirrorPort() const
    {
        return (m_mirrorPort);
    }

    void  PortCfgLocalManagedObject::setCreditRecovery(const bool &creditRecovery)
    {
        m_creditRecovery  =  creditRecovery;
    }

    bool  PortCfgLocalManagedObject::getCreditRecovery() const
    {
        return (m_creditRecovery);
    }

    void  PortCfgLocalManagedObject::setFportBuffers(const bool &fportBuffers)
    {
        m_fportBuffers  =  fportBuffers;
    }

    bool  PortCfgLocalManagedObject::getFportBuffers() const
    {
        return (m_fportBuffers);
    }

    void  PortCfgLocalManagedObject::setNpivPPLimit(const UI32 &npivPPLimit)
    {
        m_npivPPLimit  =  npivPPLimit;
    }

    UI32  PortCfgLocalManagedObject::getNpivPPLimit() const
    {
        return (m_npivPPLimit);
    }

    void  PortCfgLocalManagedObject::setCsctlMode(const bool &csctlMode)
    {
        m_csctlMode  =  csctlMode;
    }

    bool  PortCfgLocalManagedObject::getCsctlMode() const
    {
        return (m_csctlMode);
    }

	void  PortCfgLocalManagedObject::setSwitchPort(const int &switchPort)
	{
		m_switchPort = switchPort;
	}

	int  PortCfgLocalManagedObject::getSwitchPort() const
	{
		return (m_switchPort);
	}

	void  PortCfgLocalManagedObject::setPortType(const string &portType)
	{
		 m_portType  =  portType;
	}

	string  PortCfgLocalManagedObject::getPortType() const
	{
		return (m_portType);
	}

	void  PortCfgLocalManagedObject::setRbridgeId(const int &rbridgeId)
	{
		m_rbridgeId = rbridgeId;
	}

	int  PortCfgLocalManagedObject::getRbridgeId() const
	{
		return (m_rbridgeId);
	}

	void  PortCfgLocalManagedObject::setSwitchPortType(const string &switchPortType)
	{
		 m_switchPortType  =  switchPortType;
	}

	string  PortCfgLocalManagedObject::getSwitchPortType() const
	{
		return (m_switchPortType);
	}

	void  PortCfgLocalManagedObject::setSlot(const int &slot)
	{
		m_slot = slot;
	}

	int  PortCfgLocalManagedObject::getSlot() const
	{
		return (m_slot);
	}

	void  PortCfgLocalManagedObject::setPhyPort(const int &phyPort)
	{
		m_phyPort = phyPort;
	}

	int  PortCfgLocalManagedObject::getPhyPort() const
	{
		return (m_phyPort);
	}

	void  PortCfgLocalManagedObject::setFcoePort(const int &fcoePort)
	{
		m_fcoePort = fcoePort;
	}

	int  PortCfgLocalManagedObject::getFcoePort() const
	{
		return (m_fcoePort);
	}

	void  PortCfgLocalManagedObject::setVlan(const int &vlan)
	{
		m_vlan = vlan;
	}

	int  PortCfgLocalManagedObject::getVlan() const
	{
		return (m_vlan);
	}
	
}
