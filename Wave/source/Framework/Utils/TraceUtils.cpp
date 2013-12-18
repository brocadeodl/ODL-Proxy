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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

#include <stdarg.h>

namespace WaveNs
{

void trace (TraceLevel traceLevel, const string &stringToTrace)
{
    WaveManagementInterfaceRole waveManagementInterfaceRole = FrameworkToolKit::getManagementInterfaceRole ();

    if ((WAVE_MGMT_INTF_ROLE_SERVER == waveManagementInterfaceRole) || (WAVE_MGMT_INTF_ROLE_CLI == waveManagementInterfaceRole))
    {
        (PrismFrameworkObjectManager::getInstance ())->trace (traceLevel, stringToTrace);
    }
    else
    {
        (WaveUserInterfaceObjectManager::getInstance ())->trace (traceLevel, stringToTrace);
    }
}

void tracePrintf (TraceLevel traceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, ...)
{
    va_list variableArguments;

    va_start (variableArguments, pFormat);

    WaveManagementInterfaceRole waveManagementInterfaceRole = FrameworkToolKit::getManagementInterfaceRole ();

    if ((WAVE_MGMT_INTF_ROLE_SERVER == waveManagementInterfaceRole) || (WAVE_MGMT_INTF_ROLE_CLI == waveManagementInterfaceRole))
    {
        (PrismFrameworkObjectManager::getInstance ())->tracePrintf (traceLevel, addNewLine, suppressPrefix, pFormat, variableArguments);
    }
    else
    {
        (WaveUserInterfaceObjectManager::getInstance ())->tracePrintf (traceLevel, addNewLine, suppressPrefix, pFormat, variableArguments);
    }

    va_end (variableArguments);
}

void tracePrintf (TraceLevel traceLevel, const char * const pFormat, ...)
{
    va_list variableArguments;

    va_start (variableArguments, pFormat);

    WaveManagementInterfaceRole waveManagementInterfaceRole = FrameworkToolKit::getManagementInterfaceRole ();

    if ((WAVE_MGMT_INTF_ROLE_SERVER == waveManagementInterfaceRole) || (WAVE_MGMT_INTF_ROLE_CLI == waveManagementInterfaceRole))
    {
        (PrismFrameworkObjectManager::getInstance ())->tracePrintf (traceLevel, pFormat, variableArguments);
    }
    else
    {
        (WaveUserInterfaceObjectManager::getInstance ())->tracePrintf (traceLevel, pFormat, variableArguments);
    }

    va_end (variableArguments);
}

}
