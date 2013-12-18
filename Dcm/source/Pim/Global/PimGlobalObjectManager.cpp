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
 *   Author : rjain                                                     *
 **************************************************************************/

#include "Pim/Global/PimGlobalObjectManager.h"
#include "Pim/Global/PimGlobalWorker.h"
#include "Pim/Global/PimGlobalTypes.h"

namespace DcmNs
{

    PimGlobalObjectManager::PimGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pPimGlobalWorker = new PimGlobalWorker (this);
        prismAssert (NULL != m_pPimGlobalWorker, __FILE__, __LINE__);
    }

    PimGlobalObjectManager::~PimGlobalObjectManager ()
    {
    }

    PimGlobalObjectManager  *PimGlobalObjectManager::getInstance()
    {
        static PimGlobalObjectManager *pPimGlobalObjectManager = new PimGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pPimGlobalObjectManager, __FILE__, __LINE__);

        return (pPimGlobalObjectManager);
    }

    string  PimGlobalObjectManager::getClassName()
    {
        return ("PimGlobal");
    }

    PrismServiceId  PimGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *PimGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *PimGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
