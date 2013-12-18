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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef NSMGLOBALOBJECTMANAGER_H
#define NSMGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Nsm/Global/FcoeWorker/NsmFcoeGlobalWorker.h"
#include "Nsm/Global/Eld/EldConfigManagedObject.h"
#include "Nsm/Global/PortSecurityGlobalConfWorker.h"
#include "ClientInterface/Eld/EldClientUpdateRxlimConfigMessage.h"


using namespace WaveNs;

namespace DcmNs
{
  class EldGlobalWorker;
  class IntfConfGlobalWorker;
  class VlanClassifierGlobalConfWorker;		
  class PVlanGlobalConfWorker;
  class IntfXstpGlobalConfWorker;
  class Dot1xGlobalWorker;
  class XstpPortVlanConfigManagedObject;
  class NsmFcoeGlobalWorker;
  class IgmpsVlanGlobalConfWorker;
  class OSPFInterfaceGlobalConfWorker;
  class VRRPGlobalConfWorker;
  class PhyIntfLocalManagedObject;
  class PoIntfManagedObject;
  class NsmGlobalL3IpWorker;
  class PimIntfCfgGlobalWorker;

  class NsmL3Util;
  class IgmpGlobalWorker;

  class NsmGlobalObjectManager: public WaveObjectManager
  {
    private:
      PrismMessage *createMessageInstance(const UI32 &operationCode);
      WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
      /* ELD Global config replay */	
#if 0
      void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
      void ConfigReplayEldGlobalScalars();
#endif
      /* ELD Global config replay */
    protected:
    public:
      NsmGlobalObjectManager ();
      virtual    ~NsmGlobalObjectManager ();
      static NsmGlobalObjectManager *getInstance();
      static string getClassName();
      static PrismServiceId getPrismServiceId();

      friend class NsmL3Util;

      // Now the data members

    private:
      EldGlobalWorker *m_pEldGlobalWorker;
      IntfConfGlobalWorker  *m_pIntfConfGlobalWorker;
      VlanClassifierGlobalConfWorker  *m_pVlanClassifierGlobalConfWorker;
      PVlanGlobalConfWorker  *m_pPVlanGlobalConfWorker;
	  PortSecurityGlobalConfWorker *m_pPortSecurityGlobalConfWorker;
      IntfXstpGlobalConfWorker  *m_pIntfXstpGlobalConfWorker;
      Dot1xGlobalWorker  *m_pDot1xGlobalWorker;
      IgmpsVlanGlobalConfWorker  *m_pIgmpsVlanGlobalConfWorker;
      OSPFInterfaceGlobalConfWorker  *m_pOSPFInterfaceGlobalConfWorker;
      VRRPGlobalConfWorker  *m_pVRRPGlobalConfWorker;
      NsmFcoeGlobalWorker *m_pNsmFcoeGlobalWorker;
      NsmGlobalL3IpWorker *m_pNsmGlobalL3IpWorker;
      PimIntfCfgGlobalWorker *m_pPimIntfCfgGlobalWorker;
      IgmpGlobalWorker  *m_pIgmpGlobalWorker;

    protected:
    public:
      ResourceId getPoIntfObjectIdByPoIfName (const string &poName,ObjectId &poIntf_oid);
      ResourceId GetPhyMoByPoOid (ObjectId &poIntf_oid, vector<PhyIntfLocalManagedObject *> &vpPhyIntfLocalManagedObject);
      IntfXstpGlobalConfWorker* getIntfXstpGlobalConfWorker();

      void failover (FailoverAsynchronousContext *pFailoverAsynchronousContext);


      void clusterCreateCollectValidationData(WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
      void clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

      bool isVlanClassifierGroupIdAddedToPoInterface(UI32 groupId, string ifName, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
      bool isVlanClassifierRuleIdExists(UI32 ruleId, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
      bool isVlanClassifierGroupRuleEntryExists(UI32 groupId, UI32 ruleId, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
      bool isVlanClassifierGroupIdExists(UI32 groupId, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);

      ResourceId getObjectIdByVlanClassifierRuleId(int ruleId, ObjectId &ruleOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
      ResourceId getPoIntfObjectIdByPoIfName(string poName, ObjectId &poIntfOid,  bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
      ResourceId getPoMoByPoId (UI32 PoId, PoIntfManagedObject* &pPoIntfManagedObject);
      UI32 getVlanClassifierRuleByObjectId(const ObjectId &ruleOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
      UI32 getVlanIdByObjectId(const ObjectId &vlanOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
      void printToNoscli(WaveClientSessionContext &waveClientSessionContext, const string &printstring);

  };

  inline string  NsmGlobalObjectManager::getClassName()
  {
      return ("NsmGlobal");
  }


}
#endif                                            //NSMGLOBALOBJECTMANAGER_H
