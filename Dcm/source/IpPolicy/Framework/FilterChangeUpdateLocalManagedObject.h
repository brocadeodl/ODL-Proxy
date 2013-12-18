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
 *   Copyright (C) 2010-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#ifndef FILTERCHANGEUPDATELOCALMANAGEDOBJECT_H_
#define FILTERCHANGEUPDATELOCALMANAGEDOBJECT_H_

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"

using namespace WaveNs;

namespace DcmNs
{

class IpPolicyLocalObjectManager;
class IpPolicyLocalWorker;

class FilterChangeUpdateLocalManagedObject : virtual public DcmLocalManagedObject
{
	private:
	friend class IpPolicyLocalWorker;
	protected:
		virtual void setupAttributesForPersistence();
		virtual void setupAttributesForCreate();
		virtual void setupKeys();
	public:
		FilterChangeUpdateLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager);
		FilterChangeUpdateLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager,const UI32 &filterUpdateDelay);
		virtual    ~FilterChangeUpdateLocalManagedObject ();
		static string getClassName();
		void setFilterUpdateDelay(const UI32 &filterUpdateDelay);
		UI32 getFilterUpdateDelay()  const;

// Now the data members

	private:
		UI32  m_filterUpdateDelay;
};

}

#endif /* FILTERCHANGEUPDATELOCALMANAGEDOBJECT_H_ */
