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
 *   Author : ckunduru                                                     *
 **************************************************************************/

#ifndef RADIUSHOSTMANAGEDOBJECT_H
#define RADIUSHOSTMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;

class RadiusHostManagedObject : virtual public DcmManagedObject
{
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
	virtual void setupKeys();
  public:
	RadiusHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	RadiusHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const string &host,const UI16 &port,const AuthProtocol &protocol,const string &key,const UI32 &retransmit,const UI32 &timeout);
	virtual    ~RadiusHostManagedObject ();
	static string getClassName();
	void setIndex(const UI16 &index);
	UI16 getIndex()  const;
	void setHost(const string &host);
	string getHost()  const;
	void setPort(const UI16 &port);
	UI16 getPort()  const;
	void setProtocol(const AuthProtocol &protocol);
	AuthProtocol getProtocol()  const;
	void setKey(const string &key);
	string getKey()  const;
	void setRetransmit(const UI32 &retransmit);
	UI32 getRetransmit()  const;
	void setTimeout(const UI32 &timeout);
	UI32 getTimeout()  const;

  /* Data Members */
  private:
	UI16 m_index;
	string  m_host;
	UI16  m_port;
	AuthProtocol  m_protocol;
	string  m_key;
	UI32  m_retransmit;
	UI32  m_timeout;
};

}
#endif                                            //RADIUSHOSTMANAGEDOBJECT_H
