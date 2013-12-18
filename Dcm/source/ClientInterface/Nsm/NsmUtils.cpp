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

#include "ClientInterface/Nsm/NsmUtils.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "Fcoe/Global/FcoeFabricMapManagedObject.h"
#include "Fcoe/Global/FcoeGlobalFsbManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Attributes/AttributeSqlIn.h"

#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"
#include "vcs.h"
#include "ssm_dcmd.h"

#include "ClientInterface/Ssm/SsmApp.h"
#include "Cluster/Local/WaveNode.h"
//All Managed Object Headers
#include "Nsm/Global/VlanClassifierRulesManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/VlanClassifierGroupAssocRulesManagedObject.h"
#include "Nsm/Global/VlanClassifierActivateGlobalManagedObject.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Local/VlanClassifierActivateLocalManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/TenGigLocalManagedObject.h"
#include "Nsm/Local/FortyGigLocalManagedObject.h"
#include "Nsm/Local/VLAN/VlanIdForPoPhyIntfView.h"
#include "Nsm/Local/VLAN/VlanRangeForPoPhyIntfView.h"
#include "PortCfg/Local/PortFCCfgLocalManagedObject.h"
#include "Nsm/Local/OneGigLocalManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "Qos/Common/QosUtils.h"
#include "APPM/Utils/APPMUtils.h"

#include "Span/Global/SpanGlobalConfigManagedObject.h"

#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "Utils/DceContext.h"
#include "Utils/mo_iterator.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "Nsm/Global/Igmps/IgmpsVlanIntfConfManagedObject.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Nsm/Local/CtagBasedVlanClassificationMO.h"
#include "Nsm/Local/MacBasedVlanClassificationMO.h"
#include "Nsm/Local/RspanGVlanAccessPortManagedObject.h"
#include "Nsm/Local/RspanGVlanTrunkPortManagedObject.h"

namespace DcmNs
{

template<class T>
string getString(const T & t)
{
        stringstream ss;
        ss<<t;
        return ss.str();
}

string StringToLower(string strToConvert)
{    // change each element of the string to lower case
	for (unsigned int i = 0; i < strToConvert.length(); i++)
	{
		strToConvert[i] = tolower(strToConvert[i]);
	}
	return strToConvert;    // return the converted string
}


LocationId NsmUtils::getLocationId(string ifName)
{
    if(false == isValidThreeTuple(ifName) )
	{
	   return FrameworkToolKit::getThisLocationId (); 
	}
	UI32 mappedId = getMappedIdFromIfName(ifName);
	return DcmToolKit::getLocationIdFromMappedId(mappedId);
}	

int32_t NsmUtils::getRbridgeId(string ifName)
{
    int32_t rbridgeId = 0, slotId, portId;
    char *sname;

    if (ifName == "") return 0;

    sname = new char[ifName.size() + 1];
    strcpy(sname, ifName.c_str());

    if(true == isValidThreeTuple(ifName)) {
        sscanf(sname, "%d/%d/%d", &rbridgeId, &slotId, &portId);
    }

    delete[] sname;
    
    return rbridgeId;
}

int32_t NsmUtils::getSlotId(string ifName)
{
    int32_t locId, slotId, portId;
    char *sname;

    if (ifName == "") return 0;

    sname = new char[ifName.size() + 1];
    strcpy(sname, ifName.c_str());
    if (!isdigit(sname[0])) {
        delete[] sname;
        return getSlotId(ifName.substr(2));
    } 

    if(true == isValidThreeTuple(ifName)) {
        sscanf(sname, "%d/%d/%d", &locId, &slotId, &portId);
    }
    else {
        sscanf(sname, "%d/%d", &slotId, &portId);
    }

    delete[] sname;
    
    return slotId;
}
int32_t NsmUtils::getFcoeVlan() {
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeFabricMapManagedObject::getClassName());
    syncQueryCtxt.addSelectField("vlan");
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    FcoeFabricMapManagedObject *fcoeMapMO = NULL;
    int32_t vlanid;

    if (NULL == pResults) {
        trace(TRACE_LEVEL_INFO, "Query returned NULL results");
            return WAVE_MESSAGE_SUCCESS;
    }

    UI32 numberOfResults = pResults->size ();
    if (numberOfResults == 0 || numberOfResults > 1) {
        trace(TRACE_LEVEL_ERROR, "Query returned 0 or >1 results, MO not found");
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        return WAVE_MESSAGE_SUCCESS;
    }
    fcoeMapMO = dynamic_cast<FcoeFabricMapManagedObject *> ((*pResults)[0]);
    if (fcoeMapMO == NULL) {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        return WAVE_MESSAGE_SUCCESS;
    }
    vlanid = fcoeMapMO->getVlan(); 
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

    return vlanid;  
}


int32_t NsmUtils::getPortId(string ifName)
{
    int32_t locId, slotId, portId;
    char *sname;

    if (ifName == "") return 0;

    sname = new char[ifName.size() + 1];
    strcpy(sname, ifName.c_str());
    if (!isdigit(sname[0])) {
        delete[] sname;
        return getSlotId(ifName.substr(2));
    }

    if(true == isValidThreeTuple(ifName)) {
        sscanf(sname, "%d/%d/%d", &locId, &slotId, &portId);
    }
    else {
        sscanf(sname, "%d/%d", &slotId, &portId);
    }

    delete[] sname;
    
    return portId;
}

void NsmUtils::setLocalLocationIdForPhase1Message(WaveSendToClusterContext *ctx_p)
{
	vector<LocationId> locationIds;
	LocationId temp_locationId = FrameworkToolKit::getThisLocationId ();
	insert_LocationId(locationIds, temp_locationId);
	ctx_p -> setLocationsToSendToForPhase1(locationIds);
}

void NsmUtils::setLocationIdFromIfNameForPhase1Message(WaveSendToClusterContext *ctx_p, string ifName)
{
	vector<LocationId> locationIds;
	LocationId temp_locationId = getLocationId(ifName);
	insert_LocationId(locationIds, temp_locationId);
	ctx_p -> setLocationsToSendToForPhase1(locationIds);
}	
	
int NsmUtils::setLocationIdsForPoSubModeCmds(UI32 PoId, WaveSendToClusterContext *ctx_p, bool includeDisconnectedNodes)
{
	string  PoName = getString(PoId);
	return setLocationIdsForPoSubModeCmds(PoName, ctx_p, includeDisconnectedNodes);
}	

int NsmUtils::setLocationIdsForPoSubModeCmds(string PoId, WaveSendToClusterContext *ctx_p, bool includeDisconnectedNodes)
{
	vector<LocationId> locationIds;
	ResourceId status = WAVE_MESSAGE_ERROR;
	NSM_PLUG_DBG_TRACE();
	NSM_PLUG_ASSERT(ctx_p);
	status = DcmToolKit::isVcsEnabled ();
	NSM_PLUG_DBG_S(string("NsmUtils::getThreeTupleIfName -isVcsEnabled: ") +
		status);
	if (WAVE_PERSISTENCE_CLUSTER_ENABLED != status)
	{
		LocationId temp_locationId = FrameworkToolKit::getThisLocationId ();
		insert_LocationId(locationIds, temp_locationId);
		ctx_p -> setLocationsToSendToForPhase1(locationIds);
		return SET_LOCATION_ID_API_SUCCESS;
	}
	else if(WAVE_PERSISTENCE_CLUSTER_ENABLED == status)
	{	
		if(false == isPoCmdSendToLocalPlugin(PoId))
			return PORT_NOT_PART_OF_ANY_INTERFACE;
		getPoMemberLocationIds(PoId, locationIds, includeDisconnectedNodes);
		if(locationIds.size() == 0)
		{
			NSM_PLUG_INFO_S(string("PORT_NOT_PART_OF_ANY_INTERFACE"));
			return PORT_NOT_PART_OF_ANY_INTERFACE;
		}	
		ctx_p -> setLocationsToSendToForPhase1(locationIds);
		return SET_LOCATION_ID_API_SUCCESS;
	}
	return WAVE_MESSAGE_ERROR;
}
bool  NsmUtils::isLocationIdExists(vector<LocationId> &locs ,const  LocationId &locationid)
{
	for(unsigned int i = 0;i < locs.size();i++)
	{
		if(locs[i] == locationid)
		{
			return true;
		}	
	}	
	return false;
}	
void   NsmUtils::insert_LocationId(vector<LocationId> &locs ,const  LocationId &locationid)
{
	if(false == isLocationIdExists(locs, locationid))
	{
		locs.push_back(locationid);
	}
	return ;
}

bool
NsmUtils::isAnyPoMemberFcoeConfigured(const UI32 &PoId)
{
	NSM_PLUG_DBG_TRACE();
	string PoString = getString(PoId);
	return (isAnyPoMemberFcoeConfigured(PoString));
}

bool
NsmUtils::isAnyPoMemberFcoeConfigured(const string  &PoName)
{
	bool result = false;
	NSM_PLUG_DBG_TRACE();

	ObjectId PoOid;

	getPoIntfObjectIdByPoIfName(PoName, PoOid, false);
	vector<PhyIntfLocalManagedObject *> vpPhyIntfLocalManagedObject;
	GetPhyMoByPoOid(PoOid,  vpPhyIntfLocalManagedObject);

	NSM_PLUG_INFO_S(string("Number of Members =") + vpPhyIntfLocalManagedObject.size());

	for(unsigned int i = 0;i< vpPhyIntfLocalManagedObject.size(); i++)
	{
		if (result == false) {
			WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> child;
		
			child = vpPhyIntfLocalManagedObject[i]->getFcoeport();
		
			if (child.operator->() != NULL) {
				NSM_PLUG_INFO_S(string("FCOE Configured for Mbr=") + vpPhyIntfLocalManagedObject[i]->getIfName());
				result = true;
			} else {
				NSM_PLUG_INFO_S(string("Mbr ") + vpPhyIntfLocalManagedObject[i]->getIfName() +
								string("is not FCoE enabled"));
			}
		}
		
		delete vpPhyIntfLocalManagedObject[i];
	}	
	return result;
}

/*
 * This function determines the type of port-channel interface (standard/brocade) and the
 * rbridge id for member interfaces if port-channel interface type is brocade (2). If
 * port-channel interface type is standard (1) then set rbridge as 0. 
 */
ResourceId NsmUtils::getPoTypeAndRbridge(const UI32 &poId, UI32 &poType, UI32 &rBridge)
{
	ObjectId poOid;
	string poName = getString(poId);

	NSM_PLUG_DBG_TRACE();

	getPoIntfObjectIdByPoIfName(poName, poOid, false);

	vector<PhyIntfLocalManagedObject *> vpPhyIntfLocalManagedObject;
	GetPhyMoByPoOid(poOid, vpPhyIntfLocalManagedObject);

	if (vpPhyIntfLocalManagedObject.size() > 0) {
		poType = vpPhyIntfLocalManagedObject[0]->getPoType();

		if (poType == NSM_PO_TYPE_BROCADE) {
			string ifName = vpPhyIntfLocalManagedObject[0]->getIfName();
			rBridge = getRbridgeId(ifName);
		} else {
			rBridge = 0;
		}

		for(unsigned int i = 0; i < vpPhyIntfLocalManagedObject.size(); i++)
		{
			delete vpPhyIntfLocalManagedObject[i];
		}
	} else {
		rBridge = 0;
		poType = 0;
	}

	return WAVE_MESSAGE_SUCCESS;	
}

ResourceId  NsmUtils::getPoMemberIfindexList(const UI32 &PoId, vector<UI64> &ifx_list)
{
	NSM_PLUG_DBG_TRACE();
	string PoString = getString(PoId);
	return (getPoMemberIfindexList(PoString, ifx_list));
}

ResourceId  NsmUtils::getPoMemberIfindexList(const string  &PoName,  vector<UI64>& ifx_list)
{
	NSM_PLUG_DBG_TRACE();

	ObjectId PoOid;

	getPoIntfObjectIdByPoIfName(PoName, PoOid, false);
	vector<PhyIntfLocalManagedObject *> vpPhyIntfLocalManagedObject;
	GetPhyMoByPoOid(PoOid,  vpPhyIntfLocalManagedObject);
	for(unsigned int i = 0;i< vpPhyIntfLocalManagedObject.size(); i++)
	{
		UI64 ifindex = vpPhyIntfLocalManagedObject[i]->getIfIndex();
        NSM_PLUG_INFO_S(string("Retrieved Ifindex for Mbr=") + vpPhyIntfLocalManagedObject[i]->getIfName() +
                        string(" Ifindex=") + ifindex);
	    ifx_list.push_back(ifindex);
		delete vpPhyIntfLocalManagedObject[i];
	}	
	return WAVE_MESSAGE_SUCCESS;
}

ResourceId  NsmUtils::getPoMemberLocationIds(const UI32 &PoId, vector<LocationId> &locationIds, bool includeDisconnectedNodes)
{
	NSM_PLUG_DBG_TRACE();
	string PoString = getString(PoId);
	return (getPoMemberLocationIds(PoString, locationIds, includeDisconnectedNodes));
}	
ResourceId  NsmUtils::getPoMemberLocationIds(const string  &PoName,  vector<LocationId>& locationIds, bool includeDisconnectedNodes)
{
	NSM_PLUG_DBG_TRACE();

	ObjectId PoOid;

	getPoIntfObjectIdByPoIfName(PoName, PoOid, false);
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeObjectId (&PoOid, "poId"));
    syncQueryCtxt.addSelectField ("id");
    if (!includeDisconnectedNodes) {
        syncQueryCtxt.setFilterByConnectedNodes();
    }
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

    unsigned int count = pResults->size();
	for(unsigned int i = 0; i < count; i++)
	{
        pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[i]);
		string threeTuppleIfName = pPhyIntfLocalManagedObject->getId();
		if(false ==  isValidThreeTuple(threeTuppleIfName) )
		{
			/*Non Vcs Mode so send the command to local Plugin*/
			LocationId temp_locationId = FrameworkToolKit::getThisLocationId ();
			insert_LocationId(locationIds, temp_locationId);
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		    return WAVE_MESSAGE_SUCCESS;	
		}	
		SI32 MappedId = getMappedIdFromIfName(threeTuppleIfName);
		LocationId temp_locationId = DcmToolKit::getLocationIdFromMappedId(MappedId);
		insert_LocationId(locationIds, temp_locationId);
		delete pPhyIntfLocalManagedObject;
	}
    if (pResults) {
        pResults->clear();
        delete pResults;
        pResults = 0;
    }
	return WAVE_MESSAGE_SUCCESS;
}	

/*Returns no.of ports per port-channel belongs to a given location id */
int NsmUtils::getPoNumOfMemberPortsByLocationId (const string &poName, const LocationId &locId) {
    int memberCount = 0;
	ObjectId PoOid;

	getPoIntfObjectIdByPoIfName(poName, PoOid, false);
    vector<WaveManagedObject *> *pResults = queryPoMembersForLocationId (PoOid, locId);
	if (pResults && pResults->size() > 0) {
        memberCount = pResults->size();
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}

    return memberCount;
}

/*Returns no.of ports per port-channel*/
int NsmUtils::getPoNumOfMemberPorts(const string  &PoName)
{
	NSM_PLUG_DBG_TRACE();
	unsigned int i, PoSize = 0;
	ObjectId PoOid;

	getPoIntfObjectIdByPoIfName(PoName, PoOid, false);
	vector<PhyIntfLocalManagedObject *> vpPhyIntfLocalManagedObject;
	GetPhyMoByPoOid(PoOid,  vpPhyIntfLocalManagedObject);
	PoSize = vpPhyIntfLocalManagedObject.size();
	/*Free the vector*/
	for(i = 0;i< PoSize; i++)
	{
		delete vpPhyIntfLocalManagedObject[i];
	}
	return PoSize;
}
/*Returns no.of RBridges participate in the vLAG*/
int NsmUtils::getPoNumOfRBridges(const string  &PoName, vector<SI32> &memberRbridges)
{
	NSM_PLUG_DBG_TRACE();
	vector<LocationId> locationIds;
	ObjectId PoOid;

	getPoIntfObjectIdByPoIfName(PoName, PoOid, false);
	vector<PhyIntfLocalManagedObject *> vpPhyIntfLocalManagedObject;
	GetPhyMoByPoOid(PoOid,  vpPhyIntfLocalManagedObject);
	for(unsigned int i = 0;i< vpPhyIntfLocalManagedObject.size(); i++)
	{
		string threeTuppleIfName = vpPhyIntfLocalManagedObject[i]->getId();
		if(false ==  isValidThreeTuple(threeTuppleIfName) )
		{
			/*Non Vcs Mode so send the command to local Plugin*/
			LocationId temp_locationId = FrameworkToolKit::getThisLocationId ();
			insert_LocationId(locationIds, temp_locationId);
            memberRbridges.push_back(DcmToolKit::getLocalMappedId());
			for (UI32 j = 0; j < vpPhyIntfLocalManagedObject.size(); j++)
			{
				if (vpPhyIntfLocalManagedObject[j])
				{
					delete vpPhyIntfLocalManagedObject[j];
				}
			}
			return WAVE_MESSAGE_SUCCESS;
		}
		SI32 MappedId = getMappedIdFromIfName(threeTuppleIfName);
		LocationId temp_locationId = DcmToolKit::getLocationIdFromMappedId(MappedId);
        if (false == isLocationIdExists(locationIds, temp_locationId)) {
            locationIds.push_back(temp_locationId);
            memberRbridges.push_back(MappedId);
        }
		delete vpPhyIntfLocalManagedObject[i];
	}
	return locationIds.size();
}
#if 0
/*Returns no.of brocade trunks per chip*/
int NsmUtils::getPoNumOfBTrunksPerChip(const string  &ChipID)
{
}
#endif

bool NsmUtils::isPoCmdSendToLocalPlugin(UI32&  PoId)
{
	string PoString = getString(PoId);
	NSM_PLUG_DBG_TRACE();
	return ( isPoCmdSendToLocalPlugin(PoString));
}	
bool NsmUtils::isPoCmdSendToLocalPlugin(const string &PoId)
{
	ResourceId ret = WAVE_MESSAGE_ERROR;
    NSM_PLUG_DBG_TRACE();
	ObjectId PoObjectId;
	ret = getPoIntfObjectIdByPoIfName(PoId, PoObjectId, false);
	if(ret == WAVE_MESSAGE_ERROR)
	{	
		return false;
	}	
	ret = isPoIntfHasMember(PoObjectId);
	if(ret == WAVE_MESSAGE_ERROR)
	{
		return false;
	}	
	return true;
}	

