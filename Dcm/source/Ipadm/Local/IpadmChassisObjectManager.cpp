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
 *	Copyright (C) 2005-2010 Brocade Communications Systems, Inc.		 *
 *	All rights reserved.												 *
 *	Author : cshah													 *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Ipadm/Local/IpadmChassisIpConfigMessage.h"
#include "Ipadm/Local/IpadmChassisIpDeleteMessage.h"
#include "Ipadm/Local/IpadmChassisObjectManager.h"
#include "Ipadm/Local/IpadmChassisManagedObject.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "fos_swbd.h"
#include <ha/ipadm.h>

namespace DcmNs
{

	IpadmChassisObjectManager::IpadmChassisObjectManager ()
		: WaveLocalObjectManager (getClassName ())
	{
		IpadmChassisManagedObject	IpadmChassisManagedObject	(this);
		IpadmChassisManagedObject.setupOrm ();
		addManagedClass (IpadmChassisManagedObject::getClassName ());
		addOperationMap (IPADMCHASSISCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler));
		addOperationMap (IPADMCHASSISDELETE, reinterpret_cast<PrismMessageHandler> (&IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler));
	}

	IpadmChassisObjectManager::~IpadmChassisObjectManager ()
	{
	}

	IpadmChassisObjectManager  *IpadmChassisObjectManager::getInstance()
	{
		static IpadmChassisObjectManager *pIpadmChassisObjectManager = new IpadmChassisObjectManager ();

		WaveNs::prismAssert (NULL != pIpadmChassisObjectManager, __FILE__, __LINE__);

		return (pIpadmChassisObjectManager);
	}

	string  IpadmChassisObjectManager::getClassName()
	{
		return ("IpadmChassis");
	}

	PrismServiceId  IpadmChassisObjectManager::getPrismServiceId()
	{
		return ((getInstance ())->getServiceId ());
	}

	PrismMessage *IpadmChassisObjectManager::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;

		switch (operationCode)
		{

			case IPADMCHASSISCONFIG :
				pPrismMessage = new IpadmChassisIpConfigMessage ();
				break;
			case IPADMCHASSISDELETE :
				pPrismMessage = new IpadmChassisIpDeleteMessage ();
				break;
			default :
				pPrismMessage = NULL;
		}

		return (pPrismMessage);
	}

	WaveManagedObject *IpadmChassisObjectManager::createManagedObjectInstance(const string &managedClassName)
	{
		WaveManagedObject *pWaveManagedObject = NULL;

		if ((IpadmChassisManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new IpadmChassisManagedObject(this);
		}
		else
		{
			trace (TRACE_LEVEL_FATAL, "IpadmChassisObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
		}
		return (pWaveManagedObject);
	}

	void IpadmChassisObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
		ResourceId status               = FRAMEWORK_ERROR;
		ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
		struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
		ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
		addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
		ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
		struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
		addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
		ipadm_ip_get_set_t current = {
			0, /* num_locals */
			local_addr_ids,
			local_addrs,
			local_prefixes,
			local_flags,
			0, /* num_gws */
			gw_addr_ids,
			gw_addrs,
			gw_flags,
			0 /* dhcp_flag */
		};


		string ipv6_address ("fe80::21e:bff:febf:47d4/64");
		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		char *addr_c;
		string ip_interface, ip_str;
		int ipv4_address_present = 0;
		int ipv6_address_present = 0;

		int rc = -1;
		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc)
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::install ipAdmLocalAddrIdGet() failed");
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc)
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::install ipAdmGwAddrIdGet() failed");
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc)
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::install ipAdmIpAddrGet() failed");
		}

		//for pizza systems chassis address is not relevant 
                int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);	
		if (pizza)
		{
			trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::install Chassis address is not relevant on pizza systems");
			status = WAVE_MESSAGE_SUCCESS;
		}
		else
		{
			//for chassis systems we are creating chassis object
			int eth_idx = -1;

			eth_idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET);
			if (eth_idx < 0)
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::install eth_idx < 0");
			}
			else
			{
				if ((current.local_flags[eth_idx] & IPADM_ADDR_NONE) != IPADM_ADDR_NONE)
				{
					addr_c = inet_ntoa(SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr);
					ip_str = addr_c;
					int prefix = 0;
					if ((current.local_flags[eth_idx] & IPADM_MASK_NONE) != IPADM_MASK_NONE)
					{
						prefix = current.local_prefixes[eth_idx];
					}
					char buf1[10];
					sprintf(buf1, "/%d", prefix);
		
					ip_interface = (ip_str + buf1);
					ipv4_address_present = 1;
				}
			}

			int eth_idx6 = -1;
	
			eth_idx6 = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET6);
			if (eth_idx6 < 0)
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::install eth_idx < 0");
			}
			else
			{
				if ((current.local_flags[eth_idx6] & IPADM_ADDR_NONE) != IPADM_ADDR_NONE)
				{
					char buf[100];
					const char *rcc = inet_ntop(AF_INET6, &(SOCKADDR_IN6P(current.local_addrs[eth_idx6])->sin6_addr), buf, sizeof (buf));
					if (rcc == NULL) 
					{
						trace (TRACE_LEVEL_ERROR, string ("inet_ntop() failed "));
					}		
					int prefix6 = current.local_prefixes[eth_idx6];
					char bufp6[10];
					sprintf(bufp6, "/%d", prefix6);
					string prefix6string;
					prefix6string.assign(bufp6);
					ipv6_address.assign(buf);
					ipv6_address += prefix6string;
					ipv6_address_present = 1;
				}
			}
    			LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
	
	    		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, IpadmChassisManagedObject::getClassName ());
	
			if ((pResults != NULL) && (pResults->size() == 0) && (ipv4_address_present || ipv6_address_present))
			{
				trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::install : Creating ChassisObject");
				
				startTransaction ();
				IpadmChassisManagedObject *pInterface = new IpadmChassisManagedObject (this);

				if(ipv4_address_present)
					pInterface->setVipAddress (ip_interface);
	
				if(ipv6_address_present)
					pInterface->setVipV6Address (ipv6_address);
	
				updateWaveManagedObject (pInterface);
				status = commitTransaction ();

				if (FRAMEWORK_SUCCESS == status)
				{
					trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::install: Installed Management Interface for demo");
	
					status = WAVE_MESSAGE_SUCCESS;
				}
				else
				{
					trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::install: Installed Management Interface for demo FAILED");
					prismAssert (false, __FILE__, __LINE__);
				}
				delete pInterface;
	
			}
			else
			{
				//status SUCCESS even if we are not able to install
				status = WAVE_MESSAGE_SUCCESS;
			}
                if (pResults)
                	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

		}	
		if(pWaveAsynchronousContextForBootPhases == NULL)
		{
			trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::install: pWaveAsynchronousContextForBootPhases is NULL");
		}
		else
		{
			pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	    		pWaveAsynchronousContextForBootPhases->callback ();
		}
	}

	void  IpadmChassisObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	{
		SI8 PassNum  = pWaveAsynchronousContextForPostBootPhase->getPassNum();
		trace (TRACE_LEVEL_INFO, string("IpadmChassisObjectManager::postboot  Pass ") + PassNum);

    		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();

    		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, IpadmChassisManagedObject::getClassName ());

		if ((pResults != NULL) && (pResults->size() != 0))
		{
			IpadmChassisManagedObject *pVipMo = dynamic_cast<IpadmChassisManagedObject *> ((*pResults)[0]);
			
			string ipAddress = pVipMo->getVipAddress();
			string ipV6Address = pVipMo->getVipV6Address();
			
			ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
			struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
			ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
			addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
			ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
			struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
			addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
			ipadm_ip_get_set_t current = {
				0, /* num_locals */
				local_addr_ids,
				local_addrs,
				local_prefixes,
				local_flags,
				0, /* num_gws */
				gw_addr_ids,
				gw_addrs,
				gw_flags,
				0 /* dhcp_flag */
			};

			current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
			current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

			size_t pos, pos6;
			int prefix, prefix6;
			string str_prefix, str_ipaddr;
			string str_prefix6, str_ipaddr6;

				pos = ipAddress.find("/");
				str_ipaddr = ipAddress.substr(0, pos);
				str_prefix = ipAddress.substr(pos+1);
				prefix = atoi(str_prefix.c_str());
				trace (TRACE_LEVEL_INFO, string("IpadmChassisObjectManager::postboot address/prefix") + str_ipaddr + string("/") + prefix);
				pos6 = ipV6Address.find("/");
				str_ipaddr6 = ipV6Address.substr(0, pos6);
				str_prefix6 = ipV6Address.substr(pos6+1);
				prefix6 = atoi(str_prefix6.c_str());
				trace (TRACE_LEVEL_INFO, string("IpadmChassisObjectManager::postboot ipv6 address/prefix") + str_ipaddr6 + string("/") + prefix6);

			int rc = -1;
			rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
			if (rc)
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::postboot ipAdmLocalAddrIdGet() failed");
			}
			rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
			if (rc)
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::postboot ipAdmGwAddrIdGet() failed");
			}
			rc = ipAdmIpAddrGet(&current);
			if (rc)
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::postboot ipAdmIpAddrGet() failed");
			}
			/* If this is a pizzabox switch we shouldn't set the chassis ip. Just ignore the config */
			int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
			if (pizza) {

					trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::postboot Skipping Chassis Ip set on Pizzabox");
					if (pVipMo) {
  						trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::postboot deleting existing chassisObject for pizzabox");
  						delete pVipMo;
					}
					else {
  						trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::postboot chassisObject does not exists for Pizzabox");
					}
			}
			else {
						int eth_idx = -1;
						eth_idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET);
						if (eth_idx < 0)
						{
							trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::postboot eth_idx < 0");
						}
						else
						{	
							//Change current
							SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr.s_addr = inet_addr(str_ipaddr.c_str());
							current.local_prefixes[eth_idx] = prefix;
							current.local_flags[eth_idx] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);
						}

						int eth_idx6 = -1;
						eth_idx6 = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET6);
						if (eth_idx6 < 0)
						{
							trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::postboot eth_idx6 < 0");
						}
						else
						{
							struct in6_addr addr6;
							memset(&addr6, 0, sizeof(struct in6_addr));
							if (!inet_pton(AF_INET6, str_ipaddr6.c_str(), &addr6)) 
							{
								trace (TRACE_LEVEL_INFO, string ("IpadmChassisObjectManager::postboot :inet_pton() failed"));
							}
							else
							{
								trace (TRACE_LEVEL_INFO, string ("IpadmChassisObjectManager::postboot :inet_pton() success"));
								if (!IN6_IS_ADDR_UNSPECIFIED(&addr6)) {
									SOCKADDR_IN6P(current.local_addrs[eth_idx6])->sin6_addr = addr6;
								}
							}
							//Change current
							current.local_prefixes[eth_idx6] = prefix;
							current.local_flags[eth_idx6] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);
						}
					
				rc = ipAdmIpAddrSet(&current, 0, 0);
				if (rc)
				{
					trace (TRACE_LEVEL_ERROR, string("IpadmChassisObjectManager::postboot ipAdmIpAddrSet() failed rc = ") + rc);
				}
			}
		}

	if (pResults) {
		trace (TRACE_LEVEL_DEBUG, string ("IpadmChassisObjectManager::postboot : Deleting pResults"));
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}
		pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForPostBootPhase->callback ();
	}

	void IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler (IpadmChassisIpConfigMessage *pIpadmChassisIpConfigMessage)
	{
		trace (TRACE_LEVEL_INFO, string ("IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler : IP address config message"));
		ResourceId status = WAVE_MESSAGE_ERROR;


		UI32 thisNodeSwdbNumber = DcmToolKit::getSwBDModel ();

		if ((SWBD_M4 != thisNodeSwdbNumber) && (SWBD_M8 != thisNodeSwdbNumber) && (SWBD_M16 != thisNodeSwdbNumber))
		{
			tracePrintf (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler : thisNodeSwdbNumber [%u]. Command Not Supported.", thisNodeSwdbNumber);
			pIpadmChassisIpConfigMessage->setCompletionStatus (IPADM_NO_SUPPORT_ON_PIZZA);
			reply (pIpadmChassisIpConfigMessage);
			return;

		}
	
		string ipAddress = pIpadmChassisIpConfigMessage->getVirtualIpAddress();
		IpadmAddressType addrType = pIpadmChassisIpConfigMessage->getIpAddrFlag();

		trace (TRACE_LEVEL_INFO, string("IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler address type") + ipAddress + string(" ") + addrType);
		
    		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();

		ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
		struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
		ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
		addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
		ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
		struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
		addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
		ipadm_ip_get_set_t current = {
			0, /* num_locals */
			local_addr_ids,
			local_addrs,
			local_prefixes,
			local_flags,
			0, /* num_gws */
			gw_addr_ids,
			gw_addrs,
			gw_flags,
			0 /* dhcp_flag */
		};

		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		int rc = -1;
		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc)
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler ipAdmLocalAddrIdGet() failed");
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc)
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler ipAdmGwAddrIdGet() failed");
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc)
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler ipAdmIpAddrGet() failed");
		}
		int eth_idx = -1;

		if(addrType == IPADM_ADDR_IPV6)	
			eth_idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET6);
		else
			eth_idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET);
		if (eth_idx < 0)
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler eth_idx < 0");
			return;
		}

		if (!ipAddress.compare("")) {
			current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);
		} else {
			string str_prefix, str_ipaddr;
			size_t pos = ipAddress.find("/");
			str_ipaddr = ipAddress.substr(0, pos);
			str_prefix = ipAddress.substr(pos+1);
			int prefix = atoi(str_prefix.c_str());

		trace (TRACE_LEVEL_INFO, string("IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler address/prefix") + str_ipaddr + string("/") + prefix);


		//Change current
		if(addrType == IPADM_ADDR_IPV6)	
		{
			struct in6_addr addr;
			memset(&addr, 0, sizeof(struct in6_addr));
			if (!inet_pton(AF_INET6, str_ipaddr.c_str(), &addr)) 
			{
				trace (TRACE_LEVEL_INFO, string ("IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler :inet_pton() failed"));
				return;
			}
			else
			{
				trace (TRACE_LEVEL_INFO, string ("IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler :inet_pton() success"));
				if (!IN6_IS_ADDR_UNSPECIFIED(&addr)) {
					SOCKADDR_IN6P(current.local_addrs[eth_idx])->sin6_addr = addr;
				}
			}
		}
		else
		{
			SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr.s_addr = inet_addr(str_ipaddr.c_str());
		}
		current.local_prefixes[eth_idx] = prefix;
		current.local_flags[eth_idx] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);
		}

		rc = ipAdmIpAddrSet(&current, 0, 0);
		if (rc)
		{
			trace (TRACE_LEVEL_ERROR, string("IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler ipAdmIpAddrSet() failed rc = ") + rc);
				pIpadmChassisIpConfigMessage->setCompletionStatus (WRC_IPADM_INTERNAL_ERROR);
				reply (pIpadmChassisIpConfigMessage);
				return;
		}
    		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, IpadmChassisManagedObject::getClassName ());

		if (NULL == pResults)
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler : No such managed object");
			pIpadmChassisIpConfigMessage->setCompletionStatus (status);
			reply (pIpadmChassisIpConfigMessage);
			return;
		}
		else
		{
			startTransaction ();
			if (pResults->size() == 0)
			{
				trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler : pResults size 0");
				IpadmChassisManagedObject *pVipMo = new IpadmChassisManagedObject (dynamic_cast<IpadmChassisObjectManager *> (getPWaveObjectManager ()));
				prismAssert(pVipMo != NULL, __FILE__, __LINE__);
				if(addrType == IPADM_ADDR_IPV6)	
					pVipMo->setVipV6Address (ipAddress);
				else
					pVipMo->setVipAddress (ipAddress);
			}
			else
			{
				trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler : pResults exists, changing ");
				IpadmChassisManagedObject *pVipMo = dynamic_cast<IpadmChassisManagedObject *> ((*pResults)[0]);
				prismAssert(pVipMo != NULL, __FILE__, __LINE__);
				
				if(pIpadmChassisIpConfigMessage->getIpAddrFlag() == IPADM_ADDR_IPV6)	
					pVipMo->setVipV6Address (ipAddress);
				else
					pVipMo->setVipAddress (ipAddress);
				updateWaveManagedObject (pVipMo);
			}
			
			status = commitTransaction ();
			if (FRAMEWORK_SUCCESS == status)
			{
				trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler : Commit to DB Success");
				status = WAVE_MESSAGE_SUCCESS;		
			}
			else 
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpConfigMessageHandler: Commit To DB failed");
			}
		}
