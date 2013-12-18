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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : krangara                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "FspfService/Local/FspfServiceLocalObjectManager.h"
#include "FspfService/Local/BcastPriorityLocalManagedObject.h"
#include "FspfService/Local/FspfServiceGetBroadcastInfoMessage.h"
#include "FspfService/Local/FspfServiceGetTopologyInfoMessage.h"
#include "FspfService/Local/FspfServiceGetNeighborStateinfoInfoMessage.h"
#include "FspfService/Local/FspfServiceUpdateBcastPriorityInfoMessage.h"
#include "FspfService/Local/FspfServiceTypes.h"
#include "FspfService/Global/FspfServiceGlobalObjectManager.h"

#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"

#define BCAST_INDEX 256

namespace DcmNs
{

    FspfServiceLocalObjectManager::FspfServiceLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        associateWithVirtualWaveObjectManager (FspfServiceGlobalObjectManager::getInstance ());
        BcastPriorityLocalManagedObject    BcastPriorityLocalManagedObject (this);
        BcastPriorityLocalManagedObject.setupOrm ();
        addManagedClass (BcastPriorityLocalManagedObject::getClassName ());
        addOperationMap (FSPFSERVICEGETBROADCASTINFO, reinterpret_cast<PrismMessageHandler> (&FspfServiceLocalObjectManager::FspfServiceGetBroadcastInfoMessageHandler));
        addOperationMap (FSPFSERVICEGETTOPOLOGYINFO, reinterpret_cast<PrismMessageHandler> (&FspfServiceLocalObjectManager::FspfServiceGetTopologyInfoMessageHandler));
        addOperationMap (FSPFSERVICEGETNEIGHBORSTATEINFOINFO, reinterpret_cast<PrismMessageHandler> (&FspfServiceLocalObjectManager::FspfServiceGetNeighborStateinfoInfoMessageHandler));
        addOperationMap (FSPFSERVICEUPDATEBCASTPRIORITYINFO, reinterpret_cast<PrismMessageHandler> (&FspfServiceLocalObjectManager::FspfServiceUpdateBcastPriorityInfoMessageHandler));
         PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
    }

    FspfServiceLocalObjectManager::~FspfServiceLocalObjectManager ()
    {
    }

    FspfServiceLocalObjectManager  *FspfServiceLocalObjectManager::getInstance()
    {
        static FspfServiceLocalObjectManager *pFspfServiceLocalObjectManager = new FspfServiceLocalObjectManager ();

        WaveNs::prismAssert (NULL != pFspfServiceLocalObjectManager, __FILE__, __LINE__);

        return (pFspfServiceLocalObjectManager);
    }

    string  FspfServiceLocalObjectManager::getClassName()
    {
        return ("FspfService");
    }

    PrismServiceId  FspfServiceLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FspfServiceLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FSPFSERVICEGETBROADCASTINFO :
                pPrismMessage = new FspfServiceGetBroadcastInfoMessage ();
                break;
            case FSPFSERVICEGETTOPOLOGYINFO :
                pPrismMessage = new FspfServiceGetTopologyInfoMessage ();
                break;
            case FSPFSERVICEGETNEIGHBORSTATEINFOINFO :
                pPrismMessage = new FspfServiceGetNeighborStateinfoInfoMessage ();
                break;
            case FSPFSERVICEUPDATEBCASTPRIORITYINFO :
                pPrismMessage = new FspfServiceUpdateBcastPriorityInfoMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FspfServiceLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
       WaveManagedObject *pWaveManagedObject = NULL;

        if ((BcastPriorityLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new BcastPriorityLocalManagedObject(this);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "FspfServiceLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

/***********************************************
 GetBcastInfoMessage Handler's & API's
        (a.k.a bcastShow or "show fabric bcast")
***********************************************/
	ResourceId FspfServiceLocalObjectManager::getBcastEntries (PrismSynchronousLinearSequencerContext *pFspfServiceGetBroadcastInfoMessageContext)
	{
		FspfServiceGetBroadcastInfoMessage *pFspfServiceGetBroadcastInfoMessage = reinterpret_cast<FspfServiceGetBroadcastInfoMessage *> (pFspfServiceGetBroadcastInfoMessageContext->getPPrismMessage ());
		vector<UI32> port_isl;
		vector<UI32> bcast_route_avail;

		vector<SI32> srcBlade;
		vector<SI32> srcPort;
		vector<SI32> srcIndex;
		vector<SI32> srcDispType;
		vector<SI32> nbrDomain;
		vector<SI32> nbrBlade;
		vector<SI32> nbrPort;
		vector<SI32> nbrIndex;
		vector<SI32> nbrDispType;
		vector<SI32> interfaceType;
		vector<UI32> trunkPort;
		vector<UI32> bandwidth;
                vector<SI32> local_qsfpidx;
                vector<SI32> remote_qsfpidx;

		int i;
		bcast_info_t	bcastInfo;

		trace (TRACE_LEVEL_DEBUG, "FSPF Plugin: GetBroadcastInfoMessage: entering...");
		memset(&bcastInfo, 0, sizeof (bcast_info_t));

		getBcastInfo(&bcastInfo, BCAST_INDEX);

		trace(TRACE_LEVEL_INFO, "FSPF Plugin: GetBroadcastInfoMessage: bcastInfo.portCount=" + bcastInfo.portCount);

		for (i = 0; i < bcastInfo.portCount ; i++) {
			int tmpDomain, tmpBlade, tmpPort, qsfp_idx = -1;
			interface_display_type_t srcIfDispType, nbrIfDispType;
			/* get nbr port tuple info to send back */

			port_isl.push_back(bcastInfo.bcast_entry_list[i].port_isl);
			bcast_route_avail.push_back(bcastInfo.bcast_entry_list[i].bcast_route_avail);

			/* Get local port details */
			getPhyPortTuple(bcastInfo.bcast_entry_list[i].localPort, &tmpDomain, &tmpBlade, &tmpPort);
			srcIfDispType = getInterfaceDisplayType(bcastInfo.bcast_entry_list[i].localPort);
			srcBlade.push_back(tmpBlade);
			srcPort.push_back(tmpPort);
			srcIndex.push_back(bcastInfo.bcast_entry_list[i].localPort);
			srcDispType.push_back((SI32)srcIfDispType);
                        local_qsfpidx.push_back(getQSFPTupleIndex(bcastInfo.bcast_entry_list[i].localPort));

			/* Get neighbor port information */
			getPortTupleRemote(bcastInfo.bcast_entry_list[i].localPort, &tmpDomain, &tmpBlade, &tmpPort, &qsfp_idx);
			nbrIfDispType = getInterfaceDisplayTypeRemote(bcastInfo.bcast_entry_list[i].localPort);
			nbrDomain.push_back(tmpDomain);
			nbrBlade.push_back(tmpBlade);
			nbrPort.push_back(tmpPort);
			nbrIndex.push_back(bcastInfo.bcast_entry_list[i].nbrPort);
                        remote_qsfpidx.push_back(qsfp_idx);
			if (nbrIfDispType == eTYPE_INVALID || nbrIfDispType == (interface_display_type_t)-1) {
				nbrDispType.push_back((SI32)srcIfDispType);
			} else {
				nbrDispType.push_back((SI32)nbrIfDispType);
			}

			/* Interface Type */
			interfaceType.push_back((SI32)getInterfaceType(bcastInfo.bcast_entry_list[i].localPort));

			trunkPort.push_back(bcastInfo.bcast_entry_list[i].trunkPort);
			bandwidth.push_back(bcastInfo.bcast_entry_list[i].bandwidth);
		}

		pFspfServiceGetBroadcastInfoMessage->setPort_isl(port_isl);
		pFspfServiceGetBroadcastInfoMessage->setBcast_route_avail(bcast_route_avail);
		pFspfServiceGetBroadcastInfoMessage->setSrcBlade(srcBlade);
		pFspfServiceGetBroadcastInfoMessage->setSrcPort(srcPort);
		pFspfServiceGetBroadcastInfoMessage->setSrcIndex(srcIndex);
		pFspfServiceGetBroadcastInfoMessage->setSrcDispType(srcDispType);
		pFspfServiceGetBroadcastInfoMessage->setNbrDomain(nbrDomain);
		pFspfServiceGetBroadcastInfoMessage->setNbrBlade(nbrBlade);
		pFspfServiceGetBroadcastInfoMessage->setNbrPort(nbrPort);
		pFspfServiceGetBroadcastInfoMessage->setNbrIndex(nbrIndex);
		pFspfServiceGetBroadcastInfoMessage->setNbrDispType(nbrDispType);
		pFspfServiceGetBroadcastInfoMessage->setInterfaceType(interfaceType);
		pFspfServiceGetBroadcastInfoMessage->setTrunkPort(trunkPort);
		pFspfServiceGetBroadcastInfoMessage->setBandwidth(bandwidth);
                pFspfServiceGetBroadcastInfoMessage->setLocal_qsfpidx(local_qsfpidx);
                pFspfServiceGetBroadcastInfoMessage->setRemote_qsfpidx(remote_qsfpidx);
		
                pFspfServiceGetBroadcastInfoMessage->setBcastRootDomain(bcastInfo.bcastRootDomain);
		pFspfServiceGetBroadcastInfoMessage->setBcastRootPriority(bcastInfo.bcastRootPriority);
		pFspfServiceGetBroadcastInfoMessage->setBcastRootIPAddr(bcastInfo.bcastRootIPAddr);
		pFspfServiceGetBroadcastInfoMessage->setBcastRootWwn(bcastInfo.bcastRootWwn);
		pFspfServiceGetBroadcastInfoMessage->setBcastRootSwitchName(bcastInfo.bcastRootSwname);
		pFspfServiceGetBroadcastInfoMessage->setBcastLocalPriority(bcastInfo.bcastLocalPriority);

		if (bcastInfo.bcast_entry_list != NULL) {
			free(bcastInfo.bcast_entry_list);
		}

		trace(TRACE_LEVEL_DEBUG, "FSPF Plugin: GetBroadcastInfoMessage: exiting...");
		return (WAVE_MESSAGE_SUCCESS);
	}

    void  FspfServiceLocalObjectManager::FspfServiceGetBroadcastInfoMessageHandler( FspfServiceGetBroadcastInfoMessage *pFspfServiceGetBroadcastInfoMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

	    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::getBcastEntries),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFspfServiceGetBroadcastInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

/**************************************************************
 GetTopologyInfoMessage Handler's & API's
        (a.k.a topologyShow or "show fabric route topology")
***************************************************************/
    ResourceId FspfServiceLocalObjectManager:: getFabricTopologyInfo(PrismSynchronousLinearSequencerContext *pFspfServiceGetTopologyInfoMessageContext)
    {
        FspfServiceGetTopologyInfoMessage *pFspfServiceGetTopologyInfoMessage =
                reinterpret_cast<FspfServiceGetTopologyInfoMessage *> (pFspfServiceGetTopologyInfoMessageContext->getPPrismMessage());

	trace (TRACE_LEVEL_DEBUG, "Fspf Plugin: GetTopologyInfoMessage - entering...");

	unsigned int dest_domain = pFspfServiceGetTopologyInfoMessage->getDestDomainId()[0];
	int domain_count = 0; // indicates #domains or error (if any)
	topology_t *dom_topo_list = NULL;
	int i, j, size = 0;

	/*
	 * Check if request is to fetch path "To" a specific domain
	 * If not, provide path info "To all" domains
	 */
	if (dest_domain > 0 && dest_domain < FSPF_MAX_DOMAINS) {
		dom_topo_list = (topology_t *)malloc(sizeof(topology_t));
		size = sizeof(topology_t);
	} else {
		dest_domain = 0;
		dom_topo_list = (topology_t *)malloc(sizeof(topology_t) * FSPF_MAX_DOMAINS);
		size = sizeof(topology_t) * FSPF_MAX_DOMAINS;
	}

	if (dom_topo_list == NULL) {
		domain_count = ERR_MALLOC_FAILED;
		trace (TRACE_LEVEL_ERROR,string("Fsfp Plugin: GetFabricTopologyInfoMessage - failed to allocate memory!!"));
		pFspfServiceGetTopologyInfoMessage->setDomainCount(domain_count);
		trace (TRACE_LEVEL_DEBUG, string("Fspf Plugin: GetFabricTopologyInfoMessage - exiting..."));
		return (WAVE_MESSAGE_SUCCESS);
	}
	/* memeset after NULL check */
	memset(dom_topo_list, 0, size);

	/* FOS API call to get info */
	domain_count = getTopologyInfo(dest_domain, dom_topo_list);

	trace (TRACE_LEVEL_INFO, string("Fspf Plugin: getTopologyInfo() dest_dom= ") + dest_domain +
						string(" ret #entries= ") + domain_count);

	if (domain_count > 0) {
		vector<UI32>  destDomainId;
		vector<UI32>  lowestPathCost;
		vector<string>  destDomainName;
		vector<UI32>  noOfHopsFromSrc;
		vector<UI32>  pathCount;
		vector<UI32>  bitsPerSec;
		vector<UI32>  pathFlags;
		vector<UI32>  pathIngressPortCount;
		vector<UI32>  pathIngressPortBM;
		vector<UI32>  pathCost;
		vector<UI32>  pathCurrentBWAlloc;
		vector<UI32>  pathBWUtilization;
		vector<UI32>  pathTotalBW;
		vector<SI32>  srcBlade;
		vector<SI32>  srcPort;
		vector<SI32>  srcIndex;
		vector<SI32>  srcDispType;
		vector<SI32>  nbrDomain;
		vector<SI32>  nbrBlade;
		vector<SI32>  nbrPort;
		vector<SI32>  nbrIndex;
		vector<SI32>  nbrDispType;
		vector<SI32>  interfaceType;
		vector<UI32>  trunkPort;
                vector<SI32>  local_qsfpidx;
                vector<SI32>  remote_qsfpidx;


		for (i = 0; i < domain_count; i++) {
			/* Set Domain info */
			destDomainId.push_back(dom_topo_list[i].domain_info.t_domain);
			lowestPathCost.push_back(dom_topo_list[i].domain_info.t_metric);
			destDomainName.push_back(string((char *)&dom_topo_list[i].domain_info.t_name));
			noOfHopsFromSrc.push_back(dom_topo_list[i].domain_info.t_uHopsFromRoot);
			pathCount.push_back(dom_topo_list[i].domain_info.t_pathCnt);
			bitsPerSec.push_back(dom_topo_list[i].domain_info.t_bps);

			/* Set Path info associated with each domain */
			for (j = 0; j < dom_topo_list[i].domain_info.t_pathCnt; j++) {
				int tmpDomain, tmpBlade, tmpPort, qsfp_idx = -1;
				interface_type_t ifType;
				interface_display_type_t srcIfDispType, nbrIfDispType;

				pathFlags.push_back(dom_topo_list[i].path_info[j].path_entry.pflags);
				pathIngressPortCount.push_back(dom_topo_list[i].path_info[j].path_entry.portcnt);
				/* pathIngressPortBM.push_back(dom_topo_list[i].t portsP); */ /* TBD port Bit Map */
				pathCost.push_back(dom_topo_list[i].path_info[j].path_entry.metric);
				pathCurrentBWAlloc.push_back(dom_topo_list[i].path_info[j].path_entry.curr_bw_alloc);
				pathBWUtilization.push_back(dom_topo_list[i].path_info[j].path_entry.bw_utilization);
				pathTotalBW.push_back(dom_topo_list[i].path_info[j].path_entry.total_bw);
				trunkPort.push_back(dom_topo_list[i].path_info[j].trunkPort);

				/* Get local port details */
				getPhyPortTuple(dom_topo_list[i].path_info[j].path_entry.outport, &tmpDomain, &tmpBlade, &tmpPort);
				srcIfDispType = getInterfaceDisplayType(dom_topo_list[i].path_info[j].path_entry.outport);
				srcIndex.push_back(dom_topo_list[i].path_info[j].path_entry.outport);
				srcBlade.push_back(tmpBlade);
				srcPort.push_back(tmpPort);
				srcDispType.push_back((int)srcIfDispType);
                                local_qsfpidx.push_back(getQSFPTupleIndex(dom_topo_list[i].path_info[j].path_entry.outport));

				/* Get nbr port tuple info to send back */
				getPortTupleRemote(dom_topo_list[i].path_info[j].path_entry.outport, &tmpDomain, &tmpBlade, &tmpPort, &qsfp_idx);
				nbrIfDispType = getInterfaceDisplayTypeRemote(dom_topo_list[i].path_info[j].path_entry.outport);
				nbrIndex.push_back(dom_topo_list[i].path_info[j].nbrPort);
				nbrDomain.push_back(tmpDomain);
				nbrBlade.push_back(tmpBlade);
				nbrPort.push_back(tmpPort);
                                remote_qsfpidx.push_back(qsfp_idx);
				if (nbrIfDispType == eTYPE_INVALID || nbrIfDispType == (interface_display_type_t)-1) {
					nbrDispType.push_back(srcIfDispType);
				} else {
					nbrDispType.push_back(nbrIfDispType);
				}

				/* Interface Type */
				ifType = getInterfaceType(dom_topo_list[i].path_info[j].path_entry.outport);
				interfaceType.push_back((int)ifType);
			}
		}

		pFspfServiceGetTopologyInfoMessage->setDestDomainId(destDomainId);
		pFspfServiceGetTopologyInfoMessage->setLowestPathCost(lowestPathCost);
		pFspfServiceGetTopologyInfoMessage->setDestDomainName(destDomainName);
		pFspfServiceGetTopologyInfoMessage->setNoOfHopsFromSrc(noOfHopsFromSrc);
		pFspfServiceGetTopologyInfoMessage->setPathCount(pathCount);
		pFspfServiceGetTopologyInfoMessage->setBitsPerSec(bitsPerSec);
		pFspfServiceGetTopologyInfoMessage->setPathFlags(pathFlags);
		pFspfServiceGetTopologyInfoMessage->setPathIngressPortCount(pathIngressPortCount);
		pFspfServiceGetTopologyInfoMessage->setPathIngressPortBM(pathIngressPortBM);
		pFspfServiceGetTopologyInfoMessage->setPathCost(pathCost);
		pFspfServiceGetTopologyInfoMessage->setPathCurrentBWAlloc(pathCurrentBWAlloc);
		pFspfServiceGetTopologyInfoMessage->setPathBWUtilization(pathBWUtilization);
		pFspfServiceGetTopologyInfoMessage->setPathTotalBW(pathTotalBW);
		pFspfServiceGetTopologyInfoMessage->setSrcBlade(srcBlade);
		pFspfServiceGetTopologyInfoMessage->setSrcPort(srcPort);
		pFspfServiceGetTopologyInfoMessage->setSrcIndex(srcIndex);
		pFspfServiceGetTopologyInfoMessage->setSrcDispType(srcDispType);
		pFspfServiceGetTopologyInfoMessage->setNbrDomain(nbrDomain);
		pFspfServiceGetTopologyInfoMessage->setNbrBlade(nbrBlade);
		pFspfServiceGetTopologyInfoMessage->setNbrPort(nbrPort);
		pFspfServiceGetTopologyInfoMessage->setNbrIndex(nbrIndex);
		pFspfServiceGetTopologyInfoMessage->setNbrDispType(nbrDispType);
		pFspfServiceGetTopologyInfoMessage->setInterfaceType(interfaceType);
		pFspfServiceGetTopologyInfoMessage->setTrunkPort(trunkPort);
                pFspfServiceGetTopologyInfoMessage->setLocal_qsfpidx(local_qsfpidx);
                pFspfServiceGetTopologyInfoMessage->setRemote_qsfpidx(remote_qsfpidx);
	}

	/* Free allocated memory */
	free(dom_topo_list);

	pFspfServiceGetTopologyInfoMessage->setDomainCount(domain_count);
	trace (TRACE_LEVEL_DEBUG, string("Fspf Plugin: GetFabricTopologyInfoMessage - exiting..."));
	return (WAVE_MESSAGE_SUCCESS);
    }

/**************************************************************
 Get NeighborStateInfoMessage Handler's & API's
        (a.k.a nbrstateShow or "show fabric route neighbor-state")
***************************************************************/
    ResourceId FspfServiceLocalObjectManager:: getFabricNeighborStateInfo(PrismSynchronousLinearSequencerContext *pFspfServiceGetNeighborStateInfoMessageContext)
    {
        FspfServiceGetNeighborStateinfoInfoMessage *pFspfServiceGetNeighborStateInfoMessage =
                reinterpret_cast<FspfServiceGetNeighborStateinfoInfoMessage *> (pFspfServiceGetNeighborStateInfoMessageContext->getPPrismMessage());

	trace (TRACE_LEVEL_INFO, "Fspf Plugin: GetNeighborStateInfoMessage - entering...");
        unsigned int port_index = pFspfServiceGetNeighborStateInfoMessage->getInput_port();
        unsigned int port_count = pFspfServiceGetNeighborStateInfoMessage->getIsl_portcount();
        isl_info_t *isl_info_list = NULL;

        if (port_count == 0) {
		isl_info_list = (isl_info_t *)malloc(sizeof (isl_info_t) * FSPF_MAX_LINKS);
	} else {
		isl_info_list = (isl_info_t *)malloc(sizeof (isl_info_t));
	}

	if (isl_info_list == NULL) {
		port_count = ERR_MALLOC_FAILED;
		trace (TRACE_LEVEL_ERROR,string("Fsfp Plugin: GetFabricNeighborStateInfoMessage - failed to allocate memory!!"));
		pFspfServiceGetNeighborStateInfoMessage->setIsl_portcount(port_count);
		trace (TRACE_LEVEL_INFO, string("Fspf Plugin: GetFabricNeighborStatenfoMessage - exiting..."));
		return (WAVE_MESSAGE_SUCCESS);
	}

	if (port_count > 0) {
		int rval;
		if ((rval = getIslInfo(port_index, isl_info_list)) != OK) {
			pFspfServiceGetNeighborStateInfoMessage->setIsl_portcount(rval);
			trace (TRACE_LEVEL_INFO, string("Fspf Plugin: GetFabricNeighborStatenfoMessage - exiting..."));
			FREE(isl_info_list);
			return (WAVE_MESSAGE_SUCCESS);
		}
	} else {
		port_count = getIslList(isl_info_list);
	}

	unsigned int i;
	vector<SI32>  remotedomain;
	vector<SI32>  remoteport;
	vector<SI32>  nbrstate;
	vector<string>  nbrstatestr;
	vector<SI32>  destblade;
	vector<SI32>  remoteportindex;
	vector<SI32>  srcBlade;
	vector<SI32>  srcPort;
	vector<SI32>  srcIndex;
	vector<SI32>  srcDispType;
	vector<SI32>  nbrDispType;
	vector<SI32>  interfaceType;
        vector<SI32>  local_qsfpidx;
        vector<SI32>  remote_qsfpidx;

	for (i = 0; i < port_count; i++) {
		int tmpDomain   = 0;
		int tmpBlade    = 0;
		int tmpPort     = 0;
                int qsfp_idx    = -1;
		interface_type_t ifType;
		interface_display_type_t srcIfDispType, nbrIfDispType;

		/* Get local port details */
		getPhyPortTuple(isl_info_list[i].localPort, &tmpDomain, &tmpBlade, &tmpPort);
		srcIfDispType = getInterfaceDisplayType(isl_info_list[i].localPort);
		srcIndex.push_back(isl_info_list[i].localPort);
		srcBlade.push_back(tmpBlade);
		srcPort.push_back(tmpPort);
		srcDispType.push_back((int)srcIfDispType);
                local_qsfpidx.push_back(getQSFPTupleIndex(isl_info_list[i].localPort));

		/* get nbr port tuple info to send back */
		getPortTupleRemote(isl_info_list[i].localPort, &tmpDomain, &tmpBlade, &tmpPort, &qsfp_idx);
		nbrIfDispType = getInterfaceDisplayTypeRemote(isl_info_list[i].localPort);
		remotedomain.push_back(isl_info_list[i].domain);
		remoteport.push_back(tmpPort);
		nbrstate.push_back(isl_info_list[i].fspfState);
		nbrstatestr.push_back(isl_info_list[i].nbrState);
		destblade.push_back(tmpBlade);
		remoteportindex.push_back(isl_info_list[i].remPort);
                remote_qsfpidx.push_back(qsfp_idx);
		if (nbrIfDispType == eTYPE_INVALID || nbrIfDispType == (interface_display_type_t)-1) {
			nbrDispType.push_back(srcIfDispType);
		} else {
			nbrDispType.push_back(nbrIfDispType);
		}

		/* Interface Type */
		ifType = getInterfaceType(isl_info_list[i].localPort);
		interfaceType.push_back((int)ifType);
	}
	pFspfServiceGetNeighborStateInfoMessage->setSrc_port(srcPort);
	pFspfServiceGetNeighborStateInfoMessage->setSrc_index(srcIndex);
	pFspfServiceGetNeighborStateInfoMessage->setSrc_blade(srcBlade);
	pFspfServiceGetNeighborStateInfoMessage->setSrc_dispType(srcDispType);
	pFspfServiceGetNeighborStateInfoMessage->setDomain_id(remotedomain);
	pFspfServiceGetNeighborStateInfoMessage->setDest_blade(destblade);
	pFspfServiceGetNeighborStateInfoMessage->setDest_port(remoteport);
	pFspfServiceGetNeighborStateInfoMessage->setDest_index(remoteportindex);
	pFspfServiceGetNeighborStateInfoMessage->setNbr_dispType(nbrDispType);
	pFspfServiceGetNeighborStateInfoMessage->setNbr_state(nbrstate);
	pFspfServiceGetNeighborStateInfoMessage->setNbr_state_str(nbrstatestr);
	pFspfServiceGetNeighborStateInfoMessage->set_interfaceType(interfaceType);
        pFspfServiceGetNeighborStateInfoMessage->setLocal_qsfpidx(local_qsfpidx);
        pFspfServiceGetNeighborStateInfoMessage->setRemote_qsfpidx(remote_qsfpidx);

	FREE(isl_info_list);
	pFspfServiceGetNeighborStateInfoMessage->setIsl_portcount(port_count);
	trace (TRACE_LEVEL_INFO, string("Fspf Plugin: GetFabricNeighborInfoMessage - exiting..."));
	return (WAVE_MESSAGE_SUCCESS);
    }
    void  FspfServiceLocalObjectManager::FspfServiceGetTopologyInfoMessageHandler( FspfServiceGetTopologyInfoMessage *pFspfServiceGetTopologyInfoMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
	    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::getFabricTopologyInfo),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFspfServiceGetTopologyInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  FspfServiceLocalObjectManager::FspfServiceGetNeighborStateinfoInfoMessageHandler( FspfServiceGetNeighborStateinfoInfoMessage *pFspfServiceGetNeighborStateinfoInfoMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::getFabricNeighborStateInfo),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFspfServiceGetNeighborStateinfoInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
/*******************************
 * Get/Set Bcast Priority      *
 ******************************/

	/* Query Local Managed Object */
	BcastPriorityLocalManagedObject *FspfServiceLocalObjectManager::QueryBcastLocalManagedObject(UI32 switchId)
	{
		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (BcastPriorityLocalManagedObject::getClassName ());

		trace (TRACE_LEVEL_DEBUG, "BcastPriorityLMObj::queryLMObjFromId:Entering");

		synchronousQueryContext.addAndAttribute (new AttributeUI32 (switchId, "switchId"));

		vector<WaveManagedObject *> *pResults =
		       querySynchronously (&synchronousQueryContext);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();

			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "BcastPriorityLocalManagedObject::QueryBcastLocalManagedObject: Cannot find bcastPriority entry");
				pResults->clear ();
				delete pResults;
				return (NULL);
			} else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_INFO, "BcastPriorityLocalManagedObject::QueryBcastLocalManagedObject::found 1 bcastPriority entry");
				BcastPriorityLocalManagedObject *pBcastPriorityLocalManagedObject =
				              dynamic_cast<BcastPriorityLocalManagedObject *> ((*pResults)[0]);

				pResults->clear ();
				delete pResults;
				return (pBcastPriorityLocalManagedObject);
			} else {
				trace (TRACE_LEVEL_INFO, "BcastPriorityLocalManagedObject::QueryBcastLocalManagedObject: There can be only one local node.");
				pResults->clear ();
				delete pResults;
				prismAssert (false, __FILE__, __LINE__);
				return (NULL);
			}
		} else {
			trace (TRACE_LEVEL_FATAL, "BcastPriorityLocalManagedObject::QueryBcastLocalManagedObject: System failure");
			prismAssert (false, __FILE__, __LINE__);
			return (NULL);
		}

		return (NULL);
	}

	/* Install Managed Object */
	void FspfServiceLocalObjectManager::install(
	     WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		int ret = 0;
		int localId;

		trace (TRACE_LEVEL_DEBUG, "FspfServiceLocalObjMgr::install");
		localId = DcmToolKit::getLocalMappedId();
		trace (TRACE_LEVEL_INFO, string("FspfServiceLocalObjectManager::install: localId ") + localId);
		BcastPriorityLocalManagedObject *pBcastPriorityLocalManagedObject =
		                                 QueryBcastLocalManagedObject(localId);

		if (pBcastPriorityLocalManagedObject) {
			trace (TRACE_LEVEL_INFO, "FspfServiceLocalObjectManager::install - DB Exists");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			// if we are here for the very first time, install default db entries
			trace (TRACE_LEVEL_INFO, "FspfServiceLocalObjMgr::install: create default entries");
			startTransaction();

			pBcastPriorityLocalManagedObject = new BcastPriorityLocalManagedObject(this);

			if (pBcastPriorityLocalManagedObject) {
				trace (TRACE_LEVEL_INFO, string("FspfServiceLocalObjectManager::install: MO created localId ") + localId);
				pBcastPriorityLocalManagedObject->setSwitchId(localId);
				pBcastPriorityLocalManagedObject->setBcastPriority(MCAST_PRIORITY_DEFAULT);

				ret = setMcastPriority(pBcastPriorityLocalManagedObject->getBcastPriority());
				if (ret == -1) {
					trace (TRACE_LEVEL_ERROR, "FspfServiceLocalObjectManager::install: MO created Invalid priority value");
					status = WAVE_MESSAGE_SUCCESS;
				}
				status = commitTransaction ();

				trace (TRACE_LEVEL_DEBUG, "FspfServiceLocalObjectManager::install: new MO created to be deleted");
				delete pBcastPriorityLocalManagedObject;
			} else {
				trace (TRACE_LEVEL_INFO, string("FspfServiceLocalObjectManager::install: new MO not created"));
			}

			if (FRAMEWORK_SUCCESS == status) {
				trace(TRACE_LEVEL_DEBUG,
				      "FspfServiceLocalObjMgr::install: MO create: completed");
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				trace(TRACE_LEVEL_ERROR,
				      "FspfServiceLocalObjMgr::install: MO create: Failed");
				status = WAVE_MESSAGE_ERROR;
			}
		}

		if (WAVE_MESSAGE_SUCCESS == status) {
			trace (TRACE_LEVEL_INFO, "FspfServiceLocalObjectManager::install - Completed Creating Entries");
		} else {
			trace (TRACE_LEVEL_ERROR, "FspfServiceLocalObjectManager::install - WAVE_MESSAGE_ERROR returned");
		}
		pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
		pWaveAsynchronousContextForBootPhases->callback ();
	}

	/* Update BcastPrioirty */
	ResourceId FspfServiceLocalObjectManager::updateBcastPriority (PrismSynchronousLinearSequencerContext *pFspfServiceUpdateBcastPriorityInfoMessageContext)
	{
		FspfServiceUpdateBcastPriorityInfoMessage *pFspfServiceUpdateBcastPriorityInfoMsg = reinterpret_cast<FspfServiceUpdateBcastPriorityInfoMessage *> (pFspfServiceUpdateBcastPriorityInfoMessageContext->getPPrismMessage ());

		trace (TRACE_LEVEL_INFO, string ("FspfServiceLocalObjMgr::updateBcastPriority: switch id") + pFspfServiceUpdateBcastPriorityInfoMsg->getSwitchId() + string ("priority") + pFspfServiceUpdateBcastPriorityInfoMsg->getBcastPriority());


		BcastPriorityLocalManagedObject *pBcastPriorityLocalManagedObject =
		                                 QueryBcastLocalManagedObject(pFspfServiceUpdateBcastPriorityInfoMsg->getSwitchId());


		if (pBcastPriorityLocalManagedObject) {
			trace (TRACE_LEVEL_DEBUG, string ("FspfServiceLocalObjMgr::updateBcastPriority: within UPDT BcastPriority MO_exists"));

			pBcastPriorityLocalManagedObject->setSwitchId(pFspfServiceUpdateBcastPriorityInfoMsg->getSwitchId());
			pBcastPriorityLocalManagedObject->setBcastPriority(pFspfServiceUpdateBcastPriorityInfoMsg->getBcastPriority());

			updateWaveManagedObject (pBcastPriorityLocalManagedObject);

			int ret = setMcastPriority(pBcastPriorityLocalManagedObject->getBcastPriority());
			trace (TRACE_LEVEL_DEBUG, string("FspfServiceLocalObjMgr::updateBcastPriority: ret value = ") + ret);

			if (ret == -1) {
				trace (TRACE_LEVEL_ERROR, string("FspfServiceLocalObjMgr::updateBcastPriority: Invalid bcast Priority value"));
				// GC call to clear the MO
				pFspfServiceUpdateBcastPriorityInfoMessageContext->addManagedObjectForGarbageCollection(pBcastPriorityLocalManagedObject);
				return (WAVE_MESSAGE_SUCCESS);
			}
			trace(TRACE_LEVEL_INFO, string("FspfServiceLocalObjectManager::updateBcastPriority: UPDT MO done "));
			// GC call to clear the MO
			pFspfServiceUpdateBcastPriorityInfoMessageContext->addManagedObjectForGarbageCollection(pBcastPriorityLocalManagedObject);
		}

		return WAVE_MESSAGE_SUCCESS;
	}

	void FspfServiceLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
		int status = WAVE_MESSAGE_SUCCESS;

		trace (TRACE_LEVEL_INFO, "FspfServiceLocalObjectManager::boot: Entering Boot");

		pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
		pWaveAsynchronousContextForBootPhases->callback ();

		trace (TRACE_LEVEL_INFO, "FspfServiceLocalObjectManager::boot: Exiting Boot");
	}

	/*
	** This function is the FSS stage handler for FSPFd.
	** Called from: FSPFd FSS handler each time FSPFd cold starts.
	** Inputs:      Stage number (7. FSPF_CONFIG_STAGE) and service Type (FCSW)
	** Functions:   Reads MOs from database and populates the backend to make sure sync.
	*/

	void  FspfServiceLocalObjectManager::externalStateSynchronization (
	      WaveAsynchronousContextForExternalStateSynchronization
	      *pWaveAsynchronousContextForExternalStateSynchronization)
	{
		int ret = 0, status = WAVE_MESSAGE_SUCCESS;
		int localId;

		trace (TRACE_LEVEL_DEBUG,
		       string("FspfServiceLocalObjectManager::externalStateSynchronization: Entering External State Sync"));

		if (((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 7) ||
			 (pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 11))
		     && (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) ) {

			trace (TRACE_LEVEL_DEBUG, string("FspfServiceLocalObjectManager::externalStateSynchronization: FSS staging 7"));

			localId = DcmToolKit::getLocalMappedId();
			trace (TRACE_LEVEL_DEBUG, string("FspfServiceLocalObjectManager::externalStateSynchronization: localId ") + localId);
			BcastPriorityLocalManagedObject *pBcastPriorityLocalManagedObject =
			                                 QueryBcastLocalManagedObject(localId);

			if (pBcastPriorityLocalManagedObject) {
				trace (TRACE_LEVEL_DEBUG, string("FspfServiceLocalObjectManager::externalStateSynchronization: MO exists"));
				trace (TRACE_LEVEL_INFO,
				       string("FspfServiceLocalObjectManager::externalStateSynchronization: switch id") +
				       pBcastPriorityLocalManagedObject->getSwitchId() +
				       string("priority") +
				       pBcastPriorityLocalManagedObject->getBcastPriority());
				trace (TRACE_LEVEL_DEBUG,
				       string("FspfServiceLocalObjectManager::externalStateSynchronization: calling FOS API"));

				ret = setMcastPriority(pBcastPriorityLocalManagedObject->getBcastPriority());
				trace (TRACE_LEVEL_DEBUG,
				       string("FspfServiceLocalObjectManager::externalStateSynchronization: FOS API call done"));

				if (ret == -1) {
					trace (TRACE_LEVEL_ERROR,
					       "FspfServiceLocalObjectManager::externalStateSynchronization: Invalid bcast Priority value");
					status = WAVE_MESSAGE_SUCCESS;
				}

				if (pBcastPriorityLocalManagedObject) {
					trace (TRACE_LEVEL_DEBUG,
					       "FspfServiceLocalObjectManager::externalStateSynchronization: MO already exists delete MO");
					delete pBcastPriorityLocalManagedObject;
				}
			} else {
				trace (TRACE_LEVEL_INFO, string("FspfServiceLocalObjectManager::externalStateSynchronization: MO is NULL"));
				status = WAVE_MESSAGE_ERROR;
			}
		} else {
			trace (TRACE_LEVEL_DEBUG,
			       string("FspfServiceLocalObjectManager::externalStateSynchronization: Not in FSS stage 7"));
			status = WAVE_MESSAGE_SUCCESS;
		}

		trace (TRACE_LEVEL_DEBUG, string("FspfServiceLocalObjectManager::externalStateSynchronization: Staging completed"));

		pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (status);
		pWaveAsynchronousContextForExternalStateSynchronization->callback ();

		trace (TRACE_LEVEL_DEBUG,
		       string("FspfServiceLocalObjectManager::externalStateSynchronization: Exiting External State Sync"));
	}

	/* Update Bcast Priority Message Handler code */
    void  FspfServiceLocalObjectManager::FspfServiceUpdateBcastPriorityInfoMessageHandler( FspfServiceUpdateBcastPriorityInfoMessage *pFspfServiceUpdateBcastPriorityInfoMessage)
     {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::updateBcastPriority),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FspfServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFspfServiceUpdateBcastPriorityInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
}
