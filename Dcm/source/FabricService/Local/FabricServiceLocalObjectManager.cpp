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
 *   Author : mgarg                                                        *
 **************************************************************************/

#include <fstream>
#include <iostream>
using namespace std;

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"

#include "FabricService/Local/FabricServiceLocalObjectManager.h"
#include "FabricService/Local/FabricServiceGetFabricInfoMessage.h"
#include "FabricService/Local/FabricServiceGetTrunkShowMessage.h"
#include "FabricService/Local/FabricServiceTypes.h"
#include "FabricService/Local/FabricServiceGetislshowMessage.h"
#include "FabricService/Local/FabricServiceSetLocalDomainIdMessage.h"
#include "FabricService/Local/FabricServiceSwitchShowMessage.h"
#include "FabricService/Local/FabricServicePortLogClearMessage.h"
#include "FabricService/Local/FabricServicePortLogMessage.h"
#include "FabricService/Local/FabricServiceSetInsistentDomainIdMessage.h"
#include "FabricService/Local/FabricServiceDisableIslsToNeighborMessage.h"
#include "FabricService/Local/FabricServiceGetlinkInfoMessage.h"

#include "PortCfg/Local/PortCfgPortCfgPortStateUpdateMessage.h"

#include "DcmCore/DcmToolKit.h"

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

    FabricServiceLocalObjectManager::FabricServiceLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (FABRICSERVICEGETFABRICINFO, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServiceGetFabricInfoMessageHandler));
		addOperationMap (FABRICSERVICEGETISLSHOW, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServiceGetislshowMessageHandler));
        addOperationMap (FABRICSERVICEGETTRUNKSHOW, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServiceGetTrunkShowMessageHandler));
		addOperationMap (FABRICSERVICESETLOCALDOMAINID, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServiceSetLocalDomainIdMessageHandler));
		addOperationMap (FABRICSERVICESWITCHSHOW, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServiceSwitchShowMessageHandler));
        addOperationMap (FABRICSERVICEPORTLOGCLEAR, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServicePortLogClearMessageHandler));
        addOperationMap (FABRICSERVICEPORTLOG, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServicePortLogMessageHandler));
        addOperationMap (FABRICSERVICESETINSISTENTDOMAINID, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServiceSetInsistentDomainIdMessageHandler));
		addOperationMap (FABRICSERVICEDISABLEISLSTONEIGHBOR, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServiceDisableIslsToNeighborMessageHandler));
		addOperationMap (FABRICSERVICEGETLINKINFO, reinterpret_cast<PrismMessageHandler> (&FabricServiceLocalObjectManager::FabricServiceGetlinkInfoMessageHandler));
    }

    FabricServiceLocalObjectManager::~FabricServiceLocalObjectManager ()
    {
    }

    FabricServiceLocalObjectManager  *FabricServiceLocalObjectManager::getInstance()
    {
        static FabricServiceLocalObjectManager *pFabricServiceLocalObjectManager = new FabricServiceLocalObjectManager ();

        WaveNs::prismAssert (NULL != pFabricServiceLocalObjectManager, __FILE__, __LINE__);

        return (pFabricServiceLocalObjectManager);
    }

    string  FabricServiceLocalObjectManager::getClassName()
    {
        return ("FabricService");
    }

    PrismServiceId  FabricServiceLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FabricServiceLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

		case FABRICSERVICEGETFABRICINFO :
			pPrismMessage = new FabricServiceGetFabricInfoMessage ();
			break;
		case FABRICSERVICEGETISLSHOW :
			pPrismMessage = new FabricServiceGetislshowMessage ();
			break;
		case FABRICSERVICEGETTRUNKSHOW :
			pPrismMessage = new FabricServiceGetTrunkShowMessage ();
			break;
		case FABRICSERVICESETLOCALDOMAINID :
			pPrismMessage = new FabricServiceSetLocalDomainIdMessage ();
			break;
		case FABRICSERVICESWITCHSHOW :
			pPrismMessage = new FabricServiceSwitchShowMessage ();
			break;
		case FABRICSERVICEPORTLOGCLEAR :
			pPrismMessage = new FabricServicePortLogClearMessage ();
			break;
		case FABRICSERVICEPORTLOG :
			pPrismMessage = new FabricServicePortLogMessage ();
			break;
		case FABRICSERVICESETINSISTENTDOMAINID :
			pPrismMessage = new FabricServiceSetInsistentDomainIdMessage ();
			break;
		case FABRICSERVICEDISABLEISLSTONEIGHBOR :
			pPrismMessage = new FabricServiceDisableIslsToNeighborMessage ();
			break;
		case FABRICSERVICEGETLINKINFO :
			pPrismMessage = new FabricServiceGetlinkInfoMessage ();
			break;
		default :
			pPrismMessage = NULL;
	}

        return (pPrismMessage);
    }

    WaveManagedObject  *FabricServiceLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

/*****************************************************
 To allow certain operations BEFORE Plugin Service
 gets Enabled during cluster formation

 Note:
  Please be judicious in allowing such operations.
  These operations should access backend daemons ONLY.
  NO DCM DATABASE related operations should EVER BE
  allowed BEFORE the Plugin Service gets enabled!!!
  This is critical towards maintaining Db integrity.

******************************************************/
    bool FabricServiceLocalObjectManager::isOperationAllowedBeforeEnabling (const UI32 &operationCode)
    {
	switch (operationCode) {
		case FABRICSERVICEGETFABRICINFO:	/* Get Fabric info */
			return (true);
		default:
			return (WaveObjectManager::isOperationAllowedBeforeEnabling (operationCode));
	}
     }

