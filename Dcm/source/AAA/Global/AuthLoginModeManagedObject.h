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

#ifndef AUTHLOGINMODEMANAGEDOBJECT_H
#define AUTHLOGINMODEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;

class AuthLoginModeManagedObject : virtual public DcmManagedObject
{
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
  public:
	AuthLoginModeManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	AuthLoginModeManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const AAAServerType &first,const AAAServerType &second);
	virtual    ~AuthLoginModeManagedObject ();
	static string getClassName();
	void setFirst(const AAAServerType &first);
	AAAServerType getFirst()  const;
	void setSecond(const AAAServerType &second);
	AAAServerType getSecond()  const;

  /* Data Members */
  private:
	AAAServerType  m_first;
	AAAServerType  m_second;
};

}
#endif                                            //AUTHLOGINMODEMANAGEDOBJECT_H
