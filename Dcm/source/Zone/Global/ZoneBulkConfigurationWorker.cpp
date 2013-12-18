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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Zone/Global/ZoneBulkConfigurationWorker.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"
#include "Zone/Global/ZoneBulkConfigurationMessage.h"
#include "Zone/Global/CfgManagedObject.h"
#include "Zone/Global/ZoneManagedObject.h"
#include "Zone/Global/ZoneManagedObject.h"
#include "Zone/Global/ZoneMemberManagedObject.h"
#include "Zone/Global/ZoneBulkConfigurationContext.h"
#include "Zone/Global/ZoneBulkConfigurationContext.h"

namespace DcmNs
{

ZoneBulkConfigurationWorker::ZoneBulkConfigurationWorker (ZoneObjectManager *pZoneObjectManager)
    : WaveWorker           (pZoneObjectManager),
      m_pZoneObjectManager (pZoneObjectManager)
{
    addOperationMap (ZONE_BULK_CONFIGURATION, reinterpret_cast<PrismMessageHandler> (&ZoneBulkConfigurationWorker::zoneBulkConfigurationMessageHandler));
}

ZoneBulkConfigurationWorker::~ZoneBulkConfigurationWorker ()
{
}

PrismMessage  *ZoneBulkConfigurationWorker::createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {

        case ZONE_BULK_CONFIGURATION :
            pPrismMessage = new ZoneBulkConfigurationMessage ();

            break;

        default :
            pPrismMessage = NULL;

            break;
    }

    return (pPrismMessage);
}


void ZoneBulkConfigurationWorker::zoneBulkConfigurationMessageHandler (ZoneBulkConfigurationMessage *pZoneBulkConfigurationMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&ZoneBulkConfigurationWorker::zoneBulkConfigurationLoadDatabaseConciseInformationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ZoneBulkConfigurationWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ZoneBulkConfigurationWorker::zoneBulkConfigurationCreateDatabaseEntriesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ZoneBulkConfigurationWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ZoneBulkConfigurationWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ZoneBulkConfigurationWorker::prismLinearSequencerFailedStep)
    };

    ZoneBulkConfigurationContext *pZoneBulkConfigurationContext = new ZoneBulkConfigurationContext (pZoneBulkConfigurationMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pZoneBulkConfigurationContext->holdAll ();
    pZoneBulkConfigurationContext->start   ();
}

