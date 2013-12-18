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

#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/VlanClassifierActivateGlobalManagedObject.h"

#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/VlanClassifierActivateLocalManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"


#include "L2sys/Global/L2sysGlobalStaticMacManagedObject.h"
#include "Mstp/Global/MstpInstanceConfigManagedObject.h"

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Attributes/AttributeSqlIn.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

#include "Nsm/Common/PVlanUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"
#include "Utils/DceContext.h"
#include "Utils/mo_iterator.h"

#include <algorithm>

namespace DcmNs
{

	bool PVlanUtils:: isPrimaryVlan(UI32 primVId)
	{
		tracePrintf(TRACE_LEVEL_DEBUG, "isPrimaryVlan   ");
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
	   	syncQueryCtxt.addAndAttribute (new AttributeUI32(&primVId,"id"));
	   	syncQueryCtxt.addAndAttribute (new AttributeUI8(PVLAN_TYPE_PRIMARY, "pvlantype"));

	    UI32 moCount = 0;
	    querySynchronouslyForCount(&syncQueryCtxt, moCount);

	    return (moCount == 1);
	}


	bool PVlanUtils:: isSecondaryVlanRange(UI32Range vlanRange)
	{
		vector<Attribute *> vidAttributes;
		vector <UI32> msgVlanRangeVector;
		size_t vidCount=0;

	    vlanRange.getUI32RangeVector(msgVlanRangeVector);
	    vidCount = msgVlanRangeVector.size();
	    tracePrintf(TRACE_LEVEL_DEBUG, "isSecondaryVlanRange : number of vlans=%d", vidCount);

	    for(size_t i = 0; i < vidCount; ++i) {
	        vidAttributes.push_back(new AttributeUI32(msgVlanRangeVector[i], "id"));
	    }

		vector<Attribute *> pvlanTypes;
		pvlanTypes.push_back(new AttributeUI8(PVLAN_TYPE_ISOLATED, "pvlantype"));
		pvlanTypes.push_back(new AttributeUI8(PVLAN_TYPE_COMMUNITY, "pvlantype"));

	    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());

    	syncQueryCtxt.addAndAttribute (new AttributeSqlIn(vidAttributes), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
	    syncQueryCtxt.addAndAttribute (new AttributeSqlIn(pvlanTypes), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);

	    UI32 moCount = 0;
	    querySynchronouslyForCount(&syncQueryCtxt, moCount);
        tracePrintf(TRACE_LEVEL_DEBUG, "isSecondaryVlanRange : Query returned %d count, expected %d", moCount,vidCount);
        if(moCount != vidCount)
        	return false;

		return true;
	}

	bool PVlanUtils:: isPrimaryVlanRange(UI32Range vlanRange)
	{
		vector<Attribute *> vidAttributes;
		vector <UI32> msgVlanRangeVector;
		size_t vidCount=0;

	    vlanRange.getUI32RangeVector(msgVlanRangeVector);
	    vidCount = msgVlanRangeVector.size();
	    tracePrintf(TRACE_LEVEL_DEBUG, "isPrimaryVlanRange : number of vlans=%d", vidCount);

	    for(size_t i = 0; i < vidCount; ++i) {
	        vidAttributes.push_back(new AttributeUI32(msgVlanRangeVector[i], "id"));
	    }

	    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());

    	syncQueryCtxt.addAndAttribute (new AttributeSqlIn(vidAttributes), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
	    syncQueryCtxt.addAndAttribute (new AttributeUI8(PVLAN_TYPE_PRIMARY, "pvlantype"));

	    UI32 moCount = 0;
	    querySynchronouslyForCount(&syncQueryCtxt, moCount);
        tracePrintf(TRACE_LEVEL_DEBUG, "isPrimaryVlanRange : Query returned %d count, expected %d", moCount,vidCount);
        if(moCount != vidCount)
        	return false;

		return true;
	}

	bool PVlanUtils:: isSecondaryVlan(UI32 vlanId)
	{
		vector<Attribute *> pvlanTypes;
		pvlanTypes.push_back(new AttributeUI8(PVLAN_TYPE_ISOLATED, "pvlantype"));
		pvlanTypes.push_back(new AttributeUI8(PVLAN_TYPE_COMMUNITY, "pvlantype"));

	    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
    	syncQueryCtxt.addAndAttribute (new AttributeUI32(vlanId,"id"));
	    syncQueryCtxt.addAndAttribute (new AttributeSqlIn(pvlanTypes), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);

	    UI32 moCount = 0;
	    querySynchronouslyForCount(&syncQueryCtxt, moCount);

		return (moCount == 1);

	}


