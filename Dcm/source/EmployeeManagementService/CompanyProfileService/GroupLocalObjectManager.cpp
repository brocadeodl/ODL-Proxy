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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 **************************************************************************/

#include "EmployeeManagementService/CompanyProfileService/GroupLocalObjectManager.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagementObjectManager.h"
#include "EmployeeManagementService/CompanyProfileService/SubGroupLocalManagedObject.h"

namespace DcmNs
{

GroupLocalObjectManager::GroupLocalObjectManager ()
    : WaveLocalObjectManager  (getClassName ())
{
    associateWithVirtualWaveObjectManager (CompanyManagementObjectManager::getInstance ());

    SubGroupLocalManagedObject subGroupLocalManagedObject (this);
    subGroupLocalManagedObject.setupOrm ();
    addManagedClass (SubGroupLocalManagedObject::getClassName ());
}

GroupLocalObjectManager::~GroupLocalObjectManager ()
{
}

GroupLocalObjectManager  *GroupLocalObjectManager::getInstance ()
{
    static GroupLocalObjectManager *pGroupLocalObjectManager = new GroupLocalObjectManager ();

    WaveNs::prismAssert (NULL != pGroupLocalObjectManager, __FILE__, __LINE__);

    return (pGroupLocalObjectManager);
}

string  GroupLocalObjectManager::getClassName ()
{
    return ("GroupLocal");
}

PrismServiceId  GroupLocalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage  *GroupLocalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
         default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject  *GroupLocalObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if (SubGroupLocalManagedObject::getClassName () == managedClassName)
    {
        pWaveManagedObject = new SubGroupLocalManagedObject (this);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "GroupLocalObjectManager::createManagedObjectInstanc Unsupported Class Name " + managedClassName);
    }

    return pWaveManagedObject;
}

}
