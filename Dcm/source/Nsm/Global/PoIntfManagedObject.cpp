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
 *   Author : skbalasu.                                                     *
 **************************************************************************/

#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Local/NativeVlanIntfConfLocalManagedObject.h"
#include "vcs.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"

#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"

#include <algorithm>

#include "Nsm/Global/FcoeWorker/FcoeportAttrGlobalManagedObject.h"
namespace DcmNs
{

    PoIntfManagedObject::PoIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PoIntfManagedObject::getClassName (), PoPhyIntfManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        IntfManagedObject (pNsmGlobalObjectManager),
        PoPhyIntfManagedObject (pNsmGlobalObjectManager)
    {
		m_id = 0;
		m_poLacpSystemIdDomainId = 0;
		m_poMinLinks = 1;
		m_poAllowedSpeed = (LineSpeed)2;
		m_poMode = 0;
		m_poLoadBalanceMode = 7;
		m_poPartnerKey = 0;
		m_poPartnerSystemPriority = 0;
		m_poPrimaryLinkDomainId = 0;
		m_poType = 0;
		m_eld_prio_val = 128;
    }

    PoIntfManagedObject::PoIntfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &id,const UI32 &poMode,const UI32 &poType,const UI32 &poMinLinks,const LineSpeed &poAllowedSpeed, const UI32 &poLoadBalanceMode, const vector<ObjectId> &portMembers)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PoIntfManagedObject::getClassName (), PoPhyIntfManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        IntfManagedObject (pNsmGlobalObjectManager),
        PoPhyIntfManagedObject (pNsmGlobalObjectManager),
        m_id    (id),
        m_poMode    (poMode),
        m_poType    (poType),
        m_poMinLinks    (poMinLinks),
		m_poAllowedSpeed   (poAllowedSpeed),
        m_poLoadBalanceMode    (poLoadBalanceMode),
        m_portMembers    (portMembers)
    {
		m_poLacpSystemIdDomainId = 0;
		m_poPartnerKey = 0;
		m_poPartnerSystemPriority = 0;
		m_poPrimaryLinkDomainId = 0;
		m_eld_prio_val = 128;
    }

    PoIntfManagedObject::~PoIntfManagedObject ()
    {
    }

    string  PoIntfManagedObject::getClassName()
    {
        return ("PoIntfManagedObject");
    }

    void  PoIntfManagedObject::setupAttributesForPersistence()
    {
        PoPhyIntfManagedObject::setupAttributesForPersistence ();
		UI32 eld_prio_default = 128;
        addPersistableAttribute (new AttributeUI32(&m_id,"id"));
        addPersistableAttribute (new AttributeUI32(&m_poMode,"poMode"));
        addPersistableAttribute (new AttributeUI32(&m_poType,"poType"));
        addPersistableAttribute (new AttributeUI32(&m_poMinLinks,"poMinLinks",brocade_interface_minimum_links));
		addPersistableAttribute (new AttributeEnum((UI32*)&m_poAllowedSpeed,"poAllowedSpeed",brocade_interface_po_speed));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_poLoadBalanceMode,"poLoadBalanceMode",brocade_interface_load_balance));
        addPersistableAttribute (new AttributeUI32(&m_poPrimaryLinkDomainId,"poPrimaryLinkDomainId"));
        addPersistableAttribute (new AttributeUI32(&m_poPartnerSystemPriority,"poPartnerSystemPriority"));
        addPersistableAttribute (new AttributeUI32(&m_poPartnerKey,"poPartnerKey"));
        addPersistableAttribute (new AttributeString(&m_poPartnerSystem,"poPartnerSystem"));
        addPersistableAttribute (new AttributeString(&m_poLacpSystemId,"poLacpSystemId"));
        addPersistableAttribute (new AttributeUI32(&m_poLacpSystemIdDomainId,"poLacpSystemIdDomainId"));
        addPersistableAttribute (new AttributeObjectIdVectorAssociation(&m_portMembers,"portMembers", getClassName (), getObjectId (), PhyIntfLocalManagedObject::getClassName ()));
	addPersistableAttribute (new AttributeManagedObjectVectorComposition<VlanClassifierActivateGlobalManagedObject>(&m_vlanClassifierActivate,"vlanClassifierActivate", getClassName (), getObjectId (), VlanClassifierActivateGlobalManagedObject::getClassName ()));
	addPersistableAttribute (new AttributeManagedObjectComposition<NativeVlanIntfConfLocalManagedObject>(&m_nativeVlanIntfConfig,"nativeVlanIntfConfig", NativeVlanIntfConfLocalManagedObject::getClassName (), true));
        addPersistableAttribute (new AttributeManagedObjectComposition<NsmNodePoManagedObject>(&m_nodePo,"nodePo", NsmNodePoManagedObject::getClassName (), true));

        setUserTagForAttribute ("id", brocade_interface_name);
		/* ELD */
		addPersistableAttribute (new AttributeUI32((UI32 * )(&m_eld_prio_val), eld_prio_default, "eldprio",brocade_interface_eldprio));
		addPersistableAttribute (new AttributeObjectIdVectorAssociation(&m_vlanid,"vlanid", getClassName (), getObjectId (), VlanIntfManagedObject::getClassName (), 0, false, true));
		setUserTagForAttribute("vlanid", brocade_interface_eldvlan);
		/* ELD */
        addPersistableAttribute (new AttributeManagedObjectComposition<FcoeportAttrGlobalManagedObject>(&m_fcoeport, "fcoeport", FcoeportAttrGlobalManagedObject::getClassName(), true));
    }

    void  PoIntfManagedObject::setupAttributesForCreate()
    {
		UI32 eld_prio_default = 128;
        addPersistableAttributeForCreate  (new AttributeUI32(&m_id,"id"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_poMode,"poMode"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_poType,"poType"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_poMinLinks,"poMinLinks",brocade_interface_minimum_links));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_poAllowedSpeed,"poAllowedSpeed",brocade_interface_po_speed));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_poLoadBalanceMode,"poLoadBalanceMode",brocade_interface_load_balance));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_poPrimaryLinkDomainId,"poPrimaryLinkDomainId"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_poPartnerSystemPriority,"poPartnerSystemPriority"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_poPartnerKey,"poPartnerKey"));
        addPersistableAttributeForCreate  (new AttributeString(&m_poPartnerSystem,"poPartnerSystem"));
        addPersistableAttributeForCreate  (new AttributeString(&m_poLacpSystemId,"poLacpSystemId"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_poLacpSystemIdDomainId,"poLacpSystemIdDomainId"));
        addPersistableAttributeForCreate  (new AttributeObjectIdVectorAssociation(&m_portMembers,"portMembers", getClassName (), getObjectId (), PhyIntfLocalManagedObject::getClassName ()));
	addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<VlanClassifierActivateGlobalManagedObject>(&m_vlanClassifierActivate,"vlanClassifierActivate", getClassName (), getObjectId (), VlanClassifierActivateGlobalManagedObject::getClassName ()));
	addPersistableAttributeForCreate  (new AttributeManagedObjectComposition<NativeVlanIntfConfLocalManagedObject>(&m_nativeVlanIntfConfig,"nativeVlanIntfConfig", NativeVlanIntfConfLocalManagedObject::getClassName (), true));
        addPersistableAttributeForCreate (new AttributeManagedObjectComposition<NsmNodePoManagedObject>(&m_nodePo,"nodePo", NsmNodePoManagedObject::getClassName (), true));
    /* ELD */
		addPersistableAttributeForCreate (new AttributeUI32((UI32 *) (&m_eld_prio_val), eld_prio_default, "eldprio",brocade_interface_eldprio));
		addPersistableAttributeForCreate  (new AttributeObjectIdVectorAssociation(&m_vlanid,"vlanid", getClassName (), getObjectId (), VlanIntfManagedObject::getClassName (), 0, false, true));
    /* ELD */
        addPersistableAttributeForCreate (new AttributeManagedObjectComposition<FcoeportAttrGlobalManagedObject>(&m_fcoeport, "fcoeport", FcoeportAttrGlobalManagedObject::getClassName(), true));
        vector<string > keyName;
        keyName.push_back ("id");
        setUserDefinedKeyCombination (keyName);
    }

	/* ELD */
	static bool profilePOVlanSort(ObjectId obj1, ObjectId obj2)
	{
		return obj1 < obj2;
	}

    ResourceId PoIntfManagedObject::addVlanObjectId(ObjectId &vlanOid)
    {

        vector<ObjectId>::iterator vlanid;

        if (find(m_vlanid.begin(), m_vlanid.end(), vlanOid) != m_vlanid.end() ) {
            trace (TRACE_LEVEL_ERROR, "InstanceBaseManagedObject::addVlanObjectId: Vlan is already member of this instance  \n");
            return WAVE_MESSAGE_ERROR;
        }
            trace (TRACE_LEVEL_INFO, "InstanceBaseManagedObject::addVlanObjectId\n");

        m_vlanid.push_back(vlanOid);
        stable_sort(m_vlanid.begin(), m_vlanid.end(), profilePOVlanSort);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId PoIntfManagedObject::deleteVlanObjectId(ObjectId &vlanOid)
    {
        vector<ObjectId>::iterator result;
        if (vlanOid == ObjectId()) {
            m_vlanid.clear();
            trace (TRACE_LEVEL_DEBUG, "InstanceBaseManagedObject::deleteVlanObjectId: vlanOid == ObjectId() \n");
            return WAVE_MESSAGE_SUCCESS;
        }
        result = find(m_vlanid.begin(), m_vlanid.end(), vlanOid);
        if (result == m_vlanid.end() ) {
            trace (TRACE_LEVEL_ERROR, "InstanceBaseManagedObject::deleteVlanObjectId: Vlan is not present for this instance \n");
            return WAVE_MESSAGE_ERROR;
        }

		else {
            m_vlanid.erase(result);
        }

        return WAVE_MESSAGE_SUCCESS;
    }
    void  PoIntfManagedObject::setEldPrio(const UI32 &eldprio)
    {
        m_eld_prio_val = eldprio;
    }

    UI32  PoIntfManagedObject::getEldPrio() const
    { 
        return (m_eld_prio_val);
    }

    void  PoIntfManagedObject::setVlanidList(const vector<UI32> &vlanidList)
    {
        unsigned int i = 0;
        m_vlanidList  =  vlanidList;
        for (i = 0; i < m_vlanidList.size(); i++)
        {
            trace (TRACE_LEVEL_INFO, string ("\t setVlanidList(): i = ") + i + string("VLAN_ID = ") + m_vlanidList[i]);
            printf("setVlanidList():i = %d, VLAN_ID IS %d\n", i, m_vlanidList[i]);
        }
    }

    vector<UI32>  PoIntfManagedObject::getVlanidList() const
    {
        return (m_vlanidList);
    }

    vector<ObjectId>  PoIntfManagedObject::getVlanVector() const
    {
        return (m_vlanid);
    }

	/* ELD */

    void  PoIntfManagedObject::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  PoIntfManagedObject::getId() const
    {
        return (m_id);
    }

    void  PoIntfManagedObject::setPoMode(const UI32 &poMode)
    {
        m_poMode  =  poMode;
    }

    UI32  PoIntfManagedObject::getPoMode() const
    {
        return (m_poMode);
    }

    void  PoIntfManagedObject::setPoType(const UI32 &poType)
    {
        m_poType  =  poType;
    }

    UI32  PoIntfManagedObject::getPoType() const
    {
        return (m_poType);
    }

    void  PoIntfManagedObject::setPoMinLinks(const UI32 &poMinLinks)
    {
        m_poMinLinks  =  poMinLinks;
    }

    UI32  PoIntfManagedObject::getPoMinLinks() const
    {
        return (m_poMinLinks);
    }
	void  PoIntfManagedObject::setPoAllowedSpeed(const LineSpeed &poAllowedSpeed)
	{
		m_poAllowedSpeed  =  poAllowedSpeed;
	}

	LineSpeed  PoIntfManagedObject::getPoAllowedSpeed() const
	{
		return (m_poAllowedSpeed);
	}
	void  PoIntfManagedObject::setPoLoadBalanceMode(const UI32 &poLoadBalanceMode)
    {
        m_poLoadBalanceMode  =  poLoadBalanceMode;
    }

    UI32  PoIntfManagedObject::getPoLoadBalanceMode() const
    {
        return (m_poLoadBalanceMode);
    }
    void  PoIntfManagedObject::setPoPrimaryLinkDomainId(const UI32 &poPrimaryLinkDomainId)
    {
        m_poPrimaryLinkDomainId  =  poPrimaryLinkDomainId;
    }

    UI32  PoIntfManagedObject::getPoPrimaryLinkDomainId() const
    {
        return (m_poPrimaryLinkDomainId);
    }

    void  PoIntfManagedObject::setPoPartnerSystemPriority(const UI32 &poPartnerSystemPriority)
    {
        m_poPartnerSystemPriority  =  poPartnerSystemPriority;
    }

    UI32  PoIntfManagedObject::getPoPartnerSystemPriority() const
    {
        return (m_poPartnerSystemPriority);
    }

    void  PoIntfManagedObject::setPoPartnerKey(const UI32 &poPartnerKey)
    {
        m_poPartnerKey  =  poPartnerKey;
    }

    UI32  PoIntfManagedObject::getPoPartnerKey() const
    {
        return (m_poPartnerKey);
    }

    void  PoIntfManagedObject::setPoPartnerSystem(const string &poPartnerSystem)
    {
        m_poPartnerSystem  =  poPartnerSystem;
    }

    string  PoIntfManagedObject::getPoPartnerSystem() const
    {
        return (m_poPartnerSystem);
    }

    void  PoIntfManagedObject::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  PoIntfManagedObject::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  PoIntfManagedObject::setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId)
    {
        m_poLacpSystemIdDomainId  =  poLacpSystemIdDomainId;
    }

    UI32  PoIntfManagedObject::getPoLacpSystemIdDomainId() const
    {
        return (m_poLacpSystemIdDomainId);
    }

    void  PoIntfManagedObject::setPortMembers(const vector<ObjectId> &portMembers)
    {
        m_portMembers  =  portMembers;
    }

    vector<ObjectId>  PoIntfManagedObject::getPortMembers() const
    {
        return (m_portMembers);
    }
    
    void PoIntfManagedObject::setFcoeport(const WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> fcoeport)
    {
        m_fcoeport = fcoeport;
    }

    WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> PoIntfManagedObject::getFcoeport() const
    {
        return m_fcoeport;
    }
    
    void  PoIntfManagedObject::setNativeVlanIntfConfig(const WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> &nativeVlanIntfConfig)
    {
        m_nativeVlanIntfConfig  =  nativeVlanIntfConfig;
    }

    WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject>  PoIntfManagedObject::getNativeVlanIntfConfig() const
    {
        return (m_nativeVlanIntfConfig);
    }

     void  PoIntfManagedObject::setVlanClassifierActivate(const vector<WaveManagedObjectPointer<VlanClassifierActivateGlobalManagedObject> > &vlanClassifierActivate)
    {
        m_vlanClassifierActivate  =  vlanClassifierActivate;
    }

    vector<WaveManagedObjectPointer<VlanClassifierActivateGlobalManagedObject> >  PoIntfManagedObject::getVlanClassifierActivate() const
    {
        return (m_vlanClassifierActivate);
    }

    void PoIntfManagedObject::addVlanClassifierActivate(VlanClassifierActivateGlobalManagedObject *VlanClassifierActivateMO)
    {
        WaveManagedObjectPointer<VlanClassifierActivateGlobalManagedObject > VlanClassifierActivate (VlanClassifierActivateMO);
        m_vlanClassifierActivate.push_back(VlanClassifierActivate);

    }
    int PoIntfManagedObject::deleteVlanClassifierActivatedGroup(const UI32 &groupId)
    {
		NSM_PLUG_DBG("Entering ...");
		size_t vecSize = m_vlanClassifierActivate.size();
        unsigned int i;
        for (i = 0; i < vecSize; i++) {
            if ( (m_vlanClassifierActivate[i]->getGroupId()) == groupId ) {
				NSM_PLUG_DBG_S(string("vlan classifier provisioning of group ") + m_vlanClassifierActivate[i]->getGroupId() + string(" deleted"));
				m_vlanClassifierActivate.erase(m_vlanClassifierActivate.begin() + i);
				return (0);
			}
        }
        return (0);
    }
    void  PoIntfManagedObject::deleteAllVlanClassifierActivatedGroup()
	{
		m_vlanClassifierActivate.clear();
	}	

    void  PoIntfManagedObject::setNodePo(const WaveManagedObjectPointer<NsmNodePoManagedObject> &nodePo)
    {
        m_nodePo  =  nodePo;
    }

    WaveManagedObjectPointer<NsmNodePoManagedObject>  PoIntfManagedObject::getNodePo() const
    {
        return (m_nodePo);
    }

}
