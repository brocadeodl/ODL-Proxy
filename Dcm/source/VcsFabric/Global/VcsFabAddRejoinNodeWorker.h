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

/**
 *@file VcsFabAddRejoinNodeWorker.h 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved. Description: This file declares the
 *              Worker class which is responsible for all
 *              cluster creation, add and rejoin processing.
 * 
 * Author :     Aashish Akhouri 
 * Date :       11/16/2010 
 */
#ifndef VCSFABADDREJOINNODEWORKER_H
#define VCSFABADDREJOINNODEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Cluster/ClusterMessages.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
using namespace WaveNs;

namespace DcmNs
{

//forward declarations

class VcsAddSwitchMessage;
class VcsFabMessagingContext;


class VcsFabAddRejoinNodeWorker: public WaveWorker
{
public:
    VcsFabAddRejoinNodeWorker(WaveObjectManager* pWaveObjectManager);
    virtual ~VcsFabAddRejoinNodeWorker ();

protected:
   

private:
   //Message Handler for the Received received from the VcsFabricLocalObjectManager
   void addRejoinSwitchMessageHandler (VcsAddSwitchMessage *pVcsAddSwitchMessage);

   //Sequencer steps and callbacks for processing the Rejoin or Add Request received from VcsFabricLocalObjectManager
   void addRejoinValidateRequestStep    (VcsFabMessagingContext* pVcsFabMessagingContext);

   //segregates the list of nodes into two lists , one for add another for Rejoin
   void prepareAddRejoinListsStep      (VcsFabMessagingContext* pVcsFabMessagingContext);

   //Called only during Cluster Creation
   void createClusterStep               (VcsFabMessagingContext* pVcsFabMessagingContext);
   void createClusterCallBack           (FrameworkStatus frameworkStatus,ClusterObjectManagerCreateClusterMessage *pClusterObjectManagerCreateClusterMessage, void *pContext);

   //For add or rejoin scenarios
   void addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep (VcsFabMessagingContext* pVcsFabMessagingContext);
   void rejoinSwitchesToClusterStep     (VcsFabMessagingContext* pVcsFabMessagingContext);
   void replaceRejoinSwitchesToClusterStep (VcsFabMessagingContext* pVcsFabMessagingContext);
   void rejoinSwitchesToClusterCallBack (FrameworkStatus frameworkStatus,ClusterObjectManagerRejoinNodeMessage   *pClusterObjectManagerRejoinNodeMessage, void *pContext);
   void addSwitchesToClusterStep        (VcsFabMessagingContext* pVcsFabMessagingContext);
   void addSwitchesToClusterCallBack    (FrameworkStatus frameworkStatus,ClusterObjectManagerAddNodeMessage   *pClusterObjectManagerAddNodeMessage, void *pContext);
   void determineAddRejoinProcessingSuccessStep  (VcsFabMessagingContext* pVcsFabMessagingContext);
   bool isDomaidIdInFailedList          (UI32 nodeDomainId, vector<UI32> &failedDomainIdVector); 

   //For cluster Reboot Scenarios
   void startClusterReformation (VcsAddSwitchMessage* pVcsAddSwitchMessage);
   void clusterReformationValidateStep (VcsFabMessagingContext* pVcsFabMessagingContext);
   void prepareFailoverListStep (VcsFabMessagingContext* pVcsFabMessagingContext);
   void establishPrincipalAfterClusterRebootStep(VcsFabMessagingContext* pVcsFabMessagingContext);
   void establishPrincipalAfterClusterRebootCallback(FrameworkStatus frameworkStatus,FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage* pEstablishPrincipalAfterClusterRebootMessage, void *pContext);
   void sendControlledFailoverMessageAfterClusterRebootStep(VcsFabMessagingContext* pVcsFabMessagingContext);
   void sendUncontrolledFailoverMessageAfterClusterRebootStep(VcsFabMessagingContext* pVcsFabMessagingContext);
   void sendFailoverMessageAfterClusterRebootCallback (FrameworkStatus frameworkStatus,FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage, void *pContext);

};

}

#endif //VCSFABADDREJOINNODEWORKER_H
