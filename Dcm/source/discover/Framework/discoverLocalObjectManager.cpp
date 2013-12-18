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

#include "discover/Framework/discoverLocalObjectManager.h"
#include "discover/User/discoverLocalWorkers.h"
#include "discover/User/DiscoverLocalMultiWorker.h"
#include "discover/Framework/discoverLocalTypes.h"

namespace DcmNs
{

    DiscoverLocalObjectManager::DiscoverLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
  //  #ifdef _X86_COMPILE_
        m_pDiscoverMultiLocalWorker = new DiscoverLocalMultiWorker(this);
        prismAssert (NULL != m_pDiscoverMultiLocalWorker, __FILE__, __LINE__);
  /*  #else
        m_pDiscoverLocalWorker = new DiscoverLocalWorker (this);
        prismAssert (NULL != m_pDiscoverLocalWorker, __FILE__, __LINE__);
#endif */
    }

    DiscoverLocalObjectManager::~DiscoverLocalObjectManager ()
    {
    }

    DiscoverLocalObjectManager  *DiscoverLocalObjectManager::getInstance()
    {
        static DiscoverLocalObjectManager *pDiscoverLocalObjectManager = new DiscoverLocalObjectManager ();

        WaveNs::prismAssert (NULL != pDiscoverLocalObjectManager, __FILE__, __LINE__);

        return (pDiscoverLocalObjectManager);
    }

    string  DiscoverLocalObjectManager::getClassName()
    {
        return ("DiscoverLocal");
    }

    PrismServiceId  DiscoverLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *DiscoverLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DiscoverLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
