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

#ifndef AAALOCALLOCALOBJECTMANAGER_H
#define AAALOCALLOCALOBJECTMANAGER_H

#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
 #include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
using namespace WaveNs;

namespace DcmNs
{

class LocalAuthenticationWorker;
class LocalDiscreteObjsWorker;
class LocalServerMgmtWorker;
class LocalSessionWorker;
class LocalIpAclWorker;

typedef enum {
	DCM_FSS_STAGE4_AAA_PLUGIN = 4, // Chassis
	AAA_CFG_STAGE = 6 // Fcsw
} AAAPluginFssStages;

class AAALocalLocalObjectManager: public WaveLocalObjectManager
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	int syncRolesAndUsers(bool isDefaultDBSync);
	int syncRadiusHosts();
	int syncTacacsHosts();
	int syncLDAPHosts();
	int syncMapRoles();
	int syncExecAccounting();
	int syncCmdAccounting();
	int syncAuthMode();
	int syncPasswdOptions();
	int syncBanner();
	int syncIpAcls();
	int syncIp6Acls();
    int syncTelnetServerConfig();
    int syncSSHServerConfig();
    int syncSSHServerListConfig();
	void syncAAAPlugins();

  public:
	AAALocalLocalObjectManager ();
	virtual ~AAALocalLocalObjectManager ();
	static AAALocalLocalObjectManager *getInstance();
	static string getClassName();
	static PrismServiceId getPrismServiceId();
    void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
    void haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
    void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
	void externalStateSynchronization(WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
	// int sendAccMessage(int srvType, int startStop, char *user, char *userPort, char *userHost, char **cmdArgVals);
	int sendAccMessage(int srvType, int startStop, char *user,
						char *userPort, char *userHost, char *cmd,
						char *time, char *status);

  /* Data Members */
  private:
	LocalAuthenticationWorker  *m_pLocalAuthenticationWorker;
	LocalDiscreteObjsWorker  *m_pLocalDiscreteObjsWorker;
	LocalServerMgmtWorker  *m_pLocalServerMgmtWorker;
	LocalSessionWorker  *m_pLocalSessionWorker;
	LocalIpAclWorker  *m_pLocalIpAclWorker;
	bool m_isPersistentRejoinBoot;
	bool m_isPersistentDefaultBoot;
};

}
#endif // AAALOCALLOCALOBJECTMANAGER_H
