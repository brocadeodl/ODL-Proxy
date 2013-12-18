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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Interface/Test/DcmInterfaceTestObjectManager.h"
#include "Interface/Test/DcmInterfaceTestContext.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectQueryContext.h"
#include "Interface/InterfaceLocalManagedObject.h"

namespace DcmNs
{

DcmInterfaceTestObjectManager::DcmInterfaceTestObjectManager ()
    : PrismTestObjectManager (getClassName ())
{
}

DcmInterfaceTestObjectManager::~DcmInterfaceTestObjectManager ()
{
}

DcmInterfaceTestObjectManager *DcmInterfaceTestObjectManager::getInstance ()
{
    static DcmInterfaceTestObjectManager *pDcmInterfaceTestObjectManager = new DcmInterfaceTestObjectManager ();

    WaveNs::prismAssert (NULL != pDcmInterfaceTestObjectManager, __FILE__, __LINE__);

    return (pDcmInterfaceTestObjectManager);
}

PrismServiceId DcmInterfaceTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string DcmInterfaceTestObjectManager::getClassName ()
{
    return ("DCM Interface (MO) Test");
}

void DcmInterfaceTestObjectManager::testRequestHandler (RegressionTestMessage *pRegressionTestMessage)
{

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&DcmInterfaceTestObjectManager::testOperationalDataQueryInterfacesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DcmInterfaceTestObjectManager::testOperationalDataLoadOperationalDataInterfacesStep),

        reinterpret_cast<PrismLinearSequencerStep> (&DcmInterfaceTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DcmInterfaceTestObjectManager::prismLinearSequencerFailedStep)
    };

    DcmInterfaceTestContext *pDcmInterfaceTestContext = new DcmInterfaceTestContext (pRegressionTestMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDcmInterfaceTestContext->start ();
}

void DcmInterfaceTestObjectManager::testOperationalDataQueryInterfacesStep (DcmInterfaceTestContext *pDcmInterfaceTestContext)
{
    trace (TRACE_LEVEL_DEVEL, "DcmInterfaceTestObjectManager::testOperationalDataQueryInterfacesStep : Entering ...");

    vector<WaveManagedObject *> *pResults = querySynchronously (InterfaceLocalManagedObject::getClassName ());
    ResourceId                   status   = WAVE_MESSAGE_SUCCESS;

    if (NULL == pResults)
    {
        trace (TRACE_LEVEL_FATAL, "DcmInterfaceTestObjectManager::testOperationalDataQueryInterfacesStep : Query Returned Null Result Pointer.  Something is wrong with the system.");
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        UI32                         numberOfResults                                        = pResults->size ();
        UI32                         i                                                      = 0;
        InterfaceLocalManagedObject *pInterfaceLocalManagedObject                           = NULL;
        WaveManagedObject           *pWaveManagedObject                                     = NULL;
        InterfaceLocalManagedObject *pInterfaceLocalManagedObjectWithOperationalDataUpdated = NULL;
        ResourceId                   operationalDataStatus                                  = WAVE_MESSAGE_ERROR;
        vector<string>               operationalDataFields;

        trace (TRACE_LEVEL_INFO, string ("    Number Of Interface Obtained : ") + numberOfResults);

        for (i = 0; i < numberOfResults; i++)
        {
            pInterfaceLocalManagedObject = dynamic_cast<InterfaceLocalManagedObject *> ((*pResults)[i]);

            prismAssert (NULL != pInterfaceLocalManagedObject, __FILE__, __LINE__);

            operationalDataStatus = pInterfaceLocalManagedObject->loadOperationalDataSynchronousWrapper (operationalDataFields, &pWaveManagedObject);

            if (WAVE_MESSAGE_SUCCESS == operationalDataStatus)
            {
                prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

                pInterfaceLocalManagedObjectWithOperationalDataUpdated = dynamic_cast<InterfaceLocalManagedObject *> (pWaveManagedObject);

                prismAssert (NULL != pInterfaceLocalManagedObjectWithOperationalDataUpdated, __FILE__, __LINE__);

                trace (TRACE_LEVEL_INFO, string ("        Interface ") + pInterfaceLocalManagedObjectWithOperationalDataUpdated->getName ());
                trace (TRACE_LEVEL_INFO, string ("            m_operationalUI32 = ") + pInterfaceLocalManagedObjectWithOperationalDataUpdated->getOperationalUI32 ());

                delete pInterfaceLocalManagedObjectWithOperationalDataUpdated;
            }

            delete pInterfaceLocalManagedObject;
        }

        delete pResults;
    }

    pDcmInterfaceTestContext->executeNextStep (status);
}

void DcmInterfaceTestObjectManager::testOperationalDataLoadOperationalDataInterfacesStep (DcmInterfaceTestContext *pDcmInterfaceTestContext)
{
    trace (TRACE_LEVEL_DEVEL, "DcmInterfaceTestObjectManager::testOperationalDataQueryInterfacesStep : Entering ...");

    pDcmInterfaceTestContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

}
