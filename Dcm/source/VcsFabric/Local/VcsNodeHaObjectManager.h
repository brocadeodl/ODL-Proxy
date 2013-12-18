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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#ifndef VCSNODEHAOBJECTMANAGER_H
#define VCSNODEHAOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Types/BitMap.h"

#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
//#include "VcsFabric/Local/VcsNodeRemoteMessagingObjectManager.h"

#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeRemoteMessagingTypes.h"
#include "VcsFabric/Local/VcsRemoteEnqSwitchMessage.h"
#include "VcsFabric/Local/VcsNodeTimerHandler.h"
#include "VcsFabric/Local/VcsNodeHaMessagingContext.h"
#include "VcsFabric/Local/VcsNodeAsyncHaMessagingContext.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeHaPeerEnqMessage;
class VcsNodeCpStateChangeClientMessage;

class VcsNodeHaObjectManager : public WaveLocalObjectManager
{
    private :
    
									VcsNodeHaObjectManager						();
		PrismServiceId 				getPrismServiceId 								();
		void						initializeCpState 									();
        bool         				disableHaReceiveHeartbeatTimer                             						();
        bool         				startHaReceiveHeartbeatTimer                 	 		(UI32 timeoutValue);
        bool         				startHaSendHeartbeatTimer                 	 		(UI32 timeoutValue);
		bool 						disableHaSendHeartbeatTimer 					();
		void 						receiveHeartbeatMessageHandler 				(VcsNodeHaHeartbeatMessage *pVcsNodeHaHeartbeatMessage);
		void 						sendHeartbeat 		 							();
		void  						activeCpHeartbeatFailure 						();

		
		ResourceId 					savePeerCpInfo 									(VcsNodeHaMessagingContext *pVcsNodeHaMessagingContext);
		ResourceId 					getLocalCpInfo 									(VcsNodeHaMessagingContext *pVcsNodeHaMessagingContext);
		ResourceId 					reopenPeerConnectionIfFailedStep 				(VcsNodeHaMessagingContext *pVcsNodeHaMessagingContext);
		void	 					sendPeerCpEnqMessageStep    					(VcsNodeAsyncHaMessagingContext *pVcsNodeAsyncHaMessagingContext);
		void 						selectActiveCpStep 								(VcsNodeAsyncHaMessagingContext *pVcsNodeAsyncHaMessagingContext);
        ResourceId                  selectActiveCpOnPeerEnqStep 					(VcsNodeHaMessagingContext *pVcsNodeHaMessagingContext);
		void 						establishActiveCpSteps 							();
		void 						arbitrateActiveCp 									();
		void 						transitionUninitToActive 							();
		void 						transitionUninitToStandby 						();
		void 						transitionStandbyToActive 						();
		void 						transitionUninitToStandAlone 						();
		ResourceId 					connectToHaPeer 								();
//		void 						disconnectFromHaPeer 							();
		void 						sendCpActiveMessageToTestApps				();
		void 						sendCpActiveMessageToTestApp (TimerHandle timerHandle, void *pContext);
		ResourceId 					startSendCpStateTimer (VcsNodeCpStateChangeClientMessage *&pVcsNodeCpStateChangeClientMessage, UI32 timeOutValue);
		VcsNodeCpStateChangeClientMessage *createCpActiveMessage 						(UI32 opCode);
    public :
        virtual     				~VcsNodeHaObjectManager           				();
        static  					VcsNodeHaObjectManager *getInstance 				();
        static string      				getClassName                            						();
		PrismMessage 				*createMessageInstance 								(const UI32 &operationCode);
		void 						bootCompleteEventHandler (const WaveNs::BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);
		void 						listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
		
		static void					debugSetPeerHaPortMessageHandler 			(UI32 argc, vector<string> argv);
		
		
		string 						convertIpAddrFromHexToString 					(UI32 hexIpAddr);
		UI32 						convertIpAddrFromStringToBin 						(string ipAddress);
		string 						getLocalIpAddress 								();
		string 						getPeerIpAddress 								();
		void 						haPeerEnqMessageHandler (VcsNodeHaPeerEnqMessage *pVcsNodeHaPeerEnqMessage);
		void 						getHaPeerCpInfoCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodeAsyncHaMessagingContext *pVcsNodeAsyncHaMessagingContext);
		void 						heartbeatMessageCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodeAsyncHaMessagingContext *pVcsNodeAsyncHaMessagingContext);
		void 						VcsNodeCpStateChangeClientMessageCallBack (FrameworkStatus frameworkStatus, VcsNodeCpStateChangeClientMessage *pVcsNodeCpStateChangeClientMessage, void *pContext);

    // Now the Data Members

        TimerHandle      		        m_receiveHbTimerHandle;
		bool				        	m_receiveHbTimerRunningFlag;
		TimerHandle				        m_sendHbTimerHandle;
		bool					        m_sendHbTimerRunningFlag;
        bool                            m_bootComplete;
		bool					        m_haPeerConnectionEstablished;
		TimerHandle 			        m_nsmSendCpStateTimerHandle;
		TimerHandle 			        m_fcoeSendCpStateTimerHandle;
		
		
		string					        m_peerIpAddress; 
		UI32					        m_peerHaPort;
		UI32 					        m_peerCpState;
		
		string					        m_localIpAddress;
		UI32					        m_localHaPort;
		UI32 					        m_localCpState;

        friend class VcsFabricLocalObjectManager;
        friend class VcsFabObjectManager;
//        friend class VcsNodeRemoteMessagingObjectManager;

    protected :
    public :

};

inline string VcsNodeHaObjectManager::getClassName ()
{ 
    return ("VcsNodeHaObjectManager");
}

}

#endif // VCSNODEHAOBJECTMANAGER_H
