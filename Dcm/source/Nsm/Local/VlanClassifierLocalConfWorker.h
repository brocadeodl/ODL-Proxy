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
 *   Author : rmuthigi                                                     *
 **************************************************************************/

#ifndef VLANCLASSIFIERLOCALCONFWORKER_H
#define VLANCLASSIFIERLOCALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

  class NsmLocalObjectManager;
  class NsmAddVlanClassifierToPhyMessage;
  class NsmDeleteVlanClassifierToPhyMessage;
  class NsmCreateVlanClassifierRuleSPMessage;
  class NsmDeleteVlanClassifierRuleSPMessage;
  class NsmUpdateVlanClassifierGroupSPMessage;
  class NsmDeleteVlanClassifierGroupSPMessage;
  class NsmAddVlanClassifierToPoSPMessage;
  class NsmDeleteVlanClassifierToPoSPMessage;
  class NsmLocalShowVlanClassifierMessage;
  class DcmManagementInterfaceMessage;

  class VlanClassifierLocalConfWorker:public WaveWorker
  {
  private:
    PrismMessage * createMessageInstance (const UI32 & operationCode);
    WaveManagedObject *createManagedObjectInstance (const string & managedClassName);
    void NsmAddVlanClassifierToPhyMessageHandler (NsmAddVlanClassifierToPhyMessage * pNsmAddVlanClassifierToPhyMessage);
    void NsmAddVlanClassifierToPoSPMessageHandler (NsmAddVlanClassifierToPoSPMessage * pNsmAddVlanClassifierToPoSPMessage);
    void NsmCreateVlanClassifierRuleSPMessageHandler (NsmCreateVlanClassifierRuleSPMessage * pNsmCreateVlanClassifierRuleSPMessage);
    void NsmDeleteVlanClassifierGroupSPMessageHandler (NsmDeleteVlanClassifierGroupSPMessage * pNsmDeleteVlanClassifierGroupSPMessage);
    void NsmDeleteVlanClassifierRuleSPMessageHandler (NsmDeleteVlanClassifierRuleSPMessage * pNsmDeleteVlanClassifierRuleSPMessage);
    void NsmDeleteVlanClassifierToPhyMessageHandler (NsmDeleteVlanClassifierToPhyMessage * pNsmDeleteVlanClassifierToPhyMessage);
    void NsmDeleteVlanClassifierToPoSPMessageHandler (NsmDeleteVlanClassifierToPoSPMessage * pNsmDeleteVlanClassifierToPoSPMessage);
    void NsmUpdateVlanClassifierGroupSPMessageHandler (NsmUpdateVlanClassifierGroupSPMessage * pNsmUpdateVlanClassifierGroupSPMessage);
	void NsmLocalShowVlanClassifierMessageHandler(NsmLocalShowVlanClassifierMessage* pNsmLocalShowVlanClassifierMessage);

    ResourceId createVlanClassfierRuleStep (PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext);
    ResourceId deleteVlanClassfierRuleStep (PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext);
    ResourceId createVlanClassfierGroupStep (PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext);
    ResourceId DeleteVlanClassifierGroupStep (PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext);
    ResourceId VlanClassifierActivateGroupStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
    ResourceId DeactivateVlanClassfierGroupFromPo (PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext);
    ResourceId ActivateVlanClassiferOnPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
    ResourceId ActivateVlanClassiferOnPhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
    ResourceId errorCode(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status);
    ResourceId inspectErrors(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status);
	ResourceId deleteVlanClassifierGroupFromPhyStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
	ResourceId deleteVlanClassifierGroupFromPhyMoStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
	void deactivateVlanClassifierOnPhy(UI32 groupId);
	ResourceId deactivateVlanClassifierOnInterface(PrismSynchronousLinearSequencerContext * pPrismSynchronousLinearSequencerContext);
	ResourceId sendVlanClassifierCommandToNsmDaemon(PrismLinearSequencerContext *seq_ctx_p);
	void vlanClassifierShowSendToClusterCallback(WaveSendToClusterContext *ctx_p);
	void sendVlanClassifierCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);

  protected:
  public:
	VlanClassifierLocalConfWorker (NsmLocalObjectManager * pNsmLocalObjectManager);
	virtual ~ VlanClassifierLocalConfWorker ();


// Now the data members

  private:
  protected:
  };
}
#endif				//VLANCLASSIFIERLOCALCONFWORKER_H
