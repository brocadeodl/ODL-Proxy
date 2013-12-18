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
 *   Author : lhu                                                     *
 **************************************************************************/

#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmResourceIdEnums.h"

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Terminal/Global/TerminalObjectManager.h"
#include "Terminal/Global/TerminalManagedObject.h"
#include "Terminal/Global/TerminalSessionCfgMessage.h"
#include "Terminal/Global/TerminalTypes.h"

namespace DcmNs
{

    TerminalObjectManager::TerminalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        TerminalManagedObject    TerminalManagedObject    (this);
        TerminalManagedObject.setupOrm ();
        addManagedClass (TerminalManagedObject::getClassName ());
        addOperationMap (TERMINALSESSIONCFG, reinterpret_cast<PrismMessageHandler> (&TerminalObjectManager::TerminalSessionCfgMessageHandler));
    }

    TerminalObjectManager::~TerminalObjectManager ()
    {
    }

    TerminalObjectManager  *TerminalObjectManager::getInstance()
    {
        static TerminalObjectManager *pTerminalObjectManager = new TerminalObjectManager ();

        WaveNs::prismAssert (NULL != pTerminalObjectManager, __FILE__, __LINE__);

        return (pTerminalObjectManager);
    }

    string  TerminalObjectManager::getClassName()
    {
        return ("Terminal");
    }

    PrismServiceId  TerminalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *TerminalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case TERMINALSESSIONCFG :
                pPrismMessage = new TerminalSessionCfgMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *TerminalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((TerminalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new TerminalManagedObject(this);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "TerminalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

void  TerminalObjectManager::TerminalSessionCfgMessageHandler( TerminalSessionCfgMessage *pTerminalSessionCfgMessage)
{
	Terminal_cfg operation = pTerminalSessionCfgMessage->getOperationtype();
	Session_Id sessionid = pTerminalSessionCfgMessage->getSessionid();
	LeafValue_flag flag = pTerminalSessionCfgMessage->getExectimeout_flag();
	UI32 exectimeout = pTerminalSessionCfgMessage->getExectimeout();

	TerminalManagedObject *pObj = NULL;
	vector<WaveManagedObject *> *pResults = NULL;

	trace (TRACE_LEVEL_INFO, string("handler: parameters: line (") + sessionid + string("), oper(") + operation + string("), flag(") + flag + string("), exectimeout(") + exectimeout + string(") for"));

	if (operation != TERMINALCFG_CREATE)
	{
		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext =
            new WaveManagedObjectSynchronousQueryContext(TerminalManagedObject::getClassName());

          if (sessionid != SESSION_ALL)
          {
            synchronousQueryContext->addAndAttribute (new AttributeUI32 (sessionid, "sessionid"));
          }
        pResults = querySynchronously (synchronousQueryContext);

          if ((NULL == pResults) || (pResults->size() == 0))
          {
            trace (TRACE_LEVEL_FATAL, "Terminal session not found");
            pTerminalSessionCfgMessage->setCompletionStatus (TERMINAL_SESSION_NOT_EXIST);
            reply (pTerminalSessionCfgMessage);
            if (pResults != NULL)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            delete synchronousQueryContext;
            return;
          }
		delete synchronousQueryContext;
	}

	startTransaction ();
	if (operation == TERMINALCFG_CREATE)
	{
        pObj = dynamic_cast<TerminalManagedObject *>(createManagedObjectInstance(TerminalManagedObject::getClassName()));
        pObj->setSessionid(sessionid);

		if (flag == VALUE_UPDATED)
		{
        	pObj->setExectimeout(pTerminalSessionCfgMessage->getExectimeout());
		}
		else if ((flag == VALUE_DEFAULT) || (flag == VALUE_NOCHANGE))
		{
			pObj->setExectimeout(TERMINAL_DEFAULT_EXECTIMEOUT);
		}
	}
	else if (operation == TERMINALCFG_UPDATE)
	{
		pObj = dynamic_cast<TerminalManagedObject *> ((*pResults)[0]);
		updateWaveManagedObject(pObj);

        if (flag == VALUE_UPDATED)
        {
            pObj->setExectimeout(exectimeout);
			trace (TRACE_LEVEL_DEBUG, "set exectime to" + exectimeout); 
        }
        else if (flag == VALUE_DEFAULT)
        {
            pObj->setExectimeout(TERMINAL_DEFAULT_EXECTIMEOUT);
        }
	}
	else if (operation == TERMINALCFG_REMOVE)
	{
		unsigned int i;
		for (i = 0; i < (pResults->size()); i++)
		{
        	pObj = dynamic_cast<TerminalManagedObject *> ((*pResults)[i]);
		delete pObj;
		}
	}

	ResourceId status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
	{
		status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "Wave database commit is successful.");

        // TBDDCMOSS : sagar : Execute system specific code to reset terminal configuration via wyser.
	}
	else
	{
		status = TERMINAL_SESSION_CFG_FAIL;
		trace (TRACE_LEVEL_ERROR, "Wave DB commit failed. Framework error - " + FrameworkToolKit::localize (status));
	}

	/* 
	 * During create and delete it is not required to query the DB
	 * Release the memory in case of update other case delete the object 
	 */
        if (operation == TERMINALCFG_CREATE)
                delete (pObj);
	else if (operation == TERMINALCFG_REMOVE)
		delete (pResults);
	else if (operation == TERMINALCFG_UPDATE)
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

    pTerminalSessionCfgMessage->setCompletionStatus(status);
    reply (pTerminalSessionCfgMessage);

    trace (TRACE_LEVEL_DEBUG, "TerminalSessionCfgMessageHandler - EXIT.");
    return;
}


void TerminalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
	ResourceId status = WAVE_MESSAGE_SUCCESS;

    /****************************************************************************
     * Create managed object and commit to database
     ***************************************************************************/
    trace (TRACE_LEVEL_DEBUG, "TerminalObjectManager::install: Install");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
	trace(TRACE_LEVEL_INFO, string ("TerminalObjectManager::install: bootReason") + bootReason);

    if ((bootReason == WAVE_BOOT_FIRST_TIME_BOOT) ||
		(bootReason == WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT))
	{
        startTransaction ();
    	//TerminalManagedObject *pConsoleMO = new TerminalManagedObject(this, SESSION_CONSOLE, TERMINAL_DEFAULT_EXECTIMEOUT);
		TerminalManagedObject *pVtyMO =  new TerminalManagedObject(this, SESSION_VTY, TERMINAL_DEFAULT_EXECTIMEOUT);
        status = commitTransaction ();

    	if (FRAMEWORK_SUCCESS == status)
		{
            trace(TRACE_LEVEL_DEBUG, "TerminalObjectManager::install: Installing Term DB complete");
        	status = WAVE_MESSAGE_SUCCESS;
    	}
		else
		{
            trace(TRACE_LEVEL_FATAL, "TerminalObjectManager::install: Failed to install Term DB");
        	prismAssert (false, __FILE__, __LINE__);
			status = WAVE_MESSAGE_ERROR;
    	}
    	//delete pConsoleMO;
		delete pVtyMO;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}
}