	bool PVlanUtils:: isIsolatedVlanCountMore(UI32Range vlanRange)
	{

	    vector <UI32> msgVlanRangeVector;
	    vlanRange.getUI32RangeVector(msgVlanRangeVector);
	    size_t vidCount = msgVlanRangeVector.size();

	    tracePrintf(TRACE_LEVEL_DEBUG, "isIsolatedVlanCountMore : number of vlans=%d", vidCount);

	    vector<Attribute *> vidAttributes;
	    for(size_t i = 0; i < vidCount; ++i) {
	        vidAttributes.push_back(new AttributeUI32(msgVlanRangeVector[i], "id"));
	    }

	    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
	    syncQueryCtxt.addAndAttribute (new AttributeSqlIn(vidAttributes), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
	    syncQueryCtxt.addAndAttribute (new AttributeUI8(PVLAN_TYPE_ISOLATED, "pvlantype"));

	    UI32 moCount = 0;
	    querySynchronouslyForCount(&syncQueryCtxt, moCount);
	    tracePrintf(TRACE_LEVEL_DEBUG, "isIsolatedVlanCountMore: Query returned %d count, expected %d", moCount,vidCount);

	    return (moCount > 1);
	}

	bool PVlanUtils:: isPVlanIdPresent(UI32Range vlanRange, UI32 primVId)
	{
	    vector <UI32> msgVlanRangeVector;
	    vlanRange.getUI32RangeVector(msgVlanRangeVector);
	    vector<UI32>::iterator search_prim = find(msgVlanRangeVector.begin(), msgVlanRangeVector.end(), primVId);
		if (search_prim != msgVlanRangeVector.end())
		{
			return true;
		}
		return false;
	}


	bool PVlanUtils:: doesAssociationExists(UI32 primVId, UI32 secVId, DceContext *ctxt)
	{
		 tracePrintf(TRACE_LEVEL_DEBUG, "doesAssociationExists  : ");

		 mo_iterator<VlanIntfManagedObject> vlanMOIter(ctxt);
		 vlanMOIter.criteria().And("id", primVId).select("pVlanSecAssocRange");

		 if(vlanMOIter.hasNext())
		 {
			VlanIntfManagedObject *pVlanIntfMo = vlanMOIter.next();

			if(!(pVlanIntfMo ->getPvlanSecAssocRange().isEmpty()) && (pVlanIntfMo ->getPvlanSecAssocRange().find(secVId)))
			{
				tracePrintf(TRACE_LEVEL_DEBUG, "doesAssociationExists  yes %d %d", primVId, secVId );
				return true;
			}
			tracePrintf(TRACE_LEVEL_DEBUG, "doesAssociationExists  No %d %d", primVId, secVId );
		 }
		 else
		 {
			 tracePrintf(TRACE_LEVEL_ERROR, "doesAssociationExists  : cudnt find db object %d ", primVId);
		 }

		 return false;
	}


	bool PVlanUtils:: doesAssociationExists(UI32 primVId, UI32Range secVlanRange, DceContext *ctxt)
	{
		 tracePrintf(TRACE_LEVEL_DEBUG, "doesAssociationExists  : ");

		 mo_iterator<VlanIntfManagedObject> vlanMOIter(ctxt);
		 vlanMOIter.criteria().And("id", primVId).select("pVlanSecAssocRange");

		 if(vlanMOIter.hasNext())
		 {
			VlanIntfManagedObject *pVlanIntfMo = vlanMOIter.next();

			if(!(pVlanIntfMo ->getPvlanSecAssocRange().isEmpty()) && (pVlanIntfMo ->getPvlanSecAssocRange().containsAll(secVlanRange)))
			{
				tracePrintf(TRACE_LEVEL_DEBUG, "doesAssociationExists - YES ");
				return true;
			}
			tracePrintf(TRACE_LEVEL_DEBUG, "doesAssociationExists - No ");
		 }
		 else
		 {
			 tracePrintf(TRACE_LEVEL_ERROR, "doesAssociationExists  : cudnt find db object ");
		 }

		 return false;
	}


	bool PVlanUtils:: isSecVlanPartOfDomain(UI32 primVId, UI32Range msgSecVlanRange, DceContext *ctxt)
	{
		 tracePrintf(TRACE_LEVEL_DEBUG, "isSecVlanPartOfDomain  : ");

		 mo_iterator<VlanIntfManagedObject> vlanMOIter(ctxt);
		 vlanMOIter.criteria().And("pvlantype",(UI32)PVLAN_TYPE_PRIMARY).select("id").select("pVlanSecAssocRange");

		 while (vlanMOIter.hasNext())
		 {
			VlanIntfManagedObject *pVlanIntfMo = vlanMOIter.next();

			if(pVlanIntfMo->getId() != primVId)
			{
				UI32Range moRange = pVlanIntfMo ->getPvlanSecAssocRange();

				if(!moRange.isEmpty() && (moRange.containsAny(msgSecVlanRange)))
				{
					tracePrintf(TRACE_LEVEL_DEBUG, "isSecVlanPartOfDomain - Primary Vlan  %d ", pVlanIntfMo->getId());
					return true;
				}
			}
			tracePrintf(TRACE_LEVEL_DEBUG, "isSecVlanPartOfDomain - No ");

		 }

		 return false;
	}

	bool PVlanUtils:: isDomainPresentOnPromiscuous(vector<WaveManagedObjectPointer<PVlanMappingManagedObject> >  mappingMO, UI32 primVId)
	{
		bool present = false;

		for(int i=0; i < MAX_PVLAN_DOMAIN_ON_PROMISCUOS; i++)
		{
			if(mappingMO[i]->getPrimaryVlanId() == primVId)
			{
				present = true;
				break;
			}
		}
		return present;
	}

	bool PVlanUtils:: isPVlan(UI32 vlanId)
	{
		tracePrintf(TRACE_LEVEL_DEBUG, "isPVlan    ");
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());
	   	syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlanId,"id"));
	   	syncQueryCtxt.addAndAttribute (new AttributeUI8(PVLAN_TYPE_UNKNOWN, "pvlantype"));

	    UI32 moCount = 0;
	    querySynchronouslyForCount(&syncQueryCtxt, moCount);

	    return (moCount == 0);
	}


	bool PVlanUtils:: isVeConfigured(UI32 vlanId, DceContext *ctxt, VlanIntfManagedObject *pVlanMO)
	{
		ObjectId vlanOid;

		if(pVlanMO != NULL)
		{
			vlanOid = pVlanMO->getObjectId();
		}
		else
		{
			vlanOid = getVlanObjectId(vlanId, ctxt);
		}

		WaveManagedObjectSynchronousQueryContext syncGlQueryCtxt(NsmIntfSviManagedObject::getClassName());
		syncGlQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(&vlanOid, "vlan",VlanIntfManagedObject::getClassName()));

        UI32 moCount = 0;
        querySynchronouslyForCount(&syncGlQueryCtxt, moCount);
        tracePrintf(TRACE_LEVEL_DEBUG, "isVeConfigured : Query returned %d count", moCount);

        return (moCount > 0);
	}


	bool PVlanUtils:: isStaticMacConfigured(UI32 vlanId)
	{
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(L2sysGlobalStaticMacManagedObject::getClassName());
    	syncQueryCtxt.addAndAttribute (new AttributeUI32(&vlanId,"vlanid"));

        UI32 moCount = 0;
        querySynchronouslyForCount(&syncQueryCtxt, moCount);
        tracePrintf(TRACE_LEVEL_DEBUG, "isStaticMacConfigured : Query returned %d count", moCount);

        return (moCount > 0);
	}


	bool PVlanUtils::isL2AclConfigured(UI32 vlanId, DceContext *ctxt)
	{
		mo_iterator<VlanIntfManagedObject> vlanMOIter(ctxt);
		vlanMOIter.criteria().And("id", vlanId).select("ingressEgressMacAclPolicy");

		if(vlanMOIter.hasNext())
		{
			VlanIntfManagedObject *pVlanIntfMo = vlanMOIter.next();

			if(pVlanIntfMo->getIngressEgressMacAclPolicy().size() > 0)
			{
				trace(TRACE_LEVEL_DEBUG, "isL2AclConfigured : got a db object ");
				return true;
			}

		 }
		else
		{
			trace(TRACE_LEVEL_DEBUG, "isL2AclConfigured: cudnt find db object ");
		}

		return false;

	}

	bool PVlanUtils::isMSTPInstancePresent(UI32 vlanId, DceContext *ctxt, VlanIntfManagedObject *pVlanMO)
	{
		vector<ObjectId> vlanOids;
		ObjectId vlanOid;

		if(pVlanMO != NULL)
		{
			vlanOid = pVlanMO->getObjectId();
		}
		else
		{
			vlanOid = getVlanObjectId(vlanId, ctxt);
		}

	    vlanOids.push_back(vlanOid);
	    ObjectId dummyOid;

	    WaveManagedObjectSynchronousQueryContext queryCtx(MstpInstanceConfigManagedObject::getClassName());
	    queryCtx.setAttributeObjectIdVectorAssociation(
	            new AttributeObjectIdVectorAssociation(vlanOids, "vlanid",InstanceBaseManagedObject::getClassName(), dummyOid, VlanIntfManagedObject::getClassName()));

	    UI32 moCount = 0;
	    querySynchronouslyForCount(&queryCtx, moCount);
        tracePrintf(TRACE_LEVEL_DEBUG, "isMSTPInstancePresent: Query returned %d count", moCount);

        return (moCount > 0);
	}


	bool PVlanUtils::isClassifierPresentOnVlan(UI32 vlanId, DceContext *ctxt, VlanIntfManagedObject *pVlanMO)
	{

		NSM_PLUG_DBG("Entering ...");
		UI32 moCount = 0;
		ObjectId vlanOid;
	  
	    if(pVlanMO != NULL)
		{
			vlanOid = pVlanMO->getObjectId();
		}
		else
		{	
			vlanOid = getVlanObjectId(vlanId, ctxt);
		}

		WaveManagedObjectSynchronousQueryContext syncGlQueryCtxt(VlanClassifierActivateGlobalManagedObject::getClassName());
		syncGlQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(&vlanOid, "vlan",VlanIntfManagedObject::getClassName()));
		querySynchronouslyForCount(&syncGlQueryCtxt, moCount);

		if(moCount > 0)
		{
			return true;
		}
		else
		{
			WaveManagedObjectSynchronousQueryContext syncLocQueryCtxt(VlanClassifierActivateLocalManagedObject::getClassName());
			syncLocQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(&vlanOid, "vlan",VlanIntfManagedObject::getClassName()));
			querySynchronouslyForCount(&syncLocQueryCtxt, moCount);
			return moCount > 0;
		}

	}


	bool PVlanUtils::isClassifierPresentOnIntf(const string ifName, bool isGlobal)
	{
		trace(TRACE_LEVEL_DEBUG,"PVlanUtils::isClassifierPresentOnIntf");
		UI32 moCount = 0;

		if(isGlobal){
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierActivateGlobalManagedObject::getClassName());
			syncQueryCtxt.addAndAttribute(new AttributeString(ifName,"ifName"));
	        querySynchronouslyForCount(&syncQueryCtxt, moCount);
		}else{
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanClassifierActivateLocalManagedObject::getClassName());
			syncQueryCtxt.addAndAttribute(new AttributeString(ifName,"ifName"));
	        querySynchronouslyForCount(&syncQueryCtxt, moCount);
		}

        tracePrintf(TRACE_LEVEL_DEBUG, "isClassifierPresent: Query returned %d count", moCount);

        return moCount > 0;
	}


	bool PVlanUtils::isPrivateVlanMode(const string ifName)
	{
	    WaveManagedObjectSynchronousQueryContext queryCtxt(PoPhyIntfManagedObject::getClassName());

	    queryCtxt.addAndAttribute(new AttributeString(ifName, "ifName"));
	    queryCtxt.addOrAttribute(new AttributeBool(true, "pVlanModePromiscuous"));
	    queryCtxt.addOrAttribute(new AttributeBool(true, "pVlanModeHost"));
	    queryCtxt.addOrAttribute(new AttributeBool(true, "pVlanModeTrunkBas"));
	    queryCtxt.addOrAttribute(new AttributeBool(true, "pVlanModeTrunkHost"));
	    queryCtxt.addOrAttribute(new AttributeBool(true, "pVlanModeTrunkPromis"));

	    unsigned int count = 0;
	    querySynchronouslyForCount(&queryCtxt, count);
	    tracePrintf(TRACE_LEVEL_DEBUG, "PVlanUtils::isPrivateVlanMode -- Query returned %d", count);

	    return count > 0;
	}


	ObjectId PVlanUtils:: getVlanObjectId (UI32 vlanId, DceContext *ctxt)
	{

		ObjectId vlanObjectId;
        mo_iterator<VlanIntfManagedObject> vlanIter ( ctxt );
        vlanIter.criteria().select("objectId").And("id", vlanId);
        if (vlanIter.hasNext()) {
        	trace(TRACE_LEVEL_DEBUG, "getVlanObjectId :got a db object ");
        	vlanObjectId = vlanIter.next()->getObjectId();
        }
        else
        {
			trace(TRACE_LEVEL_ERROR, "getVlanObjectId : cudnt find db object ");
		}

		return vlanObjectId;
	}


	UI32Range PVlanUtils:: getSecPVlanRange (UI32Range msgVlanrange, UI32Range moVlanrange, PVlanMappingOper oper)
	{
		tracePrintf(TRACE_LEVEL_DEBUG, "getSecPVlanRangee ");

        if(oper == PVLAN_MAPPING_ADD)
        {
        	return msgVlanrange + moVlanrange;
        }
        else
        {
        	return moVlanrange - msgVlanrange;
        }
	}


	UI32Range PVlanUtils:: getSecPVlanRangeFromDb (UI32 primVId, DceContext *ctxt)
	{
		 tracePrintf(TRACE_LEVEL_DEBUG, "getSecPVlanRangeFromMo  : ");
		 UI32Range moVlanrange;
		 mo_iterator<VlanIntfManagedObject> vlanMOIter(ctxt);
		 vlanMOIter.criteria().And("id", primVId).select("pVlanSecAssocRange");

		 if(vlanMOIter.hasNext())
		 {
			VlanIntfManagedObject *pVlanIntfMo = vlanMOIter.next();

			if(!(pVlanIntfMo ->getPvlanSecAssocRange().isEmpty()))
			{
				tracePrintf(TRACE_LEVEL_DEBUG, "getSecPVlanRangeFromDb : Fetching range");
				moVlanrange =  pVlanIntfMo ->getPvlanSecAssocRange();
			}
		 }
		 else
		 {
			tracePrintf(TRACE_LEVEL_ERROR, "getSecPVlanRangeFromDb  : cudn't find db object ");
		 }

		 return moVlanrange;
	}

	bool PVlanUtils::doesSecPVlanRangeExceedMax (UI32 primVId, UI32Range msgVlanrange, DceContext *ctxt )
	{

		UI32Range moRange  = PVlanUtils::getSecPVlanRangeFromDb (primVId, ctxt);
		UI32Range totRange = PVlanUtils::getSecPVlanRange(msgVlanrange, moRange, PVLAN_MAPPING_ADD);

		UI32 count 	   = totRange.getCount();

		if(count > NSM_MAX_NUM_SEC_PVLANS_PER_PVLAN_DOMAIN)	// the max assoc limit is 24 for mercury callisto and cobra.
		{
			trace(TRACE_LEVEL_WARN,string("Assoc Pvlan count after ADD op would exceed max ") + count + "; not adding");
			return true;
		}
		else
		{
			trace(TRACE_LEVEL_DEBUG,string("IntfConfGlobalW addPvlan ") + count + " adding");
			return false;
		}
	}

	bool PVlanUtils::isNativeVlanPresentInList (UI32Range vlanRange, UI32 nativeVlanId)
	{
		NSM_PLUG_DBG_S(string(" Private Native-vlan ::  ") + nativeVlanId);

		vector<UI32> vlanRangeVector;
		vlanRange.getUI32RangeVector(vlanRangeVector);
		vector<UI32>::iterator moiter;

		if ((moiter = find(vlanRangeVector.begin(), vlanRangeVector.end(),
			nativeVlanId)) != vlanRangeVector.end()) {
		return true;
		}

		return false;
}

	UI32 PVlanUtils::getPVlanDomainCount(void)
	{	
		UI32 count = 0;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VlanIntfManagedObject::getClassName());

		syncQueryCtxt.addAndAttribute (new AttributeUI8(PVLAN_TYPE_PRIMARY, "pvlantype"));
		querySynchronouslyForCount (&syncQueryCtxt, count);

		return count;
	}

	UI32 PVlanUtils::getVlanIdInPVlanTrunkAssociationCount(UI32 vlanId, const ObjectId &pophyObjectId)
	{
		UI32 count = 0;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PVlanTrunkAssocManagedObject::getClassName());

		if(pophyObjectId != ObjectId::NullObjectId)
		{
			syncQueryCtxt.addAndAttribute (new AttributeObjectId(pophyObjectId, "ownerManagedObjectId"));
		}

		syncQueryCtxt.addOrAttribute (new AttributeUI32(vlanId, "primaryVlanId"));
		syncQueryCtxt.addOrAttribute (new AttributeUI32(vlanId, "secondaryVlanId"));
		querySynchronouslyForCount(&syncQueryCtxt, count);

		return count;
	}
	
	UI32 PVlanUtils::getVlanAsPrimaryInPVlanMappingCount(UI32 vlanId, const ObjectId &pophyObjectId)
	{
		UI32 count = 0;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PVlanMappingManagedObject::getClassName());

		if(pophyObjectId != ObjectId::NullObjectId)
		{
			syncQueryCtxt.addAndAttribute (new AttributeObjectId(pophyObjectId, "ownerManagedObjectId"));
		}

		syncQueryCtxt.addAndAttribute (new AttributeUI32(vlanId, "primaryVlanId"));
		querySynchronouslyForCount(&syncQueryCtxt, count);

		return count;
	}

}
