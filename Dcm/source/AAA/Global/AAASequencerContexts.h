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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Charanjith Reddy Kunduru                                     *
 ***************************************************************************/

#ifndef AAASEQUENCERCONTEXTS_H
#define AAASEQUENCERCONTEXTS_H

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
#include "AAA/Global/ExecAccountingManagedObject.h"
#include "AAA/Global/CmdAccountingManagedObject.h"
#include "AAA/Global/SecServicesGlobalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class AAASequencerContext //: public PrismLinearSequencerContext
{
  public:
	AAASequencerContext (PrismMessage *pPrismMessage);
	virtual ~AAASequencerContext ();
	PrismMessage* getPPrismMessage() const;
	void traceSendToClusterErrors(WaveSendToClusterContext *pWaveSendToClusterContext, const string &callerMethodName, string &errorStr, const bool &isPartialSuccessEnabled = true);

  private:
	PrismMessage *m_pPrismMsg;
};

class AAAAddUserContext : public AAASequencerContext
{
  public:
	AAAAddUserContext (PrismMessage *pMessage, RoleManagedObject *pRole, const string &usrsOfRole, const string &password, const UI8 &encryptionLevel);
	virtual ~AAAAddUserContext ();
	void setRoleObj(const RoleManagedObject *pRole);
	void setUsrsOfRole(const string &usrsOfRole);
	RoleManagedObject* getRoleObj() const;
	string getUsrsOfRole() const;
	string getPassword() const;
	UI8 getEncryptionLevel() const;

  private:
	RoleManagedObject *m_pRole;
	string m_usersOfRole;
	string m_password;
	UI8 m_encryptionLevel;
};

class AAAChangeUserContext : public AAASequencerContext
{
  public:
	AAAChangeUserContext (PrismMessage *pMessage, UserManagedObject *pUser, RoleManagedObject *pRole, RoleManagedObject *pOldRole, const string &password, const UI8 &encryptionLevel);
	virtual ~AAAChangeUserContext ();
	UserManagedObject* getUserObj() const;
	RoleManagedObject* getRoleObj() const;
	RoleManagedObject* getOldRoleObj() const;
	string getPassword() const;
	UI8 getEncryptionLevel() const;

  private:
	UserManagedObject *m_pUser;
	RoleManagedObject *m_pRole;
	RoleManagedObject *m_pOldRole;
	string m_password;
	UI8 m_encryptionLevel;
};

class AAADeleteUserContext : public AAASequencerContext
{
  public:
	AAADeleteUserContext (PrismMessage *pMessage, UserManagedObject *pUser, RoleManagedObject *pRole);
	virtual ~AAADeleteUserContext ();
	UserManagedObject* getUserObj() const;
	RoleManagedObject* getRoleObj() const;

  private:
	UserManagedObject *m_pUser;
	RoleManagedObject *m_pRole;
};

class AAAAddRoleContext : public AAASequencerContext
{
  public:
	AAAAddRoleContext (PrismMessage *pMessage);
	virtual ~AAAAddRoleContext ();
};

class AAAChangeRoleContext : public AAASequencerContext
{
  public:
	AAAChangeRoleContext (PrismMessage *pMessage, RoleManagedObject *pRole);
	virtual ~AAAChangeRoleContext ();
	RoleManagedObject* getRoleObj() const;

  private:
	RoleManagedObject *m_pRole;
};

class AAADeleteRoleContext : public AAASequencerContext
{
  public:
	AAADeleteRoleContext (PrismMessage *pMessage, RoleManagedObject *pRole);
	virtual ~AAADeleteRoleContext ();
	RoleManagedObject* getRoleObj() const;

  private:
	RoleManagedObject *m_pRole;
};

class AAASetBannerContext : public AAASequencerContext
{
  public:
	AAASetBannerContext (PrismMessage *pMessage, BannerManagedObject *pBanner);
	virtual ~AAASetBannerContext ();
	BannerManagedObject* getBannerObj() const;

  private:
	BannerManagedObject *m_pBanner;
};

#if 0
class TelnetServerConfigContext : public AAASequencerContext
{
  public:
	TelnetServerConfigContext (PrismMessage *pMessage, TelnetServerGlobalManagedObject *pTelnetServerGlobalMO);
	virtual ~TelnetServerConfigContext ();
	TelnetServerGlobalManagedObject* getTelnetServerGlobalMO() const;

  private:
	TelnetServerGlobalManagedObject *m_pTelnetServerGlobalMO;
};

