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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.		  *
 *   All rights reserved.												  *
 *   Author : cshah													 *
 **************************************************************************/

#ifndef IPADMCHASSISOBJECTMANAGER_H
#define IPADMCHASSISOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include <ha/ipadm.h>
#include "Ipadm/Local/IpadmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

	class IpadmChassisIpConfigMessage;
	class IpadmChassisIpDeleteMessage;
	
	class IpadmChassisObjectManager: public WaveLocalObjectManager
	{
		private:
			PrismMessage *createMessageInstance (const UI32 &operationCode);
			WaveManagedObject *createManagedObjectInstance (const string &managedClassName);
			void IpadmChassisIpConfigMessageHandler (IpadmChassisIpConfigMessage *pIpadmChassisIpConfigMessage);
			void IpadmChassisIpDeleteMessageHandler (IpadmChassisIpDeleteMessage *pIpadmChassisIpDeleteMessage);
			void postboot (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
        	void install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			int locate_local_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family);
			int locate_gw_idx(ipadm_ip_get_set_t *p_getset, ipadm_host_t host_type, int host_instance, int addr_family);

		protected:
		public:
			IpadmChassisObjectManager ();
			virtual	~IpadmChassisObjectManager ();
			static IpadmChassisObjectManager *getInstance();
			static string getClassName();
			static PrismServiceId getPrismServiceId ();

		private:

		protected:
		public:
	};
}
#endif											//IPADMCHASSISOBJECTMANAGER_H
