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

#ifndef BANNERMANAGEDOBJECT_H
#define BANNERMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;

class BannerManagedObject : virtual public DcmManagedObject
{
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();

  public:
	BannerManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	BannerManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const string &login,const string &motd, const string &incoming);
	virtual    ~BannerManagedObject ();
	static string getClassName();
	void setLogin(const string &banner);
	string getLogin()  const;
        void setMotd(const string &banner);
        string getMotd()  const;
        void setIncoming(const string &banner);
        string getIncoming()  const;


  /* Data members */
  private:
	string  m_login;
        string  m_motd;
        string  m_incoming;
};

}
#endif	//BANNERMANAGEDOBJECT_H
