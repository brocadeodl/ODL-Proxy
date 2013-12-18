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

#ifndef AAAGLOBALUSERACTIONMESSAGE_H
#define AAAGLOBALUSERACTIONMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalUserActionMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalUserActionMessage ();
	AAAGlobalUserActionMessage (const string &name, const AAAActionType &actionType);
	virtual ~AAAGlobalUserActionMessage ();
	void setName(const string &name);
	string getName()  const;
	void setActionType(const AAAActionType &actionType);
	AAAActionType getActionType()  const;

  /* Data members */
  private:
	string  m_name;
	AAAActionType  m_actionType;
};

}
#endif // AAAGLOBALUSERACTIONMESSAGE_H