/***********************************************
 GetFabricInfoMessage Handler's & API's
	(a.k.a fabricShow or "show fabric")
***********************************************/
    ResourceId FabricServiceLocalObjectManager:: getFabricInfo(PrismSynchronousLinearSequencerContext *pFabricServiceGetFabricInfoMessageContext)
    {
	FabricServiceGetFabricInfoMessage *pFabricServiceGetFabricInfoMessage =
		reinterpret_cast<FabricServiceGetFabricInfoMessage *> (pFabricServiceGetFabricInfoMessageContext->getPPrismMessage());

	trace (TRACE_LEVEL_DEBUG, "Fabric Plugin: GetFabricInfoMessage - entering...");

	unsigned int domain_count = 0, token_count = 0, i;
	int rval = 0;
	fab_domain_t	*domain_list = NULL;

	char *fw_id;
	char fw_version[10];

	if ((rval = fabGetCnt(&domain_count, &token_count)) != 0) {
		trace (TRACE_LEVEL_ERROR, string("Fabric Plugin:GetFabricInfoMessage - fabGetCnt() failed!! rval=") + rval);
		pFabricServiceGetFabricInfoMessage->setDomain_count(domain_count);
		trace (TRACE_LEVEL_DEBUG, "Fabric Plugin: GetFabricInfoMessage - exiting...");
		return (WAVE_MESSAGE_SUCCESS);
	}
	trace (TRACE_LEVEL_INFO, string("Fabric Plugin: getFabricInfo - domain_count=") + domain_count);

	if (domain_count > 0) {
		domain_list = (fab_domain_t *) malloc(sizeof(fab_domain_t) * domain_count);
		memset(domain_list, 0, (sizeof(fab_domain_t) * domain_count));

		rval = fabricShow(domain_list, NULL, domain_count, 0);

		if (rval == -1) {
			trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:GetFabricInfoMessage - fabricShow() failed!! rval=") + rval);
			domain_count = 0;
			FREE(domain_list);
			pFabricServiceGetFabricInfoMessage->setDomain_count(domain_count);
			trace (TRACE_LEVEL_DEBUG, "Fabric Plugin: GetFabricInfoMessage - exiting...");
			return (WAVE_MESSAGE_SUCCESS);
		}

		vector<UI32>  domain_id;
		vector<WorldWideName>  domain_wwn;
		vector<IpV4Address>  domain_eth_ip_addr;
		vector<IpV4Address>  domain_fc_ip_addr;
		vector<IpV6Address>  domain_ipv6_addr;
		vector<string>  domain_name;
		vector<UI32>  domain_hw_model;
		vector<string>  domain_fw_version;
		vector<bool>  principal_domain;

		for (i = 0; i < domain_count; i++) {
			domain_id.push_back(domain_list[i].dom_id);
			domain_wwn.push_back(string((char *)&domain_list[i].dom_wwn));
			domain_eth_ip_addr.push_back(string((char *)&domain_list[i].dom_ipaddr));
			domain_fc_ip_addr.push_back(string((char *)&domain_list[i].dom_ether));
			domain_ipv6_addr.push_back(string((char *)&domain_list[i].dom_ipaddrV6));
			domain_name.push_back(domain_list[i].dom_name);
			domain_hw_model.push_back(domain_list[i].sw_model >> 8);

			if (domain_list[i].sw_fw_id) {
				fw_id = (char *)&domain_list[i].sw_fw_id;
				snprintf(fw_version, sizeof (fw_version), "%c%c.%c.%c", fw_id[0], fw_id[1], fw_id[2], fw_id[3]);
			} else {
				snprintf(fw_version, sizeof (fw_version), "none");
			}
			trace (TRACE_LEVEL_DEBUG, string("Fabric Plugin: getFabricInfo - fw_version=") + fw_version);
			domain_fw_version.push_back(string(fw_version));

			if (domain_list[i].dom_sep == '>') {
				principal_domain.push_back(true);
			} else {
				principal_domain.push_back(false);
			}
		}

		pFabricServiceGetFabricInfoMessage->setDomain_id(domain_id);
		pFabricServiceGetFabricInfoMessage->setDomain_wwn(domain_wwn);
		pFabricServiceGetFabricInfoMessage->setDomain_eth_ip_addr(domain_eth_ip_addr);
		pFabricServiceGetFabricInfoMessage->setDomain_fc_ip_addr(domain_fc_ip_addr);
		pFabricServiceGetFabricInfoMessage->setDomain_ipv6_addr(domain_ipv6_addr);
		pFabricServiceGetFabricInfoMessage->setDomain_name(domain_name);
		pFabricServiceGetFabricInfoMessage->setDomain_hw_model(domain_hw_model);
		pFabricServiceGetFabricInfoMessage->setDomain_fw_version(domain_fw_version);
		pFabricServiceGetFabricInfoMessage->setPrincipal_domain(principal_domain);

		FREE(domain_list);
	}

	pFabricServiceGetFabricInfoMessage->setDomain_count(domain_count);
	trace (TRACE_LEVEL_DEBUG, "Fabric Plugin: GetFabricInfoMessage - exiting...");
	return (WAVE_MESSAGE_SUCCESS);
    }

	ResourceId  FabricServiceLocalObjectManager::TrunkShowGetEntries (PrismSynchronousLinearSequencerContext *pFabricServiceGetTrunkShowMessageContext)
	{
		FabricServiceGetTrunkShowMessage *pFabricServiceGetTrunkShowMessage = reinterpret_cast<FabricServiceGetTrunkShowMessage *> (pFabricServiceGetTrunkShowMessageContext->getPPrismMessage ());

    vector<UI32> group_num;
    vector<SI32> src_blade;
    vector<SI32> src_port;
    vector<UI32> src_index;
    vector<SI32> src_disp_type;
    vector<SI32> dest_blade;
    vector<SI32> dest_port;
    vector<UI32> dest_index;
    vector<SI32> dest_disp_type;
    vector<SI32> interface_type;
    vector<WorldWideName> switch_wwn;
    vector<UI32> deskew;
    vector<UI32> domain_id;
    vector<UI32> type;
    vector<bool> port_type;
    vector<UI64> tx_bandwidth;
    vector<UI64> tx_throughput;
    vector<UI64> tx_percent;
    vector<UI64> rx_bandwidth;
    vector<UI64> rx_throughput;
    vector<UI64> rx_percent;
    vector<SI32> local_qsfpidx;
    vector<SI32> remote_qsfpidx;

    int i, max;
	int dest_base_index = INT_MIN;
	int tupleDomain = -1;
	int srcTupleBlade = -1, srcTuplePort = -1;
	int destTupleBlade = -1, destTuplePort = -1;
	int destPrimaryTupleBlade = -1, destPrimaryTuplePort = -1, qsfp_idx = -1;
	interface_display_type_t srcDispType = eTYPE_INVALID, destDispType = eTYPE_INVALID, destPrimaryDispType = eTYPE_INVALID;
	interface_type_t interfaceType = eINT_INVALID;
    trunk_entry_t  *trunkshow_entry_list = NULL;

    max = getFabricTrunkInfo(&trunkshow_entry_list);
    trace (TRACE_LEVEL_INFO, string("Fabric Plugin: TrunkShowGetEntries - no. of entries=") + max);

    if (max < 0) {
        trace (TRACE_LEVEL_ERROR, string("Fabric Plugin: TrunkShowGetEntries - getFabricTrunkInfo failed!! no. of entries=") + max);
        FREE(trunkshow_entry_list);
        return (WAVE_MESSAGE_SUCCESS);
    }

    if ((max > 0) && (trunkshow_entry_list != NULL)) {
        for (i = 0; i < max; i++)
        {
			/* Reset variables */
			srcTuplePort = -1;
			srcTupleBlade = -1;
			srcDispType = eTYPE_INVALID;
			destTuplePort = -1;
			destTupleBlade = -1;
			destDispType = eTYPE_INVALID;
			interfaceType = eINT_INVALID;

			/* Local/src port */
			getPhyPortTuple(trunkshow_entry_list[i].src_port_num, &tupleDomain, &srcTupleBlade, &srcTuplePort);
			srcDispType = getInterfaceDisplayType(trunkshow_entry_list[i].src_port_num);
			interfaceType = getInterfaceType(trunkshow_entry_list[i].src_port_num);

			/* Neighbor/dest port */
			getPortTupleRemote(trunkshow_entry_list[i].src_port_num, &tupleDomain, &destTupleBlade, &destTuplePort, &qsfp_idx);
			destDispType = getInterfaceDisplayTypeRemote(trunkshow_entry_list[i].src_port_num);

			if (trunkshow_entry_list[i].port_type == 1) {
				/* Primary trunk member - should always be first of the trunk group */
				dest_base_index = trunkshow_entry_list[i].dest_port_num;

				/* If the remote display type is unknown - default to same as local dispaly type */
				if (destDispType == eTYPE_INVALID || destDispType == (interface_display_type_t)-1) {
					destDispType = srcDispType;
				}

				destPrimaryTuplePort = destTuplePort;
				destPrimaryTupleBlade = destTupleBlade;
				destPrimaryDispType = destDispType;

			} else {
				/* Secondary trunk member - should come after the primary of the trunk group */

				/*
				 * Defect 460645:
				 * Set slave interface info relative to primary interface
				 */
				if (destPrimaryTuplePort != -1) {
					destTuplePort = destPrimaryTuplePort;
				} else {
					destTuplePort = destPrimaryTuplePort + (trunkshow_entry_list[i].dest_port_num - dest_base_index);
				}
				destTupleBlade = destPrimaryTupleBlade;
				destDispType = destPrimaryDispType;
			}

			group_num.push_back(trunkshow_entry_list[i].group_num);
			src_index.push_back(trunkshow_entry_list[i].src_port_num);
                        local_qsfpidx.push_back(getQSFPTupleIndex(trunkshow_entry_list[i].src_port_num));
			src_blade.push_back(srcTupleBlade);
			src_port.push_back(srcTuplePort);
			src_disp_type.push_back((SI32)srcDispType);
			dest_index.push_back(trunkshow_entry_list[i].dest_port_num);
			dest_blade.push_back(destTupleBlade);
			dest_port.push_back(destTuplePort);
			dest_disp_type.push_back((SI32)destDispType);
			interface_type.push_back((SI32)interfaceType);
			switch_wwn.push_back(string((char *)(&trunkshow_entry_list[i].switch_wwn)));
			deskew.push_back(trunkshow_entry_list[i].deskew);
			domain_id.push_back(trunkshow_entry_list[i].domain_id);
			type.push_back(trunkshow_entry_list[i].type);
			port_type.push_back(trunkshow_entry_list[i].port_type);
			tx_bandwidth.push_back(trunkshow_entry_list[i].tx_bandwidth);
			tx_throughput.push_back(trunkshow_entry_list[i].tx_throughput);
			tx_percent.push_back(trunkshow_entry_list[i].tx_percent);
			rx_bandwidth.push_back(trunkshow_entry_list[i].rx_bandwidth);
			rx_throughput.push_back(trunkshow_entry_list[i].rx_throughput);
			rx_percent.push_back(trunkshow_entry_list[i].rx_percent);
                        remote_qsfpidx.push_back(qsfp_idx);

        }
    }

    pFabricServiceGetTrunkShowMessage->setGroup_num(group_num);
    pFabricServiceGetTrunkShowMessage->setSrc_blade(src_blade);
    pFabricServiceGetTrunkShowMessage->setSrc_port(src_port);
    pFabricServiceGetTrunkShowMessage->setSrc_index(src_index);
    pFabricServiceGetTrunkShowMessage->setSrc_disp_type(src_disp_type);
    pFabricServiceGetTrunkShowMessage->setDest_blade(dest_blade);
    pFabricServiceGetTrunkShowMessage->setDest_port(dest_port);
    pFabricServiceGetTrunkShowMessage->setDest_index(dest_index);
    pFabricServiceGetTrunkShowMessage->setDest_disp_type(dest_disp_type);
    pFabricServiceGetTrunkShowMessage->setInterface_type(interface_type);
    pFabricServiceGetTrunkShowMessage->setSwitch_wwn(switch_wwn);
    pFabricServiceGetTrunkShowMessage->setDeskew(deskew);
    pFabricServiceGetTrunkShowMessage->setDomain_id(domain_id);
    pFabricServiceGetTrunkShowMessage->setType(type);
    pFabricServiceGetTrunkShowMessage->setPort_type(port_type);
    pFabricServiceGetTrunkShowMessage->setTx_bandwidth(tx_bandwidth);
    pFabricServiceGetTrunkShowMessage->setTx_throughput(tx_throughput);
    pFabricServiceGetTrunkShowMessage->setTx_percent(tx_percent);
    pFabricServiceGetTrunkShowMessage->setRx_bandwidth(rx_bandwidth);
    pFabricServiceGetTrunkShowMessage->setRx_throughput(rx_throughput);
    pFabricServiceGetTrunkShowMessage->setRx_percent(rx_percent);
    pFabricServiceGetTrunkShowMessage->setLocal_qsfpidx(local_qsfpidx);
    pFabricServiceGetTrunkShowMessage->setRemote_qsfpidx(remote_qsfpidx);

    trace (TRACE_LEVEL_INFO, "Fabric Plugin: TrunkShowGetEntries - exiting...");
    FREE(trunkshow_entry_list);
    return (WAVE_MESSAGE_SUCCESS);
}

    void  FabricServiceLocalObjectManager::FabricServiceGetFabricInfoMessageHandler( FabricServiceGetFabricInfoMessage *pFabricServiceGetFabricInfoMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::getFabricInfo),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServiceGetFabricInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


    ResourceId FabricServiceLocalObjectManager::show_fabric_isl (PrismSynchronousLinearSequencerContext *pFabricServiceGetislshowMessageContext)
	{
		FabricServiceGetislshowMessage *pFabricServiceGetislshowMessage =
			reinterpret_cast<FabricServiceGetislshowMessage*> (pFabricServiceGetislshowMessageContext->getPPrismMessage ());
		trace (TRACE_LEVEL_DEBUG, "Fabric Plugin: GetFabricislshowMessage - entering...");
		int i, rval;
		int tupleDomain = 0, tupleBlade = 0, tuplePort = 0, qsfp_idx = -1;
		interface_display_type_t locDispType, remDispType;

		vector<UI32> serialport;
		vector<SI32> src_blade;
		vector<SI32> src_port;
		vector<UI32> src_index;
		vector<SI32> src_disp_type;
		vector<SI32> dest_blade;
		vector<SI32> dest_port;
		vector<UI32> dest_index;
		vector<SI32> dest_disp_type;
		vector<SI32> interface_type;
		vector<WorldWideName> dom_wwn;
		vector<UI32> dom_id;
		vector<string> switch_name;
		vector<string> type;
		vector<UI32> speed;
		vector<UI32> bandwidth;
                vector<SI32> local_qsfpidx;
                vector<SI32> remote_qsfpidx;

		isl_port_info_t *isl_entry_list = NULL;
		trace (TRACE_LEVEL_INFO, string ("within isl plugin. domin id ") +
				pFabricServiceGetislshowMessage->getDomain_id());

		rval = getFabricIslInfo(&isl_entry_list);
		trace (TRACE_LEVEL_DEBUG, string("Fabric Plugin: GetFabricislshowMessage - #entries=") + rval);

		if (rval == -1) {
			trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:GetFabricislshowMessage - islShow() failed!! rval=") + rval);
			FREE(isl_entry_list);
			return (WAVE_MESSAGE_SUCCESS);
		}

		if ((rval > 0) && (isl_entry_list != NULL)) {
			for (i = 0; i < rval; i++) {
				if (strlen(isl_entry_list[i].domain_wwn) == 0) {
					/* WWN is NULL for some reason. Skip it to avoid ASSERT */
					trace (TRACE_LEVEL_ERROR, string("Fabric Plugin: GetFabricislshowMessage -"
						" Invalid WWN, skip it. i=") + 	i +
						string(" src_port=") + isl_entry_list[i].src_port);
					continue;
				}

				/* Get local port tuple info to send back */
				getPhyPortTuple(isl_entry_list[i].src_port, &tupleDomain, &tupleBlade, &tuplePort);

				/* Populate Local port details */
				src_blade.push_back(tupleBlade);
				src_port.push_back(tuplePort);
				src_index.push_back(isl_entry_list[i].src_port);
				locDispType = getInterfaceDisplayType(isl_entry_list[i].src_port);
				src_disp_type.push_back((SI32)locDispType);
                                local_qsfpidx.push_back(getQSFPTupleIndex(isl_entry_list[i].src_port));

				/* Get nbr port tuple info to send back */
				getPortTupleRemote(isl_entry_list[i].src_port, &tupleDomain, &tupleBlade, &tuplePort, &qsfp_idx);

				/* Populate Neighbor port detail */
				dest_blade.push_back(tupleBlade);
				dest_port.push_back(tuplePort);
				dest_index.push_back(isl_entry_list[i].dest_port);
                                remote_qsfpidx.push_back(qsfp_idx);

				/* Get/determin the remote display type */
				remDispType = getInterfaceDisplayTypeRemote(isl_entry_list[i].src_port);
				if (remDispType == (interface_display_type_t)-1 || remDispType == eTYPE_INVALID) {
					/* Invalid/Unknown/Undefined interface type -> Assume same as local interface display type */
					dest_disp_type.push_back((SI32)locDispType);
				} else {
					/* Save the actual value */
					dest_disp_type.push_back((SI32)remDispType);
				}

				/* Populate the type of interface (media type) */
				interface_type.push_back((SI32)getInterfaceType(isl_entry_list[i].src_port));

				/* Populate additional fields */
				serialport.push_back(isl_entry_list[i].serialport);
				dom_wwn.push_back(string((char *)&isl_entry_list[i].domain_wwn));
				dom_id.push_back(isl_entry_list[i].domainid);
				switch_name.push_back(isl_entry_list[i].switch_name);
				type.push_back(isl_entry_list[i].type);
				speed.push_back(isl_entry_list[i].speed);
				bandwidth.push_back(isl_entry_list[i].bandwidth);
			}
		}

		pFabricServiceGetislshowMessage->setSerialport(serialport);
		pFabricServiceGetislshowMessage->setSrc_blade(src_blade);
		pFabricServiceGetislshowMessage->setSrc_port(src_port);
		pFabricServiceGetislshowMessage->setSrc_index(src_index);
		pFabricServiceGetislshowMessage->setSrc_disp_type(src_disp_type);
		pFabricServiceGetislshowMessage->setDest_blade(dest_blade);
		pFabricServiceGetislshowMessage->setDest_port(dest_port);
		pFabricServiceGetislshowMessage->setDest_index(dest_index);
		pFabricServiceGetislshowMessage->setDest_disp_type(dest_disp_type);
		pFabricServiceGetislshowMessage->setInterface_type(interface_type);
		pFabricServiceGetislshowMessage->setDom_wwn(dom_wwn);
		pFabricServiceGetislshowMessage->setDom_id(dom_id);
		pFabricServiceGetislshowMessage->setSwitch_name(switch_name);
		pFabricServiceGetislshowMessage->setType(type);
		pFabricServiceGetislshowMessage->setSpeed(speed);
		pFabricServiceGetislshowMessage->setBandwidth(bandwidth);
                pFabricServiceGetislshowMessage->setLocal_qsfpidx(local_qsfpidx);
                pFabricServiceGetislshowMessage->setRemote_qsfpidx(remote_qsfpidx);

		FREE(isl_entry_list);
		return (WAVE_MESSAGE_SUCCESS);
	}

    void  FabricServiceLocalObjectManager::FabricServiceGetislshowMessageHandler( FabricServiceGetislshowMessage *pFabricServiceGetislshowMessage)
    {
	    // Validations and Queries should go here

	    PrismSynchronousLinearSequencerStep sequencerSteps[] =
	    {
		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
		    // Your configuration change code goes here

		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
		    // Programming Protocol Daemons goes here
		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::show_fabric_isl),
		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
	    };

	    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServiceGetislshowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	    pPrismSynchronousLinearSequencerContext->execute ();
    }

