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
 *   Author : cshah                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Global/IpadmGlobalObjectManager.h"
#include "Ipadm/Local/IpadmLocalManagedObject.h"
#include "Ipadm/Local/IpadmIpaddressConfigMessage.h"
#include "Ipadm/Local/IpadmIpv6RouteLocalManagedObject.h"
#include "Ipadm/Local/IpadmLinkSpeedConfigMessage.h"
#include "Ipadm/Local/IpadmGatewayConfigMessage.h"
#include "Ipadm/Local/IpadmIcmpUnreachableConfigMessage.h"
#include "Ipadm/Local/IpadmIcmpEchoreplyConfigMessage.h"
#include "Ipadm/Local/IpadmIcmpRatelimitingConfigMessage.h"
#include "Ipadm/Local/IpadmIcmpv6UnreachableConfigMessage.h"
#include "Ipadm/Local/IpadmIcmpv6EchoreplyConfigMessage.h"
#include "Ipadm/Local/IpadmIcmpv6RatelimitingConfigMessage.h"
#include "Ipadm/Local/IpadmDhcpConfigMessage.h"
#include "Ipadm/Local/IpadmIpv6AddressConfigMessage.h"
#include "Ipadm/Local/IpadmIpv6DhcpConfigMessage.h"
#include "Ipadm/Local/IpadmIpv6AutoconfigMessage.h"
#include "Ipadm/Local/IpadmIpv6RouteconfigMessage.h"
#include "Ipadm/Local/IpadmRemoveMessage.h"
#include "Ipadm/Local/IpadmLocalMessageContext.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmResourceIds.h"
#include "DcmCore/DcmToolKit.h"
#include "vcs.h"
#include "Utils/Layer3LicenseUtil.h"
#include "Utils/Layer3LicenseUtil.h"
#include "unistd.h"
#include "fcntl.h"
#include <sys/wait.h>
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Nsm/NsmClientCreateIntIpConfigSPMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Ipadm/Local/IpadmChassisManagedObject.h"
#include "Ipadm/Local/IpadmChassisIpConfigMessage.h"
extern "C" {
#include <hasm/hasm.h>
}
#define READ    0
#define WRITE   1
#define ICMP_FILE "/etc/icmp.conf"
#define ICMP_UNREACHABLE "/proc/sys/net/ipv4/conf/eth0/ip_unreachable"
#define ICMP_ECHOIGNORE	"/proc/sys/net/ipv4/conf/eth0/icmp_echo_ignore"
#define ICMP_RATELIMIT	"/proc/sys/net/ipv4/conf/eth0/icmp_rlimit_enable"
#define ICMP_RATEMASK	"/proc/sys/net/ipv4/icmp_ratemask"
#define ICMP_RATEMASK_ALL (0x7F939) /* see inlcude/linux/icmp.h */
#define ICMPV6_UNREACHABLE	"/proc/sys/net/ipv6/conf/eth0/ipv6_unreach"
#define ICMPV6_ECHOIGNORE	"/proc/sys/net/ipv6/conf/eth0/echo_ignore"
#define ICMPV6_RATELIMIT	"/proc/sys/net/ipv6/icmp/ratelimit"
namespace DcmNs
{

