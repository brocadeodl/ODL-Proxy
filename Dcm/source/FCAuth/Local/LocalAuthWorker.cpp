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

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/LocalAuthWorker.h"
#include "FCAuth/Local/FCAuthLocalSetPolicyMessage.h"
#include "FCAuth/Local/FCAuthLocalSetProtocolMessage.h"
#include "FCAuth/Local/AuthPolicyLocalManagedObject.h"
#include "FCAuth/Local/AuthProtocolLocalManagedObject.h"
#include "FCAuth/Local/FCAuthTypes.h"

#include "DcmResourceIdEnums.h"
#include "auth/authipc.h"
#include "fabos/license.h"

/* hardcoded values from lib/auth/authlib.c authApiSet function */
#define AUTHUTILSTART_AUTH_SET "authset"
#define AUTHUTILSTART_GROUP_SET "groupset"
#define AUTHUTILSTART_HASH_SET "hashset"
#define AUTHUTIL_ALLTYPE_STR "all"
#define AUTHUTIL_ALLGROUP_STR "*"
#define AUTHUTIL_ALLHASH_STR "all"

namespace DcmNs
{

    LocalAuthWorker::LocalAuthWorker ( FCAuthLocalObjectManager *pFCAuthLocalObjectManager)
        : WaveWorker  (pFCAuthLocalObjectManager)
    {
        AuthPolicyLocalManagedObject    AuthPolicyLocalManagedObject    (pFCAuthLocalObjectManager);
        AuthProtocolLocalManagedObject    AuthProtocolLocalManagedObject    (pFCAuthLocalObjectManager);
        AuthPolicyLocalManagedObject.setupOrm ();
        addManagedClass (AuthPolicyLocalManagedObject::getClassName (), this);
        AuthProtocolLocalManagedObject.setupOrm ();
        addManagedClass (AuthProtocolLocalManagedObject::getClassName (), this);
        addOperationMap (FCAUTHLOCALSETPOLICY, reinterpret_cast<PrismMessageHandler> (&LocalAuthWorker::FCAuthLocalSetPolicyMessageHandler));
        addOperationMap (FCAUTHLOCALSETPROTOCOL, reinterpret_cast<PrismMessageHandler> (&LocalAuthWorker::FCAuthLocalSetProtocolMessageHandler));
    }

    LocalAuthWorker::~LocalAuthWorker ()
    {
    }

