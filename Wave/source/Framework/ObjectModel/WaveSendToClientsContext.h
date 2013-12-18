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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : James Chen                                                   *
 ***************************************************************************/

#ifndef WAVESENDTOCLIENTSCONTEXT_H
#define WAVESENDTOCLIENTSCONTEXT_H

#include "Framework/Utils/PrismAsynchronousContext.h"

#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class PrismMessage;

class WaveSendToClientsContext : public PrismAsynchronousContext
{
    private :
    protected :
    public :
                                                 WaveSendToClientsContext           (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL, const string &clientName = "", bool isSubInstanceToBeIncluded = false);
        virtual                                 ~WaveSendToClientsContext           ();

                ManagementInterfaceMessage      *getPManagementInterfaceMessageForPhase1          () const;
                void                             setPManagementInterfaceMessageForPhase1          (ManagementInterfaceMessage *pManagementInterfaceMessageForPhase1);

                ManagementInterfaceMessage      *getPManagementInterfaceMessageForPhase2          () const;
                void                             setPManagementInterfaceMessageForPhase2          (ManagementInterfaceMessage *pManagementInterfaceMessageForPhase2);

                UI32                             getTimeoutForPhase1                () const;
                void                             setTimeoutForPhase1                (const UI32 &timeoutForPhase1);

                UI32                             getTimeoutForPhase2                () const;
                void                             setTimeoutForPhase2                (const UI32 &timeoutForPhase2);

                vector<SI32>                     getInstancesToSendToForPhase1      () const;
                void                             setInstancesToSendToForPhase1      (const vector<SI32> &instancesToSendToForPhase1);
                WaveMessageStatus                getSendStatusForPhase1             (const SI32 &instance);
                void                             setSendStatusForPhase1             (const SI32 &instance, const WaveMessageStatus &sendStatusForPhase1);

                FrameworkStatus                  getFrameworkStatusForPhase1        (const SI32 &instance);
                void                             setFrameworkStatusForPhase1        (const SI32 &instance, const FrameworkStatus frameworkStatusForPhase1);

                ResourceId                       getCompletionStatusForPhase1       (const SI32 &instance);
                void                             setCompletionStatusForPhase1       (const SI32 &instance, const ResourceId &completionStatusForPhase1);

                UI32                             getNumberOfFailuresForPhase1       () const;
                void                             setNumberOfFailuresForPhase1       (const UI32 &numberOfFailuresForPhase1);

                ManagementInterfaceMessage      *getResultingMessageForPhase1       (const SI32 &instance);
                void                             setResultingMessageForPhase1       (const SI32 &instance, ManagementInterfaceMessage *pManagementInterfaceMessage);
                ManagementInterfaceMessage      *transferResultingMessageForPhase1  (const SI32 &instance);

                vector<SI32>                     getInstancesToSendToForPhase2      () const;
                void                             setInstancesToSendToForPhase2      (const vector<SI32> &instancesToSendToForPhase2);

                WaveMessageStatus                getSendStatusForPhase2             (const SI32 &instance);
                void                             setSendStatusForPhase2             (const SI32 &instance, const WaveMessageStatus &sendStatusForPhase2);

                FrameworkStatus                  getFrameworkStatusForPhase2        (const SI32 &instance);
                void                             setFrameworkStatusForPhase2        (const SI32 &instance, const FrameworkStatus frameworkStatusForPhase2);

                ResourceId                       getCompletionStatusForPhase2       (const SI32 &instance);
                void                             setCompletionStatusForPhase2       (const SI32 &instance, const ResourceId &completionStatusForPhase2);

                UI32                             getNumberOfFailuresForPhase2       () const;
                void                             setNumberOfFailuresForPhase2       (const UI32 &numberOfFailuresForPhase2);

                ManagementInterfaceMessage      *getResultingMessageForPhase2       (const SI32 &instance);
                void                             setResultingMessageForPhase2       (const SI32 &instance, ManagementInterfaceMessage *pManagementInterfaceMessage);
                ManagementInterfaceMessage      *transferResultingMessageForPhase2  (const SI32 &instance);

                string                           getClientName                      () const;
                void                             setClientName                      (const string &clientName);

                vector<SI32>                     getSubInstancesToSendToForPhase1   () const;
                void                             setSubInstancesToSendToForPhase1   (const vector<SI32> &subInstanceToSendToForPhase1);

                vector<SI32>                     getSubInstancesToSendToForPhase2   () const;
                void                             setSubInstancesToSendToForPhase2   (const vector<SI32> &subInstanceToSendToForPhase2);

                bool                             getIsSubInstanceToBeIncluded       () const;
                void                             setIsSubInstanceToBeIncluded       (bool isSubInstanceToBeIncluded);

    // Now the data members

    private :
        ManagementInterfaceMessage         *m_pManagementInterfaceMessageForPhase1;
        ManagementInterfaceMessage         *m_pManagementInterfaceMessageForPhase2;
        UI32                                m_timeoutForPhase1;
        UI32                                m_timeoutForPhase2;
        map<SI32, WaveMessageStatus>        m_sendStatusForPhase1;
        map<SI32, FrameworkStatus>          m_frameworkStatusForPhase1;
        map<SI32, ResourceId>               m_completionStatusForPhase1;
        map<SI32, ManagementInterfaceMessage *>           m_resultingMessageForPhase1;

        UI32                                m_numberOfFailuresForPhase1;

        map<SI32, WaveMessageStatus>        m_sendStatusForPhase2;
        map<SI32, FrameworkStatus>          m_frameworkStatusForPhase2;
        map<SI32, ResourceId>               m_completionStatusForPhase2;
        map<SI32, ManagementInterfaceMessage *>           m_resultingMessageForPhase2;

        UI32                                m_numberOfFailuresForPhase2;
	    vector<SI32>                        m_instancesToSendToForPhase1;
        vector<SI32>                        m_subInstancesToSendToForPhase1;
    	vector<SI32>                        m_instancesToSendToForPhase2;
        vector<SI32>                        m_subInstancesToSendToForPhase2;
        string                              m_clientName;
        bool                                m_isSubInstanceToBeIncluded;
    protected :
    public :
};

}

#endif // WAVESENDTOCLIENTSCONTEXT_H
