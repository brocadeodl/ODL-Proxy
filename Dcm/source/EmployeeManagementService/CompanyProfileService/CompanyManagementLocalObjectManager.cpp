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

#include "EmployeeManagementService/CompanyProfileService/CompanyManagementLocalObjectManager.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Messaging/Local/PrismEvent.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{
         
CompanyManagementLocalObjectManager::CompanyManagementLocalObjectManager ()
    : WaveLocalObjectManager (getClassName ())
{
}

CompanyManagementLocalObjectManager::~CompanyManagementLocalObjectManager ()
{
}

string CompanyManagementLocalObjectManager::getClassName ()
{
    return ("CompanyManagementLocalObjectManager");
}

CompanyManagementLocalObjectManager *CompanyManagementLocalObjectManager::getInstance ()
{
    static CompanyManagementLocalObjectManager *pCompanyManagementLocalObjectManager= new CompanyManagementLocalObjectManager();
 
    WaveNs::prismAssert (NULL != pCompanyManagementLocalObjectManager, __FILE__, __LINE__);
 
    return (pCompanyManagementLocalObjectManager);    
}

PrismServiceId CompanyManagementLocalObjectManager::getPrismServiceId ()
{
            return ((getInstance ())->getServiceId ());
}

}
