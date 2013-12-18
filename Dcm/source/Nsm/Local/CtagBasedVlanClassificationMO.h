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

#ifndef CTAGBASEDVLANCLASSIFICATIONMO_H
#define CTAGBASEDVLANCLASSIFICATIONMO_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

	class VlanClassificationMO : virtual public DcmLocalManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
            virtual void setupKeys();
		public:
			VlanClassificationMO (WaveObjectManager *pWaveObjectManager);
			virtual ~VlanClassificationMO();
			static string getClassName();
			void setVlanId(const UI32 &vlanId);
			UI32 getVlanId() const;
			void setCtagId(const UI32 &ctagId);
			UI32 getCtagId() const;

		private:
		protected:
		public:
			UI32 m_vlanId;
			UI32 m_ctagId;
	};

	class CtagVlanClassificationMO : public VlanClassificationMO
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			CtagVlanClassificationMO (WaveObjectManager *pWaveObjectManager);
			virtual ~CtagVlanClassificationMO();
			static string getClassName();
	};

	class CtagVlanClassificationRemoveMO : public VlanClassificationMO
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			CtagVlanClassificationRemoveMO (WaveObjectManager *pWaveObjectManager);
			virtual ~CtagVlanClassificationRemoveMO();
			static string getClassName();
	};

	class CtagPVlanClassificationMO : public VlanClassificationMO
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			CtagPVlanClassificationMO (WaveObjectManager *pWaveObjectManager);
			virtual ~CtagPVlanClassificationMO();
			static string getClassName();
	};

	class CtagPVlanClassificationRemoveMO : public VlanClassificationMO
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			CtagPVlanClassificationRemoveMO (WaveObjectManager *pWaveObjectManager);
			virtual ~CtagPVlanClassificationRemoveMO();
			static string getClassName();
	};
}
#endif // CTAGBASEDVLANCLASSIFICATIONMO_H