    PrismMessage  *LocalAuthWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FCAUTHLOCALSETPOLICY :
                pPrismMessage = new FCAuthLocalSetPolicyMessage ();
                break;
            case FCAUTHLOCALSETPROTOCOL :
                pPrismMessage = new FCAuthLocalSetProtocolMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalAuthWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((AuthPolicyLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new AuthPolicyLocalManagedObject(dynamic_cast<FCAuthLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((AuthProtocolLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new AuthProtocolLocalManagedObject(dynamic_cast<FCAuthLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalAuthWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    ResourceId LocalAuthWorker::pushBackend()
    {
        int auth_status = AUTH_SUCCESS;

        /* first push policy */
        authOptions_e policy = AUTH_PASSIVE;  /* default passive */
#if 0
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(AuthPolicyLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
#endif
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, AuthPolicyLocalManagedObject::getClassName());
        if ((pResults) && (pResults->size ()))
        {
            AuthPolicyLocalManagedObject *pMO = dynamic_cast
                <AuthPolicyLocalManagedObject *>((*pResults)[0]);
            AuthEnforceType enforce = pMO->getEnforceType();

            delete pMO;
   
            switch (enforce)
            {
                case on:      policy = AUTH_ON;      break;
                case off:     policy = AUTH_OFF;     break;
                case active:  policy = AUTH_ACTIVE;  break;
                case passive: policy = AUTH_PASSIVE; break;
                default:      policy = AUTH_PASSIVE; break;
            }
        }
        if (pResults)
           delete pResults;

        /* send to backend */
        auth_status = authSetPolicy(AUTH_SET_POLICY, (int)policy);
        trace (TRACE_LEVEL_INFO, "LocalAuthWorker::pushBackend policy auth_status " + auth_status);
        ResourceId status = translateRetcode(auth_status);
        trace (TRACE_LEVEL_INFO, "LocalAuthWorker::pushBackend policy "
            + FrameworkToolKit::localize (status));

        /* next push protocol data to backend */
        char *msg[3];
		char messages[3][20];
        auth_status = AUTH_SUCCESS;
        AuthProtocolType auth_type = dhchap; /* default dhchap */
        AuthProtocolGroup auth_group = AUTHUTIL_ALLGROUP_STR; /* all */
        AuthProtocolHash auth_hash = all; /* default all */
#if 0
        /* query for local db values */
        WaveManagedObjectSynchronousQueryContext *syncQuery2 = new WaveManagedObjectSynchronousQueryContext(AuthProtocolLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults2 = querySynchronously (syncQuery2);
#endif
		vector<WaveManagedObject *> *pResults2 = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, AuthProtocolLocalManagedObject::getClassName());
        if ((pResults2) && (pResults2->size ()))
        {
            AuthProtocolLocalManagedObject *pMO2 = dynamic_cast
                <AuthProtocolLocalManagedObject *>((*pResults2)[0]);

            auth_type = pMO2->getProtocolAuthType();
            auth_group = pMO2->getProtocolAuthGroup();
            auth_hash = pMO2->getProtocolAuthHash();

            delete pMO2;
        }
        if (pResults2)
           delete pResults2;

        /* send protocol auth-type to backend */
        /* create msg array "authset" */
        strncpy(messages[0], AUTHUTILSTART_AUTH_SET, sizeof (messages[0]));
		msg[0] = messages[0];
        msg[1] = NULL;
        msg[2] = NULL;
        switch (auth_type)
        {
            case dhchap:
				strncpy(messages[1], AUTH_DHCHAP_STR, sizeof (messages[1]));
                msg[1] = messages[1];
                break;

            case fcap:
				strncpy(messages[1], AUTH_FCAP_STR, sizeof (messages[1]));
                msg[1] = messages[1];
                break;

            case fcap_dhchap:
            case FCAUTH_PROTOCOLTYPE_MAX:
            default:
                /* all other values set to default */
				strncpy(messages[1], AUTHUTIL_ALLTYPE_STR, sizeof (messages[1]));
                msg[1] = messages[1];
                break;
        }
        auth_status = authApiSet(msg);
        trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::pushBackend setting authType auth_status ") + auth_status);
        status = translateRetcode(auth_status);
        trace (TRACE_LEVEL_INFO,
            string("LocalAuthWorker::pushBackend setting authType to ")
            + string(msg[1]) + string(" status ")
            + FrameworkToolKit::localize (status));

        /* send protocol group to backend */
        auth_status = AUTH_SUCCESS;
        if (("0" != auth_group) && ("1" != auth_group) &&
            ("2" != auth_group) && ("3" != auth_group) &&
            ("4" != auth_group))
        {
            auth_group = AUTHUTIL_ALLGROUP_STR;
        }

        /* create msg array "groupset" */
        string auth_str = (string)auth_group;
		strncpy(messages[0], AUTHUTILSTART_GROUP_SET, sizeof (messages[0]));
        msg[0] = messages[0];
        msg[1] = (char *)auth_str.c_str();
        msg[2] = NULL;

        auth_status = authApiSet(msg);
        trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::pushBackend setting authGroup auth_status ") + auth_status);
        status = translateRetcode(auth_status);
        trace (TRACE_LEVEL_INFO,
            string("LocalAuthWorker::pushBackend setting authGroup to ")
            + string(msg[1]) + string(" status ")
            + FrameworkToolKit::localize (status));

        /* send protocol hash to backend */
        auth_status = AUTH_SUCCESS;
        /* create msg array "hashset" */
		strncpy(messages[0], AUTHUTILSTART_HASH_SET, sizeof (messages[0]));
        msg[0] = messages[0];
        msg[1] = NULL;
        msg[2] = NULL;
        switch (auth_hash)
        {
            case sha1:
				strncpy(messages[1], AUTH_SHA1_STR, sizeof (messages[1]));
                msg[1] = messages[1];
                break;

            case md5:
				strncpy(messages[1], AUTH_MD5_STR, sizeof (messages[1]));
                msg[1] = messages[1];
                break;

            case all:
            case FCAUTH_PROTOCOLHASHTYPE_MAX:
            default:
				strncpy(messages[1], AUTHUTIL_ALLHASH_STR, sizeof (messages[1]));
                msg[1] = messages[1];
                break;
        }
        auth_status = authApiSet(msg);

        trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::pushBackend setting authHash auth_status ") + auth_status);
        status = translateRetcode(auth_status);
        trace (TRACE_LEVEL_INFO,
            string("LocalAuthWorker::pushBackend setting authHash to ")
            + string(msg[1]) + string(" status ")
            + FrameworkToolKit::localize (status));

        return WAVE_MESSAGE_SUCCESS;
    }

    void LocalAuthWorker::boot (WaveAsynchronousContextForBootPhases
        *pWaveAsynchronousContextForBootPhases)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

        if (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason)
        { // only CONFIGURE needs to push data to backend.
            status = pushBackend();
        }

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status); 
        pWaveAsynchronousContextForBootPhases->callback ();
    }

    int LocalAuthWorker::fwBackendOpAuthUtil ()
    {
        if (WAVE_MESSAGE_SUCCESS == pushBackend())
        {
            return 0;
        }

        trace (TRACE_LEVEL_INFO, "LocalAuthWorker::fwBackendOpAuthUtil err");
        return 1;
    }

    void  LocalAuthWorker::FCAuthLocalSetPolicyMessageHandler( FCAuthLocalSetPolicyMessage *pFCAuthLocalSetPolicyMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        int auth_status = AUTH_SUCCESS;

        trace (TRACE_LEVEL_INFO, "LocalAuthWorker::FCAuthLocalSetPolicyMessageHandler Enter.");

        /* validate data */
        if (NULL == pFCAuthLocalSetPolicyMessage)
        {
            trace (TRACE_LEVEL_FATAL, "FCAuthWorker NULL msg in the handler");
            prismAssert (false, __FILE__, __LINE__);
        }
		if (!isSupportedPlatform()) {
            trace (TRACE_LEVEL_INFO, "LocalAuthWorker::FCAuthLocalSetPolicyMessageHandler "
                    "Command not supported in this platform");
            pFCAuthLocalSetPolicyMessage->setCompletionStatus(FCSPAUTH_UNSUPPORTED_PLATFORM);
            reply(pFCAuthLocalSetPolicyMessage);
            return;
        } 

        AuthEnforceType enforce =
            pFCAuthLocalSetPolicyMessage->getEnforceType();
        authOptions_e policy;

        switch (enforce)
        {
            case on:
                policy = AUTH_ON;
                break;

            case off:
                policy = AUTH_OFF;
                break;

            case active:
                policy = AUTH_ACTIVE;
                break;

            case passive:
                policy = AUTH_PASSIVE;
                break;

            default:
                policy = AUTH_MAX;
                break;
        }

        if (AUTH_MAX != policy)
        {
            /* send to backend */
            auth_status = authSetPolicy(AUTH_SET_POLICY, (int)policy);
        }
        else
        {
            auth_status = AUTH_INVALID_POLICY;
        }

        trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::setpolicy auth_status ") + auth_status);
        status = translateRetcode(auth_status);

        /* check status */
        if (AUTH_SUCCESS == auth_status)
        {
            /* update local db */
            AuthPolicyLocalManagedObject *pMO = NULL;
#if 0
            WaveManagedObjectSynchronousQueryContext *syncQuery = new
                WaveManagedObjectSynchronousQueryContext
                (AuthPolicyLocalManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults =
                querySynchronously (syncQuery);
#endif
			LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
        	vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, AuthPolicyLocalManagedObject::getClassName());

            if (NULL == pResults || 1 > pResults->size())
            {
                trace (TRACE_LEVEL_INFO, "LocalAuthWorker::FCAuthLocalSetPolicyMessageHandler creating new entry in db");
                startTransaction();
                pMO = new AuthPolicyLocalManagedObject(dynamic_cast
                    <FCAuthLocalObjectManager *>(getPWaveObjectManager()));
                pMO->setEnforceType
                    (pFCAuthLocalSetPolicyMessage->getEnforceType());
                status = commitTransaction();
                delete pMO;
                if (FRAMEWORK_SUCCESS == status)
                {
                    trace (TRACE_LEVEL_INFO, "LocalAuthWorker::SetPolicy success");
                    status = WAVE_MESSAGE_SUCCESS;
                }
            }
            else if (1 == pResults->size())
            {
                trace (TRACE_LEVEL_INFO, "LocalAuthWorker::setting enforce policy type");
                pMO = dynamic_cast <AuthPolicyLocalManagedObject *>
                    ((*pResults)[0]);
                startTransaction();
                updateWaveManagedObject (pMO);
                pMO->setEnforceType(pFCAuthLocalSetPolicyMessage->getEnforceType());
                status = commitTransaction();
                delete pMO;
                if (FRAMEWORK_SUCCESS == status)
                {
                    trace (TRACE_LEVEL_INFO, "LocalAuthWorker::SetPolicy success");
                    status = WAVE_MESSAGE_SUCCESS;
                }
            }
            else
            {
                /* db error, too many entries */
                trace (TRACE_LEVEL_ERROR, "LocalAuthWorker::db query too many results");
                status = FCSPAUTH_SET_FAILED;
            }
            if (pResults)
                delete pResults;
        }

        /* send response */
        pFCAuthLocalSetPolicyMessage->setCompletionStatus(status);
        reply(pFCAuthLocalSetPolicyMessage);
    }

    void  LocalAuthWorker::FCAuthLocalSetProtocolMessageHandler( FCAuthLocalSetProtocolMessage *pFCAuthLocalSetProtocolMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        int auth_status = AUTH_INVALID_PROTOCOL;
        AuthProtocolType auth_type = dhchap;
        AuthProtocolGroup auth_group = "";
        AuthProtocolHash auth_hash = FCAUTH_PROTOCOLHASHTYPE_MAX;
        trace (TRACE_LEVEL_INFO, "LocalAuthWorker::FCAuthLocalSetProtocolMessageHandler Enter.");

        /* validate data */
        if (NULL == pFCAuthLocalSetProtocolMessage)
        {
            trace (TRACE_LEVEL_FATAL, "FCAuthWorker NULL msg in the handler");
            prismAssert (false, __FILE__, __LINE__);
        }

		if (!isSupportedPlatform()) {
            trace (TRACE_LEVEL_INFO, "LocalAuthWorker::FCAuthLocalSetProtocolMessageHandler "
                    "Command not supported in this platform");
            pFCAuthLocalSetProtocolMessage->setCompletionStatus(FCSPAUTH_UNSUPPORTED_PLATFORM);
            reply(pFCAuthLocalSetProtocolMessage);
            return;
        }

        auth_type = pFCAuthLocalSetProtocolMessage->getAuthType();
        if (FCAUTH_PROTOCOLTYPE_MAX != auth_type)
        {
            /* create msg array "authset" */
            char *msg[3];
			char messages[3][20];
			strncpy(messages[0], AUTHUTILSTART_AUTH_SET, sizeof (messages[0]));
            msg[0] = messages[0];
            msg[1] = NULL;
            msg[2] = NULL;
            switch (auth_type)
            {
                case dhchap:
					strncpy(messages[1], AUTH_DHCHAP_STR, sizeof (messages[1]));
		            msg[1] = messages[1];
                    auth_status = AUTH_SUCCESS;
                    break;

                case fcap:
					strncpy(messages[1], AUTH_FCAP_STR, sizeof (messages[1]));
		            msg[1] = messages[1];
                    auth_status = AUTH_SUCCESS;
                    break;

                case fcap_dhchap:
					strncpy(messages[1], AUTHUTIL_ALLTYPE_STR, sizeof (messages[1]));
		            msg[1] = messages[1];
                    auth_status = AUTH_SUCCESS;
                    break;

                default:
                    auth_status = AUTH_INVALID_PROTOCOL;
                    break;
            }

            if (AUTH_SUCCESS == auth_status)
            {
                auth_status = authApiSet(msg);
            }

            trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::settype auth_status ") + auth_status);
            status = translateRetcode(auth_status);

            trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::FCAuthLocalSetProtocolMessageHandler setting backend authType to ") + string(msg[1]));
        }

        auth_group = pFCAuthLocalSetProtocolMessage->getAuthGroup();
        if ("" != auth_group)
        {
            /* create msg array "groupset" */
            string auth_str = (string)auth_group;
            char *msg[3];
			char messages[3][20];
			strncpy(messages[0], AUTHUTILSTART_GROUP_SET, sizeof (messages[0]));
			msg[0] = messages[0];
            msg[1] = (char *)auth_str.c_str();
            msg[2] = NULL;
            auth_status = authApiSet(msg);

            trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::setgroup auth_status ") + auth_status);
            status = translateRetcode(auth_status);

            trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::FCAuthLocalSetProtocolMessageHandler setting backend authGroup to ") + string(msg[1]));
        }

        auth_hash = pFCAuthLocalSetProtocolMessage->getAuthHash();
        if (FCAUTH_PROTOCOLHASHTYPE_MAX != auth_hash)
        {
            /* create msg array "hashset" */
            char *msg[3];
			char messages[3][20];
			strncpy(messages[0], AUTHUTILSTART_HASH_SET, sizeof (messages[0]));
			msg[0] = messages[0];
            msg[1] = NULL;
            msg[2] = NULL;
            switch (auth_hash)
            {
                case sha1:
					strncpy(messages[1], AUTH_SHA1_STR, sizeof (messages[1]));
					msg[1] = messages[1];
                    auth_status = AUTH_SUCCESS;
                    break;

                case md5:
					strncpy(messages[1], AUTH_MD5_STR, sizeof (messages[1]));
					msg[1] = messages[1];
                    auth_status = AUTH_SUCCESS;
                    break;

                case all:
					strncpy(messages[1], AUTHUTIL_ALLHASH_STR, sizeof (messages[1]));
					msg[1] = messages[1];
                    auth_status = AUTH_SUCCESS;
                    break;

                default:
                    auth_status = AUTH_INVALID_HASH;
                    break;
            }

            if (AUTH_SUCCESS == auth_status)
            {
                auth_status = authApiSet(msg);
            }
            trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::sethash auth_status ") + auth_status);
            status = translateRetcode(auth_status);

            trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::FCAuthLocalSetProtocolMessageHandler setting backend authHash to ") + string(msg[1]));
        }

        /* check status */
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            /* update local db */
            AuthProtocolLocalManagedObject *pMO = NULL;
#if 0
            WaveManagedObjectSynchronousQueryContext *syncQuery = new
                WaveManagedObjectSynchronousQueryContext
                (AuthProtocolLocalManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults =
                querySynchronously (syncQuery);
#endif
			LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
        	vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, AuthProtocolLocalManagedObject::getClassName());

            if (NULL == pResults || 1 > pResults->size())
            {
                trace (TRACE_LEVEL_INFO, "LocalAuthWorker::FCAuthLocalSetProtocolMessageHandler creating new entry in db");
                startTransaction();
                pMO = new AuthProtocolLocalManagedObject(dynamic_cast<FCAuthLocalObjectManager *>(getPWaveObjectManager()));
                if (FCAUTH_PROTOCOLTYPE_MAX != auth_type)
                    pMO->setProtocolAuthType(auth_type);
                if ("" != auth_group)
                    pMO->setProtocolAuthGroup(auth_group);
                if (FCAUTH_PROTOCOLHASHTYPE_MAX != auth_hash)
                    pMO->setProtocolAuthHash(auth_hash);
                status = commitTransaction();
                delete pMO;
                if (FRAMEWORK_SUCCESS == status)
                {
                    trace (TRACE_LEVEL_INFO, "LocalAuthWorker::SetProtocol success");
                    status = WAVE_MESSAGE_SUCCESS;
                }
            }
            else if (1 == pResults->size())
            {
                trace (TRACE_LEVEL_INFO, "LocalAuthWorker::setting protocol");
                pMO = dynamic_cast <AuthProtocolLocalManagedObject *>
                    ((*pResults)[0]);
                startTransaction ();
                updateWaveManagedObject (pMO);
                if (FCAUTH_PROTOCOLTYPE_MAX != auth_type)
                    pMO->setProtocolAuthType(auth_type);
                if ("" != auth_group)
                    pMO->setProtocolAuthGroup(auth_group);
                if (FCAUTH_PROTOCOLHASHTYPE_MAX != auth_hash)
                    pMO->setProtocolAuthHash(auth_hash);
                status = commitTransaction();
                delete pMO;
                if (FRAMEWORK_SUCCESS == status)
                    status = WAVE_MESSAGE_SUCCESS;
                else
                    trace (TRACE_LEVEL_FATAL, "FCAuthLocalSetProtocolMessageHandler : Commit failed with status" + FrameworkToolKit::localize (status));
            }
            else
            {
                /* db error, too many entries */
                trace (TRACE_LEVEL_ERROR, "LocalAuthWorker::db query too many results");
                status = FCSPAUTH_SET_FAILED;
            }
            if (pResults)
                delete pResults;
        }

        /* send response */
        pFCAuthLocalSetProtocolMessage->setCompletionStatus(status);
        reply(pFCAuthLocalSetProtocolMessage);
    }

     ResourceId LocalAuthWorker::translateRetcode(int retcode)
     {
       switch (retcode)
       {
           case AUTH_SUCCESS:
               return WAVE_MESSAGE_SUCCESS;
               break;
           case AUTH_NO_MEMORY:
               return FCSPAUTH_NO_MEMORY;
               break;
           case AUTH_CMD_ERR:
           case AUTH_INPUT_ERR:
           case AUTH_DATA_ERROR:
           case AUTH_NULL_PTR:
           case AUTH_INVALID_POLICY:
           case AUTH_INVALID_PROTOCOL:
           case AUTH_INVALID_GROUP:
           case AUTH_INVALID_HASH:
               return FCSPAUTH_INVALID_PARAMETER;
               break;
           case AUTH_SET_FAILED:
               return FCSPAUTH_SET_FAILED;
               break;
           case AUTH_NOT_CFG_KEY:
           case AUTH_NO_DHCHAP:
           case AUTH_PROTOCOL_CONFLICT:
           case AUTH_MD5_IN_FIPS:
           case AUTH_INVALID_GROUP_FIPS:
               return FCSPAUTH_PROTOCOL_CONFLICT;
               break;
           default:
               return FCSPAUTH_SET_FAILED;
               break;
        }
    }

	int LocalAuthWorker::isSupportedPlatform()
	{
		int swModel = get_pod_SWBD();
		int isSupported = 0;
		trace (TRACE_LEVEL_INFO, string("LocalAuthWorker::isSupportedPlatform: swModel:  ") + swModel);

		if ((SWBD_ELARA2F != swModel) && (SWBD_CALLISTOF != swModel)) {
			isSupported = 0;
		} else {
			isSupported = 1;
		}
		return (isSupported);
	}

}
