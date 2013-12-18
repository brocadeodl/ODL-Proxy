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
 *   Author : chenj                                                     *
 **************************************************************************/

#ifndef RASLOCALOBJECTMANAGER_H
#define RASLOCALOBJECTMANAGER_H

#include "Framework/Types/IpVxAddress.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"

#include "RAS/Local/RASLocalMessageContext.h"
#include "ClientInterface/RAS/RASClientMessage.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "RAS/Local/RASLoggingMessage.h"
#include "RAS/Local/RASNodeSpecificConfigureMessage.h"
#include "RAS/Local/RASSupportMessage.h"

using namespace WaveNs;

namespace DcmNs
{
// use these class 
class BootCompleteForThisLocationEvent;
class RASLoggingMessage;
class RASSupportMessage;
class RASManagedObject;
class RASAuditManagedObject;
class RASLogShowMessage;
class RASAutoUploadManagedObject;
class RASLOGConfigureManagedObject;
class RASMODConfigureManagedObject;

class RASLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	    void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
	    void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
	    void bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);
	    void postboot (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhases);
	    void externalStateSynchronization (WaveAsynchronousContextForExternalStateSynchronization
					       *pWaveAsynchronousContextForExternalStateSynchronization);
     static ManagementInterfaceMessage *createManagementInterfaceMessageInstance (const UI32 &operationCode);
	    void ipVxSecondBoot();
		void autoUploadBoot();
		void raslogCfgBoot();
		void rasmodCfgBoot();
	    void auditClass();
		void syslogFacility();
	    void loggingBootReplay();

            void RASLoggingMessageHandler(RASLoggingMessage *pRASLoggingMessage);
            void RASGlobalConfigMessageHandler(RASLoggingMessage *pRASLoggingMessage);
	    void fosCLIMessageHandler(RASLoggingMessage *pRASLoggingMessage);
        void rasmanCLIMessageHandler(RASRasmanMessage *pRASRasmanMessage);
		void RASNodeSpecificConfigureMessageHandler(RASNodeSpecificConfigureMessage *pRASNodeSpecificConfigureMessage);
	    void netConfMessageHandler(RASNetConfMessage *pMessage);
	    ResourceId      fosCLI(RASLocalMessagingContext *pRASLocalMessagingContext);
        ResourceId      rasmanCLI(RASLocalMessagingContext *pRASLocalMessagingContext);

	    //void VCSMessageHandler(RASClientMessage *pRASClientMessage);
	    //ResourceId VCSRASLog(RASLocalMessagingContext *pRASLocalMessagingContext);

	    void ipVxMessageHandler(RASLoggingMessage *pRASLoggingMessage);
	    void auditLogMessageHandler(RASLoggingMessage *pRASLoggingMessage);
		void sysLogMessageHandler(RASLoggingMessage *pRASLoggingMessage);
	    ResourceId generateSyslogNGConfig(void);
		void autoUploadMessageHandler(RASLoggingMessage *pRASLoggingMessage);
		void autoUploadParamMessageHandler(RASLoggingMessage *pRASLoggingMessage);
		void raslogSeverityCfgMessageHandler(RASLoggingMessage *pRASLoggingMessage);
		void raslogSyslogCfgMessageHandler(RASLoggingMessage *pRASLoggingMessage);
		void raslogMsgCfgMessageHandler(RASLoggingMessage *pRASLoggingMessage);
		void raslogModCfgMessageHandler(RASLoggingMessage *pRASLoggingMessage);
	    ResourceId ipVx(RASLocalMessagingContext *pRASLocalMessagingContext);
	    ResourceId auditLog(RASLocalMessagingContext *pRASLocalMessagingContext);
		ResourceId sysLog(RASLocalMessagingContext *pRASLocalMessagingContext);
		ResourceId GlobalConfig(RASLocalMessagingContext *pRASLocalMessagingContext);
		ResourceId autoUpload(RASLocalMessagingContext *pRASLocalMessagingContext);
		ResourceId autoUploadParam(RASLocalMessagingContext *pRASLocalMessagingContext);
		ResourceId raslogSevCfg(RASLocalMessagingContext *pRASLocalMessagingContext);
		ResourceId raslogSyslogCfg(RASLocalMessagingContext *pRASLocalMessagingContext);
		ResourceId raslogMsgCfg(RASLocalMessagingContext *pRASLocalMessagingContext);
		ResourceId raslogModCfg(RASLocalMessagingContext *pRASLocalMessagingContext);
        void SSStatusMessageHandler(RASLoggingMessage *pRASLoggingMessage);
		string syncDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext);
		void asyncDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext);
        void asyncRASMANDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext);

            void RASLCCLIMessageHandler(RASLCCLIMsg *pLCCLIMsg);
	    void RASLCCLISendToClients(PrismLinearSequencerContext *pRASLocalMessagingContext);
	    void RASCLISendToClientsCallback (WaveSendToClientsContext *pWaveSendToClientsContext);
		void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
		void haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        protected:
        public:
            RASLocalObjectManager ();
            virtual    ~RASLocalObjectManager ();
            static RASLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
// Now the data members

        private:
	    // RASLocalWorker  *m_pRASLocalWorker;

        protected:
        public:
    };

inline string  RASLocalObjectManager::getClassName()
{
    return ("RASLocalObjectManager");
}


class SSLocalObjectManager: public WaveLocalObjectManager
    {
        private:
		PrismMessage *createMessageInstance(const UI32 &operationCode);
		void SSMessageHandler(RASSupportMessage *pRASSupportMessage);
        protected:
        public:
            SSLocalObjectManager ();
            virtual    ~SSLocalObjectManager ();
            static SSLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
// Now the data members

        private:

        protected:
        public:
    };
class RASLogLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void RASLogShowMessageHandler( RASLogShowMessage *pRASLogShowMessage);
        protected:
        public:
            RASLogLocalObjectManager ();
            virtual    ~RASLogLocalObjectManager ();
            static RASLogLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:

        protected:
        public:
    };



}
#endif                                            //RASLOCALOBJECTMANAGER_H
