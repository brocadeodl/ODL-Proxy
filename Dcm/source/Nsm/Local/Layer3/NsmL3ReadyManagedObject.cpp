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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : justinh                                                      *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "vcs.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"

#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrConfigManagedObject.h"


namespace DcmNs
{
    class NsmIpAddrConfigManagedObject;

    NsmL3ReadyManagedObject::NsmL3ReadyManagedObject(
     WaveObjectManager *pNsmLocalObjectManager)
     : PrismElement(pNsmLocalObjectManager),
        PrismPersistableObject(NsmL3ReadyManagedObject::getClassName(), 
            DcmLocalManagedObject::getClassName()),
        WaveManagedObject(pNsmLocalObjectManager),
        DcmManagedObject(pNsmLocalObjectManager),
        WaveLocalManagedObjectBase(this),
        DcmLocalManagedObjectBase(this),
        DcmLocalManagedObject(pNsmLocalObjectManager)
    {
        m_ipConfig = NULL;
        m_ipAddrCfg = NULL;
        m_OSPFIntfConfig = NULL;
        m_pimsmCfg = NULL;
        m_vrrpIntfConfig.clear();
        m_vrrpeIntfConfig.clear();
        m_vrrpVcsIntfConfig.clear();
        m_igmpIntfConf = NULL;
        m_igmpSgIntfConf.clear();
        m_dhcpRelIntfConf.clear();
        m_intf_type = IF_TYPE_INVALID;
    }

    NsmL3ReadyManagedObject::~NsmL3ReadyManagedObject()
    {
    }

    string  NsmL3ReadyManagedObject::getClassName()
    {
        return ("NsmL3ReadyManagedObject");
    }

    void  NsmL3ReadyManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence();

        addPersistableAttribute(
            new AttributeManagedObjectComposition<NsmIpConfigManagedObject>(
            &m_ipConfig, "ipConfig", NsmIpConfigManagedObject::getClassName(), 
            true));

        addPersistableAttribute(
            new AttributeManagedObjectComposition<NsmIpAddrConfigManagedObject>(
            &m_ipAddrCfg, "ipAddrCfg", NsmIpAddrConfigManagedObject::getClassName(), 
            true));

