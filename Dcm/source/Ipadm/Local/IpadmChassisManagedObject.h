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

#ifndef IPADMCHASSISMANAGEDOBJECT_H
#define IPADMCHASSISMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Types/IpV6Address.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Ipadm/Local/IpadmTypes.h"

#include <ha/ipadm.h>

using namespace WaveNs;

namespace DcmNs
{

	class IpadmChassisObjectManager;
	class IpadmChassisManagedObject : virtual public DcmLocalManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
			virtual void loadOperationalData(WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext);
		public:
			IpadmChassisManagedObject (IpadmChassisObjectManager *pIpadmChassisObjectManager);
			IpadmChassisManagedObject (IpadmChassisObjectManager *pIpadmChassisObjectManager,const string &VipAddress, const string &VipV6Address, const string &oper_Vip_address, const vector<string>  &oper_vector_VipV6_address);
			virtual	~IpadmChassisManagedObject ();
			static string getClassName();
			void setVipAddress(const string &VipAddress);
			string getVipAddress()  const;
			void setVipV6Address(const string &VipV6Address);
			string getVipV6Address()  const;
                        void set_oper_Vip_address(const string &oper_Vip_address);
                        void set_oper_vector_VipV6_address(const vector<string> &oper_vector_VipV6_address);
                        string get_oper_Vip_address()  const;
                        vector<string> get_oper_vector_VipV6_address()  const;

                        int locate_local_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family);
                        int locate_addr_idx(ipadm_ip_get_set_t *p_getset, ipadm_entity_t entity, int entity_instance, ipadm_interface_t interface, int interface_instance, int addr_family, int nentry);


// Now the data members

		private:
		protected:
		public:
			string  m_VipAddress;
			string  m_VipV6Address;
			string  m_oper_Vip_address;
                        vector<string>  m_oper_vector_VipV6_address;

	};
}
#endif											//IPADMCHASSISMANAGEDOBJECT_H
