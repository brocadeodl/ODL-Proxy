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

#ifndef RASVCSOBJECTMANAGER_H
#define RASVCSOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Boot/BootTypes.h"

#include "RAS/Global/RASClusterMessage.h"
#include "RAS/Global/RASObjectManagerTypes.h"
#include "RAS/Local/LoggingLocalManagedObject.h"
#include "RAS/Global/RASManagedObject.h"
#include "RAS/Global/RASMessagingContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "ClientInterface/RAS/RASClientMessage.h"
#include "RAS/Local/RASLocalMessageContext.h"
#include "RAS/Local/RASLoggingMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class LoggingLocalManagedObject;
class RASManagedObject;
class VcsNodeLocalManagedObject;
class RASLoggingMessage;

class RASVCSObjectManager : public WaveObjectManager
{
	private :
		RASVCSObjectManager();
		virtual  PrismMessage *createMessageInstance (const UI32 &operationCode);
		virtual  WaveManagedObject    *createManagedObjectInstance (const string &managedClassName);
		vector<WaveManagedObject *> *DoSynchronousQuery  (const string className);
		void failover (FrameworkObjectManagerFailoverReason failoverReason,
				vector<LocationId> failedLocationIds, PrismAsynchronousContext *pPrismAsynchronousContext);
	protected :
	public :
		virtual ~RASVCSObjectManager        ();

		static  string getClassName           ();
		static  RASVCSObjectManager *getInstance            ();
		static  PrismServiceId       getPrismServiceId      ();
		void 	RASClientMessageHandler (RASClientMessage *pRASClientMessage);
		void    RASVCSMessage(PrismLinearSequencerContext *pRASMessagingContext);
		void    RASVCSMessageCallback (FrameworkStatus frameworkStatus,
				RASClientMessage *pMessage, PrismLinearSequencerContext *pRASMessagingContext);

	private :
	protected :
	public :
};

inline string RASVCSObjectManager::getClassName ()
{
    return ("RASVCSObjectManager");
}

}
#endif // RASVCSOBJECTMANAGER_H
