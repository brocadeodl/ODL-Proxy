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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/ConfigCommunityIntWorker.h"
#include "SnmpServer/Local/ConfigHostIntWorker.h"
#include "SnmpServer/Local/ConfigV3HostIntWorker.h"
#include "SnmpServer/Local/ConfigUserIntWorker.h"
#include "SnmpServer/Local/ConfigSystemGroupIntWorker.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "SnmpServer/Local/ConfigContextMappingIntWorker.h"
#include "SnmpServer/Global/ContextMappingManagedObject.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmResourceIdEnums.h"
#include <snmp/snmplib.h>
#include "SnmpServer/Local/EngineIDLocalManagedObject.h"
#include "SnmpServer/Local/SnmpEngineIDIntWorker.h"

namespace DcmNs
{

    SnmpServerIntLocalObjectManager::SnmpServerIntLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pConfigCommunityIntWorker = new ConfigCommunityIntWorker (this);
        prismAssert (NULL != m_pConfigCommunityIntWorker, __FILE__, __LINE__);
        m_pConfigHostIntWorker = new ConfigHostIntWorker (this);
        prismAssert (NULL != m_pConfigHostIntWorker, __FILE__, __LINE__);
        m_pConfigV3HostIntWorker = new ConfigV3HostIntWorker (this);
        prismAssert (NULL != m_pConfigV3HostIntWorker, __FILE__, __LINE__);
        m_pConfigUserIntWorker = new ConfigUserIntWorker (this);
        prismAssert (NULL != m_pConfigUserIntWorker, __FILE__, __LINE__);
        m_pConfigSystemGroupIntWorker = new ConfigSystemGroupIntWorker (this);
        prismAssert (NULL != m_pConfigSystemGroupIntWorker, __FILE__, __LINE__);
        m_pConfigContextMappingIntWorker = new ConfigContextMappingIntWorker (this);
        prismAssert (NULL != m_pConfigContextMappingIntWorker, __FILE__, __LINE__);
        m_pSnmpEngineIDIntWorker = new SnmpEngineIDIntWorker (this);
        prismAssert (NULL != m_pSnmpEngineIDIntWorker, __FILE__, __LINE__);
    }

    SnmpServerIntLocalObjectManager::~SnmpServerIntLocalObjectManager ()
    {
		delete (m_pConfigCommunityIntWorker);
		delete (m_pConfigHostIntWorker);
		delete (m_pConfigV3HostIntWorker);
		delete (m_pConfigUserIntWorker);
		delete (m_pConfigSystemGroupIntWorker);
        delete (m_pConfigContextMappingIntWorker);
        delete (m_pSnmpEngineIDIntWorker);
    }

    SnmpServerIntLocalObjectManager  *SnmpServerIntLocalObjectManager::getInstance()
    {
        static SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager = new SnmpServerIntLocalObjectManager ();

        WaveNs::prismAssert (NULL != pSnmpServerIntLocalObjectManager, __FILE__, __LINE__);

        return (pSnmpServerIntLocalObjectManager);
    }

    string  SnmpServerIntLocalObjectManager::getClassName()
    {
        return ("SnmpServerInt");
    }

    PrismServiceId  SnmpServerIntLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *SnmpServerIntLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SnmpServerIntLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }
}
