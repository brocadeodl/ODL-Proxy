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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aashish Akhouri                                              *
 ***************************************************************************/

#ifndef CENTRALCLUSTERCONFIGGETDEBUGINFOWORKER_H
#define CENTRALCLUSTERCONFIGGETDEBUGINFOWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{
class CentralClusterConfigObjectManager;
class ClusterConfigObjectManagerGetClusterInfoMessage;

///Class:
///CentralClusterConfigGetDebugInfoWorker
/// 
///Description:
/// This Class is a worker in the CentralClusterConfigObjectManager
/// that handles the GetClusterInfoMsg.It receives the message and 
/// packs the Principal and Secondary Node information obtained 
/// from PrismCluster and WaveNode objects

class  CentralClusterConfigGetDebugInfoWorker: public WaveWorker
{

private :        

                  CentralClusterConfigGetDebugInfoWorker   (CentralClusterConfigObjectManager *pCentralClusterConfigObjectManager);
    PrismMessage *createMessageInstance                    (const UI32 &operationCode);

    //Called by the framework when it receives a ClusterConfigObjectManagerGetClusterInfoMessage
    void          getDebugInfoHandler                      (ClusterConfigObjectManagerGetClusterInfoMessage 
							    *pClusterObjectManagerGetClusterInfoMessage);
    //Steps in the processing of the message
    void          getPrincipalNodeInfoStep                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
    void          getSecondaryNodesInfoStep                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);

protected :

        ~CentralClusterConfigGetDebugInfoWorker();

    // Now the data members

private :
protected :
public :

    friend class CentralClusterConfigObjectManager;

};

}

#endif // CENTRALCLUSTERCONFIGGETDEBUGINFOWORKER_H
