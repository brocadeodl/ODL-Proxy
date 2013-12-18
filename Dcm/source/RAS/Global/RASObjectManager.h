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
 *   Author : Jamws Chen                                    *
 ***************************************************************************/

#ifndef RASOBJECTMANAGER_H
#define RASOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Boot/BootTypes.h"

#include "RAS/Global/RASClusterMessage.h"
#include "RAS/Global/RASSupportClusterMessage.h"
#include "RAS/Global/RASObjectManagerTypes.h"
#include "RAS/Local/LoggingLocalManagedObject.h"
#include "RAS/Global/RASManagedObject.h"
#include "RAS/Global/RASMessagingContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
//#include "ClientInterface/RAS/RASClientMessage.h"
#include "RAS/Local/RASLocalMessageContext.h"
#include "RAS/Local/RASLoggingMessage.h"
#include "RAS/Local/RASSupportMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class LoggingLocalManagedObject;
class RASManagedObject;
class RASGlobalConfigManagedObject;
class RASAutoUploadManagedObject;
class RASLOGConfigureManagedObject;
class RASMODConfigureManagedObject;
class RASAuditManagedObject;
class RASSysFcManagedObject;
class VcsNodeLocalManagedObject;
class RASLoggingMessage;

class RASObjectManager : public WaveObjectManager
{
    private :
        RASObjectManager();
	virtual  PrismMessage *createMessageInstance (const UI32 &operationCode);
        virtual  WaveManagedObject    *createManagedObjectInstance (const string &managedClassName);
        vector<WaveManagedObject *> *DoSynchronousQuery  (const string className);
	void install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
	void failover (FailoverAsynchronousContext *pFailoverAsynchronousContext);
    protected :
    public :
        virtual ~RASObjectManager        ();

        static  string getClassName           ();
        static  RASObjectManager *getInstance            ();
        static  PrismServiceId       getPrismServiceId      ();
	void    RASClusterMessageHandler (RASClusterMessage *pRASClusterMessage);
	void 	copySupport(RASMessagingContext *pRASMessagingContext);

/*	void    RASSupportSaveMessageAsyncHandler (RASClusterMessage *pRASClusterMessage);
	void 	copySupportAsync(RASMessagingContext *pRASMessagingContext);
	void  	copySupportAsyncMessageCallback (FrameworkStatus frameworkStatus,
		    RASLoggingMessage *pMessage, RASMessagingContext *pRASMessagingContext); */

	void    RASClusterCfgMessageHandler (RASClusterMessage *pRASClusterMessage);
	void    RASFoscliMessageHandler (RASClusterMessage *pRASClusterMessage);
	void    foscliAsyncMessageCallback (FrameworkStatus frameworkStatus,
			RASLoggingMessage *pMessage, RASMessagingContext *pRASMessagingContext);
	void    foscliMessageHandler (RASMessagingContext *pRASMessagingContext);
	void    cfgMessageHandler(RASMessagingContext *pRASMessagingContext);
	void  	cfgMessageCallback (FrameworkStatus frameworkStatus,
		    RASLoggingMessage *pMessage, RASMessagingContext *pRASMessagingContext);

//	void 	RASClientMessageHandler (RASClientMessage *pRASClientMessage);
//	void    RASVCSMessage(PrismLinearSequencerContext *pRASMessagingContext);
//	void    RASVCSMessageCallback (FrameworkStatus frameworkStatus,
//	            RASClientMessage *pMessage, PrismLinearSequencerContext *pRASMessagingContext);

    // NetConf support
    void RASClusterNetConfMessageHandler(RASClusterNetConfMessage *pMessage);
    void RASClusterNetConfMessageCallback(WaveSendToClusterContext *pCtx);

    private :
    protected :
    public :
};

class SSObjectManager : public WaveObjectManager
{
    private :
        SSObjectManager();
        virtual  PrismMessage *createMessageInstance (const UI32 &operationCode);
        virtual  WaveManagedObject    *createManagedObjectInstance (const string &managedClassName);
        vector<WaveManagedObject *> *DoSynchronousQuery  (const string className);
        void failover (FailoverAsynchronousContext *pFailoverAsynchronousContext);
    protected :
    public :
        virtual ~SSObjectManager();
        static  string getClassName();
        static  SSObjectManager *getInstance();
        static  PrismServiceId       getPrismServiceId();
        void    RASSupportSaveMessageAsyncHandler (RASSupportClusterMessage *pRASSupportClusterMessage);
        void    copySupportAsync(PrismLinearSequencerContext *seq_ctx_p);
		void	copySupportAsyncCallback (WaveSendToClusterContext *ctx_p);
        void    copySupportAsyncMessageCallback (FrameworkStatus frameworkStatus,
                RASSupportMessage *pMessage, RASMessagingContext *pRASMessagingContext);
    private :
    protected :
    public :
};

}

#endif // RASOBJECTMANAGER_H
