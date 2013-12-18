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

#ifndef PVLANGLOBALCONFWORKER_H
#define PVLANGLOBALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Utils/DceLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

  class NsmGlobalObjectManager;
  class NsmPVlanTrunkAssocPoSPMessage;
  class NsmGlobalPVlanTrunkAssocPoMessage;
  class NsmPVlanMappingPoSPMessage;
  class NsmGlobalPVlanMappingPoMessage;

  class PoIntfManagedObject;

  class PVlanGlobalConfWorker:public WaveWorker
  {
  private:
    PrismMessage * createMessageInstance (const UI32 & operationCode);
    WaveManagedObject *createManagedObjectInstance (const string & managedClassName);

    void NsmGlobalPVlanTrunkAssocPoMessageHandler (NsmGlobalPVlanTrunkAssocPoMessage * pNsmGlobalPVlanTrunkAssocPoMessage);
    void NsmGlobalPVlanMappingPoMessageHandler (NsmGlobalPVlanMappingPoMessage * pNsmGlobalPVlanMappingPoMessage);

    void PVlanGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p);

    void validateTrunkAssocOnPOStep (DceLinearSeqContext *pDceLinearSeqContext);
    void PVlanTrunkAssocOnPoIntfStep (DceLinearSeqContext *pDceLinearSeqContext);
    void PVlanTrunkAssocOnPoIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext);

    void validateMappingOnPOStep (DceLinearSeqContext *pDceLinearSeqContext);
    void PVlanMappingOnPoIntfStep (DceLinearSeqContext *pDceLinearSeqContext);
    void PVlanMappingOnPoIntfMOStep (DceLinearSeqContext *pDceLinearSeqContext);

	void setClusterContextFlags(WaveSendToClusterContext *ctx_p)
	{
		ctx_p->setPartialSuccessFlag(true);
		ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
	}

  protected:
  public:
	PVlanGlobalConfWorker (NsmGlobalObjectManager * pNsmGlobalObjectManager);
	virtual ~ PVlanGlobalConfWorker ();

// Now the data members

  private:
  protected:
  public:
  };
}
#endif				//PVlanGlobalConfWorker_H