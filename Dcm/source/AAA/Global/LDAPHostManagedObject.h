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
 *   Author : kghanta                                                     *
 **************************************************************************/

#ifndef LDAPHOSTMANAGEDOBJECT_H
#define LDAPHOSTMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;

class LDAPHostManagedObject : virtual public DcmManagedObject
{
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
	virtual void setupKeys();
  public:
	LDAPHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	LDAPHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const string &host, const UI16 &port, const string &basedn, const UI8 &retries, const UI8 &timeout);
	virtual ~LDAPHostManagedObject ();
	static string getClassName();
	void setIndex(const UI16 &index);
	UI16 getIndex()  const;
	void setHost(const string &host);
	string getHost() const;
	void setPort(const UI16 &port);
	UI16 getPort() const;
	void setBasedn(const string &basedn);
	string getBasedn() const;
	void setRetries(const UI8 &retries);
	UI8 getRetries() const;
	void setTimeout(const UI8 &timeout);
	UI8 getTimeout() const;

  /* Data members */
  private:
	UI16 m_index;
	string m_host;
	UI16 m_port;
	string m_basedn;
	UI8 m_retries;
	UI8 m_timeout;
};

}
#endif //LDAPHOSTMANAGEDOBJECT_H
