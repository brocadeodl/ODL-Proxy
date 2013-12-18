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

#ifndef AAAGLOBALCHANGEAUTHLOGINMODEMESSAGE_H
#define AAAGLOBALCHANGEAUTHLOGINMODEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalChangeAuthLoginModeMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalChangeAuthLoginModeMessage ();
	AAAGlobalChangeAuthLoginModeMessage (const SI8 &first, const SI8 &second, const bool &isLogout);
	virtual    ~AAAGlobalChangeAuthLoginModeMessage ();
	void setFirst(const SI8 &first);
	SI8 getFirst()  const;
	void setSecond(const SI8 &second);
	SI8 getSecond()  const;
	void setIsLogout(const bool &isLogout);
    bool getIsLogout()  const;

  /* Data Members */
  private:
	SI8 m_first;
	SI8 m_second;
	bool m_isLogout;
};

}
#endif // AAAGLOBALCHANGEAUTHLOGINMODEMESSAGE_H
