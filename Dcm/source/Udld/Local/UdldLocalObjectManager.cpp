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
 *   Author : statpatt                                                     *
 **************************************************************************/


#include "Udld/Local/UdldLocalObjectManager.h"
#include "Udld/Local/UdldLocalTypes.h"
#include "Udld/Global/UdldGlobalObjectManager.h"


using namespace WaveNs;


namespace DcmNs{


UdldLocalObjectManager::UdldLocalObjectManager()
    :WaveLocalObjectManager (UdldLocalObjectManager::getClassName())

{
    associateWithVirtualWaveObjectManager (UdldGlobalObjectManager::getInstance ());

    setPostbootPhases ();
}

UdldLocalObjectManager::~UdldLocalObjectManager()
{
}

UdldLocalObjectManager *UdldLocalObjectManager::getInstance()
{
    static UdldLocalObjectManager *s_pUdldLocalObjectManager = new UdldLocalObjectManager ();
    WaveNs::prismAssert (NULL != s_pUdldLocalObjectManager, __FILE__, __LINE__);
    return (s_pUdldLocalObjectManager);
}

string UdldLocalObjectManager::getClassName()
{
    return ("UdldLocalObjectManager");
}

PrismServiceId  UdldLocalObjectManager::getPrismServiceId()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *UdldLocalObjectManager:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *UdldLocalObjectManager:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if ( "" == managedClassName )
    {
    }
    else
    {
        trace (TRACE_LEVEL_FATAL,"UdldLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }
    return(pWaveManagedObject);
}

void UdldLocalObjectManager::setPostbootPhases ()
{
    map<string, vector<string> > postbootManagedObjectNames;
    setPostbootMap( postbootManagedObjectNames );
}



}