void  FabricServiceLocalObjectManager::FabricServiceGetTrunkShowMessageHandler( FabricServiceGetTrunkShowMessage *pFabricServiceGetTrunkShowMessage)
{
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::TrunkShowGetEntries),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServiceGetTrunkShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
}

ResourceId FabricServiceLocalObjectManager::SetLocalDomainId (PrismSynchronousLinearSequencerContext *pFabricServiceSetLocalDomainIdMessageContext)
{
		FabricServiceSetLocalDomainIdMessage *pFabricServiceSetLocalDomainIdMessage =
			reinterpret_cast<FabricServiceSetLocalDomainIdMessage*> (pFabricServiceSetLocalDomainIdMessageContext->getPPrismMessage ());
		trace (TRACE_LEVEL_DEBUG, "Fabric Plugin: SetLocalDomainId - entering...");
		int rval;
		SI32 domain_id = pFabricServiceSetLocalDomainIdMessage->getDomain_id();

		trace (TRACE_LEVEL_INFO, string ("Fabric Plugin: SetLocalDomainId plugin. domin id ") +
				pFabricServiceSetLocalDomainIdMessage->getDomain_id());

		rval = fabSetLocalDomainId(domain_id);
		domain_id = (SI32) rval;
		if (rval == 0) {
			trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:SetLocalDomainId - fail -- SwOnline!! rval=") + rval);
		} else if (rval < 0) {
			trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:SetLocalDomainId - fail!! rval=") + rval);
		}

		pFabricServiceSetLocalDomainIdMessage->setDomain_id(domain_id);

		return (WAVE_MESSAGE_SUCCESS);
}

