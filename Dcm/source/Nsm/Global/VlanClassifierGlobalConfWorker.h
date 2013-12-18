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

#ifndef VLANCLASSIFIERGLOBALCONFWORKER_H
#define VLANCLASSIFIERGLOBALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

using namespace WaveNs;

namespace DcmNs
{

  class NsmGlobalObjectManager;
  class NsmGlobalCreateVlanClassifierRuleMessage;
  class NsmGlobalDeleteVlanClassifierRuleMessage;
  class NsmGlobalUpdateVlanClassifierGroupMessage;
  class NsmGlobalDeleteVlanClassifierGroupMessage;
  class NsmGlobalAddVlanClassifierToPoMessage;
  class NsmGlobalDeleteVlanClassifierToPoMessage;
  class PoIntfManagedObject;

  class VlanClassifierGlobalConfWorker:public WaveWorker
  {
  private:
    PrismMessage * createMessageInstance (const UI32 & operationCode);
    WaveManagedObject *createManagedObjectInstance (const string & managedClassName);
    void NsmGlobalAddVlanClassifierToPoMessageHandler (NsmGlobalAddVlanClassifierToPoMessage * pNsmGlobalAddVlanClassifierToPoMessage);
    void NsmGlobalCreateVlanClassifierRuleMessageHandler (NsmGlobalCreateVlanClassifierRuleMessage * pNsmGlobalCreateVlanClassifierRuleMessage);
    void NsmGlobalDeleteVlanClassifierGroupMessageHandler (NsmGlobalDeleteVlanClassifierGroupMessage * pNsmGlobalDeleteVlanClassifierGroupMessage);
    void NsmGlobalDeleteVlanClassifierRuleMessageHandler (NsmGlobalDeleteVlanClassifierRuleMessage * pNsmGlobalDeleteVlanClassifierRuleMessage);
    void NsmGlobalDeleteVlanClassifierToPoMessageHandler (NsmGlobalDeleteVlanClassifierToPoMessage * pNsmGlobalDeleteVlanClassifierToPoMessage);
    void NsmGlobalUpdateVlanClassifierGroupMessageHandler (NsmGlobalUpdateVlanClassifierGroupMessage * pNsmGlobalUpdateVlanClassifierGroupMessage);
    void VlanClassifierRuleCreationStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void VlanClassifierRuleCreationMapMOStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void VlanClassifierRuleDeleteStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void VlanClassifierRuleDeleteMapMOStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void VlanClassifierGroupCreationStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void VlanClassifierGroupCreationMapMOStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void ActivateVlanClassfierOnPoInterfaceStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void ActivateVlanClassfierMOStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void VlanClassifierGroupAssociationMapMOStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void DeleteVlanClassiferGroupMOStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void DeleteVlanClassifierGroupStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void DeActivateVlanClassifierGroupFromPoMoStep (PrismLinearSequencerContext * pPrismLinearSequencerContext);
    void VlanClassifierDeleteGroupAssoWithRulesMapMOStep(PrismLinearSequencerContext * pPrismLinearSequencerContext);
	void DeActivateVlanClassifierGroupFromPo (PrismLinearSequencerContext * pPrismLinearSequencerContext);
	void VlanClassifierGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p);
    int VlanClassifierDeleteRuleForGroup (UI32 & groupId, UI32 & ruleId, PrismLinearSequencerContext *pPrismLinearSequencerContext);
    int deleteGroupBygroupId (UI32 & groupId);
    ResourceId deleteVlanClassifierGroup(UI32 & groupId);
    ResourceId deleteVlanClassifierGroupsAssociatedWithRules(UI32 &ruleId, PrismLinearSequencerContext *pPrismLinearSequencerContext);
	void VlanClassifierDeActivateGroupAssoWithRulesMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext );
	void deActivateVlanClassifierOnPo(const ObjectId &ruleObjectId, PrismLinearSequencerContext *pPrismLinearSequencerContext);
	void deActivateVlanClassifierOnPhy(const ObjectId &ruleObjectId, PrismLinearSequencerContext *pPrismLinearSequencerContext);
	void deactivateVlanClassifierOnPoInterface(PrismLinearSequencerContext *pPrismLinearSequencerContext );
	void setClusterContextFlags(WaveSendToClusterContext *ctx_p)
	{
		ctx_p->setPartialSuccessFlag(true);
		ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
	}	

  protected:
  public:
	VlanClassifierGlobalConfWorker (NsmGlobalObjectManager * pNsmGlobalObjectManager);
	virtual ~ VlanClassifierGlobalConfWorker ();

// Now the data members

  private:
  protected:
  public:
  };
}
#endif				//VLANCLASSIFIERGLOBALCONFWORKER_H
