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
 *   Author : sudheend                                                     *
 **************************************************************************/

#include "Mstp/Global/InstanceBaseManagedObject.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Mstp/Global/MstpGlobalUpdateMstpInstanceConfigMessage.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpInstanceConfigMessage.h"
#include "vcs.h"

#include <algorithm>

using namespace std;

namespace DcmNs
{

    InstanceBaseManagedObject::InstanceBaseManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (InstanceBaseManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager)
    {
      m_instanceId=0;
    }

    InstanceBaseManagedObject::InstanceBaseManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI8 &instanceId,const vector<ObjectId> &vlanid)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (InstanceBaseManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        m_instanceId    (instanceId),
        m_instanceVlans     (vlanid)
    {
    }

    InstanceBaseManagedObject::~InstanceBaseManagedObject ()
    {
    }

    string  InstanceBaseManagedObject::getClassName()
    {
        return ("InstanceBaseManagedObject");
    }

    void  InstanceBaseManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI8(&m_instanceId,"instanceId", xstp_id));
        addPersistableAttribute (new AttributeObjectIdVectorAssociation(&m_instanceVlans,"vlanid", getClassName (), getObjectId (), VlanIntfManagedObject::getClassName (), 0, false, true));


	setUserTagForAttribute("vlanid", xstp_vlan);

    }

    void  InstanceBaseManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI8(&m_instanceId,"instanceId", xstp_id));
        addPersistableAttributeForCreate  (new AttributeObjectIdVectorAssociation(&m_instanceVlans,"vlanid", getClassName (), getObjectId (), VlanIntfManagedObject::getClassName (), 0, false, true));
    }

    void InstanceBaseManagedObject::setupKeys ()
    {
        vector<string> keys;
        keys.push_back("instanceId");
        setUserDefinedKeyCombination(keys);
    }

    void  InstanceBaseManagedObject::setInstanceId(const UI8 &instanceId)
    {
        m_instanceId  =  instanceId;
    }


    Attribute *InstanceBaseManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
	    Attribute *pAttribute = NULL;
#if 0
	    Attribute *pAttribute = new Attribute;
	bool * temp = new bool;
	*temp = true;

	    pAttribute->setAttributeType(AttributeType::AttributeTypeBool);
	    pAttribute->setValue((void *)temp);

#endif
            pAttribute = new AttributeBool (true, "mstp");

	    trace (TRACE_LEVEL_ERROR,
			    string("MstpBridgeConfigManagedObject::getAttributeByUserTagUserDefined: Entry "));
	    //pAttribute = new AttributeBool (true, "id");

	    // This function can be used to return user defined attribute if getAttributeByUserTag returns NULL
	    // in some case
	    return (pAttribute);
    }

#if 0
    ResourceId InstanceBaseManagedObject::vlanObjectIdMemberOfInstance(ObjectId &vlanOid, UI8 instanceId)
    {
	    if (find(m_activeVlan.begin(), m_activeVlan.end(), vlanOid) != m_activeVlan.end() ||
			    find(m_inactiveVlan.begin(), m_inactiveVlan.end(), vlanOid) != m_inactiveVlan.end()) {
		    return WAVE_MESSAGE_ERROR;
	    }

	    if (inactive || m_activeVlan.size() >= MSTP_DCM_MAX_VLAN_BRIDGE) {
		    m_inactiveVlan.push_back(vlanOid);
	    } else {
		    m_activeVlan.push_back(vlanOid);
	    }

	    return WAVE_MESSAGE_SUCCESS;
    }
#endif

    UI8  InstanceBaseManagedObject::getInstanceId() const
    {
        return (m_instanceId);
    }

    void  InstanceBaseManagedObject::setInstanceVlans (const vector<ObjectId> &instVlans)
    {
        m_instanceVlans  =  instVlans;
    }

    vector<ObjectId>  InstanceBaseManagedObject::getInstanceVlans () const
    {
        return (m_instanceVlans);
    }

    void InstanceBaseManagedObject::setDefaultAttribute()
    {
        m_instanceVlans.clear();
    }

    void InstanceBaseManagedObject::setDefaultAttribute(const UI8 instanceId)
    {
	    setInstanceId(instanceId);
	    setDefaultAttribute();
    }

    bool InstanceBaseManagedObject::addInstanceVlan (const ObjectId &vlanOid)
    {
	    if (find(m_instanceVlans.begin(), m_instanceVlans.end(), vlanOid) == m_instanceVlans.end() ) {
	        m_instanceVlans.push_back(vlanOid);
	        return true;
	    } else {
	        return false;
	    }
    }

    bool InstanceBaseManagedObject::removeInstanceVlan (const ObjectId &vlanOid)
    {
        vector<ObjectId>::iterator pos = find(m_instanceVlans.begin(), m_instanceVlans.end(), vlanOid);
	    if (pos != m_instanceVlans.end()) {
	        m_instanceVlans.erase(pos);
	        return true;
	    } else {
		    return false;
	    }
    }

    ResourceId InstanceBaseManagedObject::setAttributeByMessage(const MstpGlobalUpdateMstpInstanceConfigMessage *pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;
	    switch (pMsg->m_cmdCode) {
	    case MSTP_MSG_BR_INST_ADD_VLAN:
	        for (UI32 i = 0; i < pMsg->m_vlanid.size(); ++i) {
	            addInstanceVlan(pMsg->m_vlanid[i]);
	        }
	        break;

	    case MSTP_MSG_BR_INST_REMOVE_VLAN:
	        for (UI32 i = 0; i < pMsg->m_vlanid.size(); ++i) {
	            removeInstanceVlan(pMsg->m_vlanid[i]);
	        }
	        break;

	    default:
	        trace (TRACE_LEVEL_ERROR, "InstanceBaseManagedObject::setAttributeByMessage: Invalid command code \n");
	        status = WAVE_MESSAGE_ERROR;
	        break;
	    }
	    return (status);
    }


    ResourceId InstanceBaseManagedObject::getAttributeToMessage(MstpInstanceConfigMessage *&pMsg)
    {
        pMsg->setVlanid(getInstanceVlans());
        pMsg->setInstanceId(getInstanceId());

        return (WAVE_MESSAGE_SUCCESS);
    }

}
