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
 *   Author : sholla                                                     *
 **************************************************************************/


#include "Udld/Global/UdldGlobalObjectManager.h"
#include "Udld/Global/UdldGlobalTypes.h"
#include "Udld/Global/UdldGlobalConfigManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


UdldGlobalObjectManager::UdldGlobalObjectManager()
    :WaveObjectManager (UdldGlobalObjectManager::getClassName())

{
    UdldGlobalConfigManagedObject instanceUdldGlobalConfigManagedObject (this);
    addManagedClass (UdldGlobalConfigManagedObject::getClassName(), this);
    instanceUdldGlobalConfigManagedObject.setupOrm ();
    setPostbootPhases ();
}

UdldGlobalObjectManager::~UdldGlobalObjectManager()
{
}

UdldGlobalObjectManager *UdldGlobalObjectManager::getInstance()
{
    static UdldGlobalObjectManager *s_pUdldGlobalObjectManager = new UdldGlobalObjectManager ();
    WaveNs::prismAssert (NULL != s_pUdldGlobalObjectManager, __FILE__, __LINE__);
    return (s_pUdldGlobalObjectManager);
}

string UdldGlobalObjectManager::getClassName()
{
    return ("UdldGlobalObjectManager");
}

PrismServiceId  UdldGlobalObjectManager::getPrismServiceId()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *UdldGlobalObjectManager:: createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    switch (operationCode)
    {
        default :
                pPrismMessage = NULL;
    }
    return (pPrismMessage);
}

WaveManagedObject *UdldGlobalObjectManager:: createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL ;
    if ( "" == managedClassName )
    {
    }
    else if ((UdldGlobalConfigManagedObject::getClassName()) == managedClassName )
    {
        pWaveManagedObject = new UdldGlobalConfigManagedObject (this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL,"UdldGlobalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
}
    return (pWaveManagedObject);
    }

void UdldGlobalObjectManager::setPostbootPhases ()
{
    map<string, vector<string> > postbootManagedObjectNames;
    {
        string managedObject = string ("UdldGlobalConfigManagedObject");
        vector<string> managedObjectNames;
        managedObjectNames = postbootManagedObjectNames["DCM_POSTBOOT_GLOBAL_STAGE2"];
        managedObjectNames.push_back(managedObject);
        postbootManagedObjectNames["DCM_POSTBOOT_GLOBAL_STAGE2"] =  managedObjectNames;
    }
    setPostbootMap( postbootManagedObjectNames );
}



}