UI32 NsmUtils::getVlanClassifierRuleByObjectId(const ObjectId &ruleOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p)
{
	UI32 ruleId = 0;
	NSM_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierRulesManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(ruleOid.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(ruleOid.getInstanceId(), "objectidinstanceid") );
	setAllCompositionfalse(syncQueryCtxt);
	loadOnlySelectedField(syncQueryCtxt, "ruleId");
		
	vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			VlanClassifierRulesManagedObject* p =  dynamic_cast<VlanClassifierRulesManagedObject*>((*pResults)[0]);
			ruleId = p->getRuleId();
			NSM_PLUG_DBG_S(string("vlan classifier rule ") + ruleId );
		}
		else
		{
			NSM_PLUG_DBG("could not find rule id");
		}
		if(true == isMarkedForDeletion)
		{
			NSM_PLUG_DBG("Marking Quried Result for Deletion");
			if(seq_ctx_p)
			{
				seq_ctx_p->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
		}
		else
		{
			NSM_PLUG_DBG("Deleting Queried Results");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}
	return ruleId;

}	

/* This Function returns the count value as 1 when the Port-channel exists, 
 * else returns 0 */

UI32 NsmUtils::isPoConfigured(const SI32 &poId)
{
    UI32 count = 0;
	const string id = "id";
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute(new AttributeSI32(poId, id));
    querySynchronouslyForCount(&syncQueryCtxt, count);
    return(count);
}

UI32 NsmUtils::getVlanIdByObjectId(const ObjectId &vlanOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p)
{
	UI32 vlanId = 0;
	NSM_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(vlanOid.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(vlanOid.getInstanceId(), "objectidinstanceid") );
	setAllCompositionfalse(syncQueryCtxt);
	loadOnlySelectedField(syncQueryCtxt, "id");
	
	vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
	if(pResults)
	{
		if(pResults->size() == 1)
		{
			VlanIntfManagedObject* p =  dynamic_cast<VlanIntfManagedObject*>((*pResults)[0]);
			vlanId = p->getId();
			NSM_PLUG_DBG_S(string("vlan classifier rule ") + vlanId );
		}
		else
		{
			NSM_PLUG_DBG("could not find vlan id");
		}
		if(true == isMarkedForDeletion)
		{
			NSM_PLUG_DBG("Marking Quried Result for Deletion");
			if(seq_ctx_p)
			{
				seq_ctx_p->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
		}
		else
		{
			NSM_PLUG_DBG("Deleting Queried Results");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}
	return vlanId;

}	

/*This function needs to be optimized for performance 
 * change signature to achieve ????
 * performace issues loading unnecessary composition 
 *can this method be removed?????*/
ResourceId NsmUtils::GetPhyMoByPoOid(ObjectId &poIntf_oid,vector<PhyIntfLocalManagedObject *> &vpPhyIntfLocalManagedObject)
{
	ResourceId  status   = WAVE_MESSAGE_ERROR;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeObjectId (&poIntf_oid,"poId"));
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

	if(pResults) {
		for(unsigned int i = 0;i< pResults->size() ;i++)
		{
			if (pResults && pResults->size() > 0)
			{
				pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[i]);
				if(pPhyIntfLocalManagedObject->getPoId() == poIntf_oid)
				{
					vpPhyIntfLocalManagedObject.push_back(pPhyIntfLocalManagedObject);
					status = WAVE_MESSAGE_SUCCESS;
				}
				else
				{
					delete pPhyIntfLocalManagedObject;
				}

			}
		}
	}
	if (pResults) {
		pResults->clear();
		delete pResults;
	}
	return status;

}
ResourceId NsmUtils::isPoIntfHasMember(const ObjectId &PoObjectId)
{
	NSM_PLUG_DBG_S(string("Entering ..."));
	UI32 count=0;

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeObjectId(PoObjectId,"poId"));
	ResourceId queryStatus = querySynchronouslyForCount(&syncQueryCtxt, count);
	if(WAVE_MESSAGE_SUCCESS == queryStatus && count > 0)
		return WAVE_MESSAGE_SUCCESS;
	return WAVE_MESSAGE_ERROR;
}
ResourceId NsmUtils::isPoIntfHasMemberByLocation(const ObjectId &PoObjectId,const LocationId &locationId)
{
	NSM_PLUG_DBG_S(string("Entering ..."));

	ResourceId status   = WAVE_MESSAGE_ERROR;
	vector<WaveManagedObject *> *pResults = queryPoMembersForLocationId (PoObjectId, locationId);

	if (pResults && pResults->size() > 0) {
		status = WAVE_MESSAGE_SUCCESS;
	}

	if (pResults) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}

	return status;

}

ResourceId  NsmUtils:: isPoIntfHasMemberByLocationId (const ObjectId &poIntf_oid, const LocationId &locationId)
{
	NSM_PLUG_DBG_S(string("Entering ..."));

	ResourceId status   = WAVE_MESSAGE_ERROR;
	vector<WaveManagedObject *> *pResults = queryPoMembersForLocationId (poIntf_oid,locationId);

	if (pResults && pResults->size() > 0) {
		status = WAVE_MESSAGE_SUCCESS;
	}

	if (pResults) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}

	return status;
}


vector<WaveManagedObject *>  *NsmUtils:: queryPoMembersForLocationId (const ObjectId &poIntf_oid,const LocationId &locationId)
{
	NSM_PLUG_DBG_S(string("Entering ..."));

	WaveManagedObjectSynchronousQueryContext synchronousQueryContext (WaveNode::getClassName());

	synchronousQueryContext.addAndAttribute (new AttributeUI32 (locationId, "locationId"));
	setAllCompositionfalse(synchronousQueryContext);
	vector<WaveManagedObject *> *pResultsWaveNodes = querySynchronously (&synchronousQueryContext);

	if (NULL == pResultsWaveNodes)
	{
		trace (TRACE_LEVEL_ERROR,"NsmUtils:: queryPoMembersForLocationId no wave node found for this location id");
		prismAssert (false, __FILE__, __LINE__);
	}

	if (1 != pResultsWaveNodes->size ())
	{
		trace (TRACE_LEVEL_ERROR,string("NsmUtils:: queryPoMembersForLocationId No of Node found ")+ pResultsWaveNodes->size ());
		prismAssert (false, __FILE__, __LINE__);
	}

	/*Can we make it to load only ObjectId*/
	/*if we load objectId only can we refer WaveNode*/
	WaveNode *pWaveNode = dynamic_cast<WaveNode *>((*pResultsWaveNodes)[0]);

	ObjectId objectId = pWaveNode->getObjectId ();

	WaveManagedObjectSynchronousQueryContext synchronousQueryContextForManagedObjects (PhyIntfLocalManagedObject::getClassName());

	synchronousQueryContextForManagedObjects.addAndAttribute (new AttributeObjectId (objectId, "ownerWaveNodeObjectId"));

	synchronousQueryContextForManagedObjects.addAndAttribute (new AttributeObjectId(poIntf_oid,"poId"));

	setAllCompositionfalse(synchronousQueryContextForManagedObjects);
	

	vector<WaveManagedObject *> *pResultsManagedObjects = querySynchronously (&synchronousQueryContextForManagedObjects);

	delete pWaveNode;

	pResultsWaveNodes->clear ();

	delete pResultsWaveNodes;

	return pResultsManagedObjects;
}

/*Return no.of eddge port members of given port-channel*/
int NsmUtils::queryPoEdgeMembersCountForLocationId (const ObjectId &poIntf_oid,const LocationId &locationId)
{
	UI32 count = 0;
	int status = 0;
	
	NSM_PLUG_DBG("Entering ...");

	WaveManagedObjectSynchronousQueryContext synchronousQueryContext (WaveNode::getClassName());

	synchronousQueryContext.addAndAttribute (new AttributeUI32 (locationId, "locationId"));

	vector<WaveManagedObject *> *pResultsWaveNodes = querySynchronously (&synchronousQueryContext);

	if (NULL == pResultsWaveNodes)
	{
		NSM_PLUG_DBG_ERR("No wave node found for this location id");
		prismAssert (false, __FILE__, __LINE__);
	}

	if (1 != pResultsWaveNodes->size ())
	{
		NSM_PLUG_DBG_ERR_S(string("No of Node found ")+ pResultsWaveNodes->size ());
		prismAssert (false, __FILE__, __LINE__);
	}

	WaveNode *pWaveNode = dynamic_cast<WaveNode *>((*pResultsWaveNodes)[0]);

	ObjectId objectId = pWaveNode->getObjectId ();

	WaveManagedObjectSynchronousQueryContext synchronousQueryContextForManagedObjects (PhyIntfLocalManagedObject::getClassName());

	synchronousQueryContextForManagedObjects.addAndAttribute (new AttributeObjectId (objectId, "ownerWaveNodeObjectId"));

	synchronousQueryContextForManagedObjects.addAndAttribute (new AttributeObjectId(poIntf_oid,"poId"));
	synchronousQueryContextForManagedObjects.addAndAttribute (new AttributeBool(false, "Portrole"));

    ResourceId queryStatus = querySynchronouslyForCount (&synchronousQueryContextForManagedObjects, count);
	status = count;
    NSM_PLUG_DBG_S(string("After querySynchronouslyForCount::")+count);
	  
    if (WAVE_MESSAGE_SUCCESS != queryStatus)
    {
        NSM_PLUG_DBG_ERR_S(string("Query Failed Reason : ") + FrameworkToolKit::localize (queryStatus));
        status = -1;
    }

	delete pWaveNode;
	pResultsWaveNodes->clear ();
	delete pResultsWaveNodes;
	return status;
}

VlanIntfManagedObject * NsmUtils::getVlanIntfObjectByObjectId(const ObjectId &objectId)
{
	VlanIntfManagedObject *pVlanIntfManagedObject = NULL;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute(new AttributeUI32(objectId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(objectId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

    if (pResults && pResults->size() == 1 ) {
        pVlanIntfManagedObject =  dynamic_cast<VlanIntfManagedObject*>((*pResults)[0]);
        return pVlanIntfManagedObject;
    }

	if (pResults) {
		pResults->clear();
		delete pResults;
	}
	return NULL;

}
string NsmUtils::getVlanIntfNameByObjectId(const ObjectId &objectId)
{
    VlanIntfManagedObject *pVlanIntfManagedObject;
    pVlanIntfManagedObject = getVlanIntfObjectByObjectId(objectId); 
	string ifName;
        
	if(pVlanIntfManagedObject != NULL)
    {
        ifName = pVlanIntfManagedObject->getIfName();
		delete pVlanIntfManagedObject;
		return ifName;
    }
    return NULL;
}

PoIntfManagedObject * NsmUtils::getPoIntfObjectByObjectId(const ObjectId &objectId)
{
	PoIntfManagedObject *pPoIntfManagedObject = NULL;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(objectId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(objectId.getInstanceId(), "objectidinstanceid") );
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	if (pResults && pResults->size()  == 1) {
		pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
		return pPoIntfManagedObject;
	}
	if (pResults) {
		pResults->clear();
		delete pResults;
	}
	return NULL;

}

string NsmUtils::getPoIntfNameByObjectId(const ObjectId &objectId)
{
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(objectId.getClassId(), "objectidclassid") );
	syncQueryCtxt.addAndAttribute(new AttributeUI64(objectId.getInstanceId(), "objectidinstanceid") );
    setAllCompositionfalse(syncQueryCtxt);
	loadOnlySelectedField(syncQueryCtxt, "ifName");
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	string ifName = "";
	if(pResults)  
	{	
		if(pResults->size() == 1)
		{
			PoIntfManagedObject *pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
			ifName = pPoIntfManagedObject->getIfName();
		}
		 WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}	
    return ifName;
}

bool NsmUtils::isVlanClassifierGroupIdAddedToPoInterface(UI32 groupId, string ifName, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p)
{
	NSM_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierActivateGlobalManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId") );
	syncQueryCtxt.addAndAttribute(new AttributeString(ifName, "ifName") );

	vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
	bool ret = false;

	if (NULL != pResults)
	{
		if(pResults->size() > 0)
		{
			NSM_PLUG_DBG_S(string("Group id ") + groupId + string("is added to Port-Channel interface") );
			ret = true;
		}
		if(true == isMarkedForDeletion)
		{
			NSM_PLUG_DBG("Marking for Deletion:(in sequencer step)");
			if(seq_ctx_p)
			{
				seq_ctx_p->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
		}
		else
		{
			NSM_PLUG_DBG("Deleting Queried Results");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}
	else
	{
		NSM_PLUG_DBG_S(string("Group id ") + groupId + string("is not added to Port-Channel interface") );
		ret = false;
	}
	return ret;

}	

bool NsmUtils::isVlanClassifierGroupIdAddedToPhyInterface(UI32 groupId, string ifName, bool isMarkedForDeletion)
{
	NSM_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierActivateLocalManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId") );
	syncQueryCtxt.addAndAttribute(new AttributeString(ifName, "ifName") );

	vector<WaveManagedObject * >*               pResults                                    = querySynchronously(&syncQueryCtxt);
	bool ret = false;

	if (NULL != pResults)
	{
		if(pResults->size() > 0)
		{
			NSM_PLUG_DBG_S(string("Group id ") + groupId + string("is added to physical interface") );
			ret = true;
		}
		if(true == isMarkedForDeletion)
		{
			NSM_PLUG_DBG("Marking for Deletion:(in sequencer step)");
		}
		else
		{
			NSM_PLUG_DBG("Deleting Queried Results");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}
	else
	{
		NSM_PLUG_DBG_S(string("Group id ") + groupId + string("is not added to physical interface") );
		ret = false;
	}
	return ret;

}	

/*TODO: Remove isMarkedForDeletion and seq_ctx_p
 * not necessary now*/
bool NsmUtils::isVlanClassifierRuleIdExists(UI32 ruleId, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p)
{
	NSM_PLUG_DBG("Entering ...");
	UI32 count = 0;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierRulesManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(ruleId, "ruleId"));
	ResourceId queryStatus = querySynchronouslyForCount(&syncQueryCtxt, count);

	if(WAVE_MESSAGE_SUCCESS == queryStatus && 1 == count)
		return true;
	else 
		return false;
}	

/*TODO: Remove isMarkedForDeletion and seq_ctx_p
 * not necessary now*/
bool NsmUtils::isVlanClassifierGroupRuleEntryExists(UI32 groupId, UI32 ruleId, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p)
{
	NSM_PLUG_DBG("Entering ...");
	ObjectId ruleOid;
	UI32 count = 0;
	
	getObjectIdByVlanClassifierRuleId(ruleId, ruleOid, isMarkedForDeletion, seq_ctx_p);
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));
	syncQueryCtxt.addAndAttribute(new AttributeObjectId(ruleOid, "ruleId"));
	ResourceId queryStatus = querySynchronouslyForCount(&syncQueryCtxt, count);
	
	if(WAVE_MESSAGE_SUCCESS == queryStatus && 1 == count)
		return true;
	else 
		return false;
}
/*TODO: Remove isMarkedForDeletion and seq_ctx_p
 * not necessary now*/
bool NsmUtils::isVlanClassifierGroupIdExists(UI32 groupId, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p)
{
	NSM_PLUG_DBG("Entering ...");
	UI32 count = 0;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierGroupAssocRulesManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(groupId, "groupId"));
    ResourceId queryStatus = querySynchronouslyForCount(&syncQueryCtxt, count);

	if (WAVE_MESSAGE_SUCCESS == queryStatus) {
        return (count > 0);
    } else {
        NSM_PLUG_DBG_ERR_S(string("Query Failed : ") + FrameworkToolKit::localize(queryStatus));
        return false;
    }
}


ResourceId NsmUtils:: getVlanIntfObjectIdByVlanIfName (UI32 vlanId, ObjectId &vlanOid, bool isMarkedForDeletion, PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
	ResourceId  status   = WAVE_MESSAGE_ERROR;
	VlanIntfManagedObject *pVlanIntfManagedObject = NULL;

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlanId,"id"));
	setAllCompositionfalse(syncQueryCtxt);
	loadOnlySelectedField(syncQueryCtxt, "objectId", true);
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (pResults && pResults->size() > 0) {
		pVlanIntfManagedObject =  dynamic_cast<VlanIntfManagedObject*>((*pResults)[0]);
		vlanOid = pVlanIntfManagedObject->getObjectId();
		status = WAVE_MESSAGE_SUCCESS;
	}

	if (pResults) {
		if(!isMarkedForDeletion)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		else
		{
			if(pPrismSynchronousLinearSequencerContext)
				pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			pResults->clear();
			delete pResults;
		}
	}
	return status;
}

ResourceId NsmUtils::getVlanIntfObjectIdByVlanIfName(UI32 vlanId, ObjectId &vlanOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p)
{
	NSM_PLUG_DBG("Entering ...");
	ResourceId                               status                 = WAVE_MESSAGE_ERROR;
	VlanIntfManagedObject*                   pVlanIntfManagedObject = NULL;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(&vlanId, "id"));
	setAllCompositionfalse(syncQueryCtxt);
	loadOnlySelectedField(syncQueryCtxt, "objectId", true);
	vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
	if (pResults && (pResults -> size() > 0))
	{
		pVlanIntfManagedObject = dynamic_cast<VlanIntfManagedObject*>((*pResults)[0]);
		vlanOid                = pVlanIntfManagedObject -> getObjectId();
		status                 = WAVE_MESSAGE_SUCCESS;
		NSM_PLUG_DBG_S(string("vlanId = ") + vlanId + string("vlanOid = ") + vlanOid.toString());
		if(true == isMarkedForDeletion)
		{
			NSM_PLUG_DBG("Marking Quried Result for Deletion");
			if(seq_ctx_p)
			{
				seq_ctx_p->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
		}
		else
		{
			NSM_PLUG_DBG("Deleting Queried Results");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}

	}
	return status;
}

