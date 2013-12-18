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

#ifndef VIRTUALIPGLOBALMANAGEDOBJECT_H
#define VIRTUALIPGLOBALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

	class VcsFabObjectManager;
	class VirtualIpGlobalManagedObject : public DcmManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			VirtualIpGlobalManagedObject (VcsFabObjectManager *pVcsFabObjectManager);
			VirtualIpGlobalManagedObject (VcsFabObjectManager *pVcsFabObjectManager,const string &VipAddress);
			virtual	~VirtualIpGlobalManagedObject ();
			static string getClassName();
			void setVipAddress(const string &VipAddress);
			string getVipAddress()  const;

// Now the data members

		private:
		protected:
		public:
			string  m_VipAddress;
	};
}
#endif											//VIRTUALIPGLOBALMANAGEDOBJECT_H
