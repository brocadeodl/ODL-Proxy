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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : jrodrigu                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Zone/Local/ZoneLocalObjectManager.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "SystemManagement/WaveSystemManagementGateway.h"

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"

namespace DcmNs
{
	ZoneLocalObjectManager::ZoneLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        /* Empty constructor due to no messages needing to be processed. This is service exists purely for listening for Local Events. */
    }

    ZoneLocalObjectManager::~ZoneLocalObjectManager ()
    {
    }

	ZoneLocalObjectManager  *ZoneLocalObjectManager::getInstance()
    {
        static ZoneLocalObjectManager *pZoneLocalObjectManager = new ZoneLocalObjectManager ();

        WaveNs::prismAssert (NULL != pZoneLocalObjectManager, __FILE__, __LINE__);

        return (pZoneLocalObjectManager);
    }

	string  ZoneLocalObjectManager::getClassName()
	{
		return ("ZoneLocal");
	}

	PrismServiceId  ZoneLocalObjectManager::getPrismServiceId()
	{
		return ((getInstance ())->getServiceId ());
	}

	void ZoneLocalObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases)
	{
		listenForEvent (VcsFabricLocalObjectManager::getPrismServiceId (),
						VCS_NODE_ZONE_ABORTED,
						reinterpret_cast<PrismEventHandler> (&ZoneLocalObjectManager::zoneAbortedEventHandler));

		listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (),
						FRAMEWORK_OBJECT_MANAGER_BROADCAST_PHASE3_COMPLETE_EVENT,
						reinterpret_cast<PrismEventHandler> (&ZoneLocalObjectManager::phase3CompleteEventHandler));

		pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForBootPhases->callback ();
	}

	/* handle the SCN event when zone transaction has aborted.  For the local ZoneService, we do
	   * nothing when this event comes in besides log the event.  The DCMD database should already
	   * have the latest and greatest information via the Principle switch.  We should only see 
	   * the zone aborted event locally for cluster-join scenarios where the local switch has an 
	   * open becomes the Subordinate switch. */ 
	void ZoneLocalObjectManager::zoneAbortedEventHandler (const VcsNodeZoneAbortedEvent *&pEvent)
	{
	   // ResourceId status = WAVE_MESSAGE_SUCCESS; 
	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	                "ZoneLocalObjectManager::zoneAbortedEventHandler: zone trans aborted here\n");

		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
	}

	/*
	 * This method overrides the virtual function defined in the WaveObjectManager:: base class.  The 
	 * purpose of this is to be have the Zone Plugin be notified when the Local switch has completed 
	 * cluster-join and we need to check FOS ZONEd if there is an open transaction that needs to be 
	 * aborted.  There should be no need to reload the DCMD database since the database is populated 
	 * by the Principle switch and should already have the latest data.
     */
	void ZoneLocalObjectManager::phase3CompleteEventHandler( const ClusterPhase3CompleteEvent* &pEvent )
	{   
		SI32 localDomainId = DcmToolKit::getThisVcsNodeDomainId ();
		ResourceId phase3Status =  DcmToolKit::getSpecificStatusFromMappedId(localDomainId);

		int token = 0;
	    int rc = 0;
	    int abortable;

		if (LOCATION_SECONDARY == FrameworkToolKit::getThisLocationRole ())
		{
	        cfgTransShow(&token, &abortable);

			tracePrintf (TRACE_LEVEL_INFO, false, false,"ZoneLocalObjectManager::phase3Complete "
						 "phase3Status %s, locallocRole = LOCATION_SECONDARY\n",
						 (FrameworkToolKit::localizeToSourceCodeEnum (phase3Status)).c_str());

			/** If there is an open transaction in eyes of FOS Zoning,
			 *  and we are the subordinate switch, abort the open
			 *  transaction. */
			if (token > 0) {
				rc = cfgTransForceAbort(token);
				if (rc != 0) {
					tracePrintf (TRACE_LEVEL_INFO, false, false,"ZoneLocalObjectManager::phase3Complete "
						 "open transaction, abort failed! rc %d\n", rc);
				} else {
					tracePrintf (TRACE_LEVEL_INFO, false, false,"ZoneLocalObjectManager::phase3Complete "
						 "open transaction, abort successful, rc %d\n", rc);
				}
			}

			/** We also need to delete any Wyser-cache entries from the
			 *  Subordinate switch to avoid invalid cache-lookups. The
			 *  principle switch should be the only one to have the Wyser
			 *  cache table. */
			trace (TRACE_LEVEL_INFO, string ("ZoneLocalObjectManager::phase3Complete: deleting all Wyser cfg MO ObjIds"));
			WaveSystemManagementGatewayWorker::deleteObjectIdsForBaseTargetNodeName("zoning defined-configuration cfg");
			trace (TRACE_LEVEL_INFO, string ("ZoneLocalObjectManager::phase3Complete: deleting all Wyser zone MO ObjIds"));
			WaveSystemManagementGatewayWorker::deleteObjectIdsForBaseTargetNodeName("zoning defined-configuration zone");
			trace (TRACE_LEVEL_INFO, string ("ZoneLocalObjectManager::phase3Complete: deleting all Wyser alias MO ObjIds"));
			WaveSystemManagementGatewayWorker::deleteObjectIdsForBaseTargetNodeName("zoning defined-configuration alias");

		} else {
			tracePrintf (TRACE_LEVEL_INFO, false, false,"ZoneLocalObjectManager::phase3Complete "
						 "phase3Status %s, locallocRole = LOCATION_PRIMARY\n",
						 (FrameworkToolKit::localizeToSourceCodeEnum (phase3Status)).c_str());
		}
		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
	}

}