ResourceId NsmUtils::getObjectIdByVlanClassifierRuleId(int ruleId, ObjectId &ruleOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p)
{
	NSM_PLUG_DBG("Entering ...");
	ResourceId                               status                            = WAVE_MESSAGE_ERROR;
	VlanClassifierRulesManagedObject*        pVlanClassifierRulesManagedObject = NULL;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierRulesManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(ruleId, "ruleId"));
	setAllCompositionfalse(syncQueryCtxt);
	loadOnlySelectedField(syncQueryCtxt, "objectId", true);
	vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);

	if (pResults && (pResults -> size() == 1))
	{
		pVlanClassifierRulesManagedObject = dynamic_cast<VlanClassifierRulesManagedObject*>((*pResults)[0]);
		ruleOid                           = pVlanClassifierRulesManagedObject -> getObjectId();
		status                            = WAVE_MESSAGE_SUCCESS;
		NSM_PLUG_DBG_S(string("ruleId = ") + ruleId + string("ruleOid = ") + ruleOid.toString());
		if(true == isMarkedForDeletion)
		{
			NSM_PLUG_DBG("Marking Quried Result for Deletion");
			if(seq_ctx_p)
			{
				seq_ctx_p->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
		}
		else
		{
			NSM_PLUG_DBG("Deleting Queried Results");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
	}
	return status;

}

ResourceId NsmUtils::getPoIntfObjectIdByPoIfName(string poName, ObjectId &poIntfOid,  bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p)
{
	NSM_PLUG_DBG("Entering ...");
	ResourceId                               status               = WAVE_MESSAGE_ERROR;
	PoIntfManagedObject*                     pPoIntfManagedObject = NULL;
	UI32                                     poId;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
	poId = strtoul(poName.c_str(), NULL, 0);
	syncQueryCtxt.addAndAttribute(new AttributeUI32(&poId, "id"));
	setAllCompositionfalse(syncQueryCtxt);
	loadOnlySelectedField(syncQueryCtxt, "objectId", true);
	vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
	if (pResults && (pResults -> size() ==  1))
	{
		pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
		poIntfOid           = pPoIntfManagedObject -> getObjectId();
		status               = WAVE_MESSAGE_SUCCESS;
		NSM_PLUG_DBG_S(string("poObjectId = ") + poIntfOid.toString() );
		if(true == isMarkedForDeletion)
		{
			NSM_PLUG_DBG("Marking Quried Result for Deletion");
			if(seq_ctx_p)
			{
				seq_ctx_p->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
			}
		}
		else
		{
			NSM_PLUG_DBG("Deleting Queried Results");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}

	}
	return status;

}
              


/*This function needs to be optimized for performance 
 * change signature to achieve ????
 * performace issues loading unnecessary composition 
 */

ResourceId NsmUtils::getPoMoByPoId (UI32 PoId, PoIntfManagedObject* &pPoIntfManagedObject)
{
	NSM_PLUG_DBG("Entering ...");
	ResourceId                               status = WAVE_MESSAGE_SUCCESS;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeUI32(&PoId, "id"));
	syncQueryCtxt.addLimitForQueryResults(1); // for safety
	vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
	if (pResults && (pResults -> size() == 1))
	{
		pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject*>((*pResults)[0]);
		status                 = WAVE_MESSAGE_SUCCESS;
	}

    delete pResults;
	return status;
}	

/*This function needs to be optimized for performance 
 * change signature to achieve ????
 * performace issues loading unnecessary composition 
 */
ResourceId NsmUtils::getPhyMoByPhyName(const string &PhyName, PhyIntfLocalManagedObject* &pPhyIntfLocalManagedObject )
{
	NSM_PLUG_DBG("Entering ...");
	ResourceId                               status = WAVE_MESSAGE_ERROR;
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeString(PhyName, "id"));
    syncQueryCtxt.addLimitForQueryResults(1); // for safety
	vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
	if (pResults && (pResults -> size() == 1))
	{
		NSM_PLUG_DBG_S(string("Found Phy Mo for") + PhyName);
		pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
		status                       = WAVE_MESSAGE_SUCCESS;
	}

    delete pResults;
	return status;

}	

/* Function: NsmUtils::getThreeTupleIfName
 *  
 * @ifName (IN) : Two tuple interface name (Eg. 0/0)
 * @threeTupleIfName (OUT) : Equivalent Three tuple interface name (Eg. 1/0/0)
 * @mappedId (IN) : optional mappedId param. If mappedId passed in '0', then Local 
 *                  Mapped Id is queried and filled.
 *
 * Returns: WAVE_MESSAGE_SUCCESS if successful with  OUT param appropriately filled.
 *          WAVE_PERSISTENCE_CLUSTER_DISABLED if VCS is not enabled.
 *
 * Description: Given Two tuple interface name, get Three Tuple inteface. 
 */

ResourceId 
NsmUtils::getThreeTupleIfName (const string& ifName, string& threeTupleIfName, 
                               const SI32 mappedId) 
{
    ResourceId  ret = WAVE_MESSAGE_SUCCESS;
    SI32        retMappedId = 0;
    
    ret = DcmToolKit::isVcsEnabled ();

    NSM_PLUG_DBG_S(string("NsmUtils::getThreeTupleIfName -isVcsEnabled: ") + 
            ret);

    if (WAVE_PERSISTENCE_CLUSTER_DISABLED == ret) { 
        return ret;
    }
    ret = 0;

    if (mappedId) { 
        retMappedId = mappedId;
    } else { 
        retMappedId = DcmToolKit::getLocalMappedId();
    }

     NSM_PLUG_DBG_S(string("NsmUtils::getThreeTupleIfName  - retMappedId:") + 
            retMappedId);

    if (retMappedId != (-1) ) {
        threeTupleIfName = getString(retMappedId) + "/" + ifName;
               NSM_PLUG_DBG_S(string("NsmUtils::getThreeTupleIfName: Three tuple val: ") + 
                threeTupleIfName);
    }
    return ret;
}

SI32 
NsmUtils::getMappedIdFromIfName (const string& threeTupleIfName) 
{
    string  delimiter = "/";
    SI32    mappedId = -1;
    size_t  pos = threeTupleIfName.find_first_of(delimiter, 0);
    
    if (pos == string::npos) { 
        return mappedId;
    } 
    string mappedIdStr = threeTupleIfName.substr(0, pos);
    mappedId = strtol(mappedIdStr.c_str(), NULL, 0);
    if (!mappedId) { 
        mappedId = -1;
    }
    return mappedId;
}

bool
NsmUtils::isValidTwoTuple (const string& ifName)
{
    string  delimiter = "/";
    int count = 0;
    size_t  last_pos = ifName.find_first_of(delimiter, 0);
    size_t len = 0;

    while (string::npos != last_pos) {
        count ++;
        len = last_pos + 1;
        last_pos = ifName.find_first_of(delimiter, len);
    }

    if (count != 1) {
        return false;
    }
    return true;
}

int NsmUtils::convertThreeTuppleToTwoTupple(const string ifName,string &twoTuppleIfName)
{
	if(isValidThreeTuple(ifName))
	{
		getTwoTupleIfName(ifName, twoTuppleIfName);
		return 0;
	}
	else if(isValidTwoTuple(ifName))
	{
		twoTuppleIfName = ifName;
		return 0;
	}
	else
	{
		return -1;
	}
	return -1;
}

void NsmUtils::ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid)
{
	vector<LocationId>  locationsToSendToForPhase1 = ctx_p -> getLocationsToSendToForPhase1 ();

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	for(unsigned int i = 0; i < locationsToSendToForPhase1.size(); i++)
	{
		locationid = locationsToSendToForPhase1[i];
		status = ctx_p -> getCompletionStatusForPhase1(locationid);
		if (status != WAVE_MESSAGE_SUCCESS)
			return;

	}	
}

UI32 
NsmUtils::getLocalLocationId ()
{
    SI32 mappedId = 0;
    SI32 locationId = 0;
    
    mappedId = DcmToolKit::getLocalMappedId();
    if (mappedId != (-1) ) { 
        locationId = DcmToolKit::getLocationIdFromMappedId (mappedId);
    }
    return locationId;
}

string 
NsmUtils::getMeIfName(UI32  mappedId, const string& ifName)
{
	NSM_PLUG_DBG("Entering...");
	string tempIfName = getTeThreeTuple(mappedId, ifName);
	NSM_PLUG_DBG_S(tempIfName);
	if(false == isValidThreeTuple(tempIfName))
	{	
		NSM_PLUG_DBG_S(string("is not valid thre tupple name ifName =  ") + ifName); 
		tempIfName = ifName;
	}
	return tempIfName;
}	

string
NsmUtils::getTeThreeTuple (UI32  mappedId, const string& ifName)
{
    int         pos =  ifName.find(" ");
    ResourceId  ret = WAVE_MESSAGE_SUCCESS;
    SI32        retMappedId = 0;
    string      threeTupleIfName = "";

    ret = DcmToolKit::isVcsEnabled ();

    NSM_PLUG_DBG_S(string("NsmUtils::getThreeTupleIfName -isVcsEnabled: ") +
            ret);

    if (WAVE_PERSISTENCE_CLUSTER_DISABLED == ret) {
        return ifName;
    }

    if (mappedId) {
        retMappedId = mappedId;
    } else {
        retMappedId = DcmToolKit::getLocalMappedId();
    }

    NSM_PLUG_DBG_S(string("getTeThreeTuple:retMappedId:") + retMappedId);

    if (retMappedId != (-1) ) {
        threeTupleIfName = ifName.substr(0, pos + 1) + getString(retMappedId) +
                            "/" + ifName.substr(pos + 1);
        NSM_PLUG_DBG_S(string("TeThreeTuple:") + threeTupleIfName);
    } else { 
        threeTupleIfName = ifName;
    }

    return threeTupleIfName;
}

bool NsmUtils::isIntfL2 (const InterfaceType ifType, const string &ifName)
{
    // TODO should be using different class names??
    WaveManagedObjectSynchronousQueryContext queryCtxt(PoPhyIntfManagedObject::getClassName());
    queryCtxt.addAndAttribute(new AttributeString(ifName, "ifName"));
    queryCtxt.addAndAttribute(new AttributeBool(true, "swModeFlag"));
    unsigned int count = 0;
    ResourceId status = querySynchronouslyForCount(&queryCtxt, count);
    if (status != WAVE_MESSAGE_SUCCESS) {
        trace(TRACE_LEVEL_ERROR, "NsmUtils::isIntfL2 -- Query failed!! " + FrameworkToolKit::localize(status));
    } else {
        tracePrintf(TRACE_LEVEL_DEBUG, "NsmUtils::isIntfL2 -- Query returned %d", count);
    }

    return count == 1;
}

/* TODO: Need to add support for L3 Mode */
ResourceId NsmUtils::getIntfMode(const  string &ifName, UI32 type)
{
	ResourceId status = WAVE_MESSAGE_ERROR;
	vector<WaveManagedObject *> *pResults = NULL;
	if (type == PO_INTF_TYPE) {
		UI32 po_id;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
		po_id = strtoul(ifName.c_str (), NULL, 0);
		syncQueryCtxt.addAndAttribute (new AttributeUI32(&po_id,"id"));
		setAllCompositionfalse(syncQueryCtxt);
		loadOnlySelectedField(syncQueryCtxt, "swModeFlag");
		pResults = querySynchronously(&syncQueryCtxt);
	} else if (type == PHY_INTF_TYPE) {
		WaveManagedObjectSynchronousQueryContext  syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (ifName,"Id"));
		setAllCompositionfalse(syncQueryCtxt);
		loadOnlySelectedField(syncQueryCtxt, "swModeFlag");
		pResults = querySynchronously(&syncQueryCtxt);
	} else {
		return (WAVE_MESSAGE_ERROR);
	}

	trace (TRACE_LEVEL_DEBUG, string("NsmUtils::getIntfMode : Enter:    ") );

	if (!pResults)
		return (WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE);

	if (pResults->size() <= 0) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		return (WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE);
	} else if (pResults->size() >  1) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		return (WRC_NSM_ERR_INTF_MULTIPLE);
	}else {
		if (type == PO_INTF_TYPE) {
			PoIntfManagedObject *pIntfMo =  dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
			if (pIntfMo->getSwModeFlag() == true) {
				status =  WRC_NSM_ERR_INTF_MODE_L2;
			} else {
				status =  WRC_NSM_ERR_INTF_MODE_UNK;
			}
		} else if (type == PHY_INTF_TYPE) {
			PhyIntfLocalManagedObject *pIntfMo =  dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
			if (pIntfMo->getSwModeFlag() == true) {
				status =  WRC_NSM_ERR_INTF_MODE_L2;
			} else {
				status =  WRC_NSM_ERR_INTF_MODE_UNK;
			}
		}
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		return (status);
	}
}

bool NsmUtils::isIntfShut(UI32 ifType, const string &ifName)
{
    string moClassName = (ifType == IF_TYPE_PO) ? PoIntfManagedObject::getClassName() : PhyIntfLocalManagedObject::getClassName();
    WaveManagedObjectSynchronousQueryContext qc (moClassName);
    qc.addAndAttribute(new AttributeString(ifName, "id"));
    qc.addAndAttribute(new AttributeBool(true, "status"));
    UI32 count = 0;
    querySynchronouslyForCount(&qc, count);
    tracePrintf(TRACE_LEVEL_INFO, true, false, "NsmUtils::isIntfShut : moClass=%s, ifName=%s, count=%u",
            moClassName.c_str(), ifName.c_str(), count);
    return count == 1;
}
bool NsmUtils::isIntfPortProfiled(const InterfaceType ifType, const string &ifName)
{
    string moClassName = (ifType == IF_TYPE_PO) ? PoIntfManagedObject::getClassName() : PhyIntfLocalManagedObject::getClassName();
    WaveManagedObjectSynchronousQueryContext qc (moClassName);
    qc.addAndAttribute(new AttributeString(ifName, "id"));
    qc.addAndAttribute(new AttributeBool(true, "isPortProfiled"));
    UI32 count = 0;
    querySynchronouslyForCount(&qc, count);
    tracePrintf(TRACE_LEVEL_DEBUG, true, false, "NsmUtils::isIntfPortProfiled : moClass=%s, ifName=%s, count=%u",
            moClassName.c_str(), ifName.c_str(), count);
    return count == 1;
}

ResourceId NsmUtils::getIntfPortProfiled(const  string &ifName, UI32 type)
{
	ResourceId status = WAVE_MESSAGE_ERROR;
	vector<WaveManagedObject *> *pResults = NULL;
	
	NSM_PLUG_DBG("Entering..");
	
	if (type == PO_INTF_TYPE) {
		UI32 po_id;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
		po_id = strtoul(ifName.c_str (), NULL, 0);
		syncQueryCtxt.addAndAttribute (new AttributeUI32(&po_id,"id"));
		setAllCompositionfalse(syncQueryCtxt);
        loadOnlySelectedField(syncQueryCtxt, "isPortProfiled");
        loadOnlySelectedField(syncQueryCtxt, "associatedPortProfiles");
        loadOnlySelectedField(syncQueryCtxt, "associatedPortProfileDomain");
        pResults = querySynchronously(&syncQueryCtxt);
	} else if (type == PHY_INTF_TYPE) {
		WaveManagedObjectSynchronousQueryContext  syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString (ifName,"Id"));
		setAllCompositionfalse(syncQueryCtxt);
		loadOnlySelectedField(syncQueryCtxt, "isPortProfiled");
        loadOnlySelectedField(syncQueryCtxt, "associatedPortProfiles");
        loadOnlySelectedField(syncQueryCtxt, "associatedPortProfileDomain");
		pResults = querySynchronously(&syncQueryCtxt);
	} else {
		return (WAVE_MESSAGE_ERROR);
	}


	if (!pResults)
		return (WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE);

	if (pResults->size() <= 0) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		return (WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE);
	} else if (pResults->size() >  1) {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		return (WRC_NSM_ERR_INTF_MULTIPLE);
	}else {
		if (type == PO_INTF_TYPE) {
			PoIntfManagedObject *pIntfMo =  dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
            //When CDP sends a port-profile-port notification, verify whether the port
            //is port-profile-port.
            bool isProfiledPort = false;
            APPMUtils::isPortProfilePort(pIntfMo, isProfiledPort);
            if (isProfiledPort == true) {
				status =  WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
			} else {
				status =  WRC_NSM_ERR_DCM_APPM_PROFILED_PORT;
			}
		} else if (type == PHY_INTF_TYPE) {
			PhyIntfLocalManagedObject *pIntfMo =  dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
            //When CDP sends a port-profile-port notification, verify whether the port
            //is port-profile-port.
            bool isProfiledPort = false;
            APPMUtils::isPortProfilePort(pIntfMo, isProfiledPort);
            if (isProfiledPort == true) {
				status =  WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
			} else {
				status =  WRC_NSM_ERR_DCM_APPM_PROFILED_PORT;
			}
		}
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		return (status);
	}
	NSM_PLUG_DBG_ERR("Impossible case...");
	return (WAVE_MESSAGE_ERROR);
}

InterfaceType NsmUtils::getIfType(const ObjectId &ifOid)
{
    static UI32 GI_CLASS_ID = OrmRepository::getTableId(OneGigLocalManagedObject::getClassName());
    static UI32 TE_CLASS_ID = OrmRepository::getTableId(TenGigLocalManagedObject::getClassName());
    static UI32 FO_CLASS_ID = OrmRepository::getTableId(FortyGigLocalManagedObject::getClassName());
    static UI32 PO_CLASS_ID = OrmRepository::getTableId(PoIntfManagedObject::getClassName());

    UI32 classId = ifOid.getClassId();
    if (classId == GI_CLASS_ID) return IF_TYPE_GI;
    if (classId == TE_CLASS_ID) return IF_TYPE_TE;
    if (classId == FO_CLASS_ID) return IF_TYPE_FO;
    if (classId == PO_CLASS_ID) return IF_TYPE_PO;
    return IF_TYPE_INVALID;
}

string NsmUtils::getIfShortName (InterfaceType ifType, const string &ifName)
{
    string shortName;
    switch (ifType) {
    case IF_TYPE_GI:    shortName.append("Gi "); break;
    case IF_TYPE_TE:    shortName.append("Te "); break;
    case IF_TYPE_FO:    shortName.append("Fo "); break;
    case IF_TYPE_PO:    shortName.append("Po "); break;
    default:
        tracePrintf(TRACE_LEVEL_WARN, "NsmUtils::getIfShortName : ***** Unhandled ifType %u", ifType);
        return ifName;
    }

    shortName.append(ifName);
    return shortName;
}


