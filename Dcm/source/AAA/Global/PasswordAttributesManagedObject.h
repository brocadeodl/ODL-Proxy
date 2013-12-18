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

#ifndef PASSWORDATTRIBUTESMANAGEDOBJECT_H
#define PASSWORDATTRIBUTESMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

#define PASSWD_MIN_LEN_DEFAULT 8
#define PASSWD_MAX_RETRY_DEFAULT 0
#define PASSWD_UPPER_DEFAULT 0
#define PASSWD_LOWER_DEFAULT 0
#define PASSWD_NUMERICS_DEFAULT 0
#define PASSWD_SPL_CHAR_DEFAULT 0
#define PASSWD_ADMIN_LOCKOUT_DEFAULT 0

class AAAGlobalObjectManager;

class PasswordAttributesManagedObject : virtual public DcmManagedObject
{
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
  public:
	PasswordAttributesManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	PasswordAttributesManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const UI32 &minlength,const UI32 &maxretry,const UI32 &uppercase,const UI32 &lowercase,const UI32 &numerics,const UI32 &splchars, const bool &adminlockout);
	virtual    ~PasswordAttributesManagedObject ();
	static string getClassName();
	void setMinlength(const UI32 &minlength);
	UI32 getMinlength()  const;
	void setMaxretry(const UI32 &maxretry);
	UI32 getMaxretry()  const;
	void setUppercase(const UI32 &uppercase);
	UI32 getUppercase()  const;
	void setLowercase(const UI32 &lowercase);
	UI32 getLowercase()  const;
	void setNumerics(const UI32 &numerics);
	UI32 getNumerics()  const;
	void setSplchars(const UI32 &splchars);
	UI32 getSplchars()  const;
	void setAdminlockout(const bool &adminlockout);
	bool getAdminlockout() const;

  /* Data members */
  private:
	UI32  m_minlength;
	UI32  m_maxretry;
	UI32  m_uppercase;
	UI32  m_lowercase;
	UI32  m_numerics;
	UI32  m_splchars;
	bool  m_adminlockout;	
};

}
#endif	//PASSWORDATTRIBUTESMANAGEDOBJECT_H
