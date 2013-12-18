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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kghanta                                                      *
 **************************************************************************/

#ifndef LOCALSERVERMGMTWORKER_H
#define LOCALSERVERMGMTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalLocalObjectManager;
class AAALocalAddRadiusHostMessage;
class AAALocalChangeRadiusHostMessage;
class AAALocalDeleteRadiusHostMessage;
class AAALocalAddTacacsHostMessage;
class AAALocalChangeTacacsHostMessage;
class AAALocalDeleteTacacsHostMessage;
class AAALocalAddLDAPHostMessage;
class AAALocalChangeLDAPHostMessage;
class AAALocalDeleteLDAPHostMessage;
class AAALocalChangeAuthLoginModeMessage;
class AAALocalChangeAccountingMessage;

class LocalServerMgmtWorker: public WaveWorker
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	void AAALocalAddRadiusHostMessageHandler(AAALocalAddRadiusHostMessage *pAAALocalAddRadiusHostMessage);
	void AAALocalChangeRadiusHostMessageHandler(AAALocalChangeRadiusHostMessage *pAAALocalChangeRadiusHostMessage);
	void AAALocalDeleteRadiusHostMessageHandler(AAALocalDeleteRadiusHostMessage *pAAALocalDeleteRadiusHostMessage);
	void AAALocalAddTacacsHostMessageHandler(AAALocalAddTacacsHostMessage *pAAALocalAddTacacsHostMessage);
	void AAALocalChangeTacacsHostMessageHandler(AAALocalChangeTacacsHostMessage *pAAALocalChangeTacacsHostMessage);
	void AAALocalDeleteTacacsHostMessageHandler(AAALocalDeleteTacacsHostMessage *pAAALocalDeleteTacacsHostMessage);
	void AAALocalAddLDAPHostMessageHandler(AAALocalAddLDAPHostMessage *pAAALocalAddLDAPHostMessage);
	void AAALocalChangeLDAPHostMessageHandler(AAALocalChangeLDAPHostMessage *pAAALocalChangeLDAPHostMessage);
	void AAALocalDeleteLDAPHostMessageHandler(AAALocalDeleteLDAPHostMessage *pAAALocalDeleteLDAPHostMessage);
	void AAALocalChangeAuthLoginModeMessageHandler(AAALocalChangeAuthLoginModeMessage *pAAALocalChangeAuthLoginModeMessage);
	void AAALocalChangeAccountingMessageHandler(AAALocalChangeAccountingMessage *pAAALocalChangeAccountingMessage);

  public:
	LocalServerMgmtWorker(AAALocalLocalObjectManager *pAAALocalLocalObjectManager);
	virtual ~LocalServerMgmtWorker ();

};

}
#endif // LOCALSERVERMGMTWORKER_H
