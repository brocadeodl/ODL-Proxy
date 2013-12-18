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
 *   Author : skrastog                                                     *
 **************************************************************************/

#include "Nsm/Local/PortSecurityLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/PortSecAllowedMacsLocalManagedObject.h"
#include "Nsm/Local/PortSecStickyMacsLocalManagedObject.h"
#include "Nsm/Local/PortSecOuiLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Nsm/Local/NsmTypes.h"

#include "brocade-interface.h"

namespace DcmNs
{

    PortSecurityLocalManagedObject::PortSecurityLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PortSecurityLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_enabled = false;
		m_numAllowedMacs = 0;
		m_numOuis = 0;
		m_action = 0;
		m_sticky = false;
		m_ifName = "";
		m_ifType = IF_TYPE_INVALID;
		m_allowedMacs.clear();
		m_ouis.clear();
		m_shutdownTime = 0;
		m_numStaticSecureMacs = 0;
    }

    PortSecurityLocalManagedObject::PortSecurityLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PortSecurityLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager)
    {
		m_enabled = false;
		m_numAllowedMacs = 0;
		m_numOuis = 0;
		m_action = 0;
		m_sticky = false;
		m_ifName = "";
		m_ifType = IF_TYPE_INVALID;
		m_allowedMacs.clear();
		m_ouis.clear();
		m_shutdownTime = 0;
		m_numStaticSecureMacs = 0;
    }

    PortSecurityLocalManagedObject::~PortSecurityLocalManagedObject ()
    {
    }

    string  PortSecurityLocalManagedObject::getClassName()
    {
        return ("PortSecurityLocalManagedObject");
    }

    void  PortSecurityLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_enabled, "is_enabled", brocade_interface_port_security));
        addPersistableAttribute (new AttributeString(&m_ifName, "ifName"));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_ifType, "ifType"));
        addPersistableAttribute (new AttributeUI32(&m_numAllowedMacs, 0, "num_allowed_macs", brocade_interface_port_sec_max));
        addPersistableAttribute (new AttributeSI32(&m_action,"action"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<PortSecAllowedMacsLocalManagedObject>(&m_allowedMacs, "allowed_macs", getClassName(), getObjectId(), PortSecAllowedMacsLocalManagedObject::getClassName()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<PortSecStickyMacsLocalManagedObject>(&m_stickyMacs, "stickyMacs", getClassName(), getObjectId(), PortSecStickyMacsLocalManagedObject::getClassName()));
        addPersistableAttribute (new AttributeUI32(&m_numOuis, "numOuis"));
        addPersistableAttribute (new AttributeUI32(&m_numStaticSecureMacs, "numStaticSecureMacs"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<PortSecOuiLocalManagedObject>(&m_ouis, "ouis", getClassName(), getObjectId(), PortSecOuiLocalManagedObject::getClassName()));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_newAction, 0, "newAction", brocade_interface_port_sec_violation));
        addPersistableAttribute (new AttributeBool(&m_sticky, "sticky", brocade_interface_sticky));
        addPersistableAttribute (new AttributeUI32(&m_shutdownTime, 0, "shutdownTime", brocade_interface_shutdown_time));
    }

    void  PortSecurityLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeBool(&m_enabled, "is_enabled", brocade_interface_port_security));
        addPersistableAttributeForCreate (new AttributeString(&m_ifName, "ifName"));
        addPersistableAttributeForCreate (new AttributeEnum((UI32 *)&m_ifType, "ifType"));
        addPersistableAttributeForCreate (new AttributeUI32(&m_numAllowedMacs, 0, "num_allowed_macs", brocade_interface_port_sec_max));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_action,"action"));
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<PortSecAllowedMacsLocalManagedObject>(&m_allowedMacs, "allowed_macs", getClassName(), getObjectId(), PortSecAllowedMacsLocalManagedObject::getClassName()));
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<PortSecStickyMacsLocalManagedObject>(&m_stickyMacs, "stickyMacs", getClassName(), getObjectId(), PortSecStickyMacsLocalManagedObject::getClassName()));
        addPersistableAttributeForCreate (new AttributeUI32(&m_numOuis, "numOuis"));
        addPersistableAttributeForCreate (new AttributeUI32(&m_numStaticSecureMacs, "numStaticSecureMacs"));
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<PortSecOuiLocalManagedObject>(&m_ouis, "ouis", getClassName(), getObjectId(), PortSecOuiLocalManagedObject::getClassName()));
        addPersistableAttributeForCreate (new AttributeEnum((UI32 *)&m_newAction, 0, "newAction", brocade_interface_port_sec_violation));
        addPersistableAttributeForCreate (new AttributeBool(&m_sticky, "sticky", brocade_interface_sticky));
        addPersistableAttributeForCreate (new AttributeUI32(&m_shutdownTime, 0, "shutdownTime", brocade_interface_shutdown_time));
    }

    void  PortSecurityLocalManagedObject::setEnabled(const bool &isEnabled)
    {
        m_enabled  =  isEnabled;
    }

    bool  PortSecurityLocalManagedObject::getEnabled() const
    {
        return m_enabled;
    }

    void  PortSecurityLocalManagedObject::setNumAllowedMacs(const UI32 &numAllowedMacs)
    {
        m_numAllowedMacs  =  numAllowedMacs;
    }

    UI32  PortSecurityLocalManagedObject::getNumAllowedMacs() const
    {
        return m_numAllowedMacs;
    }

    void  PortSecurityLocalManagedObject::setNumOuis(const UI32 &numOuis)
    {
        m_numOuis  =  numOuis;
    }

    UI32  PortSecurityLocalManagedObject::getNumOuis() const
    {
        return m_numOuis;
    }

    void  PortSecurityLocalManagedObject::setStaticSecureMacs(const UI32 &macs)
    {
        m_numStaticSecureMacs  =  macs;
    }

    UI32  PortSecurityLocalManagedObject::getStaticSecureMacs() const
    {
        return m_numStaticSecureMacs;
    }

    void  PortSecurityLocalManagedObject::setShutdownTime(const UI32 &time)
    {
        m_shutdownTime  =  time;
    }

    UI32  PortSecurityLocalManagedObject::getShutdownTime() const
    {
        return m_shutdownTime;
    }

    void  PortSecurityLocalManagedObject::setAllowedMacs(const vector<WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject> > &allowedMacs)
    {
        m_allowedMacs  =  allowedMacs;
    }

    void PortSecurityLocalManagedObject::addAllowedMac (PortSecAllowedMacsLocalManagedObject *allowedMacMO)
    {
        WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject > allowedMac (allowedMacMO);
		UI32 i, size = m_allowedMacs.size();

		for (i = 0; i < size; i++) {
			if ((allowedMac->m_allowedMac.compare(m_allowedMacs[i]->m_allowedMac) == 0) && allowedMac->m_vlanId == m_allowedMacs[i]->m_vlanId) {
				return;
			}
		}

       	m_allowedMacs.push_back(allowedMac);
		m_numStaticSecureMacs++;
    }

    vector<WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject> >  PortSecurityLocalManagedObject::getAllowedMacs() const
    {
        return m_allowedMacs;
    }

	void PortSecurityLocalManagedObject::deleteAllowedMacs()
	{
		UI32 size = m_allowedMacs.size(), i;

		for (i = 0; i < size; i++) {
			m_allowedMacs.erase(m_allowedMacs.begin() + i);
		}

		m_numStaticSecureMacs = 0;
	}

    void  PortSecurityLocalManagedObject::setStickyMacs(const vector<WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject> > &stickyMacs)
    {
        m_stickyMacs  =  stickyMacs;
    }

    vector<WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject> >  PortSecurityLocalManagedObject::getStickyMacs() const
    {
        return m_stickyMacs;
    }

	void PortSecurityLocalManagedObject::addStickyMac(PortSecStickyMacsLocalManagedObject *stickyMacMo)
	{
		WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject > stickyMac (stickyMacMo);
		UI32 i, size = m_stickyMacs.size();

		trace(TRACE_LEVEL_INFO, string("The record came for addition MAC: ") + stickyMacMo->m_stickyMac + string(" Vlan: ") + stickyMacMo->m_vlanId);

		for (i = 0; i < size; i++) {
			if ((stickyMac->m_stickyMac.compare(m_stickyMacs[i]->m_stickyMac) == 0) && stickyMac->m_vlanId == m_stickyMacs[i]->m_vlanId) {
				trace(TRACE_LEVEL_INFO, "Condition for existence becomes true");
				return;
            }
		}

		m_stickyMacs.push_back(stickyMac);

		size = m_stickyMacs.size();
		trace(TRACE_LEVEL_INFO, string("the size of vector: ") + size);
		trace(TRACE_LEVEL_INFO, "Following records are there in vector:");
		for (i = 0; i < size; i++)
			trace(TRACE_LEVEL_INFO, string("MAC: ") + m_stickyMacs[i]->m_stickyMac + string("Vlan: ") + m_stickyMacs[i]->m_vlanId);
	}

    void  PortSecurityLocalManagedObject::setOuis(const vector<WaveManagedObjectPointer<PortSecOuiLocalManagedObject> > &ouis)
    {
        m_ouis  =  ouis;
    }

    void PortSecurityLocalManagedObject::addOui (PortSecOuiLocalManagedObject *ouiMO)
    {
        WaveManagedObjectPointer<PortSecOuiLocalManagedObject > oui (ouiMO);
		UI32 i, size = m_ouis.size();

		for (i = 0; i < size; i++) {
			if (oui->m_oui.compare(m_ouis[i]->m_oui) == 0) {
				return;
			}
		}

       	m_ouis.push_back(oui);
		m_numOuis++;
    }

    vector<WaveManagedObjectPointer<PortSecOuiLocalManagedObject> >  PortSecurityLocalManagedObject::getOuis() const
    {
        return m_ouis;
    }

	void PortSecurityLocalManagedObject::deleteAllOuis()
	{
		UI32 i;
		UI32 vecSize = m_ouis.size();

		for (i = 0; i < vecSize; i++) {
			m_ouis.erase(m_ouis.begin() + i);
		}

		m_numOuis = 0;
	}

    void  PortSecurityLocalManagedObject::setAction(const UI32 &action)
    {
        m_newAction  =  action;
    }

    UI32  PortSecurityLocalManagedObject::getAction() const
    {
        return m_newAction;
    }

    void  PortSecurityLocalManagedObject::setSticky(const bool &sticky)
    {
        m_sticky  =  sticky;
    }

    bool  PortSecurityLocalManagedObject::getSticky() const
    {
        return m_sticky;
    }

    void  PortSecurityLocalManagedObject::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  PortSecurityLocalManagedObject::getIfName() const
    {
        return m_ifName;
    }

    void  PortSecurityLocalManagedObject::setIfType(const InterfaceType &ifType)
    {
        m_ifType  =  ifType;
    }

    InterfaceType PortSecurityLocalManagedObject::getIfType() const
    {
        return m_ifType;
    }
}
