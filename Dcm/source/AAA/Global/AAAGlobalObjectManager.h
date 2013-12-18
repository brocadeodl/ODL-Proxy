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

#ifndef AAAGLOBALOBJECTMANAGER_H
#define AAAGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Core/ConfigReplayCompletedEvent.h"

using namespace WaveNs;

namespace DcmNs
{

class GlobalAuthenticationWorker;
class GlobalAuthorizationWorker;
class GlobalServerMgmtWorker;
class GlobalDiscreteObjsWorker;
class GlobalSessionWorker;
//class GlobalIpAclWorker;

class AAAGlobalObjectManager: public WaveObjectManager
{
  private:
	PrismMessage *createMessageInstance(const UI32 &operationCode);
	WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	void configReplayCompletedEventHandler (const ConfigReplayCompletedEvent* &pEvent);
	void configReplayCompletedCallback(WaveSendToClusterContext *pWaveSendToClusterContextCache);
	void setCmdAccFlag();

  public:
	AAAGlobalObjectManager ();
	virtual    ~AAAGlobalObjectManager ();
	static AAAGlobalObjectManager *getInstance();
	static string getClassName();
	static PrismServiceId getPrismServiceId();
	void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
	void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

  /* Data Members */
  private:
	GlobalAuthenticationWorker  *m_pGlobalAuthenticationWorker;
	GlobalAuthorizationWorker *m_pGlobalAuthorizationWorker;
	GlobalServerMgmtWorker  *m_pGlobalServerMgmtWorker;
	GlobalDiscreteObjsWorker  *m_pGlobalDiscreteObjsWorker;
	GlobalSessionWorker  *m_pGlobalSessionWorker;
	// GlobalIpAclWorker  *m_pGlobalIpAclWorker;

};

}
#endif                                            //AAAGLOBALOBJECTMANAGER_H