void  FabricServiceLocalObjectManager::FabricServiceSetLocalDomainIdMessageHandler( FabricServiceSetLocalDomainIdMessage *pFabricServiceSetLocalDomainIdMessage)
{
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::SetLocalDomainId),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServiceSetLocalDomainIdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
}

ResourceId FabricServiceLocalObjectManager::switch_show_isl(PrismSynchronousLinearSequencerContext *pFabricServiceSwitchShowMessageContext)
{
		FabricServiceSwitchShowMessage *pFabricServiceSwitchShowMessage =
			reinterpret_cast<FabricServiceSwitchShowMessage *> (pFabricServiceSwitchShowMessageContext->getPPrismMessage ());
		trace (TRACE_LEVEL_INFO, "Fabric Plugin: Switch Show- entering...");
		int rval;
		char switchShowPath[256];
		char tempLine[256];
		vector<string>  switch_details;

		snprintf(switchShowPath, 255, "/tmp/switchInfo_tmp_%d", getpid());
		/*
		 * UN-comment the line below to test this in simulation env.
		 * If not uncommented it will just throw an error.
		 */
		// snprintf(switchShowPath, 255, "/vobs/projects/springboard/fabos/src/vcs/dcm/application/switchshow_%d.txt", getpid());

		trace (TRACE_LEVEL_DEBUG, string ("within Switch Show plugin:") + sizeof(switchShowPath));

		rval = fabGetSwitchShow(switchShowPath);
		if (rval != 0) {
			trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:Switchshow - fail!! rval=") + rval);

			snprintf(tempLine, 255, "Error!Unable to fetch the details");
			switch_details.push_back(tempLine);

			pFabricServiceSwitchShowMessage->setSwitch_details(switch_details);
		} else {
			trace (TRACE_LEVEL_INFO,string("Fabric Plugin:SwitchShow - SUCC!! ") +
						string("FileName:") + switchShowPath);

			ifstream in;

			in.open(switchShowPath);
			if (!in) {
				trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:Switchshow - Unable to Open file!!"));

				snprintf(tempLine, 255, "Error!Unable to fetch the details");
				switch_details.push_back(tempLine);

				pFabricServiceSwitchShowMessage->setSwitch_details(switch_details);
			} else {
				trace (TRACE_LEVEL_DEBUG, string ("switchShow\n"));
				UI32 mapped_id = 0;
				mapped_id = DcmToolKit::getLocalMappedId();

				while (!in.eof()) {
					in.getline(tempLine, 256);
					if (!in.eof() && in.fail()) {
						trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:Switchshow - Max string size exceeded!!"));

						return(WAVE_MESSAGE_ERROR);
					}
					if (strstr(tempLine, "switchId:") != NULL) {
						snprintf(tempLine, 255, "%s\t%d", "switchId:", mapped_id);
						switch_details.push_back(tempLine);
					} else {
						switch_details.push_back(tempLine);
					}

					trace (TRACE_LEVEL_DEBUG, tempLine);
				}
				in.close();

				pFabricServiceSwitchShowMessage->setSwitch_details(switch_details);
			}
			unlink(switchShowPath);
		}

		return (WAVE_MESSAGE_SUCCESS);
}

