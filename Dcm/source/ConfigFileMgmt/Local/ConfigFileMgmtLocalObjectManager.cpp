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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"

#include "Framework/Persistence/PersistenceLocalObjectManagerSetStartupFileMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerCopySchemaMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerCopyFileMessage.h"
#include "ConfigFileMgmt/Local/ConfigFileMgmtLocalObjectManager.h"
#include "ConfigFileMgmt/Local/ConfigFileMgmtLocalTypes.h"
#include "ConfigFileMgmt/Local/ConfigFileMgmtMessages.h"

#include <raslog/raslog.h>
#include <raslog/raslog_dcm.h>
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "ValClientInterface/ValClientSynchronousConnection.h" 

/*
 * ATTENTION:
 *  All new FOS includes should be added to this
 *  global header file.  This will help to ensure
 *  that libInit() has been called for any libraries
 *  that are required.
 */
#include "dcm/fos_headers.h"

namespace DcmNs
{

ConfigFileMgmtLocalObjectManager::ConfigFileMgmtLocalObjectManager ()
    : WaveLocalObjectManager (getClassName ())
{
    trace (TRACE_LEVEL_INFO, "ConfigFileMgmtLocalObjectManager:: "
            "Adding copyToStartupConfigMessageHandler");

    addOperationMap (COPY_TO_STARTUP_CONFIG, reinterpret_cast<PrismMessageHandler> (&ConfigFileMgmtLocalObjectManager::copyToStartupConfigMessageHandler));
    addOperationMap (COPY_RUNNING_TO_STARTUP, reinterpret_cast<PrismMessageHandler> (&ConfigFileMgmtLocalObjectManager::copyRunningToStartupMessageHandler));
    addOperationMap (COPY_DEFAULT_TO_STARTUP, reinterpret_cast<PrismMessageHandler> (&ConfigFileMgmtLocalObjectManager::copyDefaultToStartupMessageHandler));
    addOperationMap (COPY_TO_STARTUP_ALLOWED, reinterpret_cast<PrismMessageHandler> (&ConfigFileMgmtLocalObjectManager::copyToStartupAllowedMessageHandler));
}

ConfigFileMgmtLocalObjectManager::~ConfigFileMgmtLocalObjectManager ()
{
}

string ConfigFileMgmtLocalObjectManager::getClassName ()
{
    return ("Config File Local Management");
}

ConfigFileMgmtLocalObjectManager *ConfigFileMgmtLocalObjectManager::getInstance ()
{
    static ConfigFileMgmtLocalObjectManager *pConfigFileMgmtLocalObjectManager =
        new ConfigFileMgmtLocalObjectManager ();

    WaveNs::prismAssert (NULL != pConfigFileMgmtLocalObjectManager, __FILE__, __LINE__);

    return (pConfigFileMgmtLocalObjectManager);
}

PrismServiceId ConfigFileMgmtLocalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *ConfigFileMgmtLocalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case COPY_TO_STARTUP_CONFIG:
            pPrismMessage = new ConfigFileMgmtCopyToStartupConfigMessage ();
            break;
        case COPY_RUNNING_TO_STARTUP:
            pPrismMessage = new ConfigFileMgmtCopyRunningToStartupMessage ();
            break;
        case COPY_DEFAULT_TO_STARTUP:
            pPrismMessage = new ConfigFileMgmtCopyDefaultToStartupMessage ();
            break;
        case COPY_TO_STARTUP_ALLOWED:
            pPrismMessage = new ConfigFileMgmtCopyToStartupAllowedMessage ();
            break;
        default :
            pPrismMessage = NULL;
            break;
    }

    return (pPrismMessage);
}

WaveManagedObject  *ConfigFileMgmtLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
{
        return NULL;
}


void ConfigFileMgmtLocalObjectManager::copyToStartupConfigMessageHandler (ConfigFileMgmtCopyToStartupConfigMessage *pCopyToStartupConfigMessage)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "ConfigFileMgmtLocalObjectManager::copyToStartupConfigMessageHandler has been deprecated.  Please use the VcsCopyToStartupMessage instead.");
}

void ConfigFileMgmtLocalObjectManager::copyRunningToStartupMessageHandler (ConfigFileMgmtCopyRunningToStartupMessage *pCopyRunningToStartupMessage)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "ConfigFileMgmtLocalObjectManager::copyRunningToStartupMessageHandler has been deprecated.  Please use the VcsCopyRunningToStartupMessage instead.");
}

void ConfigFileMgmtLocalObjectManager::copyDefaultToStartupMessageHandler (ConfigFileMgmtCopyDefaultToStartupMessage *pCopyDefaultToStartupMessage)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "ConfigFileMgmtLocalObjectManager::copyDefaultToStartupMessageHandler been deprecated.  Please use the VcsCopyDefaultToStartupMessage instead.");
}

void ConfigFileMgmtLocalObjectManager::copyToStartupAllowedMessageHandler (ConfigFileMgmtCopyToStartupAllowedMessage *pCopyToStartupAllowedMessage)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "ConfigFileMgmtLocalObjectManager::copyToStartupAllowedMessageHandler been deprecated.  Please use the VcsCopyToStartupAllowedMessage instead.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
}