    IpadmLocalObjectManager::IpadmLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        IpadmLocalManagedObject    IpadmLocalManagedObject    (this);
	IpadmIpv6RouteLocalManagedObject    IpadmIpv6RouteLocalManagedObject (this);
        IpadmLocalManagedObject.setupOrm ();
	IpadmIpv6RouteLocalManagedObject.setupOrm ();
		associateWithVirtualWaveObjectManager (IpadmGlobalObjectManager::getInstance ());
        addManagedClass (IpadmLocalManagedObject::getClassName ());
	addManagedClass (IpadmIpv6RouteLocalManagedObject::getClassName ());
        addOperationMap (IPADMIPADDRESSCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler));
        addOperationMap (IPADMGATEWAYCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler));
        addOperationMap (IPADMICMPUNREACHABLECONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler));
        addOperationMap (IPADMICMPECHOREPLYCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIcmpEchoreplyConfigMessageHandler));
        addOperationMap (IPADMICMPRATELIMITINGCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIcmpRatelimitingConfigMessageHandler));
        addOperationMap (IPADMICMPV6UNREACHABLECONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler));
        addOperationMap (IPADMICMPV6ECHOREPLYCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIcmpv6EchoreplyConfigMessageHandler));
        addOperationMap (IPADMICMPV6RATELIMITINGCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIcmpv6RatelimitingConfigMessageHandler));
	addOperationMap (IPADMIPV6ROUTECONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIpv6RouteconfigMessageHandler));
        addOperationMap (IPADMDHCPCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmDhcpConfigMessageHandler));
        addOperationMap (IPADMIPV6ADDRESSCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler));
        addOperationMap (IPADMIPV6AUTOCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIpv6AutoconfigMessageHandler));
        addOperationMap (IPADMREMOVECONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmRemoveMessageHandler));
        addOperationMap (IPADMLINKSPEEDCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmLinkSpeedConfigMessageHandler));
	/* we use IpadmConfigMessageHandler for DN and NS */
        addOperationMap (IPADMDNCFG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmConfigMessageHandler));
        addOperationMap (IPADMNSCFG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmConfigMessageHandler));
        addOperationMap (IPADMIPV6DHCPCONFIG, reinterpret_cast<PrismMessageHandler> (&IpadmLocalObjectManager::IpadmIpv6DhcpConfigMessageHandler));
    }

    IpadmLocalObjectManager::~IpadmLocalObjectManager ()
    {
    }

    IpadmLocalObjectManager  *IpadmLocalObjectManager::getInstance()
    {
        static IpadmLocalObjectManager *pIpadmLocalObjectManager = new IpadmLocalObjectManager ();

        WaveNs::prismAssert (NULL != pIpadmLocalObjectManager, __FILE__, __LINE__);

        return (pIpadmLocalObjectManager);
    }

    string  IpadmLocalObjectManager::getClassName()
    {
        return ("Ipadm");
    }

    PrismServiceId  IpadmLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *IpadmLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case IPADMIPADDRESSCONFIG :
                pPrismMessage = new IpadmIpaddressconfigMessage ();
                break;
            case IPADMGATEWAYCONFIG :
                pPrismMessage = new IpadmGatewayconfigMessage ();
                break;
            case IPADMICMPUNREACHABLECONFIG:
                pPrismMessage = new IpadmIcmpUnreachableConfigMessage();
                break;
            case IPADMICMPECHOREPLYCONFIG:
                pPrismMessage = new IpadmIcmpEchoreplyConfigMessage();
                break;
            case IPADMICMPRATELIMITINGCONFIG:
                pPrismMessage = new IpadmIcmpRatelimitingConfigMessage();
                break;
            case IPADMICMPV6UNREACHABLECONFIG:
                pPrismMessage = new IpadmIcmpv6UnreachableConfigMessage();
                break;
            case IPADMICMPV6ECHOREPLYCONFIG:
                pPrismMessage = new IpadmIcmpv6EchoreplyConfigMessage();
                break;
            case IPADMICMPV6RATELIMITINGCONFIG:
                pPrismMessage = new IpadmIcmpv6RatelimitingConfigMessage();
                break;
            case IPADMIPV6ROUTECONFIG :
                pPrismMessage = new IpadmIpv6RouteconfigMessage ();
                break;
            case IPADMDHCPCONFIG :
                pPrismMessage = new IpadmDhcpConfigMessage ();
                break;
			case IPADMIPV6ADDRESSCONFIG:
                pPrismMessage = new IpadmIpv6AddressConfigMessage ();
				break;
			case IPADMIPV6AUTOCONFIG:
                pPrismMessage = new IpadmIpv6AutoconfigMessage ();
				break;
			case IPADMREMOVECONFIG:
				pPrismMessage = new IpadmRemoveMessage ();
				break;
			case IPADMLINKSPEEDCONFIG:
				pPrismMessage = new IpadmLinkSpeedConfigMessage ();
				break;
            case IPADMDNCFG:
            case IPADMNSCFG:
                pPrismMessage = new IpadmLocalDnsCfgMessage ();
                break;
			case IPADMIPV6DHCPCONFIG:
                pPrismMessage = new IpadmIpv6DhcpConfigMessage ();
				break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IpadmLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((IpadmLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IpadmLocalManagedObject(this);
        }
        else if ((IpadmIpv6RouteLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IpadmIpv6RouteLocalManagedObject(this);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "IpadmLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	/*
	 * this function searches the get/set at p_getset for a local interface of
	 * the specified entity, interface type, entity instance and address family
	 * and returns the local address table index containing the specified
	 * local interface.
	 *
	 * returns: >= 0 => valid local index
	 *	<0 => some error occurred (specified interface not found)
	 */

	int IpadmLocalObjectManager::locate_local_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family)
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

	/*
	 * this function searches the get/set at p_getset for a gateway of
	 * the specified instance and returns the gateway
	 * address table index containing the specified gateway.
	 *
	 * returns: >= 0 => valid gateway index
	 *	<0 => some error occurred (specified gateway not found)
	 */

	int IpadmLocalObjectManager::locate_gw_idx(ipadm_ip_get_set_t *p_getset, ipadm_host_t host_type, int host_instance, int addr_family)
	{
		int i;

		for (i = 0; i < p_getset->num_gws; ++i) {
			if ((p_getset->gw_addr_ids[i].host_instance == host_instance) &&
				(p_getset->gw_addr_ids[i].host_type == host_type) &&
				(p_getset->gw_addr_ids[i].addr_family == addr_family)) {
				return (i);
			}
		}
		return (-1);
	}
	void IpadmLocalObjectManager::RouteExecCommand(char *cmdline, char* errBuf)
	{
		int outfd[2];
		pid_t pid;
		int flags;
		int status = 0;
		int rc = 0;
		int errfd[2];

		tracePrintf(TRACE_LEVEL_INFO, "RouteWorker::RouteExecCommand cmd= %s", cmdline);
		if (pipe(outfd) != 0) {
			tracePrintf(TRACE_LEVEL_ERROR, "Unable execute the command errcode: 1\n");
			return;
		}

		pipe(errfd);
		pid = fork();

		if (pid < 0) {
			tracePrintf(TRACE_LEVEL_ERROR, "Unable execute the command errcode: 2\n");
			return;
		} else if (pid == 0) {
			close(outfd[READ]);
			dup2(outfd[WRITE], WRITE);
			dup2 (errfd[1], STDERR_FILENO);
			close(errfd[0]);
			flags = fcntl(WRITE, F_GETFD);
			if (flags == -1) {
				tracePrintf(TRACE_LEVEL_ERROR, "RouteExecCommand : Error in getting flag for fd\n");
			}

			flags |= FD_CLOEXEC;
			if (fcntl(WRITE, F_SETFD, flags) == -1) {
				tracePrintf(TRACE_LEVEL_ERROR, "RouteExecCommand : Error in setting flag for fd\n");
			}

			execl("/bin/sh", "sh", "-c", cmdline, NULL);
			close(outfd[WRITE]);
			exit (-1);
		} else {
			close(outfd[WRITE]);
			/* Set the fd to non blocking to make sure we read the user abort signal as and when it happens */
			fcntl(outfd[READ], F_SETFL, O_NONBLOCK);
			close(errfd[1]);
			read(errfd[0], errBuf, 4096);
			WaveNs::tracePrintf(TRACE_LEVEL_DEBUG, "RouteWorker::RouteExecCommand error=%s", errBuf);
			waitpid(pid, &status, 0);   // Get child process status so that we dont leave Zombies

			close(errfd[0]);
			if ((rc = close(outfd[READ])) != 0) {
				tracePrintf(TRACE_LEVEL_ERROR, "RouteActionpointWorker::RouteExecCommand pclose fail:");
			}
		}
	}

	void IpadmLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
		ResourceId status               = FRAMEWORK_ERROR;
		UI32 mappedId = DcmToolKit::getLocalMappedId();	
		
		string ip_interface;
		IpV4Address ip_gateway;
		string ipv6_address ("fe80::21e:bff:febf:47d4/64");
		int rc = 0;

		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		current.local_addr_ids = local_addr_ids;
		current.local_addrs = local_addrs;
		current.local_prefixes = local_prefixes;
		current.local_flags = local_flags;
		current.gw_addr_ids = gw_addr_ids;
		current.gw_addrs = gw_addrs;
		current.gw_flags = gw_flags;

		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmLocalAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmIpAddrGet() returned ") + rc);
			//return;
		}
		
		//if pizza
		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
		int end_cp_num = 1;

		if (pizza)
			end_cp_num = 0;

		IpadmLocalManagedObject *pInterface[2];
		int j;

		for (j = 0; j <= end_cp_num; j++)
		{
			char *addr_c;
			int idx = 0;
			string ip_str;
													
			//	Now we have the ipaddr, mask and gateway - convert them to sting formats
			//	1.	ipaddr
			if (pizza)
				idx = locate_local_idx(&current, SWITCH_ENTITY, j, ETHERNET_INTERFACE, 0, AF_INET);
			else 
				idx = locate_local_idx(&current, CP_ENTITY, j, ETHERNET_INTERFACE, 0, AF_INET);

			if (idx < 0) {
				trace (TRACE_LEVEL_ERROR, string ("could not locate ethernet interface"));
			} else {
				//show_ip_addr(SHOW_FMT_STRING, ETH_ADDR_LABEL, &current, idx);
				//show_mask(SHOW_FMT_STRING, ETH_MASK_LABEL, &current, idx);
				if ((current.local_flags[idx] & IPADM_ADDR_NONE) == IPADM_ADDR_NONE) {
					ip_str = "";
				}
				else {
					addr_c = inet_ntoa(SOCKADDR_INP(current.local_addrs[idx])->sin_addr);
					trace (TRACE_LEVEL_INFO, string ("got this IP address -- ") + addr_c);
					ip_str = addr_c;
				}
				//	2.	mask
				int prefix = 0;
				if ((current.local_flags[idx] & IPADM_MASK_NONE) == IPADM_MASK_NONE) {
					ip_interface = ip_str;
				}
				else {
					prefix = current.local_prefixes[idx];

					char buf1[10];
					sprintf(buf1, "/%d", prefix);
					
					ip_interface = (ip_str + buf1);
				}
			}

			//	3. gateway	
			// locate and display the gateway IP address information
			idx = locate_gw_idx(&current, SPECIFIC_CP_HOST, j, AF_INET);
			if (idx < 0) {
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::install :could not locate gateway");
			} else {
				if ((current.gw_flags[idx] & IPADM_ADDR_NONE) == IPADM_ADDR_NONE) {
					ip_str = "0.0.0.0";
				}
				else {
					addr_c = inet_ntoa(SOCKADDR_INP(current.gw_addrs[idx])->sin_addr);
					ip_str = addr_c;
				}
				ip_gateway.fromString(ip_str);
			}
			trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::install got these values * ") + ip_interface + string (" ") +  ip_gateway.toString());
		
			//DHCP and stateless autoconfig
			//By default we will let the dhcp, dhcpv6 and autoconfig to come disabled on default config 
			bool dhcp = false;
			bool autoconfig = false;
			bool dhcpv6 = false;
		
	
			//ipv6 local static address
			char buf[100];
			int ipv6_present = 0;
			if (pizza)
				idx = locate_local_idx(&current, SWITCH_ENTITY, j, ETHERNET_INTERFACE, 0, AF_INET6);
			else 
				idx = locate_local_idx(&current, CP_ENTITY, j, ETHERNET_INTERFACE, 0, AF_INET6);

			if (idx < 0) {
				trace (TRACE_LEVEL_ERROR, string ("could not locate any static ipv6 address"));
			} else {
				addr_flags_t flags;
				
				/* make sure this is static address index */
				flags = current.local_flags[idx];
				//If no address is set, skip this
				if( flags & (IPADM_ADDR_AUTO_STATELESS |IPADM_ADDR_DHCPV6 )){	
					trace (TRACE_LEVEL_ERROR, string ("could not locate any static ipv6 address index"));
				} else {
					/* if ipv6 address is null */
					if ((flags & IPADM_ADDR_NONE) == IPADM_ADDR_NONE) {
						ipv6_address = "";
					}else {
						const char *rcc = inet_ntop(AF_INET6, &(SOCKADDR_IN6P(current.local_addrs[idx])->sin6_addr), buf, sizeof (buf));
						if (rcc == NULL) {
							trace (TRACE_LEVEL_ERROR, string ("inet_ntop() failed "));
							ipv6_address = "";
						}
						ipv6_address.assign(buf);
					}
					ipv6_present = 1;
				}
				/* address prefix */
				int prefix6 = 0;
				string prefix6string = "";
				if ((flags & IPADM_MASK_NONE) == IPADM_MASK_NONE) {
					trace (TRACE_LEVEL_ERROR, string ("could not locate any static ipv6 address prefix"));
				}
				else {
					prefix6 = current.local_prefixes[idx];

					char bufp6[10];
					sprintf(bufp6, "/%d", prefix6);
					prefix6string.assign(bufp6);
					ipv6_address += prefix6string;
					trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::install got these values * ") + ipv6_address);
				}
			}

			//Change names of managed object
			startTransaction ();
			pInterface[j] = new IpadmLocalManagedObject (this);
		
			//Do set of key here
			pInterface[j]->setIPv4Address (ip_interface);
			pInterface[j]->setIPv4Gateway(ip_gateway);
			pInterface[j]->setDhcp(dhcp);
			if (ipv6_present)
				pInterface[j]->setIPv6Address(ipv6_address);
			pInterface[j]->setAutoconfig(autoconfig);
			pInterface[j]->setIpv6Dhcp(dhcpv6);
			//Defect fix:382328 To keep backward compatibilty for pizza box systems.  
			if (!pizza)
				pInterface[j]->setId(mappedId + string ("/") + (j + 1));
			else 
	 			pInterface[j]->setId(mappedId + string ("/0"));

			pInterface[j]->set_link_speed(M_LINK_SPEED_AUTO);

			/* icmp control */
			pInterface[j]->setUnreachable(true);
			pInterface[j]->setEchoreply(true);
			pInterface[j]->setRatelimiting(1000);

			/* icmpv6 control */
			pInterface[j]->setV6Unreachable(true);
			pInterface[j]->setV6Echoreply(true);
			pInterface[j]->setV6Ratelimiting(1000);
			system("echo 1 0 100 1 0 100 > " ICMP_FILE ";echo 1 0 100 1 0 100 > /mnt" ICMP_FILE);

			//Do all sets of mask here
			updateWaveManagedObject (pInterface[j]);
			status = commitTransaction ();

			if (FRAMEWORK_SUCCESS == status)
			{
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::install: Installed Management Interface for demo");

				status = WAVE_MESSAGE_SUCCESS;
			}
			else
			{
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::install: Installed Management Interface for demo FAILED");
				prismAssert (false, __FILE__, __LINE__);
			}
			delete pInterface[j];
		}

		pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	    pWaveAsynchronousContextForBootPhases->callback ();
	}

	int IpadmLocalObjectManager::setLinkSpeed(char* ifname, LinkSpeed speed)
	{
		string cmd;

		cmd.assign("/fabos/libexec/ethmode ");
		cmd.append(ifname);

		trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::setLinkSpeed : ifname=") + ifname + string(" speed=") + speed);

		/* bitmask settings are available in fabos/src/utils/sys/ifmode_to_ethmode.sh */
		switch(speed) 
		{
			case M_LINK_SPEED_AUTO :
				cmd.append(" 1");   /* auto negotiate  = 1 */
				cmd.append(" 0");   /* Mode = 0 */
				break;
			case M_LINK_SPEED_10  :
				cmd.append(" 0");   /* Auto negotiation = 0, means forced */
				cmd.append(" 2");   /* 0x02 : 10 Mbps, Full Duplex */
				break;
			case M_LINK_SPEED_100  :
				cmd.append(" 0");   /* Auto negotiation = 0. means forced */
				cmd.append(" 8");   /* 0x08 : 100 Mbps, Full Duplex */
				break;
#if 0
			/* ethmode says GigE modes can not be forced */
			case M_LINK_SPEED_1000F :
				cmd.append(" 0");
				cmd.append(" 32");  /* 0x20 : 1Gbps, Full Duplex */
				break;
#endif
			default :
				return (-1);
				break;
		}
		/*Redirecting the error message (if any) that appears on running above command to a new file*/
		cmd.append(" 2>/tmp/ifmode.txt");
		
		/* Execute the ethmode */
		FILE* fd = popen(cmd.c_str(), "r");
		char buf[256];
		if (fd) {
			if(fgets(buf, 256, fd)) {
				fclose(fd);
				trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::setLinkSpeed : ethmode error = ") + buf);
				return (-1);
			}
			fclose(fd);
			return (0);
		}
		return (-1);
	}


    void  IpadmLocalObjectManager::IpadmLinkSpeedConfigMessageHandler( IpadmLinkSpeedConfigMessage *pIpadmLinkSpeedConfigMessage)
    {
	string id = pIpadmLinkSpeedConfigMessage->getSwitchid ();
	LinkSpeed speed = pIpadmLinkSpeedConfigMessage->getLinkSpeed ();
	trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmLinkSpeedConfigMessageHandler : ") + id + "LinkSpeed : " + speed);

	WaveManagedObjectSynchronousQueryContext *syncQueryContext =
		new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
	syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
	vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
	if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
	{
		trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
		pIpadmLinkSpeedConfigMessage->setCompletionStatus (WRC_IPADM_INTERNAL_ERROR);
		reply (pIpadmLinkSpeedConfigMessage);
		delete syncQueryContext;
		return;
	}

	IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
	delete syncQueryContext;

	/* FIXME : How do we get interface name from switchid? */
	if(setLinkSpeed((char *) "eth0", speed) != 0) {
		pIpadmLinkSpeedConfigMessage->setCompletionStatus (WRC_IPADM_SPEED_ERROR);
		trace (TRACE_LEVEL_ERROR, string("IpadmLocalObjectManager::LinkSpeedConfigMessageHandler : ethmode error"));
		reply (pIpadmLinkSpeedConfigMessage);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
		return;
	}

#if 0
	FILE* fd = popen("/bin/sync", "r");
	if (fd) {
		fclose(fd);
	}
#endif
	/*Reading the file to check if any error has been thrown */	
	FILE* fd = fopen("/tmp/ifmode.txt", "r");
	if(fd != NULL) {
		char buf[512] = {0};

		/*File remains empty if there is no error. Later removing the temporarily created file*/
		if(fread(buf, 1, 512, fd) > 0) {
			pIpadmLinkSpeedConfigMessage->setCompletionStatus (WRC_IPADM_SPEED_ERROR);
			reply (pIpadmLinkSpeedConfigMessage);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			fclose(fd);
			remove("/tmp/ifmode.txt"); 
			return;
		}
		fclose(fd);
		remove("/tmp/ifmode.txt"); 
	}

	ResourceId status = WAVE_MESSAGE_SUCCESS;
		
	startTransaction ();
	updateWaveManagedObject (pIpadmManagedObject);

	pIpadmManagedObject->set_link_speed (speed);

	status = commitTransaction ();
	if (FRAMEWORK_SUCCESS == status)
	{
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmLinkSpeedConfigMessageHandler : Commit to DB Success");
		status = WAVE_MESSAGE_SUCCESS;		
	}
	else 
	{
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmLinkSpeedConfigMessageHandler : Commit to DB Failed");
	}

	pIpadmLinkSpeedConfigMessage->setCompletionStatus (status);
	reply (pIpadmLinkSpeedConfigMessage);
	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
    }

    void  IpadmLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	{

		string ss;
		char errBuf[4096] = {'\0'};
		/* For IPv6 Route */
		{
			SI8 PassNum  = pWaveAsynchronousContextForPostBootPhase->getPassNum();
			trace (TRACE_LEVEL_ERROR, string("IpadmLocalObjectManager::postboot  ipv6 Pass ") + PassNum);

			trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager=== Route::postboot ipv6 ipAdmLocalAddrIdGet() returned ") );
			WaveManagedObjectSynchronousQueryContext *syncQueryContext =
				new WaveManagedObjectSynchronousQueryContext(IpadmIpv6RouteLocalManagedObject::getClassName());

			vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
			if ((NULL != pResultsRollback) && (pResultsRollback->size() != 0))
			{

				// Can handle atmax 20000 entries from wavedatabase
				IpadmIpv6RouteLocalManagedObject *pIpadmIpv6RouteManagedObject;
				for (UI32 cp_num = 0; cp_num < pResultsRollback->size();cp_num++)
				{
					pIpadmIpv6RouteManagedObject = dynamic_cast<IpadmIpv6RouteLocalManagedObject *> ((*pResultsRollback)[cp_num]);
	
					if (pIpadmIpv6RouteManagedObject != NULL) {
						IpV6AddressNetworkMask dest = pIpadmIpv6RouteManagedObject->getDest();
						IpV6Address nhIp = pIpadmIpv6RouteManagedObject->getNhIp();
						UI32 routeType = pIpadmIpv6RouteManagedObject->getRouteType();
	
						if (routeType == MGMNT_ROUTE_NEXTHOP) {
							ss = string("ip -6 route ") + string("add") + " " + string(dest.toString()) + string(" via ") + string(nhIp.toString()) + string(" dev eth0"); 
						} else {
							ss = string("ip -6 route ") + string("add") + " " + string(dest.toString()) + string(" dev eth0");
						}
	
						// define wavemessage success and do the routeexec here
						char *id1 = (char *)ss.c_str();
						RouteExecCommand(id1, errBuf);
						trace (TRACE_LEVEL_DEBUG, "IpadmLocalObjectManager=== Route: IPv6: Done exec");
					} else {
						trace (TRACE_LEVEL_INFO, string("IpadmLocalObjectManager=== Route::postboot,ipv6  got ethmode= in else pIpadmRouteManagedObject[cp_num] is null"));
					}
				}

				delete syncQueryContext;
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			} else {
				trace (TRACE_LEVEL_INFO, string("IpadmLocalObjectManager=== Route::postboot,ipv6 pResultsRollback->size() is zero or pResultsRollback is null " ));
				delete syncQueryContext;
			}
		}

		SI8 PassNum  = pWaveAsynchronousContextForPostBootPhase->getPassNum();
		int eth_idx0 = 0, eth_idx1 = 0;
		UI32 cp_max = 0;
		string ip_addr0, ip_addr1;
		bool reset_addr = false;
		trace (TRACE_LEVEL_ERROR, string("IpadmLocalObjectManager::postboot  Pass ") + PassNum);
		
		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		current.local_addr_ids = local_addr_ids;
		current.local_addrs = local_addrs;
		current.local_prefixes = local_prefixes;
		current.local_flags = local_flags;
		current.gw_addr_ids = gw_addr_ids;
		current.gw_addrs = gw_addrs;
		current.gw_flags = gw_flags;

		/* apply dns configuration to backend */
		dnsConfigBoot();

		int rc = -1;
		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::postboot ipAdmLocalAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("IpadmLocalObjectManager::postboot ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::postboot ipAdmIpAddrGet() returned ") + rc);
			//return;
		}
		trace (TRACE_LEVEL_INFO, string("IpadmLocalObjectManager::postboot Entered with pass: ") + PassNum);
		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);

		if (pizza)
			cp_max = 1;
		else
			cp_max = 2;

	for (UI32 cp_num = 0; cp_num < cp_max; cp_num++)
	{
			UI32 mappedId = DcmToolKit::getLocalMappedId();	
			string id; 
			if (!pizza)
				id =  mappedId + string ("/") + (cp_num + 1);
			else 
				id = mappedId + string ("/0");

			WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
			syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
			vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
			if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
			{
				trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
				delete syncQueryContext;
				continue;	
			}	
			IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);


		if (pIpadmManagedObject != NULL) {
			//	Now replay the configuration to the ipadm daemon
			string ip_address = pIpadmManagedObject->getIPv4Address();
			IpV4Address ipv4_gateway = pIpadmManagedObject->getIPv4Gateway();
			bool dhcp = pIpadmManagedObject->getDhcp();
			string ipv6_address = pIpadmManagedObject->getIPv6Address();
			bool autoconfig = pIpadmManagedObject->getAutoconfig();
			bool dhcpv6 = pIpadmManagedObject->getIpv6Dhcp();
			LinkSpeed speed = pIpadmManagedObject->get_link_speed ();
			string mgmnt_name = pIpadmManagedObject->getId();
		
			string str_prefix, str_ipaddr;
			size_t pos;
			int prefix;
			trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : ") + "cp_num " + cp_num + " management interface" + mgmnt_name);
			if (dhcp == true) {
				current.dhcp_flag |= IPADM_DHCP_ENABLE;		//	On
				trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : ") + "cp_num " + cp_num + " management interface " + mgmnt_name + " DHCP Enable");
			} else {
				current.dhcp_flag &= ~IPADM_DHCP_ENABLE;	//	Off
				trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : ") + "cp_num " + cp_num + " management interface " + mgmnt_name + " DHCP Disable");
				
	//	int localId = DcmToolKit::getLocalMappedId();
				int eth_idx;
				if (pizza)
					eth_idx = locate_local_idx(&current, SWITCH_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET);
				else
					eth_idx = locate_local_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET);

				if (eth_idx < 0) {
					trace (TRACE_LEVEL_FATAL, string ("IpadmLocalObjectManager::postboot : eth_idx is <0 ") + eth_idx);
				} else {
					if (!ip_address.compare("")) {
						current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);	
						trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : none"));
					} else {
						pos = ip_address.find("/");
						str_ipaddr = ip_address.substr(0, pos);
						str_prefix = ip_address.substr(pos+1);

						trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : ") + "cp_num " + cp_num + "ip address" + ip_address + " " + " PREFIX ---- " + str_prefix);


						/* ipv4 management port addresses need to reset first before changing to new value. This is because new addresses may be owned by
						 * any other interface and address set may fail. Defect 398033 */

						if (!pizza && (SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr.s_addr != inet_addr(str_ipaddr.c_str()))) {
							reset_addr = true;
							trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : Address change noticed reset needed") + mgmnt_name);
							if ( cp_num == 0) {

								eth_idx0  = eth_idx;
								ip_addr0 = str_ipaddr;

							} else if ( cp_num == 1) {

								eth_idx1  = eth_idx;
								ip_addr1 = str_ipaddr;

							}

						}
						SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr.s_addr = inet_addr(str_ipaddr.c_str());
						
						prefix = atoi(str_prefix.c_str());
						current.local_prefixes[eth_idx] = prefix;
						current.local_flags[eth_idx] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);
					}
				}
				
				int gw_idx = locate_gw_idx(&current, SPECIFIC_CP_HOST, cp_num, AF_INET);
				if (gw_idx < 0) {
					trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::postboot :could not locate gateway idx");
				}
                 else {
				   if(SOCKADDR_INP(current.gw_addrs[gw_idx])->sin_addr.s_addr)
					current.gw_flags[gw_idx] &= ~IPADM_ADDR_NONE;
				   else
					current.gw_flags[gw_idx] |= IPADM_ADDR_NONE;
				   trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : Skipping gw address replay."));	
			   }		
			}

			if (dhcpv6 == true) {
				current.dhcp_flag |= IPADM_DHCPV6_ENABLE;       //  On
				trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : ") + "cp_num " + cp_num + " management interface " + mgmnt_name + " DHCPV6 Enable ");
			} else {
				current.dhcp_flag &= ~IPADM_DHCPV6_ENABLE;  //  Off
				trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : ") + "cp_num " + cp_num + " management interface " + mgmnt_name + " DHCPV6 Disable ");
			}	
			//Find the index
			int eth_idx;
			if (pizza)
				eth_idx = locate_local_idx(&current, SWITCH_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET6);
			else
				eth_idx = locate_local_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET6);

			if (eth_idx < 0) {
				trace (TRACE_LEVEL_FATAL, string ("IpadmLocalObjectManager::postboot : eth_idx is <0 ") + eth_idx);
			} else {
				if (!ipv6_address.compare("")) {
					current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);	
				} else {
					struct in6_addr addr;
					memset(&addr, 0, sizeof(struct in6_addr));

					pos = ipv6_address.find("/");
					str_ipaddr = ipv6_address.substr(0, pos);
					str_prefix = ipv6_address.substr(pos+1);
					prefix = atoi(str_prefix.c_str());
					
					if (!inet_pton(AF_INET6, str_ipaddr.c_str(), &addr)) { 
						trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::postboot : inet_pton() failed"));
					} else {
						trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::postboot : inet_pton() success"));	
						if (!IN6_IS_ADDR_UNSPECIFIED(&addr)) {
							SOCKADDR_IN6P(current.local_addrs[eth_idx])->sin6_addr = addr;
						}

						trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : ") + "cp_num " + cp_num + "ipv6 address" + str_ipaddr + " " + " PREFIX ---- " + str_prefix);

						current.local_prefixes[eth_idx] = (unsigned char) prefix;
						current.local_flags[eth_idx] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);
					}
				}
			}
			
			if (autoconfig == true) {
				current.dhcp_flag |=  IPADM_AUTOCONFIG_ENABLE;
				trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : ") + "cp_num " + cp_num + "management interface" + mgmnt_name + "AUTOCONFIG Enable");
			} else {
				current.dhcp_flag &=  ~IPADM_AUTOCONFIG_ENABLE;
				trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : ") + "cp_num " + cp_num + "management interface" + mgmnt_name + "AUTOCONFIG Disable");
					}

		
			/* Set Link Speed */
			trace (TRACE_LEVEL_ERROR, string("IpadmLocalObjectManager::postboot, got ethmode=") + speed);
			if(setLinkSpeed((char *) "eth0", speed) != 0) {
				trace (TRACE_LEVEL_ERROR, string("IpadmLocalObjectManager::postboot failed to set ethmode"));
			}

			/* Set icmp unreachable */
			char cmd[100];
			char unreach, echo, unreachv6, echov6;
			strcpy(cmd,"echo 0 > " ICMP_UNREACHABLE);
			bool unreachable = pIpadmManagedObject->getUnreachable();
			trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : set ipv4 unreachable = ") + id + " " + unreachable);
			unreach = cmd[5] = (unreachable ? '1' : '0');
			sendcmd(id, cmd);

			/* Set icmp echo reply */
			strcpy(cmd, "echo 0 > " ICMP_ECHOIGNORE);
			bool echoreply = pIpadmManagedObject->getEchoreply();
			trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : set ipv4 echo-reply = ") + id + " " + echoreply);
			echo = cmd[5] = (echoreply ? '0' : '1');
			sendcmd(id, cmd);

			/* Set icmp rate limiting */
			unsigned int ratelimiting = pIpadmManagedObject->getRatelimiting();
			sprintf(cmd, "echo %u > " ICMP_RATELIMIT, ratelimiting);
			if (ratelimiting > 0)
				sprintf(&cmd[strlen(cmd)], ";echo %d > " ICMP_RATEMASK, ICMP_RATEMASK_ALL);
			else
				sprintf(&cmd[strlen(cmd)], ";echo 0 > " ICMP_RATEMASK);
				
			trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : set ipv4 ratelimiting = ") + id + " " + ratelimiting);
			sendcmd(id, cmd);

			/* Set icmpv6 unreachable */
			strcpy(cmd, "echo 0 > " ICMPV6_UNREACHABLE);
			bool v6unreachable = pIpadmManagedObject->getV6Unreachable();
			trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : set ipv6 unreahable = ") + id + " " + v6unreachable);
			unreachv6 = cmd[5] = (v6unreachable ? '1' : '0');
			sendcmd(id, cmd);

			/* Set icmpv6 echo reply */
			strcpy(cmd, "echo 0 > " ICMPV6_ECHOIGNORE);
			bool v6echoreply = pIpadmManagedObject->getV6Echoreply();
			trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : set ipv6 echo-reply = ") + id + " " + v6echoreply);
			echov6 = cmd[5] = (v6echoreply ? '0' : '1');
			sendcmd(id, cmd);

			/* Set icmpv6 rate limiting */
			unsigned int v6ratelimiting = pIpadmManagedObject->getV6Ratelimiting();
			sprintf(cmd, "echo %u > " ICMPV6_RATELIMIT, v6ratelimiting);
			trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::postboot : set ipv6 ratelimiting = ") + id + " " + v6ratelimiting);
			sendcmd(id, cmd);

			/* update backend */
			sprintf(cmd, "echo %c %c %u %c %c %u > " ICMP_FILE ";echo %c %c %u %c %c %u > /mnt" ICMP_FILE,
					unreach, echo, ratelimiting, unreachv6, echov6, v6ratelimiting,
					unreach, echo, ratelimiting, unreachv6, echov6, v6ratelimiting);
			sendcmd(id, cmd);

			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			delete syncQueryContext;
		} else {
			delete syncQueryContext;
		}
	}
	if (reset_addr == true){
			ipadm_ip_get_set_t current_tmp = current; 
			addr_flags_t local_flags_tmp[MAX_LOCAL_ADDR_TABLE_SIZE];
			current_tmp.local_flags = local_flags_tmp;
			for (int i = 0; i < MAX_LOCAL_ADDR_TABLE_SIZE; i++) {
					current_tmp.local_flags[i] = current.local_flags[i];
			}

			/* reset the management port addresses */
			current_tmp.local_flags[eth_idx0] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);	
			current_tmp.local_flags[eth_idx1] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);	
			//Call library
			rc = ipAdmIpAddrSet(&current_tmp, 0, 0);
			if (rc) {
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::postboot : ipAdmIpAddrSet for  reset failed");
			}
	}
	//Call library
	rc = ipAdmIpAddrSet(&current, 0, 0);
	if (rc) {
		trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::postboot : ipAdmIpAddrSet failed");
	}
	
	pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	pWaveAsynchronousContextForPostBootPhase->callback ();
}

