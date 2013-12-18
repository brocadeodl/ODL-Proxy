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
 *   Author : sharis                                                     *
 **************************************************************************/

#include "vCenter/Framework/vCenterLocalObjectManager.h"
#include "vCenter/User/vCenterLocalWorkers.h"
#include "vCenter/Framework/vCenterLocalTypes.h"
#include "DcmCore/DcmToolKit.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace DcmNs
{

    vCenterLocalObjectManager::vCenterLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pVcenterLocalWorker = new VcenterLocalWorker (this);
        prismAssert (NULL != m_pVcenterLocalWorker, __FILE__, __LINE__);

    }

    vCenterLocalObjectManager::~vCenterLocalObjectManager ()
    {
    }

    vCenterLocalObjectManager  *vCenterLocalObjectManager::getInstance()
    {
        static vCenterLocalObjectManager *pvCenterLocalObjectManager = new vCenterLocalObjectManager ();

        WaveNs::prismAssert (NULL != pvCenterLocalObjectManager, __FILE__, __LINE__);

        return (pvCenterLocalObjectManager);
    }

    string  vCenterLocalObjectManager::getClassName()
    {
        return ("vCenterLocal");
    }

    PrismServiceId  vCenterLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *vCenterLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }




    WaveManagedObject  *vCenterLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
