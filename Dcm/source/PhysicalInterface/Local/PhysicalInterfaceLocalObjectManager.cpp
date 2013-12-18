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

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"

#include "PhysicalInterface/Local/PhysicalInterfaceLocalObjectManager.h"
#include "PhysicalInterface/Local/PhysicalInterfaceLocalManagedObject.h"
#include "PhysicalInterface/Local/PhysicalInterfaceMessages.h"
#include "PhysicalInterface/Local/PhysicalInterfaceLocalTypes.h"


namespace DcmNs
{

PhysicalInterfaceLocalObjectManager::PhysicalInterfaceLocalObjectManager ()
    : WaveLocalObjectManager (getClassName ())
{
    PhysicalInterfaceLocalManagedObject physicalInterfaceLocalManagedObject (this);

    physicalInterfaceLocalManagedObject.setupOrm ();
    addManagedClass (PhysicalInterfaceLocalManagedObject::getClassName ());

    trace (TRACE_LEVEL_INFO, "PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : Setting IP Address to " + physicalInterfaceLocalManagedObject.getClassName() + physicalInterfaceLocalManagedObject.getObjectClassName() + physicalInterfaceLocalManagedObject.getIfIndex() + physicalInterfaceLocalManagedObject.getMtu() + physicalInterfaceLocalManagedObject.getDescription() );
    
    addOperationMap (PHYSICAL_INTERFACE_LOCAL_CREATE, reinterpret_cast<PrismMessageHandler> (&PhysicalInterfaceLocalObjectManager::createPhysicalInterfaceMessageHandler));
    addOperationMap (PHYSICAL_INTERFACE_LOCAL_UPDATE, reinterpret_cast<PrismMessageHandler> (&PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler));
}

PhysicalInterfaceLocalObjectManager::~PhysicalInterfaceLocalObjectManager ()
{
}

PhysicalInterfaceLocalObjectManager *PhysicalInterfaceLocalObjectManager::getInstance ()
{
    static PhysicalInterfaceLocalObjectManager *pPhysicalInterfaceLocalObjectManager = new PhysicalInterfaceLocalObjectManager ();

    WaveNs::prismAssert (NULL != pPhysicalInterfaceLocalObjectManager, __FILE__, __LINE__);

    return (pPhysicalInterfaceLocalObjectManager);
}

PrismServiceId PhysicalInterfaceLocalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *PhysicalInterfaceLocalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case PHYSICAL_INTERFACE_LOCAL_CREATE :
            pPrismMessage = new PhysicalInterfaceCreateMessage ();
            break;
        case PHYSICAL_INTERFACE_LOCAL_UPDATE:
            printf("============================ \n");
            pPrismMessage = new PhysicalInterfaceUpdateMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler (PhysicalInterfaceUpdateMessage *pPhysicalInterfaceUpdateMessage)
{
    ObjectId            objectId   = pPhysicalInterfaceUpdateMessage->getInterfaceObjectId ();
    string name        = pPhysicalInterfaceUpdateMessage->getName ();
    UI32   ifIndex     = pPhysicalInterfaceUpdateMessage->getIfIndex ();
    UI32   mtu         = pPhysicalInterfaceUpdateMessage->getMtu ();
    string description = pPhysicalInterfaceUpdateMessage->getDescription ();

    PhysicalInterfaceLocalManagedObject          *pPhy_Int_LocManObj          = NULL;
    WaveManagedObject *pWaveManagedObject = NULL;
    ResourceId          status              = WAVE_MESSAGE_SUCCESS;

    pWaveManagedObject = queryManagedObject (objectId);

    /*
    vector<WaveManagedObject *> *pPhy_IntLocManObjs = NULL;

    pPhy_IntLocManObjs = querySynchronously (PhysicalInterfaceLocalManagedObject::getClassName (),name);

    if (NULL != pPhy_IntLocManObjs)
    {
        UI32 numberOfInterfaces = pPhy_IntLocManObjs->size ();

        prismAssert (numberOfInterfaces <= 1, __FILE__, __LINE__);

        if (1 == numberOfInterfaces)
        {
            pWaveManagedObject = ((*pPhy_IntLocManObjs)[0]);
            pPhy_Int_LocManObj = dynamic_cast<PhysicalInterfaceLocalManagedObject *> ((*pPhy_IntLocManObjs)[0]);
        }
        pPhy_IntLocManObjs->clear ();

        delete pPhy_IntLocManObjs;
    }

    trace (TRACE_LEVEL_ERROR, "PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : No Interface Found with the given ObjectId. 222 ");
    
    */
    
    if (NULL == pWaveManagedObject)
    {
        trace (TRACE_LEVEL_ERROR, "PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : No Interface Found with the given ObjectId.");

        pPhysicalInterfaceUpdateMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pPhysicalInterfaceUpdateMessage);

        return;
    }
    
    
    pPhy_Int_LocManObj = dynamic_cast<PhysicalInterfaceLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pPhy_Int_LocManObj, __FILE__, __LINE__);

    startTransaction ();


    trace (TRACE_LEVEL_INFO, string("PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : Setting te interface ...") + pPhy_Int_LocManObj->getClassName() + pPhy_Int_LocManObj->getObjectClassName() + (pPhy_Int_LocManObj->getIfIndex())  + (pPhy_Int_LocManObj->getMtu()) + pPhy_Int_LocManObj->getDescription() );
    
    updateWaveManagedObject (pPhy_Int_LocManObj);
 
    pPhysicalInterfaceUpdateMessage->setCmdCode(UNKNOWN_CMD);
    
    if ("" != description)
    {
        trace (TRACE_LEVEL_INFO, "PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : Setting IP Address to " + description);

        pPhy_Int_LocManObj->setDescription (description);
        pPhysicalInterfaceUpdateMessage->setCmdCode(PHYSICALINTERFACE_SET_DESC);
    }

    if (0 != ifIndex)
    {
        trace (TRACE_LEVEL_INFO, string ("PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : Setting MTU to ") + mtu);

        pPhy_Int_LocManObj->setIfIndex (ifIndex);
    }

    if (0 != mtu)
    {
        trace (TRACE_LEVEL_INFO, string ("PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : Setting MTU to ") + mtu);

        pPhy_Int_LocManObj->setMtu (mtu);
        pPhysicalInterfaceUpdateMessage->setCmdCode(PHYSICALINTERFACE_SET_MTU);
    }

    /* Sending Msg to Backend nsm Client */
    trace (TRACE_LEVEL_INFO, string ("PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler: Sending Message to Backend .."));


    PhysicalInterfaceUpdateMessage pMsg;
    pMsg.setCmdCode(UNKNOWN_CMD);
    /*
    status = sendSynchronouslyToWaveClient ("nsm", &pMsg);
    */

    trace (TRACE_LEVEL_INFO, string ("PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler: Sending Message completed to Backend "));

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        status = pPhysicalInterfaceUpdateMessage->getCompletionStatus ();
        trace (TRACE_LEVEL_INFO, string ("PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : Success "));
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string ("PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : Failed "));

        pPhysicalInterfaceUpdateMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pPhysicalInterfaceUpdateMessage);

        return;
    }

    /* Commit Transaction */
    status = commitTransaction ();
       
    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : Could not create a Physical Interface in DB. IF Name : " + name);
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;

        trace (TRACE_LEVEL_INFO, "PhysicalInterfaceLocalObjectManager::updatePhysicalInterfaceMessageHandler : Successfully created a Physical Interface in DB. IF Name : " + name);
    }

    delete pPhy_Int_LocManObj;

    pPhysicalInterfaceUpdateMessage->setCompletionStatus (status);
    reply (pPhysicalInterfaceUpdateMessage);
}