void  IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler( IpadmIpaddressconfigMessage *pIpadmIpaddressconfigMessage)
{
// Validations and Queries should go here
string id	= pIpadmIpaddressconfigMessage->getSwitchid ();
string ip_address = pIpadmIpaddressconfigMessage->getIp_address ();

NsmClientCreateIntIpConfigSPMessage *m = NULL;

size_t position = id.find_first_of("/");
if (position == string::npos) {
	trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : position error");
	return;
}
string temp = id.substr(position+1);
int cp_num = atoi(temp.c_str());

// retrieve the local and gw ID tables and address values
WaveManagedObjectSynchronousQueryContext *syncQueryContext =
	new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
{
	trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
	pIpadmIpaddressconfigMessage->setCompletionStatus (WRC_IPADM_INTERNAL_ERROR);
	reply (pIpadmIpaddressconfigMessage);
	delete syncQueryContext;
	return;
}

IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
delete syncQueryContext;

string current_ip_address = pIpadmManagedObject->getIPv4Address();

string str_prefix, str_ipaddr;
size_t pos;

pos = ip_address.find("/");
str_ipaddr = ip_address.substr(0, pos);
str_prefix = ip_address.substr(pos+1);

trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : ") + id + "cp_num " + cp_num + " New- " + ip_address + " " + pIpadmManagedObject->getId() + " Old- " + current_ip_address + " IP ---- " + str_ipaddr + " PREFIX ---- " + str_prefix);

#if 1
current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

current.local_addr_ids = local_addr_ids;
current.local_addrs = local_addrs;
current.local_prefixes = local_prefixes;
current.local_flags = local_flags;
current.gw_addr_ids = gw_addr_ids;
current.gw_addrs = gw_addrs;
current.gw_flags = gw_flags;

int rc = -1;
rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
if (rc) {
	trace (TRACE_LEVEL_ERROR, string ("ipAdmLocalAddrIdGet() returned ") + rc);
	//return;
}
rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmIpAddrGet() returned ") + rc);
			//return;
		}
		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
		if(!pizza) {
			cp_num -= 1;
			if (cp_num < 0) {
				trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : cp_num < 0"));
			}
		}
		
		//Find the index
		int eth_idx;
		if (pizza)
			eth_idx = locate_local_idx(&current, SWITCH_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET);
		else 
			eth_idx = locate_local_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET);
		if (eth_idx < 0) {
			trace (TRACE_LEVEL_FATAL, string ("IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : eth_idx is <0 ") + eth_idx);
			pIpadmIpaddressconfigMessage->setCompletionStatus (WRC_IPADM_IPV4_ADDRSET_ERROR);
			reply (pIpadmIpaddressconfigMessage);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			return;	
		}
		
		//Change current
		SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr.s_addr = inet_addr(str_ipaddr.c_str());
		
		int prefix = atoi(str_prefix.c_str());
		trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::IpadmMaskconfigMessageHandler : Got this prefix - ") + prefix);
		current.local_prefixes[eth_idx] = prefix;
		current.local_flags[eth_idx] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);
		
                //Defect 377082 - Check if MGMT IP subnet overlaps with that of front end ports
                ResourceId status = WAVE_MESSAGE_SUCCESS;
                m = new NsmClientCreateIntIpConfigSPMessage();
                if (m == NULL) return;

                m->setOpcode(NSM_MSG_MGMT_IP_ADDRESS);
                m->setIntfType(MGMT_INT);
                m->setIfName("eth0");
                m->setIp_addr_mask(ip_address);

                status = sendSynchronouslyToWaveClient ("nsm", m);
                if (status == WAVE_MESSAGE_SUCCESS) {
                        trace(TRACE_LEVEL_SUCCESS, string(__FUNCTION__) +
                                        " : (NSM_MSG_MGMT_IP_ADDRESS) sendToClient success");
                        status = m->getCompletionStatus();
                        if (status != WAVE_MESSAGE_SUCCESS) {
                                trace (TRACE_LEVEL_INFO, string ("(NSM) getCompletionStatus() FAILED "));

                                pIpadmIpaddressconfigMessage->setCompletionStatus (status);
                                reply (pIpadmIpaddressconfigMessage);
                                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
                                return;

                        }
                        else if (m->getClientStatus()) {
                                int err = m->getClientStatus();
                                status = NsmUtils::NsmBackendErrorMapToDcmStatus(err);
                                trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : Error from NSM, status -") + status);

                                pIpadmIpaddressconfigMessage->setCompletionStatus (status);
                                reply (pIpadmIpaddressconfigMessage);
                                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
                                return;

                        }
                } else {
                        trace(TRACE_LEVEL_ERROR, string(__FUNCTION__) +
                                        " : (NSM_MSG_MGMT_IP_ADDRESS) sendToClient failed");
                         pIpadmIpaddressconfigMessage->setCompletionStatus (status);
                         reply (pIpadmIpaddressconfigMessage);
                         WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
                         return;
                }
                delete m;

		//Call library
		rc = ipAdmIpAddrSet(&current, 0, 0);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : ipAdmIpAddrSet failed");
			pIpadmIpaddressconfigMessage->setCompletionStatus (WRC_IPADM_IPV4_ADDRSET_ERROR);
			reply (pIpadmIpaddressconfigMessage);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			return;
		}
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : ipAdmIpAddrSet Succeeded!");
		