        addPersistableAttribute(
            new AttributeManagedObjectComposition<OSPFInterfaceLocalManagedObject>(
            &m_OSPFIntfConfig, "OSPFIntfConfig", OSPFInterfaceLocalManagedObject::getClassName(),
            true));
        addPersistableAttribute(
            new AttributeManagedObjectComposition<PimIntfCfgLocalManagedObject>(
            &m_pimsmCfg, "pimsmCfg", PimIntfCfgLocalManagedObject::getClassName(), 
            true));
        addPersistableAttribute (
            new AttributeManagedObjectVectorComposition<VRRPSessionConfigLocalManagedObject>(
            &m_vrrpIntfConfig,"vrrpIntfConfig", getClassName(), getObjectId(), VRRPSessionConfigLocalManagedObject::getClassName ()));
        addPersistableAttribute (
           new AttributeManagedObjectVectorComposition<VRRPESessionConfigLocalManagedObject>(
           &m_vrrpeIntfConfig,"vrrpeIntfConfig", getClassName(), getObjectId(), VRRPESessionConfigLocalManagedObject::getClassName ()));
        addPersistableAttribute (
           new AttributeManagedObjectVectorComposition<VRRPVcsSessionConfigLocalManagedObject>(
           &m_vrrpVcsIntfConfig,"vrrpVcsIntfConfig", getClassName(), getObjectId(), VRRPVcsSessionConfigLocalManagedObject::getClassName ()));
        addPersistableAttribute(
            new AttributeManagedObjectComposition<IgmpIntfConfManagedObject>(
            &m_igmpIntfConf, "igmpIntfConf", IgmpIntfConfManagedObject::getClassName(), 
            true));
        addPersistableAttribute(
            new AttributeManagedObjectVectorComposition<IgmpSgIntfConfManagedObject>(
            &m_igmpSgIntfConf, "igmpSgIntfConf", getClassName(), getObjectId(), IgmpSgIntfConfManagedObject::getClassName()));
        addPersistableAttribute(
            new AttributeManagedObjectVectorComposition<DhcpRelInterfaceLocalManagedObject>(
            &m_dhcpRelIntfConf, "dhcpRelIntfConf", getClassName(), getObjectId(), DhcpRelInterfaceLocalManagedObject::getClassName()));
        addPersistableAttribute (new AttributeString(&m_interfaceName,"interfaceName"));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_intf_type,"interfaceType"));
    }

    void  NsmL3ReadyManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();

        addPersistableAttributeForCreate(
            new AttributeManagedObjectComposition<NsmIpConfigManagedObject>(
            &m_ipConfig, "ipConfig", NsmIpConfigManagedObject::getClassName(), 
            true));

        addPersistableAttributeForCreate(
            new AttributeManagedObjectComposition<NsmIpAddrConfigManagedObject>(
            &m_ipAddrCfg, "ipAddrCfg", NsmIpAddrConfigManagedObject::getClassName(), 
            true));

        addPersistableAttributeForCreate(
            new AttributeManagedObjectComposition<OSPFInterfaceLocalManagedObject>(
            &m_OSPFIntfConfig, "OSPFIntfConfig", OSPFInterfaceLocalManagedObject::getClassName(),
            true));
        addPersistableAttributeForCreate(
            new AttributeManagedObjectComposition<PimIntfCfgLocalManagedObject>(
            &m_pimsmCfg, "pimsmCfg", PimIntfCfgLocalManagedObject::getClassName(), 
            true));
        addPersistableAttributeForCreate  (
            new AttributeManagedObjectVectorComposition<VRRPSessionConfigLocalManagedObject>(
            &m_vrrpIntfConfig,"vrrpIntfConfig", getClassName(), getObjectId(), VRRPSessionConfigLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (
            new AttributeManagedObjectVectorComposition<VRRPESessionConfigLocalManagedObject>(
            &m_vrrpeIntfConfig,"vrrpeIntfConfig", getClassName(), getObjectId(), VRRPESessionConfigLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (
            new AttributeManagedObjectVectorComposition<VRRPVcsSessionConfigLocalManagedObject>(
            &m_vrrpVcsIntfConfig,"vrrpVcsIntfConfig", getClassName(), getObjectId(), VRRPVcsSessionConfigLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate(
            new AttributeManagedObjectComposition<IgmpIntfConfManagedObject>(
            &m_igmpIntfConf, "igmpIntfConf", IgmpIntfConfManagedObject::getClassName(), 
            true));
        addPersistableAttributeForCreate(
            new AttributeManagedObjectVectorComposition<IgmpSgIntfConfManagedObject>(
            &m_igmpSgIntfConf, "igmpSgIntfConf", getClassName(), getObjectId(), IgmpSgIntfConfManagedObject::getClassName())); 
        addPersistableAttributeForCreate(
            new AttributeManagedObjectVectorComposition<DhcpRelInterfaceLocalManagedObject>(
            &m_dhcpRelIntfConf, "dhcpRelIntfConf", getClassName(), getObjectId(), DhcpRelInterfaceLocalManagedObject::getClassName())); 
        addPersistableAttributeForCreate(new AttributeString(&m_interfaceName,"interfaceName"));
        addPersistableAttributeForCreate(new AttributeEnum((UI32*)&m_intf_type,"interfaceType"));
    }

    void  NsmL3ReadyManagedObject::setIpConfig(
        const WaveManagedObjectPointer<NsmIpConfigManagedObject> &ipCfg)
    {
        m_ipConfig = ipCfg;
    }

    WaveManagedObjectPointer<NsmIpConfigManagedObject> 
        NsmL3ReadyManagedObject::getIpConfig() const
    {
        return (m_ipConfig);
    }
    
    void  NsmL3ReadyManagedObject::setIpAddrConfig(
        const WaveManagedObjectPointer<NsmIpAddrConfigManagedObject> &ipAddrCfg)
    {
        m_ipAddrCfg = ipAddrCfg;
    }

    WaveManagedObjectPointer<NsmIpAddrConfigManagedObject> 
        NsmL3ReadyManagedObject::getIpAddrConfig() const
    {
        return (m_ipAddrCfg);
    }
    
    /* Accessor and mutator functions for PIM configs */
    void NsmL3ReadyManagedObject::setPimSmCfg(
        const WaveManagedObjectPointer<PimIntfCfgLocalManagedObject> &pimsmCfg)
    {
        m_pimsmCfg = pimsmCfg;
    }
    
    WaveManagedObjectPointer<PimIntfCfgLocalManagedObject> 
        NsmL3ReadyManagedObject::getPimSmCfg() const
    {
        return (m_pimsmCfg);
    }

    void  NsmL3ReadyManagedObject::setOSPFIntfConfig(
        const WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject> &OSPFIntfConfig)
    {
        m_OSPFIntfConfig = OSPFIntfConfig;
    }

    WaveManagedObjectPointer<OSPFInterfaceLocalManagedObject> 
        NsmL3ReadyManagedObject::getOSPFIntfConfig() const
    {
        return (m_OSPFIntfConfig);
    }

    void  NsmL3ReadyManagedObject::setVRRPIntfConfig(
        const vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > &vrrpIntfConfig)
    {   
        m_vrrpIntfConfig = vrrpIntfConfig;
    }

    vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> >
        NsmL3ReadyManagedObject::getVRRPIntfConfig() const
    {   
        return (m_vrrpIntfConfig);
    }

	void NsmL3ReadyManagedObject::addVRRPIntfConfig(VRRPSessionConfigLocalManagedObject *pVRRPSessionConfig)
	{
		WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> VRRPSession (pVRRPSessionConfig);
		m_vrrpIntfConfig.push_back(VRRPSession);
	}

	void NsmL3ReadyManagedObject::deleteVRRPIntfConfig(UI8 vrid)

	{
		for (UI32 i= 0; i < m_vrrpIntfConfig.size(); i++)
		{
			if ((m_vrrpIntfConfig[i])->getVrid() == vrid)
			{
				m_vrrpIntfConfig.erase(m_vrrpIntfConfig.begin() + i);
			}
		}
	}

	void NsmL3ReadyManagedObject::deleteallVRRPIntfConfig()
	{
		UI32 size = m_vrrpIntfConfig.size();
		for (UI32 i= 0; i < size; i++)
		{
			m_vrrpIntfConfig.erase(m_vrrpIntfConfig.begin() + i);
		}
	}

	WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> NsmL3ReadyManagedObject::getVRRPIntfSession(UI8 vrid)
	{
		for (UI32 i = 0; i < m_vrrpIntfConfig.size(); i++)
		{
			if ((m_vrrpIntfConfig[i])->getVrid() == vrid)
			{
				return m_vrrpIntfConfig[i];
			}
		}
		return NULL;
	}

    void  NsmL3ReadyManagedObject::setVRRPEIntfConfig(
        const vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> > &vrrpeIntfConfig)
    {   
        m_vrrpeIntfConfig = vrrpeIntfConfig;
    }

    vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> >
        NsmL3ReadyManagedObject::getVRRPEIntfConfig() const
    {   
        return (m_vrrpeIntfConfig);
    }

	void NsmL3ReadyManagedObject::addVRRPEIntfConfig(VRRPESessionConfigLocalManagedObject *pVRRPESessionConfig)
	{
		WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> VRRPESession (pVRRPESessionConfig);
		m_vrrpeIntfConfig.push_back(VRRPESession);
	}

	void NsmL3ReadyManagedObject::deleteVRRPEIntfConfig(UI8 vrid)
	{
		for (UI32 i = 0; i < m_vrrpeIntfConfig.size(); i++)
		{
			if ((m_vrrpeIntfConfig[i])->getVrid() == vrid)
			{
				m_vrrpeIntfConfig.erase(m_vrrpeIntfConfig.begin() + i);
			}
		}
	}

	void NsmL3ReadyManagedObject::deleteallVRRPEIntfConfig()
	{
		UI32 size = m_vrrpeIntfConfig.size();
		for (UI32 i= 0; i < size; i++)
		{
			m_vrrpeIntfConfig.erase(m_vrrpeIntfConfig.begin() + i);
		}
	}

	WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> NsmL3ReadyManagedObject::getVRRPEIntfSession(UI8 vrid)
	{
		for (UI32 i = 0; i < m_vrrpeIntfConfig.size(); i++)
		{
			if ((m_vrrpeIntfConfig[i])->getVrid() == vrid)
			{
				return m_vrrpeIntfConfig[i];
			}
		}
		return NULL;
	}


    void  NsmL3ReadyManagedObject::setVRRPVcsIntfConfig(
        const vector<WaveManagedObjectPointer<VRRPVcsSessionConfigLocalManagedObject> > &vrrpVcsIntfConfig)
    {
        m_vrrpVcsIntfConfig = vrrpVcsIntfConfig;
    }

    vector<WaveManagedObjectPointer<VRRPVcsSessionConfigLocalManagedObject> >
        NsmL3ReadyManagedObject::getVRRPVcsIntfConfig() const
    {
        return (m_vrrpVcsIntfConfig);
    }

	void NsmL3ReadyManagedObject::addVRRPVcsIntfConfig(VRRPVcsSessionConfigLocalManagedObject *pVRRPVcsSessionConfig)
	{
		WaveManagedObjectPointer<VRRPVcsSessionConfigLocalManagedObject> VRRPVcsSession (pVRRPVcsSessionConfig);
		m_vrrpVcsIntfConfig.push_back(VRRPVcsSession);
	}

	void NsmL3ReadyManagedObject::deleteVRRPVcsIntfConfig(UI8 vrid)
	{
		for (UI32 i = 0; i < m_vrrpVcsIntfConfig.size(); i++)
		{
			if ((m_vrrpVcsIntfConfig[i])->getVrid() == vrid)
			{
				m_vrrpVcsIntfConfig.erase(m_vrrpVcsIntfConfig.begin() + i);
			}
		}
	}

	WaveManagedObjectPointer<VRRPVcsSessionConfigLocalManagedObject> NsmL3ReadyManagedObject::getVRRPVcsIntfSession(UI8 vrid)
	{
		for (UI32 i = 0 ; i  < m_vrrpVcsIntfConfig.size(); i++)
		{
			if ((m_vrrpVcsIntfConfig[i])->getVrid() == vrid)
			{
				return m_vrrpVcsIntfConfig[i];
			}
		}
		return NULL;
	}


    void NsmL3ReadyManagedObject::setIgmpIntfConf(
        const WaveManagedObjectPointer<IgmpIntfConfManagedObject> &igmpIntfConf)
    {
        m_igmpIntfConf = igmpIntfConf;
    }
    
    IgmpIntfConfManagedObject* 
        NsmL3ReadyManagedObject::getIgmpIntfConf() 
    {
        return (m_igmpIntfConf.operator->());
    }

    void  NsmL3ReadyManagedObject::setIgmpSgIntfConf(const vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> > &igmpSgIntfConf)
    {
        m_igmpSgIntfConf  =  igmpSgIntfConf;
    }

    vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> >  NsmL3ReadyManagedObject::getIgmpSgIntfConf() const
    {
        return (m_igmpSgIntfConf);
    }

    void NsmL3ReadyManagedObject::addIgmpSgIntfConfToVector(IgmpSgIntfConfManagedObject *igmpSgMO)
    {
        WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> igmpSg (igmpSgMO);
        m_igmpSgIntfConf.push_back(igmpSg);
    }

    void  NsmL3ReadyManagedObject::setDhcpRelIntfConf(const vector<WaveManagedObjectPointer<DhcpRelInterfaceLocalManagedObject> > &dhcpRelIntfConf)
    {
        m_dhcpRelIntfConf  =  dhcpRelIntfConf;
    }

    vector<WaveManagedObjectPointer<DhcpRelInterfaceLocalManagedObject> >  NsmL3ReadyManagedObject::getDhcpRelIntfConf() const
    {
        return (m_dhcpRelIntfConf);
    }

    void NsmL3ReadyManagedObject::addDhcpRelIntfConfToVector(DhcpRelInterfaceLocalManagedObject *dhcpRelMO)
    {
        WaveManagedObjectPointer<DhcpRelInterfaceLocalManagedObject> dhcpRel (dhcpRelMO);
        m_dhcpRelIntfConf.push_back(dhcpRel);
    }

    void NsmL3ReadyManagedObject::delDhcpRelIntfConfFromVector(IpV4Address serverAddress, 
                                                              UI32 ifType, string ifName, UI32 vlanId)
    {
        trace(TRACE_LEVEL_DEBUG, 
              string ("Entering NsmL3ReadyManagedObject::delDhcpRelIntfConfFromVector"));
		
        for (UI32 i = 0; i < m_dhcpRelIntfConf.size(); i++) {
			if ((ifType == IF_TYPE_GI) || (ifType == IF_TYPE_TE) || (ifType == IF_TYPE_FO))
			{
            	if ( ((m_dhcpRelIntfConf[i])->getServerAddress() == serverAddress)
                	&& ((m_dhcpRelIntfConf[i])->getIfType() == ifType)
                	&& ((m_dhcpRelIntfConf[i])->getIfName() == ifName)) {
                	trace (TRACE_LEVEL_DEBUG, string ("NsmL3ReadyManagedObject::delDhcpRelIntfConfFromVector:deleting ")
                    	+ serverAddress.toString() + string (" on interface ") + ifName + string (" at index ") + i);
                	m_dhcpRelIntfConf.erase (m_dhcpRelIntfConf.begin() + i);
                	break;
            	}
			}
			else if (ifType == IF_TYPE_VE)
			{
            	if ( ((m_dhcpRelIntfConf[i])->getServerAddress() == serverAddress)
                	&& ((m_dhcpRelIntfConf[i])->getIfType() == ifType)
                	&& ((m_dhcpRelIntfConf[i])->getVlanId() == vlanId)) {
                	trace (TRACE_LEVEL_DEBUG, string ("NsmL3ReadyManagedObject::delDhcpRelIntfConfFromVector:deleting ")
                    	+ serverAddress.toString() + string (" on interface ") + vlanId + string (" at index ") + i);
                	m_dhcpRelIntfConf.erase (m_dhcpRelIntfConf.begin() + i);
                	break;
            	}
			}
        } /* end of for-loop */
        return;
    }

    void NsmL3ReadyManagedObject::setInterfaceName(string ifName) 
    {
        m_interfaceName = ifName;
    }

    string NsmL3ReadyManagedObject::getInterfaceName (void) const
    {
        return m_interfaceName;
    }

    void NsmL3ReadyManagedObject::setIntfType(InterfaceType type)
    {
        m_intf_type = type;
    }

    InterfaceType NsmL3ReadyManagedObject::getIntfType() const 
    {
        return m_intf_type;
    }    




    void NsmL3ReadyManagedObject::delIgmpSgIntfConfFromVector(IpV4Address groupAddress, 
                                                              UI32 ifType, string ifName)
    {
        trace(TRACE_LEVEL_INFO, 
              string ("Entering NsmL3ReadyManagedObject::delIgmpSgIntfConfigFromVector"));
        for (UI32 i = 0; i < m_igmpSgIntfConf.size(); i++) {
            if (((m_igmpSgIntfConf[i])->getGroupAddress() == groupAddress)
                && ((m_igmpSgIntfConf[i])->getIfType() == ifType)
                && ((m_igmpSgIntfConf[i])->getIfName() == ifName)) {
                trace (TRACE_LEVEL_INFO, string ("NsmL3ReadyManagedObject::delIgmpSgIntfConfigFromVector:deleting ")
                    + groupAddress.toString() + string (" on interface ") + ifName + string (" at index ") + i);
                m_igmpSgIntfConf.erase (m_igmpSgIntfConf.begin() + i);
                break;
            }
        }
        return;
    }

    bool NsmL3ReadyManagedObject::getCanDeleteL3ReadyMO(void)
    {
        if( (m_OSPFIntfConfig.operator-> ()) || (m_pimsmCfg.operator-> ()) || 
            (m_vrrpIntfConfig.size() != 0) || (m_vrrpeIntfConfig.size() != 0) || (m_vrrpVcsIntfConfig.size() != 0) ||
            ((m_igmpIntfConf.operator-> ()) && m_igmpIntfConf->hasNonDefaultConfig() == true) || (m_igmpSgIntfConf.size() != 0) ||
			(m_dhcpRelIntfConf.size() != 0)|| ((m_ipAddrCfg.operator ->())))
        {
            trace(TRACE_LEVEL_INFO, string("NsmL3ReadyManagedObject::getCanDeleteL3ReadyMO: CANNOT DELETE"));
            return (false);
        }
        else
        {
            trace(TRACE_LEVEL_INFO, string("NsmL3ReadyManagedObject::getCanDeleteL3ReadyMO: DELETE!!"));
            return (true);
        }
    }

    bool NsmL3ReadyManagedObject::isL3ConfigPresent()
    {
        if( (m_OSPFIntfConfig.operator-> ()) || (m_pimsmCfg.operator-> ()) || (m_ipAddrCfg.operator-> ()) || 
            (m_vrrpIntfConfig.size() != 0) || (m_vrrpeIntfConfig.size() != 0) || (m_vrrpVcsIntfConfig.size() != 0) ||
            ((m_igmpIntfConf.operator-> ()) && m_igmpIntfConf->hasNonDefaultConfig() == true) || (m_igmpSgIntfConf.size() != 0) ||
            (m_dhcpRelIntfConf.size() != 0) || ((m_ipConfig.operator->()) &&(m_ipConfig->hasNonDefaultConfig())))
        {
            trace(TRACE_LEVEL_INFO, string("NsmL3ReadyManagedObject::isL3ConfigPresent- true"));
            return (true);
        }
        else
        {
            trace(TRACE_LEVEL_INFO, string("NsmL3ReadyManagedObject::isL3ConfigPresent:false !!"));
            return (false);
        }
    }


} // DcmNs
