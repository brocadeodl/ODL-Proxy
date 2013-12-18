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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "License/LicenseObjectManager.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

LicenseObjectManager::LicenseObjectManager ()
    : WaveObjectManager ("Licnese")
{
}

LicenseObjectManager *LicenseObjectManager::getInstance ()
{
    static LicenseObjectManager *pLicenseObjectManager = NULL;

    if (NULL == pLicenseObjectManager)
    {
        pLicenseObjectManager = new LicenseObjectManager ();
        WaveNs::prismAssert (NULL != pLicenseObjectManager, __FILE__, __LINE__);
    }

    return (pLicenseObjectManager);
}

LicenseObjectManager::~LicenseObjectManager ()
{
}

PrismServiceId LicenseObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

}
