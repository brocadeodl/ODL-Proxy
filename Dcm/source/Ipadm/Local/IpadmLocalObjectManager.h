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

#ifndef IPADMLOCALOBJECTMANAGER_H
#define IPADMLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

#include "Ipadm/Local/IpadmLocalDnsCfgMessage.h"
#include "Ipadm/Local/IpadmLocalMessageContext.h"
#include "Ipadm/Local/IpadmIpv6RouteLocalManagedObject.h"
#include "Ipadm/Local/IpadmChassisManagedObject.h"

#include <ha/ipadm.h>
#include "Ipadm/Local/IpadmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpadmIPv4ConfigureMessage;
    class IpadmIpaddressconfigMessage;
    class IpadmGatewayconfigMessage;
    class IpadmIcmpUnreachableConfigMessage;
    class IpadmIcmpEchoreplyConfigMessage;
    class IpadmIcmpRatelimitingConfigMessage;
    class IpadmIcmpv6UnreachableConfigMessage;
    class IpadmIcmpv6EchoreplyConfigMessage;
    class IpadmIcmpv6RatelimitingConfigMessage;
	class IpadmDhcpConfigMessage;
	class IpadmIpv6AddressConfigMessage;
	class IpadmIpv6AutoconfigMessage;
	class IpadmRemoveMessage;
	class IpadmLinkSpeedConfigMessage;
    class IpadmLocalDnsCfgMessage;
	class IpadmIpv6RouteconfigMessage;
	class IpadmIpv6DhcpConfigMessage;

    class IpadmLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void IpadmGatewayconfigMessageHandler( IpadmGatewayconfigMessage *pIpadmGatewayconfigMessage);
            void IpadmIcmpUnreachableConfigMessageHandler( IpadmIcmpUnreachableConfigMessage *pIpadmIcmpUnreachableConfigMessage);
            void IpadmIcmpEchoreplyConfigMessageHandler( IpadmIcmpEchoreplyConfigMessage *pIpadmIcmpEchoreplyConfigMessage);
            void IpadmIcmpRatelimitingConfigMessageHandler( IpadmIcmpRatelimitingConfigMessage *pIpadmIcmpRatelimitingConfigMessage);
            void IpadmIcmpv6UnreachableConfigMessageHandler( IpadmIcmpv6UnreachableConfigMessage *pIpadmIcmpv6UnreachableConfigMessage);
            void IpadmIcmpv6EchoreplyConfigMessageHandler( IpadmIcmpv6EchoreplyConfigMessage *pIpadmIcmpv6EchoreplyConfigMessage);
            void IpadmIcmpv6RatelimitingConfigMessageHandler( IpadmIcmpv6RatelimitingConfigMessage *pIpadmIcmpv6RatelimitingConfigMessage);
            void IpadmIPv4ConfigureMessageHandler( IpadmIPv4ConfigureMessage *pIpadmIPv4ConfigureMessage);
            void IpadmIpaddressconfigMessageHandler( IpadmIpaddressconfigMessage *pIpadmIpaddressconfigMessage);
            void IpadmDhcpConfigMessageHandler( IpadmDhcpConfigMessage *pIpadmDhcpConfigMessage);
			void IpadmIpv6AddressConfigMessageHandler( IpadmIpv6AddressConfigMessage *pIpadmIpv6AddressConfigMessage);
			void IpadmIpv6AutoconfigMessageHandler( IpadmIpv6AutoconfigMessage *pIpadmIpv6AutoconfigMessage);
    		void IpadmRemoveMessageHandler(IpadmRemoveMessage *pIpadmRemoveMessage);
        	void install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
			int locate_local_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family);
			int locate_gw_idx(ipadm_ip_get_set_t *p_getset, ipadm_host_t host_type, int host_instance, int addr_family);
			void dump_all();
			void dump_get_set(FILE *fp, ipadm_ip_get_set_t *p_getset);
			void dump_local_id(FILE *fp, ipadm_local_addr_id_t *p_id);
            void IpadmLinkSpeedConfigMessageHandler( IpadmLinkSpeedConfigMessage *pIpadmLinkSpeedConfigMessage);
	    int setLinkSpeed(char* ifname, LinkSpeed speed);
	    void IpadmConfigMessageHandler (IpadmLocalDnsCfgMessage *pIpadmLocalDnsCfgMessage);
	    ResourceId dnsConfigHandler(IpadmLocalMessageContext *pIpadmLocalMessageContext);
	    void dnsConfigBoot();
		void IpadmIpv6RouteconfigMessageHandler(IpadmIpv6RouteconfigMessage *pIpadmIpv6RouteconfigMessage);
		void RouteExecCommand(char *cmdline, char *errBuf);
		void IpadmIpv6DhcpConfigMessageHandler( IpadmIpv6DhcpConfigMessage *pIpadmIpv6DhcpConfigMessage);
		int		sendcmd(const string &id, const char *cmd);
		
	protected:
        public:
            IpadmLocalObjectManager ();
            virtual    ~IpadmLocalObjectManager ();
            static IpadmLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:
			ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
			struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
			ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
			addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
			ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
			struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
			addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
			ipadm_ip_get_set_t current; 

        protected:
        public:
    };
}
#endif                                            //IPADMLOCALOBJECTMANAGER_H
