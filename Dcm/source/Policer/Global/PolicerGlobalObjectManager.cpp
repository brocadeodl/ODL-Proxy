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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#include "Policer/Global/PolicerGlobalObjectManager.h"
#include "Policer/Global/PolicerGlobalWorker.h"
#include "Policer/Global/PolicerGlobalTypes.h"

namespace DcmNs
{

    PolicerGlobalObjectManager::PolicerGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pPolicerGlobalWorker = new PolicerGlobalWorker (this);
        prismAssert (NULL != m_pPolicerGlobalWorker, __FILE__, __LINE__);
    }

    PolicerGlobalObjectManager::~PolicerGlobalObjectManager ()
    {
    }

    PolicerGlobalObjectManager  *PolicerGlobalObjectManager::getInstance()
    {
        static PolicerGlobalObjectManager *pPolicerGlobalObjectManager = new PolicerGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pPolicerGlobalObjectManager, __FILE__, __LINE__);

        return (pPolicerGlobalObjectManager);
    }

    string  PolicerGlobalObjectManager::getClassName()
    {
        return ("PolicerGlobal");
    }

    PrismServiceId  PolicerGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *PolicerGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *PolicerGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
