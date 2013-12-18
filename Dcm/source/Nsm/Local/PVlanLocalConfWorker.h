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
 *   Author : rprashar                                                     *
 **************************************************************************/

#ifndef PVLANLOCALCONFWORKER_H
#define PVLANLOCALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

  class NsmLocalObjectManager;
  class DcmManagementInterfaceMessage;
  class NsmPVlanTrunkAssocPhyMessage;
  class NsmPVlanTrunkAssocPoSPMessage;
  class NsmPVlanMappingPhyMessage;
  class NsmPVlanMappingPoSPMessage;
  class NsmVlanProvisionLocalMessage;

  class PVlanLocalConfWorker:public WaveWorker
  {
  private:
    PrismMessage * createMessageInstance (const UI32 & operationCode);
    WaveManagedObject *createManagedObjectInstance (const string & managedClassName);
    void NsmPVlanTrunkAssocPhyMessageHandler (NsmPVlanTrunkAssocPhyMessage * pNsmPVlanTrunkAssocPhyMessage);
    void NsmPVlanTrunkAssocPoSPMessageHandler (NsmPVlanTrunkAssocPoSPMessage * pNsmPVlanTrunkAssocPoSPMessage);

    void NsmPVlanMappingPhyMessageHandler (NsmPVlanMappingPhyMessage * pNsmPVlanMappingPhyMessage);
    void NsmPVlanMappingPoSPMessageHandler (NsmPVlanMappingPoSPMessage * pNsmPVlanMappingPoSPMessage);

    ResourceId validateTrunkAssocOnPhyStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
    ResourceId PVlanTrunkAssocOnPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
    ResourceId PVlanTrunkAssocOnPhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);

    ResourceId validateMappingOnPhyStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
    ResourceId PVlanMappingOnPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
    ResourceId PVlanMappingOnPhyIntfMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);

    ResourceId PVlanTrunkAssocOnPoSPIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);
    ResourceId PVlanMappingOnPoSPIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext);

    void GvlanProvisionMessageHandler (NsmVlanProvisionLocalMessage *pMessage);

    ResourceId errorCode(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status);


  protected:
  public:
	PVlanLocalConfWorker (NsmLocalObjectManager * pNsmLocalObjectManager);
	virtual ~ PVlanLocalConfWorker ();

  private:
  protected:
  };
}
#endif
