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
 *@file VcsFabRemoveSegmentNodeWorker.h 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file declares the
 *              Worker class which is responsible for processing
 *              Delete and segment Node request received from the
 *              VcsFabricLocalObjectManager.
 *         
 *              
 * Author :     Aashish Akhouri 
 *  
 */
#ifndef VCSFABREMOVESEGMENTNODEWORKER_H
#define VCSFABREMOVESEGMENTNODEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Cluster/ClusterMessages.h"
using namespace WaveNs;

namespace DcmNs
{

//forward declarations

class VcsRemoveSwitchMessage;
class VcsFabMessagingContext;
class VcsSegmentSwitchMessage;

class VcsFabRemoveSegmentNodeWorker: public WaveWorker
{
public:
    VcsFabRemoveSegmentNodeWorker(WaveObjectManager* pWaveObjectManager);
    virtual ~VcsFabRemoveSegmentNodeWorker ();

protected:
   

private:
   //Message Handler for the Received received from the VcsFabricLocalObjectManager
   void removeSwitchMessageHandler (VcsRemoveSwitchMessage *pVcsRemoveSwitchMessage);
   void sendDeleteNodeToClusterStep (VcsFabMessagingContext* pVcsFabMessagingContext);
   virtual PrismMessage *createMessageInstance (const UI32 &operationCode);
   void segmentSwitchMessageHandler (VcsSegmentSwitchMessage *pVcsSegmentSwitchMessage);
   void sendVcsNodeSegmentSwitchMessageToAllNodesStep(VcsFabMessagingContext* pVcsFabMessagingContext);
   void sendSegmentSwitchMessageToAllNodesCallback (FrameworkStatus frameworkStatus, VcsNodeSegmentSwitchMessage *pVcsNodeSegmentSwitchMessage, VcsFabMessagingContext *pVcsFabMessagingContext);
};

}

#endif //VCSFABREMOVESEGMENTNODEWORKER_H
