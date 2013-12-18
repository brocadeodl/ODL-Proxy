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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Database/MultiDatabaseObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryWorker.h"
#include "Framework/Database/MultiDatabaseObjectManagerSetupServersWorker.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessage.h"
#include "Framework/Database/DatabaseObjectManagerExecuteCursorCommandWorker.h"
#include "Framework/Database/MultiDatabaseToolKit.h"

namespace WaveNs
{

MultiDatabaseObjectManager::MultiDatabaseObjectManager ()
    : WaveLocalObjectManager (getClassName ())
{
    MultiDatabaseToolKit::setIsMultiModeEnabled (true);

    DatabaseObjectManagerExecuteQueryMessage::setDatabaseObjectManagerServiceId (getServiceId ());

    m_pDatabaseObjectManagerExecuteQueryWorker = new DatabaseObjectManagerExecuteQueryWorker (this, true);

    prismAssert (NULL != m_pDatabaseObjectManagerExecuteQueryWorker, __FILE__, __LINE__);

    m_pMultiDatabaseObjectManagerSetupServersWorker = new MultiDatabaseObjectManagerSetupServersWorker (this);

    prismAssert (NULL != m_pMultiDatabaseObjectManagerSetupServersWorker, __FILE__, __LINE__);

    m_pDatabaseObjectManagerExecuteCursorCommandWorker = new DatabaseObjectManagerExecuteCursorCommandWorker (this, true);

    prismAssert (NULL != m_pDatabaseObjectManagerExecuteCursorCommandWorker, __FILE__, __LINE__);
}

MultiDatabaseObjectManager::~MultiDatabaseObjectManager ()
{
    if (NULL != m_pDatabaseObjectManagerExecuteQueryWorker)
    {
        delete m_pDatabaseObjectManagerExecuteQueryWorker;

        m_pDatabaseObjectManagerExecuteQueryWorker = NULL;
    }

    if (NULL != m_pMultiDatabaseObjectManagerSetupServersWorker)
    {
        delete m_pMultiDatabaseObjectManagerSetupServersWorker;

        m_pMultiDatabaseObjectManagerSetupServersWorker = NULL;
    }

    if (NULL != m_pDatabaseObjectManagerExecuteCursorCommandWorker)
    {
        delete m_pDatabaseObjectManagerExecuteCursorCommandWorker;

        m_pDatabaseObjectManagerExecuteCursorCommandWorker = NULL;
    }
}

MultiDatabaseObjectManager *MultiDatabaseObjectManager::getInstance ()
{
    static MultiDatabaseObjectManager *pMultiDatacaseObjectManager = NULL;

    if (NULL == pMultiDatacaseObjectManager)
    {
        pMultiDatacaseObjectManager = new MultiDatabaseObjectManager ();

        WaveNs::prismAssert (NULL != pMultiDatacaseObjectManager, __FILE__, __LINE__);
    }

    return (pMultiDatacaseObjectManager);
}

PrismServiceId MultiDatabaseObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string MultiDatabaseObjectManager::getClassName ()
{
    return ("Multi Wave Database");
}

void MultiDatabaseObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

WaveManagedObject *MultiDatabaseObjectManager::loadWaveManagedObjectFromDatabase (const ObjectId &waveManagedObjectObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    return (m_pDatabaseObjectManagerExecuteQueryWorker->loadWaveManagedObjectFromDatabase (waveManagedObjectObjectId, schema, pWaveObjectManager));
}

WaveManagedObject *MultiDatabaseObjectManager::loadWaveManagedObjectFromDatabaseWrapper (const ObjectId &waveManagedObjectObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    return ((getInstance ())->loadWaveManagedObjectFromDatabase (waveManagedObjectObjectId, schema, pWaveObjectManager));
}

vector<WaveManagedObject *> MultiDatabaseObjectManager::loadWaveManagedObjectsFromDatabase (const vector<ObjectId> &waveManagedObjectObjectIds, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    return (m_pDatabaseObjectManagerExecuteQueryWorker->loadWaveManagedObjectsFromDatabase (waveManagedObjectObjectIds, schema, pWaveObjectManager));
}

vector<WaveManagedObject *> MultiDatabaseObjectManager::loadWaveManagedObjectsFromDatabaseWrapper (const vector<ObjectId> &waveManagedObjectObjectIds, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    return ((getInstance ())->loadWaveManagedObjectsFromDatabase (waveManagedObjectObjectIds, schema, pWaveObjectManager));
}

vector<WaveManagedObject *> MultiDatabaseObjectManager::loadWaveManagedObjectsFromDatabase (const string &childClassName, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    return (m_pDatabaseObjectManagerExecuteQueryWorker->loadWaveManagedObjectsFromDatabase (childClassName, parentObjectId, schema, pWaveObjectManager));
}

vector<WaveManagedObject *> MultiDatabaseObjectManager::loadWaveManagedObjectsFromDatabaseWrapper (const string &childClassName, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    return ((getInstance ())->loadWaveManagedObjectsFromDatabase (childClassName, parentObjectId, schema, pWaveObjectManager));
}


}