void PhysicalInterfaceLocalObjectManager::createPhysicalInterfaceMessageHandler (PhysicalInterfaceCreateMessage *pPhysicalInterfaceCreateMessage)
{
    string name        = pPhysicalInterfaceCreateMessage->getName ();
    UI32   ifIndex     = pPhysicalInterfaceCreateMessage->getIfIndex ();
    UI32   mtu         = pPhysicalInterfaceCreateMessage->getMtu ();
    string description = pPhysicalInterfaceCreateMessage->getDescription ();

    startTransaction ();

    PhysicalInterfaceLocalManagedObject *pPhysicalInterfaceLocalManagedObject = new PhysicalInterfaceLocalManagedObject (this, name, ifIndex, mtu, description);

    pPhysicalInterfaceLocalManagedObject->setName (name);

    ResourceId status = commitTransaction ();

    if (FRAMEWORK_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PhysicalInterfaceLocalObjectManager::createPhysicalInterfaceMessageHandler : Could not create a Physical Interface in DB. IF Name : " + name);
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;

        trace (TRACE_LEVEL_INFO, "PhysicalInterfaceLocalObjectManager::createPhysicalInterfaceMessageHandler : Successfully created a Physical Interface in DB. IF Name : " + name);
    }

    delete pPhysicalInterfaceLocalManagedObject;

    pPhysicalInterfaceCreateMessage->setCompletionStatus (status);
    reply (pPhysicalInterfaceCreateMessage);
}

WaveManagedObject *PhysicalInterfaceLocalObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((PhysicalInterfaceLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new PhysicalInterfaceLocalManagedObject(this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "PhysicalInterfaceLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }

    return (pWaveManagedObject);
}

}