#endif
		startTransaction ();
		updateWaveManagedObject (pIpadmManagedObject);

		pIpadmManagedObject->setIPv4Address (ip_address);
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : Commit to DB Success");
			status = WAVE_MESSAGE_SUCCESS;		
		}
		else 
		{
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : Commit To DB failed");
		}
		pIpadmIpaddressconfigMessage->setCompletionStatus (status);
		reply (pIpadmIpaddressconfigMessage);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
    }

    void  IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler( IpadmIcmpUnreachableConfigMessage *pIpadmIcmpUnreachableConfigMessage)
	{
		/* get set value */
		string	id = pIpadmIcmpUnreachableConfigMessage->getSwitchid();
		bool	unreachable = pIpadmIcmpUnreachableConfigMessage->getUnreachable();
		trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler : set ") + id + " " + unreachable);

		/* get current value in DB */
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
		{
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			delete syncQueryContext;
			return;
		}
		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		bool cur_unreachable = pIpadmManagedObject->getUnreachable();
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler : Got these -------" +
			id + " " + unreachable + " " + pIpadmManagedObject->getId() + " " + cur_unreachable);

		/* update new value to DB */
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (unreachable != cur_unreachable) {
			startTransaction ();
			updateWaveManagedObject (pIpadmManagedObject);
			pIpadmManagedObject->setUnreachable(unreachable);
			status = commitTransaction ();
			if (FRAMEWORK_SUCCESS == status) {
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler : Commit to DB Success");
				status = WAVE_MESSAGE_SUCCESS;		
			} else 
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler : Commit To DB failed");
		}

		/* update back end */
		char cmd[200];
		sprintf(cmd, "echo %c > " ICMP_UNREACHABLE
			";val=(%c %c %u %c %c %u);echo ${val[@]} > " ICMP_FILE
			";echo ${val[@]} > /mnt" ICMP_FILE, unreachable ? '1' : '0',
			pIpadmManagedObject->getUnreachable() ? '1' : '0',
			pIpadmManagedObject->getEchoreply() ? '0' : '1',
			pIpadmManagedObject->getRatelimiting(),
			pIpadmManagedObject->getV6Unreachable() ? '1' : '0',
			pIpadmManagedObject->getV6Echoreply() ? '0' : '1',
			pIpadmManagedObject->getV6Ratelimiting());
		sendcmd(id, cmd);

		/* set return status and clean up */
		pIpadmIcmpUnreachableConfigMessage->setCompletionStatus (status);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
		delete syncQueryContext;

		reply (pIpadmIcmpUnreachableConfigMessage);
	}

    void  IpadmLocalObjectManager::IpadmIcmpEchoreplyConfigMessageHandler( IpadmIcmpEchoreplyConfigMessage *pIpadmIcmpEchoreplyConfigMessage)
	{        
		/* get set value */
		string	id = pIpadmIcmpEchoreplyConfigMessage->getSwitchid();
		bool	echoreply = pIpadmIcmpEchoreplyConfigMessage->getEchoreply();
		trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmIcmpEchoreplyConfigMessageHandler : set ") + id + " " + echoreply);

		/* get current value in DB */
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
		{
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			delete syncQueryContext;
			return;
		}
		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		bool cur_echoreply = pIpadmManagedObject->getEchoreply();
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpEchoreplyConfigMessageHandler : Got these -------" +
			id + " " + echoreply + " " + pIpadmManagedObject->getId() + " " + cur_echoreply);

		/* update new value to DB */
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (echoreply != cur_echoreply) {
			startTransaction ();
			updateWaveManagedObject (pIpadmManagedObject);
			pIpadmManagedObject->setEchoreply(echoreply);
			status = commitTransaction ();
			if (FRAMEWORK_SUCCESS == status) {
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler : Commit to DB Success");
				status = WAVE_MESSAGE_SUCCESS;		
			} else 
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler : Commit To DB failed");
		}

		/* update back end */
		char cmd[200];
		sprintf(cmd, "echo %c > " ICMP_ECHOIGNORE
			";val=(%c %c %u %c %c %u);echo ${val[@]} > " ICMP_FILE
			";echo ${val[@]} > /mnt" ICMP_FILE, echoreply ? '0' : '1',
			pIpadmManagedObject->getUnreachable() ? '1' : '0',
			pIpadmManagedObject->getEchoreply() ? '0' : '1',
			pIpadmManagedObject->getRatelimiting(),
			pIpadmManagedObject->getV6Unreachable() ? '1' : '0',
			pIpadmManagedObject->getV6Echoreply() ? '0' : '1',
			pIpadmManagedObject->getV6Ratelimiting());
		sendcmd(id, cmd);

		/* set return status and clean up */
		pIpadmIcmpEchoreplyConfigMessage->setCompletionStatus (status);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
		delete syncQueryContext;

		reply (pIpadmIcmpEchoreplyConfigMessage);
	}

    void  IpadmLocalObjectManager::IpadmIcmpRatelimitingConfigMessageHandler( IpadmIcmpRatelimitingConfigMessage *pIpadmIcmpRatelimitingConfigMessage)
	{
		/* get set value */
		string	id = pIpadmIcmpRatelimitingConfigMessage->getSwitchid();
		UI32 ratelimiting = pIpadmIcmpRatelimitingConfigMessage->getRatelimiting();
		trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmIcmpRatelimitingConfigMessageHandler : set ") + id + " " + ratelimiting);

		/* get current value in DB */
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
		{
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			delete syncQueryContext;
			return;
		}
		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		UI32 cur_ratelimiting = pIpadmManagedObject->getRatelimiting();
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpEchoreplyConfigMessageHandler : Got these -------" +
			id + " " + ratelimiting + " " + pIpadmManagedObject->getId() + " " + cur_ratelimiting);

		/* update new value to DB */
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (ratelimiting != cur_ratelimiting) {
			startTransaction ();
			updateWaveManagedObject (pIpadmManagedObject);
			pIpadmManagedObject->setRatelimiting(ratelimiting);
			status = commitTransaction ();
			if (FRAMEWORK_SUCCESS == status) {
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler : Commit to DB Success");
				status = WAVE_MESSAGE_SUCCESS;		
			} else 
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIcmpUnreachableConfigMessageHandler : Commit To DB failed");
		}

		/* update back end */
		char cmd[200];
		sprintf(cmd, "echo %u > " ICMP_RATELIMIT
			";val=(%c %c %u %c %c %u);echo ${val[@]} > " ICMP_FILE
			";echo ${val[@]} > /mnt" ICMP_FILE, ratelimiting,
			pIpadmManagedObject->getUnreachable() ? '1' : '0',
			pIpadmManagedObject->getEchoreply() ? '0' : '1',
			pIpadmManagedObject->getRatelimiting(),
			pIpadmManagedObject->getV6Unreachable() ? '1' : '0',
			pIpadmManagedObject->getV6Echoreply() ? '0' : '1',
			pIpadmManagedObject->getV6Ratelimiting());
		if (ratelimiting > 0)
			sprintf(&cmd[strlen(cmd)], ";echo %d > " ICMP_RATEMASK, ICMP_RATEMASK_ALL);
		else
			sprintf(&cmd[strlen(cmd)], ";echo 0 > " ICMP_RATEMASK);
		sendcmd(id, cmd);

		/* set return status and clean up */
		pIpadmIcmpRatelimitingConfigMessage->setCompletionStatus (status);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
		delete syncQueryContext;

		reply (pIpadmIcmpRatelimitingConfigMessage);
	}

    void  IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler( IpadmIcmpv6UnreachableConfigMessage *pIpadmIcmpv6UnreachableConfigMessage)
	{
		/* get set value */
		string	id = pIpadmIcmpv6UnreachableConfigMessage->getSwitchid();
		bool	unreachable = pIpadmIcmpv6UnreachableConfigMessage->getUnreachable();
		trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler : set ") + id + " " + unreachable);

		/* get current value in DB */
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
		{
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			delete syncQueryContext;
			return;
		}
		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		bool cur_unreachable = pIpadmManagedObject->getV6Unreachable();
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler : Got these -------" +
			id + " " + unreachable + " " + pIpadmManagedObject->getId() + " " + cur_unreachable);

		/* update new value to DB */
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (unreachable != cur_unreachable) {
			startTransaction ();
			updateWaveManagedObject (pIpadmManagedObject);
			pIpadmManagedObject->setV6Unreachable(unreachable);
			status = commitTransaction ();
			if (FRAMEWORK_SUCCESS == status) {
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler : Commit to DB Success");
				status = WAVE_MESSAGE_SUCCESS;		
			} else 
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler : Commit To DB failed");
		}

		/* update back end */
		char cmd[200];
		sprintf(cmd, "echo %c > " ICMPV6_UNREACHABLE
			";val=(%c %c %u %c %c %u);echo ${val[@]} > " ICMP_FILE
			";echo ${val[@]} > /mnt" ICMP_FILE, unreachable ? '1' : '0',
			pIpadmManagedObject->getUnreachable() ? '1' : '0',
			pIpadmManagedObject->getEchoreply() ? '0' : '1',
			pIpadmManagedObject->getRatelimiting(),
			pIpadmManagedObject->getV6Unreachable() ? '1' : '0',
			pIpadmManagedObject->getV6Echoreply() ? '0' : '1',
			pIpadmManagedObject->getV6Ratelimiting());
		sendcmd(id, cmd);

		/* set return status and clean up */
		pIpadmIcmpv6UnreachableConfigMessage->setCompletionStatus (status);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
		delete syncQueryContext;

		reply (pIpadmIcmpv6UnreachableConfigMessage);
	}

    void  IpadmLocalObjectManager::IpadmIcmpv6EchoreplyConfigMessageHandler( IpadmIcmpv6EchoreplyConfigMessage *pIpadmIcmpv6EchoreplyConfigMessage)
	{        
		/* get set value */
		string	id = pIpadmIcmpv6EchoreplyConfigMessage->getSwitchid();
		bool	echoreply = pIpadmIcmpv6EchoreplyConfigMessage->getEchoreply();
		trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmIcmpv6EchoreplyConfigMessageHandler : set ") + id + " " + echoreply);

		/* get current value in DB */
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
		{
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			delete syncQueryContext;
			return;
		}
		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		bool cur_echoreply = pIpadmManagedObject->getV6Echoreply();
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpv6EchoreplyConfigMessageHandler : Got these -------" +
			id + " " + echoreply + " " + pIpadmManagedObject->getId() + " " + cur_echoreply);

		/* update new value to DB */
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (echoreply != cur_echoreply) {
			startTransaction ();
			updateWaveManagedObject (pIpadmManagedObject);
			pIpadmManagedObject->setV6Echoreply(echoreply);
			status = commitTransaction ();
			if (FRAMEWORK_SUCCESS == status) {
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler : Commit to DB Success");
				status = WAVE_MESSAGE_SUCCESS;		
			} else 
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler : Commit To DB failed");
		}

		/* update back end */
		char cmd[200];
		sprintf(cmd, "echo %c > " ICMPV6_ECHOIGNORE
			";val=(%c %c %u %c %c %u);echo ${val[@]} > " ICMP_FILE
			";echo ${val[@]} > /mnt" ICMP_FILE, echoreply ? '0' : '1',
			pIpadmManagedObject->getUnreachable() ? '1' : '0',
			pIpadmManagedObject->getEchoreply() ? '0' : '1',
			pIpadmManagedObject->getRatelimiting(),
			pIpadmManagedObject->getV6Unreachable() ? '1' : '0',
			pIpadmManagedObject->getV6Echoreply() ? '0' : '1',
			pIpadmManagedObject->getV6Ratelimiting());
		sendcmd(id, cmd);

		/* set return status and clean up */
		pIpadmIcmpv6EchoreplyConfigMessage->setCompletionStatus (status);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
		delete syncQueryContext;

		reply (pIpadmIcmpv6EchoreplyConfigMessage);
	}

    void  IpadmLocalObjectManager::IpadmIcmpv6RatelimitingConfigMessageHandler( IpadmIcmpv6RatelimitingConfigMessage *pIpadmIcmpv6RatelimitingConfigMessage)
	{
		/* get set value */
		string	id = pIpadmIcmpv6RatelimitingConfigMessage->getSwitchid();
		unsigned int ratelimiting = pIpadmIcmpv6RatelimitingConfigMessage->getRatelimiting();
		trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmIcmpv6RatelimitingConfigMessageHandler : set ") + id + " " + ratelimiting);

		/* get current value in DB */
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
		{
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			delete syncQueryContext;
			return;
		}
		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		unsigned int cur_ratelimiting = pIpadmManagedObject->getV6Ratelimiting();
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpv6EchoreplyConfigMessageHandler : Got these -------" +
			id + " " + ratelimiting + " " + pIpadmManagedObject->getId() + " " + cur_ratelimiting);

		/* update new value to DB */
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (ratelimiting != cur_ratelimiting) {
			startTransaction ();
			updateWaveManagedObject (pIpadmManagedObject);
			pIpadmManagedObject->setV6Ratelimiting(ratelimiting);
			status = commitTransaction ();
			if (FRAMEWORK_SUCCESS == status) {
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler : Commit to DB Success");
				status = WAVE_MESSAGE_SUCCESS;		
			} else 
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIcmpv6UnreachableConfigMessageHandler : Commit To DB failed");
		}

		/* update back end */
		char cmd[200];
		sprintf(cmd, "echo %u > " ICMPV6_RATELIMIT
			";val=(%c %c %u %c %c %u);echo ${val[@]} > " ICMP_FILE
			";echo ${val[@]} > /mnt" ICMP_FILE, ratelimiting,
			pIpadmManagedObject->getUnreachable() ? '1' : '0',
			pIpadmManagedObject->getEchoreply() ? '0' : '1',
			pIpadmManagedObject->getRatelimiting(),
			pIpadmManagedObject->getV6Unreachable() ? '1' : '0',
			pIpadmManagedObject->getV6Echoreply() ? '0' : '1',
			pIpadmManagedObject->getV6Ratelimiting());
		sendcmd(id, cmd);

		/* set return status and clean up */
		pIpadmIcmpv6RatelimitingConfigMessage->setCompletionStatus (status);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
		delete syncQueryContext;

		reply (pIpadmIcmpv6RatelimitingConfigMessage);
	}

    void  IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler( IpadmGatewayconfigMessage *pIpadmGatewayconfigMessage)
    {
		/* If the L3 license is enabled, we are not allowing the CLI to go through.
		 * On L3 switches rtm CLI will configure the gateway and default route.
                 */
                if (licenseCheck(LAYER_3_LICENSE)) {

        	        trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler : Command is not allowed in L3 switches"));
			pIpadmGatewayconfigMessage->setCompletionStatus (WRC_IPADM_NO_SUPPORT_ON_L3_SWITCH);
			reply (pIpadmGatewayconfigMessage);
			return;
                }

		IpV4Address ip_gateway = pIpadmGatewayconfigMessage->getGateway ();
#if 1
		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		current.local_addr_ids = local_addr_ids;
		current.local_addrs = local_addrs;
		current.local_prefixes = local_prefixes;
		current.local_flags = local_flags;
		current.gw_addr_ids = gw_addr_ids;
		current.gw_addrs = gw_addrs;
		current.gw_flags = gw_flags;

		int rc = -1;
		int eth_idx;
		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmLocalAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmIpAddrGet() returned ") + rc);
			//return;
		}
		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
		UI32 cp_max;

		if (pizza)
	            	cp_max = 1;
	       	else
	           	cp_max = 2;

		for (UI32 cp_num = 0; cp_num < cp_max; cp_num++) 
		{
			//Find the index
			eth_idx = locate_gw_idx(&current, SPECIFIC_CP_HOST, cp_num, AF_INET);
			if (eth_idx < 0) {
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler :could not locate gateway idx");
				continue;
			}  else {
			    IpV4Address zero_gateway;
			    zero_gateway.fromString("0.0.0.0");
			    if (ip_gateway == zero_gateway) {
				current.gw_flags[eth_idx] |= IPADM_ADDR_NONE;
				trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler : ") + "cp_num " + cp_num + "gw address" + "none" );
			    } else {
				//Change current
				string str_ipgw = ip_gateway.toString();
				trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler : ") + "cp_num " + cp_num + "gw address" + str_ipgw );
				SOCKADDR_INP(current.gw_addrs[eth_idx])->sin_addr.s_addr = inet_addr(str_ipgw.c_str());
				current.gw_flags[eth_idx] &= ~IPADM_ADDR_NONE;
			    }
			}
		
#endif
			/* Call ipAdmIpAddrSet only once for pizza/chassis system */ 
			if (cp_max == 1 || (cp_max == 2 && cp_num == 1))
			{
				//Call library
				rc = ipAdmIpAddrSet(&current, 0, 0);
				if (rc) {
					trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler : ipAdmIpAddrSet failed");
					pIpadmGatewayconfigMessage->setCompletionStatus (WRC_IPADM_IPV4_GWSET_ERROR);
					reply (pIpadmGatewayconfigMessage);
					return;
				}
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler : ipAdmIpAddrSet Succeeded!");
			}
			
		}

		for (UI32 cp_num = 0; cp_num < cp_max; cp_num++) 
		{
#if 0
			string id	= pIpadmGatewayconfigMessage->getSwitchid ();
			size_t position = id.find_first_of("/");
			if (position == string::npos) {
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : position error");
				return;
			}
#endif
            		UI32 mappedId = DcmToolKit::getLocalMappedId();	
           	 	string id; 
     		       	if (!pizza)
        			id =  mappedId + string ("/") + (cp_num + 1);
       			else 
                		id = mappedId + string ("/0");
		
    			/* retrieve the gw ID tables and address values */
		
			WaveManagedObjectSynchronousQueryContext *syncQueryContext =
				new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
			syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
			vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
			if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
			{
				trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
				delete syncQueryContext;
				continue;
			}

		    	IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);

			IpV4Address current_ip_gateway = pIpadmManagedObject->getIPv4Gateway();

			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler : Got these -------" + id + " " + ip_gateway.toString() + " " + pIpadmManagedObject->getId() + " " + current_ip_gateway.toString());

		
			ResourceId status = WAVE_MESSAGE_SUCCESS;
			
			startTransaction ();
			updateWaveManagedObject (pIpadmManagedObject);

			pIpadmManagedObject->setIPv4Gateway (ip_gateway);
			status = commitTransaction ();
			if (FRAMEWORK_SUCCESS == status)
			{
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler : Commit to DB Success");
				status = WAVE_MESSAGE_SUCCESS;		
			}
			else 
			{
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmGatewayconfigMessageHandler : Commit To DB failed");
			}
			pIpadmGatewayconfigMessage->setCompletionStatus (status);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			delete syncQueryContext;
		}
		reply (pIpadmGatewayconfigMessage);
    }

	void  IpadmLocalObjectManager::IpadmIpv6RouteconfigMessageHandler(IpadmIpv6RouteconfigMessage *pIpadmIpv6RouteconfigMessage)
	{
		//getting IpadmIpv6RouteLocal MO
		IpadmIpv6RouteLocalManagedObject *pIpadmIpv6RouteLocalManagedObject = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;
		ResourceId db_status = WAVE_MESSAGE_ERROR;
		char errBuf[4096] = {'\0'};
		IpV6AddressNetworkMask dest = pIpadmIpv6RouteconfigMessage->getDest();
		WaveClientSessionContext waveClientSessionContext = pIpadmIpv6RouteconfigMessage->getWaveClientSessionContext();
		trace (TRACE_LEVEL_DEBUG, "IpadmIpv6RouteLocalObjectManager::getIpadmIpv6RouteMo:  "+IpadmIpv6RouteLocalManagedObject::getClassName());
		WaveManagedObjectSynchronousQueryContext *syncQueryCtxt =
			new WaveManagedObjectSynchronousQueryContext(IpadmIpv6RouteLocalManagedObject::getClassName());
		syncQueryCtxt->addAndAttribute (new AttributeIpV6AddressNetworkMask(&dest,"dest"));
		vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryCtxt);
		if (pResults) {
			if (pResults->size() == 1) {
				pIpadmIpv6RouteLocalManagedObject = dynamic_cast<IpadmIpv6RouteLocalManagedObject*> ((*pResults)[0]);
				delete syncQueryCtxt;
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpv6RouteRouteMo: Successfully query Ipv6 Route config MO \n");
				status = WAVE_MESSAGE_SUCCESS;
			}

			pResults->clear();
			delete pResults;
		} else {
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpv6RouteRouteMo: Fail to query \n");
		}

		IpV6Address nhIp = pIpadmIpv6RouteconfigMessage->getNhIp();
		UI32 routeType = pIpadmIpv6RouteconfigMessage->getRouteType();
		string type = pIpadmIpv6RouteconfigMessage->getType();
		const char *str2 = type.c_str();
		string add = "add";
		const char *str1 = add.c_str();
		const char *str3 = "";
		string ss;
		if(strcmp(str2,str3) == 0  || strcmp(str2,str1)==0 ) {
			//Add operation. If object all ready exists reject this operation.
			if (pIpadmIpv6RouteLocalManagedObject != NULL) {
				//If the  object already exists can not add new. Reject this operation
				trace (TRACE_LEVEL_ERROR, "LocalObjectManager::IpadmIpv6RouteLocalObjectManagerHandler: MO Already exists for add operation.\n");
				status = WRC_IPADM_ROUTE_EXISTS_ERROR;
			} else {
				type = "add";
				if (routeType == MGMNT_ROUTE_NEXTHOP) {
					ss = string("ip -6 route ") + string(type) + " " + string(dest.toString()) + string(" via ") + string(nhIp.toString()) + string(" dev eth0");
				} else {
					ss = string("ip -6 route ") + string(type) + " " + string(dest.toString()) + string(" dev eth0");
				}

				//Execute the route operation and if it is successfull add to DB
				char *id1 = (char *) ss.c_str();
				RouteExecCommand(id1, errBuf);
				if (errBuf[0] != '\0') {
					tracePrintf(TRACE_LEVEL_DEBUG, "Route Error=%s", errBuf);
					printfToWaveClientSession (waveClientSessionContext, "%s",errBuf);
					status = WRC_IPADM_ROUTE_ERROR;
				} else {
					startTransaction();
					pIpadmIpv6RouteLocalManagedObject = new IpadmIpv6RouteLocalManagedObject(dynamic_cast<IpadmLocalObjectManager*>(getPWaveObjectManager()));
					pIpadmIpv6RouteLocalManagedObject->setDest(dest);
					pIpadmIpv6RouteLocalManagedObject->setNhIp(nhIp);
					pIpadmIpv6RouteLocalManagedObject->setRouteType(routeType);

					updateWaveManagedObject (pIpadmIpv6RouteLocalManagedObject);
					db_status = commitTransaction();
					status = WAVE_MESSAGE_SUCCESS;
				}
			}
		} else {
			//Delete Operation. If the object doesn't exists, retrun error.
			trace (TRACE_LEVEL_DEBUG, "LocalObjectManager::IpadmIpv6RouteLocalObjectManagerHandler: Route Delete operation \n");
			if (pIpadmIpv6RouteLocalManagedObject == NULL) {
				trace (TRACE_LEVEL_ERROR, "LocalObjectManager::IpadmIpv6RouteLocalObjectManagerHandler: MO doesn't exists \n");
				status = WRC_IPADM_NO_ROUTE_EXISTS_ERROR;
			} else {
				trace (TRACE_LEVEL_DEBUG, "LocalObjectManager::IpadmIpv6RouteLocalObjectManagerHandler: Executing Route Delete operation \n");
				type = "del";
				const IpV6Address nhIp_del =  pIpadmIpv6RouteLocalManagedObject->getNhIp();
				const UI32 routeType_del = pIpadmIpv6RouteLocalManagedObject->getRouteType();
				if (routeType_del == MGMNT_ROUTE_NEXTHOP) {
					ss = string("ip -6 route ") + string(type) + " " + string(dest.toString()) + string(" via ") + string(nhIp_del.toString());
				} else {
					ss = string("ip -6 route ") + string(type) + " " + string(dest.toString()) + string(" dev eth0");
				}

				//Execute the route operation and if it is successfull remove from db.
				char *id1 = (char *) ss.c_str();
				RouteExecCommand(id1, errBuf);
				if (errBuf[0] != '\0') {
					tracePrintf(TRACE_LEVEL_DEBUG, "Route Error=%s", errBuf);
					printfToWaveClientSession (waveClientSessionContext, "%s",errBuf);
					status = WRC_IPADM_ROUTE_ERROR;
				} else {
					startTransaction ();
					status = WAVE_MESSAGE_SUCCESS;
					delete pIpadmIpv6RouteLocalManagedObject;
					pIpadmIpv6RouteLocalManagedObject = NULL;
					//updateWaveManagedObject (pIpadmIpv6RouteLocalManagedObject);
					db_status = commitTransaction ();
				}
			}
		}

		if (status != WAVE_MESSAGE_SUCCESS) {
			trace (TRACE_LEVEL_ERROR, "LocalObjectManager::IpadmIpv6RouteLocalObjectManagerHandler: Route operation Failed.");
		}

		if (FRAMEWORK_SUCCESS == db_status)
		{
			trace (TRACE_LEVEL_INFO, "ROUTE *** IpadmLocalObjectManager::IpadmIpv6RouteconfigMessageHandler : Commit to DB Success");
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			trace (TRACE_LEVEL_ERROR, "ROUTE *** IpadmLocalObjectManager::IpadmIpv6RouteconfigMessageHandler : Commit To DB failed");
		}

		pIpadmIpv6RouteconfigMessage->setCompletionStatus (status);
		reply (pIpadmIpv6RouteconfigMessage);
	}








