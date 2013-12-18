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

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "DcmShell/DcmShell.h"
#include "DcmCore/DcmToolKit.h"
#include "RAS/Local/RASVCSLocalObjectManager.h"
#include "RAS/Local/RASTypes.h"
// #include "RAS.h"          // may be not used in hydra
#include "vcs.h"
#include "RAS/Local/LoggingLocalManagedObject.h"
#include "RAS/Local/RASLoggingMessage.h"
#include "ClientInterface/RAS/RASClientMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "DcmResourceIds.h"
#include <iostream>
#include <string>
#include <errno.h> 
#include <raslog/raslogd.h>
#include <raslog/raslogm.h>
#include <raslog/raslog.h>
#include <raslog/raslog_ras.h>
#include <em/em_if.h>

using namespace std;

namespace DcmNs
{   
RASVCSLocalObjectManager::RASVCSLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
		{
			trace (TRACE_LEVEL_INFO, "RASVCSLocalObjectManager::RASVCSLocalObjectManager: Entering...");

			addOperationMap (RAS_VCSLOGGING_LOCAL, reinterpret_cast<PrismMessageHandler> (&RASVCSLocalObjectManager::VCSMessageHandler));
		}

RASVCSLocalObjectManager::~RASVCSLocalObjectManager ()
{
}

RASVCSLocalObjectManager *RASVCSLocalObjectManager::getInstance ()
{
	static RASVCSLocalObjectManager *pRASVCSLocalObjectManager = new  RASVCSLocalObjectManager ();

	WaveNs::prismAssert (NULL !=  pRASVCSLocalObjectManager, __FILE__, __LINE__);

	return (pRASVCSLocalObjectManager);
}

PrismServiceId  RASVCSLocalObjectManager::getPrismServiceId()
{
	return ((getInstance ())->getServiceId ());
}

PrismMessage  *RASVCSLocalObjectManager::createMessageInstance(const UI32 &operationCode)
{
	PrismMessage *pPrismMessage = NULL;
	trace (TRACE_LEVEL_DEBUG, string ("RASVCSLocalOM::createMessageInstance opcode=") + operationCode);
	switch (operationCode)
	{
		case RAS_VCSLOGGING_LOCAL :
			pPrismMessage = new RASClientMessage(operationCode);
			break;
		default :
			pPrismMessage = NULL;
	}
	return (pPrismMessage);
}

WaveManagedObject  *RASVCSLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
{
	return(NULL);
}

// VCS MSG handler
void  RASVCSLocalObjectManager::VCSMessageHandler(RASClientMessage *pRASClientMessage)
{
	// Validations and Queries should go here
	tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSLocalOM::VCSMessageHandler RLID=%d SLID=%d",
			DcmToolKit::getLocalMappedId(), pRASClientMessage->getMappedId());

	// UI32 mappedId =  DcmToolKit::getLocalMappedId();
	// if (pRASClientMessage->getMappedId() ==  mappedId)
	//	return;

	// setup the sequence 
	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
		// If MappedId -> MO is needed, use queryLocalManagedObjectFromMappedId() to new/setup MO in RASLocalMessagingContext
		// reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASVCSLocalObjectManager::queryLocalManagedObjectFromMappedId),

		reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASVCSLocalObjectManager::VCSRASLog),
		reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASVCSLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
		reinterpret_cast<PrismSynchronousLinearSequencerStep> (&RASVCSLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
	};

	// put MSG to RASLocalMessagingContext
	RASLocalMessagingContext *pRASLocalMessagingContext = new RASLocalMessagingContext (reinterpret_cast<PrismMessage *> (pRASClientMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
	pRASLocalMessagingContext->execute ();
}

ResourceId RASVCSLocalObjectManager::VCSRASLog(RASLocalMessagingContext *pRASLocalMessagingContext)
{
	vcslog_t *pvcslog = new vcslog_t;
	rasevt_t *prasevt = new rasevt_t;
	rls_msg_t *msg = &pvcslog->vcsmsg;
	RASClientMessage *pRASClientMessage = reinterpret_cast<RASClientMessage *> (pRASLocalMessagingContext->getPPrismMessage ());

	tracePrintf(TRACE_LEVEL_DEBUG, "RASVCSLocalOM::RASClientMessage:"
			"swname=%s, VER=0x%x MAGIC=0x%x",
			pRASClientMessage->m_swname.c_str(), pRASClientMessage->m_hdr_ver,
			pRASClientMessage->m_hdr_magic);

	pRASClientMessage->setCStructure(pvcslog);

	RLS_SET_FLAGS(msg,(RLS_GET_FLAGS(msg) | RASEVT_FRCLOG | RASEVT_IGNORE_FFDC));
	RLS_SET_IQDROP(msg, 0);

	tracePrintf(TRACE_LEVEL_INFO, "RASVCSLocalOM::VCSRASLog:"
			"swname=%s, VER=0x%x MAGIC=0x%x OID=0x%x FLG=0x%x",
			RLS_GET_SWNAME(msg), RLS_GET_VER(msg), RLS_GET_MAGIC(msg), RLS_GET_OBJID(msg),
			RLS_GET_FLAGS(msg));

	prasevt->hdr = msg->hdr;
	prasevt->evt = msg->evt;

	/* preserve swname in audit username */
	RASEVT_SET_USERNAME(prasevt, pRASClientMessage->m_swname.c_str());
	/* indicate swname stored in audit_t */
	int instance = RASEVT_GET_INSTANCE_ALL(prasevt) | RAS_INSTANCE_VCS;
	RASEVT_SET_INSTANCE_ALL(prasevt,  instance);

	if (raslog_msg(prasevt)) {
		tracePrintf(TRACE_LEVEL_INFO, "RASVCSLocalOM::fail to call raslog_msg");
	}

	delete pvcslog;
	delete prasevt;

	return (WAVE_MESSAGE_SUCCESS);

}
} // namespace DcmNs
