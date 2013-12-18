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
 *   Author : singhb	                                                   *
 **************************************************************************/

#ifndef CMDACCOUNTINGMANAGEDOBJECT_H
#define CMDACCOUNTINGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;

class CmdAccountingManagedObject : virtual public DcmManagedObject
{
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
  public:
	CmdAccountingManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	CmdAccountingManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager/*,const AccType &accType*/,const AccServerType &accSrvType);
	virtual    ~CmdAccountingManagedObject ();
	static string getClassName();
	/*
	void setAccType(const AccType &accType);
	AccType getAccType()  const;
	*/
	void setAccSrvType(const AccServerType &accSrvType);
	AccServerType getAccSrvType()  const;

  /* Data Members */
  private:
	// AccType  m_accType;
	AccServerType  m_accSrvType;
};

}
#endif                                            //CMDACCOUNTINGMANAGEDOBJECT_H
