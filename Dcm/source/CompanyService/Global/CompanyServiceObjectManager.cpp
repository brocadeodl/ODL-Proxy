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
#include "CompanyService/Global/CompanyServiceTypes.h"
#include "CompanyService/Global/CompanyServicecompanyWorker.h"


using namespace WaveNs;


namespace DcmNs{


CompanyServiceObjectManager::CompanyServiceObjectManager()
    :WaveObjectManager (CompanyServiceObjectManager::getClassName())

{
    m_pCompanyServicecompanyWorker =  new CompanyServicecompanyWorker(this);
    prismAssert (NULL != m_pCompanyServicecompanyWorker, __FILE__, __LINE__);
    setPostbootPhases ();
}

CompanyServiceObjectManager::~CompanyServiceObjectManager()
{
}

CompanyServiceObjectManager *CompanyServiceObjectManager::getInstance()
{
    static CompanyServiceObjectManager *s_pCompanyServiceObjectManager = new CompanyServiceObjectManager ();
    WaveNs::prismAssert (NULL != s_pCompanyServiceObjectManager, __FILE__, __LINE__);
    return (s_pCompanyServiceObjectManager);
}

string CompanyServiceObjectManager::getClassName()
{
    return ("CompanyServiceObjectManager");
}

PrismServiceId  CompanyServiceObjectManager::getPrismServiceId()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *CompanyServiceObjectManager:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *CompanyServiceObjectManager:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL ;
    if ( "" == managedClassName )
    {
    }
    else
    {
        trace (TRACE_LEVEL_FATAL,"CompanyServiceObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
}
    return (pWaveManagedObject);
    }

void CompanyServiceObjectManager::setPostbootPhases ()
{
    map<string, vector<string> > postbootManagedObjectNames;
    setPostbootMap( postbootManagedObjectNames );
}



}