void IpadmLocalObjectManager::IpadmConfigMessageHandler(IpadmLocalDnsCfgMessage *pIpadmLocalDnsCfgMessage)
{
    trace (TRACE_LEVEL_DEBUG, "IpadmLocalOM::IpadmConfigMessageHandler..");
 
    // setup the sequence 
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        // If MappedId -> MO is needed, use queryLocalManagedObjectFromMappedId() to new/setup MO in IpadmLocalMessageContext
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IpadmLocalObjectManager::dnsConfigHandler),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IpadmLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IpadmLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };
    
    // put MSG to IpadmLocalMessageContext
    IpadmLocalMessageContext *pIpadmLocalMessageContext = new IpadmLocalMessageContext (reinterpret_cast<PrismMessage *> (pIpadmLocalDnsCfgMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    // If MappedId is needed to get associated its MO
    pIpadmLocalMessageContext->execute ();
}

ResourceId IpadmLocalObjectManager::dnsConfigHandler(IpadmLocalMessageContext *pIpadmLocalMessageContext)
{
   string 	domain_name;
   int 		i = 0, size_dn = 0, size_ns = 0, rc = 0;
   dns_t 	dns;
   string	strIpAddr;
   IpVxAddress 	db_ip;
   char 	nameServer_db[MAX_DOM_SIZE];
   const string orderField = "index";

   IpadmGlobalDNManagedObject *pIpadmGlobalDNManagedObject = NULL;
   IpadmGlobalNSManagedObject *pIpadmGlobalNSManagedObject = NULL;

   memset(&dns, '\0', sizeof (dns));

   WaveManagedObjectSynchronousQueryContext *syncQueryContextDN =
                new WaveManagedObjectSynchronousQueryContext(IpadmGlobalDNManagedObject::getClassName());
   vector<WaveManagedObject *> *pDNResults = querySynchronously (syncQueryContextDN);

   WaveManagedObjectSynchronousQueryContext *syncQueryContextNS =
                new WaveManagedObjectSynchronousQueryContext(IpadmGlobalNSManagedObject::getClassName());
   syncQueryContextNS->addOrderField (orderField, true);
   vector<WaveManagedObject *> *pNSResults = querySynchronously (syncQueryContextNS);

   delete syncQueryContextDN;
   delete syncQueryContextNS;

   if ((NULL == pDNResults) || (pDNResults->size() == 0)) {
           tracePrintf(TRACE_LEVEL_INFO, true, true, "IpadmLocalOM::dnsConfigHandler No DN MO\n");
	   /* if no DN configured reset backend */
	   rc = domNsReset();
   }
   size_dn = pDNResults->size();

   if ((NULL == pNSResults) || (pNSResults->size() == 0)) {
           tracePrintf(TRACE_LEVEL_INFO, true, true, "IpadmLocalOM::dnsConfigHandler No NS MO\n");
	   /* if no NS configured reset backend */
	   rc = domNsReset();
   }
   size_ns = pNSResults->size();

   if (size_dn != 0 ) {
      pIpadmGlobalDNManagedObject = dynamic_cast<IpadmGlobalDNManagedObject *> ((*pDNResults)[0]);
      if (pIpadmGlobalDNManagedObject != NULL) {
        domain_name = pIpadmGlobalDNManagedObject->getDomainName();
	strncpy(dns.domainName, domain_name.c_str(), domain_name.size());
   	trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigHandler: DN=") + domain_name.c_str());
      }
   }

   if (size_ns != 0 ) {
     for (i = 0; i < size_ns; i++) {
        pIpadmGlobalNSManagedObject = dynamic_cast<IpadmGlobalNSManagedObject *> ((*pNSResults)[i]);
        if (pIpadmGlobalNSManagedObject != NULL) {
          db_ip = pIpadmGlobalNSManagedObject->getNameSrvIp();
	  strIpAddr = db_ip.toString();
	  snprintf(nameServer_db, strIpAddr.size(), "%s", strIpAddr.c_str());
	  /* fill the local DS */
	  if (i == 0) {
		strncpy(dns.nameServer1, strIpAddr.c_str(), strIpAddr.size());
	  } else {
		strncpy(dns.nameServer2, strIpAddr.c_str(), strIpAddr.size());
	  }
		
   	  trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigHandler: NS=") + strIpAddr.c_str());
        }
     }
   }

   if ((size_ns != 0) && (size_dn != 0)) {
       /* Let's set the backend if both values are present */
       rc = domNsSet(&dns);
       if (rc == 0)
          trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigHandler: DNS config update success!..."));
       else {
          trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigHandler: DNS config update failed..."));
	  
          if (pDNResults)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDNResults);
          if (pNSResults)
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pNSResults);
			      
          return (WRC_IPADM_DNS_CONFIG_FAIL_ERROR);
       }
   } else if ((size_ns != 0) && (size_dn == 0)) {
       /*set warning message in case domain name entry is missing*/	 
       trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigHandler: NS entry found when DN entry is absent"));
       if (pNSResults){
           WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pNSResults);
       }
       return (WRC_IPADM_DN_UNDEFINED_WARNING);
   } else if ((size_ns == 0) && (size_dn != 0)) {
       /*set warning message in case domain name entry is missing*/	 
       trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigHandler: DN entry found when NS entry is absent"));
       if (pDNResults){
           WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDNResults);
       }
       return (WRC_IPADM_NS_UNDEFINED_WARNING);
   } else {
       trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigHandler: skip DNS config to backend..."));
   }

   if (pDNResults)
      WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDNResults);
   if (pNSResults)
      WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pNSResults);

   return (WAVE_MESSAGE_SUCCESS);
}

