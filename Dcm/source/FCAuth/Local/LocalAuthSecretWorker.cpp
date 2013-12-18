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
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/LocalAuthSecretWorker.h"
#include "FCAuth/Local/FCAuthLocalAddAuthSecretMessage.h"
#include "FCAuth/Local/FCAuthLocalDeleteAuthSecretMessage.h"
#include "FCAuth/Local/FCAuthLocalShowAuthSecretMessage.h"
#include "FCAuth/Local/FCAuthTypes.h"

#include "DcmResourceIdEnums.h"

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"

/* from security/svr.h SEC_WWN_STR_LEN */
#define WWN_STR_LEN 24

namespace DcmNs
{

    LocalAuthSecretWorker::LocalAuthSecretWorker ( FCAuthLocalObjectManager *pFCAuthLocalObjectManager)
        : WaveWorker  (pFCAuthLocalObjectManager)
    {
        addOperationMap (FCAUTHLOCALADDAUTHSECRET, reinterpret_cast<PrismMessageHandler> (&LocalAuthSecretWorker::FCAuthLocalAddAuthSecretMessageHandler));
        addOperationMap (FCAUTHLOCALDELETEAUTHSECRET, reinterpret_cast<PrismMessageHandler> (&LocalAuthSecretWorker::FCAuthLocalDeleteAuthSecretMessageHandler));
        addOperationMap (FCAUTHLOCALSHOWAUTHSECRET, reinterpret_cast<PrismMessageHandler> (&LocalAuthSecretWorker::FCAuthLocalShowAuthSecretMessageHandler));
    }

    LocalAuthSecretWorker::~LocalAuthSecretWorker ()
    {
    }

