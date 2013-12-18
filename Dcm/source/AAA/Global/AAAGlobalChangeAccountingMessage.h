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
 *   Author : singhb                                                       *
 **************************************************************************/

#ifndef AAAGLOBALCHANGEACCOUNTINGMESSAGE_H
#define AAAGLOBALCHANGEACCOUNTINGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalChangeAccountingMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalChangeAccountingMessage ();
	AAAGlobalChangeAccountingMessage (const SI8 &accType, const SI8 &serverType);
	virtual    ~AAAGlobalChangeAccountingMessage ();
	void setAccType(const SI8 &accType);
	SI8 getAccType()  const;
	void setServerType(const SI8 &serverType);
	SI8 getServerType()  const;

  /* Data Members */
  private:
	SI8 m_accType;
	SI8 m_serverType;
};

}
#endif // AAAGLOBALCHANGEACCOUNTINGMESSAGE_H