ResourceId NsmUtils::isAllUI32RangeVlanConfigured (UI32Range vlanRange, vector<ObjectId> *vlanOids,
        PrismLinearSequencerContext *prismCntx, PrismSynchronousLinearSequencerContext *prismSyncCntx)
{
    trace(TRACE_LEVEL_DEBUG, "NsmUtils::isAllUI32RangeVlanConfigured : Enter:");
    ResourceId  status = WAVE_MESSAGE_SUCCESS;

    vector <UI32> msgVlanRangeVector;
    vlanRange.getUI32RangeVector(msgVlanRangeVector);
    size_t vidCount = msgVlanRangeVector.size();

    tracePrintf(TRACE_LEVEL_DEBUG, "NsmUtils::isAllUI32RangeVlanConfigured : number of vlans=%d", vidCount);
    if (msgVlanRangeVector.empty()) {
        // nothing to check
        return status;
    }

    vector<Attribute *> vidAttributes;
    for(size_t i = 0; i < vidCount; ++i) {
        vidAttributes.push_back(new AttributeUI32(msgVlanRangeVector[i], "id"));
    }

    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeSqlIn(vidAttributes), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
    syncQueryCtxt.addAndAttribute(new AttributeBool(false, "remoteSpanEnabled"));

    // If VLAN oids need not be collected, do not query VlanIntfManagedObject
    // count is sufficient..
    if (vlanOids == NULL) {
        UI32 moCount = 0;
        status = querySynchronouslyForCount(&syncQueryCtxt, moCount);
        tracePrintf(TRACE_LEVEL_DEBUG, "NsmUtils::isAllUI32RangeVlanConfigured : Query returned %d count, expected %d", moCount, msgVlanRangeVector.size());
        return (status == WAVE_MESSAGE_SUCCESS) && (moCount != vidCount) ? WAVE_MESSAGE_ERROR : status;
    }

    syncQueryCtxt.addOrderField("id");
    syncQueryCtxt.addSelectField("id");
    syncQueryCtxt.addSelectFieldForObjectId("objectId");
    setAllCompositionfalse(syncQueryCtxt);

    // query - select objectid, id from VlanIntfManagedObject where id in ( .... )
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

    if (!pResults || pResults->empty()) {
        trace(TRACE_LEVEL_ERROR, "NsmUtils::isAllUI32RangeVlanConfigured : Query fetched no VlanIntfManagedObject !!!");
        delete pResults;
        return WAVE_MESSAGE_ERROR;  // TODO find better return value
    }

    size_t moCount = pResults->size();
    tracePrintf(TRACE_LEVEL_DEBUG, "NsmUtils::isAllUI32RangeVlanConfigured : Query fetched %d VlanIntfManagedObject records..", moCount);
    if (moCount == vidCount) {
        for (size_t i = 0; i < moCount; ++i) {
            VlanIntfManagedObject *pVlanIntf = dynamic_cast<VlanIntfManagedObject*>(pResults->at(i));
            ObjectId vlanOid = pVlanIntf->getObjectId();
            vlanOids->push_back(vlanOid);
        }
    } else {
        trace(TRACE_LEVEL_ERROR, "NsmUtils::isAllUI32RangeVlanConfigured : some vlans are missing!");
        status = WAVE_MESSAGE_ERROR;
    }

    if (prismCntx) {
        prismCntx->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
    } else if (prismSyncCntx) {
        prismSyncCntx->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
    } else {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    }

    trace (TRACE_LEVEL_DEBUG, string("NsmUtils::isAllUI32RangeVlanConfigured : return    ") +status );
    return (status);
}

ResourceId NsmUtils::isAllUI32RangeRspanVlanConfigured (UI32Range vlanRange)
{
	NSM_PLUG_INFO_S(string("Entering"));
    ResourceId  status = WAVE_MESSAGE_SUCCESS;

    vector <UI32> msgVlanRangeVector;
    vlanRange.getUI32RangeVector(msgVlanRangeVector);
    size_t vidCount = msgVlanRangeVector.size();

    NSM_PLUG_INFO_S(string("number of vlans=")+vidCount);
    if (msgVlanRangeVector.empty()) {
        // nothing to check
        return status;
    }

    vector<Attribute *> vidAttributes;
    for(size_t i = 0; i < vidCount; ++i) {
        vidAttributes.push_back(new AttributeUI32(msgVlanRangeVector[i], "id"));
    }

    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeSqlIn(vidAttributes), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
	syncQueryCtxt.addAndAttribute(new AttributeBool(true, "remoteSpanEnabled"));

	UI32 moCount = 0;
	status = querySynchronouslyForCount(&syncQueryCtxt, moCount);
	tracePrintf(TRACE_LEVEL_INFO, "NsmUtils::isAllUI32RangeRspanVlanConfigured : Query returned %d count, expected %d", moCount, msgVlanRangeVector.size());
	return (status == WAVE_MESSAGE_SUCCESS) && (moCount != vidCount) ? WAVE_MESSAGE_ERROR : status;
}


ResourceId NsmUtils::isAllUI32RangeVlanConfigured (UI32Range vlanRange, vector<ObjectId>  &vlanid)
{
    return isAllUI32RangeVlanConfigured(vlanRange, &vlanid, NULL, NULL);
}

/* This API can be used to to remove Native vlan  list */
bool NsmUtils::isNativeVlanPresentInList (UI32Range &vlanAddrange, UI32 nativeVlanId, PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject,   PoIntfManagedObject *pPoIntfManagedObject)
{
          bool isVlanIdPresent = false;
          vector<UI32>::iterator msgiter;
          vector <UI32> vlanRangeVector;
          string vlanRangeStr ;
  
          NSM_PLUG_DBG("Entering  ...");
  
          NSM_PLUG_DBG_S(string(" Native-vlan ::  ") +   nativeVlanId);
  
          vlanAddrange.getUI32RangeVector(vlanRangeVector);
          vector<UI32>::iterator moiter;
          moiter = std::find(vlanRangeVector.begin(),vlanRangeVector.end(),nativeVlanId);
          if (moiter != vlanRangeVector.end()) {
              vlanRangeVector.erase(moiter);
              vlanRangeStr = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
              vlanAddrange.fromString(vlanRangeStr);
              NSM_PLUG_DBG_S(string(" Removing from list ::  ") +   nativeVlanId);
              isVlanIdPresent = true;
          }
  
          return (isVlanIdPresent);
}