void IpadmLocalObjectManager::dnsConfigBoot()
{
   string	domain_name;
   int 		i = 0, size_dn = 0, size_ns = 0, rc = 0;
   dns_t 	dns; 
   string	strIpAddr;
   IpVxAddress 	db_ip;
   char 	nameServer_db[MAX_DOM_SIZE];
   const string orderField = "index";

   IpadmGlobalDNManagedObject *pIpadmGlobalDNManagedObject = NULL;
   IpadmGlobalNSManagedObject *pIpadmGlobalNSManagedObject = NULL;

   memset(&dns, '\0', sizeof (dns));
   
   WaveManagedObjectSynchronousQueryContext *syncQueryContextDN =
                new WaveManagedObjectSynchronousQueryContext(IpadmGlobalDNManagedObject::getClassName());
   vector<WaveManagedObject *> *pDNResults = querySynchronously (syncQueryContextDN);
   
   WaveManagedObjectSynchronousQueryContext *syncQueryContextNS =
                new WaveManagedObjectSynchronousQueryContext(IpadmGlobalNSManagedObject::getClassName());
   syncQueryContextNS->addOrderField (orderField, true);
   vector<WaveManagedObject *> *pNSResults = querySynchronously (syncQueryContextNS);
   
   delete syncQueryContextDN;
   delete syncQueryContextNS;

   if ((NULL == pDNResults) || (pDNResults->size() == 0)) {
           tracePrintf(TRACE_LEVEL_INFO, true, true, "IpadmLocalOM::dnsConfigBoot No DN MO\n");
	   /* if no DN found reset backend and return */
           rc = domNsReset();
	   if (pNSResults)
   		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDNResults);
	   if (pNSResults)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pNSResults);
	   return;
   }
   size_dn = pDNResults->size();

   if ((NULL == pNSResults) || (pNSResults->size() == 0)) {
           tracePrintf(TRACE_LEVEL_INFO, true, true, "IpadmLocalOM::dnsConfigBoot No NS MO\n");
           rc = domNsReset();
	   /* if no NS found reset backend and return */
	   if (pNSResults)
   		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDNResults);
	   if (pNSResults)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pNSResults);
	   return;
   }
   size_ns = pNSResults->size();

   pIpadmGlobalDNManagedObject = dynamic_cast<IpadmGlobalDNManagedObject *> ((*pDNResults)[0]);
   if (pIpadmGlobalDNManagedObject != NULL) {
        domain_name = pIpadmGlobalDNManagedObject->getDomainName();
        strncpy(dns.domainName, domain_name.c_str(), domain_name.size());
        trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigBoot DN=") + domain_name.c_str());
   }

   for (i = 0; i < size_ns; i++) {
      pIpadmGlobalNSManagedObject = dynamic_cast<IpadmGlobalNSManagedObject *> ((*pNSResults)[i]);
      if (pIpadmGlobalNSManagedObject != NULL) {
          db_ip = pIpadmGlobalNSManagedObject->getNameSrvIp();
          strIpAddr = db_ip.toString();
          snprintf(nameServer_db, strIpAddr.size(), "%s", strIpAddr.c_str());
          if (i == 0) {
                strncpy(dns.nameServer1, strIpAddr.c_str(), strIpAddr.size());
          } else {
                strncpy(dns.nameServer2, strIpAddr.c_str(), strIpAddr.size());
          }
          trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigBoot NS=") + strIpAddr.c_str());
      }
   }

   if ((size_ns != 0) && (size_dn != 0)) {
       /* Let's set the backend if both values are present */
       rc = domNsSet(&dns);
       if (rc == 0)
          trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigBoot DNS config update success!"));
       else
          trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigBoot DNS config update failed"));
   } else {
          trace (TRACE_LEVEL_INFO, string ("IpadmLocalOM::dnsConfigBoot skip DNS config to backend"));
   }

   if (pDNResults)
     WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDNResults);
   if (pNSResults)
     WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pNSResults);

   return;
}

    void  IpadmLocalObjectManager::IpadmDhcpConfigMessageHandler( IpadmDhcpConfigMessage *pIpadmDhcpConfigMessage)
    {
	bool dhcp = pIpadmDhcpConfigMessage->getDhcp ();
#if 1
	current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
	current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

	current.local_addr_ids = local_addr_ids;
	current.local_addrs = local_addrs;
	current.local_prefixes = local_prefixes;
	current.local_flags = local_flags;
	current.gw_addr_ids = gw_addr_ids;
	current.gw_addrs = gw_addrs;
	current.gw_flags = gw_flags;

	int rc = -1;
	rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
	if (rc) {
		trace (TRACE_LEVEL_ERROR, string ("ipAdmLocalAddrIdGet() returned ") + rc);
		//return;
	}
	rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
	if (rc) {
		trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
		//return;
	}
	rc = ipAdmIpAddrGet(&current);
	if (rc) {
		trace (TRACE_LEVEL_ERROR, string ("ipAdmIpAddrGet() returned ") + rc);
		//return;
	}
	int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
	UI32 cp_max;
	if (pizza)
		cp_max = 1;
	else
		cp_max = 2;
		
	for (UI32 cp_num = 0; cp_num < cp_max; cp_num++) {

		UI32 mappedId = DcmToolKit::getLocalMappedId();
		string id;

		if (!pizza)
			id =  mappedId + string ("/") + (cp_num + 1);
		else
			id = mappedId + string ("/0");

		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) {
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			pIpadmDhcpConfigMessage->setCompletionStatus (WRC_IPADM_INTERNAL_ERROR);
			reply (pIpadmDhcpConfigMessage);
			delete syncQueryContext;
			return;
			
		}

		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		delete syncQueryContext;

		if (pIpadmManagedObject != NULL) {
			//      Now replay the configuration to the ipadm daemon
			string ip_address = pIpadmManagedObject->getIPv4Address();
			IpV4Address ipv4_gateway = pIpadmManagedObject->getIPv4Gateway();
			bool current_dhcp = pIpadmManagedObject->getDhcp();
			string str_prefix, str_ipaddr;
			size_t pos;
			int prefix;
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmDhcpConfigMessageHandler : Got these -------" + id + " " + dhcp + " " + pIpadmManagedObject->getId() + " " + current_dhcp);

			if (dhcp == true) {
				current.dhcp_flag |= IPADM_DHCP_ENABLE;         //      On

			} else {
				current.dhcp_flag &= ~IPADM_DHCP_ENABLE;        //      Off

				//Find the index
				int eth_idx;
				if (pizza)
					eth_idx = locate_local_idx(&current, SWITCH_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET);
				else
					eth_idx = locate_local_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET);
				if (eth_idx < 0) {
					trace (TRACE_LEVEL_FATAL, string ("IpadmLocalObjectManager::IpadmDhcpConfigMessageHandler : eth_idx is <0 ") + eth_idx);
				} else {
					if (!ip_address.compare("")) {
						current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);	
					} else {
						pos = ip_address.find("/");
						str_ipaddr = ip_address.substr(0, pos);
						str_prefix = ip_address.substr(pos+1);
	
						//Change current
						SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr.s_addr = inet_addr(str_ipaddr.c_str());
						
						prefix = atoi(str_prefix.c_str());
						current.local_prefixes[eth_idx] = prefix;
						current.local_flags[eth_idx] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);
					}
				}
			
				int gw_idx = locate_gw_idx(&current, SPECIFIC_CP_HOST, cp_num, AF_INET);
				if (gw_idx < 0) {
				    	trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmDhcpConfigMessageHandler :could not locate gateway idx");
				} else {
				    	IpV4Address zero_gateway;
				    	zero_gateway.fromString("0.0.0.0");
				    	if (ipv4_gateway == zero_gateway) {
						current.gw_flags[gw_idx] |= IPADM_ADDR_NONE;
				    	} else {
						string str_ipgw = ipv4_gateway.toString();
						SOCKADDR_INP(current.gw_addrs[gw_idx])->sin_addr.s_addr = inet_addr(str_ipgw.c_str());
						current.gw_flags[gw_idx] &= ~IPADM_ADDR_NONE;
					}
				}
			}
		}
