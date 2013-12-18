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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "DeviceService/Local/DeviceServiceLocalObjectManager.h"
#include "DeviceService/Local/DeviceServiceGetAllMessage.h"
#include "DeviceService/Local/DeviceServiceTypes.h"

extern "C"
{
#include "fabric/getid.h"
#include "fc/wwn.h"
// #include "switch/port.h"
#include "sys/fabos/if.h"
#include "ns/ns_common.h"
#include "ns/public.h"
}

namespace DcmNs
{

    DeviceServiceLocalObjectManager::DeviceServiceLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (DEVICESERVICEGETALL, reinterpret_cast<PrismMessageHandler> (&DeviceServiceLocalObjectManager::DeviceServiceGetAllMessageHandler));
    }

    DeviceServiceLocalObjectManager::~DeviceServiceLocalObjectManager ()
    {
    }

    DeviceServiceLocalObjectManager  *DeviceServiceLocalObjectManager::getInstance()
    {
        static DeviceServiceLocalObjectManager *pDeviceServiceLocalObjectManager = new DeviceServiceLocalObjectManager ();

        WaveNs::prismAssert (NULL != pDeviceServiceLocalObjectManager, __FILE__, __LINE__);

        return (pDeviceServiceLocalObjectManager);
    }

    string  DeviceServiceLocalObjectManager::getClassName()
    {
        return ("DeviceService");
    }

    PrismServiceId  DeviceServiceLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *DeviceServiceLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case DEVICESERVICEGETALL :
                pPrismMessage = new DeviceServiceGetAllMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DeviceServiceLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

	ResourceId DeviceServiceLocalObjectManager::getAllEntries (PrismSynchronousLinearSequencerContext *pDeviceServiceGetAllMessageContext)
	{
        DeviceServiceGetAllMessage *pDeviceServiceGetAllMessage = reinterpret_cast<DeviceServiceGetAllMessage *> (pDeviceServiceGetAllMessageContext->getPPrismMessage ());
//	VcsNodeLocalManagedObject               *pvcsNodeLocalManagedObject = pVcsNodeLocalMessagingContext->getPVcsNodeLocalManagedObject();
//	pVcsNodeGetVcsIdMessage->setVcsId(pvcsNodeLocalManagedObject->getVcsId());
	    vector<UI32> port_id;
        vector<UI32> port_type;
        vector<UI32> class_of_service;
        vector<WorldWideName> port_wwn_name;
        vector<WorldWideName> node_wwn_name;
        vector<UI32> state_change_registration;
        vector<UI32> fc4s_supported;
        vector<string> port_symbolic_name;
        vector<string> node_symbolic_name;
        vector<WorldWideName> fabric_port_wwn_name;
        vector<WorldWideName> permanent_port_wwn_name;
        vector<UI32> port_index;
        vector<UI32> shared_area;
        vector<UI32> frame_redirection;
        vector<UI32> partial_entry;
        vector<UI32> device_type;
        vector<IpV6Address> device_ip_addr;
        vector<UI32> device_ipa;
        vector<IpV6Address> device_ip_port;
		vector<UI32> is_connected_via_ag;
		vector<UI32> is_real;
		vector<UI32> is_cascaded;
		vector<UI32> is_ag_base_device;
	    ns_entry_t  *ns_entry_list = NULL;
		int numEntries;
	    ns_detail_entry_t  *ns_d_list = NULL;
	    char sym[NS_XBUF_LEN+1];
		// char *buf = NULL;

	    trace (TRACE_LEVEL_INFO, string ("within NS plugin. domin id ") +
	        pDeviceServiceGetAllMessage->getDomain_id());

	    if (pDeviceServiceGetAllMessage->getRequest_type() ==
	        DeviceServiceGetAllMessage::get_all_type) {

	        trace (TRACE_LEVEL_INFO, string ("got here"));

	        numEntries = nsGetAllEntries_full_db(&ns_entry_list);

	        trace (TRACE_LEVEL_INFO, string ("retrieving ") + numEntries +
	            string (" entries"));

	        for (int i = 0; i < numEntries; i++) {
	            port_id.push_back(ns_entry_list[i].ns_pid);
                port_type.push_back(ns_entry_list[i].ns_ptype);
	            // fc4s supported is defined as a vector of
	            // uint while it takes 8 uints. So, I am using
	            // groups of 8 uints for fc4s supported per
	            // each entry
	            for (int j = 0; j < 8; j++) {
                    fc4s_supported.push_back(
	                    ns_entry_list[i].ns_fc4s[j]);
	            }
	        }

	        if (numEntries > 0) {
	            free(ns_entry_list);
	        }
	    } else if (pDeviceServiceGetAllMessage->getRequest_type() ==
	        DeviceServiceGetAllMessage::get_detail_type) {
	        // if we need to get a hold of detail entries, we'll go
	        // ahead and get the detail entries from local domain
	        // and agument with cache information for other domains
	        // once gw gets the full information, it can decide
	        // how to filter and print

	        // doing the local full db
	        numEntries = nsGetLocalDetailEntries(&ns_d_list);

	        trace (TRACE_LEVEL_INFO, string ("retrieving ") + numEntries +
	            string (" local entries"));

	        for (int i = 0; i < numEntries; i++) {
	            port_id.push_back(ns_d_list[i].ns_med.ns_pid);
                port_type.push_back(ns_d_list[i].ns_med.ns_ptype);
                class_of_service.push_back(ns_d_list[i].ns_med.ns_cos);
	            // fc4s supported is defined as a vector of
	            // uint while it takes 8 uints. So, I am using
	            // groups of 8 uints for fc4s supported per
	            // each entry
	            for (int j = 0; j < 8; j++) {
                    fc4s_supported.push_back(
	                    ns_d_list[i].ns_med.ns_fc4s[j]);
	            }
	            nsSymbToStrEx(&(ns_d_list[i].ns_med.ns_psymb), sym, sizeof(nssymb_t));
                port_symbolic_name.push_back(sym);
	            nsSymbToStrEx(&(ns_d_list[i].ns_med.ns_nsymb), sym, sizeof(nssymb_t));
                node_symbolic_name.push_back(sym);
                port_index.push_back( ns_d_list[i].ns_med.ns_portindex);
                shared_area.push_back( ns_d_list[i].ns_med.ns_sharearea);
                device_type.push_back( ns_d_list[i].ns_med.ns_devtype);
	            port_wwn_name.push_back(
	                string(wwnfmt(&ns_d_list[i].ns_med.ns_pname)));
	            node_wwn_name.push_back(
	                string(wwnfmt(&ns_d_list[i].ns_med.ns_nname)));
	            fabric_port_wwn_name.push_back(
	                string(wwnfmt(&ns_d_list[i].ns_med.ns_fxpname)));
				/*
				sprintf(buf, "%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d",
						(ns_d_list[i].ns_med.ns_ipaddr[0] & 0xFF), (ns_d_list[i].ns_med.ns_ipaddr[1] & 0xFF),
						(ns_d_list[i].ns_med.ns_ipaddr[2] & 0xFF), (ns_d_list[i].ns_med.ns_ipaddr[3] & 0xFF),
						(ns_d_list[i].ns_med.ns_ipaddr[4] & 0xFF), (ns_d_list[i].ns_med.ns_ipaddr[5] & 0xFF),
						(ns_d_list[i].ns_med.ns_ipaddr[6] & 0xFF), (ns_d_list[i].ns_med.ns_ipaddr[7] & 0xFF),
						(ns_d_list[i].ns_med.ns_ipaddr[8] & 0xFF), (ns_d_list[i].ns_med.ns_ipaddr[9] & 0xFF),
						(ns_d_list[i].ns_med.ns_ipaddr[10] & 0xFF), (ns_d_list[i].ns_med.ns_ipaddr[11] & 0xFF),
						(ns_d_list[i].ns_med.ns_ipaddr[12] & 0xFF), (ns_d_list[i].ns_med.ns_ipaddr[13] & 0xFF),
						(ns_d_list[i].ns_med.ns_ipaddr[14] & 0xFF), (ns_d_list[i].ns_med.ns_ipaddr[15] & 0xFF));
				*/
				if (ns_d_list[i].ns_med.ns_ipaddr != NULL) {
					nsIpAddrToStr((unsigned char *)&ns_d_list[i].ns_med.ns_ipaddr, sym, sizeof(sym));
					device_ip_addr.push_back(
						string(sym));
				} else {
					trace (TRACE_LEVEL_INFO, string ("ns_d_list[i].ns_med.ns_ipaddr == Null "));

				}
	            // ipa supported is defined as a vector of
	            // uchar while it takes 8 uchars. So, I am using
	            // groups of 8 uchars for ipa supported per
	            // each entry
	            for (int j = 0; j < 8; j++) {
                    device_ipa.push_back(
	                    ns_d_list[i].ns_med.ns_ipa[j]);
	            }

                state_change_registration.push_back(ns_d_list[i].ns_scr);
	            permanent_port_wwn_name.push_back(
	                string(wwnfmt(&ns_d_list[i].ns_ppn)));
	            int redirect_map;
	            redirect_map =
	                ns_d_list[i].is_redirect << 12 |
	                ns_d_list[i].is_vivt << 8 |
	                ns_d_list[i].is_rdhost << 4 |
	                ns_d_list[i].is_rdtarget;
                frame_redirection.push_back(redirect_map);

	            device_ip_port.push_back(
	                string((char *)ns_d_list[i].ns_med.ns_ipport));

                partial_entry.push_back(ns_d_list[i].is_partial);
				
				/* Get all AG related Information */
				is_connected_via_ag.push_back(ns_d_list[i].is_connected_via_ag);
				is_real.push_back(ns_d_list[i].ns_real);
				is_cascaded.push_back(ns_d_list[i].ns_cascaded);
				is_ag_base_device.push_back(ns_d_list[i].is_ag_base_device);
	        }

	        if (numEntries > 0) {
	            free(ns_d_list);
	        }

	        // let's do the rest of the fabric
	        numEntries = nsGetAllDetailEntries_fullDb(&ns_d_list);
	        int returned_count = 0;

	        trace (TRACE_LEVEL_INFO, string ("retrieving ") + numEntries +
	            string (" all entries"));

	        for (int i = 0; i < numEntries; i++) {
	            // let's skip over the local domain entries since
	            // I got them aboe
	            if (getMyDomain() ==
	                (int)((ns_d_list[i].ns_med.ns_pid & 0xff0000) >> 16)) {
	                continue;
	            }

	            returned_count++;
	            port_id.push_back(ns_d_list[i].ns_med.ns_pid);
                port_type.push_back(ns_d_list[i].ns_med.ns_ptype);
                class_of_service.push_back(ns_d_list[i].ns_med.ns_cos);
	            // fc4s supported is defined as a vector of
	            // uint while it takes 8 uints. So, I am using
	            // groups of 8 uints for fc4s supported per
	            // each entry
	            for (int j = 0; j < 8; j++) {
                    fc4s_supported.push_back(
	                    ns_d_list[i].ns_med.ns_fc4s[j]);
	            }
	            nsSymbToStrEx(&(ns_d_list[i].ns_med.ns_psymb), sym, sizeof(sym));
                port_symbolic_name.push_back(sym);
	            nsSymbToStrEx(&(ns_d_list[i].ns_med.ns_nsymb), sym, sizeof(sym));
                node_symbolic_name.push_back(sym);
                port_index.push_back(ns_d_list[i].ns_med.ns_portindex);
                shared_area.push_back(ns_d_list[i].ns_med.ns_sharearea);
                device_type.push_back(ns_d_list[i].ns_med.ns_devtype);
	            port_wwn_name.push_back(
	                string(wwnfmt(&ns_d_list[i].ns_med.ns_pname)));
	            node_wwn_name.push_back(
	                string(wwnfmt(&ns_d_list[i].ns_med.ns_nname)));
	            fabric_port_wwn_name.push_back(
	                string(wwnfmt(&ns_d_list[i].ns_med.ns_fxpname)));

				nsIpAddrToStr((unsigned char *)&ns_d_list[i].ns_med.ns_ipaddr, sym, sizeof(sym));
	            device_ip_addr.push_back(
	                string(sym));
	            // ipa supported is defined as a vector of
	            // uchar while it takes 8 uchars. So, I am using
	            // groups of 8 uchars for ipa supported per
	            // each entry
	            for (int j = 0; j < 8; j++) {
                    device_ipa.push_back(
	                    ns_d_list[i].ns_med.ns_ipa[j]);
	            }

	            device_ip_port.push_back(
	                string((char *)ns_d_list[i].ns_med.ns_ipport));

                partial_entry.push_back(ns_d_list[i].is_partial);

				/* Get all AG related Information */
				is_connected_via_ag.push_back(ns_d_list[i].is_connected_via_ag);
				is_real.push_back(ns_d_list[i].ns_real);
				is_cascaded.push_back(ns_d_list[i].ns_cascaded);
				is_ag_base_device.push_back(ns_d_list[i].is_ag_base_device);

				permanent_port_wwn_name.push_back(
	                string(wwnfmt(&ns_d_list[i].ns_ppn)));

	            // rest is filled in with empty info
                state_change_registration.push_back(0xffffffff);
	            // nswwn_t empty_wwn;
	            // memset ((char *)&empty_wwn, 0, sizeof (nswwn_t));
	            // permanent_port_wwn_name.push_back(string(wwnfmt(&empty_wwn)));
                frame_redirection.push_back(0xffffffff);
	        }

	        trace (TRACE_LEVEL_INFO, string ("returned ") + returned_count +
	            string (" all entries"));

	        if (numEntries > 0) {
				/* Defect 403244: NSd memory leak - free proper memory */
	            free(ns_d_list);
	        }
	    } else if (pDeviceServiceGetAllMessage->getRequest_type() ==
	        DeviceServiceGetAllMessage::node_find_type) {
	       	ns_detail_entry_t ns_detail_entry;
	       	// ns_entry_t ns_entry;
	        int single_entry_found = 0;
	        int local_entry = 0;

	        // if the size of port id array is equal to one, we are
	        // looking for entry based on port id.
	        // otherwise, check for port wwn location
	        if (pDeviceServiceGetAllMessage->getPort_id().size() == 1) {
	            trace (TRACE_LEVEL_INFO, string ("pid passed in") +
	                pDeviceServiceGetAllMessage->getPort_id().at(0));
	            if (getMyDomain() ==
                    (int)((pDeviceServiceGetAllMessage->getPort_id().at(0) &
	                0xff0000) >> 16)) {
	                if (nsGetLocalDetailEntryByPid(
	                    pDeviceServiceGetAllMessage->getPort_id().at(0),
	                    &ns_detail_entry) == 0) {
	                    single_entry_found++;
	                    local_entry = 1;
	                }
	            } else {
	                if (nsGetDetailEntryByPid_no_ad(
	                    pDeviceServiceGetAllMessage->getPort_id().at(0),
	                    NSDB_GETEXACT, &ns_detail_entry) == 0) {
	                    single_entry_found++;
	                    local_entry = 0;
	                }
	            }
	        }

	        trace (TRACE_LEVEL_INFO, string ("single entry flag ") +
	            single_entry_found + string (" local entry flag ") +
	            local_entry);

	        if (single_entry_found) {
	            port_id.push_back(ns_detail_entry.ns_med.ns_pid);
                port_type.push_back(ns_detail_entry.ns_med.ns_ptype);
                class_of_service.push_back(ns_detail_entry.ns_med.ns_cos);
	            for (int j = 0; j < 8; j++) {
                    fc4s_supported.push_back(
	                    ns_detail_entry.ns_med.ns_fc4s[j]);
	            }
	            nsSymbToStrEx(&(ns_detail_entry.ns_med.ns_psymb), sym, sizeof(sym));
                port_symbolic_name.push_back(sym);
	            nsSymbToStrEx(&(ns_detail_entry.ns_med.ns_nsymb), sym, sizeof(sym));
                node_symbolic_name.push_back(sym);
                port_index.push_back( ns_detail_entry.ns_med.ns_portindex);
                shared_area.push_back( ns_detail_entry.ns_med.ns_sharearea);
                device_type.push_back( ns_detail_entry.ns_med.ns_devtype);
	            port_wwn_name.push_back(
	                string(wwnfmt(&ns_detail_entry.ns_med.ns_pname)));
	            node_wwn_name.push_back(
	                string(wwnfmt(&ns_detail_entry.ns_med.ns_nname)));
	            fabric_port_wwn_name.push_back(
	                string(wwnfmt(&ns_detail_entry.ns_med.ns_fxpname)));

				nsIpAddrToStr((unsigned char *)&ns_detail_entry.ns_med.ns_ipaddr, sym, sizeof(sym));
					device_ip_addr.push_back(string(sym));

	            for (int j = 0; j < 8; j++) {
                    device_ipa.push_back(
	                    ns_detail_entry.ns_med.ns_ipa[j]);
	            }


	            permanent_port_wwn_name.push_back(
	                string(wwnfmt(&ns_detail_entry.ns_ppn)));

	            device_ip_port.push_back(
	                string((char *)ns_detail_entry.ns_med.ns_ipport));

                partial_entry.push_back(ns_detail_entry.is_partial);

				/* Get all AG related Information */
				is_connected_via_ag.push_back(ns_detail_entry.is_connected_via_ag);
				is_real.push_back(ns_detail_entry.ns_real);
				is_cascaded.push_back(ns_detail_entry.ns_cascaded);
				is_ag_base_device.push_back(ns_detail_entry.is_ag_base_device);

				if (local_entry) {
					state_change_registration.push_back(ns_detail_entry.ns_scr);
					int redirect_map;
					redirect_map =
						ns_detail_entry.is_redirect << 12 |
						ns_detail_entry.is_vivt << 8 |
						ns_detail_entry.is_rdhost << 4 |
						ns_detail_entry.is_rdtarget;
					frame_redirection.push_back(redirect_map);
				} else {
					state_change_registration.push_back(0xffffffff);
					frame_redirection.push_back(0xffffffff);
				}
	        }

		} else if (pDeviceServiceGetAllMessage->getRequest_type() ==
	        DeviceServiceGetAllMessage::zone_member_type) {

			int entries_found = 0;

			if (pDeviceServiceGetAllMessage->getPort_id().size() == 1) {
				/*
				 * Notable Difference ---
				 *		'zone_member_type' retrieves 'ns_entry_t' type entries,
				 *  	where other operations retrieve 'ns_detail_entry_t'
				 *  	type entries.
				 */
				entries_found = nsGetZonedEntryByPid(pDeviceServiceGetAllMessage->getPort_id().at(0), &ns_entry_list);
			}

			for (int i = 0; i < entries_found; i++) {

	            port_id.push_back(ns_entry_list[i].ns_pid);
                port_type.push_back(ns_entry_list[i].ns_ptype);
                class_of_service.push_back(ns_entry_list[i].ns_cos);
	            // fc4s supported is defined as a vector of
	            // uint while it takes 8 uints. So, I am using
	            // groups of 8 uints for fc4s supported per
	            // each entry
	            for (int j = 0; j < 8; j++) {
                    fc4s_supported.push_back(
	                    ns_entry_list[i].ns_fc4s[j]);
	            }
	            nsSymbToStrEx(&(ns_entry_list[i].ns_psymb), sym, sizeof(sym));
                port_symbolic_name.push_back(sym);
	            nsSymbToStrEx(&(ns_entry_list[i].ns_nsymb), sym, sizeof(sym));
                node_symbolic_name.push_back(sym);
                port_index.push_back(ns_entry_list[i].ns_portindex);
                shared_area.push_back(ns_entry_list[i].ns_sharearea);
                device_type.push_back(ns_entry_list[i].ns_devtype);
	            port_wwn_name.push_back(
	                string(wwnfmt(&ns_entry_list[i].ns_pname)));
	            node_wwn_name.push_back(
	                string(wwnfmt(&ns_entry_list[i].ns_nname)));
	            fabric_port_wwn_name.push_back(
	                string(wwnfmt(&ns_entry_list[i].ns_fxpname)));

				nsIpAddrToStr((unsigned char *)&ns_entry_list[i].ns_ipaddr, sym, sizeof(sym));
	            device_ip_addr.push_back(
	                string(sym));
	            // ipa supported is defined as a vector of
	            // uchar while it takes 8 uchars. So, I am using
	            // groups of 8 uchars for ipa supported per
	            // each entry
	            for (int j = 0; j < 8; j++) {
                    device_ipa.push_back(
	                    ns_entry_list[i].ns_ipa[j]);
	            }

	            device_ip_port.push_back(
	                string((char *)ns_entry_list[i].ns_ipport));

				/* 'ns_entry_t' does not store 'is_partial'; set to zero */
                partial_entry.push_back(0);

				/* 'ns_entry_t' does not store 'is_connected_via_ag'; set to zero */
				is_connected_via_ag.push_back(0);
				/* 'ns_entry_t' does not store 'ns_real'; set to zero */
				is_real.push_back(0);
				/* 'ns_entry_t' does not store 'ns_cascad'; set to zero */
				is_cascaded.push_back(0);
				/* 'ns_entry_t' does not store 'is_ag_base_device'; set to zero */
				is_ag_base_device.push_back(0);

	            // rest is filled in with empty info
                state_change_registration.push_back(0xffffffff);
	            nswwn_t empty_wwn;
	            memset ((char *)&empty_wwn, 0, sizeof (nswwn_t));
	            permanent_port_wwn_name.push_back(string(wwnfmt(&empty_wwn)));
                frame_redirection.push_back(0xffffffff);

	        }

	        if (entries_found > 0) {
	            free(ns_entry_list);
	        }
		}


	    pDeviceServiceGetAllMessage->setPort_id(port_id);
    	pDeviceServiceGetAllMessage->setPort_type(port_type);
    	pDeviceServiceGetAllMessage->setClass_of_service(class_of_service);
    	pDeviceServiceGetAllMessage->setPort_wwn_name(port_wwn_name);
    	pDeviceServiceGetAllMessage->setNode_wwn_name(node_wwn_name);
    	pDeviceServiceGetAllMessage->setState_change_registration(
	    state_change_registration);
	    pDeviceServiceGetAllMessage->setFc4s_supported(fc4s_supported);
	    pDeviceServiceGetAllMessage->setPort_symbolic_name(
	        port_symbolic_name);
	    pDeviceServiceGetAllMessage->setNode_symbolic_name(
	        node_symbolic_name);
	    pDeviceServiceGetAllMessage->setFabric_port_wwn_name(
	        fabric_port_wwn_name);
	    pDeviceServiceGetAllMessage->setPermanent_port_wwn_name(
	        permanent_port_wwn_name);
    	pDeviceServiceGetAllMessage->setPort_index(port_index);
    	pDeviceServiceGetAllMessage->setShared_area(shared_area);
    	pDeviceServiceGetAllMessage->setFrame_redirection(
	        frame_redirection);
    	pDeviceServiceGetAllMessage->setPartial_entry(partial_entry);
    	pDeviceServiceGetAllMessage->setDevice_type(device_type);
    	pDeviceServiceGetAllMessage->setDevice_ip_addr(device_ip_addr);
    	pDeviceServiceGetAllMessage->setDevice_ipa(device_ipa);
    	pDeviceServiceGetAllMessage->setDevice_ip_port(device_ip_port);
		pDeviceServiceGetAllMessage->setIs_connected_via_ag(is_connected_via_ag);
		pDeviceServiceGetAllMessage->setIs_real(is_real);
		pDeviceServiceGetAllMessage->setIs_cascaded(is_cascaded);
		pDeviceServiceGetAllMessage->setIs_ag_base_device(is_ag_base_device);
            

        return (WAVE_MESSAGE_SUCCESS);
	}
    void  DeviceServiceLocalObjectManager::DeviceServiceGetAllMessageHandler( DeviceServiceGetAllMessage *pDeviceServiceGetAllMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DeviceServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DeviceServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DeviceServiceLocalObjectManager::getAllEntries),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DeviceServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DeviceServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pDeviceServiceGetAllMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

}
