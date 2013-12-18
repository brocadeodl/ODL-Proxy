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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <iostream>
#include "Framework/Core/Wave.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/Fsc.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"

#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"

using namespace WaveNs;
using namespace DcmNs;

extern "C" int prismMain (int argc, char *argv[]);

extern "C" int fscMain (int argc, char *argv[], int argc1, char *argv1[])
{
    string bpBackPlaneIpAddress  = argv1[0];
    string cpBackPlaneIpAddress  = argv1[1];
    string wyserAlternateExternalApplicationIpAddress        = argv1[2];
    SI32   wyserAlternateExternalApplicationPort             = atoi (argv1[3]);
    string vcsConfPath           =  argv1[4];
    string ipotEnabled			 = argv1[5];
    string fcPrincipalEnabled    = argv1[6];
    string useFileService    	 = argv1[7];
    string distributionMode      = argv1[8];
    UI32   swBDModel             = atoi(argv1[9]);

    UI32   mgmtPort              = atoi (argv [7]);

    if ("" != wyserAlternateExternalApplicationIpAddress)
    {
        wyserAlternateExternalApplicationIpAddress = "127.0.0.1";
    }

    if (wyserAlternateExternalApplicationPort <= 0)
    {
        wyserAlternateExternalApplicationPort = 4567;
    }

    if ("" != bpBackPlaneIpAddress && "" != cpBackPlaneIpAddress)
    {
        DcmToolKit::setBackPlaneIpAddresses (bpBackPlaneIpAddress, cpBackPlaneIpAddress);
    }

    if ("" == vcsConfPath)
    {
        vcsConfPath = ".";
    }

    DcmToolKit::setWyserAlternateExternalApplicationIpAddress           (wyserAlternateExternalApplicationIpAddress);
    DcmToolKit::setWyserAlternateExternalApplicationPort                (wyserAlternateExternalApplicationPort);
    DcmToolKit::setMgmtPort                 (mgmtPort);
    DcmToolKit::setVcsConfPath              (vcsConfPath);
    DcmToolKit::setIpotEnabledFlag          (ipotEnabled);
    DcmToolKit::setEnableFCPrincipal        (fcPrincipalEnabled);
    DcmToolKit::setFileServiceUsage         (useFileService);
    DcmToolKit::setDistributionMode         (distributionMode);
    DcmToolKit::setSwBDModel                (swBDModel);

    VcsFabricLocalObjectManager::setThisVcsNodeSWBDModel (swBDModel);

    initializeDcmResourceIds ();

    Fsc::initialize ();

    return (prismMain (argc, argv));
}