#endif
		/* Call ipAdmIpAddrSet only once for pizza/chassis system */ 
		if (cp_max == 1 || (cp_max == 2 && cp_num == 1)) {

			if(!pizza && dhcp == false) {
				/* get the  chassis static adress also before calling ipaddrset() */ 
                                LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
                        	WaveManagedObjectSynchronousQueryContext *synchronousQueryContext =
                               	new WaveManagedObjectSynchronousQueryContext(IpadmChassisManagedObject::getClassName());
                                vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, synchronousQueryContext);
                       		if ((pResults != NULL) && (pResults->size() != 0))
                       		{
                       			IpadmChassisManagedObject *pInterface = dynamic_cast<IpadmChassisManagedObject *> ((*pResults)[0]);
                       			string ip_addr = pInterface->getVipAddress();
					/* now set the chassis address also */
					int eth_idx = -1;

					eth_idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET);
					if ((eth_idx < 0))
					{
						trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmDhcpConfigMessageHandler eth_idx < 0");
						return;
					}
					
					/* seeting the chassis static ip address */
					if (!ip_addr.compare("")) {
						current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);
					} else {
						string str_prefix, str_ipaddr;
						size_t pos = ip_addr.find("/");
						str_ipaddr = ip_addr.substr(0, pos);
						str_prefix = ip_addr.substr(pos+1);
						int prefix = atoi(str_prefix.c_str());
						trace (TRACE_LEVEL_INFO, string("IpadmLocalObjectManager::IpadmDhcpConfigMessageHandler address/prefix") + str_ipaddr + string("/") + prefix);
						SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr.s_addr = inet_addr(str_ipaddr.c_str());

						current.local_prefixes[eth_idx] = prefix;
						current.local_flags[eth_idx] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);
					}
				}



			}	
			rc = ipAdmIpAddrSet(&current, 0, 0);
			if (rc) {
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmDhcpMessageHandler : ipAdmIpAddrSet failed");
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
				pIpadmDhcpConfigMessage->setCompletionStatus (WRC_IPADM_IPV4_DHCP_ERROR);
				reply (pIpadmDhcpConfigMessage);
				return;
			}
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmDhcpMessageHandler : ipAdmIpAddrSet Succeeded!");
		}
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
	}		

	for (UI32 cp_num = 0; cp_num < cp_max; cp_num++) {

		UI32 mappedId = DcmToolKit::getLocalMappedId();
		string id;

		if (!pizza)
			id =  mappedId + string ("/") + (cp_num + 1);
		else
			id = mappedId + string ("/0");

		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) {
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			pIpadmDhcpConfigMessage->setCompletionStatus (WRC_IPADM_INTERNAL_ERROR);
			reply (pIpadmDhcpConfigMessage);
			delete syncQueryContext;
			return;
			
		}

		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		delete syncQueryContext;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		
		startTransaction ();
		updateWaveManagedObject (pIpadmManagedObject);

		pIpadmManagedObject->setDhcp (dhcp);
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmDhcpConfigMessageHandler : Commit to DB Success");
			status = WAVE_MESSAGE_SUCCESS;		
		}
		else 
		{
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmDhcpConfigMessageHandler : Commit To DB failed");
		}
		pIpadmDhcpConfigMessage->setCompletionStatus (status);

		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
	}		
	reply (pIpadmDhcpConfigMessage);
    }

    void  IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler(IpadmIpv6AddressConfigMessage *pIpadmIpv6AddressConfigMessage)
	{
		string id	= pIpadmIpv6AddressConfigMessage->getSwitchid ();
		string ip_address = pIpadmIpv6AddressConfigMessage->getIpv6Address ();
		
		size_t position = id.find_first_of("/");
		if (position == string::npos) {
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : position error");
			return;
		}
		string temp = id.substr(position+1);
		int cp_num = atoi(temp.c_str());
		
    	// retrieve the local and gw ID tables and address values
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
		{
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			pIpadmIpv6AddressConfigMessage->setCompletionStatus (WRC_IPADM_INTERNAL_ERROR);
			reply (pIpadmIpv6AddressConfigMessage);
			delete syncQueryContext;
			return;
		}

	    IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		delete syncQueryContext;
		string current_ip_address = pIpadmManagedObject->getIPv6Address();

		string str_prefix, str_ipaddr;
		size_t pos;

		pos = ip_address.find("/");
		str_ipaddr = ip_address.substr(0, pos);
		str_prefix = ip_address.substr(pos+1);
		int prefix = atoi(str_prefix.c_str());

		trace (TRACE_LEVEL_INFO, string ("IpadmLocalObjectManager::IpadmIpv6AddressConfigMessage : ") + id + string(" New- ") + ip_address + string(" ") + pIpadmManagedObject->getId() + string(" Old- ") + current_ip_address + string(" Prefix - ") + prefix);

		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		current.local_addr_ids = local_addr_ids;
		current.local_addrs = local_addrs;
		current.local_prefixes = local_prefixes;
		current.local_flags = local_flags;
		current.gw_addr_ids = gw_addr_ids;
		current.gw_addrs = gw_addrs;
		current.gw_flags = gw_flags;

		int rc = -1;
		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmLocalAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmIpAddrGet() returned ") + rc);
			//return;
		}
		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
		if(!pizza) {
			cp_num -= 1;
			if (cp_num < 0) {
				trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : cp_num < 0"));
			}
		}

		dump_all();
		
		//Find the index
		int eth_idx;
		if (pizza)
			eth_idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET6);
		else 
			eth_idx = locate_local_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET6);
		if (eth_idx < 0) {
			trace (TRACE_LEVEL_FATAL, string ("IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler : eth_idx is <0 ") + eth_idx);
			pIpadmIpv6AddressConfigMessage->setCompletionStatus (WRC_IPADM_IPV6_ADDRSET_ERROR);
			reply (pIpadmIpv6AddressConfigMessage);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			return;	
		}
		
		struct in6_addr addr;
		memset(&addr, 0, sizeof(struct in6_addr));
		
		if (!inet_pton(AF_INET6, str_ipaddr.c_str(), &addr)) { 
			trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler : inet_pton() failed"));
			pIpadmIpv6AddressConfigMessage->setCompletionStatus (WRC_IPADM_IPV6_ADDRSET_ERROR);
			reply (pIpadmIpv6AddressConfigMessage);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			return;
		}
		trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler : inet_pton() success"));	
		if (IN6_IS_ADDR_UNSPECIFIED(&addr)) {
			trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler : Invalid ipv6 address") + str_ipaddr);
			pIpadmIpv6AddressConfigMessage->setCompletionStatus (WRC_IPADM_IPV6_ADDRSET_ERROR);
			reply (pIpadmIpv6AddressConfigMessage);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			return;
		}
		SOCKADDR_IN6P(current.local_addrs[eth_idx])->sin6_addr = addr;
		current.local_prefixes[eth_idx] = (unsigned char) prefix;
		current.local_flags[eth_idx] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);

		dump_all();
		
		//Call library
		rc = ipAdmIpAddrSet(&current, 0, 0);
		if (rc) {
			if (rc == IPADM_ERR_DUP_ADDRESS) {
				pIpadmIpv6AddressConfigMessage->setCompletionStatus (WRC_IPADM_IPV6_ADDRSET_DUP_ERROR);
			} else {
				pIpadmIpv6AddressConfigMessage->setCompletionStatus (WRC_IPADM_IPV6_ADDRSET_ERROR);
			}
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler : ipAdmIpAddrSet failed");
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			reply (pIpadmIpv6AddressConfigMessage);
			return;
		}
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler : ipAdmIpAddrSet Succeeded!");
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		
		startTransaction ();
		updateWaveManagedObject (pIpadmManagedObject);

		pIpadmManagedObject->setIPv6Address (ip_address);
	
		status = commitTransaction ();
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler : Transcation committed");
		if (FRAMEWORK_SUCCESS == status)
		{
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler : Commit to DB Success");
			status = WAVE_MESSAGE_SUCCESS;		
		}
		else 
		{
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpv6AddressConfigMessageHandler : Commit To DB failed");
		}
		pIpadmIpv6AddressConfigMessage->setCompletionStatus (status);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
		
		reply (pIpadmIpv6AddressConfigMessage);
	}

    void  IpadmLocalObjectManager::IpadmIpv6AutoconfigMessageHandler(IpadmIpv6AutoconfigMessage *pIpadmIpv6AutoconfigMessage)
	{
		bool autoconfig = pIpadmIpv6AutoconfigMessage->getAutoconfig ();
#if 1
		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		current.local_addr_ids = local_addr_ids;
		current.local_addrs = local_addrs;
		current.local_prefixes = local_prefixes;
		current.local_flags = local_flags;
		current.gw_addr_ids = gw_addr_ids;
		current.gw_addrs = gw_addrs;
		current.gw_flags = gw_flags;

		int rc = -1;
		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmLocalAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmIpAddrGet() returned ") + rc);
			//return;
		}

		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
		
		UI32 cp_max;
		if (pizza)
			cp_max = 1;
		else
			cp_max = 2;
		
	   for (UI32 cp_num = 0; cp_num < cp_max; cp_num++) {

    	    	UI32 mappedId = DcmToolKit::getLocalMappedId();	
		string id;

		if (!pizza)
			id =  mappedId + string ("/") + (cp_num + 1);
		else
			id = mappedId + string ("/0");

	    	/* retrieve the local and gw ID tables and address values */
				
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
       		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
           			vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
        	if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) {
	                trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			pIpadmIpv6AutoconfigMessage->setCompletionStatus (WRC_IPADM_IPV6_AUTOCONF_ERROR);
			reply (pIpadmIpv6AutoconfigMessage);
                	delete syncQueryContext;
			return;
		}
		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
               	delete syncQueryContext;

        	if (pIpadmManagedObject != NULL) {
            	    	//	Now replay the configuration to the ipadm daemon
                	string ipv6_address = pIpadmManagedObject->getIPv6Address();
                	bool current_autoconfig = pIpadmManagedObject->getAutoconfig();
		
        	  	string str_prefix, str_ipaddr;

			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmAutoconfigMessageHandler : Got these -------" + id + " " + autoconfig + " " + pIpadmManagedObject->getId() + " " + current_autoconfig);

        		trace (TRACE_LEVEL_ERROR, string ("currrent dhcp flag before ") + current.dhcp_flag);
			if (autoconfig == true) {
				current.dhcp_flag |=  IPADM_AUTOCONFIG_ENABLE;
			} else {
				current.dhcp_flag &=  ~IPADM_AUTOCONFIG_ENABLE;
		        }
	        }
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmAutoconfigMessageHandler : ipAdmIpAddrSet Succeeded!");
	#endif
		if (cp_max == 1 || (cp_max == 2 && cp_num == 1)) {
			rc = ipAdmIpAddrSet(&current, 0, 0);
			if (rc) {
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmAutoconfigMessageHandler : ipAdmIpAddrSet failed");
				pIpadmIpv6AutoconfigMessage->setCompletionStatus (WRC_IPADM_IPV6_AUTOCONF_ERROR);
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
				reply (pIpadmIpv6AutoconfigMessage);
				return;
			}
		}
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
	   }	
	   for (UI32 cp_num = 0; cp_num < cp_max; cp_num++) {

    	    	UI32 mappedId = DcmToolKit::getLocalMappedId();	
		string id;

		if (!pizza)
			id =  mappedId + string ("/") + (cp_num + 1);
		else
			id = mappedId + string ("/0");

	    	/* retrieve the local and gw ID tables and address values */
				
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
       		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
           			vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
        	if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) {
	                trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			pIpadmIpv6AutoconfigMessage->setCompletionStatus (WRC_IPADM_IPV6_AUTOCONF_ERROR);
			reply (pIpadmIpv6AutoconfigMessage);
                	delete syncQueryContext;
			return;
		}
		IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
               	delete syncQueryContext;


		ResourceId status = WAVE_MESSAGE_SUCCESS;
			
		startTransaction ();
		updateWaveManagedObject (pIpadmManagedObject);
	
		pIpadmManagedObject->setAutoconfig (autoconfig);
		status = commitTransaction ();
		if (FRAMEWORK_SUCCESS == status)
		{
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmAutoconfigMessageHandler : Commit to DB Success");
			status = WAVE_MESSAGE_SUCCESS;		
		}
		else 
		{
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmAutoconfigMessageHandler : Commit To DB failed");
		}
		pIpadmIpv6AutoconfigMessage->setCompletionStatus (status);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
	   }	
		reply (pIpadmIpv6AutoconfigMessage);
	}

    void  IpadmLocalObjectManager::IpadmRemoveMessageHandler(IpadmRemoveMessage *pIpadmRemoveMessage)
	{
		string id	= pIpadmRemoveMessage->getSwitchid ();
		UI32 tag = pIpadmRemoveMessage->getTag ();
		
		size_t position = id.find_first_of("/");
		if (position == string::npos) {
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : position error");
			return;
		}
		string temp = id.substr(position+1);
		int cp_num = atoi(temp.c_str());
		
    	/* retrieve the local and gw ID tables and address values */
		
		WaveManagedObjectSynchronousQueryContext *syncQueryContext =
			new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
		syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
		vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
		if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
		{
			trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
			pIpadmRemoveMessage->setCompletionStatus (WRC_IPADM_INTERNAL_ERROR);
			reply (pIpadmRemoveMessage);
			delete syncQueryContext;
			return;
		}
		
	    IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
		delete syncQueryContext;

		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		current.local_addr_ids = local_addr_ids;
		current.local_addrs = local_addrs;
		current.local_prefixes = local_prefixes;
		current.local_flags = local_flags;
		current.gw_addr_ids = gw_addr_ids;
		current.gw_addrs = gw_addrs;
		current.gw_flags = gw_flags;

		int rc = -1;
		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmLocalAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmIpAddrGet() returned ") + rc);
			//return;
		}
		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
		if(!pizza) {
			cp_num -= 1;
			if (cp_num < 0) {
				trace (TRACE_LEVEL_ERROR, string ("IpadmLocalObjectManager::IpadmIpaddressconfigMessageHandler : cp_num < 0"));
			}
		}
		
		if (tag == brocade_interface_ip_address) {
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmRemoveMessageHandler : REMOVE IP ADDRESS");
			int eth_idx;
			if (pizza)
				eth_idx = locate_local_idx(&current, SWITCH_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET);
			else 
				eth_idx = locate_local_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET);
			if (eth_idx < 0) {
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmRemoveMessageHandler : eth_idx < 0");
				pIpadmRemoveMessage->setCompletionStatus (WRC_IPADM_IPV4_ADDRCLR_ERROR);
				reply (pIpadmRemoveMessage);
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
				return;
			}
			current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);
		}
		else if (tag == brocade_interface_gateway_address) {
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmRemoveMessageHandler : REMOVE GATEWAY");
#if 0
			int eth_idx = locate_gw_idx(&current, SPECIFIC_CP_HOST, 0, AF_INET);
			if (eth_idx < 0) {
				trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmRemoveMessageHandler : could not locate gateway idx");
				pIpadmRemoveMessage->setCompletionStatus (WRC_IPADM_IPV4_GWCLR_ERROR);
				reply (pIpadmRemoveMessage);
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
				return;
			}
			current.gw_flags[eth_idx] |= IPADM_ADDR_NONE;
#endif
		}
		else if (tag == brocade_interface_ipv6_global_address) {
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmRemoveMessageHandler : REMOVE IPv6 ADDRESS");
			int eth_idx;
			if (pizza)
				eth_idx = locate_local_idx(&current, SWITCH_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET6);
			else 
				eth_idx = locate_local_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET6);
			if (eth_idx < 0) {
				trace (TRACE_LEVEL_FATAL, string ("IpadmLocalObjectManager::IpadmRemoveMessageHandler : eth_idx is <0 ") + eth_idx);
				pIpadmRemoveMessage->setCompletionStatus (WRC_IPADM_IPV6_ADDRCLR_ERROR);
				reply (pIpadmRemoveMessage);
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
				return;	
			}
			current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);
		}
		else {
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmRemoveMessageHandler : INVALID REMOVE");
			pIpadmRemoveMessage->setCompletionStatus (WRC_IPADM_IPV6_INV_REMOVE);
			reply (pIpadmRemoveMessage);
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
			return;
		}


