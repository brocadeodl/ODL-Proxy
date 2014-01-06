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

/********************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.					*
 *   All rights reserved.														*
 *   Author : Venkat															*
 ********************************************************************************/

#ifndef MACBASEDVLANCLASSIFICATIONMO_H
#define MACBASEDVLANCLASSIFICATIONMO_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmLocalObjectManager;
	class NsmGlobalObjectManager;
	class MacAddressVlanClassificationMO : virtual public DcmLocalManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			MacAddressVlanClassificationMO (NsmLocalObjectManager *pNsmLocalObjectManager);
			MacAddressVlanClassificationMO (NsmGlobalObjectManager *pNsmGlobalObjectManager);

			virtual ~MacAddressVlanClassificationMO();
			static string getClassName();
			void setAccessVlanId(const UI32 &accessVlanId);
			UI32 getAccessVlanId() const;
			void setMac(const string &mac);
			string getMac() const;

		private:
		protected:
		public:
			UI32 m_accessVlanId;
			string m_mac;
	};

	class MacGroupVlanClassificationMO : virtual public DcmLocalManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			MacGroupVlanClassificationMO (NsmLocalObjectManager *pNsmLocalObjectManager);
			MacGroupVlanClassificationMO (NsmGlobalObjectManager *pNsmGlobalObjectManager);

			virtual ~MacGroupVlanClassificationMO();
			static string getClassName();
			void setVlanId(const UI32 &vlanId);
			UI32 getVlanId() const;
			void setMacGroupId(const UI32 &macGroupId);
			UI32 getMacGroupId() const;

		private:
		protected:
		public:
			UI32 m_vlanId;
			UI32 m_macGroupId;
	};
}
#endif //MACBASEDVLANCLASSIFICATIONMO_H
