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

/* **
 * IntfXstpBaseWorker.cpp
 *
 * May 2, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include <algorithm>

#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "Nsm/Common/IntfXstpBaseWorker.h"
#include "Nsm/Common/NsmPortStpConfigMessage.h"
#include "Nsm/Common/NsmUpdateMstpInstanceConfigMessage.h"
#include "Nsm/Common/PVlanUtils.h"
#include "APPM/Utils/APPMUtils.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/XstpPortVlanConfigManagedObject.h"
#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "Nsm/Local/XstpPortInstanceConfigManagedObject.h"
#include "Nsm/Local/OneGigLocalManagedObject.h"
#include "Nsm/Local/TenGigLocalManagedObject.h"
#include "Nsm/Local/FortyGigLocalManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Mstp/Global/MstpInstanceConfigManagedObject.h"
#include "UidService/Global/UidServiceQueryMessage.h"
#include "UidService/Common/UidToolKit.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
#include "Utils/mo_iterator.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeSqlIn.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "WaveResourceIdEnums.h"
#include "DcmResourceIds.h"


using namespace WaveNs;

namespace DcmNs {

#define trace_print( _LVL, _FMT) \
    tracePrintf(_LVL, "IntfXstpBaseWorker::%s : " _FMT, __FUNCTION__)

#define trace_printf( _LVL, _FMT, ...) \
    tracePrintf(_LVL, true, false, "IntfXstpBaseWorker::%s : " _FMT, __FUNCTION__, ##__VA_ARGS__)


#define IS_NSM_LOCAL_CONTEXT()   (m_targetIntfClass == PhyIntfLocalManagedObject::getClassName())

#define IS_NSM_GLOBAL_CONTEXT()  (m_targetIntfClass == PoIntfManagedObject::getClassName())

#define DELETED_VLAN_ID_KEY     "xstp.vlanBeingDeleted.id"
#define DELETED_VLAN_OID_KEY    "xstp.vlanBeingDeleted.oid"

#define VECTOR_CONTAINS( _V, _X )   (find(_V.begin(), _V.end(), (_X)) != _V.end())

template <class T>
static bool containsAny (vector<T> lhs, vector<T> rhs)
{
    for (UI32 i = 0; i < lhs.size(); ++i) {
        if (VECTOR_CONTAINS(rhs, lhs[i])) return true;
    }
    return false;
}

template <class T>
static bool containsAll(vector<T> lhs, vector<T> rhs)
{
    for (UI32 i = 0; i < rhs.size(); ++i) {
        if (!VECTOR_CONTAINS(lhs, rhs[i])) return false;
    }
    return true;
}


static inline string getIntfClass (const InterfaceType ifType)
{
    switch (ifType) {
    case IF_TYPE_GI  : return OneGigLocalManagedObject::getClassName();
    case IF_TYPE_TE  : return TenGigLocalManagedObject::getClassName();
    case IF_TYPE_FO  : return FortyGigLocalManagedObject::getClassName();
    case IF_TYPE_PHY : return PhyIntfLocalManagedObject::getClassName();
    case IF_TYPE_PO  : return PoIntfManagedObject::getClassName();
    default          : return "";
    }
}


IntfXstpBaseWorker::IntfXstpBaseWorker (WaveObjectManager *objectMgr, InterfaceType targetIfType)
        : WaveWorker ( objectMgr )
{
    m_targetIntfClass = getIntfClass(targetIfType);
}

IntfXstpBaseWorker::~IntfXstpBaseWorker ()
{
}

ResourceId IntfXstpBaseWorker::validatePortXstpConfig (NsmPortStpConfigMessage *message, DceContext *pContext)
{
    if (message == NULL) {
        trace_print(TRACE_LEVEL_ERROR, " ******* Not a NsmPortStpConfigMessage!!");
        return WAVE_MESSAGE_ERROR;
    }

    InterfaceType ifType = message->getIfType();
    string ifName = message->getIfName();

    mo_iterator<PoPhyIntfManagedObject> ifIter ( getIntfClass(ifType), pContext );
    ifIter.criteria().And("id", ifName).select("swModeFlag").select("isPortProfiled").
        select("associatedPortProfiles").select("associatedPortProfileDomain").select("vlanid").select("pVlanModeHost").select("pVlanModeTrunkHost");
    PoPhyIntfManagedObject *intfMo = ifIter.next();
    if (intfMo == NULL) {
        trace_printf(TRACE_LEVEL_ERROR, "Interface %s no found!", ifName.c_str());
        return WAVE_MESSAGE_ERROR;
    }

    if(intfMo->getPvlanModeHost() == true || intfMo->getPvlanModeTrunkHost() == true){
        return WRC_MSTP_PVLAN_STP_HOST_MODE;
    }

    if (!intfMo->getSwModeFlag()) {
        bool isPortProfiled = false;
        APPMUtils::isPortProfilePort(intfMo, isPortProfiled);        
        if (isPortProfiled) {
            trace_printf(TRACE_LEVEL_ERROR, "Interface %s is port profile port.", ifName.c_str());
            return WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED;
        } else {
            trace_printf(TRACE_LEVEL_ERROR, "Interface %s is not in L2 mode!!", ifName.c_str());
            return WRC_NSM_ERR_INTF_NOT_MODE_L2;
        }
    }

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    switch (message->getCmdCode()) {
    case MSTP_MSG_IF_PROTO_ENABLE:
        // STP cannot be enabled on interfaces running ELD
        {
            Attribute *attr = intfMo->getAttributeByName("vlanid");
            vector<ObjectId> *eldVlans = (attr == NULL) ? NULL
                    : reinterpret_cast<vector<ObjectId> *>(attr->getPData());

            if (eldVlans != NULL && !eldVlans->empty()) {
                trace_printf(TRACE_LEVEL_ERROR, "ELD is enabled on %s; cannot enable spanning-tree!", ifName.c_str());
                status = WRC_MSTP_PORT_ELD_ENABLED_ERROR;
            }
        }
        break;

    case DCM_SSM_BPDUGUARD_ENABLE:
    case DCM_SSM_TAGGED_BPDU_ENABLE:
        // Bpdu-drop or tagged-bpdu configs are not allowed when spanning tree is configured..
        {
            bool sptConfigured = false;
            status = MstpToolKit::isSpanningTreeConfigured(sptConfigured);
            if (status == WAVE_MESSAGE_SUCCESS && sptConfigured) {
                trace_printf(TRACE_LEVEL_ERROR, "Spanning tree is configured on %s; bpdu-drop or tagged-bpdu configs are not allowed!!", ifName.c_str());
                status = WRC_NOT_ALLOWED_WITH_SPT;
            }
        }
        break;
    }

    if (pContext != NULL) {
        pContext->setWaveManagedObjectId(intfMo->getObjectId());
    }

    return status;
}

ResourceId IntfXstpBaseWorker::updateXstpPortConfigMO(NsmPortStpConfigMessage *pMessage, DceContext *pContext)
{
    string ifName = pMessage->getIfName();
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace_printf(TRACE_LEVEL_INFO, "port=%s", ifName.c_str());

    mo_iterator<XstpPortConfigManagedObject> moIter ( pContext );
    moIter.criteria().And("port", ifName);
    XstpPortConfigManagedObject *intfStpMo = moIter.next();

    if (intfStpMo != NULL) {
        status = fillMo(intfStpMo, pMessage);
        if (status == WAVE_MESSAGE_SUCCESS) {
            int uid = pContext->getCache()->getIntValue(PORT_UID_KEY);
            if (uid > 0) {
                intfStpMo->setPortUid((UI32) uid);
            }

            updateWaveManagedObject(intfStpMo);
        }
    } else {
        // *** Should not happen in today's design ****
        trace_printf(TRACE_LEVEL_ERROR, "Could not find XstpPortConfigManagedObject for port %s", ifName.c_str());
        status = WAVE_MESSAGE_ERROR;
    }

    return status;
}

ResourceId IntfXstpBaseWorker::updateXstpPortVlanMO(UI32 vid, NsmPortStpConfigMessage *pMessage, DceContext *pContext)
{
    string ifName = pMessage->getIfName();
    ObjectId intfOid = pContext->getWaveManagedObjectId();

    trace_printf(TRACE_LEVEL_INFO, "port=%s, vid=%u", ifName.c_str(), vid);

    mo_iterator<XstpPortVlanConfigManagedObject> moIter ( pContext );
    moIter.criteria().And("port", ifName).And("vlanId", vid);

    XstpPortVlanConfigManagedObject *portVlanMo = moIter.next();

    if (portVlanMo == NULL) {
        trace_print(TRACE_LEVEL_DEBUG, "XstpPortVlanConfigManagedObject does not exist");

        if (XstpPortVlanConfigManagedObject::isConfigDefault(pMessage)) {
            trace_print(TRACE_LEVEL_DEBUG, "default config, skip MO creation..");
            return WAVE_MESSAGE_SUCCESS;
        }

        mo_iterator<VlanIntfManagedObject> vlanIter ( pContext );
        vlanIter.criteria().select("objectId").And("id", vid);
        if (!vlanIter.hasNext()) {
            trace_printf(TRACE_LEVEL_ERROR, "Vlan %u does not exist!!", vid);
            return WRC_MSTP_VLAN_NOT_EXIST;
        }

        ObjectId vlanOid = vlanIter.next()->getObjectId();

        portVlanMo = new XstpPortVlanConfigManagedObject(getPWaveObjectManager());
        portVlanMo->setDefaultAttribute(vid, vlanOid, ifName);
        portVlanMo->setAttributeByMessage(pMessage);

        portVlanMo->setOwnerManagedObjectId(intfOid);
        addToComposition(PoPhyIntfManagedObject::getClassName(), XstpPortVlanConfigManagedObject::getClassName(),
                "portVlanXstp", intfOid, portVlanMo->getObjectId());

        pContext->addManagedObjectForGarbageCollection(portVlanMo);
        return WAVE_MESSAGE_SUCCESS;
    }

    portVlanMo->setAttributeByMessage(pMessage);
    if (portVlanMo->isDefault()) {
        trace_print(TRACE_LEVEL_DEBUG, "Updating to default config, deleteing the MO");
        deleteFromComposition(PoPhyIntfManagedObject::getClassName(), XstpPortVlanConfigManagedObject::getClassName(),
                "portVlanXstp", intfOid, portVlanMo->getObjectId());
    } else {
        updateWaveManagedObject(portVlanMo);
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfXstpBaseWorker::updateXstpPortInstanceMO(UI32 instId, NsmPortStpConfigMessage *pMessage, DceContext *pContext)
{
    string ifName = pMessage->getIfName();
    ObjectId intfOid = pContext->getWaveManagedObjectId();

    trace_printf(TRACE_LEVEL_INFO, "port=%s, instId=%u", ifName.c_str(), instId);

    if (intfOid == ObjectId::NullObjectId) {
        trace_print(TRACE_LEVEL_ERROR, "*** null ObjectId *** Pls fill interface ObjectId in the sequencer context..");
        return WAVE_MESSAGE_ERROR;
    }

    mo_iterator<XstpPortInstanceConfigManagedObject> moIter ( pContext );
    moIter.criteria().And("port", ifName).And("id", instId);

    XstpPortInstanceConfigManagedObject *pPortInstMo = moIter.next();
    if (pPortInstMo == NULL) {
        trace_print(TRACE_LEVEL_DEBUG, "No matching XstpPortInstanceConfigManagedObject, creating new one...");

        mo_iterator<MstpInstanceConfigManagedObject> mstiIter ( pContext );
        mstiIter.criteria().And("instanceId", instId).select("objectId");
        if (!mstiIter.hasNext()) {
            trace_printf(TRACE_LEVEL_ERROR, "MSTI %u does not exist!", instId);
            return WRC_MSTP_INST_NOT_EXIST;
        }

        UI8 instanceId = (UI8) instId;
        ObjectId instanceOid = mstiIter.next()->getObjectId();
        pPortInstMo = new XstpPortInstanceConfigManagedObject(getPWaveObjectManager());
        pPortInstMo->setDefaultAttribute(instanceId, instanceOid, ifName);
        fillMo(pPortInstMo, pMessage);

        pPortInstMo->setOwnerManagedObjectId(intfOid);
        addToComposition(PoPhyIntfManagedObject::getClassName(), XstpPortInstanceConfigManagedObject::getClassName(),
                "portInstanceXstp", intfOid, pPortInstMo->getObjectId());

        pContext->addManagedObjectForGarbageCollection(pPortInstMo);
        return WAVE_MESSAGE_SUCCESS;
    }

    fillMo(pPortInstMo, pMessage);

    if (pPortInstMo->isDefault()) {
        trace_print(TRACE_LEVEL_DEBUG, "Updating to default config, deleteing the MO");
        deleteFromComposition(PoPhyIntfManagedObject::getClassName(), XstpPortInstanceConfigManagedObject::getClassName(),
                "portInstanceXstp", intfOid, pPortInstMo->getObjectId());
    } else {
        updateWaveManagedObject(pPortInstMo);
    }

    return WAVE_MESSAGE_SUCCESS;
}

void IntfXstpBaseWorker::NsmUpdateMstpInstanceConfigMessageHandler(NsmUpdateMstpInstanceConfigMessage *pMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] = {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpBaseWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpBaseWorker::handleInstanceVlanAssocUpdateStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpBaseWorker::prismSynchronousLinearSequencerCommitTransactionStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpBaseWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpBaseWorker::prismSynchronousLinearSequencerFailedStep)
    };

    DceSynchronousLinearSeqContext *pSequencerContext = new DceSynchronousLinearSeqContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pSequencerContext->execute();
}

ResourceId IntfXstpBaseWorker::handleInstanceVlanAssocUpdateStep (DceSynchronousLinearSeqContext *pSequencerContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    NsmUpdateMstpInstanceConfigMessage *pMessage = dynamic_cast<NsmUpdateMstpInstanceConfigMessage*> (pSequencerContext->getPPrismMessage());

    UI32 inst = pMessage->getInstanceId();
    vector<ObjectId> vlans = pMessage->getInstanceVlans();
    trace_printf(TRACE_LEVEL_DEBUG, "instanceId=%u, memebr vlan count=%u", inst, vlans.size());

    if (vlans.empty()) {
        trace_printf(TRACE_LEVEL_DEBUG, "Instance %u being deleted; clear all XstpPortInstanceMO objects...", inst);
        status = deleteXstpPortInstanceMO (inst, pSequencerContext);
    } else {
        trace_printf(TRACE_LEVEL_DEBUG, "Instance %u vlan association changed; remove irrelevant XstpPortInstanceMO objects..", inst);
        status = handleInstanceVlanAssocUpdate(inst, vlans, pSequencerContext);
    }

    return (status);
}

ResourceId IntfXstpBaseWorker::handleInstanceVlanAssocUpdate (UI32 instId, vector<ObjectId> newInstVlans, DceContext *pContext)
{
    mo_iterator<XstpPortInstanceConfigManagedObject> portInstIter ( pContext );
    portInstIter.criteria().And("id", instId).select("port").select("ownerManagedObjectId");
    fillIntfXstpQueryCriteria(portInstIter.criteria().getQueryContext());
    if (!portInstIter.hasNext()) {
        trace_printf(TRACE_LEVEL_DEBUG, "No XstpPortInstanceConfigManagedObject for instance %u", instId);
        return WAVE_MESSAGE_SUCCESS;
    }

    vector<Attribute*> ifNameAttrs;
    map<string, ObjectId> ifNameToPortInstOid;

    while (portInstIter.hasNext()) {
        XstpPortInstanceConfigManagedObject *mo = portInstIter.next();
        //ifNameAttrs.push_back(new AttributeString(mo->getPort(), "id"));
        ifNameAttrs.push_back(new AttributeUI64(mo->getOwnerManagedObjectId().getInstanceId(), "objectIdInstanceId"));
        ifNameToPortInstOid[mo->getPort()] = mo->getObjectId();
    }

    mo_iterator<PoPhyIntfManagedObject> intfIter ( m_targetIntfClass, pContext );
    intfIter.criteria().select("ifName").select("swMode").select("vlanObjectId").select("vlanRangeAdd").select("vlanRangeRemove")
            .And(new AttributeSqlIn(ifNameAttrs), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);

    UI32Range newVlanIds;
    ObjectId vlanBeingDeleted = pContext->getCache()->getObjectId(DELETED_VLAN_OID_KEY);
    ObjectId defaultVlanOid;

    while (intfIter.hasNext()) {
        PoPhyIntfManagedObject *intfMo = intfIter.next();
        string ifName = intfMo->getIfName();
        bool deletePortInst = false;

        if (intfMo->getSwMode() == SW_MODE_ACCESS) {
            ObjectId accessVlanOid = intfMo->getAccessVlanObjectId();
            if (accessVlanOid == vlanBeingDeleted) {
                if (defaultVlanOid == ObjectId::NullObjectId) {
                    mo_iterator<VlanIntfManagedObject> vlanIter ( pContext );
                    vlanIter.criteria().And("id", (UI32) 1).select("objectId");
                    if (vlanIter.hasNext()) defaultVlanOid = vlanIter.next()->getObjectId();
                }

                accessVlanOid = defaultVlanOid;
            }

            // TODO move this to query
            deletePortInst = (find(newInstVlans.begin(), newInstVlans.end(), accessVlanOid) == newInstVlans.end());
        }
        else if (intfMo->getSwMode() == SW_MODE_TRUNK) {
            if (newVlanIds.isEmpty()) {
                vector<UI32> v = MstpToolKit::getVlanIdsForVlanObjectIds(newInstVlans, pContext);
                newVlanIds.fromString(UI32Range::getUI32RangeStringFromVector(v));
            }

            if (!intfMo->m_vlanRangeAdd.isEmpty()) {
                deletePortInst = !intfMo->m_vlanRangeAdd.containsAny(newVlanIds);
            }
            else if (!intfMo->m_vlanRangeRemove.isEmpty()) {
                deletePortInst = intfMo->m_vlanRangeRemove.containsAll(newVlanIds);
            }
        }

        if (deletePortInst) {
            trace_printf(TRACE_LEVEL_DEBUG, "Deleting XstpPortInstanceConfigManagedObject for interface %s", ifName.c_str());
            deleteFromComposition(PoPhyIntfManagedObject::getClassName(), XstpPortInstanceConfigManagedObject::getClassName(),
                    "portInstanceXstp", intfMo->getObjectId(), ifNameToPortInstOid[ifName]);
        }
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfXstpBaseWorker::deleteXstpPortInstanceMO (UI32 instId, DceContext *pContext)
{
    mo_iterator<XstpPortInstanceConfigManagedObject> portInstIter ( pContext );
    mo_criteria &criteria = portInstIter.criteria();
    fillIntfXstpQueryCriteria(criteria.getQueryContext());
    if (instId > 0) {
        criteria.And("id", instId);
    }
    criteria.select("port").select("ownerManagedObjectId").select("objectId");

    while (portInstIter.hasNext()) {
        XstpPortInstanceConfigManagedObject *mo = portInstIter.next();

        trace_printf(TRACE_LEVEL_DEBUG, "Deleting XstpPortInstanceConfigManagedObject for interface %s", mo->getPort().c_str());
        deleteFromComposition(PoPhyIntfManagedObject::getClassName(), XstpPortInstanceConfigManagedObject::getClassName(),
                "portInstanceXstp", mo->getOwnerManagedObjectId(), mo->getObjectId());
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfXstpBaseWorker::deleteXstpPortVlanMO (const string &ifName, const vector<UI32> &vids, DceContext *pSequencerContext)
{
    mo_iterator<XstpPortVlanConfigManagedObject> ifVlanXstpIter ( pSequencerContext );
    mo_criteria &criteria = ifVlanXstpIter.criteria();
    fillIntfXstpQueryCriteria(criteria.getQueryContext(), ifName);

    if (vids.size() == 1) {
        criteria.And("vlanId", vids[0]);
    } else if (!vids.empty()) {
        vector<Attribute*> inValues (vids.size());
        for (UI32 i = 0; i < vids.size(); ++i) {
            inValues.push_back(new AttributeUI32(vids[i], "vlanId"));
        }
        criteria.And(new AttributeSqlIn(inValues), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
    }

    criteria.select("port").select("vlanId").select("ownerManagedObjectId");

    while (ifVlanXstpIter.hasNext()) {
        XstpPortVlanConfigManagedObject *pPortVlan =  ifVlanXstpIter.next();

        trace_printf(TRACE_LEVEL_DEBUG, "Removing port vlan configs for port %s; vlan id %u", pPortVlan->getPort().c_str(), pPortVlan->getId());
        deleteFromComposition(PoPhyIntfManagedObject::getClassName(), XstpPortVlanConfigManagedObject::getClassName(),
                "portVlanXstp", pPortVlan->getOwnerManagedObjectId(), pPortVlan->getObjectId());
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfXstpBaseWorker::handleVlanDelete (UI32 vid, const ObjectId &vlanOid, DceContext *pSequencerContext)
{
    // Clear all XstpPortVlanConfigManagedObject for the vlan id
    vector<UI32> vids ( &vid, &vid+1 );
    deleteXstpPortVlanMO("", vids, pSequencerContext);

    // Remove affected XstpPortInstanceConfigManagedObject ...

    MstpInstanceConfigManagedObject *instConfigMo = MstpToolKit::getMstpInstanceByVlanOid(vlanOid);
    if (instConfigMo == NULL) {
        // Not in MSTP mode or no MSTI is associated with this vlan ...
        trace_printf(TRACE_LEVEL_DEBUG, "no MSTP instance is affected by vlan %u deletion!", vid);
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    UI32 instId = (UI32) instConfigMo->getInstanceId();

    // Compute new set of vlans for this MSTI
    vector<ObjectId> vlans = instConfigMo->getInstanceVlans();
    vlans.erase(find(vlans.begin(), vlans.end(), vlanOid));

    if (vlans.empty()) {
        trace_printf(TRACE_LEVEL_DEBUG, "%u is the only vlan associated with MSTI %u", vid, instId);
        status = deleteXstpPortInstanceMO(instId, pSequencerContext);
    }
    else {
        pSequencerContext->getCache()->put(DELETED_VLAN_OID_KEY, vlanOid);
        status = handleInstanceVlanAssocUpdate(instId, vlans, pSequencerContext);
    }

    pSequencerContext->addManagedObjectForGarbageCollection(instConfigMo);
    return status;
}

ResourceId IntfXstpBaseWorker::clearIntfXstpConfigs (const string &ifName, bool resetShutState, DceContext *pSequencerContext)
{
    trace_printf(TRACE_LEVEL_DEBUG, "Reset port xSTP settings for ifName=\"%s\", class=%s, resetShutState=%u",
            ifName.c_str(), m_targetIntfClass.c_str(), resetShutState);

    WaveManagedObjectSynchronousQueryContextForDeletion *deleteCtx =
            new WaveManagedObjectSynchronousQueryContextForDeletion(m_targetIntfClass);

    if (IS_NSM_LOCAL_CONTEXT()) {
        LocationId loc = FrameworkToolKit::getThisLocationId();
        trace_printf(TRACE_LEVEL_DEBUG, "Location id filter = %u", loc);
        deleteCtx->setLocationIdFilter(loc);
    }

    if (!ifName.empty()) {
        deleteCtx->addAndAttribute(new AttributeString(ifName, "id"));
    }

    set<string> childObjects;
    childObjects.insert("portVlanXstp");
    childObjects.insert("portInstanceXstp");
    deleteManagedObjectCompositions(deleteCtx, childObjects);

    WaveManagedObjectSynchronousQueryContextForUpdate *updateCtx =
            new WaveManagedObjectSynchronousQueryContextForUpdate (XstpPortConfigManagedObject::getClassName());
    fillIntfXstpQueryCriteria(updateCtx, ifName);
    XstpPortConfigManagedObject *criteriaMo = dynamic_cast<XstpPortConfigManagedObject*>(updateCtx->getWaveManagedObjectToAddUpdateAttributes());
    if (criteriaMo != NULL) {
        criteriaMo->setDefaultAttribute(resetShutState, true);
        updateMultipleWaveManagedObjects(updateCtx);
    }
    else {
        trace_print(TRACE_LEVEL_ERROR, "***** NULL criteriaMo *****");
        return WAVE_MESSAGE_ERROR;
    }

    return WAVE_MESSAGE_SUCCESS;
}


ResourceId IntfXstpBaseWorker::handleIntfVlanAssocUpdate (const string &ifName, VlanIdList &vlist, DceContext *pSequencerContext)
{
    // TODO have optimized implementation..
    mo_iterator<PoPhyIntfManagedObject> ifIter ( pSequencerContext );
    ifIter.criteria().And("ifName", ifName).select("ifName").select("portVlanXstp").select("portInstanceXstp");
    PoPhyIntfManagedObject *pIntfMo = ifIter.next();

    if (pIntfMo == NULL) {
        trace_printf(TRACE_LEVEL_ERROR, "Interafce not found: %s", ifName.c_str());
        return WAVE_MESSAGE_ERROR;
    }

    ResourceId status = handleIntfVlanAssocUpdate(pIntfMo, vlist, pSequencerContext, true);

    if (status == WAVE_MESSAGE_SUCCESS &&
            !pIntfMo->getAttributesToBeUpdated().empty()) {
        updateWaveManagedObject(pIntfMo);
    }

    return status;
}

ResourceId IntfXstpBaseWorker::handleIntfVlanAssocUpdate (PoPhyIntfManagedObject *pIntfMo, VlanIdList &vlist, DceContext *pSequencerContext, bool recordUpdatedFields)
{
    bool emptyList = vlist.m_vlanOids.empty() && vlist.m_vlanIds.empty();
    if (vlist.m_type == NONMEMBER_VLAN_LIST && emptyList) {
        // All vlans are included..
        return WAVE_MESSAGE_SUCCESS;
    }

    vector<WaveManagedObjectPointer<XstpPortVlanConfigManagedObject> > &ifXstpVlanMos = pIntfMo->m_portVlanXstpConfig;
    vector<WaveManagedObjectPointer<XstpPortInstanceConfigManagedObject> > &ifXstpInstMos = pIntfMo->m_portInstanceXstpConfig;

    if (!ifXstpVlanMos.empty()) {
        bool updated = false;
        bool toDelete = (vlist.m_type == NONMEMBER_VLAN_LIST);
        bool compareOids = !vlist.m_vlanOids.empty();
        bool compareIds = !vlist.m_vlanIds.empty();
        XstpPortVlanConfigManagedObject *pPortCfgMo;

        for (int i = ifXstpVlanMos.size()-1; i >= 0; --i) {
            pPortCfgMo = ifXstpVlanMos[i].operator->();
            if (( compareOids && VECTOR_CONTAINS(vlist.m_vlanOids, pPortCfgMo->getVlanObjectId()) == toDelete )
                    || ( compareIds && VECTOR_CONTAINS(vlist.m_vlanIds, pPortCfgMo->getId()) == toDelete )) {
                ifXstpVlanMos.erase( ifXstpVlanMos.begin() + i );
                updated = true;
            }
        }

        if (updated && recordUpdatedFields) {
            pIntfMo->addAttributeToBeUpdated("portVlanXstp");
        }
    } else if (!ifXstpInstMos.empty()) {
        // Find the VLAN object ids if not specified
        // TODO avoid this id to oid conversion by having the vlan id range in the MstpInstanceMO
        if (!vlist.m_vlanIds.empty()) {
            vector<WaveManagedObject*> *pVlanMos = NsmUtils::queryVlanMo(vlist.m_vlanIds, "objectId");
            if (pVlanMos != NULL) {
                for (UI32 i = 0; i < pVlanMos->size(); ++i) {
                    WaveManagedObject *mo = pVlanMos->at(i);
                    vlist.m_vlanOids.push_back(mo->getObjectId());
                }

                pSequencerContext->addManagedObjectsForGarbageCollection(*pVlanMos);
                delete pVlanMos;
            }
        }

        vector<Attribute*> instOidAttrs;
        for (UI32 i = 0; i < ifXstpInstMos.size(); ++i) {
            instOidAttrs.push_back(
                    new AttributeUI64(ifXstpInstMos[i]->getInstanceId().getInstanceId(), "objectidinstanceid"));
        }

        bool updated = false;
        mo_iterator<MstpInstanceConfigManagedObject> instIter (pSequencerContext);
        instIter.criteria().select("vlanid").And(new AttributeSqlIn(instOidAttrs), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);

        while (instIter.hasNext()) {
            MstpInstanceConfigManagedObject *pMstiMo = instIter.next();
            if (vlist.m_type == MEMBER_VLAN_LIST ?
                    (!containsAny(vlist.m_vlanOids, pMstiMo->m_instanceVlans))
                    : containsAll(vlist.m_vlanOids, pMstiMo->m_instanceVlans)) {
                pIntfMo->deletePortInstanceXstpConfig(pMstiMo->getObjectId());
                updated = true;
            }
        }

        if (updated && recordUpdatedFields) {
            pIntfMo->addAttributeToBeUpdated("portInstanceXstp");
        }
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfXstpBaseWorker::clearXstpPortInstAndPortVlanMOs (PoPhyIntfManagedObject *pIntfMo, bool recordUpdatedFields)
{
    if (!pIntfMo->m_portVlanXstpConfig.empty()) {
        pIntfMo->deletePortVlanXstpConfigAll();
        if (recordUpdatedFields)
            pIntfMo->addAttributeToBeUpdated("portVlanXstp");
    }

    if (!pIntfMo->m_portInstanceXstpConfig.empty()) {
        pIntfMo->deletePortInstanceXstpConfigAll();
        if (recordUpdatedFields)
            pIntfMo->addAttributeToBeUpdated("portInstanceXstp");
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId IntfXstpBaseWorker::clearXstpPortInstAndPortVlanMOs (const string &ifName, DceContext *pSequencerContext)
{
    WaveManagedObjectSynchronousQueryContextForDeletion *deleteCtx =
            new WaveManagedObjectSynchronousQueryContextForDeletion(m_targetIntfClass);

    deleteCtx->addAndAttribute(new AttributeString(ifName, "id"));

    set<string> childObjects;
    childObjects.insert("portVlanXstp");
    childObjects.insert("portInstanceXstp");
    deleteManagedObjectCompositions(deleteCtx, childObjects);

    return WAVE_MESSAGE_SUCCESS;
}


void IntfXstpBaseWorker::fillIntfXstpQueryCriteria(WaveManagedObjectSynchronousQueryContext *qc, const string &ifName)
{
    if (!ifName.empty()) {
        qc->addAndAttribute(new AttributeString(ifName, "port"));
    }
    else if (!IS_NSM_LOCAL_CONTEXT()) {
        UI32 poClassId = OrmRepository::getTableId(PoIntfManagedObject::getClassName());
        qc->addAndAttribute(new AttributeUI32(poClassId, "ownerManagedObjectIdclassId"));
    }
    else if (MstpToolKit::isVcsEnabled()) {
        int mappedId = DcmToolKit::getMappedIdFromLocationId(FrameworkToolKit::getThisLocationId());
        trace_printf(TRACE_LEVEL_DEBUG, "My mapped id = %d", mappedId);
        char pattern [16];
        snprintf(pattern, sizeof pattern, "%d/%%", mappedId);
        qc->addAndAttribute(new AttributeString(pattern, "port"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
    }
    else {
        UI32 poClassId = OrmRepository::getTableId(PoIntfManagedObject::getClassName());
        qc->addAndAttribute(new AttributeUI32(poClassId, "ownerManagedObjectIdclassId"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);
    }
}

void IntfXstpBaseWorker::failover (IntfXstpFailoverContext &ctx)
{
    int brMode = MSTP_DCM_BRIDGE_NONE;
    ResourceId status = MstpToolKit::getBridgeMode(brMode);

    if (status != WAVE_MESSAGE_SUCCESS) {
        trace_printf(TRACE_LEVEL_ERROR, "Error querying bridge mode.. err=%u", status);
        return;
    }
    if (brMode == MSTP_DCM_BRIDGE_NONE) {
        trace_print(TRACE_LEVEL_DEBUG, "Spanning tree not enabled.. nothing to cleanup!");
        return;
    }

    char buff[32];
    UidServiceQueryMessage msg (UID_FREE_BULK);

    for (UI32 i = 0; i < ctx.m_phyIntfNames.size(); ++i) {
        // hopefully no one will use actual if type to format the uid name
        msg.addEntityName(ctx.m_phyIntfNames[i]);
    }

    for (set<SI32>::iterator iter = ctx.m_rbridges.begin(); iter != ctx.m_rbridges.end(); ++iter) {
        snprintf(buff, sizeof(buff), "%d/%%", *iter);
        msg.addEntityName(string(buff));
    }

    // Done with physical interfaces.. Now cleanup port ids for port channels..
    if (! ctx.m_poObjectIds.empty()) {

    vector<Attribute*> iidAttribs;
    set<UI64>::iterator poEndIter = ctx.m_poObjectIds.end();
    for (set<UI64>::iterator iter = ctx.m_poObjectIds.begin(); iter != poEndIter; ++iter) {
        iidAttribs.push_back(new AttributeUI64(*iter, "ownerManagedObjectIdInstanceId"));
    }

    mo_iterator<XstpPortConfigManagedObject> poXstpIter;
    poXstpIter.criteria().select("port").select("ownerManagedObjectId")
                         .And("stpDisable", false)
                         .And(new AttributeSqlIn(iidAttribs), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);

    while (poXstpIter.hasNext()) {
        XstpPortConfigManagedObject *pPortXstpMo = poXstpIter.next();
        string poName = pPortXstpMo->getPort();

        // See if this PO is an empty PO. Assuming that the PhyIntf MO has already been deleted
        // with previous transaction.. Otherwise need to consider the collected PhyIntf names too.
        mo_criteria phyMemberCriteria (PhyIntfLocalManagedObject::getClassName());
        phyMemberCriteria.And("poId", pPortXstpMo->getOwnerManagedObjectId());
        if (phyMemberCriteria.queryExists()) {
            trace_printf(TRACE_LEVEL_DEBUG, "PO %s is having member interfaces.. keep the port id", poName.c_str());
            continue;
        }

        msg.addEntityName("po"+poName);
    }
    }

    status = UidToolKit::invoke(msg);
    if (status != WAVE_MESSAGE_SUCCESS) {
        trace_printf(TRACE_LEVEL_WARN, "Error freeing port ids; err=%u", status);
    }

    // Done finally!!
}


VlanIdList::VlanIdList (const UI32Range &vlans, VlanListType type)
{
    m_type = type;
    vlans.getUI32RangeVector(m_vlanIds);
}

VlanIdList::VlanIdList (UI32 vid, VlanListType type)
{
    m_type = type;

    if (vid > 0) {
        m_vlanIds.push_back(vid);
    }
}

VlanIdList::VlanIdList (const ObjectId &vlanOid, VlanListType type)
{
    m_type = type;

    if (vlanOid != ObjectId::NullObjectId) {
        m_vlanOids.push_back(vlanOid);
    }
}

VlanIdList::VlanIdList (const vector<UI32> &vlanIds, VlanListType type)
    : m_vlanIds ( vlanIds )
{
    m_type = type;
}

VlanIdList::VlanIdList (const vector<UI32> &vlanIds, const vector<ObjectId> &vlanOids, VlanListType type)
    : m_vlanIds ( vlanIds ), m_vlanOids ( vlanOids )
{
    m_type = type;
}

VlanIdList::VlanIdList (const vector<ObjectId> &vlanOids, VlanListType type)
    : m_vlanOids ( vlanOids )
{
    m_type = type;
}


void IntfXstpFailoverContext::recordPhyIntfDelete (PhyIntfLocalManagedObject *pIntfMo)
{
    if (pIntfMo == NULL) return;
    ObjectId poOid = pIntfMo->getPoId();
    if (poOid != ObjectId::NullObjectId) {
        m_poObjectIds.insert(poOid.getInstanceId());
    }
    else if (pIntfMo->getPortStpConfig()->getStpEnable()) {
        string ifName = pIntfMo->getId();
        m_phyIntfNames.push_back(ifName);
    }
}

void IntfXstpFailoverContext::reset ()
{
    m_poObjectIds.clear();
    m_phyIntfNames.clear();
    m_rbridges.clear();
}

}

