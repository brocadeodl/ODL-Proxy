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

#ifndef LOCALAUTHENTICATIONWORKER_H
#define LOCALAUTHENTICATIONWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "AAA/Local/AAALocalCleanCacheTimer.h"
#include "AAA/Utils/AAAErrorUtils.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalLocalObjectManager;
class AAALocalAddUserMessage;
class AAALocalChangeUserMessage;
class AAALocalDeleteUserMessage;
class AAALocalAddRoleMessage;
class AAALocalChangeRoleMessage;
class AAALocalDeleteRoleMessage;
class AAALocalCleanCacheMessage;
class AAALocalUserActionMessage;
class AAALocalCleanCacheTimer;

class LocalAuthenticationWorker: public WaveWorker
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	void AAALocalAddUserMessageHandler (AAALocalAddUserMessage *pAAALocalAddUserMessage);
	void AAALocalChangeUserMessageHandler (AAALocalChangeUserMessage *pAAALocalChangeUserMessage);
	void AAALocalDeleteUserMessageHandler (AAALocalDeleteUserMessage *pAAALocalDeleteUserMessage);
	void AAALocalAddRoleMessageHandler (AAALocalAddRoleMessage *pAAALocalAddRoleMessage);
	void AAALocalChangeRoleMessageHandler (AAALocalChangeRoleMessage *pAAALocalChangeRoleMessage);
	void AAALocalDeleteRoleMessageHandler (AAALocalDeleteRoleMessage *pAAALocalDeleteRoleMessage);
	void AAALocalUserActionMessageHandler (AAALocalUserActionMessage *pAAALocalUserActionMessage);
	void  AAALocalCleanCacheMessageHandler( AAALocalCleanCacheMessage *pAAALocalCleanCacheMessage);
	void cleanCacheTimerHandler(TimerHandle &timerHandle);
	int changeRoleInFileSystem(const string &name, const string &description, const bool &descFlag, const string &users);
	int syncDefaultAccts(int passwdDefaultSync);
	AAALocalCleanCacheTimer	*pAAALocalCleanCacheTimer;
	void setGEPS(const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName);
  public:
	LocalAuthenticationWorker (AAALocalLocalObjectManager *pAAALocalObjectManager);
	virtual    ~LocalAuthenticationWorker ();

  /* Data Members */
  private:
	AAAErrorUtils m_utils;
};

}
#endif //LOCALAUTHENTICATIONWORKER_H