class SSHServerConfigContext : public AAASequencerContext
{
  public:
	SSHServerConfigContext (PrismMessage *pMessage, SSHServerGlobalManagedObject *pSSHServerGlobalMO);
	virtual ~SSHServerConfigContext ();
	SSHServerGlobalManagedObject* getSSHServerGlobalMO() const;

  private:
	SSHServerGlobalManagedObject *m_pSSHServerGlobalMO;
};

class SSHServerListAddContext : public AAASequencerContext
{
  public:
	SSHServerListAddContext (PrismMessage *pMessage);
	virtual ~SSHServerListAddContext ();

  private:
};

class SSHServerListDeleteContext : public AAASequencerContext
{
  public:
	SSHServerListDeleteContext (PrismMessage *pMessage, SSHServerListManagedObject *pSSHServerListMO);
	virtual ~SSHServerListDeleteContext ();
	SSHServerListManagedObject* getSSHServerListMO() const;

  private:
	SSHServerListManagedObject *m_pSSHServerListMO;
};
#endif

class AAASetPasswordAttributesContext : public AAASequencerContext
{
  public:
	AAASetPasswordAttributesContext (PrismMessage *pMessage, PasswordAttributesManagedObject *pPassword);
	virtual ~AAASetPasswordAttributesContext ();
	PasswordAttributesManagedObject* getPasswordAttributesObj() const;

  private:
	PasswordAttributesManagedObject *m_pPassword;
};
class AAAAddRadiusHostContext : public AAASequencerContext
{
  public:
	AAAAddRadiusHostContext (PrismMessage *pMessage, UI16 index);
	virtual ~AAAAddRadiusHostContext ();
	UI16 getIndex() const;

  private:
	UI16 m_index;
};

class AAAChangeRadiusHostContext : public AAASequencerContext
{
  public:
	AAAChangeRadiusHostContext (PrismMessage *pMessage, RadiusHostManagedObject *pRadiusHost);
	virtual ~AAAChangeRadiusHostContext ();
	RadiusHostManagedObject* getRadiusHostObj() const;

  private:
	RadiusHostManagedObject *m_pRadiusHost;
};

class AAADeleteRadiusHostContext : public AAASequencerContext
{
  public:
	AAADeleteRadiusHostContext (PrismMessage *pMessage, RadiusHostManagedObject *pRadiusHost, vector<RadiusHostManagedObject *> *radiusHostsForUpdatingIndex);
	virtual ~AAADeleteRadiusHostContext ();
	RadiusHostManagedObject* getRadiusHostObj() const;
	vector<RadiusHostManagedObject *>* getRadiusHostsForUpdatingIndex() const;

  private:
	RadiusHostManagedObject *m_pRadiusHost;
	vector<RadiusHostManagedObject *> *m_pRadiusHostsForUpdatingIndex;
};

class AAAAddTacacsHostContext : public AAASequencerContext
{
  public:
	AAAAddTacacsHostContext (PrismMessage *pMessage, UI16 index);
	virtual ~AAAAddTacacsHostContext ();
	UI16 getIndex() const;

  private:
	UI16 m_index;
};

class AAAChangeTacacsHostContext : public AAASequencerContext
{
  public:
	AAAChangeTacacsHostContext (PrismMessage *pMessage, TacacsHostManagedObject *pTacacsHost);
	virtual ~AAAChangeTacacsHostContext ();
	TacacsHostManagedObject* getTacacsHostObj() const;

  private:
	TacacsHostManagedObject *m_pTacacsHost;
};

class AAADeleteTacacsHostContext : public AAASequencerContext
{
  public:
	AAADeleteTacacsHostContext (PrismMessage *pMessage, TacacsHostManagedObject *pTacacsHost, vector<TacacsHostManagedObject *> *tacacsHostsForUpdatingIndex);
	virtual ~AAADeleteTacacsHostContext ();
	TacacsHostManagedObject* getTacacsHostObj() const;
	vector<TacacsHostManagedObject *>* getTacacsHostsForUpdatingIndex() const;

  private:
	TacacsHostManagedObject *m_pTacacsHost;
	vector<TacacsHostManagedObject *> *m_pTacacsHostsForUpdatingIndex;
};

class AAAAddLDAPHostContext : public AAASequencerContext
{
  public:
	AAAAddLDAPHostContext (PrismMessage *pMessage, UI16 index);
	virtual ~AAAAddLDAPHostContext ();
	UI16 getIndex() const;

  private:
	UI16 m_index;
};

