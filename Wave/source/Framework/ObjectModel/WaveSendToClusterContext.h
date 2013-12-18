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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVESENDTOCLUSTERCONTEXT_H
#define WAVESENDTOCLUSTERCONTEXT_H

#include "Framework/Utils/PrismAsynchronousContext.h"

#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class PrismMessage;

/**
 * @addtogroup PartialSuccess Partial success from sendToWaveCluster
 * @{
 */
/**
 * 
 * <b>Key Notes</b>\n
	
 * 	1.	A new flag m_returnSuccessForPartialSuccessFlag is added in WaveSendToClusterContext (by default, this flag is false). 
		Plug-in needs to set this to true (WaveSendToClusterContext::setPartialSuccessFlag) if partial success is to be treated 
		as SUCCESS from sendToWaveCluster.\n

	2.	In the callback for step calling sendToWaveCluster, which checks the status returned (from sendToWaveCluster), plug-in must 
		check the completion status for each node to know which node has encountered failure if any and take the precautionary steps if required\n

	3.	To check the individual node status:\n
			a.	Invoke WaveSendToClusterContext::getLocationsToSendToForPhase1() to get the vector of location-ids to which phase 1 message was sent\n
			b.	Then for each location id from vector above, get the completion status using the method WaveSendToClusterContext::getCompletionStatusForPhase1(locationId)\n

	4.	In case this flag is set and rollback message is also specified, then DCMd asserts. This is due to the fact that rollback can.t be triggered 
		on partial success as both the features are mutually exclusive.\n
 */

class WaveSendToClusterContext : public PrismAsynchronousContext
{
    private :
        const   bool                            &getIsSendOneWayToWaveCluster       () const;
                void                             setIsSendOneWayToWaveCluster       (bool isSendOneWayToWaveCluster);

    protected :
    public :
                                                 WaveSendToClusterContext           (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL);
        virtual                                 ~WaveSendToClusterContext           ();

                PrismMessage                    *getPPrismMessageForPhase1          () const;
                void                             setPPrismMessageForPhase1          (PrismMessage *pPrismMessageForPhase1);

                PrismMessage                    *getPPrismMessageForPhase2          () const;
                void                             setPPrismMessageForPhase2          (PrismMessage *pPrismMessageForPhase2);

                UI32                             getTimeoutForPhase1                () const;
                void                             setTimeoutForPhase1                (const UI32 &timeoutForPhase1);

                UI32                             getTimeoutForPhase2                () const;
                void                             setTimeoutForPhase2                (const UI32 &timeoutForPhase2);

                vector<LocationId>               getLocationsToSendToForPhase1      () const;
                void                             setLocationsToSendToForPhase1      (const vector<UI32> &locationsToSendToForPhase1);

                WaveMessageStatus                getSendStatusForPhase1             (const LocationId &locationId);
                void                             setSendStatusForPhase1             (const LocationId &locationId, const WaveMessageStatus &sendStatusForPhase1);

                FrameworkStatus                  getFrameworkStatusForPhase1        (const LocationId &locationId);
                void                             setFrameworkStatusForPhase1        (const LocationId &locationId, const FrameworkStatus frameworkStatusForPhase1);

                ResourceId                       getCompletionStatusForPhase1       (const LocationId &locationId);
                void                             setCompletionStatusForPhase1       (const LocationId &locationId, const ResourceId &completionStatusForPhase1);

                UI32                             getNumberOfFailuresForPhase1       () const;
                void                             setNumberOfFailuresForPhase1       (const UI32 &numberOfFailuresForPhase1);

                UI32                             getNumberOfDisconnectedNodesForPhase1    () const;
                void                             setNumberOfDisconnectedNodesForPhase1    (const UI32 &numberOfDisconnectsForPhase1);

                PrismMessage                    *getResultingMessageForPhase1       (const LocationId &locationId);
                void                             setResultingMessageForPhase1       (const LocationId &locationId, PrismMessage *pPrismMessage);
                PrismMessage                    *transferResultingMessageForPhase1  (const LocationId &locationId);

                vector<LocationId>               getLocationsToSendToForPhase2      () const;
                void                             setLocationsToSendToForPhase2      (const vector<UI32> &locationsToSendToForPhase2);

                WaveMessageStatus                getSendStatusForPhase2             (const LocationId &locationId);
                void                             setSendStatusForPhase2             (const LocationId &locationId, const WaveMessageStatus &sendStatusForPhase2);

                FrameworkStatus                  getFrameworkStatusForPhase2        (const LocationId &locationId);
                void                             setFrameworkStatusForPhase2        (const LocationId &locationId, const FrameworkStatus frameworkStatusForPhase2);

                ResourceId                       getCompletionStatusForPhase2       (const LocationId &locationId);
                void                             setCompletionStatusForPhase2       (const LocationId &locationId, const ResourceId &completionStatusForPhase2);

