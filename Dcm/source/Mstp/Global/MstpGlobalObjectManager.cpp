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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Mstp/Global/MstpGlobalObjectManager.h"

namespace DcmNs
{

    MstpGlobalObjectManager::MstpGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pMstpGlobalWorker = new MstpGlobalWorker (this);
        prismAssert (NULL != m_pMstpGlobalWorker, __FILE__, __LINE__);
    }

    MstpGlobalObjectManager::~MstpGlobalObjectManager ()
    {
    }

    MstpGlobalObjectManager  *MstpGlobalObjectManager::getInstance()
    {
        static MstpGlobalObjectManager *pMstpGlobalObjectManager = new MstpGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pMstpGlobalObjectManager, __FILE__, __LINE__);

        return (pMstpGlobalObjectManager);
    }

    PrismServiceId  MstpGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *MstpGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        return (pPrismMessage);
    }

    WaveManagedObject  *MstpGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