class AAAChangeLDAPHostContext : public AAASequencerContext
{
  public:
	AAAChangeLDAPHostContext (PrismMessage *pMessage, LDAPHostManagedObject *pLDAPHost);
	virtual ~AAAChangeLDAPHostContext ();
	LDAPHostManagedObject* getLDAPHostObj() const;

  private:
	LDAPHostManagedObject *m_pLDAPHost;
};

class AAADeleteLDAPHostContext : public AAASequencerContext
{
  public:
	AAADeleteLDAPHostContext (PrismMessage *pMessage, LDAPHostManagedObject *pLDAPHost, vector<LDAPHostManagedObject *> *tacacsHostsForUpdatingIndex);
	virtual ~AAADeleteLDAPHostContext ();
	LDAPHostManagedObject* getLDAPHostObj() const;
	vector<LDAPHostManagedObject *>* getLDAPHostsForUpdatingIndex() const;

  private:
	LDAPHostManagedObject *m_pLDAPHost;
	vector<LDAPHostManagedObject *> *m_pLDAPHostsForUpdatingIndex;
};
class AAAAuthLoginModeContext : public AAASequencerContext
{
  public:
	AAAAuthLoginModeContext (PrismMessage *pMessage, AuthLoginModeManagedObject *pAuthLoginMode, const AAAServerType &first, const AAAServerType &second);
	virtual ~AAAAuthLoginModeContext ();
	AuthLoginModeManagedObject* getAuthLoginModeObj() const;
	AAAServerType getFirst() const;
	AAAServerType getSecond() const;

  private:
	AuthLoginModeManagedObject *m_pAuthLoginMode;
	AAAServerType m_first;
	AAAServerType m_second;
};

class AAAChangeExecAccountingContext : public AAASequencerContext
{
  public:
    AAAChangeExecAccountingContext (PrismMessage *pMessage, ExecAccountingManagedObject *pExecAcc);
    virtual ~AAAChangeExecAccountingContext ();
    ExecAccountingManagedObject* getExecAccObj() const;

  private:
    ExecAccountingManagedObject *m_pExecAcc;
};

class AAAChangeCmdAccountingContext : public AAASequencerContext
{
  public:
    AAAChangeCmdAccountingContext (PrismMessage *pMessage, CmdAccountingManagedObject *pCmdAcc);
    virtual ~AAAChangeCmdAccountingContext ();
    CmdAccountingManagedObject* getCmdAccObj() const;
    
  private:
    CmdAccountingManagedObject *m_pCmdAcc;
};
class AAACleanCacheContext : public AAASequencerContext
{
  public:
	AAACleanCacheContext (PrismMessage *pMessage);
	virtual ~AAACleanCacheContext ();
};

class AAAAddMapRoleContext : public AAASequencerContext
{
  public:
	AAAAddMapRoleContext (PrismMessage *pMessage);
	virtual ~AAAAddMapRoleContext ();
};

class AAAChangeMapRoleContext : public AAASequencerContext
{
  public:
	AAAChangeMapRoleContext (PrismMessage *pMessage, MapRoleManagedObject *pRole);
	virtual ~AAAChangeMapRoleContext ();
	MapRoleManagedObject* getMapRoleObj() const;

  private:
	MapRoleManagedObject *m_pMapRole;
};

class AAADeleteMapRoleContext : public AAASequencerContext
{
  public:
	AAADeleteMapRoleContext (PrismMessage *pMessage, MapRoleManagedObject *pMapRole);
	virtual ~AAADeleteMapRoleContext ();
	MapRoleManagedObject* getMapRoleObj() const;

  private:
	MapRoleManagedObject *m_pMapRole;
};

class AAAIpAclRuleContext : public AAASequencerContext
{
  public:
	AAAIpAclRuleContext (PrismMessage *pMessage);
	virtual ~AAAIpAclRuleContext ();
};

class AAAStdAclRuleContext : public AAASequencerContext
{
  public:
	AAAStdAclRuleContext (PrismMessage *pMessage);
	virtual ~AAAStdAclRuleContext ();
};

class AAAExtAclRuleContext : public AAASequencerContext
{
  public:
	AAAExtAclRuleContext (PrismMessage *pMessage);
	virtual ~AAAExtAclRuleContext ();
};


class AAACertutilContext : public AAASequencerContext
{
  public:
	AAACertutilContext (PrismMessage *pMessage);
	virtual ~AAACertutilContext ();
};
}
#endif // AAASEQUENCERCONTEXTS_H
