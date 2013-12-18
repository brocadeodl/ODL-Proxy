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
 **************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "FCAuth/Global/FCAuthGlobalObjectManager.h"
#include "FCAuth/Local/ActivePolicyLocalManagedObject.h"
#include "FCAuth/Local/ActivePolicyMemberLocalManagedObject.h"
#include "FCAuth/Local/DefinedPolicyLocalManagedObject.h"
#include "FCAuth/Local/DefinedPolicyMemberLocalManagedObject.h"
#include "FCAuth/Local/AuthPolicyLocalManagedObject.h"
#include "FCAuth/Local/AuthProtocolLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"

namespace DcmNs
{

    FCAuthGlobalObjectManager::FCAuthGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {

    }

    FCAuthGlobalObjectManager::~FCAuthGlobalObjectManager ()
    {
    }

    FCAuthGlobalObjectManager  *FCAuthGlobalObjectManager::getInstance()
    {
        static FCAuthGlobalObjectManager *pFCAuthGlobalObjectManager = new FCAuthGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pFCAuthGlobalObjectManager, __FILE__, __LINE__);

        return (pFCAuthGlobalObjectManager);
    }

    string  FCAuthGlobalObjectManager::getClassName()
    {
        return ("FCAuthGlobal");
    }

    PrismServiceId  FCAuthGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    void FCAuthGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
    {
        FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
        vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_DEVEL, string ("FCAuthGlobalObjectManager::failover failover reason") + FrameworkToolKit::localize(failoverReason));
		if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
		{
		UI32 noOfFailedLocations = failedLocationIds.size ();
        
        for (UI32 i = 0; i < noOfFailedLocations; i++)
        {
			startTransaction ();
			WaveManagedObjectSynchronousQueryContextForDeletion *pContextActiveMember =
                new WaveManagedObjectSynchronousQueryContextForDeletion(ActivePolicyMemberLocalManagedObject::getClassName());
            pContextActiveMember->setLocationIdFilter (failedLocationIds[i]);
			deleteWaveManagedObject(pContextActiveMember);
            WaveManagedObjectSynchronousQueryContextForDeletion *pContextActive =
                new WaveManagedObjectSynchronousQueryContextForDeletion(ActivePolicyLocalManagedObject::getClassName());
            pContextActive->setLocationIdFilter (failedLocationIds[i]);
			deleteWaveManagedObject(pContextActive);
			WaveManagedObjectSynchronousQueryContextForDeletion *pContextDefineMember = 
                        new WaveManagedObjectSynchronousQueryContextForDeletion (DefinedPolicyMemberLocalManagedObject::getClassName());
			pContextDefineMember->setLocationIdFilter (failedLocationIds[i]);
            deleteWaveManagedObject(pContextDefineMember);
            WaveManagedObjectSynchronousQueryContextForDeletion *pContextDefine =
                new WaveManagedObjectSynchronousQueryContextForDeletion(DefinedPolicyLocalManagedObject::getClassName());
            pContextDefine->setLocationIdFilter (failedLocationIds[i]);
            deleteWaveManagedObject(pContextDefine);
			WaveManagedObjectSynchronousQueryContextForDeletion *pContextAuthProto = 
				new WaveManagedObjectSynchronousQueryContextForDeletion(AuthProtocolLocalManagedObject::getClassName ());
			pContextAuthProto->setLocationIdFilter (failedLocationIds[i]);
			deleteWaveManagedObject(pContextAuthProto);
			WaveManagedObjectSynchronousQueryContextForDeletion *pContextAuthPolicy =
				new WaveManagedObjectSynchronousQueryContextForDeletion(AuthPolicyLocalManagedObject::getClassName ());
			pContextAuthPolicy->setLocationIdFilter (failedLocationIds[i]);
			deleteWaveManagedObject(pContextAuthPolicy);
			status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "FCAuthGlobalObjectManager::failover can not commit local managed object delete to database ");
            }
#if 0
            vector<WaveManagedObject *> *pDefinedPolicyMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], DefinedPolicyLocalManagedObject::getClassName ());
			vector<WaveManagedObject *> *pDefinedPolicyMemberMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], DefinedPolicyMemberLocalManagedObject::getClassName ());
			vector<WaveManagedObject *> *pActivePolicyMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], ActivePolicyLocalManagedObject::getClassName ());
			vector<WaveManagedObject *> *pActivePolicyMemberMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], ActivePolicyMemberLocalManagedObject::getClassName ());
			vector<WaveManagedObject *> *pAuthProtoMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], AuthProtocolLocalManagedObject::getClassName ());
			vector<WaveManagedObject *> *pAuthPolicyMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], AuthPolicyLocalManagedObject::getClassName ());

            startTransaction ();

			UI32 sizeDefPolMemMO = pDefinedPolicyMemberMO->size ();
			for (UI32 j = 0; j < sizeDefPolMemMO; j++)
			{
				delete (*pDefinedPolicyMemberMO)[j];
			}

			UI32 sizeDefPolMO = pDefinedPolicyMO->size ();
            for (UI32 j = 0; j < sizeDefPolMO; j++)
            {
                delete (*pDefinedPolicyMO)[j];
            }

			UI32 sizeActPolMemMO = pActivePolicyMemberMO->size ();
			for (UI32 j = 0; j < sizeActPolMemMO; j++)
			{
				delete (*pActivePolicyMemberMO)[j];
			}

			UI32 sizeActPolMO = pActivePolicyMO->size ();
			for (UI32 j = 0; j < sizeActPolMO; j++)
			{
				delete (*pActivePolicyMO)[j];
			}

			UI32 sizeAuthProtoMO = pAuthProtoMO->size ();
			for (UI32 j = 0; j < sizeAuthProtoMO; j++)
			{
				delete (*pAuthProtoMO)[j];
			}

			UI32 sizeAuthPolicyMO = pAuthPolicyMO->size ();
			for (UI32 j = 0; j < sizeAuthPolicyMO; j++)
			{
				delete (*pAuthPolicyMO)[j];
			}

            status = commitTransaction ();

            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "FCAuthGlobalObjectManager::failover can not commit local managed object delete to database ");
            }

            pDefinedPolicyMemberMO->clear ();
            delete pDefinedPolicyMemberMO;
			pDefinedPolicyMO->clear ();
			delete pDefinedPolicyMO;
			pActivePolicyMemberMO->clear ();
			delete pActivePolicyMemberMO;
			pActivePolicyMO->clear ();
			delete pActivePolicyMO;
			pAuthProtoMO->clear ();
			delete pAuthProtoMO;
			pAuthPolicyMO->clear ();
			delete pAuthPolicyMO;
#endif
        }
		}
        pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pFailoverAsynchronousContext->callback ();
    }
    PrismMessage  *FCAuthGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }
    WaveManagedObject  *FCAuthGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
