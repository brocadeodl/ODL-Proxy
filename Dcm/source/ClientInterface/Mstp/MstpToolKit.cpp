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

#include "ClientInterface/Mstp/MstpToolKit.h"

#include <iostream>
#include <sstream>
#include <string>
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeSqlIn.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Mstp/Global/PvstBridgeConfigManagedObject.h"
#include "Mstp/Global/RpvstBridgeConfigManagedObject.h"
#include "Mstp/Global/StpBridgeConfigManagedObject.h"
#include "Mstp/Global/RstpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpInstanceConfigManagedObject.h"
#include "Mstp/Global/VlanBridgeStpConfigManagedObject.h"
#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "Nsm/Common/PVlanUtils.h"
#include "UidService/Common/UidToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/mo_iterator.h"
#include "raslog/raslog_mstp.h"

using namespace WaveNs;

namespace DcmNs
{

ResourceId MstpToolKit::getBridgeConfigMO(XstpBridgeConfigManagedObject *&pMo, int &mode, DceContext *pSequencerContext)
{
    string className = "";
    ResourceId status = getBridgeClassName(className, pSequencerContext);
    if (status != WAVE_MESSAGE_SUCCESS) {
        return status;
    }

    if (className.empty()) {
        mode = MSTP_DCM_BRIDGE_NONE;
        pMo = NULL;
        return WAVE_MESSAGE_SUCCESS;
    }

    vector<WaveManagedObject*> *queryResults = querySynchronously(className);
    if (queryResults && !queryResults->empty()) {
        prismAssert(queryResults->size() == 1, __FILE__, __LINE__);
        pMo = dynamic_cast<XstpBridgeConfigManagedObject*>(queryResults->front());
        mode = getModeForClassName(className);
    }

    // Delete only the vector.. MO needs to be deleted by the caller..
    delete queryResults;

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId MstpToolKit::getBridgeMode(int& mode, DceContext *pSequencerContext)
{
    string className = "";
    ResourceId status = getBridgeClassName(className, pSequencerContext);
    if (status == WAVE_MESSAGE_SUCCESS) {
        mode = getModeForClassName(className);
    }
    return status;
}

ResourceId MstpToolKit::getBridgeClassName(string &o_brClassName, DceContext *pSequencerContext)
{
    WaveManagedObjectSynchronousQueryContext queryContext(XstpBridgeConfigManagedObject::getClassName());
    queryContext.addSelectField("objectId");
    vector<WaveManagedObject*> *queryResults = querySynchronously(&queryContext);

    if (queryResults && !queryResults->empty()) {
        XstpBridgeConfigManagedObject *mo = dynamic_cast<XstpBridgeConfigManagedObject*>(queryResults->front());
        o_brClassName = OrmRepository::getTableNameById(mo->getObjectId().getClassId());
    }

    if (pSequencerContext) {
        pSequencerContext->addManagedObjectsForGarbageCollection(*queryResults);
        delete queryResults;
    } else {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(queryResults);
    }

    return WAVE_MESSAGE_SUCCESS;
}

int MstpToolKit::getModeForClassName(const string &className) {
    if (className == MstpBridgeConfigManagedObject::getClassName()) return MSTP_DCM_BRIDGE_MSTP;
    if (className == RstpBridgeConfigManagedObject::getClassName()) return MSTP_DCM_BRIDGE_RSTP;
    if (className == StpBridgeConfigManagedObject::getClassName())  return MSTP_DCM_BRIDGE_STP;
    if (className == PvstBridgeConfigManagedObject::getClassName()) return MSTP_DCM_BRIDGE_PVSTP;
    if (className == RpvstBridgeConfigManagedObject::getClassName()) return MSTP_DCM_BRIDGE_RPVSTP;
    return MSTP_DCM_BRIDGE_NONE;
}

string MstpToolKit::getClassNameForMode(const int brMode)
{
    switch (brMode) {
    case MSTP_DCM_BRIDGE_STP:    return StpBridgeConfigManagedObject::getClassName();
    case MSTP_DCM_BRIDGE_RSTP:   return RstpBridgeConfigManagedObject::getClassName();
    case MSTP_DCM_BRIDGE_MSTP:   return MstpBridgeConfigManagedObject::getClassName();
    case MSTP_DCM_BRIDGE_PVSTP:  return PvstBridgeConfigManagedObject::getClassName();
    case MSTP_DCM_BRIDGE_RPVSTP: return RpvstBridgeConfigManagedObject::getClassName();
    }
    return "";
}


ResourceId MstpToolKit::isSpanningTreeConfigured (bool &o_configured)
{
    mo_criteria criteria (XstpBridgeConfigManagedObject::getClassName());
    return criteria.queryExists(o_configured);
}

ResourceId MstpToolKit::isSpanningTreeEnabled (bool &o_enabled)
{
    mo_criteria criteria (XstpBridgeConfigManagedObject::getClassName());
    return criteria.And("stpDisable", false).queryExists(o_enabled);
}

ResourceId MstpToolKit::isSpanningTreeEnabledOnIntf (bool &o_enabled, InterfaceType ifType, const string &ifName)
{
    mo_criteria criteria (XstpPortConfigManagedObject::getClassName());
    return criteria.And("port", ifName).And("stpDisable", false).queryExists(o_enabled);
}

ResourceId MstpToolKit::isSpanningTreeEnabledOnIntf (bool &o_enabled, const ObjectId &intfOid)
{
    mo_criteria criteria (XstpPortConfigManagedObject::getClassName());
    return criteria.And("ownerManagedObjectId", intfOid).And("stpDisable", false).queryExists(o_enabled);
}

static ResourceId isPortXstpConfigExists (bool &o_exists, const string &fieldName, UI32 locationFilterType, vector<string> &disconnectedPortPatterns)
{
    mo_criteria criteria (XstpPortConfigManagedObject::getClassName());

    if (disconnectedPortPatterns.empty()) {
        criteria.And(fieldName, true);
    } else {
        WaveManagedObjectSynchronousQueryContext *qc = criteria.getQueryContext();
        qc->createConditionSet("_C1");
        qc->addAttributeToConditionSet("_C1", new AttributeBool(true, fieldName));

        qc->createConditionSet("_C2");
        AttributeConditionOperator condOp = (locationFilterType == LOCATION_FILTER_CONNECTED) ? WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE : WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE;
        AttributeConcatenationOperator concatOp = (locationFilterType == LOCATION_FILTER_CONNECTED) ? WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND : WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR;
        for (UI32 i = 0; i < disconnectedPortPatterns.size(); ++i) {
            qc->addAttributeToConditionSet("_C2", new AttributeString(disconnectedPortPatterns[i], "port"), concatOp, condOp);
        }

        qc->combineConditionSets("_C1 * _C2");
    }

    return criteria.queryExists(o_exists);
}

ResourceId MstpToolKit::validateBpduTunnellingParamsNotConfigured (UI32 locationFilterType)
{
    vector<string> disconnectedPortPatterns;

    if (locationFilterType == LOCATION_FILTER_CONNECTED || locationFilterType == LOCATION_FILTER_DISCONNECTED) {
        char buff[16];
        vector<LocationId> disconnectedLocs = getDisconnectedLocations();
        for (UI32 i = 0; i < disconnectedLocs.size(); ++i) {
            snprintf(buff, sizeof buff, "%d/%%", DcmToolKit::getMappedIdFromLocationId(disconnectedLocs[i]));
            disconnectedPortPatterns.push_back(string(buff));
        }

        if (locationFilterType == LOCATION_FILTER_DISCONNECTED && disconnectedPortPatterns.empty()) {
            tracePrintf(TRACE_LEVEL_DEBUG, "MstpToolKit::%s : No disconnected locations!!", __FUNCTION__);
            return WAVE_MESSAGE_SUCCESS;
        }
    }

    bool exists = false;
    ResourceId status = isPortXstpConfigExists(exists, "bpduGuardEnable", locationFilterType, disconnectedPortPatterns);
    if (exists) status = WRC_NOT_ALLOWED_WITH_BPDU_DROP;

    if (status == WAVE_MESSAGE_SUCCESS) {
        status = isPortXstpConfigExists(exists, "taggedBpduEnable", locationFilterType, disconnectedPortPatterns);
        if (exists) status = WRC_NOT_ALLOWED_WITH_TAGGED_BPDU;
    }

    return status;
}

ResourceId MstpToolKit::checkMstpInstancePresent (int instanceId)
{
    WaveManagedObjectSynchronousQueryContext queryCtx(MstpInstanceConfigManagedObject::getClassName());
    queryCtx.addAndAttribute(new AttributeUI8((UI8) instanceId, "instanceId"));
    UI32 count = 0;
    ResourceId status = querySynchronouslyForCount(&queryCtx, count);
    if ((status == WAVE_MESSAGE_SUCCESS) && (count == 0)) {
//        status = WRC_MSTP_INST_NOT_EXIST;
        status = WAVE_MESSAGE_ERROR;
    }
    return status;
}

MstpInstanceConfigManagedObject* MstpToolKit::getMstpInstanceByVlanOid(const ObjectId &vlanOid, vector<string> &selectFields) {
    vector<ObjectId> vlanOids;
    vlanOids.push_back(vlanOid);

    WaveManagedObjectSynchronousQueryContext queryCtx(MstpInstanceConfigManagedObject::getClassName());
    queryCtx.setAttributeObjectIdVectorAssociation(
            new AttributeObjectIdVectorAssociation(vlanOids, "vlanid",
                    InstanceBaseManagedObject::getClassName(), ObjectId::NullObjectId, VlanIntfManagedObject::getClassName()));
    if (!selectFields.empty()) queryCtx.addSelectFields(selectFields);
    queryCtx.addLimitForQueryResults(1);

    vector<WaveManagedObject *> *pResults = querySynchronously(&queryCtx);

    MstpInstanceConfigManagedObject *instanceMO = NULL;
    if (pResults && !pResults->empty()) {
        instanceMO = dynamic_cast<MstpInstanceConfigManagedObject*>(pResults->front());
    }

    delete pResults;
    return instanceMO;
}

MstpInstanceConfigManagedObject* MstpToolKit::getMstpInstanceByVlanOid(const ObjectId &vlanOid, const string &selectField) {
    vector<string> fields;
    if (!selectField.empty()) fields.push_back(selectField);
    return getMstpInstanceByVlanOid(vlanOid, fields);
}

vector<UI32> MstpToolKit::getVlanIdsForVlanObjectIds (const vector<ObjectId> &oids, DceContext *pContext)
{
    vector<Attribute*> iidAttrs;
    for (UI32 i = 0; i < oids.size(); ++i) {
        iidAttrs.push_back(new AttributeUI64(oids[i].getInstanceId(), "objectIdinstanceid"));
    }

    mo_iterator<VlanIntfManagedObject> vlanIter ( pContext );
    vlanIter.criteria().select("id").orderBy("id")
            .And(new AttributeSqlIn(iidAttrs), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);

    vector<UI32> v;
    while (vlanIter.hasNext()) {
        v.push_back(vlanIter.next()->getId());
    }

    return v;
}

ResourceId MstpToolKit::verifyBridgePriority(UI32 priority)
{
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	if ((priority % MSP_DCM_BRIDGE_PRIORITY_MULTIPLIER) != 0)
		status = WRC_MSTP_BRIDGE_PRIORITY_ERROR; 
	return (status);
}

ResourceId MstpToolKit::verifyPortPriority(UI32 priority)
{
	ResourceId status = WAVE_MESSAGE_SUCCESS;
	if ((priority % MSTP_DCM_PORT_PRIORITY_MULTIPLIER) != 0)
		status = WRC_MSTP_PORT_PRIORITY_ERROR; 
	return (status);
}

ResourceId MstpToolKit::verifyFwdDlyMaxAgeHelloTime(UI32 forward_delay, UI32 max_age, UI32 hello_time)
{
	ResourceId status = WAVE_MESSAGE_SUCCESS;

	if ((max_age >= 2 * (hello_time + 1)) && ((2 * (forward_delay - 1 )) >= max_age))
		return (status);

	status = WRC_MSTP_MAXAGE_FWDDLY_HELLOTIME_MISMATCH; 
	return (status);
}

ResourceId MstpToolKit::validateVlanForXstpConfig(UI32 vid, DceContext *pSequencerContext)
{
    mo_criteria criteria (VlanIntfManagedObject::getClassName());
    criteria.And("id", vid);
    if (!criteria.queryExists()) {
        tracePrintf(TRACE_LEVEL_ERROR, "MstpToolKit::validateVlanForXstpConfig : VLAN %u does not exists", vid);
        return WRC_MSTP_VLAN_NOT_EXIST;
    }

    if (PVlanUtils::isSecondaryVlan(vid)) {
        tracePrintf(TRACE_LEVEL_ERROR, "MstpToolKit::validateVlanForXstpConfig : VLAN %u is a secondary private vlan; xSTP configs not allowed", vid);
        return WRC_MSTP_PVLAN_XSTP_SECVLAN;
    }

    if (NsmUtils::isRemoteSpanVlan(vid)) {
        tracePrintf(TRACE_LEVEL_ERROR, "MstpToolKit::validateVlanForXstpConfig : VLAN %u is a configured as remote span!", vid);
        return WRC_MSTP_REMOTE_SPAN_VLAN_ERROR;
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId MstpToolKit::validateVlanForXstpConfig(const UI32Range &vidRange, DceContext *pSequencerContext)
{
    vector<UI32> vids;
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (PVlanUtils::isSecondaryVlanRange(vidRange)) {
        tracePrintf(TRACE_LEVEL_ERROR, "MstpToolKit::validateVlanForXstpConfig : One or more vlans among '%s' are secondary private vlans..", vidRange.toString().c_str());
        return WRC_MSTP_PVLAN_MSTP_SECVLAN;
    }

    vidRange.getUI32RangeVector(vids);
    vector<Attribute*> vidAttrs;
    for (UI32 i = 0; i < vids.size(); ++i) {
        vidAttrs.push_back(new AttributeUI32(vids[i], "id"));
    }
    bool anyRemoteSpanEnabled = false;
    mo_criteria rspanCriteria ( VlanIntfManagedObject::getClassName());
    rspanCriteria.And(new AttributeSqlIn(vidAttrs), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN).And("remoteSpanEnabled", true);

    if ((status = rspanCriteria.queryExists(anyRemoteSpanEnabled)) == WAVE_MESSAGE_SUCCESS && anyRemoteSpanEnabled) {
        tracePrintf(TRACE_LEVEL_ERROR, "MstpToolKit::validateVlanForXstpConfig : One or more vlans among '%s' are remote span vlan/s..", vidRange.toString().c_str());
        status = WRC_MSTP_REMOTE_SPAN_VLAN_ERROR;
    }

    return status;
}



ResourceId MstpToolKit::mapBackEndErrorsToDcmdErrors(int err)
{
	switch(err)
	{
		case 0 : return (WAVE_MESSAGE_SUCCESS);
		case -1 : return (WAVE_MESSAGE_ERROR);
		default:
			return (err);
	}
	}


inline static const char* getMessageName (UI32 opcode)
{
    switch (opcode) {
    case MSTPBRIDGECONFIG:      return "MSTPBRIDGECONFIG";
    case MSTPVLANBRIDGECONFIG:  return "MSTPVLANBRIDGECONFIG";
    case MSTPBRIDGEINSTCONFIG:  return "MSTPBRIDGEINSTCONFIG";
    case MSTPPORTCONFIG:        return "MSTPPORTCONFIG";
    case MSTPPORTVLANCONFIG:    return "MSTPPORTVLANCONFIG";
    case MSTPPORTINSTCONFIG:    return "MSTPPORTINSTCONFIG";
    case MSTP_SHOW:             return "MSTP_SHOW";
    case MSTPCLEAR:             return "MSTPCLEAR";
    case MSTPVLANCONFIG:        return "MSTPVLANCONFIG";
    case MSTPSHOWGENERIC:       return "MSTPSHOWGENERIC";
    case MSTP_PORT_ID_CONFIG:   return "MSTP_PORT_ID_CONFIG";
    }
    return "UNKNOWN";
}

ResourceId MstpToolKit::sendToBackend (const char *name, DcmManagementInterfaceMessage *pMessage)
{
    UI32 opCode = pMessage->getOperationCode();
    const char *msgName = getMessageName(opCode);
    tracePrintf(TRACE_LEVEL_DEBUG, "MstpToolKit::sendToBackend : Sending message %s<%u> to %sd...", msgName, opCode, name);

    ResourceId status = sendSynchronouslyToWaveClient(name, pMessage);

    if (status != WAVE_MESSAGE_SUCCESS) {
        tracePrintf(TRACE_LEVEL_ERROR, "MstpToolKit::sendToBackend : Cannot send %s<%u> message to %sd.. err=%u ( %s )", msgName, opCode, name, status, FrameworkToolKit::localize(status).c_str());
        return status;
    }

    status = pMessage->getCompletionStatus();
    if (status != WAVE_MESSAGE_SUCCESS) {
        tracePrintf(TRACE_LEVEL_ERROR, "MstpToolKit::sendToBackend : Message %s<%u> processing failed in %sd.. err=%u ( %s )", msgName, opCode, name, status, FrameworkToolKit::localize(status).c_str());
        return status;
    }

    if (strcmp(name, MSTP_DCM_CLIENT_NAME) == 0) {
        status = mapBackEndErrorsToDcmdErrors(pMessage->getClientStatus());
        if (status != WAVE_MESSAGE_SUCCESS) {
            tracePrintf(TRACE_LEVEL_ERROR, "MstpToolKit::sendToBackend : %sd returned %d for message %s<%u>. err=%u ( %s )", name, pMessage->getClientStatus(), msgName, opCode, status, FrameworkToolKit::localize(status).c_str());
            return status;
        }
    }

    tracePrintf(TRACE_LEVEL_DEBUG, "MstpToolKit::sendToBackend : Message %s<%u> to %sd was successful", msgName, opCode, name);
    return status;
}


static inline Attribute *newClassIdAttr(const string &className) {
    return new AttributeUI32(OrmRepository::getTableId(className), "objectidclassid");
}

bool MstpToolKit::isAllowPortfastEdgeport(const string &keypath) {
    vector<Attribute*> typeFilter;
    if (keypath.find("/edgeport/") != string::npos) {
        typeFilter.push_back(newClassIdAttr(RstpBridgeConfigManagedObject::getClassName()));
        typeFilter.push_back(newClassIdAttr(MstpBridgeConfigManagedObject::getClassName()));
        typeFilter.push_back(newClassIdAttr(RpvstBridgeConfigManagedObject::getClassName()));
    } else if (keypath.find("/portfast/") != string::npos) {
        typeFilter.push_back(newClassIdAttr(StpBridgeConfigManagedObject::getClassName()));
        typeFilter.push_back(newClassIdAttr(PvstBridgeConfigManagedObject::getClassName()));
    } else {
        // Not a portfast or edgeport config
        return true;
    }

    UI32 count = 0;
    WaveManagedObjectSynchronousQueryContext queryContext(XstpBridgeConfigManagedObject::getClassName());
    queryContext.addAndAttribute(new AttributeSqlIn(typeFilter), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
    querySynchronouslyForCount(&queryContext, count);
    return count == 1;
}

ResourceId MstpToolKit::allocateVcsBridgeMac(MacAddress &mac)
{
    UI32 uid;
    ResourceId status = UidToolKit::alloc(uid, IDTYPE_BRIDGE_ID_LSB, XSTP_SYSID_LSB_UIDNAME);
    if (status == WAVE_MESSAGE_SUCCESS) {
        mac = getVcsBridgeMac((UI8)(uid & 0xFF));
    }
    return status;
}

ResourceId MstpToolKit::freeVcsbridgeMac()
{
    return UidToolKit::free(IDTYPE_BRIDGE_ID_LSB, XSTP_SYSID_LSB_UIDNAME);
}

static inline UI32 toIdType(int brMode)
{
    return (brMode == MSTP_DCM_BRIDGE_STP || brMode == MSTP_DCM_BRIDGE_PVSTP) ? IDTYPE_STP_PORT_ID : IDTYPE_RSTP_PORT_ID;
}

string MstpToolKit::toPortIdName (InterfaceType ifType, const string &ifName)
{
    return ifType == IF_TYPE_PO ? "po" + ifName : ifName;
}

ResourceId MstpToolKit::allocatePortId(UI32 &uidOut, InterfaceType ifType, const string &ifName, bool ignoreNoSpt, DceContext *pSequencerCtx)
{
    int brMode = MSTP_DCM_BRIDGE_NONE;
    ResourceId status = getBridgeMode(brMode, pSequencerCtx);
    if (status == WAVE_MESSAGE_SUCCESS) {
        if (brMode != MSTP_DCM_BRIDGE_NONE) {
            status = allocatePortId(uidOut, brMode, ifType, ifName);
        } else if (!ignoreNoSpt) {
            status = WRC_MSTP_NO_SPT_ERROR;
        }
    }
    return status;
}

ResourceId MstpToolKit::allocatePortId(UI32 &uidOut, int brMode, InterfaceType ifType, const string &ifName)
{
    return UidToolKit::alloc(uidOut, toIdType(brMode), toPortIdName(ifType, ifName));
}

ResourceId MstpToolKit::freePortId(InterfaceType ifType, const string &ifName, bool ignoreNoSpt, DceContext *pSequencerCtx)
{
    int brMode = MSTP_DCM_BRIDGE_NONE;
    ResourceId status = getBridgeMode(brMode, pSequencerCtx);
    if (status == WAVE_MESSAGE_SUCCESS) {
        if (brMode != MSTP_DCM_BRIDGE_NONE) {
            status = freePortId(brMode, ifType, ifName);
        } else if (!ignoreNoSpt) {
            status = WRC_MSTP_NO_SPT_ERROR;
        }
    }
    return status;
}

ResourceId MstpToolKit::freePortId(int brMode, InterfaceType ifType, const string &ifName)
{
    return UidToolKit::free(toIdType(brMode), toPortIdName(ifType, ifName));
}

MacAddress MstpToolKit::getVcsBridgeMac(UI8 lastByte)
{
    UI16 vcsId = (UI16) DcmToolKit::getVcsId();
    UI8 macBytes[6] = { 0x01, 0xE0, 0x52, (UI8)(vcsId >> 8), (UI8)(vcsId & 0xFF), lastByte };
    return MacAddress(macBytes);
}

void MstpToolKit::logBridgeMacAllocError (ResourceId err, const char *context)
{
    tracePrintf(TRACE_LEVEL_ERROR, true, false, "MstpToolKit::%s : Could not allocate bridge id during %s; err=%u ( %s ). Going ahed with protocol configuration with shut state!!",
            __FUNCTION__, context, err, FrameworkToolKit::localizeToSourceCodeEnum(err).c_str());

    // Write a raslog to notify the admin
    raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, MSTP_3001);
}

void MstpToolKit::logPortIdAllocError (ResourceId err, const char *context, InterfaceType ifType, const string &ifName)
{
    string name = NsmUtils::getIfShortName(ifType, ifName);
    tracePrintf(TRACE_LEVEL_ERROR, true, false, "MstpToolKit::%s : Could not allocate port id during %s; ifName=%s, err=%u ( %s ). Going ahead with interface configuration with spanning tree shut state!!",
            __FUNCTION__, context, name.c_str(), err, FrameworkToolKit::localizeToSourceCodeEnum(err).c_str());

    // Write a raslog to notify the admin
    if (err == WRC_UIDSVC_ID_EXHAUSTED_ERROR) {
        raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, MSTP_3003, name.c_str());
    } else {
        raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_FRCLOG, MSTP_3002, name.c_str());
    }
}


bool MstpToolKit::isVcsEnabled()
{
    return DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED;
}

vector<LocationId> MstpToolKit::getDisconnectedLocations ()
{
    vector<LocationId> allLocations, disconnectedLocations;
    LocationId loc;
    FrameworkToolKit::getKnownLocations(allLocations);
    for (UI32 i = 0; i < allLocations.size(); ++i) {
        loc = allLocations[i];
        if (!FrameworkToolKit::isAConnectedLocation(loc))
            disconnectedLocations.push_back(loc);
    }
    return disconnectedLocations;
}

bool MstpToolKit::isXstpConfigPresent(UI32 vlanId)
{
    WaveManagedObjectSynchronousQueryContext queryCtx(VlanBridgeStpConfigManagedObject::getClassName());
    queryCtx.addAndAttribute (new AttributeUI32(&vlanId, "vlan"));
    UI32 moCount = 0;
    querySynchronouslyForCount(&queryCtx, moCount);
    tracePrintf(TRACE_LEVEL_DEBUG, "isXstpConfigPresent: Query returned %d count", moCount);
    if(moCount > 0){
    	return true;
    }
	return false;
}

ResourceId MstpToolKit::isMstpEnabled(bool &isEnabled)
{
	mo_criteria criteria (MstpBridgeConfigManagedObject::getClassName());
	return criteria.And("stpDisable", false).queryExists(isEnabled);
}

ResourceId MstpToolKit::checkSTPEnabledGvlanExists(UI32 &stpEnabledGvlanCount) {
    	WaveManagedObjectSynchronousQueryContext queryCtx(VlanIntfManagedObject::getClassName());
    	queryCtx.addAndAttribute(new AttributeBool(false, "stpDisable"));
    	queryCtx.addAndAttribute(new AttributeUI32(MAX_DOT1Q_VLAN_ID, "id"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN);
    	return querySynchronouslyForCount(&queryCtx, stpEnabledGvlanCount);
}
}	// namespace DcmNs

