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
*   Author : bkesanam                                                     *
**************************************************************************/

#ifndef RASNODESPECIFICLOCALOBJECTMANAGER_H
#define RASNODESPECIFICLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

#include "ClientInterface/RAS/RASClientMessage.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "RAS/Local/RASNodeSpecificConfigureMessage.h"

using namespace WaveNs;

namespace DcmNs
{
#if 0	
// use these class 
class BootCompleteForThisLocationEvent;
class RASNodeSpecificConfigureMessage;

class RASNodeSpecificLocalObjectManager: public WaveLocalObjectManager
{
	private:
		PrismMessage *createMessageInstance(const UI32 &operationCode);
		WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
	    void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
	    void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
	    void bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);
	    void postboot (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhases);
	    
		void loggingBootReplay();

		void RASNodeSpecificConfigureMessageHandler(RASNodeSpecificConfigureMessage *pRASNodeSpecificConfigureMessage);

	protected:
	public:
		RASNodeSpecificLocalObjectManager ();
		virtual    ~RASNodeSpecificLocalObjectManager ();
		static RASNodeSpecificLocalObjectManager *getInstance();
		static string getClassName();
		static PrismServiceId getPrismServiceId();
	// Now the data members

	private:

	protected:
	public:
};
#endif
}
#endif                                            //RASNODESPECIFICLOCALOBJECTMANAGER_H
