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

#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "Framework/ObjectModel/WaveObjectManager.h"

#include "Diag/Local/DiagLocalObjectManager.h"
#include "Diag/Local/DiagLocalPostMessage.h"
#include "Diag/Local/DiagLocalPostWorker.h"
#include "Diag/Local/DiagLocalTypes.h"
#include "DcmShell/DcmShell.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace WaveNs;

namespace DcmNs
{

DiagLocalPostWorker::DiagLocalPostWorker (DiagLocalObjectManager *pDiagLocalObjectManager)
	: WaveWorker  (pDiagLocalObjectManager)
{
	DiagLocalPostManagedObject    DiagLocalPostManagedObject    (pDiagLocalObjectManager);
    DiagLocalPostManagedObject.setupOrm ();
    addManagedClass (DiagLocalPostManagedObject::getClassName (), this);
	addOperationMap (DIAGLOCALPOST, reinterpret_cast<PrismMessageHandler> (&DiagLocalPostWorker::DiagLocalPostMessageHandler));
}

	DiagLocalPostWorker::~DiagLocalPostWorker ()
	{
	}

    PrismMessage  *DiagLocalPostWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case DIAGLOCALPOST :
                pPrismMessage = new DiagLocalPostMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

	WaveManagedObject  *DiagLocalPostWorker::createManagedObjectInstance(const string &managedClassName)
	{
		return NULL;
    }
#define DIAGCLI_FILE "/var/log/diagcli.data"
	void DiagLocalPostWorker::DiagLocalPostMessageHandler (DiagLocalPostMessage *pDiagLocalPostMessage)
	{
        string result = "";
        char cmdline[512];
        ifstream in;
		string fosCommand;

		trace (TRACE_LEVEL_INFO, "DiagLocalPostWorker::DiagLocalPostMessageHandler()");
#if 0
		ResourceId status;
		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = NULL;
		vector<WaveManagedObject *> *pResults = NULL;
		int ret = 0;
#endif

		// Validations and Queries should go here
		if (pDiagLocalPostMessage->getDiagPost() == true) {
			fosCommand = "/fabos/link_sbin/diagenablepost";
		} else {
			fosCommand = "/fabos/link_sbin/diagdisablepost";
		}

		snprintf(cmdline, 512, "%s > %s 2>/dev/null", (char *)fosCommand.data(), DIAGCLI_FILE);

        system(cmdline);

	    in.open(DIAGCLI_FILE);
        if ( ! in ) {
            trace (TRACE_LEVEL_INFO, string ("DiagLocalPostMessageHandler::fosCLI fail:") + cmdline);
            result = "Fail to run ";
            result += fosCommand;
            result += '\n';
        } else {
            string str;
            getline(in, str);  // Get the frist line from the file, if any.
            while ( in ) {  // Continue if the line was sucessfully read.
                result = result + str + '\n';  // Process the line.
                trace (TRACE_LEVEL_DEBUG, string ("fosCLI=") + str);
                getline(in, str, in.widen ('\n'));   // Try to get another line.
            }
            in.close();
        }

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalPostWorker::prismSynchronousLinearSequencerStartTransactionStep),
			// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalPostWorker::prismSynchronousLinearSequencerCommitTransactionStep),
			// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalPostWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalPostWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pDiagLocalPostMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

	}

}