void  FabricServiceLocalObjectManager::FabricServiceSwitchShowMessageHandler( FabricServiceSwitchShowMessage *pFabricServiceSwitchShowMessage)
{
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::switch_show_isl),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServiceSwitchShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
}

ResourceId FabricServiceLocalObjectManager::fabric_port_log_clear(PrismSynchronousLinearSequencerContext *pFabricServicePortLogClearMessageContext)
{
		FabricServicePortLogClearMessage *pFabricServicePortLogClearMessage =
			reinterpret_cast<FabricServicePortLogClearMessage *> (pFabricServicePortLogClearMessageContext->getPPrismMessage ());
		trace (TRACE_LEVEL_INFO, "Fabric Plugin: Port_log_clear- entering...");
		int rval;
		string port_log = "";

		trace (TRACE_LEVEL_DEBUG, string ("within portLog Clear plugin:"));

		rval = fabPortLogClear();
		if (rval != 0) {
			trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:PortLogClear - fail!! rval=") + rval);

			port_log = "Error!Unable to Clear PortLog\n";

			pFabricServicePortLogClearMessage->setStatus(port_log);
		} else {
			trace (TRACE_LEVEL_DEBUG,string("Fabric Plugin:PortLogClear - SUCC!! "));

			port_log = "PortLog Cleared!\n";

			pFabricServicePortLogClearMessage->setStatus(port_log);
		}

		return (WAVE_MESSAGE_SUCCESS);
}

