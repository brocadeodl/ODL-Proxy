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
 *   Author : mmahajan                                                     *
 **************************************************************************/

#include "TrillOam/Local/TrillOamLocalObjectManager.h"
#include "TrillOam/Local/TrillOamLocalWorker.h"
#include "TrillOam/Local/TrillOamTypes.h"

namespace DcmNs
{

    TrillOamLocalObjectManager::TrillOamLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pTrillOamLocalWorker = new TrillOamLocalWorker (this);
        prismAssert (NULL != m_pTrillOamLocalWorker, __FILE__, __LINE__);
    }

    TrillOamLocalObjectManager::~TrillOamLocalObjectManager ()
    {
    }

    TrillOamLocalObjectManager  *TrillOamLocalObjectManager::getInstance()
    {
        static TrillOamLocalObjectManager *pTrillOamLocalObjectManager = new TrillOamLocalObjectManager ();

        WaveNs::prismAssert (NULL != pTrillOamLocalObjectManager, __FILE__, __LINE__);

        return (pTrillOamLocalObjectManager);
    }

    string  TrillOamLocalObjectManager::getClassName()
    {
        return ("TrillOam");
    }

    PrismServiceId  TrillOamLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *TrillOamLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *TrillOamLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
