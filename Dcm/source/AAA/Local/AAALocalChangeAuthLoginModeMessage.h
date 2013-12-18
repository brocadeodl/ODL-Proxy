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

#ifndef AAALOCALCHANGEAUTHLOGINMODEMESSAGE_H
#define AAALOCALCHANGEAUTHLOGINMODEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalChangeAuthLoginModeMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAALocalChangeAuthLoginModeMessage ();
	AAALocalChangeAuthLoginModeMessage (const UI8 &authMode, const bool &radMode, const bool &tacMode, const bool &ldapMode, const bool &logout);
	virtual    ~AAALocalChangeAuthLoginModeMessage ();
	UI8 getAuthMode()  const;
	bool getRadiusMode()  const;
	bool getTacacsMode()  const;
	bool getLDAPMode()  const;
	bool getLogout()  const;

  /* Data Members */
  private:
	UI8 m_authMode;
	bool m_radiusMode;
	bool m_tacacsMode;
	bool m_ldapMode;
	bool m_logout;
};

}
#endif // AAALOCALCHANGEAUTHLOGINMODEMESSAGE_H