void  FabricServiceLocalObjectManager::FabricServicePortLogClearMessageHandler( FabricServicePortLogClearMessage *pFabricServicePortLogClearMessage)
{
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::fabric_port_log_clear),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServicePortLogClearMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
}

ResourceId FabricServiceLocalObjectManager::fabric_port_log_show(PrismSynchronousLinearSequencerContext *pFabricServicePortLogMessageContext)
{
		FabricServicePortLogMessage *pFabricServicePortLogMessage =
			reinterpret_cast<FabricServicePortLogMessage *> (pFabricServicePortLogMessageContext->getPPrismMessage ());
		trace (TRACE_LEVEL_INFO, "Fabric Plugin: Port_log_show- entering...");
		int rval;
		char portLogShowPath[256];
		char tempLine[256];
		vector<string>  port_log;

		snprintf(portLogShowPath, 255, "/tmp/portlogshow_tmp_%d", getpid());

		trace (TRACE_LEVEL_DEBUG, string ("within portLog-plugin:") + sizeof(portLogShowPath));

		rval = fabPortLogShow(portLogShowPath);
		if (rval != 0) {
			trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:PortLogShow - fail!! rval=") + rval);

			snprintf(tempLine, 255, "Error!Unable to fetch the details");
			port_log.push_back(tempLine);

			pFabricServicePortLogMessage->setLog_entries(port_log);
		} else {
			trace (TRACE_LEVEL_INFO,string("Fabric Plugin:PortLogShow - SUCC!! ") +
						string("FileName:") + portLogShowPath);

			ifstream in;

			in.open(portLogShowPath);
			if ( ! in ) {
				trace (TRACE_LEVEL_ERROR,string("Fabric Plugin:portLogShow - Unable to Open file!!"));

				snprintf(tempLine, 255, "Error!Unable to fetch the details");
				port_log.push_back(tempLine);

				pFabricServicePortLogMessage->setLog_entries(port_log);
			} else {
				trace (TRACE_LEVEL_DEBUG, string ("portLogShow\n"));

				while (!in.eof()) {
					in.getline(tempLine, 256);

					port_log.push_back(tempLine);

					trace (TRACE_LEVEL_DEBUG, tempLine);
				}
				in.close();

				pFabricServicePortLogMessage->setLog_entries(port_log);
			}
			unlink(portLogShowPath);
		}

		return (WAVE_MESSAGE_SUCCESS);
}

