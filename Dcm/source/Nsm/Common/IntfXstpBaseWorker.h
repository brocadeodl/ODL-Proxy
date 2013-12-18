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
 * IntfXstpBaseWorker.h
 *
 * May 2, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef INTFXSTPBASEWORKER_H_
#define INTFXSTPBASEWORKER_H_

#include "Framework/ObjectModel/WaveWorker.h"
#include "ClientInterface/InterfaceCommon.h"

#define PORT_UID_KEY "__port_uid"

using namespace WaveNs;

namespace DcmNs {

class DceContext;
class DceSynchronousLinearSeqContext;
class NsmPortStpConfigMessage;
class NsmUpdateMstpInstanceConfigMessage;
class PoPhyIntfManagedObject;
class PhyIntfLocalManagedObject;
class XstpPortConfigManagedObject;
class XstpPortInstanceConfigManagedObject;
class XstpPortVlanConfigManagedObject;


typedef enum {
    MEMBER_VLAN_LIST = 1,

    NONMEMBER_VLAN_LIST

} VlanListType;


class VlanIdList {
public:
    VlanIdList (UI32 vid = 0, VlanListType type = MEMBER_VLAN_LIST);

    VlanIdList (const ObjectId &vanOid, VlanListType type = MEMBER_VLAN_LIST);

    VlanIdList (const UI32Range &vlans, VlanListType type = MEMBER_VLAN_LIST);

    VlanIdList (const vector<UI32> &vlanIds, VlanListType type = MEMBER_VLAN_LIST);

    VlanIdList (const vector<UI32> &vlanIds, const vector<ObjectId> &vlanOids, VlanListType type = MEMBER_VLAN_LIST);

    VlanIdList (const vector<ObjectId> &vlanOids, VlanListType type = MEMBER_VLAN_LIST);

public:
    vector<UI32> m_vlanIds;
    vector<ObjectId> m_vlanOids;

    VlanListType m_type;
};

class IntfXstpFailoverContext {
public:
    void recordPhyIntfDelete (PhyIntfLocalManagedObject *pIntfMo);

    void reset();

public:
    set<UI64> m_poObjectIds;
    vector<string> m_phyIntfNames;
    set<SI32> m_rbridges;
};

/**
 * Common base worker to handle interface specific spanning-tree
 * configurations..
 */
class IntfXstpBaseWorker : public WaveWorker
{
public:
    IntfXstpBaseWorker(WaveObjectManager *objectMgr, InterfaceType targetIfType);
    virtual ~IntfXstpBaseWorker();


    ResourceId validatePortXstpConfig (NsmPortStpConfigMessage *pMessage, DceContext *pContext);

    ResourceId updateXstpPortConfigMO (NsmPortStpConfigMessage *pMessage, DceContext *pContext);
    ResourceId updateXstpPortVlanMO (UI32 vid, NsmPortStpConfigMessage *pMessage, DceContext *pContext);
    ResourceId updateXstpPortInstanceMO (UI32 instId, NsmPortStpConfigMessage *pMessage, DceContext *pContext);

    // notification about MSTP instance being removed or vlan association changing
    void NsmUpdateMstpInstanceConfigMessageHandler (NsmUpdateMstpInstanceConfigMessage *pMessage);
    ResourceId handleInstanceVlanAssocUpdateStep (DceSynchronousLinearSeqContext *pSequencerContext);
    ResourceId deleteXstpPortInstanceMO (UI32 instId, DceContext *pContext);
    ResourceId handleInstanceVlanAssocUpdate (UI32 instId, vector<ObjectId> newInstVlans, DceContext *pContext);

    /**
     * Remove all XstpPortVlanConfigManagedObject instances for given vlan id..
     */
    ResourceId deleteXstpPortVlanMO (const string &ifName, const vector<UI32> &vids, DceContext *pSequencerContext);

    /**
     * Cleanup port specific xSTP configs due to vlan delete..
     */
    ResourceId handleVlanDelete (UI32 vid, const ObjectId &vlanOid, DceContext *pSequencerContext);

    /**
     * Cleanup all port specific xstp configs for one/all interfaces..
     */
    ResourceId clearIntfXstpConfigs (const string &ifName, bool resetShutState, DceContext *pSequencerContext);

    /**
     * Handle port vlan association changes : Remove per-instance and per-vlan xSTP
     * configuratiosn which are no longer relevant.
     *
     * Caller is expected to pass the ifName and the updated vlan associations for
     * that interface.
     */
    ResourceId handleIntfVlanAssocUpdate (const string &ifName, VlanIdList &vlist, DceContext *pSequencerContext);

    ResourceId handleIntfVlanAssocUpdate (PoPhyIntfManagedObject *pIntfMo, VlanIdList &vlist, DceContext *pSequencerContext, bool recordUpdatedFields);

    ResourceId clearXstpPortInstAndPortVlanMOs (PoPhyIntfManagedObject *pIntfMo, bool recordUpdatedFields);

    ResourceId clearXstpPortInstAndPortVlanMOs (const string &ifName, DceContext *pSequencerContext);

    void failover (IntfXstpFailoverContext &ctx);

protected:
    void fillIntfXstpQueryCriteria (WaveManagedObjectSynchronousQueryContext *qc, const string &ifName = "");

    virtual ResourceId fillMo(XstpPortConfigManagedObject *pPortXstpMo, NsmPortStpConfigMessage *pMessage) = 0;
    virtual ResourceId fillMo(XstpPortInstanceConfigManagedObject *pPortInstMo, NsmPortStpConfigMessage *pMessage) = 0;
    virtual ResourceId fillMo(XstpPortVlanConfigManagedObject *pPortVlanMo, NsmPortStpConfigMessage *pMessage) = 0;


private:
    string m_targetIntfClass;

};

}

#endif /* INTFXSTPBASEWORKER_H_ */