                UI32                             getNumberOfFailuresForPhase2       () const;
                void                             setNumberOfFailuresForPhase2       (const UI32 &numberOfFailuresForPhase2);

                PrismMessage                    *getResultingMessageForPhase2       (const LocationId &locationId);
                void                             setResultingMessageForPhase2       (const LocationId &locationId, PrismMessage *pPrismMessage);
                PrismMessage                    *transferResultingMessageForPhase2  (const LocationId &locationId);

                bool                             getTreatFailureOnFailingOverAsSuccessFlag  () const;
                void                             setTreatFailureOnFailingOverAsSuccessFlag  (const bool &treatFailureOnFailingOverAsSuccessFlag);

                bool                             getPartialSuccessFlag              () const;
                void                             setPartialSuccessFlag              (const bool &partialSuccessFlag);

                bool                             getIsPartialSuccessCaseFlag        () const;
                void                             setIsPartialSuccessCaseFlag        (const bool partialSuccessFlag);

                vector<LocationId>               getFailedOverLocations             () const;
                void                             setFailedOverLocations             (const vector<UI32> &disconnectedLocationsForSurrogate);

                bool                             getIsLocationsForPhase1SetByUserFlag       () const;

    // Now the data members

    private :
        PrismMessage                       *m_pPrismMessageForPhase1;
        PrismMessage                       *m_pPrismMessageForPhase2;
        UI32                                m_timeoutForPhase1;
        UI32                                m_timeoutForPhase2;

        vector<LocationId>                  m_locationsToSendToForPhase1;

        map<LocationId, WaveMessageStatus>  m_sendStatusForPhase1;
        map<LocationId, FrameworkStatus>    m_frameworkStatusForPhase1;
        map<LocationId, ResourceId>         m_completionStatusForPhase1;
        map<LocationId, PrismMessage *>     m_resultingMessageForPhase1;

        UI32                                m_numberOfFailuresForPhase1;
        UI32                                m_numberOfDisconnectedNodesForPhase1;

        vector<LocationId>                  m_locationsToSendToForPhase2;

        map<LocationId, WaveMessageStatus>  m_sendStatusForPhase2;
        map<LocationId, FrameworkStatus>    m_frameworkStatusForPhase2;
        map<LocationId, ResourceId>         m_completionStatusForPhase2;
        map<LocationId, PrismMessage *>     m_resultingMessageForPhase2;

        UI32                                m_numberOfFailuresForPhase2;

        vector<LocationId>                  m_failedOverLocations;
        // In case this flag is set and all failures are because of node status being disconnected, then dont rollback.
        bool                                m_treatFailureOnFailingOverAsSuccessFlag;      

        // This flag can be set in case partial success (i.e. atleast a node failed while others (atleast one) have succeeded
        // in handling phase 1 message), the status for sendToWaveCluster will be returned as WAVE_MESSAGE_SUCCESS.
		/**
		 * This flag can be set if a partial success case is supposed to return success from sendToWaveCluster. 
		 *	<b>Partial Success</b>: is when atleast a node gives completion status as "not Success" when other 
	   	 *  nodes have completion status as Success. With this scenario, sendToWaveCluster returns FAILED_ON_FEW_NODES 
		 *  status by default. But, if partialSuccess flag would have been set true then it will return a status as SUCCESS.\n\n

		 *  <b>partialSuccess</b> flag and rollback message can not be set along. Setting rollback message (i.e. phase 2 message) 
		 *  along with partial Success flag does not give clear idea about the expected behaviour from sendToWaveCluster. 
         *  When partialSuccess flag is set sendToWaveCluster is expected to return success though it receives a failure 
		 *  from few nodes whereas rollback message expects to send this message to failed nodes.\n
		 *
		 *  @see WaveNs::WaveObjectManager::sendToWaveCluster
		 */
        bool                                m_returnSuccessForPartialSuccessFlag;  
        
        /**
        * This flag will be set when there will be a Partial success case.
        * Plug-ins can read this flag using getIsPartialSuccessCaseFlag() to knwo if WAVE_MESSAGE_SUCCESS is genuine or because of
        * Partial success scenario. If the method returns true , then it is a success because of Partial success scenario.
        */
        bool                                m_isPartialSuccessCaseFlag;  
        bool                                m_isLocationsForPhase1SetByUserFlag;

        // m_isSendOneWayToWaveCluster is an internal private variable that should only be get/set from WaveObjectManager's sendOneWayToWaveCluster/sendToWaveCluster API's
        bool                                m_isSendOneWayToWaveCluster;

/**
 * @}
 */
    protected :
    public :

    friend class WaveObjectManager;
};

}

#endif // WAVESENDTOCLUSTERCONTEXT_H
