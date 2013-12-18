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
 *   Author : Charanjith Reddy Kunduru                                     *
 **************************************************************************/

#ifndef PASSWORDENCRYPTIONMANAGEDOBJECT_H
#define PASSWORDENCRYPTIONMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;

class PasswordEncryptionManagedObject : virtual public DcmManagedObject
{
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();

  public:
	PasswordEncryptionManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	PasswordEncryptionManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const bool &passwdEnc);
	virtual ~PasswordEncryptionManagedObject ();
	static string getClassName();
	void setPasswordEncryption(const bool &passwdEnc);
	bool getPasswordEncryption() const;

  /* Data members */
  private:
	bool m_passwdEnc;
};

}
#endif	//PASSWORDENCRYPTIONMANAGEDOBJECT_H
