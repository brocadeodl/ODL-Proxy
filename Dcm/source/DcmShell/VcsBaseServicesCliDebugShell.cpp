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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "DcmShell/VcsBaseServicesCliDebugShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "VcsFabric/Local/VcsNodeLocalClientInterface.h"
#include "VcsFabric/Local/VcsClientLocalParams.h"
#include "VcsFabric/Local/VcsClientVcsIdParams.h"
#include "VcsFabric/Local/VcsClientVcsModeParams.h"

//#include "ValClientInterface/ValClientPhysicalInterface.h"
//#include "ValClientInterface/val_interface.h"

#include <stdlib.h>

namespace DcmNs
{

VcsBaseServicesCliDebugShell::VcsBaseServicesCliDebugShell (ValClientSynchronousConnection &connection)
    : WaveCliShell                     ("baseservicesdebug", connection),
      m_valClientSynchronousConnection (connection)
{
    addCommandfunction ("getLocalMappedId",     reinterpret_cast<WaveShellCommandFunction> (&VcsBaseServicesCliDebugShell::getLocalMappedId),      "Queries DCMd for the mapped id of local switch",       reinterpret_cast<WaveShellCommandHelpFunction> (&VcsBaseServicesCliDebugShell::queryForBaseServicesHelp));
    addCommandfunction ("getLocalMappedIdCway", reinterpret_cast<WaveShellCommandFunction> (&VcsBaseServicesCliDebugShell::getLocalMappedIdCWay),  "Queries DCMd for the mapped id of local switch.",       reinterpret_cast<WaveShellCommandHelpFunction> (&VcsBaseServicesCliDebugShell::queryForBaseServicesHelp));
    addCommandfunction ("setLocalFcfMac",     reinterpret_cast<WaveShellCommandFunction> (&VcsBaseServicesCliDebugShell::setLocalFcfMac),      "Sets the FCF MAC for local vcs node",     reinterpret_cast<WaveShellCommandHelpFunction> (&VcsBaseServicesCliDebugShell::queryForBaseServicesHelp));
    addCommandfunction ("setLocalFcfMacCWay",     reinterpret_cast<WaveShellCommandFunction> (&VcsBaseServicesCliDebugShell::setLocalFcfMacCWay),      "Sets the FCF MAC for local vcs node",     reinterpret_cast<WaveShellCommandHelpFunction> (&VcsBaseServicesCliDebugShell::queryForBaseServicesHelp));
    addCommandfunction ("getVcsId",     reinterpret_cast<WaveShellCommandFunction> (&VcsBaseServicesCliDebugShell::getVcsId),      "Queries DCMd for the VCS id of local switch",       reinterpret_cast<WaveShellCommandHelpFunction> (&VcsBaseServicesCliDebugShell::queryForBaseServicesHelp));
    addCommandfunction ("getVcsMode",     reinterpret_cast<WaveShellCommandFunction> (&VcsBaseServicesCliDebugShell::getVcsMode),      "Queries DCMd for the VCS Mode of local switch",       reinterpret_cast<WaveShellCommandHelpFunction> (&VcsBaseServicesCliDebugShell::queryForBaseServicesHelp));
}

VcsBaseServicesCliDebugShell::~VcsBaseServicesCliDebugShell ()
{
}

ValClientSynchronousConnection &VcsBaseServicesCliDebugShell::getValClientSynchronousConnection ()
{
    return (m_valClientSynchronousConnection);
}

void VcsBaseServicesCliDebugShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Debug shell provides a way to execute debug actions.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Developer can execute pre-registered actions from this shell.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Debug actions that are not particularly part of any other shells");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        typically land here.");
}

ResourceId VcsBaseServicesCliDebugShell::getVcsMode (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    VcsClientVcsModeParams  vcsModeParams;
    int vcsMode;

    ValClientSynchronousConnection::initializeCApi (&connection);

    vcsMode = get_vcs_mode ();

    if ((0 == vcsMode) || (1 == vcsMode)) 
    {
        tracePrintf (TRACE_LEVEL_INFO, false, false, "The Vcs Mode is %d", vcsMode);
        status = WAVE_MESSAGE_SUCCESS;
    }
    else 
    {
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId VcsBaseServicesCliDebugShell::getVcsId (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    VcsClientVcsIdParams  vcsIdParams;
    int vcsId;

    ValClientSynchronousConnection::initializeCApi (&connection);

    vcsId = get_vcs_id ();

    if (0 == vcsId) 
    {
        trace (TRACE_LEVEL_INFO, "The Vcs ID is 0");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else if (0 < vcsId)
    {
        trace (TRACE_LEVEL_INFO, string("Vcs Id : ") + vcsId);
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to get VCS Id - C way");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else 
    {
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId VcsBaseServicesCliDebugShell::getLocalMappedId (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    string                         interfaceName;
    VcsClientLocalParams            params;

    status = connection.vcsNodeLocalGetMappedId (params);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO,    string ("Local Mapped id        : ") + params.getMappedId ());
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to get local mapped_id ");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to get local mapped_id. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId VcsBaseServicesCliDebugShell::getLocalMappedIdCWay (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    vcs_local_node_params            local_params;

    ValClientSynchronousConnection::initializeCApi (&connection);

    status = vcs_get_local_mapped_id_interface (&local_params);

    if (0 == status)
    {
        trace (TRACE_LEVEL_INFO, string("Local Mapped Id : ") + local_params.mapped_id);
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to get local mapped_id - C way");

    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to get local mapped_id - C way. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId VcsBaseServicesCliDebugShell::setLocalFcfMac (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    char                           macArray[6];
    UI32                             i;

    if (1 != arguments.size ())
    {
        queryForBaseServicesHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    MacAddress   tmpMac(arguments[0]);

    for (i=0; i<6; i++)
    {
        macArray[i] = tmpMac[i];
    }

    status = connection.vcsNodeLocalSetFcfMac (macArray);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to set local FCF Mac ");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to set local FCF Mac. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId VcsBaseServicesCliDebugShell::setLocalFcfMacCWay (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);
    char                           macArray[6];
    UI32                             i;

    if (1 != arguments.size ())
    {
        queryForBaseServicesHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    MacAddress   tmpMac(arguments[0]);

    for (i=0; i<6; i++)
    {
        macArray[i] = tmpMac[i];
    }

    status = vcs_set_local_fcf_mac_interface (macArray);

    if (0 == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to set local FCF Mac - C way");

    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to set local FCF Mac - C way. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void VcsBaseServicesCliDebugShell::queryForBaseServicesHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : get_local_mappedid");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Gets the Mapped Id of the local VCS NODE");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    none");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Displays the Mapped Id of the local VCS Node");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

}
