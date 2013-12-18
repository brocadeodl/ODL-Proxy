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
 *   Author : Subhani Shaik                                                *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Diag/Local/DiagTestsLocalObjectManager.h"
#include "Diag/Local/DiagTestsActionMessage.h"
#include "Diag/Local/DiagLocalTypes.h"
#include "DcmShell/DcmShell.h"
#include <iostream>
#include <string>
#include <fstream>
#include "errno.h"


namespace DcmNs
{

    DiagTestsLocalObjectManager::DiagTestsLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (DIAGTESTSACTION, reinterpret_cast<PrismMessageHandler> (&DiagTestsLocalObjectManager::DiagTestsActionMessageHandler));
    }

    DiagTestsLocalObjectManager::~DiagTestsLocalObjectManager ()
    {
    }

    DiagTestsLocalObjectManager  *DiagTestsLocalObjectManager::getInstance()
    {
        static DiagTestsLocalObjectManager *pDiagTestsLocalObjectManager = new DiagTestsLocalObjectManager ();

        WaveNs::prismAssert (NULL != pDiagTestsLocalObjectManager, __FILE__, __LINE__);

        return (pDiagTestsLocalObjectManager);
    }

    string  DiagTestsLocalObjectManager::getClassName()
    {
        return ("Diag Tests Local Manager");
    }

    PrismServiceId  DiagTestsLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *DiagTestsLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case DIAGTESTSACTION :
                pPrismMessage = new DiagTestsActionMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DiagTestsLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

#define DIAGCLI_FILE "/var/log/diagcli.data"
    void  DiagTestsLocalObjectManager::DiagTestsActionMessageHandler( DiagTestsActionMessage *pDiagTestsActionMessage)
    {
   		FILE	*cmd_file = 0;
		int		rc;
		char	cmd_output[513];
        string 	result = "";
        ifstream in;

		WaveClientSessionContext waveClientSessionContext = pDiagTestsActionMessage->getWaveClientSessionContext();
		trace (TRACE_LEVEL_INFO, "DiagTestsActionMessageHandler enter..");

		string fosCommand = pDiagTestsActionMessage->getCommand();

		trace (TRACE_LEVEL_INFO, "DiagTestsActionMessageHandler get command: " + fosCommand);
		if ((cmd_file = popen((char *)fosCommand.data(), "r")) != NULL) {
			while (fgets(cmd_output, 512, cmd_file)) {
				trace (TRACE_LEVEL_INFO, string("DiagTestsActionMessageHandler: In fgets"));
				printfToWaveClientSession (waveClientSessionContext, "%s", cmd_output);
			}
			pDiagTestsActionMessage->setResult(string ("\n"));
			if ((rc = pclose(cmd_file)) < 0) {
				trace (TRACE_LEVEL_INFO, string("DiagTestsActionMessageHandler: pclose failed - CMD = ") +
				fosCommand.data() + string (" rc=") + rc + string (" errno=") + -errno);
				printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
			}
		} else {
			trace (TRACE_LEVEL_INFO, string("DiagTestsActionMessageHandler: popen failed - CMD =  ") +
				fosCommand.data() + string (" errno=") + -errno);
			printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
			pDiagTestsActionMessage->setResult(string ("Command execution failed."));
		}

		/*The below function needs to be called to ensure that
          the command returns to prompt after execution - TR000324161*/
		printfToWaveClientSessionOver(waveClientSessionContext); 
		pDiagTestsActionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		reply (pDiagTestsActionMessage);
		trace (TRACE_LEVEL_INFO, "DiagTestsActionMessageHandler replied");
    }

}