    PrismMessage  *LocalAuthSecretWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FCAUTHLOCALADDAUTHSECRET :
                pPrismMessage = new FCAuthLocalAddAuthSecretMessage ();
                break;
            case FCAUTHLOCALDELETEAUTHSECRET :
                pPrismMessage = new FCAuthLocalDeleteAuthSecretMessage ();
                break;
            case FCAUTHLOCALSHOWAUTHSECRET :
                pPrismMessage = new FCAuthLocalShowAuthSecretMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalAuthSecretWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void LocalAuthSecretWorker::install (WaveAsynchronousContextForBootPhases
        *pWaveAsynchronousContextForBootPhases)
    {
        pWaveAsynchronousContextForBootPhases->setCompletionStatus
            (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }


    void  LocalAuthSecretWorker::FCAuthLocalAddAuthSecretMessageHandler( FCAuthLocalAddAuthSecretMessage *pFCAuthLocalAddAuthSecretMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string nodeid, peer_secret, local_secret;
        trace (TRACE_LEVEL_INFO, string("LocalAuthSecretWorker::FCAuthLocalAddAuthSecretMessageHandler Enter."));

        // Validations and Queries should go here
        if (pFCAuthLocalAddAuthSecretMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL,
                string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        nodeid = pFCAuthLocalAddAuthSecretMessage->getWwn();
        peer_secret = pFCAuthLocalAddAuthSecretMessage->getPeerSecret();
        local_secret = pFCAuthLocalAddAuthSecretMessage->getLocalSecret();

        if (WWN_STR_LEN != nodeid.size() + 1)
        {
            trace (TRACE_LEVEL_INFO, string("LocalAuthSecretWorker::FCAuthLocalAddAuthSecretMessageHandler invalid wwn size"));
            pFCAuthLocalAddAuthSecretMessage->setCompletionStatus(AUTHSECRET_INVALID_PARAMETER);
            reply (pFCAuthLocalAddAuthSecretMessage);
            return;
        }

        int ret_code = authApiSecretSet((char *)nodeid.c_str(),
            (char *)peer_secret.c_str(), (char *)local_secret.c_str());

        trace (TRACE_LEVEL_INFO, string("LocalAuthSecretWorker::AddAuthSecret ret_code ") + ret_code);
        status = authTranslateRetcode(ret_code);
        trace (TRACE_LEVEL_INFO, string("LocalAuthSecretWorker::FCAuthLocalAddAuthSecretMessageHandler Exiting."));
        pFCAuthLocalAddAuthSecretMessage->setCompletionStatus(status);
        reply (pFCAuthLocalAddAuthSecretMessage);
    }

    void  LocalAuthSecretWorker::FCAuthLocalDeleteAuthSecretMessageHandler( FCAuthLocalDeleteAuthSecretMessage *pFCAuthLocalDeleteAuthSecretMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_INFO, string("LocalAuthSecretWorker::FCAuthLocalDeleteAuthSecretMessageHandler Enter."));

        // Validations and Queries should go here
        if (pFCAuthLocalDeleteAuthSecretMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL,
                string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        string nodeid = pFCAuthLocalDeleteAuthSecretMessage->getWwn();

        int retcode = AUTH_SUCCESS;

        if (0 == nodeid.size())
        {   /* delete all */
            retcode = authApiSecretRemove(NULL);
        }
        else
        {
            retcode = authApiSecretRemove((char *)nodeid.c_str());
        }
        trace (TRACE_LEVEL_INFO, string("LocalAuthSecretWorker::DeleteAuthSecret retcode ") + retcode);
        status = authTranslateRetcode(retcode);

        trace (TRACE_LEVEL_INFO, string("LocalAuthSecretWorker::FCAuthLocalDeleteAuthSecretMessageHandler Exiting."));
        pFCAuthLocalDeleteAuthSecretMessage->setCompletionStatus(status);
        reply (pFCAuthLocalDeleteAuthSecretMessage);
    }

    void  LocalAuthSecretWorker::FCAuthLocalShowAuthSecretMessageHandler( FCAuthLocalShowAuthSecretMessage *pFCAuthLocalShowAuthSecretMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        int count = 0;
        trace (TRACE_LEVEL_INFO, string("LocalAuthSecretWorker::FCAuthLocalShowAuthSecretMessageHandler Enter."));

        // Validations and Queries should go here
        if (pFCAuthLocalShowAuthSecretMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL,
                string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }
        pFCAuthLocalShowAuthSecretMessage->clearNumVectors();

        char *secret_wwns = authApiSecretGet(&count);
        trace (TRACE_LEVEL_INFO,
            string("LocalAuthSecretWorker::GetAuthSecret retcode ") + count);
        if (0 > count)
        {
            status = authTranslateRetcode(count);
        }
        else if (0 == count)
        {
            trace (TRACE_LEVEL_INFO,
                string("LocalAuthSecretWorker::GetAuthSecret no nodes"));
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            char *tmp_str = secret_wwns;
            status = WAVE_MESSAGE_SUCCESS;
            for (int i = 0; i < count; i++)
            {
                string secret_string = tmp_str;
                trace (TRACE_LEVEL_INFO,
                    string("LocalAuthSecretWorker::GetAuthSecret retcode ")
                    + secret_string);
                pFCAuthLocalShowAuthSecretMessage->setWwn(secret_string);
                tmp_str += WWN_STR_LEN;
            }
            if (secret_wwns)
                free (secret_wwns);
        }

        trace (TRACE_LEVEL_INFO, string("LocalAuthSecretWorker::FCAuthLocalShowAuthSecretMessageHandler Exiting."));
        pFCAuthLocalShowAuthSecretMessage->setCompletionStatus(status);
        reply (pFCAuthLocalShowAuthSecretMessage);
    }

    ResourceId LocalAuthSecretWorker::authTranslateRetcode(int retcode)
    {
       switch (retcode)
       {
           case AUTH_SUCCESS:
               return WAVE_MESSAGE_SUCCESS;
               break;
           case AUTH_NO_MEMORY:
               return AUTHSECRET_NO_MEMORY;
               break;
           case AUTH_CMD_ERR:
           case AUTH_INPUT_ERR:
           case AUTH_DATA_ERROR:
           case AUTH_NULL_PTR:
           case AUTH_INVALID_POLICY:
           case AUTH_INVALID_PROTOCOL:
           case AUTH_INVALID_GROUP:
           case AUTH_INVALID_HASH:
               return AUTHSECRET_INVALID_PARAMETER;
               break;
           case AUTH_SET_FAILED:
           case AUTH_NOT_CFG_KEY:
           case AUTH_NO_DHCHAP:
           case AUTH_PROTOCOL_CONFLICT:
           case AUTH_MD5_IN_FIPS:
           case AUTH_INVALID_GROUP_FIPS:
               return AUTHSECRET_CREATE_FAILED;
               break;
           case AUTH_DB_MAX_ENTRIES_LIMIT_REACHED:
               return AUTHSECRET_DB_MAX_ENTRIES_LIMIT_REACHED;
               break;
           default:
               return AUTHSECRET_BACKEND;
               break;
        }
    }

}
