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
 *   Author : Charanjith Reddy Kunduru                                     *
 **************************************************************************/

#ifndef GLOBALDISCRETEOBJSWORKER_H
#define GLOBALDISCRETEOBJSWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;
class AAAGlobalSetPasswordAttributesMessage;
class AAAGlobalSetBannerMessage;
class AAAGlobalSetPasswordEncryptionMessage;
class AAAGlobalAddMapRoleMessage;
class AAAGlobalChangeMapRoleMessage;
class AAAGlobalDeleteMapRoleMessage;
class AAAGlobalCertutilOpActionMessage;
class TelnetServerGlobalConfigMessage;
class SSHServerGlobalConfigMessage;
class SSHServerListGlobalAddMessage;
class SSHServerListGlobalDeleteMessage;

class GlobalDiscreteObjsWorker: public WaveWorker
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	void AAAGlobalSetBannerMessageHandler( AAAGlobalSetBannerMessage *pAAAGlobalSetBannerMessage);
	void AAAGlobalSetPasswordAttributesMessageHandler( AAAGlobalSetPasswordAttributesMessage *pAAAGlobalSetPasswordAttributesMessage);
	void AAAGlobalSetPasswordEncryptionMessageHandler( AAAGlobalSetPasswordEncryptionMessage *pAAAGlobalSetPasswordEncryptionMessage);
	void setBannerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void setPasswordAttributesSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void AAAGlobalAddMapRoleMessageHandler( AAAGlobalAddMapRoleMessage *pAAAGlobalAddMapRoleMessage);
	void AAAGlobalChangeMapRoleMessageHandler( AAAGlobalChangeMapRoleMessage *pAAAGlobalChangeMapRoleMessage);
	void AAAGlobalDeleteMapRoleMessageHandler( AAAGlobalDeleteMapRoleMessage *pAAAGlobalDeleteMapRoleMessage);
	void AAAGlobalCertutilOpActionMessageHandler(AAAGlobalCertutilOpActionMessage *pAAAGlobalCertutilOpActionMessage);
	void TelnetServerGlobalConfigMessageHandler( TelnetServerGlobalConfigMessage *pTelnetServerGlobalConfigMessage);
	void SSHServerGlobalConfigMessageHandler( SSHServerGlobalConfigMessage *pSSHServerGlobalConfigMessage);
	void SSHServerListGlobalAddMessageHandler (SSHServerListGlobalAddMessage *pSSHServerListGlobalAddMessage);
	void SSHServerListGlobalDeleteMessageHandler (SSHServerListGlobalDeleteMessage *pSSHServerListGlobalDeleteMessage);
	void addMapRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void changeMapRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void deleteMapRoleSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void doCertutilSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void TelnetServerConfigSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void SSHServerConfigSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
	void SSHServerListAddSTCCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
	void SSHServerListDeleteSTCCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
	bool isValidRole(string role);
 public:
	GlobalDiscreteObjsWorker ( AAAGlobalObjectManager *pAAAGlobalObjectManager);
	virtual    ~GlobalDiscreteObjsWorker ();

};

}
#endif //GLOBALDISCRETEOBJSWORKER_H
