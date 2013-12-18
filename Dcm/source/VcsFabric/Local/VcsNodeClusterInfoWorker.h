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
 *   Copyright (C) 2010-2013 Brocade Communications Systems, Inc.           *
 *   All rights reserved.                                                   *
 *   Author : Anand Subramanian                                             *
 ***************************************************************************/

#ifndef VCSNODECLUSTERINFOWORKER_H
#define VCSNODECLUSTERINFOWORKER_H


#include "Framework/ObjectModel/WaveWorker.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsFabricLocalMessages.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeClusterInfoWorker : public WaveWorker
{
    private :
                                        VcsNodeClusterInfoWorker                         ( WaveObjectManager* pWaveObjectManager );

                    PrismMessage       *createMessageInstance                            ( const UI32 &operationCode);

                    void                collectSwitchIdentifierInfoMessageHandler        (VcsFabricCollectSwitchIdentifierInfoMessage *pVcsFabricCollectSwitchIdentifierInfoMessage);
                    void                virtualIpLocalShowVipMessageHandler              (VirtualIpLocalShowVipMessage *pVirtualIpLocalShowVipMessage);

                    void                collectSwitchIdentifierInfoSendToClusterStep     (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                    void                collectSwitchIdentifierInfoStep                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                    void                collectSwitchIdentifierInfoSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);

                    void                getBackendVirtualIpStep                          (PrismLinearSequencerContext *pPrismLinearSequencerContext);
    protected :
    public :
        virtual                        ~VcsNodeClusterInfoWorker                         ();

                    // Now the data memebers

    private :
    protected :
    public :

        friend class VcsFabricLocalObjectManager;

};

}

#endif // VCSNODECLUSTERINFOWORKER_H

