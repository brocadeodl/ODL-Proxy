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

#ifndef IPCOMMUNITYSTDACCESSLISTLOCALMANAGEDOBJECT_H_
#define IPCOMMUNITYSTDACCESSLISTLOCALMANAGEDOBJECT_H_

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Framework/Attributes/AttributeStringVectorUC.h"
#include "Framework/Types/StringVectorUC.h"

using namespace WaveNs;

namespace DcmNs
{

class IpPolicyLocalObjectManager;
class IpPolicyLocalWorker;

class IpCommunityStdAccessListLocalManagedObject : virtual public DcmLocalManagedObject
{
	private:
	friend class IpPolicyLocalWorker;
	protected:
		virtual void setupAttributesForPersistence();
		virtual void setupAttributesForCreate();
		virtual void setupKeys();
	public:
		IpCommunityStdAccessListLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager);
		IpCommunityStdAccessListLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager,const string &name,
			const UI32 &seq,
			const UI16 &instance,
			const PolicyAction &action,
			const string &communityExpr);
		virtual    ~IpCommunityStdAccessListLocalManagedObject ();
		static string getClassName();
		void setInstance(const UI16 &instance);
		UI16 getInstance()  const;
		void setAction(const PolicyAction &action);
		PolicyAction getAction()  const;
		string getCommunityExpr() const;
		void setCommunityExpr(const string &communityExpr);

// Now the data members

	private:
		UI32  m_seq;
		UI16  m_instance;
		PolicyAction  m_action;
		StringUC m_communityExpr;
};

}

#endif /* IPCOMMUNITYSTDACCESSLISTLOCALMANAGEDOBJECT_H_ */
