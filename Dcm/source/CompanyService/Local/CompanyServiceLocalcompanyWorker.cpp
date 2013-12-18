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


#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService/Local/CompanyServiceLocalcompanyWorker.h"
#include "CompanyService/Local/CompanyServiceLocalEmployeeManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalGroupManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalSubgroupManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalStockOptionManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalGroupmanagerManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalSubgroupmanagerManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


CompanyServiceLocalcompanyWorker::CompanyServiceLocalcompanyWorker(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager)
    :WaveWorker  (pCompanyServiceLocalObjectManager )

{
    CompanyServiceLocalEmployeeManagedObject instanceCompanyServiceLocalEmployeeManagedObject (pCompanyServiceLocalObjectManager);
    addManagedClass (CompanyServiceLocalEmployeeManagedObject::getClassName(), this);
    instanceCompanyServiceLocalEmployeeManagedObject.setupOrm ();
    CompanyServiceLocalGroupManagedObject instanceCompanyServiceLocalGroupManagedObject (pCompanyServiceLocalObjectManager);
    addManagedClass (CompanyServiceLocalGroupManagedObject::getClassName(), this);
    instanceCompanyServiceLocalGroupManagedObject.setupOrm ();
    CompanyServiceLocalSubgroupManagedObject instanceCompanyServiceLocalSubgroupManagedObject (pCompanyServiceLocalObjectManager);
    addManagedClass (CompanyServiceLocalSubgroupManagedObject::getClassName(), this);
    instanceCompanyServiceLocalSubgroupManagedObject.setupOrm ();
    CompanyServiceLocalStockOptionManagedObject instanceCompanyServiceLocalStockOptionManagedObject (pCompanyServiceLocalObjectManager);
    addManagedClass (CompanyServiceLocalStockOptionManagedObject::getClassName(), this);
    instanceCompanyServiceLocalStockOptionManagedObject.setupOrm ();
    CompanyServiceLocalGroupmanagerManagedObject instanceCompanyServiceLocalGroupmanagerManagedObject (pCompanyServiceLocalObjectManager);
    addManagedClass (CompanyServiceLocalGroupmanagerManagedObject::getClassName(), this);
    instanceCompanyServiceLocalGroupmanagerManagedObject.setupOrm ();
    CompanyServiceLocalSubgroupmanagerManagedObject instanceCompanyServiceLocalSubgroupmanagerManagedObject (pCompanyServiceLocalObjectManager);
    addManagedClass (CompanyServiceLocalSubgroupmanagerManagedObject::getClassName(), this);
    instanceCompanyServiceLocalSubgroupmanagerManagedObject.setupOrm ();
}

CompanyServiceLocalcompanyWorker::~CompanyServiceLocalcompanyWorker()
{
}

PrismMessage *CompanyServiceLocalcompanyWorker:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *CompanyServiceLocalcompanyWorker:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if ((CompanyServiceLocalEmployeeManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new CompanyServiceLocalEmployeeManagedObject (dynamic_cast<CompanyServiceLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((CompanyServiceLocalGroupManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new CompanyServiceLocalGroupManagedObject (dynamic_cast<CompanyServiceLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((CompanyServiceLocalSubgroupManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new CompanyServiceLocalSubgroupManagedObject (dynamic_cast<CompanyServiceLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((CompanyServiceLocalStockOptionManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new CompanyServiceLocalStockOptionManagedObject (dynamic_cast<CompanyServiceLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((CompanyServiceLocalGroupmanagerManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new CompanyServiceLocalGroupmanagerManagedObject (dynamic_cast<CompanyServiceLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((CompanyServiceLocalSubgroupmanagerManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new CompanyServiceLocalSubgroupmanagerManagedObject (dynamic_cast<CompanyServiceLocalObjectManager *>(getPWaveObjectManager()));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL,"CompanyServiceLocalcompanyWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }
    return (pWaveManagedObject);
}



}

