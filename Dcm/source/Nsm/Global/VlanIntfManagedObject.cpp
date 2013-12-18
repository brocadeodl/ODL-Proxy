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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/Igmps/IgmpsVlanIntfConfManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsSgVlanConfigManagedObject.h"
#include "Nsm/Global/Igmps/IgmpsMrtrVlanConfigManagedObject.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "vcs.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

    VlanIntfManagedObject::VlanIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (VlanIntfManagedObject::getClassName (), IntfManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        IntfManagedObject (pNsmGlobalObjectManager),
        m_igmpsVlanIntfConfig (WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject> (NULL))
    {
	 m_type = NSM_VLAN_INTF_TYPE_DEFAULT;
	 m_id = 0;
	 m_pVlanType = PVLAN_TYPE_UNKNOWN;
	 m_pVlanSecAssocRange = UI32Range("");
	 m_stpDisable = false;
	 m_remoteSpanEnabled = false;
    }

	VlanIntfManagedObject::VlanIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &id, const  UI32 &type, const string &ifName,const UI64 &ifIndex,const string &desc,const bool &status,const UI32 &mtu)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (VlanIntfManagedObject::getClassName (), IntfManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        IntfManagedObject (pNsmGlobalObjectManager, ifName, ifIndex, desc, status, mtu),
        m_id    (id),
        m_type (type),
        m_igmpsVlanIntfConfig (WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject> (NULL))
    {
		if (id <= MAX_DOT1Q_VLAN_ID) {
			m_stpDisable = false;
		} else {
			m_stpDisable = true;
		}

		m_pVlanType = PVLAN_TYPE_UNKNOWN;
		m_pVlanSecAssocRange = UI32Range("");
		m_remoteSpanEnabled = false;
    }

    VlanIntfManagedObject::VlanIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &id, const WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject> &igmpsVlanIntfConfig,const vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> > &igmpsSgVlanConfig,const vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> > &igmpsMrtrVlanConfig)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (VlanIntfManagedObject::getClassName (), IntfManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        IntfManagedObject (pNsmGlobalObjectManager),
        m_id    (id),
        m_igmpsVlanIntfConfig    (igmpsVlanIntfConfig),
        m_igmpsSgVlanConfig    (igmpsSgVlanConfig),
        m_igmpsMrtrVlanConfig    (igmpsMrtrVlanConfig)
    {
    	if (id <= MAX_DOT1Q_VLAN_ID) {
    		m_stpDisable = false;
    	} else {
    		m_stpDisable = true;
    	}

    	m_type = NSM_VLAN_INTF_TYPE_DEFAULT;
    	m_pVlanType = PVLAN_TYPE_UNKNOWN;
    	m_pVlanSecAssocRange = UI32Range("");
    	m_remoteSpanEnabled = false;
    }

    VlanIntfManagedObject::~VlanIntfManagedObject ()
    {
    }

    string  VlanIntfManagedObject::getClassName()
    {
        return ("VlanIntfManagedObject");
    }

    void  VlanIntfManagedObject::setupAttributesForPersistence()
    {
		UI8 pVlanDefault = 0;
		const UI32Range defaultData("");
        IntfManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_id,"id"));
        addPersistableAttribute (new AttributeUI32(&m_type,"type"));
        setUserTagForAttribute ("id", brocade_interface_name);
        addPersistableAttribute (new AttributeBool(&m_stpDisable,"stpDisable", xstp_stp_shutdown));
        addPersistableAttribute (new AttributeBool(&m_remoteSpanEnabled,"remoteSpanEnabled", span_remote_span));
        addPersistableAttribute (new AttributeManagedObjectComposition<IgmpsVlanIntfConfManagedObject>(&m_igmpsVlanIntfConfig,"igmpsVlanIntfConfig", IgmpsVlanIntfConfManagedObject::getClassName (), true));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<IgmpsSgVlanConfigManagedObject>(&m_igmpsSgVlanConfig,"igmpsSgVlanConfig", getClassName (), getObjectId (), IgmpsSgVlanConfigManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<IgmpsMrtrVlanConfigManagedObject>(&m_igmpsMrtrVlanConfig,"igmpsMrtrVlanConfig", getClassName (), getObjectId (), IgmpsMrtrVlanConfigManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_pVlanType,pVlanDefault,"pvlantype", brocade_interface_pvlan_type_leaf));
			
        addPersistableAttribute (new AttributeUI32Range(&m_pVlanSecAssocRange,defaultData,"pVlanSecAssocRange",brocade_interface_sec_assoc_add));
        addPersistableAttribute (new AttributeUI32Range(&m_pVlanRemSecAssocRange,defaultData,"pVlanRemSecAssocRange", brocade_interface_sec_assoc_remove));
    }

    void  VlanIntfManagedObject::setupAttributesForCreate()
    {
		UI8 pVlanDefault = 0;
		const UI32Range defaultData("");
        addPersistableAttributeForCreate  (new AttributeUI32(&m_id,"id"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_type,"type"));
        addPersistableAttributeForCreate (new AttributeBool(&m_stpDisable,"stpDisable", xstp_stp_shutdown));
        addPersistableAttributeForCreate (new AttributeBool(&m_remoteSpanEnabled,"remoteSpanEnabled", span_remote_span));
        addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<IgmpsVlanIntfConfManagedObject>(&m_igmpsVlanIntfConfig,"igmpsVlanIntfConfig", IgmpsVlanIntfConfManagedObject::getClassName (), true));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<IgmpsSgVlanConfigManagedObject>(&m_igmpsSgVlanConfig,"igmpsSgVlanConfig", getClassName (), getObjectId (), IgmpsSgVlanConfigManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<IgmpsMrtrVlanConfigManagedObject>(&m_igmpsMrtrVlanConfig,"igmpsMrtrVlanConfig", getClassName (), getObjectId (), IgmpsMrtrVlanConfigManagedObject::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_pVlanType,pVlanDefault,"pvlantype", brocade_interface_pvlan_type_leaf));
		addPersistableAttributeForCreate (new AttributeUI32Range(&m_pVlanSecAssocRange,defaultData,"pVlanSecAssocRange", brocade_interface_sec_assoc_add));
		addPersistableAttributeForCreate (new AttributeUI32Range(&m_pVlanRemSecAssocRange,defaultData,"pVlanRemSecAssocRange", brocade_interface_sec_assoc_remove));
        vector<string > keyName;
        keyName.push_back ("id");
        setUserDefinedKeyCombination (keyName);   
    }

    void  VlanIntfManagedObject::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  VlanIntfManagedObject::getId() const
    {
        return (m_id);
    }

    void  VlanIntfManagedObject::setStpDisable(const bool &stpDisable)
    {
        m_stpDisable  =  stpDisable;
    }

    bool  VlanIntfManagedObject::getStpDisable() const
    {
        return (m_stpDisable);
    }

    void  VlanIntfManagedObject::setRemoteSpanEnabled(const bool &remoteSpanEnabled)
    {
    	m_remoteSpanEnabled  =  remoteSpanEnabled;
    }

    bool  VlanIntfManagedObject::getRemoteSpanEnabled() const
    {
        return (m_remoteSpanEnabled);
    }

    void VlanIntfManagedObject::setType (const UI32 &type)
    {
        m_type = type;
    }

    UI32 VlanIntfManagedObject::getType() const
    {
        return m_type;
    }
	void VlanIntfManagedObject::setPVlanType (const pvlanTypeEnum &pVlanType)
    {
        m_pVlanType = pVlanType;
    }

    pvlanTypeEnum VlanIntfManagedObject::getPVlanType() const
    {
        return m_pVlanType;
    }

    void VlanIntfManagedObject::setPvlanSecAssocRange(const UI32Range &pVlanSecAssocRange)
    	{
    	m_pVlanSecAssocRange = pVlanSecAssocRange;
    	}

    UI32Range VlanIntfManagedObject::getPvlanSecAssocRange() const
    	{
    		return (m_pVlanSecAssocRange);
    	}

	void VlanIntfManagedObject::setPvlanRemSecAssocRange(const UI32Range &pVlanRemSecAssocRange)
	{
		m_pVlanRemSecAssocRange = pVlanRemSecAssocRange;
	}

	UI32Range VlanIntfManagedObject::getPvlanRemSecAssocRange() const
	{
		return (m_pVlanRemSecAssocRange);
	}

    void  VlanIntfManagedObject::setIgmpsVlanIntfConfig(const WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject> &igmpsVlanIntfConfig)
    {
        m_igmpsVlanIntfConfig  =  igmpsVlanIntfConfig;
    }

    WaveManagedObjectPointer<IgmpsVlanIntfConfManagedObject>  VlanIntfManagedObject::getIgmpsVlanIntfConfig() const
    {
        return (m_igmpsVlanIntfConfig);
    }

    void  VlanIntfManagedObject::setIgmpsSgVlanConfig(const vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> > &igmpsSgVlanConfig)
    {
        m_igmpsSgVlanConfig  =  igmpsSgVlanConfig;
    }

    vector<WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> >  VlanIntfManagedObject::getIgmpsSgVlanConfig() const
    {
        return (m_igmpsSgVlanConfig);
    }

    void  VlanIntfManagedObject::setIgmpsMrtrVlanConfig(const vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> > &igmpsMrtrVlanConfig)
    {
        m_igmpsMrtrVlanConfig  =  igmpsMrtrVlanConfig;
    }

    vector<WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> >  VlanIntfManagedObject::getIgmpsMrtrVlanConfig() const
    {
        return (m_igmpsMrtrVlanConfig);
    }

    void VlanIntfManagedObject::addIgmpsSgVlanConfigToVector(IgmpsSgVlanConfigManagedObject *igmpsSgMO)
    {
        WaveManagedObjectPointer<IgmpsSgVlanConfigManagedObject> igmpsSg (igmpsSgMO);
        m_igmpsSgVlanConfig.push_back(igmpsSg);
    }

    void VlanIntfManagedObject::delIgmpsSgVlanConfigFromVector(IpV4Address groupAddress, UI32 ifType, string ifName)
    {
        trace (TRACE_LEVEL_DEBUG, string ("Entering VlanIntfManagedObject::delIgmpsSgVlanConfigFromVector"));
        for (UI32 i = 0; i < m_igmpsSgVlanConfig.size(); i++) {
            if (((m_igmpsSgVlanConfig[i])->getGroupAddress() == groupAddress)
                && ((m_igmpsSgVlanConfig[i])->getIfType() == ifType)
                && ((m_igmpsSgVlanConfig[i])->getIfName() == ifName)) {
                trace (TRACE_LEVEL_DEBUG, string ("VlanIntfManagedObject::delIgmpsSgVlanConfigFromVector:deleting ")
                    + groupAddress.toString() + string (" on interface ") + ifName + string (" at index ") + i);
                m_igmpsSgVlanConfig.erase (m_igmpsSgVlanConfig.begin() + i);
                break;
            }
        }
        return;
    }

    void VlanIntfManagedObject::addIgmpsMrtrVlanConfigToVector(IgmpsMrtrVlanConfigManagedObject *igmpsMrtrMO)
    {
        WaveManagedObjectPointer<IgmpsMrtrVlanConfigManagedObject> igmpsMrtr (igmpsMrtrMO);
        m_igmpsMrtrVlanConfig.push_back(igmpsMrtr);
    }

    void VlanIntfManagedObject::delIgmpsMrtrVlanConfigFromVector(UI32 ifType, string ifName)
    {
        trace (TRACE_LEVEL_DEBUG, string ("Entering VlanIntfManagedObject::delIgmpsMrtrVlanConfigFromVector"));
        for (UI32 i = 0; i < m_igmpsMrtrVlanConfig.size(); i++) {
            if (((m_igmpsMrtrVlanConfig[i])->getIfType() == ifType)
                && ((m_igmpsMrtrVlanConfig[i])->getIfName() == ifName)) {
                trace (TRACE_LEVEL_DEBUG, string ("VlanIntfManagedObject::delIgmpsMrtrVlanConfigFromVector:deleting ") + ifName + string (" at index ") + i);
                m_igmpsMrtrVlanConfig.erase (m_igmpsMrtrVlanConfig.begin() + i);
                break;
            }
        }
        return;
    }

    bool VlanIntfManagedObject::getPluginDetailsForDistribution (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag)
    {
        prismServiceId = FrameworkToolKit::getServiceIdByName (NsmLocalObjectManager::getClassName ());

        return (true);
    }

    bool VlanIntfManagedObject::programHardwareForInputWaveManagedObject (vector<string> &attributeNames, vector<string> &attributeValues, string &clientName)
    {

        trace (TRACE_LEVEL_INFO, "VlanIntfManagedObject::programHardwareForInputWaveManagedObject : Filled client name and returned true");
        clientName = "nsm";

        return (true);
    }
    
}
