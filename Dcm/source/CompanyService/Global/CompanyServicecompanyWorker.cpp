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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : apurva                                                     *
 **************************************************************************/


#include "CompanyService/Global/CompanyServiceObjectManager.h"
#include "CompanyService/Global/CompanyServicecompanyWorker.h"
#include "CompanyService/Global/CompanyServiceCityManagedObject.h"
#include "CompanyService/Global/CompanyServiceCompanyManagedObject.h"
#include "CompanyService/Global/CompanyServiceBoardOfDirectorManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


CompanyServicecompanyWorker::CompanyServicecompanyWorker(CompanyServiceObjectManager *pCompanyServiceObjectManager)
    :WaveWorker  (pCompanyServiceObjectManager )

{
    CompanyServiceCityManagedObject instanceCompanyServiceCityManagedObject (pCompanyServiceObjectManager);
    addManagedClass (CompanyServiceCityManagedObject::getClassName(), this);
    instanceCompanyServiceCityManagedObject.setupOrm ();
    CompanyServiceCompanyManagedObject instanceCompanyServiceCompanyManagedObject (pCompanyServiceObjectManager);
    addManagedClass (CompanyServiceCompanyManagedObject::getClassName(), this);
    instanceCompanyServiceCompanyManagedObject.setupOrm ();
    CompanyServiceBoardOfDirectorManagedObject instanceCompanyServiceBoardOfDirectorManagedObject (pCompanyServiceObjectManager);
    addManagedClass (CompanyServiceBoardOfDirectorManagedObject::getClassName(), this);
    instanceCompanyServiceBoardOfDirectorManagedObject.setupOrm ();
}

CompanyServicecompanyWorker::~CompanyServicecompanyWorker()
{
}

PrismMessage *CompanyServicecompanyWorker:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *CompanyServicecompanyWorker:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if ((CompanyServiceCityManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new CompanyServiceCityManagedObject (dynamic_cast<CompanyServiceObjectManager *>(getPWaveObjectManager()));
    }
    else if ((CompanyServiceCompanyManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new CompanyServiceCompanyManagedObject (dynamic_cast<CompanyServiceObjectManager *>(getPWaveObjectManager()));
    }
    else if ((CompanyServiceBoardOfDirectorManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new CompanyServiceBoardOfDirectorManagedObject (dynamic_cast<CompanyServiceObjectManager *>(getPWaveObjectManager()));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL,"CompanyServicecompanyWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }
    return (pWaveManagedObject);
}



}