#if 0
		pIpadmChassisIpConfigMessage->setCompletionStatus (status);
		reply (pIpadmChassisIpConfigMessage);

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
#endif
		
		pIpadmChassisIpConfigMessage->setCompletionStatus (status);
		reply (pIpadmChassisIpConfigMessage);

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);

		return;
	}

	void IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler (IpadmChassisIpDeleteMessage *pIpadmChassisIpDeleteMessage)
	{
		trace (TRACE_LEVEL_INFO, string ("IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler : Remove VIP/VIPV6"));
		ResourceId status = WAVE_MESSAGE_SUCCESS;

        	UI32 thisNodeSwdbNumber = DcmToolKit::getSwBDModel ();
       	 	if ((SWBD_M4 == thisNodeSwdbNumber) || (SWBD_M8 == thisNodeSwdbNumber) || (SWBD_M16 == thisNodeSwdbNumber))
        	{
			ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
			struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
			ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
			addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
			ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
			struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
			addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
			ipadm_ip_get_set_t current = {
				0, /* num_locals */
				local_addr_ids,
				local_addrs,
				local_prefixes,
				local_flags,
				0, /* num_gws */
				gw_addr_ids,
				gw_addrs,
				gw_flags,
				0 /* dhcp_flag */
			};
	
			current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
			current.num_gws = MAX_GW_ADDR_TABLE_SIZE;
	
			int rc = -1;
			rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
			if (rc)
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler ipAdmLocalAddrIdGet() failed");
			}
			rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
			if (rc)
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler ipAdmGwAddrIdGet() failed");
			}
			rc = ipAdmIpAddrGet(&current);
			if (rc)
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler ipAdmIpAddrGet() failed");
			}
	
			int eth_idx = -1;
			IpadmAddressType addrType = pIpadmChassisIpDeleteMessage->getIpAddrFlag();
			if(addrType == IPADM_ADDR_IPV6)
			{
				eth_idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET6);
				if (eth_idx < 0)
				{
					trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler ipv6 eth_idx < 0");
					pIpadmChassisIpDeleteMessage->setCompletionStatus (WRC_IPADM_IPV6_ADDRCLR_ERROR);
					reply (pIpadmChassisIpDeleteMessage);
					return;
				}
			}
			else
			{
				eth_idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET);
				if (eth_idx < 0)
				{
					trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler eth_idx < 0");
					pIpadmChassisIpDeleteMessage->setCompletionStatus (WRC_IPADM_IPV4_ADDRCLR_ERROR);
					reply (pIpadmChassisIpDeleteMessage);
					return;
				}
			}
	
			//Change current
			current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);
			
			rc = ipAdmIpAddrSet(&current, 0, 0);
			if (rc)
			{
				trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler ipAdmIpAddrSet() failed");
				pIpadmChassisIpDeleteMessage->setCompletionStatus (WRC_IPADM_IPV6_ADDRSET_ERROR);
				reply (pIpadmChassisIpDeleteMessage);
				return;
			}
		}
		trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler ipAdmIpAddrSet Succeeded!");
		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmChassisManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
		if (NULL == pResults) 
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler : No such managed object");
			pIpadmChassisIpDeleteMessage->setCompletionStatus (status);
			reply (pIpadmChassisIpDeleteMessage);
			delete synchronousQueryContext;
			return;
		} else if (pResults->size() == 0) {
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler : pResults size 0");
			pIpadmChassisIpDeleteMessage->setCompletionStatus (status);
			reply (pIpadmChassisIpDeleteMessage);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
			delete synchronousQueryContext;
			return;
		}

		startTransaction ();
		IpadmChassisManagedObject *pVipMo = dynamic_cast<IpadmChassisManagedObject *> ((*pResults)[0]);
		prismAssert(pVipMo != NULL, __FILE__, __LINE__);
		//delete pVipMo;
		updateWaveManagedObject (pVipMo);
		if(pIpadmChassisIpDeleteMessage->m_AddrType == IPADM_ADDR_IPV6)
                        pVipMo->setVipV6Address ("");
                if(pIpadmChassisIpDeleteMessage->m_AddrType == IPADM_ADDR_IPV4)
                        pVipMo->setVipAddress ("");

		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			trace (TRACE_LEVEL_INFO, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler : Commit to DB Success");
			status = WAVE_MESSAGE_SUCCESS;		
		}
		else 
		{
			trace (TRACE_LEVEL_ERROR, "IpadmChassisObjectManager::IpadmChassisIpDeleteMessageHandler : Commit To DB failed");
		}
		pIpadmChassisIpDeleteMessage->setCompletionStatus (status);
		reply (pIpadmChassisIpDeleteMessage);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
		delete synchronousQueryContext;
		return;
	}

	int IpadmChassisObjectManager::locate_local_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family)
	{
		int i;

		for (i = 0; i < p_getset->num_locals; ++i) {
			ipadm_local_addr_id_t *p = &(p_getset->local_addr_ids[i]);
			if ((p->entity_type == entity) &&
				(p->entity_instance == entity_instance) &&
				(p->interface_type == interface) &&
				(p->addr_family == addr_family)) {
				// Only worry about interface matching for non-blades
				if ((entity != BLADE_ENTITY) ||
					(p->interface_instance == interface_instance))
					return (i);
			}
		}
		return (-1);
	}

}