void ZoneBulkConfigurationWorker::zoneBulkConfigurationLoadDatabaseConciseInformationStep (ZoneBulkConfigurationContext *pZoneBulkConfigurationContext)
{
    trace (TRACE_LEVEL_INFO, "ZoneBulkConfigurationWorker::zoneBulkConfigurationLoadDatabaseConciseInformationStep : Entering ...");

    pZoneBulkConfigurationContext->loadZoneDatabaseConciseInformarionIntoMemory ();

    pZoneBulkConfigurationContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void ZoneBulkConfigurationWorker::zoneBulkConfigurationCreateDatabaseEntriesStep (ZoneBulkConfigurationContext *pZoneBulkConfigurationContext)
{
    trace (TRACE_LEVEL_INFO, "ZoneBulkConfigurationWorker::zoneBulkConfigurationCreateDatabaseEntriesStep : Entering ...");

    vector<string>          &cfgs                = pZoneBulkConfigurationContext->getCfgs        ();
    vector<vector<string> > &cfgMembers          = pZoneBulkConfigurationContext->getCfgMembers  ();
    vector<string>          &zones               = pZoneBulkConfigurationContext->getZones       ();
    vector<vector<string> > &zoneMembers         = pZoneBulkConfigurationContext->getZoneMembers ();

    UI32                     numberOfCfgs        = 0;
    UI32                     numberOfCfgMembers  = 0;
    UI32                     numberOfZones       = 0;
    UI32                     numberOfZoneMembers = 0;

    UI32                     i                   = 0;
    UI32                     j                   = 0;

    ResourceId               status              = WAVE_MESSAGE_SUCCESS;

    bool                     isValid             = false;
    ObjectId                 objectId;
    bool                     doesExist           = false;
    CfgManagedObject        *pCfgManagedObject   = NULL;
    ZoneManagedObject       *pZoneManagedObject  = NULL;
    string                   keyString;
    string                   memberKeyString;

    numberOfCfgs = cfgs.size ();

    for (i = 0; i < numberOfCfgs; i++)
    {
        pZoneBulkConfigurationContext->getExistanceAndValidatyDetailsForCfg (cfgs[i], isValid, doesExist, objectId);

        if (true == isValid)
        {
            keyString = CfgManagedObject::getClassName () + "*" + cfgs[i];

            if (false == doesExist)
            {
                pCfgManagedObject = new CfgManagedObject (m_pZoneObjectManager);

                prismAssert (NULL != pCfgManagedObject, __FILE__, __LINE__);

                pZoneBulkConfigurationContext->addManagedObjectForGarbageCollection (pCfgManagedObject);

                pCfgManagedObject->setCfgName (cfgs[i]);

                pZoneBulkConfigurationContext->addKeyStringInformation (CfgManagedObject::getClassName (), keyString, pCfgManagedObject->getObjectId (), cfgs[i], true);
            }

            numberOfCfgMembers = (cfgMembers[i]).size ();

            for (j = 0; j < numberOfCfgMembers; j++)
            {
                memberKeyString = keyString + "." + CfgMemberManagedObject::getClassName () + "*" + cfgMembers[i][j];

                if (false == (pZoneBulkConfigurationContext->isAKnownKeyString (memberKeyString)))
                {
                    CfgMemberManagedObject *pCfgMemberManagedObject = new CfgMemberManagedObject (m_pZoneObjectManager);

                    prismAssert (NULL != pCfgMemberManagedObject, __FILE__, __LINE__);

                    pCfgMemberManagedObject->setCfgEntry (cfgMembers[i][j]);

                    pCfgMemberManagedObject->setListIndex (j);

                    if (false == doesExist)
                    {
                        pCfgManagedObject->addCfgMembers (pCfgMemberManagedObject);
                    }
                    else
                    {
                        pCfgMemberManagedObject->setOwnerManagedObjectId (objectId);

                        pCfgMemberManagedObject->prependOwnerKeyString (keyString);

                        addToComposition (CfgManagedObject::getClassName (), CfgMemberManagedObject::getClassName (), "cfgMembers", objectId, pCfgMemberManagedObject->getObjectId ());

                        pZoneBulkConfigurationContext->addManagedObjectForGarbageCollection (pCfgMemberManagedObject);
                    }

                    pZoneBulkConfigurationContext->addKeyStringInformation (CfgMemberManagedObject::getClassName (), memberKeyString, pCfgMemberManagedObject->getObjectId (), cfgMembers[i][j], false);
                }
            }
        }
    }

    numberOfZones = zones.size ();

    for (i = 0; i < numberOfZones; i++)
    {
        pZoneBulkConfigurationContext->getExistanceAndValidatyDetailsForZone (zones[i], isValid, doesExist, objectId);

        if (true == isValid)
        {
            keyString = ZoneManagedObject::getClassName () + "*" + zones[i];

            if (false == doesExist)
            {
                pZoneManagedObject = new ZoneManagedObject (m_pZoneObjectManager);

                prismAssert (NULL != pZoneManagedObject, __FILE__, __LINE__);

                pZoneBulkConfigurationContext->addManagedObjectForGarbageCollection (pZoneManagedObject);

                pZoneManagedObject->setZoneName (zones[i]);

                pZoneBulkConfigurationContext->addKeyStringInformation (ZoneManagedObject::getClassName (), keyString, pZoneManagedObject->getObjectId (), zones[i], true);
            }

            numberOfZoneMembers = (zoneMembers[i]).size ();

            for (j = 0; j < numberOfZoneMembers; j++)
            {
                memberKeyString = keyString + "." + ZoneMemberManagedObject::getClassName () + "*" + zoneMembers[i][j];

                if (false == (pZoneBulkConfigurationContext->isAKnownKeyString (memberKeyString)))
                {
                    ZoneMemberManagedObject *pZoneMemberManagedObject = new ZoneMemberManagedObject (m_pZoneObjectManager);

                    prismAssert (NULL != pZoneMemberManagedObject, __FILE__, __LINE__);

                    pZoneMemberManagedObject->setZoneEntry (zoneMembers[i][j]);

                    pZoneMemberManagedObject->setListIndex (j);

                    if (false == doesExist)
                    {
                        pZoneManagedObject->addZoneMembers (pZoneMemberManagedObject);
                    }
                    else
                    {
                        pZoneMemberManagedObject->setOwnerManagedObjectId (objectId);

                        pZoneMemberManagedObject->prependOwnerKeyString (keyString);

                        addToComposition (ZoneManagedObject::getClassName (), ZoneMemberManagedObject::getClassName (), "zoneMembers", objectId, pZoneMemberManagedObject->getObjectId ());

                        pZoneBulkConfigurationContext->addManagedObjectForGarbageCollection (pZoneMemberManagedObject);
                    }

                    pZoneBulkConfigurationContext->addKeyStringInformation (ZoneMemberManagedObject::getClassName (), memberKeyString, pZoneMemberManagedObject->getObjectId (), zoneMembers[i][j], false);
                }
            }
        }
    }

    pZoneBulkConfigurationContext->executeNextStep (status);
}

}
