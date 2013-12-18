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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kghanta                                                      *
 **************************************************************************/

#ifndef AAAGLOBALCHANGEMAPROLEMESSAGE_H
#define AAAGLOBALCHANGEMAPROLEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalChangeMapRoleMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalChangeMapRoleMessage ();
	AAAGlobalChangeMapRoleMessage (const string &group, const string &role);
	virtual    ~AAAGlobalChangeMapRoleMessage ();
	void setGroup(const string &group);
	string getGroup()  const;
	void setRole(const string &role);
	string getRole()  const;

  /* Data members */
  private:
	string  m_group;
	string  m_role;
};

}
#endif	//AAAGLOBALCHANGEMAPROLEMESSAGE_H
