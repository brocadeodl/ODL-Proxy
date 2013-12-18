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

/*
 * RspanGVlanTrunkPortManagedObject.h
 *
 *  Created on: Jun 6, 2013
 *      Author: sunniram
 */

#ifndef RSPANGVLANTRUNKPORTMANAGEDOBJECT_H_
#define RSPANGVLANTRUNKPORTMANAGEDOBJECT_H_

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmLocalObjectManager;
	class RspanGVlanTrunkPortManagedObject : virtual public DcmLocalManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			RspanGVlanTrunkPortManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);

			virtual ~RspanGVlanTrunkPortManagedObject();
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

	class RspanGVlanTrunkPortRemoveManagedObject : virtual public DcmLocalManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			RspanGVlanTrunkPortRemoveManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
			virtual ~RspanGVlanTrunkPortRemoveManagedObject();
			static string getClassName();
		private:
		protected:
		public:
			UI32 m_vlanId;
			UI32 m_ctagId;
	};
}


#endif /* RSPANGVLANTRUNKPORTMANAGEDOBJECT_H_ */
