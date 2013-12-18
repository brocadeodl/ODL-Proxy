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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : Venkat														*
 ****************************************************************************/

#ifndef MACGROUPANAGEDOBJECT_H
#define MACGROUPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"

using namespace WaveNs;

namespace DcmNs
{
	class MacGroupEntryManagedObject : virtual public DcmManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			MacGroupEntryManagedObject (WaveObjectManager *pWaveObjectManager);
			MacGroupEntryManagedObject (WaveObjectManager *pWaveObjectManager, const string &address);

			virtual ~MacGroupEntryManagedObject ();
			static string getClassName();
			void setAddress(const string &address);
			string getAddress() const;
			void setMask(const string &mask);
			string getMask() const;

// Now the data members
		private:
		protected:
		public:
			string m_address;
			string m_mask;
	};

	class MacGroupManagedObject : virtual public DcmManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			MacGroupManagedObject (WaveObjectManager *pWaveObjectManager);
			MacGroupManagedObject (WaveObjectManager *pWaveObjectManager, const UI32 &id);

			virtual ~MacGroupManagedObject ();
			static string getClassName();
			void setMacGroupId(const UI32 &id);
			UI32 getMacGroupId() const;
			vector<WaveManagedObjectPointer<MacGroupEntryManagedObject> > getMacGroupEntry() const;

// Now the data members
		private:
		protected:
		public:
			UI32 m_id;
            vector<WaveManagedObjectPointer<MacGroupEntryManagedObject> >  m_entry;
	};
}

#endif		//MACGROUPMANAGEDOBJECT_H