#if 1
		rc = ipAdmIpAddrSet(&current, 0, 0);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmRemoveMessageHandler : ipAdmIpAddrSet failed");
			pIpadmRemoveMessage->setCompletionStatus (WRC_IPADM_IPV6_ADDRSET_ERROR);
			reply (pIpadmRemoveMessage);
			return;
		}
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmRemoveMessageHandler : ipAdmIpAddrSet Succeeded!");
#endif

		ResourceId status = WAVE_MESSAGE_SUCCESS;

#if 1	
		startTransaction ();
		updateWaveManagedObject (pIpadmManagedObject);

		if (tag == brocade_interface_ip_address)
			pIpadmManagedObject->setIPv4Address ("");
#if 0
		if (tag == brocade_interface_gateway_address) {
			IpV4Address ip_gateway;
			ip_gateway.fromString("0.0.0.0");	
			pIpadmManagedObject->setIPv4Gateway (ip_gateway);
		}
#endif
		if (tag == brocade_interface_ipv6_global_address)
			pIpadmManagedObject->setIPv6Address ("");
			
		status = commitTransaction ();
#endif
		if (FRAMEWORK_SUCCESS == status)
		{
			trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmAutoconfigMessageHandler : Commit to DB Success");
			status = WAVE_MESSAGE_SUCCESS;		
		}
		else 
		{
			trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmAutoconfigMessageHandler : Commit To DB failed");
		}
		pIpadmRemoveMessage->setCompletionStatus (status);
		reply (pIpadmRemoveMessage);
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);

	}

	void IpadmLocalObjectManager::dump_all()
	{
		FILE *fp;
		fp = fopen("/tmp/ipadm_dcmd_data.txt", "a+");
		
		if (!fp) {
			trace (TRACE_LEVEL_ERROR, "fopen(/tmp/ipadm_dcmd_data.txt) failed");
			return;
		}

		trace (TRACE_LEVEL_DEBUG, "fopen(/tmp/ipadm_dcmd_data.txt) success");
		
		dump_get_set(fp, &current);
		fclose(fp);

		return;
	}

	/*
	 * dumps the local address ID at p_id to fp
	 */

	void IpadmLocalObjectManager::dump_local_id(FILE *fp, ipadm_local_addr_id_t *p_id)
	{
		fprintf(fp, "host %d/%d, interface %d/%d, entity %d/%d, "
				"family %d, tag %d",
			p_id->host_type,
			p_id->host_instance,
			p_id->interface_type,
			p_id->interface_instance,
			p_id->entity_type,
			p_id->entity_instance,
			p_id->addr_family,
			p_id->tag);
	}

	/*
	 * this function dumps the contents of the get/set at p_getset to fp
	 */

	void IpadmLocalObjectManager::dump_get_set(FILE *fp, ipadm_ip_get_set_t *p_getset)
	{
		int i;
		int num_locals, num_gws;
		char buf[100];

		fprintf (fp, "num_locals %d, num_gws %d, dhcp_flag 0x%x\n",
			p_getset->num_locals, p_getset->num_gws, p_getset->dhcp_flag);

		num_locals = p_getset->num_locals;
		if (num_locals > MAX_LOCAL_ADDR_TABLE_SIZE)
			num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		num_gws = p_getset->num_gws;
		if (num_gws > MAX_GW_ADDR_TABLE_SIZE)
			num_gws = MAX_GW_ADDR_TABLE_SIZE;

		for (i = 0; i < num_locals; ++i) {
			fprintf (fp, "[%d] ", i);
			dump_local_id(fp, &(p_getset->local_addr_ids[i]));
			buf[0] = 0;
			inet_ntop(p_getset->local_addr_ids[i].addr_family,
				(p_getset->local_addr_ids[i].addr_family == AF_INET6) ?
				(void *) &(SOCKADDR_IN6P(p_getset->local_addrs[i])->sin6_addr) :
				(void *) &(SOCKADDR_INP(p_getset->local_addrs[i])->sin_addr),
				buf, sizeof(buf));
			fprintf(fp, ", flags 0x%x, prefix %d, addr %s\n",
				p_getset->local_flags[i],
				p_getset->local_prefixes[i], buf);
		}

		for (i = 0; i < num_gws; ++i) {
			buf[0] = 0;
			inet_ntop(p_getset->gw_addr_ids[i].addr_family,
				(p_getset->gw_addr_ids[i].addr_family == AF_INET6) ?
				(void *) &(SOCKADDR_IN6P(p_getset->gw_addrs[i])->sin6_addr) :
				(void *) &(SOCKADDR_INP(p_getset->gw_addrs[i])->sin_addr),
				buf, sizeof(buf));
			fprintf(fp, "[%d] host %d/%d, "
					"family %d, tag %d, flags 0x%x, addr %s\n", i,
				p_getset->gw_addr_ids[i].host_type,
				p_getset->gw_addr_ids[i].host_instance,
				p_getset->gw_addr_ids[i].addr_family,
				p_getset->gw_addr_ids[i].tag,
				p_getset->gw_flags[i], buf);
		}
	}
void  IpadmLocalObjectManager::IpadmIpv6DhcpConfigMessageHandler(IpadmIpv6DhcpConfigMessage *pIpadmIpv6DhcpConfigMessage)
{
        bool dhcpv6 = pIpadmIpv6DhcpConfigMessage->getIpv6Dhcp ();
        current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
        current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

        current.local_addr_ids = local_addr_ids;
        current.local_addrs = local_addrs;
        current.local_prefixes = local_prefixes;
        current.local_flags = local_flags;
        current.gw_addr_ids = gw_addr_ids;
        current.gw_addrs = gw_addrs;
        current.gw_flags = gw_flags;

        int rc = -1;
        rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
        if (rc) {
            trace (TRACE_LEVEL_ERROR, string ("ipAdmLocalAddrIdGet() returned ") + rc);
            //return;
        }
        rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
        if (rc) {
            trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
            //return;
        }
        rc = ipAdmIpAddrGet(&current);
        if (rc) {
            trace (TRACE_LEVEL_ERROR, string ("ipAdmIpAddrGet() returned ") + rc);
            //return;
        }                                                                                                          
	int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);

	UI32 cp_max;
	if (pizza)
		cp_max = 1;
	else
		cp_max = 2;
	
    for (UI32 cp_num = 0; cp_num < cp_max; cp_num++) {

       	UI32 mappedId = DcmToolKit::getLocalMappedId();	
	string id;

	if (!pizza)
		id =  mappedId + string ("/") + (cp_num + 1);
	else
		id = mappedId + string ("/0");
			
        WaveManagedObjectSynchronousQueryContext *syncQueryContext =
        new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
        syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
        vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
        if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
        {
            trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
            pIpadmIpv6DhcpConfigMessage->setCompletionStatus (WRC_IPADM_IPV6_DHCP_ERROR);
            reply (pIpadmIpv6DhcpConfigMessage);
            delete syncQueryContext;
	    return;
	}
        IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
        delete syncQueryContext;

        if (pIpadmManagedObject != NULL) {
        //	Now replay the configuration to the ipadm daemon
            string ipv6_address = pIpadmManagedObject->getIPv6Address();
            bool current_dhcpv6 = pIpadmManagedObject->getIpv6Dhcp();
		
            string str_prefix, str_ipaddr;
            trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpv6DhcpConfigMessageHandler : Got these -------" + id + " " + dhcpv6 + " " + pIpadmManagedObject->getId() + " " + current_dhcpv6);

            trace (TRACE_LEVEL_ERROR, string ("currrent dhcp flag before ") + current.dhcp_flag);
            if (dhcpv6 == true) {
                 current.dhcp_flag |=  IPADM_DHCPV6_ENABLE;
            } else {
                current.dhcp_flag &=  ~IPADM_DHCPV6_ENABLE;

                int eth_idx;
                if (pizza)
                    eth_idx = locate_local_idx(&current, SWITCH_ENTITY, 0, ETHERNET_INTERFACE, 0, AF_INET6);
                else
                    eth_idx = locate_local_idx(&current, CP_ENTITY, cp_num, ETHERNET_INTERFACE, 0, AF_INET6);

                if (eth_idx < 0) {
                    trace (TRACE_LEVEL_FATAL, string ("IpadmLocalObjectManager::IpadmIpv6DhcpConfigMessageHandler : eth_idx is <0 ") + eth_idx);
                } else {
                    if (!ipv6_address.compare("")) {
                        current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);
                    }
                }
            }
        }
	if (cp_max == 1 || (cp_max == 2 && cp_num == 1)) {
        	rc = ipAdmIpAddrSet(&current, 0, 0);
        	if (rc) {
        	    trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpv6DhcpConfigMessageHandler : ipAdmIpAddrSet failed");
        	    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
        	    pIpadmIpv6DhcpConfigMessage->setCompletionStatus (WRC_IPADM_IPV6_DHCP_ERROR);
        	    reply (pIpadmIpv6DhcpConfigMessage);
        	    return;
        	}
	}
        trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpv6DhcpConfigMessageHandler : ipAdmIpAddrSet Succeeded!");
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
    }
    for (UI32 cp_num = 0; cp_num < cp_max; cp_num++) {

       	UI32 mappedId = DcmToolKit::getLocalMappedId();	
	string id;

	if (!pizza)
		id =  mappedId + string ("/") + (cp_num + 1);
	else
		id = mappedId + string ("/0");
			
        WaveManagedObjectSynchronousQueryContext *syncQueryContext =
        new WaveManagedObjectSynchronousQueryContext(IpadmLocalManagedObject::getClassName());
        syncQueryContext->addAndAttribute (new AttributeString (id, "switchid"));
        vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
        if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
        {
            trace (TRACE_LEVEL_ERROR, "No such managed object" + id);
            pIpadmIpv6DhcpConfigMessage->setCompletionStatus (WRC_IPADM_IPV6_DHCP_ERROR);
            reply (pIpadmIpv6DhcpConfigMessage);
            delete syncQueryContext;
	    return;
	}
        IpadmLocalManagedObject *pIpadmManagedObject = dynamic_cast<IpadmLocalManagedObject *> ((*pResultsRollback)[0]);
        delete syncQueryContext;


        ResourceId status = WAVE_MESSAGE_SUCCESS;

        startTransaction ();
        updateWaveManagedObject (pIpadmManagedObject);

        pIpadmManagedObject->setIpv6Dhcp (dhcpv6);
        status = commitTransaction ();
        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::IpadmIpv6DhcpConfigMessageHandler : Commit to DB Success");
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "IpadmLocalObjectManager::IpadmIpv6DhcpConfigMessageHandler : Commit To DB failed");
        }
        pIpadmIpv6DhcpConfigMessage->setCompletionStatus (status);
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsRollback);
    }
        reply (pIpadmIpv6DhcpConfigMessage);

	}

	int IpadmLocalObjectManager::sendcmd(const string &id, const char *cmd)
	{
		static bool slotbased = ham_is_slotbased();
		if (slotbased) {
			string myid = DcmToolKit::getLocalMappedId() + string("/") + (1+ham_get_cpid());
			if (myid != id) {
				char peercmd[220];
				sprintf(peercmd, "rsh 127.2.1.%d '%s'", ham_get_peer_slotno(), cmd);
				trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::sendcmd : " + string(peercmd));
				return system(peercmd);
			}
		}
		trace (TRACE_LEVEL_INFO, "IpadmLocalObjectManager::sendcmd : " + string(cmd));
		return system(cmd);
    }
}
