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

#include "Diag/Local/DiagShowActionMessage.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Diag/Local/DiagShowLocalObjectManager.h"
#include "Diag/Local/DiagShowActionMessage.h"
#include "Diag/Local/DiagLocalTypes.h"
#include "DcmShell/DcmShell.h"
#include <iostream>
#include <string>
#include <fstream>
#include "errno.h"

namespace DcmNs
{

    DiagShowLocalObjectManager::DiagShowLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
		addOperationMap (DIAGSHOWACTION, reinterpret_cast<PrismMessageHandler> (&DiagShowLocalObjectManager::DiagShowActionMessageHandler));
    }

    DiagShowLocalObjectManager::~DiagShowLocalObjectManager ()
    {
    }

    DiagShowLocalObjectManager  *DiagShowLocalObjectManager::getInstance()
    {
        static DiagShowLocalObjectManager *pDiagShowLocalObjectManager = new DiagShowLocalObjectManager ();

        WaveNs::prismAssert (NULL != pDiagShowLocalObjectManager, __FILE__, __LINE__);

        return (pDiagShowLocalObjectManager);
    }

    string  DiagShowLocalObjectManager::getClassName()
    {
        return ("Diag Show Local Manager");
    }

    PrismServiceId  DiagShowLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *DiagShowLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
			case DIAGSHOWACTION :
				pPrismMessage = new DiagShowActionMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DiagShowLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

#define DIAGCLI_FILE "/var/log/diagcli.data"
	 void  DiagShowLocalObjectManager::DiagShowActionMessageHandler( DiagShowActionMessage *pDiagShowActionMessage)
    {
        FILE    *cmd_file = 0;
        int     rc;
        char    cmd_output[513];
        string  result = "";
        ifstream in;

        WaveClientSessionContext waveClientSessionContext = pDiagShowActionMessage->getWaveClientSessionContext();
        trace (TRACE_LEVEL_INFO, "DiagShowActionMessageHandler enter..");

        string fosCommand = pDiagShowActionMessage->getCommand();

        trace (TRACE_LEVEL_INFO, "DiagShowActionMessageHandler get command: " + fosCommand);
        if ((cmd_file = popen((char *)fosCommand.data(), "r")) != NULL) {
            while (fgets(cmd_output, 512, cmd_file)) {
                trace (TRACE_LEVEL_INFO, string("DiagShowActionMessageHandler: In fgets"));
//              printfToWaveClientSession (waveClientSessionContext, "%s", cmd_output);
                result += cmd_output;
            }
            pDiagShowActionMessage->setResult(result);
            if ((rc = pclose(cmd_file)) < 0) {
                trace (TRACE_LEVEL_INFO, string("DiagShowActionMessageHandler: pclose failed - CMD = ") +
                fosCommand.data() + string (" rc=") + rc + string (" errno=") + -errno);
                printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
            }
        } else {
            trace (TRACE_LEVEL_INFO, string("DiagShowActionMessageHandler: popen failed - CMD =  ") +
                fosCommand.data() + string (" errno=") + -errno);
            printfToWaveClientSession (waveClientSessionContext, "%s", "Command Failed\n");
            pDiagShowActionMessage->setResult(string ("Command execution failed."));
        }

		/*The below function needs to be called to ensure that
        the command returns to prompt after execution - TR000324161*/
        printfToWaveClientSessionOver(waveClientSessionContext);
        pDiagShowActionMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pDiagShowActionMessage);
        trace (TRACE_LEVEL_INFO, "DiagShowActionMessageHandler replied");
    }

}