bool NsmUtils::isAclAppliedOnVlanInterfaceMember(const ObjectId &vlanObjectId, const UI32 &vlanId, const ssmAcl_binding_direction &direction)
{
	bool isAclApplied = false;

	// query only the members of the vlan, and then return true if the acl is applied on its vlan member.

	WaveManagedObjectSynchronousQueryContext queryCtx(PhyIntfLocalManagedObject::getClassName());

	queryCtx.createConditionSet ("ACCESS_VLAN_CHECK");

	queryCtx.addAttributeToConditionSet ("ACCESS_VLAN_CHECK", new AttributeObjectIdAssociation(vlanObjectId, "vlanObjectId",
			VlanIntfManagedObject::getClassName()), WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

	queryCtx.createConditionSet ("NO_VLAN_REMOVE_RANGE");

	queryCtx.addAttributeToConditionSet("NO_VLAN_REMOVE_RANGE",new AttributeBool(true,"isAllowedVlanAll"),
			WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

	UI32Range vlanRangeRemove;

	queryCtx.addAttributeToConditionSet("NO_VLAN_REMOVE_RANGE",new AttributeUI32Range(&vlanRangeRemove, "vlanRangeRemove"),
			WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

	queryCtx.combineConditionSets ("ACCESS_VLAN_CHECK+NO_VLAN_REMOVE_RANGE");

	queryCtx.addSelectField("ingressEgressMacAclPolicy");

	vector<WaveManagedObject *> *pResults = querySynchronously(&queryCtx);

	if (NULL != pResults)
	{
		UI32 numberOfResults = pResults->size ();

		for (UI32 i = 0; i < numberOfResults; i++)
		{
			PhyIntfLocalManagedObject* phyIntfMO = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[i]);
			if( phyIntfMO->getIngressEgressMacAclPolicyDirectioncount (direction) != 0 ) {
				isAclApplied = true;
				break;
			}
		}

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

		if(isAclApplied) {
			return true;
		}
	}

	// The command is "switchport trunk allowed vlan add 2-9"
	// If the vlan exists in any vlanRangeAdd, query only the members of the vlan,
	// then return true if the acl is applied on its vlan member.

	UI32Range nullRange;
	PoPhyIntfManagedObject *pPoPhyIntfManagedObject = NULL;

	vector<string> selectFields;

	WaveManagedObjectSynchronousQueryContext tmpCtxt(PoPhyIntfManagedObject::getClassName());

	tmpCtxt.setLoadOneToManyRelationships(false);
	tmpCtxt.setLoadCompositions(false);

	selectFields.push_back("vlanRangeAdd");
	selectFields.push_back("ingressEgressMacAclPolicy");

	tmpCtxt.addSelectFields(selectFields);

	tmpCtxt.addAndAttribute (new AttributeUI32Range(&nullRange, "vlanRangeAdd"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

	vector<WaveManagedObject *> *pTmpResults = querySynchronously(&tmpCtxt);

	if (NULL != pTmpResults)
	{
		UI32 tmpResultsSize = pTmpResults->size ();

		for (UI32 idx = 0; idx < tmpResultsSize; idx++)
		{
			pPoPhyIntfManagedObject = dynamic_cast<PoPhyIntfManagedObject *>((*pTmpResults)[idx]);
			vector <UI32> vlanRangeVector;
			pPoPhyIntfManagedObject->getVlanRangeAdd().getUI32RangeVector(vlanRangeVector);
			vector <UI32>::iterator it = find(vlanRangeVector.begin(), vlanRangeVector.end(), vlanId);
			if(it != vlanRangeVector.end())
			{
				if( pPoPhyIntfManagedObject->getIngressEgressMacAclPolicyDirectioncount (direction) != 0 )
				{
					isAclApplied = true;
					break;
				}
			}
		}

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pTmpResults);

		if(isAclApplied) {
			return true;
		}
	}

	//The command is
	//"switchport trunk allowed vlan all"
	//"switchport trunk allowed vlan remove 3"
	// query only the members of the vlan, and then return true if the acl is applied on its vlan member.

	WaveManagedObjectSynchronousQueryContext tmpCtxt2(PoPhyIntfManagedObject::getClassName());

	selectFields.clear();

	selectFields.push_back("vlanRangeRemove");
	selectFields.push_back("ingressEgressMacAclPolicy");

	tmpCtxt2.addSelectFields(selectFields);

	tmpCtxt2.addAndAttribute (new AttributeBool(true, "isAllowedVlanAll"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
	tmpCtxt2.addAndAttribute (new AttributeUI32Range(&nullRange, "vlanRangeRemove"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

	tmpCtxt2.setLoadOneToManyRelationships(false);
	tmpCtxt2.setLoadCompositions(false);

	vector<WaveManagedObject *> *pTmpResults2 = querySynchronously(&tmpCtxt2);

	if (NULL != pTmpResults2)
	{
		UI32 tmpResultsSize2 = pTmpResults2->size ();

		for (UI32 idx1 = 0; idx1 < tmpResultsSize2; idx1++)
		{
			pPoPhyIntfManagedObject = dynamic_cast<PoPhyIntfManagedObject *>((*pTmpResults2)[idx1]);

			vector <UI32> vlanRangeVector;
			pPoPhyIntfManagedObject->getVlanRangeRemove().getUI32RangeVector(vlanRangeVector);
			vector <UI32>::iterator it = find(vlanRangeVector.begin(), vlanRangeVector.end(), vlanId);
			if(it == vlanRangeVector.end())
			{
				if( pPoPhyIntfManagedObject->getIngressEgressMacAclPolicyDirectioncount (direction) != 0 )
				{
					isAclApplied = true;
					break;
				}
			}
		}

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pTmpResults2);
	}

	return isAclApplied;
}
  
bool NsmUtils::isNativeVlanPresentInList (UI32Range &vlanAddrange, PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject, PoIntfManagedObject *pPoIntfManagedObject)
{
          bool isVlanIdPresent = false;
          vector<UI32>::iterator msgiter;
          UI32 nativeVlanId =  NSM_NATIVE_VLAN_DEFAULT;
          vector <UI32> vlanRangeVector;
          string vlanRangeStr ;
  
          NSM_PLUG_DBG("Entering ...");
  
          WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> pNativeVlanIntfConfLocalManagedObject;
  
          if (pPhyIntfLocalManagedObject) {
              pNativeVlanIntfConfLocalManagedObject =
              WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> ( pPhyIntfLocalManagedObject->getNativeVlanIntfConfig());
          } else if (pPoIntfManagedObject) {
              pNativeVlanIntfConfLocalManagedObject =
              WaveManagedObjectPointer<NativeVlanIntfConfLocalManagedObject> ( pPoIntfManagedObject->getNativeVlanIntfConfig());
          } else {
              NSM_PLUG_DBG_ERR(" Both Po and Phy  pointers are NULL ");
              return (isVlanIdPresent);
          }
  
          if(pNativeVlanIntfConfLocalManagedObject.operator->() != NULL) {
              nativeVlanId = pNativeVlanIntfConfLocalManagedObject->getNativeVlanId();
          }
  
          NSM_PLUG_DBG_S(string(" Native-vlan ::  ") +   nativeVlanId);
  
          vlanAddrange.getUI32RangeVector(vlanRangeVector);
                  vector<UI32>::iterator moiter;
  
          if ((moiter = find(vlanRangeVector.begin(), vlanRangeVector.end(), nativeVlanId)) != vlanRangeVector.end()) {
              isVlanIdPresent = true;
          }
  
          return (isVlanIdPresent);
}

/* A port can be associated with a vlan 2(eg) with one of the following steps
 * 1. switchport access vlan 2
 * 2. switchport trunk native-vlan 2
 * 3. switchport trunk allowed vlan all
 * 4. switchport trunk allowed vlan add 1-4 (2 is in the range)
 * 5. switchport trunk allowed vlan except 4-7 (2 is not in the range)
 *
 * Only the following combinations will result after configuring the
 * above 5 commands in different ways. The checks should follow this
 * order only.
 * 1. switchport access vlan 2
 * 2. switchport trunk native-vlan 2
 * 3. switchport trunk allowed vlan add 2-9
 * 4. switchport trunk allowed vlan all
 *    switchport trunk allowed vlan remove 3
 * 5. switchport trunk allowed vlan all
 * 
 * 
 * An appropriate error should be returned for all the above cases
 * */
ResourceId NsmUtils::isAnyPortAMemberofVlan(UI32 vlanId, DceContext *ctxt)
{
	bool isRspanVlan = false;
	//The command is "switchport access vlan 2"
	//If any Access port is associated with this Vlan, return true.
	{
		mo_iterator<VlanIntfManagedObject> vlanMOIter(ctxt);
		vlanMOIter.criteria().And("id", vlanId).select("objectId").select("remoteSpanEnabled");
		if(vlanMOIter.hasNext())
		{
			VlanIntfManagedObject *vlanMO = vlanMOIter.next();
			isRspanVlan = vlanMO->getRemoteSpanEnabled();

			mo_iterator<PoPhyIntfManagedObject> poPhyIter(ctxt);
			poPhyIter.criteria().And("vlanObjectId", vlanMO->getObjectId());
			if(poPhyIter.criteria().queryCount() > 0)
			{
				trace(TRACE_LEVEL_INFO, (string("switchport access vlan ") + vlanId + string(" is set on a port")));
				return WAVE_MESSAGE_ERROR;
			}
		}
		else
		{
			trace(TRACE_LEVEL_INFO, (string("VlanIntfManagedObject is NULL")));
			return WRC_NSM_API_ERR_VLAN_NOT_CONFIGURED;
		}
	}
	//The command is "switchport trunk allowed vlan all"
	// If any port exists, then return error.
	//For rspan-vlan, we don't need to check for the allowed all config. For rspan-vlan, we check rspan_trunkvlanrange and rspan_trunkvlanrange.
	if(false == isRspanVlan)
	{
		mo_iterator<PoPhyIntfManagedObject> poPhyMOItr(ctxt);
		UI32Range vlanRangeRemove;
		poPhyMOItr.criteria().And("isAllowedVlanAll", true);
		poPhyMOItr.criteria().And(new AttributeUI32Range(&vlanRangeRemove, "vlanRangeRemove"));

		if (poPhyMOItr.criteria().queryCount() > 0)
		{
			trace(TRACE_LEVEL_INFO, (string("switchport trunk allowed vlan all is set on a port")));
			return WAVE_MESSAGE_ERROR;
		}
	}
	//The command is "switchport trunk native-vlan 2"
	//If any port is configured with this vlan as native vlan, then return error.
	{
		mo_iterator<NativeVlanIntfConfLocalManagedObject> nativeVlanMOIter(ctxt);
		nativeVlanMOIter.criteria().And("nativevlanId", vlanId);
		if(nativeVlanMOIter.criteria().queryCount() > 0)
		{
			trace(TRACE_LEVEL_INFO, (string("NativeVlan count is > 0 for the given vlan")));
			return WAVE_MESSAGE_ERROR;
		}
	}
	// The command is "switchport trunk allowed vlan add 2-9"
	// If the vlan exists in any vlanRangeAdd, then return error.
	{
		UI32Range nullRange;
		PoPhyIntfManagedObject *pPoPhyIntfManagedObject = NULL;
		mo_iterator<PoPhyIntfManagedObject> poPhyMOIter(ctxt);
		poPhyMOIter.criteria().select("vlanRangeAdd");
		poPhyMOIter.criteria().And(new AttributeUI32Range(&nullRange, "vlanRangeAdd"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

		for(pPoPhyIntfManagedObject = poPhyMOIter.next(); pPoPhyIntfManagedObject; pPoPhyIntfManagedObject = poPhyMOIter.next())	
		{
			trace(TRACE_LEVEL_INFO, (string("UI32Range is") + pPoPhyIntfManagedObject->getVlanRangeAdd().toString()));
			vector <UI32> vlanRangeVector;
		    pPoPhyIntfManagedObject->getVlanRangeAdd().getUI32RangeVector(vlanRangeVector);
			vector <UI32>::iterator it = find(vlanRangeVector.begin(), vlanRangeVector.end(), vlanId);
			if(it != vlanRangeVector.end())
			{
				trace(TRACE_LEVEL_INFO, (string("Vlan exists in vlanRangeAdd on some port")));
				return WAVE_MESSAGE_ERROR;
			}
		}
	}
	//The command is
	//"switchport trunk allowed vlan all"
	//"switchport trunk allowed vlan remove 3"
	//If the vlan doesn't exist in vlanRangeRemove, then return error
	//For rspan-vlan, we don't need to check for the remove configs. For rspan-vlan, we check rspan_trunkvlanrange and rspan_trunkvlanrange.
	if(false == isRspanVlan)
	{
		UI32Range nullRange;
		PoPhyIntfManagedObject *pPoPhyIntfManagedObject = NULL;
		mo_iterator<PoPhyIntfManagedObject> poPhyMOIter(ctxt);
		poPhyMOIter.criteria().select("vlanRangeRemove");
		poPhyMOIter.criteria().And("isAllowedVlanAll", true);
		poPhyMOIter.criteria().And(new AttributeUI32Range(&nullRange, "vlanRangeRemove"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

		for(pPoPhyIntfManagedObject = poPhyMOIter.next(); pPoPhyIntfManagedObject; pPoPhyIntfManagedObject = poPhyMOIter.next())	
		{
			trace(TRACE_LEVEL_INFO, (string("UI32Range is") + pPoPhyIntfManagedObject->getVlanRangeRemove().toString()));
			vector <UI32> vlanRangeVector;
		    pPoPhyIntfManagedObject->getVlanRangeRemove().getUI32RangeVector(vlanRangeVector);
			vector <UI32>::iterator it = find(vlanRangeVector.begin(), vlanRangeVector.end(), vlanId);
			if(it == vlanRangeVector.end())
			{
				trace(TRACE_LEVEL_INFO, (string("Vlan doesn't exist in vlanRangeRemove on some port")));
				return WAVE_MESSAGE_ERROR;
			}
		}
	}

	// The command is "switchport trunk rspan-vlan add 2-9"
	// If the vlan exists in any rspan_trunkvlanrange, then return error.
	{
		UI32Range nullRange;
		PhyIntfLocalManagedObject *phyIntfMo = NULL;
		mo_iterator<PhyIntfLocalManagedObject> poPhyMOIter(ctxt);
		poPhyMOIter.criteria().select("rspan_trunkvlanrange");
		poPhyMOIter.criteria().And(new AttributeUI32Range(&nullRange, "rspan_trunkvlanrange"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

		for(phyIntfMo = poPhyMOIter.next(); phyIntfMo; phyIntfMo = poPhyMOIter.next())
		{
			trace(TRACE_LEVEL_INFO, (string("Rspan-vlan UI32Range is") + phyIntfMo->getRspanVlanRangeAdd().toString()));
			vector <UI32> vlanRangeVector;
		    phyIntfMo->getRspanVlanRangeAdd().getUI32RangeVector(vlanRangeVector);
			vector <UI32>::iterator it = find(vlanRangeVector.begin(), vlanRangeVector.end(), vlanId);
			if(it != vlanRangeVector.end())
			{
				trace(TRACE_LEVEL_INFO, (string("Vlan exists in rspan_trunkvlanrange on some port")));
				return WAVE_MESSAGE_ERROR;
			}
		}
	}
	//The command is "switchport access rspan-vlan 2"
	//If any port is configured with this vlan as rspan-access vlan, then return error.
	{
		UI32 defaultVlanId = DCM_NSM_VLAN_DEFAULT_VID;
		PhyIntfLocalManagedObject *phyIntfMo = NULL;
		mo_iterator<PhyIntfLocalManagedObject> phyMOIter(ctxt);
		phyMOIter.criteria().select("rspan_accessvlan");
		phyMOIter.criteria().And(new AttributeUI32(&defaultVlanId, "rspan_accessvlan"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

		for(phyIntfMo = phyMOIter.next(); phyIntfMo; phyIntfMo = phyMOIter.next())
		{
			trace(TRACE_LEVEL_INFO, (string("Rspan-Access vlan is") + phyIntfMo->getAccessRspanVlan()));
		    UI32 rspanAccessVlan = phyIntfMo->getAccessRspanVlan();
		    if(vlanId == rspanAccessVlan) {
				trace(TRACE_LEVEL_INFO, (string("Vlan exists in rspan_accessvlan on some port")));
				return WAVE_MESSAGE_ERROR;
		    }
		}
	}
	//If all conditions passed sucessfully, then return success.
	return WAVE_MESSAGE_SUCCESS;
}

bool NsmUtils::isProfileSpeficFcoeApplied(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
{
	return (NsmLocalObjectManager::getInstance())->isProfileSpecificFcoeApplied(pPhyIntfLocalManagedObject);
}

bool NsmUtils::isProfileSpecificFcoeApplied(string ifName)
{
	return (NsmLocalObjectManager::getInstance())->isProfileSpecificFcoeApplied(ifName);
}

bool NsmUtils::isFcoeConfiguredOnAnyProfiledPort()
{
	return (NsmLocalObjectManager::getInstance())->isFcoeConfiguredOnAnyProfiledPort();
}

UI64
NsmUtils::getPhyPortIfindexByName(const string ifName)
{
    UI64 ifindex = 0;

    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute(new AttributeString(ifName, "id"));
    setAllCompositionfalse(syncQueryCtxt);
    loadOnlySelectedField(syncQueryCtxt, "ifIndex");
    vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
    PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
    NSM_PLUG_INFO_S(string("Searching for Name=") + ifName);
    if (pResults && (pResults -> size() == 1)) {
        NSM_PLUG_DBG_S(string("Found Phy Mo for") + ifName);
        pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
        ifindex = pPhyIntfLocalManagedObject->getIfIndex();
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    return ifindex;
}

bool NsmUtils::isPhyPortInISLMode(const string ifName, bool isMarkForDeletion ,PrismSynchronousLinearSequencerContext *seq_ctx_p)
{
	bool ret = true;
	NSM_PLUG_DBG("Entering ...");
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeString(ifName, "id"));
	setAllCompositionfalse(syncQueryCtxt);
	loadOnlySelectedField(syncQueryCtxt, "Portrole");
	vector<WaveManagedObject * >* pResults = querySynchronously(&syncQueryCtxt);
	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
	if (pResults && (pResults -> size() == 1))
	{
		NSM_PLUG_DBG_S(string("Found Phy Mo for") + ifName);
		pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
		ret =  pPhyIntfLocalManagedObject->getPortrole();
		if(isMarkForDeletion)
		{
			if(seq_ctx_p)
			{
				//seq_ctx_p->addManagedObjectsForGarbageCollection(*pResults);
				pResults->clear();
				delete pResults;
				seq_ctx_p->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
			}
		}

		else
		{
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}

	}
	return ret;

}
    /*
    * Checks whether the link is an ISL link
    */
ResourceId NsmUtils::checkIsISLLink(const string &ifName, bool &isISL) {
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute(new AttributeString(ifName, "id"));
    syncQueryCtxt.addAndAttribute(new AttributeBool(true, "Portrole"));
    UI32 count = 0;
    ResourceId result = WaveObjectManagerToolKit::querySynchronouslyForCount(&syncQueryCtxt, count);
    tracePrintf(TRACE_LEVEL_DEBUG, "NsmUtils::isISLLink, interface name = %s, count = %u, result = %u\n", ifName.c_str() , count, result);
    isISL = count == 1 ? true : false;
    if (WAVE_MESSAGE_SUCCESS != result) {
        trace(TRACE_LEVEL_ERROR, "NsmUtils::isISLLink failed to check the interface status, WaveObjectManagerToolKit::querySynchronouslyForCount failed \n");
    }
    return result;
}

ResourceId NsmUtils::mapBackEndErrorsToDcmdErrors(int err)
{
	switch(err)
	{
        case NSM_API_ERR_INVALID_MAC:
			return WRC_NSM_API_ERR_INVALID_MAC;

        case NSM_API_ERR_FPMA_MAC:
			return WRC_NSM_API_ERR_FPMA_MAC;

        case NSM_API_ERR_BCAST_MAC:
			return WRC_NSM_API_ERR_BCAST_MAC;

        case NSM_API_ERR_MCAST_MAC:
			return WRC_NSM_API_ERR_MCAST_MAC;

        case NSM_API_ERR_RULE_ID_EXIST:
			return WRC_NSM_API_ERR_RULE_ID_EXIST;

        case NSM_API_ERR_SAME_RULE:
			return WRC_NSM_API_ERR_SAME_RULE;

        case NSM_API_ERR_RULE_ADD_FAILED:
			return WRC_NSM_API_ERR_RULE_ADD_FAILED;

        case NSM_API_ERR_IF_RULE_OVERLAP:
			return WRC_NSM_API_ERR_IF_RULE_OVERLAP;

        case NSM_API_ERR_NO_GROUP:
			return WRC_NSM_API_ERR_NO_GROUP;

		case NSM_API_ERR_NO_RULE:                                                                  
			 return WRC_NSM_API_ERR_NO_RULE;

        case SWITCHING_NOT_ENABLED:
			return WRC_SWITCHING_NOT_ENABLED;

        case BRIDGE_NOT_VLAN_AWARE:
			return WRC_BRIDGE_NOT_VLAN_AWARE;

        case NSM_API_ERR_GRP_UNCONFIG:
			return WRC_NSM_API_ERR_GRP_UNCONFIG;

        case NSM_API_SET_ERR_NO_SUCH_INTERFACE:
			
			return WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE;

        case NSM_API_ERR_IF_INVLD_MODE:
			return WRC_NSM_API_ERR_IF_INVLD_MODE;

        case NSM_API_ERR_IF_UNBOUND:
			return WRC_NSM_API_ERR_IF_UNBOUND ;

        case NSM_API_ERR_VLAN_UNCONFIG:
			return WRC_NSM_API_ERR_VLAN_UNCONFIG;

        case NSM_API_VLAN_CLASS_ERR_FCOE:
			return WRC_NSM_API_VLAN_CLASS_ERR_FCOE;
        case NSM_API_VLAN_CLASS_ERR_TRUNKPORT:
			return WRC_NSM_API_VLAN_CLASS_ERR_TRUNKPORT;

		case NSM_API_ERR_IF_GROUP_OVERLAP :
			return WRC_NSM_API_ERR_IF_GROUP_OVERLAP;

		case NSM_API_ERR_GRP_ACTIVATE :
			return WRC_NSM_API_ERR_GRP_ACTIVATE;

		case NSM_APPM_PROFILED_PORT:
			return  WRC_NSM_APPM_PROFILED_PORT;

		case NSM_API_ERR_PORTCHANNEL_DOT1X_ENABLE    : return (WRC_NSM_API_ERR_PORTCHANNEL_DOT1X_ENABLE);
		case NSM_API_ERR_PORTCHANNEL_MTU_CONFIG      : return (WRC_NSM_API_ERR_PORTCHANNEL_MTU_CONFIG);
		case NSM_API_ERR_PORTCHANNEL_IP_MTU_CONFIG   : return (WRC_NSM_API_ERR_PORTCHANNEL_IP_MTU_CONFIG);
		case NSM_API_ERR_PORTCHANNEL_MAX_LAGS        : return (WRC_NSM_API_ERR_PORTCHANNEL_MAX_LAGS);
		case NSM_API_ERR_PORTCHANNEL_MAX_LINKS_STD   : return (WRC_NSM_API_ERR_PORTCHANNEL_MAX_LINKS_STD);
		case NSM_API_ERR_PORTCHANNEL_MAX_LINKS_BRCD  : return (WRC_NSM_API_ERR_PORTCHANNEL_MAX_LINKS_BRCD);
		case NSM_API_ERR_PORTCHANNEL_MIRROR          : return (WRC_NSM_API_ERR_PORTCHANNEL_MIRROR);
		case NSM_API_ERR_PORTCHANNEL_MAX_LAGS_BRCD   : return (WRC_NSM_API_ERR_PORTCHANNEL_MAX_LAGS_BRCD);
		case NSM_API_ERR_PORTCHANNEL_ALREADY_STATIC  : return (WRC_NSM_API_ERR_PORTCHANNEL_ALREADY_STATIC);
		case NSM_API_ERR_PORTCHANNEL_ALREADY_DYNAMIC : return (WRC_NSM_API_ERR_PORTCHANNEL_ALREADY_DYNAMIC);
		case NSM_API_ERR_PORTCHANNEL_ALREADY_STD     : return (WRC_NSM_API_ERR_PORTCHANNEL_ALREADY_STD);
		case NSM_API_ERR_PORTCHANNEL_ALREADY_BRCD    : return (WRC_NSM_API_ERR_PORTCHANNEL_ALREADY_BRCD);
		case NSM_LACP_ERROR_ADMIN_KEY_MISMATCH    : return ( WRC_NSM_LACP_ERROR_ADMIN_KEY_MISMATCH);
		case NSM_LACP_ERROR_TRUNK_TYPE_MISMATCH   : return ( WRC_NSM_LACP_ERROR_TRUNK_TYPE_MISMATCH);
		case NSM_LACP_ERROR_CHIP_NUM_MISMATCH     : return ( WRC_NSM_LACP_ERROR_CHIP_NUM_MISMATCH);
		case NSM_LACP_ERROR_CONFIGURED            : return ( WRC_NSM_LACP_ERROR_CONFIGURED );
		case NSM_LACP_ERROR_MAX_LAGS_REACHED      : return ( WRC_NSM_API_ERR_PORTCHANNEL_MAX_LAGS);
		case NSM_LACP_ERROR_SPEED_MISMATCH        : return ( WRC_NSM_LACP_ERROR_SPEED_MISMATCH);



        case NSM_ERR_PORT_PROFILE_PORT_NOT_ALLOWED:
			return WRC_NSM_ERR_DCM_PORT_PROFILE_PORT_NOT_ALLOWED;
		case SSM_NSM_INTERFACE_DOES_NOT_EXIST:
			return WRC_NSM_INTERFACE_DOES_NOT_EXIST;
		case SSM_NSM_INTERFACE_IS_DESTINATION_MIRROR_PORT:
			return WRC_NSM_INTERFACE_IS_DESTINATION_MIRROR_PORT;
		case SSM_NSM_INTERFACE_IS_NOT_PHYSICAL_PORT:
			return WRC_NSM_INTERFACE_IS_NOT_PHYSICAL_PORT;
		case SSM_NSM_INTERFACE_IS_NOT_PHYSICAL_OR_LAG_PORT:
			return WRC_NSM_INTERFACE_IS_NOT_PHYSICAL_OR_LAG_PORT;
		case SSM_NSM_INTERFACE_IS_NOT_ACTIVE:
			return WRC_NSM_INTERFACE_IS_NOT_ACTIVE;
		case SSM_NSM_INTERFACE_LAG_MEMBER_QOS_NOT_ALLOWED:
			return WRC_NSM_INTERFACE_LAG_MEMBER_QOS_NOT_ALLOWED;
		case SSM_QOS_TRUST_UNSUPPORTED:
			return WRC_SSM_QOS_TRUST_UNSUPPORTED;
		case SSM_QOS_NO_PFC_TC_MAP_STILL_ACTIVATED:
			return WRC_SSM_QOS_NO_PFC_TC_MAP_STILL_ACTIVATED;
		case SSM_NSM_INTERFACE_IS_NOT_PHYSICAL_PORT_OR_TRUNK:
			return WRC_NSM_INTERFACE_IS_NOT_PHYSICAL_PORT_OR_TRUNK;
		case SSM_QOS_MAP_NOT_FOUND:
			return WRC_SSM_QOS_MAP_NOT_FOUND;
		case SSM_NSM_INTERFACE_COSTOCOSMAP_ALREADY_APPLIED:
			return WRC_NSM_INTERFACE_COSTOCOSMAP_ALREADY_APPLIED;
		case SSM_NSM_INTERFACE_COSTOTRAFFICCLASSMAP_ALREADY_APPLIED:
			return WRC_NSM_INTERFACE_COSTOTRAFFICCLASSMAP_ALREADY_APPLIED;
		case SSM_NSM_INTERFACE_PAUSE_ALREADY_APPLIED:
			return WRC_NSM_INTERFACE_PAUSE_ALREADY_APPLIED;
		case SSM_QOS_ASYMMETRIC_PFC_NOT_SUPPORTED:
			return WRC_SSM_QOS_ASYMMETRIC_PFC_NOT_SUPPORTED;
		case SSM_CEE_MAP_EXIST_ON_INTERFACE:
			return WRC_SSM_CEE_MAP_EXIST_ON_INTERFACE;
		case SSM_QOS_COMMAND_NOT_SUPPORTED_IN_VCS_NON_CEE:
			return WRC_SSM_QOS_COMMAND_NOT_SUPPORTED_IN_VCS_NON_CEE;
		case SSM_NSM_ERR_APPM_PROFILED_PORT:
			return WRC_SSM_DCM_ERR_APPM_PROFILED_PORT;
		case SSM_NSM_INTERFACE_PFC_ALREADY_ON_INTERFACE:
			return WRC_NSM_INTERFACE_PFC_ALREADY_ON_INTERFACE;
		case WRC_NSM_SFLOW_SET_ERR_DESTINATION_MIRROR_PORT:
			return WRC_NSM_SFLOW_SET_ERR_DESTINATION_MIRROR_PORT;
		case WRC_NSM_SFLOW_SET_ERR_NOT_ENABLED:
			return WRC_NSM_SFLOW_SET_ERR_NOT_ENABLED;
		case 0:
			return WAVE_MESSAGE_SUCCESS;
		default:
			return err;
	}	
}	
ResourceId NsmUtils::wyserEaConfigurationErrorCode(WaveNs::PrismMessage* msg_p, ResourceId status)
{
	NSM_PLUG_DBG_TRACE();
	if(WAVE_MESSAGE_SUCCESS == status)
	{
		NSM_PLUG_DBG("sendSynchronously:success");
		if(WAVE_MESSAGE_SUCCESS != msg_p -> getCompletionStatus())
		{
			status = msg_p -> getCompletionStatus();
			NSM_PLUG_DBG_S(string("error from global plugin") + status);
		}
	}
	else 
	{
		NSM_PLUG_DBG_S(string("sendSynchronously:failed") + status);
	}
    
    if(msg_p != NULL)
	    delete msg_p;

	return status;
}	

InterfaceType NsmUtils::getInterfaceType(UI32 ifcapability)
{
	InterfaceType iftype;

	/*TODO: Need to fix this function based on the backend fix to capability*/
	if (ifcapability == IF_TYPE_GI) {
		iftype = IF_TYPE_GI;
	} else if (ifcapability == IF_TYPE_TE) {
		iftype = IF_TYPE_TE;
	} else if (ifcapability == IF_TYPE_FO) {
		iftype = IF_TYPE_FO;
	} else {
		iftype = IF_TYPE_INVALID;
	}

	return (iftype);
}

/*verify port-channel range based on VCS mode*/
bool NsmUtils::verifyPORange(char *argument)
{
	char *endp;
	unsigned int poid = 0;
	ResourceId vcs_status = DcmToolKit::isVcsEnabled ();

	poid = strtoul (argument, &endp, 10);
	if (*argument == '\0' || *endp != '\0')
		return false;

	if (WAVE_PERSISTENCE_CLUSTER_DISABLED == vcs_status) {
		if (poid < 1 || poid > DCM_PORTCHANNEL_MAX_LAGS)
			return false;
	} else {
		if (poid < 1 || poid > DCM_PORTCHANNEL_MAX_VLAGS)
			return false;
	}
	return true;
}

bool NsmUtils::isValidInterface(string ifName, InterfaceType ifType, LocationId loc)
{
	tracePrintf(TRACE_LEVEL_DEBUG, "NsmUtils::isValidInterface : ifName=%s, ifType=%u, locationId=%u", ifName.c_str(), ifType, loc);

	string className;
	UI32 count = 0;
	switch (ifType)
	{
		case IF_TYPE_GI:
		{
			className = OneGigLocalManagedObject::getClassName();
			break;
		}
		case IF_TYPE_TE:
		{
			className = TenGigLocalManagedObject::getClassName();
			break;
		}
		case IF_TYPE_FO:
		{
			className = FortyGigLocalManagedObject::getClassName();
			break;
		}
		case IF_TYPE_PO:
		{
			className = PoIntfManagedObject::getClassName();
			break;
		}
		case IF_TYPE_VL:
		{
			className = VlanIntfManagedObject::getClassName();
			break;
		}
		case IF_TYPE_VE:
		{
			className = NsmIntfSviManagedObject::getClassName();
			break;
		}
		case IF_TYPE_INTF_LOOPBACK:
		{
			className = NsmIntfLoopbackManagedObject::getClassName();
			break;
		}
        case IF_TYPE_FI:
        {
            className = PortFCCfgLocalManagedObject::getClassName ();
            break;
        }		
		default:
		{
			return false;
		}
	}

	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
	if (ifType == IF_TYPE_GI || ifType == IF_TYPE_TE || ifType == IF_TYPE_FO)
	{
    	syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifname"));
    	if (loc != 0) {
    	    syncQueryCtxt.setLocationIdFilter(loc);
    	}
	}
	else if (ifType == IF_TYPE_PO || ifType == IF_TYPE_VL)
	{
	  	UI32 id = strtoul(ifName.c_str (), NULL, 0);
	  	syncQueryCtxt.addAndAttribute (new AttributeUI32(&id,"id"));
	}
	else if (ifType == IF_TYPE_VE)
	{
	  	UI32 id = strtoul(ifName.c_str (), NULL, 0);
	  	syncQueryCtxt.addAndAttribute (new AttributeUI32(&id,"vlanId"));

	  	if (loc != 0) {
	  	  syncQueryCtxt.setLocationIdFilter(loc);
	  	}
	}
	else if (ifType == IF_TYPE_INTF_LOOPBACK)
	{
	  	UI32 id = strtoul(ifName.c_str (), NULL, 0);
	  	syncQueryCtxt.addAndAttribute (new AttributeUI32(&id,"ifId"));

	  	if (loc != 0) {
	  	  syncQueryCtxt.setLocationIdFilter(loc);
	  	}
	}
    else if (ifType == IF_TYPE_FI)
    {
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"id"));
    }

    ResourceId queryStatus = querySynchronouslyForCount (&syncQueryCtxt, count);
	return (queryStatus == WAVE_MESSAGE_SUCCESS) && (count == 1);
}

bool NsmUtils::checkLagFcoeQosCompatibility(PoIntfManagedObject *pPoIntfManagedObject , ResourceId &status)
{
	UI32 i = 0;

	if (!pPoIntfManagedObject)
	{
		status = WRC_WAVE_MESSAGE_SUCCESS;
    	trace(TRACE_LEVEL_DEBUG, string("pPoIntfManagedObject is NULL"));
		return false;
	}

	// If the port-channel has cee or non-cee configuration, return error.
	if (QosUtils::isInterfaceQosCeeConfigured(pPoIntfManagedObject) ||
		QosUtils::isInterfaceQosNonCeeConfigured(pPoIntfManagedObject))
	{
		status = WRC_SSM_INTF_QOS_ON_LAG_IS_ALREADY_APPLIED;
    	trace(TRACE_LEVEL_DEBUG, string("Qos on LAG already applied"));
		return false;
	} else if (QosUtils::isInterfaceMQCConfigured(pPoIntfManagedObject)) {
		status = WRC_SSM_QOS_FLOWBASED_SERVICE_POLICY_AND_FCOE_CANNOT_CO_EXIST;
		return false;
	}

	ObjectId poObjectId, nullObjectId;

	poObjectId = pPoIntfManagedObject->getObjectId(); 
	nullObjectId = ObjectId::NullObjectId;

	// If any of the port-channel member has cee-map enabled, return error 
	PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(poObjectId, "poId", PhyIntfLocalManagedObject::getClassName()));
    syncQueryCtxt.addSelectField("poId");
    syncQueryCtxt.addSelectField("ceeMap");
	vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	if (NULL != pResults) {
		UI32 numberOfResults = pResults->size ();
		if (0 == numberOfResults) {
			trace (TRACE_LEVEL_DEBUG, "checkLagFcoeQosCompatibility no port-channel member");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			status = WRC_WAVE_MESSAGE_SUCCESS;
			return true;
		} else {
			for (i=0; i < numberOfResults; i++)
			{
				pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[i]); 
				if (pPhyIntfLocalManagedObject->getCeeMap() != ObjectId::NullObjectId)
				{
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
					status = WRC_SSM_INTF_QOS_ON_LAG_MEMBER_IS_ALREADY_APPLIED;
					return false;
				}
			}
		}
    }
	else 
    {
		status = WRC_WAVE_MESSAGE_SUCCESS;
		return true;
	}

	if (pResults)
	{
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}
	return true;
}

bool NsmUtils::checkPhyPortFcoeQosCompatibility(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject, ResourceId &status)
{
    trace(TRACE_LEVEL_DEBUG, string("inside isPhyPortFcoeConfigurable"));
	if (!pPhyIntfLocalManagedObject)
	{
		status = WRC_WAVE_MESSAGE_SUCCESS;
    	trace(TRACE_LEVEL_DEBUG, string("inside isPhyPortFcoeConfigurable null false"));
		return false;
	}

    /* If any qos configuration(cee or non-cee) is applied on the Physical interface, return false */
	if (QosUtils::isInterfaceQosCeeConfigured(pPhyIntfLocalManagedObject) ||
		QosUtils::isInterfaceQosNonCeeConfigured (pPhyIntfLocalManagedObject))
	{
    	trace(TRACE_LEVEL_DEBUG, string("inside isPhyPortFcoeConfigurable phy config conflict"));
		status = WRC_FCOE_PORT_ERR_QOS_CONFIG_CONFLICT;   /* Update with proper error message */
		return false;
	} else if (QosUtils::isInterfaceMQCConfigured(pPhyIntfLocalManagedObject)) {
		status = WRC_SSM_QOS_FLOWBASED_SERVICE_POLICY_AND_FCOE_CANNOT_CO_EXIST;
		return false;
	}

    /* If any qos configuration(cee or non-cee) is applied on Port channel, return false */
	if(pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId)
	{
		PoIntfManagedObject *pPoIntfManagedObject = NULL;
		pPoIntfManagedObject = NsmUtils:: getPoIntfObjectByObjectId(pPhyIntfLocalManagedObject->getPoId());
		if (QosUtils::isInterfaceQosCeeConfigured(pPoIntfManagedObject) ||
			QosUtils::isInterfaceQosNonCeeConfigured (pPoIntfManagedObject))
		{
    		trace(TRACE_LEVEL_DEBUG, string("inside isPhyPortFcoeConfigurable PO  config conflict"));
			status = WRC_SSM_INTF_QOS_ON_LAG_IS_ALREADY_APPLIED;  /*Update with proper error message */
            delete pPoIntfManagedObject;
			return false;
		}
		if (pPoIntfManagedObject) { 
			delete pPoIntfManagedObject;
		}

	}
    trace(TRACE_LEVEL_DEBUG, string("inside isPhyPortFcoeConfigurable returning true"));

	status = WRC_WAVE_MESSAGE_SUCCESS;
	return true;
}

/*This function should load only fcoe composition*/

bool NsmUtils::isFcoeConfiguredForLagPort(const UI32 &po_id)
{
    PoIntfManagedObject   *if_mo_p;
    WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> fcoeport_mo_p = NULL;
    UI32 id = po_id;
 
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeUI32(&id,"id"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
   
    if (pResults == NULL) {
        return false;
    }

    if (pResults->size() == 0) {
        return false;
    }

    if_mo_p = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
    if (if_mo_p == NULL) {
        return false;
    }
    
    fcoeport_mo_p = if_mo_p->getFcoeport();
    bool retVal = false;

    if (fcoeport_mo_p.operator ->() == NULL) {
        retVal = false;
    } else {
        retVal = true;
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    return retVal; 
}

bool
NsmUtils::isFcoeConfiguredForAnyPhysicalPort(const UI32 &loc_id)
{
    PhyIntfLocalManagedObject   *if_mo_p;
    WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> fcoeport_mo_p = NULL;
    UI32        i = 0;
    bool        allowed = false;
 
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
    if (loc_id != 0) {
        // Add location id filter to the context 
        syncQueryCtxt.setLocationIdFilter(loc_id);
    }

    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    
    if (pResults == NULL) {
        return allowed;
    }

    for (i = 0; i < pResults->size (); i++) {
        if_mo_p = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[i]);
        fcoeport_mo_p = if_mo_p->getFcoeport();

        if (fcoeport_mo_p.operator ->() == NULL) {
            continue;
        } else {
            allowed = true;
            break;
        }
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    return allowed;
}

bool
NsmUtils::isFcoeConfiguredForAnyLagPort(const UI32 &loc_id)
{
    PoIntfManagedObject *if_mo_p;
    WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> fcoeport_mo_p = NULL;
    UI32        i = 0;
    bool        allowed = false;
 
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
    if (loc_id != 0) {
        // Add location id filter to the context 
        syncQueryCtxt.setLocationIdFilter(loc_id);
    }

    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
    if (pResults == NULL) {
        return allowed;
    }

    for (i = 0; i < pResults->size (); i++) {
        if_mo_p = dynamic_cast<PoIntfManagedObject *>((*pResults)[i]);
        fcoeport_mo_p = if_mo_p->getFcoeport();

        if (fcoeport_mo_p.operator ->() == NULL) {
            continue;
        } else {
            allowed = true;
            break;
        }
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    return allowed;
}

bool NsmUtils::isFcoeConfiguredForEthPort(const UI32 &loc_id)
{
    if (NsmUtils::isFcoeConfiguredForAnyPhysicalPort(loc_id)) {
        return true;
    }

    if (NsmUtils::isFcoeConfiguredForAnyLagPort(loc_id)) {
        return true;
    }
    
    return false;
}

bool
    NsmUtils::isFsbEnabled()
    {
        bool fsb_mode = false;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(FcoeGlobalFsbManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL == pResults) {
            NSM_PLUG_DBG_S(string("Query returned NULL results"));
            return false;
        }

        FcoeGlobalFsbManagedObject *fsb_mo_p = NULL;

        UI32 numberOfResults = pResults->size ();
        if (numberOfResults == 0 || numberOfResults > 1) {
            NSM_PLUG_DBG_S(string("Query returned 0 or >1 results, MO not found"));
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            return false;
        }

        fsb_mo_p = dynamic_cast<FcoeGlobalFsbManagedObject *> ((*pResults)[0]);
        if (fsb_mo_p == NULL) {
            return false;
        }

        fsb_mode = fsb_mo_p->getMode();

        pResults->clear ();
        delete pResults;
        delete fsb_mo_p;

        return fsb_mode;
    }



/*This Function passing pointer back need to revisit this function
 * for optimization purpose*/
    void NsmUtils::SlotGetPhyIntfMOs (vector<PhyIntfLocalManagedObject*>& mos,
        int32_t slotId, LocationId locId)
    {
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());

        if (locId != 0) syncQueryCtxt.setLocationIdFilter(locId); 

        if (slotId != 0) syncQueryCtxt.setSlotFilter(slotId);        
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            for (UI32 i = 0; i < numberOfResults; ++i) {
                    mos.push_back(dynamic_cast
                        <PhyIntfLocalManagedObject *> ((*pResults)[i]));
            }

            delete pResults;
        }

        return;
    }

/*This Function passing pointer back need to revisit this function
 * for optimization purpose*/
    void NsmUtils::GetPoIntfMOs (vector<PoIntfManagedObject*>& mos) 
    {
        vector<WaveManagedObject *> *pResults;

        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(PoIntfManagedObject::getClassName());
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            for (UI32 i = 0; i < numberOfResults; ++i) {
                    mos.push_back(dynamic_cast
                        <PoIntfManagedObject *> ((*pResults)[i]));
            }

            delete pResults;
        }

        return;
    }

    InterfaceType NsmUtils::getIntfTypeFromName(const string &name)
    {
        string strtype = "";
        char  stype[3];

        strtype = name.substr(0, 2);
        strncpy(stype, strtype.c_str(), 2);
        stype[2] = '\0';
        
        if (isdigit(stype[0])) return IF_TYPE_PHY; // OLD STYLE

        if (strcasecmp(stype, "gi") == 0) return IF_TYPE_GI;
        if (strcasecmp(stype, "te") == 0) return IF_TYPE_TE;
        if (strcasecmp(stype, "fo") == 0) return IF_TYPE_FO;
        if (strcasecmp(stype, "fe") == 0) return IF_TYPE_FO;
        // Temporarily support both "fo" and "fe" because there is confusion.

        return IF_TYPE_INVALID;
    }

    ResourceId NsmUtils::NsmBackendErrorMapToDcmStatus(int err)
    {
        switch (err) {
            case 0:
                 return WAVE_MESSAGE_SUCCESS;

            case  DCM_NSM_API_SET_ERR_NO_SUCH_INTERFACE:
                return WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE;

            case DCM_NSM_API_ERR_IP_INVLD_IF:
                return WRC_NSM_API_ERR_IP_INVLD_IF;

            case DCM_NSM_API_ERR_FCF_ON_IP_VLAN:
                return WRC_NSM_API_ERR_FCF_ON_IP_VLAN;

            case DCM_NSM_API_ERR_IP_INVLD_HOST:
                return WRC_NSM_API_ERR_IP_INVLD_HOST;

            case DCM_NSM_API_ERR_IP_INVLD_MASK:
                return WRC_NSM_API_ERR_IP_INVLD_MASK;

            case  DCM_NSM_API_ERR_IP_OVERLAP_MANAGE:
                return WRC_NSM_API_ERR_IP_OVERLAP_MANAGE;

            case  DCM_NSM_API_ERR_IP_OVERLAP_STATIC:
                return WRC_NSM_API_ERR_IP_OVERLAP_STATIC;

            case  DCM_NSM_API_ERR_SVI_CREAT:
                return WRC_NSM_API_ERR_SVI_CREAT;

            case  DCM_NSM_API_ERR_UPDATE_MAC_IN_HW:
                return WRC_NSM_API_ERR_UPDATE_MAC_IN_HW;

            case  DCM_NSM_API_ERR_ROUTERPORT_CREATE:
                return WRC_NSM_API_ERR_ROUTERPORT_CREATE;

            case DCM_NSM_API_SET_ERR_SECONDARY_IP_ADDR_EXISTS:
                return WRC_NSM_API_SET_ERR_SECONDARY_IP_ADDR_EXISTS;

            case DCM_NSM_API_ERR_ROUTERPORT_DELETE:
                return WRC_NSM_API_ERR_ROUTERPORT_DELETE;

            case  DCM_NSM_ERR_DCM_INTERNAL_INTERFACE:
                return WRC_NSM_ERR_DCM_INTERNAL_INTERFACE;

            case DCM_NSM_API_SET_ERR_MASTER_NOT_EXIST:
                return WRC_NSM_API_SET_ERR_MASTER_NOT_EXIST;

            case DCM_NSM_API_SET_ERR_INVALID_IPV4_ADDRESS:
                return WRC_NSM_API_SET_ERR_INVALID_IPV4_ADDRESS;

            case DCM_NSM_API_SET_ERR_CANT_SET_ADDRESS_ON_P2P:
                return WRC_NSM_API_SET_ERR_CANT_SET_ADDRESS_ON_P2P;

            case DCM_NSM_API_SET_ERR_CANT_SET_SECONDARY_FIRST:
                return WRC_NSM_API_SET_ERR_CANT_SET_SECONDARY_FIRST;

            case DCM_NSM_API_SET_ERR_CANT_CHANGE_PRIMARY:
                return WRC_NSM_API_SET_ERR_CANT_CHANGE_PRIMARY;

            case DCM_NSM_API_ERR_AGG_L3:
                return WRC_NSM_API_ERR_AGG_L3;

            case DCM_NSM_API_ERR_L2_L3:
                return WRC_NSM_API_ERR_L2_L3;

            case DCM_NSM_API_ERR_SPAN_L3:
                return WRC_NSM_API_ERR_SPAN_L3;
			case DCM_NSM_API_ERR_DEFAULT_VE_NOT_ALLOWED:
				return WRC_NSM_API_ERR_DEFAULT_VE_NOT_ALLOWED;

            case DCM_NSM_API_SET_ERR_INVALID_PORT_ROLE_MODE:
                return WRC_NSM_VRF_ISL_ERROR;    

            case DCM_NSM_API_VRF_SET_INVLD_CONFIG_FOR_AGG_MEM:
                return WRC_VRF_SET_INVLD_CONFIG_FOR_AGG_MEM;    

            // Gradually stopping using DCM_NSM~ error code
            // Use NSM_API_ERR for backend
            // Use WRC_NSM_ERR for DCMD
            // dcm_map() does the conversion.
            // At last, stop using this function.
            case WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR:
            case WRC_NSM_API_SET_ERR_NO_MTU_USED_LIMIT:
            case WRC_NSM_ERR_NETWORK_ADDRESS_AS_HOST:
            case WRC_NSM_ERR_BROADCAST_ADDRESS_AS_HOST:
            case WRC_NSM_ERR_IP_ADDR_MASK31_AS_SECONDARY:
			case WRC_NSM_API_SET_ERR_MAX_ADDRESS_LIMIT:
			case WRC_NSM_API_SET_ERR_REAL_INTERFACE_ACT:
			case WRC_NSM_API_SET_ERR_NOT_SECONDARY_IP_ADDR:
			case WRC_NSM_API_SET_ERR_NOT_PRIMARY_IP_ADDR:
			case WRC_NSM_API_SET_ERR_ADDRESS_NOT_EXIST:
			case WRC_NSM_API_SET_ERR_ADDRESS_OVERLAPPED:
			case WRC_NSM_API_ERR_IP_OVERLAP:
			case WRC_NSM_API_SET_ERR_CANT_CHANGE_VIRT_TO_SECONDARY:
			case WRC_NSM_API_SET_ERR_CANT_CHANGE_VIRT_TO_PRIMARY:
                return err;

            default: 
                return WRC_NSM_API_ERR_GENERIC;
        }
        return WRC_NSM_API_ERR_GENERIC;
    }

    ResourceId NsmUtils::ArpBackendErrorMapToDcmStatus(int err)
    {
        switch (err) {
            case 0:
                 return WAVE_MESSAGE_SUCCESS;

            case ARP_MEMORY_ALLOCATION_FAILED:
                return WRC_ARP_MEMORY_ALLOCATION_FAILED;

            case ARP_DCM_API_MSG_ERROR:
                return WRC_ARP_DCM_API_MSG_ERROR;

            case ARP_DCM_LINUX_FILEDOESNOT_EXIST:
                return WRC_ARP_DCM_LINUX_FILEDOESNOT_EXIST;

            case ARP_L3_COMMANDS_NOT_ALLOWED_ON_L2_INTERFACE:
                return WRC_ARP_L3_COMMANDS_NOT_ALLOWED_ON_L2_INTERFACE;

            case ARP_L3_COMMANDS_NOT_ALLOWED_ON_PO_INTERFACE:
                return  WRC_ARP_L3_COMMANDS_NOT_ALLOWED_ON_PO_INTERFACE;

            case ARP_INTERFACE_DOES_NOT_EXIST:
                return WRC_ARP_INTERFACE_DOES_NOT_EXIST;

            case ARP_UNKNOWN_INTERNAL_ERROR:
                return WRC_ARP_UNKNOWN_INTERNAL_ERROR;

            case ARP_INVALID_IF_TYPE_ERROR:
                return WRC_ARP_INVALID_IF_TYPE_ERROR;

            default :
                return WRC_NSM_API_ERR_GENERIC;
        }
        return WRC_NSM_API_ERR_GENERIC;
    }

    UI32 NsmUtils::getClientOpCode(UI32 cmd, UI32 op)
    {
        UI32 opcode = NSM_MSG_CMD_CODE;

        switch (cmd)
        {
            case NSM_MSG_CMD_NODE_VLAN:
            {
                if (op != NSM_MSG_OP_DELETE) {
                     opcode = NSM_MSG_VE_SET;
                }
                else {
                    opcode = NSM_MSG_VE_UNSET;
                }
                break;
            }
            case NSM_MSG_CMD_IP_ADDRESS:
            {
                if (op != NSM_MSG_OP_DELETE) {
                     opcode = NSM_MSG_SEC_IP_ADDRESS;
                }
                else {
                    opcode = NSM_MSG_NO_SEC_IP_ADDRESS;
                }
                break;

           }
            case NSM_MSG_CMD_ALL_IPADDR:
            {
                if (op == NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_NO_SEC_IP_ADDRESS_ALL;
                }
                break;
            }
            case NSM_MSG_CMD_MTU:
            {
                if (op != NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_IP_MTU;
                }
                else {
                    opcode = NSM_MSG_NO_IP_MTU;
                }
                break;
            }
            case NSM_MSG_CMD_SWITCHPORT:
            {
                if (op != NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_IP_SWITCHPORT;
                }
                else {
                    opcode = NSM_MSG_NO_IP_SWITCHPORT;
                }
                break;
            }
            case NSM_MSG_CMD_PROXY_ARP:
            {
                if (op != NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_IP_PROXY_ARP;
                }
                else {
                    opcode = NSM_MSG_NO_IP_PROXY_ARP;
                }
                break;
            }
            case NSM_MSG_CMD_ARP_AGING_TIMEOUT:
            {
                if (op != NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_ARP_AGEING_TIMEOUT;
                }
                else {
                    opcode = NSM_MSG_NO_ARP_AGEING_TIMEOUT;
                }
                break;
            }
            case NSM_MSG_CMD_INTF_LOOPBACK:
            {
                if (op != NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_INTF_LOOPBACK_SET;
                }
                else {
                    opcode = NSM_MSG_INTF_LOOPBACK_UNSET;
                }
                break;
            }
            case NSM_MSG_CMD_INTF_STATUS:
            {
                if (op != NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_INTF_STATUS;
                }
                else {
                    opcode = NSM_MSG_INTF_NO_STATUS;
                }
                break;
            }
            case NSM_MSG_CMD_INTF_SHUTDOWN:
            {
                if (op != NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_INTF_SHUTDOWN;
                }
                else {
                    opcode = NSM_MSG_INTF_NO_SHUTDOWN;
                }
                break;
            }
            case NSM_MSG_CMD_IP_DIRECTED_BROADCAST:
            {
                if (op != NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_IP_DIRECTED_BROADCAST;
                }
                else {
                    opcode = NSM_MSG_NO_IP_DIRECTED_BROADCAST;
                }
                break;
            }
            case NSM_MSG_CMD_L3READY_MO:
            {
                if (op != NSM_MSG_OP_DELETE) {
                    opcode = NSM_MSG_L3READY_MO;
                }
                else {
                    opcode = NSM_MSG_NO_L3READY_MO;
                }
            }    
        }
        return opcode;
    }

    void NsmUtils::setAllCompositionfalse(WaveManagedObjectSynchronousQueryContext& syncQueryCtxt)
    {
	    syncQueryCtxt.setLoadOneToManyRelationships(false);
	    syncQueryCtxt.setLoadCompositions(false);
    }
    inline void NsmUtils::loadOnlySelectedField(WaveManagedObjectSynchronousQueryContext& syncQueryCtxt, const char *field, bool isSelectedFieldOid)
    {
	    if(isSelectedFieldOid)
		{
			syncQueryCtxt.addSelectFieldForObjectId(field);
			return;
		}
		syncQueryCtxt.addSelectField (field);
	}

    bool NsmUtils::isVlanPartOfActiveProfile(UI32 vlanId, bool &isChangeVlanList)
        	{
    	/*Validate if it is ok to go ahead with vlan deletion.
    	  We will check if the vlan is a part of any activated profile.
    	  If yes, we do not allow vlan deletion.*/

    	WaveManagedObjectSynchronousQueryContext vlanSyncQueryCtxt(VlanProfileCfgManagedObject::getClassName());
    	vlanSyncQueryCtxt.addAndAttribute(new AttributeString("default", "description"),WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);
    	vlanSyncQueryCtxt.addAndAttribute(new AttributeBool(false, "isAllowedVlanNone"));

    	vector<WaveManagedObject *> *pVlanResults = querySynchronously (&vlanSyncQueryCtxt);
    	VlanProfileCfgManagedObject *pVlanProfileCfgManagedObject = NULL;
    	isChangeVlanList = false;
    	bool changeVlanList = false;

    	if (NULL != pVlanResults)
    	{
    		UI32 numberOfResults = pVlanResults->size ();
    		if (0 == numberOfResults) {
    			APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG," No MO in VlanProfileCfgManagedObject found");
    		}else {
    			UI32 count = 0;

    			APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG,
    				string(" numberOfResults for VlanProfileCfgManagedObject = ") +
    				numberOfResults);

    			while(count != numberOfResults)
    			{
    				changeVlanList = false;
    				pVlanProfileCfgManagedObject = dynamic_cast<VlanProfileCfgManagedObject *>((*pVlanResults)[count]);

    				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG,
    					string(" count for VlanProfileCfgManagedObject = ") + count);

    				APPM_PLUG_TRACE(TRACE_LEVEL_DEBUG,
    					string(" for VlanProfileCfgManagedObject name = ")
    					+ pVlanProfileCfgManagedObject->getDescription());

    				/*string profName("\0");

    				profName = pVlanProfileCfgManagedObject->getDescription();
    				We do not make any changes to the default profile

    				if (profName.compare("default") == 0)
    				{
    					count++;
    					continue;
    				}*/

    				//if(!(pVlanProfileCfgManagedObject->getIsAllowedVlanNone()))
    				//{
    					if(pVlanProfileCfgManagedObject->getIsAllowedVlanAllV2())
    					{
    						vector<UI32>::iterator moremoveiter;
    						UI32Range moRemoveVlanrange = UI32Range("");
    						vector <UI32> moRemoveVlanRangeVector;
    						moRemoveVlanrange = pVlanProfileCfgManagedObject->getVlanRangeRemove();
    						moRemoveVlanrange.getUI32RangeVector(moRemoveVlanRangeVector);

    						for (moremoveiter = moRemoveVlanRangeVector.begin(); moremoveiter != moRemoveVlanRangeVector.end();
    							moremoveiter++)
    						{
    							if(*moremoveiter == vlanId)
    							{
    								isChangeVlanList = true;
    								changeVlanList = true;
    								break;
    							}
    						}
    					}
    					else
    					{/*Search if it is a part of trunk or access vlan*/
    						if(pVlanProfileCfgManagedObject->getSwMode() == VLAN_SW_MODE_ACCESS)
    						{
    							if(pVlanProfileCfgManagedObject->getAccessVlan() == vlanId)
    							{
    								changeVlanList = true;
    								isChangeVlanList = true;
    							}
                                if(pVlanProfileCfgManagedObject->doesVlanContainMacClassification(vlanId)) {
                                    changeVlanList = true;
                                    isChangeVlanList = true;
                                }
    						}
    						else if (pVlanProfileCfgManagedObject->getSwMode() == VLAN_SW_MODE_TRUNK)
    						{
    							WaveManagedObjectPointer<APPMNativeVlanManagedObject> pNativeVlanMo =
    								pVlanProfileCfgManagedObject->getNativeVlanIntfConfig();

    							APPMNativeVlanManagedObject *nativeTmp = pNativeVlanMo.operator-> ();
    							if(nativeTmp->getNativeVlanId() == vlanId)
    							{
    								changeVlanList = true;
    								isChangeVlanList = true;
    							}
    							else
    							{
    								vector<UI32>::iterator moadditer;
    								UI32Range moAddVlanrange = UI32Range("");
    								vector <UI32> moAddVlanRangeVector;
    								moAddVlanrange = pVlanProfileCfgManagedObject->getVlanRangeAdd();
    								moAddVlanrange.getUI32RangeVector(moAddVlanRangeVector);

    								for (moadditer = moAddVlanRangeVector.begin(); moadditer != moAddVlanRangeVector.end(); moadditer++)
    								{
    									if(*moadditer == vlanId)
    									{
    										isChangeVlanList = true;
    										changeVlanList = true;
    										break;
    									}
                                    }
                                    if(pVlanProfileCfgManagedObject->doesVlanContainCtagClassification(vlanId)) {
                                        changeVlanList = true;
                                        isChangeVlanList = true;
                                        break;
                                    }
    							}
    						}
    					}

    					if(changeVlanList)
    					{
    						APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " changeVlanList is true!!");

    						WaveManagedObjectSynchronousQueryContext profileSyncQueryCtxt(ProfileManagedObject::getClassName());
    						profileSyncQueryCtxt.addSelectField ("isActivated");

    						profileSyncQueryCtxt.addAndAttribute (new AttributeString
    							(pVlanProfileCfgManagedObject->getDescription(), "profileName"));
    						vector<WaveManagedObject *> *pProfileResults = querySynchronously (&profileSyncQueryCtxt);

    						ProfileManagedObject *pProfileManagedObject = NULL;

    						if (NULL != pProfileResults)
    						{
    							UI32 numberOfPPResults = pProfileResults->size ();
    							if (0 == numberOfPPResults) {
    								APPM_PLUG_TRACE(TRACE_LEVEL_ERROR," cannot find Port-profile Managed object");
    							}else if (1 == numberOfPPResults) {
    								pProfileManagedObject = dynamic_cast<ProfileManagedObject *>((*pProfileResults)[0]);
    								APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Found Port-profile Managed Object");
    							} else {
    								APPM_PLUG_TRACE(TRACE_LEVEL_ERROR," found Two or more objects exists");}

    							if(pProfileManagedObject->getIsActivated())
    							{
    								APPM_PLUG_TRACE(TRACE_LEVEL_ERROR," cannot delete this vlan, an activated port-profile exists with this vlan");
    								isChangeVlanList = true;
    								WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pProfileResults);
    								return (true);
    							}
    						}
    						if(NULL != pProfileResults)
    							WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pProfileResults);
    					}
    				//}
    				count++;
    			}
    		}
    		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pVlanResults);
    	}
    	return(false);
    }


    bool NsmUtils::isRemoteSpanVlan (UI32 vlanId) {
       NSM_PLUG_DBG_S(string("Entering: Vlan Id String:")+vlanId);
	   bool remoteSpanVlan = false;
	   bool RSPAN_ENABLED = true;
	   WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
	   syncQueryCtxt.addAndAttribute (new AttributeUI32 (&vlanId, "id"));
	   syncQueryCtxt.addAndAttribute(new AttributeBool(RSPAN_ENABLED, "remoteSpanEnabled"));
	   UI32 rspanVlanCount = 0;
	   ResourceId queryStatus = querySynchronouslyForCount(&syncQueryCtxt, rspanVlanCount);
	   if(WAVE_MESSAGE_SUCCESS == queryStatus) {
		  if (rspanVlanCount > 0 ) {
				NSM_PLUG_INFO_S(string("RSPAN Configured. No Other config permitted on Vlan ")+vlanId);
				remoteSpanVlan = true;
		  }
	   }
	   else {
		  NSM_PLUG_DBG_ERR_S(string("Failed to query the existing rspan session!!"));
	   }
	   NSM_PLUG_DBG_S(string("Exiting with returnValue=")+remoteSpanVlan);
	   return remoteSpanVlan;
    }

    vector<WaveManagedObject*>* NsmUtils::queryVlanMo (vector<UI32> &vids, const string &selectField)
    {
        vector<string> v;
        if (!selectField.empty()) v.push_back(selectField);
        return queryVlanMo(vids, v);
    }

    vector<WaveManagedObject*>* NsmUtils::queryVlanMo (vector<UI32> &vids, vector<string> &selectFields)
    {
        vector<Attribute*> vidAttrs;
        for (UI32 i = 0; i < vids.size(); ++i) {
            vidAttrs.push_back(new AttributeUI32(vids[i], "id"));
        }

        WaveManagedObjectSynchronousQueryContext qc (VlanIntfManagedObject::getClassName());
        qc.addAndAttribute(new AttributeSqlIn(vidAttrs), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
        if (!selectFields.empty()) {
            qc.addSelectFields(selectFields);
        }

        vector<WaveManagedObject*> *mos = querySynchronously(&qc);
        return mos == NULL ? new vector<WaveManagedObject*>() : mos;
    }

    bool NsmUtils::isVlanPartOfRspanSession(UI32 iVlanId) {
    	//ResourceId status = WAVE_MESSAGE_SUCCESS;
    	bool vlanPartOfRspanSession = false;
		NSM_PLUG_DBG_S(string("Vlan ID")+iVlanId);
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute(new AttributeUI32(&iVlanId, "id"));
		syncQueryCtxt.addSelectField("remoteSpanEnabled");

		vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
		VlanIntfManagedObject *vlanMo = NULL;
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size();
			if (numberOfResults == 0 || numberOfResults > 1) {
				NSM_PLUG_DBG_S(string("Query returned 0 or >1 results, MO not found"));
				vlanMo = NULL;
			} else {
				vlanMo = dynamic_cast<VlanIntfManagedObject*>((*pResults)[0]);
			}
		}
		if (vlanMo) {
			if (true == vlanMo->getRemoteSpanEnabled()) {
				NSM_PLUG_DBG_S(string("The vlan ")+ iVlanId+" is a remote-span vlan");
				//Check if there is any RSPAN config with this remote-span vlan
				WaveManagedObjectSynchronousQueryContext spanQueryCtxt(SpanGlobalConfigManagedObject::getClassName());
				spanQueryCtxt.addAndAttribute(new AttributeUI32(&iVlanId, "destVlanId"));

				UI32 numberOfRspanSessions = 0;
				ResourceId queryStatus = querySynchronouslyForCount(&spanQueryCtxt, numberOfRspanSessions);
				if(WAVE_MESSAGE_SUCCESS == queryStatus) {
					NSM_PLUG_DBG_S(string("Number of span session with the RSPAN Vlan:")+numberOfRspanSessions);
					if (numberOfRspanSessions > 0 ) {
						NSM_PLUG_DBG_S(string("Vlan ")+iVlanId+" cannot be deleted");
						//status = WRC_NSM_RSPAN_VLAN_PARTOF_ACTIVE_RSPAN_CONFIG_CANNOT_BE_DELETED;
						vlanPartOfRspanSession = true;
					}
				}
				else {
					NSM_PLUG_DBG_S(string("Failed to query the existing rspan session!!"));
				}
			}
		}
		if (NULL != pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
    	NSM_PLUG_DBG_S(string("Exiting with status:")+vlanPartOfRspanSession);
    	return vlanPartOfRspanSession;
    }

    bool NsmUtils::isIgmpSnoopingConfiguredOnVlan(UI32 vlanId) {
    	bool isIgmpsConfigured = false;
		UI32 vlanCount = 0;
		bool enableFlag = true;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsVlanIntfConfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute(new AttributeUI32(vlanId, "vlanId"));
		syncQueryCtxt.addAndAttribute(new AttributeBool(enableFlag, "enable"));
		WaveObjectManagerToolKit::querySynchronouslyForCount(&syncQueryCtxt, vlanCount);
		if(vlanCount) {
			NSM_PLUG_DBG_S(string("IGMP Snooping configured on the vlan"));
			isIgmpsConfigured = true;
		}
    	return isIgmpsConfigured;
    }

    bool NsmUtils::isInterfaceInService(const string ifName) {
    	NSM_PLUG_DBG_S(string("Entering: Interface Name = ")+ifName);
    	bool isInterfaceInService = true;
		UI32 ifcCount = 0;
		bool activeFlag = true;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute(new AttributeString(ifName, "id"));
		syncQueryCtxt.addAndAttribute(new AttributeBool(activeFlag, "active"));
		WaveObjectManagerToolKit::querySynchronouslyForCount(&syncQueryCtxt, ifcCount);
		if(ifcCount != 1) {
			NSM_PLUG_DBG_ERR_S(string("Could not find an active interface ")+ifName);
			isInterfaceInService = false;
		}
		NSM_PLUG_DBG_S(string("Exiting with status = ")+isInterfaceInService);
    	return isInterfaceInService;
    }

    UI32Range NsmUtils::getRspanVlanUI32Range (PrismSynchronousLinearSequencerContext *pContext) {
       NSM_PLUG_DBG_S(string("Entering: "));
       UI32Range rspanVlans("");
	   bool RSPAN_ENABLED = true;
	   WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
	   syncQueryCtxt.addSelectField("remoteSpanEnabled");
	   syncQueryCtxt.addSelectField("id");
	   syncQueryCtxt.addAndAttribute(new AttributeBool(RSPAN_ENABLED, "remoteSpanEnabled"));
	   vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
	   VlanIntfManagedObject *vlanMo = NULL;
	   if (NULL != pResults) {
		    unsigned int numberOfResults = pResults->size();
		    NSM_PLUG_INFO_S(string("Number of rspan-vlans configured=")+numberOfResults);
		    if (numberOfResults == 0) {
			   NSM_PLUG_INFO_S(string("No rspan-vlan configured"));
		    }
		    else {
			    for(unsigned int i=0;i<numberOfResults;i++) {
				  vlanMo = dynamic_cast<VlanIntfManagedObject*>((*pResults)[i]);
				  UI32 vlanId = vlanMo->getId();
				  NSM_PLUG_DBG_S(string("Vlan ")+vlanId+" is rspan-vlan");
				  rspanVlans = rspanVlans + UI32Range(vlanId,vlanId);
				}
			    NSM_PLUG_INFO_S(string("Configured rspan-vlan UI32Range=")+rspanVlans.toString());
		    }
		    pContext->addManagedObjectsForGarbageCollection(*pResults);
	   }
	   return rspanVlans;
    }

    void NsmUtils::getPoMemberIfindexListForLocalNode(const string  &PoName, vector<UI64> &ifx_list)
    {

        ObjectId PoOid;
        SI32 MappedId = -1;
        LocationId locationId = 0;
        UI64 ifindex = 0;
        vector<PhyIntfLocalManagedObject *> vpPhyIntfLocalManagedObject;
        getPoIntfObjectIdByPoIfName(PoName, PoOid, false);
        GetPhyMoByPoOid(PoOid,  vpPhyIntfLocalManagedObject);
        for(unsigned int i = 0;i< vpPhyIntfLocalManagedObject.size(); i++)
        {
            ifindex = vpPhyIntfLocalManagedObject[i]->getIfIndex();
            MappedId = getMappedIdFromIfName(vpPhyIntfLocalManagedObject[i]->getIfName());
            locationId = DcmToolKit::getLocationIdFromMappedId(MappedId);
            if(locationId == FrameworkToolKit::getThisLocationId ()) {
                ifx_list.push_back(ifindex);

                NSM_PLUG_INFO_S(string("Retrieved Ifindex for Mbr=") + vpPhyIntfLocalManagedObject[i]->getIfName() +
                        string(" Ifindex=") + ifindex);
            }
            delete vpPhyIntfLocalManagedObject[i];
        }
    }

	/*
     * API : doesVlanExist
	 * Descrtiption : Verifies if the input vlan is configured or not.
	 * Return value : WAVE_MESSAGE_SUCCESS if vlan exits.
	 *     			  Else, returns the specific error code.
     */
	ResourceId NsmUtils::doesVlanExist(UI32 vlanId)
    {
        UI32 vlanCount = 0;
        ResourceId  status   = WAVE_MESSAGE_SUCCESS;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlanId,"id"));
        status = querySynchronouslyForCount (&syncQueryCtxt, vlanCount);

        if (status != WAVE_MESSAGE_SUCCESS) {
			trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount on vlanId failed"));
		} else {
			if (vlanCount != 1) {
				trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount on vlanId= ") + vlanCount);
				status = WAVE_MESSAGE_ERROR;
            } else {
				status = WAVE_MESSAGE_SUCCESS;
			}
        }
        return status;
    }
/* Used to calculate the list of vlans to be configured/provisioned, 
   during postboot config replay or slot power on or isl to edge port transition.*/
    ResourceId NsmUtils::populateVlanInformation(VlanInformation &vlanInformation, 
            const SI32 slotId, const string &ifname) {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        const char* func = __FUNCTION__;
        tracePrintf(TRACE_LEVEL_INFO,
                "NsmUtils::%s: Computing VLANs to be provisioned", func);

        set<UI32> vlans;

        /* Get the list of vlans to be provisioned for the svi. */
        mo_iterator<NsmIntfSviManagedObject> iterSvi;
        mo_criteria &critSvi = iterSvi.criteria();
        critSvi.select("vlanId");
        critSvi.location(FrameworkToolKit::getThisLocationId ());
        NsmIntfSviManagedObject *sviMo;
        while (iterSvi.hasNext()) {
            sviMo = iterSvi.next();
            vlans.insert(sviMo->getIfId());
        }
        const UI32 svis = vlans.size();
        tracePrintf(TRACE_LEVEL_INFO,
                "NsmUtils::%s: Computed %d VLANs (SVIs) to be provisioned",
                func, svis);

        /* Get the list of vlans to be provisioned for po/phy/port-profiled-ports */
        mo_iterator<VlanIdForPoPhyIntfView> iterId;
        mo_criteria &critId = iterId.criteria();
        critId.location(FrameworkToolKit::getThisLocationId ());

        if ( slotId != INVALID_SLOT_ID) {
            critId.slot(slotId);
        }
        if ( !ifname.empty() ) {
            critId.And(ATTR_IF_NAME, ifname);
        }

        VlanIdForPoPhyIntfView *vlanIdForPhyIntfView;
        while (iterId.hasNext()) {
            vlanIdForPhyIntfView = iterId.next();
            vlans.insert(vlanIdForPhyIntfView->getVlanId());
        }
        const UI32 individualVlans = vlans.size() - svis;
        tracePrintf(TRACE_LEVEL_INFO,
                "NsmUtils::%s: Computed %d VLANs to be provisioned",
                func, individualVlans);

        /* SELECT id from wavecurrent.vlanintfmanagedobject where id < 4092
         *
         */
        // If we retrieve all the VLANs and try to compare in-memory we may
        // have upto 4K comparisons; not sure which whether retrieving separtely
        // is cheaper than  4K comparison either!
        set<UI32> dot1qVlans;
        mo_iterator<VlanIntfManagedObject> vlanIter;
        mo_criteria &vlanCrit = vlanIter.criteria();
        vlanCrit.select("id");
        vlanCrit.And("id", MAX_DOT1Q_VLAN,
                WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN);
        vlanCrit.And("id", DEFAULT_DOT1Q_VLAN,
                WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);

        while (vlanIter.hasNext()) {
            dot1qVlans.insert(vlanIter.next()->getId());
        }

        vector<UI32> allVlans(dot1qVlans.begin(), dot1qVlans.end());
        // Now retrieve non-dot1qvlans
        mo_iterator<VlanIntfManagedObject> nonDot1QIter;
        mo_criteria &nonDot1QCrit = nonDot1QIter.criteria();
        nonDot1QCrit.select("id");
        nonDot1QCrit.And("id", MAX_DOT1Q_VLAN,
                WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN_OR_EQUAL_TO);
        while (nonDot1QIter.hasNext()) {
            allVlans.push_back(nonDot1QIter.next()->getId());
        }

        vlanInformation.unProvisionedVlans = UI32Range(allVlans);

            tracePrintf(TRACE_LEVEL_INFO,
                    "NsmUtils::%s: Now computing VLAN ranges to be provisioned",
                    func);
            mo_iterator<VlanRangeForPoPhyIntfView> iterRange;
            mo_criteria &critRange = iterRange.criteria();
            critRange.location(FrameworkToolKit::getThisLocationId ());
        if ( slotId != INVALID_SLOT_ID ) {
                critRange.slot(slotId);
            }
            if ( !ifname.empty() ) {
                critRange.And(ATTR_IF_NAME, ifname);
            }

            UI32Range addRange, removeRange, curAddRange, curRemoveRange;

            bool removeEmpty = false;
        bool isTrunkAllSet = false;

            VlanRangeForPoPhyIntfView *vlanRangeForPhyIntfView;
            // if we have mutual exclusive trunk (all+remove) no need to compute further
            while (!removeEmpty && iterRange.hasNext()) {
                vlanRangeForPhyIntfView = iterRange.next();
                curAddRange = vlanRangeForPhyIntfView->getVlanRangeAdd();
                curRemoveRange =  vlanRangeForPhyIntfView->getVlanRangeRemove();
                if (!vlanRangeForPhyIntfView->isAllowedVlanAll() &&
                        curAddRange.isEmpty() && curRemoveRange.isEmpty()) {
                    continue;
                }
                if (!curAddRange.isEmpty()) {
                    addRange += curAddRange;
                }
            if (vlanRangeForPhyIntfView->isAllowedVlanAll()) {
                if (!isTrunkAllSet) {
                        removeRange = curRemoveRange;
                    isTrunkAllSet = true;
                    } else {
                        removeRange = removeRange.intersection(curRemoveRange);
                    }
                    removeEmpty = removeRange.isEmpty();
                }
            }
            tracePrintf(TRACE_LEVEL_INFO,
                "NsmUtils::%s: removeEmpty = %d, isTrunkAllSet = %d, "
                    "addRange = %s, removeRange = %s",
                func, removeEmpty, isTrunkAllSet, addRange.toString().c_str(),
                    removeRange.toString().c_str());
        if (isTrunkAllSet) {
            if(!removeEmpty) {
                // Now provisionedVlans = AllConfigured - (commonRemove - addUnion) for dot1q only.
                UI32Range actualRemove = removeRange - addRange;
                vector<UI32> vlansToRemove;
                actualRemove.getUI32RangeVector(vlansToRemove);
                for(vector<UI32>::const_iterator ci=vlansToRemove.begin();
                        ci != vlansToRemove.end(); ci++) {
                    dot1qVlans.erase(*ci);
                }
            }
                vlans.insert(dot1qVlans.begin(), dot1qVlans.end());
            }

            vector<UI32> vlansToAdd;
            addRange.getUI32RangeVector(vlansToAdd);
            vlans.insert(vlansToAdd.begin(), vlansToAdd.end());

            tracePrintf(TRACE_LEVEL_INFO,
                    "NsmUtils::%s: Computed %d VLANs (based on ranges) to be provisioned",
                    func, (vlans.size() - (svis + individualVlans)));
        vector<UI32> v(vlans.begin(), vlans.end());
        UI32Range provisionedRange(v);
        vlanInformation.provisionedVlans += provisionedRange;
        vlanInformation.unProvisionedVlans -= provisionedRange;
        tracePrintf(TRACE_LEVEL_INFO,
                "NsmUtils::%s: In total (individual+range), "
                "%d VLANs to be provisioned. ProvisionedVlans = %s, "
                "ConfiguredVlans = %s",
                func, vlans.size(),
                vlanInformation.provisionedVlans.toString().c_str(),
                vlanInformation.unProvisionedVlans.toString().c_str());
        return status;
    }

    bool NsmUtils::isTrunkAllPresent(const SI32 slotId, const string &ifname) {
        string moClass = (slotId != -1)?
                PhyIntfLocalManagedObject::getClassName():
                PoPhyIntfManagedObject::getClassName();
        WaveManagedObjectSynchronousQueryContext queryCtx(moClass);

        if ( (slotId != -1) ) {
            queryCtx.setSlotFilter(slotId);
        }
        if ( !ifname.empty() ) {
            queryCtx.addAndAttribute(new AttributeString(ifname, ATTR_IF_NAME));
        }

        UI32Range emptyVlanRangeRemove;

        queryCtx.createConditionSet ("NO_VLAN_REMOVE_RANGE");
        queryCtx.addAttributeToConditionSet("NO_VLAN_REMOVE_RANGE",
                new AttributeBool(true,"isAllowedVlanAll"),
                WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        queryCtx.addAttributeToConditionSet("NO_VLAN_REMOVE_RANGE",
                new AttributeUI32Range(&emptyVlanRangeRemove, "vlanRangeRemove"),
                WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        // For PVLAN
        queryCtx.createConditionSet ("NO_PVLAN_REMOVE_RANGE");
        queryCtx.addAttributeToConditionSet("NO_PVLAN_REMOVE_RANGE",
                new AttributeBool(true,"pVlanIsAllowedVlanAll"),
                WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        queryCtx.addAttributeToConditionSet("NO_PVLAN_REMOVE_RANGE",
                new AttributeUI32Range(&emptyVlanRangeRemove, "pVlanRangeRemove"),
                WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);

        queryCtx.combineConditionSets ("NO_VLAN_REMOVE_RANGE+NO_PVLAN_REMOVE_RANGE");

        UI32 n = 0;
        querySynchronouslyForCount(&queryCtx, n);
        tracePrintf(TRACE_LEVEL_INFO,
                "NsmUtils::isTrunkAllPresent: Number of ports with trunk ALL = %d", n);
        return n > 0;
    }

    UI32 NsmUtils::getVlanClassificationMOCount(UI32 vlanId, UI32 ctagId, const ObjectId &pophyObjectId)
    {
        UI32 count = 0;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt (VlanClassificationMO::getClassName ());
		if (vlanId > 0)
        {
            syncQueryCtxt.addAndAttribute (new AttributeUI32 (vlanId, "vlanId"));
        }
        if (ctagId > 0)
        {
            syncQueryCtxt.addAndAttribute (new AttributeUI32 (ctagId, "ctagId"));
        }
		if(pophyObjectId != ObjectId::NullObjectId)
		{
			syncQueryCtxt.addAndAttribute (new AttributeObjectId (pophyObjectId, "ownerManagedObjectId"));
		}

        querySynchronouslyForCount(&syncQueryCtxt, count);
        return count;
    }

    bool NsmUtils::isGvlanIdPresentInRange(UI32Range &vlanIdRange)
    {
        string vlanRange = vlanIdRange.toString();
        UI32 counter = vlanRange.size() - 1;
        for (; counter >= 0 && isdigit(vlanRange[counter]); --counter);
        return  atoi(vlanRange.c_str() + counter + 1) > MAX_DOT1Q_VLAN_ID; 
    }

    bool NsmUtils::isGVlanConfigPresent()
	{
        UI32 count = 0;
        bool gvlanConfigPresent = false;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI32(MAX_DOT1Q_VLAN_ID, "id"),  WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN_OR_EQUAL_TO);
        querySynchronouslyForCount(&syncQueryCtxt, count);

        trace(TRACE_LEVEL_INFO, string("NsmUtils::isGVlanConfigPresent: No of vlans greater than MAX_DOT1Q_VLAN:") + count);

        if(count > 0) {
            gvlanConfigPresent = true;
        }

        status = APPMUtils::isGvlanSpecificAPPMConfigPresent();
        if(WAVE_MESSAGE_SUCCESS != status) {
            gvlanConfigPresent = true;
        }

		return gvlanConfigPresent;
	}

	UI32 NsmUtils::getVlanClassificationMOCount(UI32Range ctagRange, const ObjectId &pophyObjectId)
	{
		vector<Attribute *> vAttributes;
		vector <UI32> ctagRangeVector;
		size_t ctagCount = 0;

		ctagRange.getUI32RangeVector(ctagRangeVector);
		ctagCount = ctagRangeVector.size();

		for(size_t i = 0; i < ctagCount; ++i)
	   	{
			vAttributes.push_back(new AttributeUI32(ctagRangeVector[i], "ctagId"));
		}

		UI32 ctagMOCount = 0;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassificationMO::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeSqlIn(vAttributes), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
		syncQueryCtxt.addAndAttribute (new AttributeObjectId (pophyObjectId, "ownerManagedObjectId"));
		querySynchronouslyForCount(&syncQueryCtxt, ctagMOCount);

		return ctagMOCount;
	}

	UI32 NsmUtils::getVlanClassificationMOCount(const string &vlanClassification, const UI32 &vlanId, const string &mac, const UI32 &macGroupId, const UI32 &ctagId, const ObjectId &pophyObjectId)
	{
        UI32 count = 0;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt (vlanClassification);
		if (vlanId > 0)
        {
            syncQueryCtxt.addAndAttribute (new AttributeUI32 (vlanId, "vlanId"));
        }
		if(pophyObjectId != ObjectId::NullObjectId)
		{
			syncQueryCtxt.addAndAttribute (new AttributeObjectId (pophyObjectId, "ownerManagedObjectId"));
		}

		if((vlanClassification== MacAddressVlanClassificationMO::getClassName()) || (vlanClassification== RspanMacAddressVlanClassificationMO::getClassName()))
		{
	        if (mac != "")
    	    {
        	    syncQueryCtxt.addAndAttribute (new AttributeString (mac, "mac"));
	        }
		}
		else if((vlanClassification== MacGroupVlanClassificationMO::getClassName()) || (vlanClassification== RspanMacGroupVlanClassificationMO::getClassName()))
		{
	        if (macGroupId != 0)
    	    {
        	    syncQueryCtxt.addAndAttribute (new AttributeUI32 (macGroupId, "macGroupId"));
	        }
		}
		else if((vlanClassification== CtagVlanClassificationMO::getClassName()) || (vlanClassification== RspanGVlanTrunkPortManagedObject::getClassName()) || (vlanClassification== CtagPVlanClassificationMO::getClassName()))
		{
	        if (ctagId != 0)
    	    {
        	    syncQueryCtxt.addAndAttribute (new AttributeUI32 (ctagId, "ctagId"));
	        }
		}
		else
		{
			trace(TRACE_LEVEL_ERROR, string("NsmUtils::getVlanClassificationMOCount:Invalid vlan classificationMO is being queried."));
			
			return 0;
		}

        querySynchronouslyForCount(&syncQueryCtxt, count);
        return count;
    }
} // DcmNs

