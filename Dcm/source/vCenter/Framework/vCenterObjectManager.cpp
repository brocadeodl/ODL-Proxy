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

#include "vCenter/Framework/vCenterObjectManager.h"
#include "vCenter/User/vCenterWorkers.h"
#include "vCenter/Framework/vCenterTypes.h"

namespace DcmNs
{

    vCenterObjectManager::vCenterObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pVcenterWorker = new VcenterWorker (this);
        prismAssert (NULL != m_pVcenterWorker, __FILE__, __LINE__);
    }

    vCenterObjectManager::~vCenterObjectManager ()
    {
    }

    vCenterObjectManager  *vCenterObjectManager::getInstance()
    {
        static vCenterObjectManager *pvCenterObjectManager = new vCenterObjectManager ();

        WaveNs::prismAssert (NULL != pvCenterObjectManager, __FILE__, __LINE__);

        return (pvCenterObjectManager);
    }

    string  vCenterObjectManager::getClassName()
    {
        return ("vCenter");
    }

    PrismServiceId  vCenterObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *vCenterObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *vCenterObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}

