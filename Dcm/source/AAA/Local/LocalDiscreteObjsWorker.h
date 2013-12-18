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

#ifndef LOCALDISCRETEOBJSWORKER_H
#define LOCALDISCRETEOBJSWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalLocalObjectManager;
class AAALocalSetPasswordAttributesMessage;
class AAALocalSetBannerMessage;
class AAALocalAddMapRoleMessage;
class AAALocalChangeMapRoleMessage;
class AAALocalDeleteMapRoleMessage;
class CertutilOpActionMessage;
class CertutilOpShowMessage;
class TelnetServerLocalConfigMessage;
class TelnetServerLocalShowMessage;
class SSHServerLocalConfigMessage;
class SSHServerListLocalAddMessage;
class SSHServerListLocalDeleteMessage;
class SSHServerLocalShowMessage;
class SSHServerListLocalShowMessage;
class ClearSessionsLocalMessage;

class LocalDiscreteObjsWorker: public WaveWorker
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	void AAALocalSetBannerMessageHandler( AAALocalSetBannerMessage *pAAALocalSetBannerMessage);
	void AAALocalSetPasswordAttributesMessageHandler( AAALocalSetPasswordAttributesMessage *pAAALocalSetPasswordAttributesMessage);
	void AAALocalAddMapRoleMessageHandler (AAALocalAddMapRoleMessage *pAAALocalAddMapRoleMessage);
	void AAALocalChangeMapRoleMessageHandler (AAALocalChangeMapRoleMessage *pAAALocalChangeMapRoleMessage);
	void AAALocalDeleteMapRoleMessageHandler (AAALocalDeleteMapRoleMessage *pAAALocalDeleteMapRoleMessage);
	void AAALocalCertutilOpActionMessageHandler(CertutilOpActionMessage *pCertutilOpActionMessage);
	void AAALocalCertutilOpShowMessageHandler(CertutilOpShowMessage *pCertutilOpShowMessage);
	void TelnetServerLocalConfigMessageHandler(TelnetServerLocalConfigMessage *pTelnetServerLocalConfigMessage);
	void TelnetServerLocalShowMessageHandler(TelnetServerLocalShowMessage *pTelnetServerLocalShowMessage);
	void SSHServerLocalConfigMessageHandler(SSHServerLocalConfigMessage *pSSHServerLocalConfigMessage);
	void SSHServerListLocalAddMessageHandler(SSHServerListLocalAddMessage *pSSHServerListLocalAddMessage);
	void SSHServerListLocalDeleteMessageHandler(SSHServerListLocalDeleteMessage *pSSHServerListLocalDeleteMessage);
	void SSHServerLocalShowMessageHandler(SSHServerLocalShowMessage *pSSHServerLocalShowMessage);
	void SSHServerListLocalShowMessageHandler(SSHServerListLocalShowMessage *pSSHServerListLocalShowMessage);
	void ClearSessionsLocalMessageHandler(ClearSessionsLocalMessage *pClearSessionsLocalMessage);
  public:
	LocalDiscreteObjsWorker ( AAALocalLocalObjectManager *pAAALocalLocalObjectManager);
	virtual    ~LocalDiscreteObjsWorker ();
};

}
#endif	//LOCALDISCRETEOBJSWORKER_H
