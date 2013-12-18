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
 *   Author : Ayush Jaiswal												   *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/LocalSecShowWorker.h"
#include "ThresholdMonitor/Local/SecLocalShowMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include <thresh/fwdPub.h>
#include <thresh/fwd.h>
#include <thresh/portCmd.h>
//#include <fabos/fabioctl.h>
#include <hasm/ls.h>
#include <em/em_if.h>
namespace DcmNs
{
	LocalSecShowWorker::LocalSecShowWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
		: WaveWorker  (pThresholdMonitorLocalObjectManager)
		{

			addOperationMap (SECURITYLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&LocalSecShowWorker::SecLocalShowMessageHandler));
		}
	LocalSecShowWorker::~LocalSecShowWorker ()
	{
	}
	
	PrismMessage  *LocalSecShowWorker::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		switch (operationCode)
		{
			case SECURITYLOCALSHOW :
				pPrismMessage = new SecLocalShowMessage();
				break;
			default :
				pPrismMessage = NULL;
		}
		return (pPrismMessage);
			
	}

	void LocalSecShowWorker::SecLocalShowMessageHandler(SecLocalShowMessage *pSecLocalShowMessage)
	{
		trace (TRACE_LEVEL_INFO, "Entering SecLocalShowMessageHandler..");
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		int		   ret	  = 0;
		thThresholdEntry thEntry;
		memset(&thEntry, 0, sizeof (thEntry));
		UI32 loc_id = FrameworkToolKit::getThisLocationId();

		UI32 areaName = pSecLocalShowMessage->getAreaName();
		
		/* Call backend API and fill fetched values*/		
		ret = thaThresholdStructGet(AGENT_SECURITY, areaName, 1, &thEntry);

		trace(TRACE_LEVEL_INFO, string("Ret val of LocalSecShowWorker::SecLocalShowMessageHandler: thaThresholdStructGet is ") +  ret);

		/* Fill buffer with structure fetched from backend and tag with location Id*/
		pSecLocalShowMessage->addBuffer (loc_id, sizeof (thThresholdEntry), &thEntry, false);
		trace (TRACE_LEVEL_INFO, "LocalSecShowWorker::SecLocalShowMessageHandler: Added buffer to msg");
		pSecLocalShowMessage->setCompletionStatus (status);
		reply(pSecLocalShowMessage);
		trace (TRACE_LEVEL_INFO, "LocalSecShowWorker::SecLocalShowMessageHandler: Replied with pSecLocalShowMessage");
		return;
	}
}
