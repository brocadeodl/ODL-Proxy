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

#ifndef GLOBALSERVERMGMTWORKER_H
#define GLOBALSERVERMGMTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

enum radiusParamType {
	RADIUS_HOST = 0,
	RADIUS_PORT,
	RADIUS_PROTOCOL,
	RADIUS_KEY,
	RADIUS_RETRANSMIT,
	RADIUS_TIMEOUT,
	RADIUS_DEADTIME,
	TACACS_KEY
};

class AAAGlobalObjectManager;
class AAAGlobalAddRadiusHostMessage;
class AAAGlobalChangeRadiusHostMessage;
class AAAGlobalDeleteRadiusHostMessage;
class AAAGlobalAddTacacsHostMessage;
class AAAGlobalChangeTacacsHostMessage;
class AAAGlobalDeleteTacacsHostMessage;
class AAAGlobalAddLDAPHostMessage;
class AAAGlobalChangeLDAPHostMessage;
class AAAGlobalDeleteLDAPHostMessage;
class AAAGlobalChangeAuthLoginModeMessage;
class AAAGlobalChangeAccountingMessage;

class GlobalServerMgmtWorker: public WaveWorker
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	void AAAGlobalAddRadiusHostMessageHandler(AAAGlobalAddRadiusHostMessage *pAAAGlobalAddRadiusHostMessage);
	void AAAGlobalChangeRadiusHostMessageHandler(AAAGlobalChangeRadiusHostMessage *pAAAGlobalChangeRadiusHostMessage);
	void AAAGlobalDeleteRadiusHostMessageHandler(AAAGlobalDeleteRadiusHostMessage *pAAAGlobalDeleteRadiusHostMessage);
	void AAAGlobalAddTacacsHostMessageHandler(AAAGlobalAddTacacsHostMessage *pAAAGlobalAddTacacsHostMessage);
   	void AAAGlobalChangeTacacsHostMessageHandler(AAAGlobalChangeTacacsHostMessage *pAAAGlobalChangeTacacsHostMessage);
	void AAAGlobalDeleteTacacsHostMessageHandler(AAAGlobalDeleteTacacsHostMessage *pAAAGlobalDeleteTacacsHostMessage);
	void AAAGlobalAddLDAPHostMessageHandler(AAAGlobalAddLDAPHostMessage *pAAAGlobalAddLDAPHostMessage);
   	void AAAGlobalChangeLDAPHostMessageHandler(AAAGlobalChangeLDAPHostMessage *pAAAGlobalChangeLDAPHostMessage);
	void AAAGlobalDeleteLDAPHostMessageHandler(AAAGlobalDeleteLDAPHostMessage *pAAAGlobalDeleteLDAPHostMessage);
	void AAAGlobalChangeAuthLoginModeMessageHandler(AAAGlobalChangeAuthLoginModeMessage *pAAAGlobalChangeAuthLoginModeMessage);
	void AAAGlobalChangeAccountingMessageHandler( AAAGlobalChangeAccountingMessage *pAAAGlobalChangeAccountingMessage);
	void addRadiusHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void changeRadiusHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void deleteRadiusHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void addTacacsHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void changeTacacsHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void deleteTacacsHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void addLDAPHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void changeLDAPHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void deleteLDAPHostSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void changeAuthLoginModeSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void changeExecAccountingSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void changeCmdAccountingSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	int radiusParamValidate(const string &strParam, const UI32 &intParam, const radiusParamType &type);
	int getAuthModes(const AAAServerType &first, const AAAServerType &second, void *authMode, void *radMode, void *tacMode, void *ldapMode);
	void traceSendToClusterErrors(WaveSendToClusterContext *pWaveSendToClusterContext, const string &callerMethodName);

  public:
	GlobalServerMgmtWorker ( AAAGlobalObjectManager *pAAAGlobalObjectManager);
	virtual ~GlobalServerMgmtWorker ();
};

}

#endif // GLOBALSERVERMGMTWORKER_H
