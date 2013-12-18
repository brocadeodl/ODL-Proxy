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

#if 0
/***************************************************************************
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Charanjith Reddy Kunduru                                     *
 ***************************************************************************/

#ifndef IP6AAASEQUENCERCONTEXTS_H
#define IP6AAASEQUENCERCONTEXTS_H

#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"

#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/RoleManagedObject.h"
#include "AAA/Global/PasswordAttributesManagedObject.h"
#include "AAA/Global/BannerManagedObject.h"
#include "AAA/Global/RadiusHostManagedObject.h"
#include "AAA/Global/TacacsHostManagedObject.h"
#include "AAA/Global/LDAPHostManagedObject.h"
#include "AAA/Global/MapRoleManagedObject.h"
#include "AAA/Global/AuthLoginModeManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class IP6AAASequencerContext //: public PrismLinearSequencerContext
{
  public:
	IP6AAASequencerContext (PrismMessage *pPrismMessage);
	virtual ~AAASequencerContext ();
	PrismMessage* getPPrismMessage() const;
	void traceSendToClusterErrors(WaveSendToClusterContext *pWaveSendToClusterContext, const string &callerMethodName, string &errorStr, const bool &isPartialSuccessEnabled = true);

  private:
	PrismMessage *m_pPrismMsg;
};

	virtual ~AAAChangeMapRoleContext ();
	MapRoleManagedObject* getMapRoleObj() const;

  private:
	MapRoleManagedObject *m_pMapRole;
};

class IP6AAAAclRuleContext : public IP6AAASequencerContext
{
  public:
	IP6AAAAclRuleContext (PrismMessage *pMessage);
	virtual ~IP6AAAAclRuleContext ();
};

}
#endif // AAASEQUENCERCONTEXTS_H
#endif
