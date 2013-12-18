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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "SoftwareManagement/SoftwareManagementVersionWorker.h"
#include "SoftwareManagement/SoftwareManagementObjectManager.h"
#include "SoftwareManagement/SoftwareManagementTypes.h"
#include "SoftwareManagement/SoftwareManagementGetVersionMessage.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Version/PrismVersion.h"
#include "Shell/ShellDebug.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

SoftwareManagementVersionWorker::SoftwareManagementVersionWorker (SoftwareManagementObjectManager *pSoftwareManagementObjectManager)
    : WaveWorker (pSoftwareManagementObjectManager)
{
    addDebugFunction ((ShellCmdFunction) (&SoftwareManagementVersionWorker::debugGetVersion), "getversion");

    addOperationMap (SOFTWAREMANAGEMENT_GET_VERSION, reinterpret_cast<PrismMessageHandler> (&SoftwareManagementVersionWorker::softwareManagementGetVersionMessageHandler));
}

SoftwareManagementVersionWorker::~SoftwareManagementVersionWorker ()
{
}

PrismMessage *SoftwareManagementVersionWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case SOFTWAREMANAGEMENT_GET_VERSION :
            pPrismMessage = new SoftwareManagementGetVersionMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void SoftwareManagementVersionWorker::softwareManagementGetVersionMessageHandler (SoftwareManagementGetVersionMessage *pSoftwareManagementGetVersionMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&SoftwareManagementVersionWorker::getVersionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&SoftwareManagementVersionWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&SoftwareManagementVersionWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSoftwareManagementGetVersionMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void SoftwareManagementVersionWorker::getVersionStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "SoftwareManagementVersionWorker::getVersionStep : Entering ...");

          SoftwareManagementGetVersionMessage *pSoftwareManagementGetVersionMessage = reinterpret_cast<SoftwareManagementGetVersionMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    const PrismVersion                        *pPrismVersion                        = PrismVersion::getInstance ();
    const string                               version                              = pPrismVersion->getMajor () + "." + pPrismVersion->getMinor () + "." + pPrismVersion->getPatch () + "." + pPrismVersion->getRevision () + "-" + pPrismVersion->getBuildType () + "." + pPrismVersion->getBuildNumber ();

    pSoftwareManagementGetVersionMessage->setVersion (version);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void SoftwareManagementVersionWorker::debugGetVersion (UI32 argc, vector<string> argv)
{
    string serverIpAddress              = FrameworkToolKit::getAPublicIpAddressForThisMachine ();
    UI32   serverPort                   = 9111;
    char   serverIpCharArray[64] = {0};
    UI32   i;

    for (i = 0; i < argc; i += 1)
    {
        if (0 == strcasecmp ("-serverip", argv[i].c_str ()))
        {
            if (argc > (i + 1))
            {
                strncpy (serverIpCharArray, argv[i + 1].c_str (), 63);
                serverIpAddress = serverIpCharArray;
                i++;
            }
        }
        else if (0 == strcasecmp ("-serverport", argv[i].c_str ()))
        {
            if (argc > (i + 1))
            {
                serverPort = atoi (argv[i + 1].c_str ());
                i++;
            }
        }
    }

    WaveNs::trace (TRACE_LEVEL_DEBUG, "SoftwareManagementVersionWorker::debugGetVersion : Server IP Address : " + serverIpAddress + string (", Port : ") + serverPort);

    WaveClientSynchronousConnection connection ("Wave Debug Client", serverIpAddress, serverPort);

    WaveConnectionStatus connectionstatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;
    string               version;

    connectionstatus = connection.connect ();

    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER == connectionstatus)
    {
        ResourceId status = connection.getVersion (version);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("SoftwareManagementVersionWorker::debugGetVersion : IP Address : \"") + serverIpAddress + string ("\""));
            WaveNs::trace (TRACE_LEVEL_INFO, string ("SoftwareManagementVersionWorker::debugGetVersion : Port       : \"") + serverPort + string ("\""));
            WaveNs::trace (TRACE_LEVEL_INFO, string ("SoftwareManagementVersionWorker::debugGetVersion : Version    : \"") + version + string ("\""));
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "SoftwareManagementVersionWorker::debugGetVersion : Failed to obtain version from the server.  Status : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "Could not establish connection to Wave Server");
    }

    connection.close ();
}

}
