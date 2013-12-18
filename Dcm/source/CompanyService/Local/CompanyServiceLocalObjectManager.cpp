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
#include "CompanyService/Local/CompanyServiceLocalTypes.h"
#include "CompanyService/Global/CompanyServiceObjectManager.h"
#include "CompanyService/Local/CompanyServiceLocalcompanyWorker.h"
#include "CompanyService/Local/CompanyServiceLocalEmployeeManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalGroupManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalSubgroupManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalStockOptionManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalGroupmanagerManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalSubgroupmanagerManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


CompanyServiceLocalObjectManager::CompanyServiceLocalObjectManager()
    :WaveLocalObjectManager (CompanyServiceLocalObjectManager::getClassName())

{
    associateWithVirtualWaveObjectManager (CompanyServiceObjectManager::getInstance ());

    m_pCompanyServiceLocalcompanyWorker =  new CompanyServiceLocalcompanyWorker(this);
    prismAssert (NULL != m_pCompanyServiceLocalcompanyWorker, __FILE__, __LINE__);
    setPostbootPhases ();
}

CompanyServiceLocalObjectManager::~CompanyServiceLocalObjectManager()
{
}

CompanyServiceLocalObjectManager *CompanyServiceLocalObjectManager::getInstance()
{
    static CompanyServiceLocalObjectManager *s_pCompanyServiceLocalObjectManager = new CompanyServiceLocalObjectManager ();
    WaveNs::prismAssert (NULL != s_pCompanyServiceLocalObjectManager, __FILE__, __LINE__);
    return (s_pCompanyServiceLocalObjectManager);
}

string CompanyServiceLocalObjectManager::getClassName()
{
    return ("CompanyServiceLocalObjectManager");
}

PrismServiceId  CompanyServiceLocalObjectManager::getPrismServiceId()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *CompanyServiceLocalObjectManager:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *CompanyServiceLocalObjectManager:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if ( "" == managedClassName )
    {
    }
    else
    {
        trace (TRACE_LEVEL_FATAL,"CompanyServiceLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }
    return(pWaveManagedObject);
}

void CompanyServiceLocalObjectManager::setPostbootPhases ()
{
    map<string, vector<string> > postbootManagedObjectNames;
    setPostbootMap( postbootManagedObjectNames );
}



}

