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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka, Jayanth Venkataraman              *
 ***************************************************************************/

#include <iostream>
#include "Framework/Core/WaveSystemManagement.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Core/WaveMainConfiguration.h"
#include "SystemManagement/SystemManagementToolKit.h"
#include "Framework/Database/MultiDatabaseToolKit.h"
#include "DcmCore/Dcm.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIds.h"

using namespace WaveNs;
using namespace DcmNs;

namespace WaveNs
{
}

extern "C" int waveSystemManagementMain (int argc, char *argv[]);

extern "C" int networkOsWaveSystemManagementMain (int argc, char *argv[], int argcNosSmdArguments, char *argvNosSmdArguments[])
{
    DcmToolKit::setDistributionMode ("DCM_SMD");
    DcmToolKit::setSwBDModel (atoi(argvNosSmdArguments[9]));

    Dcm::initializeForWsm ();

	initializeDcmResourceIds ();

    // Get the WSYER Alternal External Application port from the command line arguments and then setWyserAlternateExternalApplicationPort
    SI32 wyserAlternateExternalApplicationPort = atoi(argvNosSmdArguments[3]);
    DcmToolKit::setWyserAlternateExternalApplicationPort (wyserAlternateExternalApplicationPort);
    
    return (waveSystemManagementMain (argc, argv));

    return EXIT_SUCCESS;
}