void  FabricServiceLocalObjectManager::FabricServicePortLogMessageHandler( FabricServicePortLogMessage *pFabricServicePortLogMessage)
{
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::fabric_port_log_show),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServicePortLogMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    /* Set Insistent DomainId */

    ResourceId FabricServiceLocalObjectManager::SetInsistentDomainId (PrismSynchronousLinearSequencerContext *pFabricServiceSetInsistentDomainIdMessageContext)
    {
        FabricServiceSetInsistentDomainIdMessage *pFabricServiceSetInsistentDomainIdMessage =
            reinterpret_cast<FabricServiceSetInsistentDomainIdMessage*> (pFabricServiceSetInsistentDomainIdMessageContext->getPPrismMessage ());
        trace (TRACE_LEVEL_DEBUG, string("Fabric Plugin: SetInsistentDomainId - entering..."));

        int rval;
        int ididMode = pFabricServiceSetInsistentDomainIdMessage->getInsistent_domainId();

        trace (TRACE_LEVEL_INFO, string ("Fabric Plugin: SetInsistentDomainId: initial ididMode ") + ididMode);

        rval = fabSetInsistentDomainIdMode(ididMode);
        if (rval == -1) {
            trace (TRACE_LEVEL_ERROR, string("Fabric Plugin:SetLocalDomainId - fail -- SwOnline!! "));
        } else if (rval == -2) {
            trace (TRACE_LEVEL_ERROR, string("Fabric Plugin:Same Mode "));
        }
        ididMode = rval;
        trace (TRACE_LEVEL_DEBUG, string ("Fabric Plugin: SetInsistentDomainId: ididMode ") + ididMode);

        pFabricServiceSetInsistentDomainIdMessage->setInsistent_domainId(ididMode);

        trace (TRACE_LEVEL_DEBUG, "Fabric Plugin: SetInsistentDomainId - exiting...");
        return (WAVE_MESSAGE_SUCCESS);
    }

    void  FabricServiceLocalObjectManager::FabricServiceSetInsistentDomainIdMessageHandler( FabricServiceSetInsistentDomainIdMessage *pFabricServiceSetInsistentDomainIdMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::SetInsistentDomainId),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServiceSetInsistentDomainIdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	/* Disable the ISLs connected to the neighbor switch */

	ResourceId FabricServiceLocalObjectManager::DisableIslsToNeighbor(PrismSynchronousLinearSequencerContext *pFabricServiceDisableIslsToNeighborMessageContext)
	{
		trace (TRACE_LEVEL_DEBUG, string("Fabric Plugin::DisableIslsToNeighbor - entering..."));

		FabricServiceDisableIslsToNeighborMessage *pFabricServiceDisableIslsToNeighborMessage =
		                 reinterpret_cast<FabricServiceDisableIslsToNeighborMessage*>
		                 (pFabricServiceDisableIslsToNeighborMessageContext->getPPrismMessage ());

//		ResourceId status;
//		int *portList = NULL;
//		int rval, i, slotId = 0, mappedId = 0;
		int rval;
//		string portId;
//		char convertToString[20];
//		PortCfgPortCfgPortStateUpdateMessage *pcfg_msg_p = NULL;
		wwn_t wwn;
//		int dis_reason;

//		mappedId = DcmToolKit::getLocalMappedId();

		wwnscanf((char *)((pFabricServiceDisableIslsToNeighborMessage->getSwitch_wwn().toString()).c_str()), &wwn);
		trace (TRACE_LEVEL_INFO, string ("Fabric Plugin::DisableIslsToNeighbor: neighbor wwn ") + wwnfmt(&wwn));

//		dis_reason = pFabricServiceDisableIslsToNeighborMessage->getReason_code();
//		trace (TRACE_LEVEL_INFO, string("Fabric Plugin::DisableIslsToNeighbor: reason code ") + dis_reason);

//		/* Allocate memory for portList (MAX_PORT + 1) */
//		portList = (int *)malloc((MAX_PORT + 1) * (sizeof (int *)));

//		if (portList == NULL) {
//			trace (TRACE_LEVEL_ERROR, string ("Fabric Plugin::DisableIslsToNeighbor: malloc failed!"));
//			return (WAVE_MESSAGE_ERROR);
//		}
//		memset(portList, 0, ((MAX_PORT + 1) * sizeof (int *)));

		/* Calling FOS fabric API to get the list of ISLs to be disabled */
//		rval = fabGetIslList(&wwn, portList, ((MAX_PORT + 1) * sizeof (int *)));

		rval = fabToggleWWNLinks(&wwn);

		if (rval != 0) {
			trace (TRACE_LEVEL_ERROR, string ("Fabric Plugin::DisableIslsToNeighbor: IPC to fabGetIslList failed! ") + rval);
//			FREE(portList);
			return (WAVE_MESSAGE_ERROR);
		}

//		if (portList[0] <= 0) {
//			trace (TRACE_LEVEL_ERROR, string ("Fabric Plugin::DisableIslsToNeighbor: No ISLs found! ") + portList[0]);
//			FREE(portList);
//			return (WAVE_MESSAGE_SUCCESS);
//		}
//
//		for (i = 1; i <= portList[0]; i++) {
//			snprintf(convertToString, sizeof (convertToString), "%d/%d/%d", mappedId, slotId, portList[i]);
//
//			/* portId will have the portnumber in MappedId/Slot/Port format */
//			portId = convertToString;
//
//			trace (TRACE_LEVEL_INFO, string("Fabric Plugin::DisableIslsToNeighbor: portId ") + portId);
//
//			/* Sending message to PortCfg Plugin */
//			pcfg_msg_p = new PortCfgPortCfgPortStateUpdateMessage(portId, false, true, dis_reason);
//
//			status = sendSynchronously (pcfg_msg_p);
//
//			if (WAVE_MESSAGE_SUCCESS == status) {
//				trace (TRACE_LEVEL_INFO, string("Fabric Plugin::DisableIslsToNeighbor: Successfully Committed."));
//			} else {
//				trace (TRACE_LEVEL_INFO, string("Fabric Plugin::DisableIslsToNeighbor: Failed to commit"));
//			}
//			delete (pcfg_msg_p);
//		}
//
//		FREE(portList);
		trace (TRACE_LEVEL_DEBUG, "Fabric Plugin::DisableIslsToNeighbor - exiting...");
		return (WAVE_MESSAGE_SUCCESS);
	}

    void  FabricServiceLocalObjectManager::FabricServiceDisableIslsToNeighborMessageHandler( FabricServiceDisableIslsToNeighborMessage *pFabricServiceDisableIslsToNeighborMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::DisableIslsToNeighbor),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServiceDisableIslsToNeighborMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId  FabricServiceLocalObjectManager::LinkInfoShowGetEntries (PrismSynchronousLinearSequencerContext *pFabricServiceGetlinkInfoMessageContext)
	{
		FabricServiceGetlinkInfoMessage *pFabricServiceGetlinkInfoMessage = reinterpret_cast<FabricServiceGetlinkInfoMessage *> (pFabricServiceGetlinkInfoMessageContext->getPPrismMessage ());

		vector<UI32>  domain_id;
		vector<UI32>  domainreachable;
		vector<UI32>  version;
		vector<WorldWideName>  domainwwn;
		vector<UI32>  links_count;
		vector<UI32>  link_id;
		vector<UI32>  remote_port;
		vector<UI32>  pathOutPort;
		vector<UI32>  costCount;
		vector<UI32>  pathCost;
		vector<UI32>  link_type;
		vector<UI32>  trunked;
		vector<SI32>  pathOutPort_dispType;
		vector<SI32>  pathOutPort_tupleBlade;
		vector<SI32>  pathOutPort_tuplePort;
		vector<SI32>  pathRemPort_dispType;
		vector<SI32>  pathRemPort_tupleBlade;
		vector<SI32>  pathRemPort_tuplePort;
                vector<SI32>  local_qsfpidx;
                vector<SI32>  remote_qsfpidx;

		int i, j, max;
                int tupleDomain = 0, tupleBlade = 0, tuplePort = 0, qsfp_idx = -1;
		links_info_t  *linkinfo_entry_list = NULL;

		trace (TRACE_LEVEL_INFO, "Fabric Plugin: LinkInfoShowGetEntries - entering...");
                int myDomain = getMyDomain();
		max =  getFabricLinkInfo(&linkinfo_entry_list);

		trace (TRACE_LEVEL_INFO, string("Fabric Plugin: LinkInfoShowGetEntries - No. of entries=") + max);
		if (max < 0) {
			FREE(linkinfo_entry_list);
			return (WAVE_MESSAGE_SUCCESS);
		}

		if ((max >= 0) && (linkinfo_entry_list != NULL)) {
			for (i = 0; i < max; i++) {
				domain_id.push_back(linkinfo_entry_list[i].domainid);
				domainreachable.push_back(linkinfo_entry_list[i].domainReachable);
				version.push_back(linkinfo_entry_list[i].version);
				domainwwn.push_back(string((char *)&linkinfo_entry_list[i].domain_wwn));
				links_count.push_back(linkinfo_entry_list[i].numberOfLinks);

				/* Loop for the numberoflinks per Domain basis */
				for (j = 0; j < linkinfo_entry_list[i].numberOfLinks; j++) {
					link_id.push_back(linkinfo_entry_list[i].links[j].linkId);
					remote_port.push_back(linkinfo_entry_list[i].links[j].remPort);
					pathOutPort.push_back(linkinfo_entry_list[i].links[j].outPort);
					costCount.push_back(linkinfo_entry_list[i].links[j].costCnt);
					pathCost.push_back(linkinfo_entry_list[i].links[j].cost);
					link_type.push_back(linkinfo_entry_list[i].links[j].type);
					trunked.push_back(linkinfo_entry_list[i].links[j].trunked);
					pathOutPort_dispType.push_back(linkinfo_entry_list[i].links[j].dispType);
					pathOutPort_tupleBlade.push_back(linkinfo_entry_list[i].links[j].blade);
					pathOutPort_tuplePort.push_back(linkinfo_entry_list[i].links[j].phyPort);

					/* retrieve neighbor interface tuple */
					ngbhr_info_t nbr_interface = {0xff, 0xff, 0xff};
					neighborInterfaceTuple(linkinfo_entry_list, linkinfo_entry_list[i].links[j].linkId, linkinfo_entry_list[i].links[j].remPort, max, &nbr_interface);
					pathRemPort_dispType.push_back(nbr_interface.nbrdispType);
					pathRemPort_tupleBlade.push_back(nbr_interface.nbrtupleBlade);
					pathRemPort_tuplePort.push_back(nbr_interface.nbrtuplePort);
                                        
                                        /* Get local and remote qsfp index to send back */
                                        if (myDomain == linkinfo_entry_list[i].domainid ) {
                                                getPortTupleRemote(linkinfo_entry_list[i].links[j].outPort, &tupleDomain, &tupleBlade, &tuplePort, &qsfp_idx);
                                                remote_qsfpidx.push_back(qsfp_idx);
                                                local_qsfpidx.push_back(getQSFPTupleIndex(linkinfo_entry_list[i].links[j].outPort));
                                        } else {
                                                getPortTupleRemote(linkinfo_entry_list[i].links[j].remPort, &tupleDomain, &tupleBlade, &tuplePort, &qsfp_idx);
                                                local_qsfpidx.push_back(qsfp_idx);
                                                remote_qsfpidx.push_back(getQSFPTupleIndex(linkinfo_entry_list[i].links[j].remPort));
                                        }
				}
			}
			for (i = 0; i < max; i++) {
				FREE(linkinfo_entry_list[i].links);
			}
			FREE(linkinfo_entry_list);
		}

		// Set Call for the objects defined in the XML
		pFabricServiceGetlinkInfoMessage->setDomain_id(domain_id);
		pFabricServiceGetlinkInfoMessage->setDomainreachable(domainreachable);
		pFabricServiceGetlinkInfoMessage->setVersion(version);
		pFabricServiceGetlinkInfoMessage->setDomainwwn(domainwwn);
		pFabricServiceGetlinkInfoMessage->setLinks_count(links_count);
		pFabricServiceGetlinkInfoMessage->setLink_id(link_id);
		pFabricServiceGetlinkInfoMessage->setRemote_port(remote_port);
		pFabricServiceGetlinkInfoMessage->setPathCost(pathCost);
		pFabricServiceGetlinkInfoMessage->setCostCount(costCount);
		pFabricServiceGetlinkInfoMessage->setTrunked(trunked);
		pFabricServiceGetlinkInfoMessage->setPathOutPort(pathOutPort);
		pFabricServiceGetlinkInfoMessage->setLink_type(link_type);
		pFabricServiceGetlinkInfoMessage->setPathOutPort_dispType(pathOutPort_dispType);
		pFabricServiceGetlinkInfoMessage->setPathOutPort_tupleBlade(pathOutPort_tupleBlade);
		pFabricServiceGetlinkInfoMessage->setPathOutPort_tuplePort(pathOutPort_tuplePort);
		pFabricServiceGetlinkInfoMessage->setPathRemPort_dispType(pathRemPort_dispType);
		pFabricServiceGetlinkInfoMessage->setPathRemPort_tupleBlade(pathRemPort_tupleBlade);
		pFabricServiceGetlinkInfoMessage->setPathRemPort_tuplePort(pathRemPort_tuplePort);
                pFabricServiceGetlinkInfoMessage->setLocal_qsfpidx(local_qsfpidx);
                pFabricServiceGetlinkInfoMessage->setRemote_qsfpidx(remote_qsfpidx);
		trace (TRACE_LEVEL_INFO, "Fabric Plugin: LinkInfoShowGetEntries - exiting...");
		return (WAVE_MESSAGE_SUCCESS);
}

