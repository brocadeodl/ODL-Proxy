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

#ifndef GLOBALAUTHENTICATIONWORKER_H
#define GLOBALAUTHENTICATIONWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

typedef enum {
	USER_NAME,
	USER_PASSWORD,
	USER_DESCRIPTION,
	ROLE_NAME
} AuthParamType;

class AAAGlobalObjectManager;
class AAAGlobalAddUserMessage;
class AAAGlobalChangeUserMessage;
class AAAGlobalDeleteUserMessage;
class AAAGlobalUserActionMessage;
class AAAGlobalAddRoleMessage;
class AAAGlobalChangeRoleMessage;
class AAAGlobalDeleteRoleMessage;

class GlobalAuthenticationWorker: public WaveWorker
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	void AAAGlobalAddUserMessageHandler( AAAGlobalAddUserMessage *pAAAGlobalAddUserMessage);
	void AAAGlobalChangeUserMessageHandler( AAAGlobalChangeUserMessage *pAAAGlobalChangeUserMessage);
	void AAAGlobalDeleteUserMessageHandler( AAAGlobalDeleteUserMessage *pAAAGlobalDeleteUserMessage);
	void AAAGlobalUserActionMessageHandler( AAAGlobalUserActionMessage *pAAAGlobalUserActionMessage);
	void AAAGlobalAddRoleMessageHandler( AAAGlobalAddRoleMessage *pAAAGlobalAddRoleMessage);
	void AAAGlobalChangeRoleMessageHandler( AAAGlobalChangeRoleMessage *pAAAGlobalChangeRoleMessage);
	void AAAGlobalDeleteRoleMessageHandler( AAAGlobalDeleteRoleMessage *pAAAGlobalDeleteRoleMessage);
	void addUserSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void changeUserSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void deleteUserSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void userActionSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void addRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void changeRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void deleteRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void cleanAAACacheSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	int paramValidate(string str, AuthParamType type);
	int setGEPS(const string &loginUser);
	string formatString(vector<int> value, vector<string> strValue);
	string setErrorMessage(ResourceId &status, string operation);
  public:
	GlobalAuthenticationWorker (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	virtual    ~GlobalAuthenticationWorker ();
};

}
#endif //GLOBALAUTHENTICATIONWORKER_H