void  FabricServiceLocalObjectManager::FabricServiceGetlinkInfoMessageHandler( FabricServiceGetlinkInfoMessage *pFabricServiceGetlinkInfoMessage)
{
	// Validations and Queries should go here

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
		// Your configuration change code goes here

		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
		// Programming Protocol Daemons goes here
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::LinkInfoShowGetEntries),
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
		reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FabricServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFabricServiceGetlinkInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
	pPrismSynchronousLinearSequencerContext->execute ();

}
	int FabricServiceLocalObjectManager::neighborInterfaceTuple(links_info_t *linkinfo, int nbrdomain, int nbrport, int domaincount, ngbhr_info_t *nbr_interface) {
		int i = 0, j = 0, found = 0;

		if (linkinfo != NULL) {
			/* for all the domains */
			 for (i = 0; i < domaincount && !found; i++) {
				/* check for the neighbor domain to loop thro' */
				if (linkinfo[i].domainid == nbrdomain) {
					/* loop thro' all the links in the domain */
					for (j = 0; j < linkinfo[i].numberOfLinks && !found; j++) {
						/* copy source tuple information */
						if (linkinfo[i].links[j].outPort == nbrport) {
							nbr_interface->nbrdispType = linkinfo[i].links[j].dispType;
							nbr_interface->nbrtupleBlade = linkinfo[i].links[j].blade;
							nbr_interface->nbrtuplePort = linkinfo[i].links[j].phyPort;
							found = 1;
						}
					}
				}
			}
			return (0);
		} else {
			return (-1);
		}
	}
}
