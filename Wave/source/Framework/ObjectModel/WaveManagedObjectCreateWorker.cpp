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
 ***************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectCreateWorker.h"
#include "Framework/ObjectModel/WaveManagedObjectCreateContext.h"
#include "Framework/Types/Types.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCreateMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectAsynchronousCreateContext.h"

namespace WaveNs
{

WaveManagedObjectCreateWorker::WaveManagedObjectCreateWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (WAVE_OBJECT_MANAGER_CREATE_MANAGED_OBJECT, reinterpret_cast<PrismMessageHandler> (&WaveManagedObjectCreateWorker::createHandler));
}

WaveManagedObjectCreateWorker::~WaveManagedObjectCreateWorker ()
{
}

PrismMessage *WaveManagedObjectCreateWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case WAVE_OBJECT_MANAGER_CREATE_MANAGED_OBJECT :
            pPrismMessage = new WaveObjectManagerCreateWaveManagedObjectMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void WaveManagedObjectCreateWorker::createHandler (WaveObjectManagerCreateWaveManagedObjectMessage *pWaveObjectManagerCreateWaveManagedObjectMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createQueryManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createValidateManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::WaveManagedObjectStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createPreUpdateManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createUpdateManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createUpdateNewAttributeInManagedObject),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createPostUpdateManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::commitWaveManagedObjectBeforeUpdatingBackend),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createPreUpdateHardwareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createUpdateHardwareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createUpdateHardwareStepForChild),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createPostUpdateHardwareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createAttributesPostUpdateHardwareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::commitWaveManagedObjectAfterUpdatingBackend),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::prismLinearSequencerFailedStep),
    };

    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = new WaveManagedObjectCreateContext (pWaveObjectManagerCreateWaveManagedObjectMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pWaveManagedObjectCreateContext->setOperateOnWaveManagedObjectId            (pWaveObjectManagerCreateWaveManagedObjectMessage->getOperateOnWaveManagedObjectId ());
    pWaveManagedObjectCreateContext->setAttributeUserTags                       (pWaveObjectManagerCreateWaveManagedObjectMessage->getAttributeUserTags   ());
    pWaveManagedObjectCreateContext->setAttributeNames                          (pWaveObjectManagerCreateWaveManagedObjectMessage->getAttributeNames      ());
    pWaveManagedObjectCreateContext->setAttributeValues                         (pWaveObjectManagerCreateWaveManagedObjectMessage->getAttributeValues     ());
    pWaveManagedObjectCreateContext->setManagedObjectClassNameNeedToBeCreated   (pWaveObjectManagerCreateWaveManagedObjectMessage->getManagedObjectClassNameNeedToBeCreated ());
    pWaveManagedObjectCreateContext->setParentObjectId                          (pWaveObjectManagerCreateWaveManagedObjectMessage->getParentObjectId ());
    pWaveManagedObjectCreateContext->setChildUserTag                            (pWaveObjectManagerCreateWaveManagedObjectMessage->getChildUserTag ());
    pWaveManagedObjectCreateContext->setConfigReplayInProgressFlag              (pWaveObjectManagerCreateWaveManagedObjectMessage->getConfigReplayInProgressFlag ());


    pWaveManagedObjectCreateContext->holdAll ();
    pWaveManagedObjectCreateContext->start   ();

}

void WaveManagedObjectCreateWorker::asynchronousCreateHandler (WaveManagedObjectAsynchronousCreateContext *pWaveManagedObjectAsynchronousCreateContext)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createQueryManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createValidateManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::WaveManagedObjectStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createPreUpdateManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createUpdateManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createUpdateNewAttributeInManagedObject),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createPostUpdateManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::commitWaveManagedObjectBeforeUpdatingBackend),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createPreUpdateHardwareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createUpdateHardwareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createUpdateHardwareStepForChild),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::createPostUpdateHardwareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::commitWaveManagedObjectAfterUpdatingBackend),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveManagedObjectCreateWorker::prismLinearSequencerFailedStep),
    };

    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = new WaveManagedObjectCreateContext (pWaveManagedObjectAsynchronousCreateContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pWaveManagedObjectCreateContext->setAttributeNames                          (pWaveManagedObjectAsynchronousCreateContext->getAttributeNames      ());
    pWaveManagedObjectCreateContext->setAttributeValues                         (pWaveManagedObjectAsynchronousCreateContext->getAttributeValues     ());
    pWaveManagedObjectCreateContext->setManagedObjectClassNameNeedToBeCreated   (pWaveManagedObjectAsynchronousCreateContext->getManagedObjectClassNameNeedToBeCreated ());

    pWaveManagedObjectCreateContext->holdAll ();
    pWaveManagedObjectCreateContext->start   ();
}

void WaveManagedObjectCreateWorker::createQueryManagedObjectStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createQueryManagedObjectStep : Entering ..");

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    string     className;
    ObjectId   operateOnWaveManagedObjectId = pWaveManagedObjectCreateContext->getOperateOnWaveManagedObjectId ();
    bool       isALocalService = FrameworkToolKit::isALocalService(getServiceId());
    bool       isALocalMO      = false;

    trace (TRACE_LEVEL_INFO, string ("WaveManagedObjectCreateWorker::createQueryManagedObjectStep : For service =  ") + FrameworkToolKit::getServiceNameById(getServiceId()));

    if (0 != operateOnWaveManagedObjectId.getClassId ())
    {
        className  = OrmRepository::getTableNameById (operateOnWaveManagedObjectId.getClassId ());
        isALocalMO = OrmRepository::isALocalManagedObject(className); 

        trace (TRACE_LEVEL_INFO, string ("WaveManagedObjectCreateWorker::createQueryManagedObjectStep : For ManagedObject = ")+className);
    }
	else if(isALocalService)
	{
		isALocalMO = true;
	}
  
    if (!isALocalService && !isALocalMO)
    {
        // Global Service & Global MO    
        pWaveManagedObjectCreateContext->setIsDatabaseUpdateRequired (true);
        pWaveManagedObjectCreateContext->setIsBackendUpdateRequired (false);        

        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createQueryManagedObjectStep : Global Service & Global MO");
    }  
    else if(isALocalService && isALocalMO)
    {
        // Local Service Local MO    
        pWaveManagedObjectCreateContext->setIsDatabaseUpdateRequired (true);
        pWaveManagedObjectCreateContext->setIsBackendUpdateRequired (true);        

        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createQueryManagedObjectStep : Local Service Local MO");
    }
    else if(isALocalService && !isALocalMO)
    {
        // Local Service Global MO    
        pWaveManagedObjectCreateContext->setIsDatabaseUpdateRequired (false);
        pWaveManagedObjectCreateContext->setIsBackendUpdateRequired (true);        

        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createQueryManagedObjectStep : Local Service Global MO ");
    }
    else
    {
        // Global Service Local MO    
        pWaveManagedObjectCreateContext->setIsDatabaseUpdateRequired (false);
        pWaveManagedObjectCreateContext->setIsBackendUpdateRequired (false);       

        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createQueryManagedObjectStep : Global Service Local MO");
        
        prismAssert (false, __FILE__, __LINE__);        
    }

    WaveManagedObject *pOperateOnWaveManagedObject = NULL;

    if (ObjectId::NullObjectId != operateOnWaveManagedObjectId)
    {
        pOperateOnWaveManagedObject = queryManagedObject (operateOnWaveManagedObjectId);
    }

    if (NULL != pOperateOnWaveManagedObject)
    {
        pWaveManagedObjectCreateContext->setPOperateOnWaveManagedObject (pOperateOnWaveManagedObject);
    }

    if (NULL == pOperateOnWaveManagedObject )
    {
        if (pWaveManagedObjectCreateContext->getIsDatabaseUpdateRequired())
        {
            pWaveManagedObjectCreateContext->setNeedToCreateManagedObject (true);
        }
    }
   

    pWaveManagedObjectCreateContext->executeNextStep (status);
    return;
}

void WaveManagedObjectCreateWorker::WaveManagedObjectStartTransactionStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::WaveManagedObjectStartTransactionStep: Entering ...");
 
    if (!pWaveManagedObjectCreateContext->getIsDatabaseUpdateRequired())
    {
        trace (TRACE_LEVEL_DEVEL , "WaveManagedObjectCreateWorker::WaveManagedObjectStartTransactionStep : skipping for Local Service & Global MO");
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }
 
    prismLinearSequencerStartTransactionStep (pWaveManagedObjectCreateContext);
}

#if 0
void WaveManagedObjectCreateWorker::createValidateManagedObjectStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    WaveManagedObject *pWaveManagedObject        = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();
    vector<string>     attributeNames            = pWaveManagedObjectCreateContext->getAttributeNames    ();
    vector<string>     attributeValues           = pWaveManagedObjectCreateContext->getAttributeValues   ();
    ResourceId resourceId                        = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::updateValidateManagedObjectStep : Entering ...");

    if (FrameworkToolKit::isALocalService(getServiceId())) 
    {
        if (NULL != pWaveManagedObject)
        {
            resourceId = pWaveManagedObject->validateForCreateAtThisNode(attributeNames, attributeValues);
        }
    }

    pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}
#endif

void WaveManagedObjectCreateWorker::createValidateManagedObjectStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    trace (TRACE_LEVEL_DEVEL, string("WaveManagedObjectCreateWorker::createValidateManagedObjectStep : Entering for Service ID : ") + getServiceId());

    vector<UI32>       attributeUserTags         = pWaveManagedObjectCreateContext->getAttributeUserTags ();
    vector<string>     attributeNames            = pWaveManagedObjectCreateContext->getAttributeNames    ();
    vector<string>     attributeValues           = pWaveManagedObjectCreateContext->getAttributeValues   ();
    UI32               numberOfAttributeUserTags = attributeUserTags.size ();
    UI32               numberOfAttributeNames    = attributeNames.size ();
    UI32               numberOfAttributeValues   = attributeValues.size ();
    UI32               i                         = 0;
    WaveManagedObject *pWaveManagedObject        = NULL;

    if (true == pWaveManagedObjectCreateContext->isNeedToCreateManagedObject ())
    {
        pWaveManagedObject = createManagedObject (pWaveManagedObjectCreateContext->getManagedObjectClassNameNeedToBeCreated ());

        if (NULL != pWaveManagedObject)
        {
            pWaveManagedObjectCreateContext->setPOperateOnWaveManagedObject     (pWaveManagedObject);
            pWaveManagedObjectCreateContext->setOperateOnWaveManagedObjectId    (pWaveManagedObject->getObjectId ());
        }

        pWaveManagedObject = NULL; 
    }
    else if (false == pWaveManagedObjectCreateContext->getIsDatabaseUpdateRequired())
    {
        //If Backend communication needed before database commit, we need to create a InMemory MO for the backend communciation.
        trace (TRACE_LEVEL_INFO,"WaveManagedObjectCreateWorker::createValidateManagedObjectStep : Create InMemoryMO step ");

        pWaveManagedObject = createInMemoryManagedObject(pWaveManagedObjectCreateContext->getManagedObjectClassNameNeedToBeCreated ());
        pWaveManagedObject->setOwnerManagedObjectId (pWaveManagedObjectCreateContext->getParentObjectId ());
        pWaveManagedObjectCreateContext->setPOperateOnWaveManagedObject (pWaveManagedObject);
        pWaveManagedObjectCreateContext->addManagedObjectForGarbageCollection (pWaveManagedObject);
        pWaveManagedObject = NULL;
    }
 
    pWaveManagedObject = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();
  
    if (NULL != pWaveManagedObject)
    {

        if ((0 != numberOfAttributeUserTags) && (0 != numberOfAttributeNames))
        {
            prismAssert (false, __FILE__, __LINE__);
        }

        prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

        if (0 != numberOfAttributeNames)
        {
            prismAssert (numberOfAttributeNames == numberOfAttributeValues, __FILE__, __LINE__);

            for (i = 0; i < numberOfAttributeNames; i++)
            {
                string attributeName  = attributeNames[i];
                string attributeValue = attributeValues[i];
                prismAssert ("" != attributeName, __FILE__, __LINE__);

                Attribute *pAttribute = pWaveManagedObject->getAttributeByName (attributeName);

                if (NULL == pAttribute)
                {
                    trace (TRACE_LEVEL_ERROR, string ("Attribute Name: ") + attributeName + string (" does not exist in this Managed Object. Please check your Managed Object Implementation to make sure this Attribtue exists."));
            
                    prismAssert (false,  __FILE__, __LINE__);
                }

                /* Add to the Attribute vector if it is not an "Association" type attribute */
                if ((pAttribute->getAttributeType() != AttributeType::AttributeTypeObjectId) && (pAttribute->getAttributeType () != AttributeType::AttributeTypeObjectIdVector)) 
                { 
                    Attribute *pClonedAttribute = pAttribute->clone ();

                    prismAssert (NULL != pClonedAttribute, __FILE__, __LINE__);
        
                    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createValidateManagedObjectStep : Setting by attribute name \'" + attributeName + "\' to \'" + attributeValue + "\'");

                    pClonedAttribute->fromString (attributeValue);
                    pWaveManagedObjectCreateContext->addToAttributeVector (pClonedAttribute);
                }
            }
       }   
        else
        {
            prismAssert (numberOfAttributeUserTags == numberOfAttributeValues, __FILE__, __LINE__);

            for (i = 0; i < numberOfAttributeUserTags; i++)
            {
                UI32   attributeUserTag = attributeUserTags[i];
                string attributeValue   = attributeValues[i];

                prismAssert (0 != attributeUserTag, __FILE__, __LINE__);

                Attribute *pAttribute = pWaveManagedObject->getAttributeByUserTag (attributeUserTag);

                if (NULL == pAttribute)
                {
                    trace (TRACE_LEVEL_ERROR, string ("This is no attribute corresponding to user tag: ") + attributeUserTag + string (" Please check your yang and Managed Object file to make sure this user tag exists in Managed Object"));

                    prismAssert (false, __FILE__, __LINE__);
                }

                trace (TRACE_LEVEL_DEVEL, string ("WaveManagedObjectCreateWorker::createValidateManagedObjectStep : Setting by attribute user tag \'") + attributeUserTag + string ("\' to \'") + attributeValue + string ("\'"));

                /* Add to the Attribute vector if it is not an "Association" type attribute */
                if ((pAttribute->getAttributeType() != AttributeType::AttributeTypeObjectId) && (pAttribute->getAttributeType () != AttributeType::AttributeTypeObjectIdVector))
                { 
                    Attribute *pClonedAttribute = pAttribute->clone ();

                    prismAssert (NULL != pClonedAttribute, __FILE__, __LINE__);

                    pClonedAttribute->fromString (attributeValue);
                    pWaveManagedObjectCreateContext->addToAttributeVector (pClonedAttribute);
                }
                attributeNames.push_back (pAttribute->getAttributeName ());
            }

            pWaveManagedObjectCreateContext->setAttributeNames (attributeNames);
        }

        // After Managed Object is create Validation will done for that case


        if (FrameworkToolKit::isALocalService (getServiceId()))
        {
            trace (TRACE_LEVEL_DEBUG , "WaveManagedObjectCreateWorker::validateForCreateAtThisNode Local service");      
            if (NULL != pWaveManagedObject)
            {
                ValidateForCreateAtThisNodeContext *pValidateForCreateAtThisNodeContext = new ValidateForCreateAtThisNodeContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveManagedObjectCreateWorker::validateForCreateAtThisNodeCallBack), pWaveManagedObjectCreateContext);

                pValidateForCreateAtThisNodeContext->setAttributeNames      (attributeNames);
                pValidateForCreateAtThisNodeContext->setAttributeValues     (attributeValues);
                pValidateForCreateAtThisNodeContext->setAttributeVector     (pWaveManagedObjectCreateContext->getAttributes ());
                           
                pWaveManagedObject->validateForCreateAtThisNode (pValidateForCreateAtThisNodeContext);
            }
        }
        else     // TODO: Validation for Global service
        {
            trace (TRACE_LEVEL_DEBUG , "WaveManagedObjectCreateWorker::validateForCreateAtThisNode Global service");

            if (NULL != pWaveManagedObject)
            {
                ValidateForCreateAtThisNodeContext *pValidateGlobalForCreateAtThisNodeContext = new ValidateForCreateAtThisNodeContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveManagedObjectCreateWorker::validateForCreateAtThisNodeCallBack), pWaveManagedObjectCreateContext);
                                             
                pValidateGlobalForCreateAtThisNodeContext->setAttributeNames      (attributeNames);
                pValidateGlobalForCreateAtThisNodeContext->setAttributeValues     (attributeValues);
                pValidateGlobalForCreateAtThisNodeContext->setAttributeVector     (pWaveManagedObjectCreateContext->getAttributes ());
                  
                pWaveManagedObject->validateGlobalForCreateAtThisNode (pValidateGlobalForCreateAtThisNodeContext);
            }
        }
    }
    else
    {
         trace (TRACE_LEVEL_INFO , "WaveManagedObjectCreateWorker::validateForCreateAtThisNode ERROR");   
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }

    return;
}


void WaveManagedObjectCreateWorker::validateForCreateAtThisNodeCallBack (ValidateForCreateAtThisNodeContext *pValidateForCreateAtThisNodeContext)
{
    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pValidateForCreateAtThisNodeContext->getPCallerContext ());
    ResourceId                                               status = pValidateForCreateAtThisNodeContext->getCompletionStatus ();
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR , "WaveManagedObjectCreateWorker::validateForCreateAtThisNodeCallBack Validation Fails");

        pWaveManagedObjectCreateContext->executeNextStep (status);
        return;
    }

    pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveManagedObjectCreateWorker::createPreUpdateManagedObjectStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    tracePrintf (TRACE_LEVEL_DEBUG , "WaveManagedObjectCreateWorker::createPreUpdateManagedObjectStep entering for service ID  :%d", getServiceId());

    if (!pWaveManagedObjectCreateContext->getIsDatabaseUpdateRequired())
    {
        trace(TRACE_LEVEL_DEBUG , "WaveManagedObjectCreateWorker::createPreUpdateManagedObjectStep skipping for  Local Service & Global MO");
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    vector<Attribute*> attributes                = pWaveManagedObjectCreateContext->getAttributes ();
    vector<string>     attributeNames            = pWaveManagedObjectCreateContext->getAttributeNames ();
    vector<string>     attributeValues           = pWaveManagedObjectCreateContext->getAttributeValues ();
    string             attributeValue;
    UI32               numberOfAttributeNames    = attributeNames.size ();
    UI32               i;
    WaveManagedObject *pWaveManagedObject        = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();
    ObjectId           parentObjectId            = pWaveManagedObject->getObjectId ();
    string             parentClassName           = OrmRepository::getTableNameById (parentObjectId.getClassId ());

    /* Manage Context counter to handle multiple Associations */
    ++(*pWaveManagedObjectCreateContext);

    for (i = 0; i < numberOfAttributeNames; i++)
    {
        Attribute *pAttribute = pWaveManagedObject->getAttributeByName (attributeNames[i]);

        attributeValue = attributeValues[i];

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        if ((pAttribute->getAttributeType () == AttributeType::AttributeTypeObjectId) || (pAttribute->getAttributeType () == AttributeType::AttributeTypeObjectIdVector)) 
        {
            /* Get the class name of the associated object */

            AttributeObjectIdAssociation *pAttributeObjectIdAssociation = dynamic_cast<AttributeObjectIdAssociation *> (pAttribute);
            string                        associatedClassName           = pAttributeObjectIdAssociation->getAssociatedTo ();
            map<string, Attribute*>       associatedKeyNamesAndTypesMap;
            vector <string>               associatedKeyNames;
            Attribute                    *pResultAttribute;
            vector<WaveManagedObject *>  *pResults;

            /* Get the Keys (or combination thereof) used for this class */

            associatedKeyNamesAndTypesMap = OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (associatedClassName, associatedKeyNames);
            pResultAttribute = associatedKeyNamesAndTypesMap[associatedKeyNames[0]];

            Attribute *pAssociatedAttribute = Attribute::getAttributeFromAttributeType((pResultAttribute->getAttributeType()).getAttributeTypeResourceId());

            /* We are only supporting MOs with a single primary key */
            pAssociatedAttribute->setAttributeName(pResultAttribute->getAttributeName());

            prismAssert (associatedKeyNames.size () == 1, __FILE__, __LINE__);

            pAssociatedAttribute->fromString(attributeValue);

            /* Create a clone of the associated key and set the attribute name as parent attribute name and append it to the attribute vector */
            Attribute *pClonedAttribute = pAssociatedAttribute->clone ();
            prismAssert (NULL != pClonedAttribute, __FILE__, __LINE__);
            pClonedAttribute->setAttributeName(pAttribute->getAttributeName ());
            pWaveManagedObjectCreateContext->addToAttributeVector (pClonedAttribute);

            /* Query the database to find the instance */

            WaveManagedObjectSynchronousQueryContext synchronousQueryContext(associatedClassName);
            synchronousQueryContext.addAndAttribute (pAssociatedAttribute);

            if (true == OrmRepository::isALocalManagedObject(associatedClassName))
            {
                synchronousQueryContext.setLocationIdFilter (FrameworkToolKit::getThisLocationId ());
            }

            pResults = querySynchronously (&synchronousQueryContext);

            /* Set the Object ID in the Location MO to point to the returned results */
            if (pResults->size () != 0) 
            {
                ObjectId childObjectId = (*pResults)[0]->getObjectId (); 

                addRelationship (parentClassName, associatedClassName, attributeNames[i], parentObjectId, childObjectId);

                pWaveManagedObjectCreateContext->addManagedObjectsForGarbageCollection (*pResults);
            } 
            else 
            {
                IsAssociatedManagedObjectToBeCreatedContext *pAssociatedContext = new IsAssociatedManagedObjectToBeCreatedContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveManagedObjectCreateWorker::isAssociatedManagedObjectToBeCreatedCallback), pWaveManagedObjectCreateContext);

                pAssociatedContext->setAssociatedClassName(associatedClassName);
                pAssociatedContext->setAssociatedAttributeName(pAssociatedAttribute->getAttributeName());
                pAssociatedContext->setAssociatedAttributeValue(attributeValue);
                pAssociatedContext->setAttributeIndex(i);

                /* Manage Context counter to handle multiple Associations */
                ++(*pWaveManagedObjectCreateContext);
                pWaveManagedObject->isAssociatedManagedObjectToBeCreated(pAssociatedContext); 
            }
        }
    }

    --(*pWaveManagedObjectCreateContext);

    if (WAVE_MESSAGE_SUCCESS != pWaveManagedObjectCreateContext->getErrorInCreatingMO())
    {
        pWaveManagedObjectCreateContext->executeNextStep (pWaveManagedObjectCreateContext->getErrorInCreatingMO());
    }
    else
    {
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

    return;
}

ObjectId WaveManagedObjectCreateWorker::createAssociatedManagedObject (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext, IsAssociatedManagedObjectToBeCreatedContext *pAssociatedContext, string associatedClassName, string associatedAttributeName, string associatedAttributeValue)
{
    trace (TRACE_LEVEL_DEVEL, string ("WaveManagedObjectCreateWorker::createAssociatedManagedObject:: Entering"));

    WaveObjectManager                                  *pWaveObjectManager          = WaveObjectManager::getOwnerForManagedClass (associatedClassName);
    //bool                isALocalManagedObject = OrmRepository::isALocalManagedObject (associatedClassName);
    vector<string>                                      associatedAttributeNames;
    vector<string>                                      associatedAttributeValues;
    UI32                                                status;
    WaveObjectManagerCreateWaveManagedObjectMessage    *pCreateMessage              = NULL; 
    PrismMessage                                       *pAssociatedMessage          = pAssociatedContext->getMessage(); 

    pAssociatedContext->setAttributeVector (pWaveManagedObjectCreateContext->getAttributes ());
    pAssociatedContext->setAttributeNames (pWaveManagedObjectCreateContext->getAttributeNames ());
    pAssociatedContext->setAttributeValues (pWaveManagedObjectCreateContext->getAttributeValues ());

    if (!pAssociatedMessage) 
    {
        /* Allocate the message with the appropriate service Id */
        pCreateMessage = new WaveObjectManagerCreateWaveManagedObjectMessage (pWaveObjectManager->getServiceId ());
        pCreateMessage->setManagedObjectClassNameNeedToBeCreated (associatedClassName);

        /* Setup the attributes */

        associatedAttributeValues.clear   ();
        associatedAttributeNames.clear    ();

        associatedAttributeNames.push_back(associatedAttributeName);
        associatedAttributeValues.push_back(associatedAttributeValue);
        pCreateMessage->setAttributeValues   (associatedAttributeValues);
        pCreateMessage->setAttributeNames    (associatedAttributeNames);

#if 0
                        /* Check to see if the associated class/object is of type composition or is not locally managed*/
                        if (!isALocalManagedObject) {
                            prismAssert (0, __FILE__, __LINE__);
                        }
#endif
    } 
    else 
    {
        /* We have the message, so no changes required */
    }

    /* Send this message to the OM on the local node to create the object */
    if (NULL != pAssociatedMessage) 
    {
        trace (TRACE_LEVEL_DEVEL, string ("WaveManagedObjectCreateWorker::createAssociatedManagedObject:: Sending associated Message"));

        status = sendSynchronously(pAssociatedMessage, FrameworkToolKit::getThisLocationId ());
    } 
    else 
    {
        trace (TRACE_LEVEL_DEVEL, string ("WaveManagedObjectCreateWorker::createAssociatedManagedObject:: Sending Message to create a new associated managed object"));

        status = sendSynchronously(pCreateMessage, FrameworkToolKit::getThisLocationId ());
    }

    trace (TRACE_LEVEL_DEVEL, string ("WaveManagedObjectCreateWorker::createAssociatedManagedObject:: Response received"));

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (NULL != pCreateMessage) 
        {
            /* Check for completion status */
            if (pCreateMessage->getCompletionStatus() == WAVE_MESSAGE_SUCCESS)
            {
                /* MO was successfully created, so get the object Id */
                ObjectId childObjectId = pCreateMessage->getObjectId();

                delete pCreateMessage;

                return (childObjectId);
            } 
        }
    }
    return (ObjectId::NullObjectId);
}

void WaveManagedObjectCreateWorker::isAssociatedManagedObjectToBeCreatedCallback (IsAssociatedManagedObjectToBeCreatedContext *pAssociatedContext)
{
    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pAssociatedContext->getPCallerContext ());
    ResourceId                      status = pAssociatedContext->getCompletionStatus ();
    string                          associatedClassName = pAssociatedContext->getAssociatedClassName();
    WaveManagedObject              *pWaveManagedObject  = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();
    vector<string>                  attributeNames      = pWaveManagedObjectCreateContext->getAttributeNames ();
    ObjectId                        parentObjectId      = pWaveManagedObject->getObjectId ();
    string                          parentClassName     = OrmRepository::getTableNameById (parentObjectId.getClassId ());
    
    if (WAVE_MESSAGE_SUCCESS == status && pAssociatedContext->getIsAssociatedMOToBeCreated())
    {
        trace (TRACE_LEVEL_DEVEL, string ("WaveManagedObjectCreateWorker::isAssociatedManagedObjectToBeCreatedCallback: Creating associated managed object"));

        ObjectId childObjectId = createAssociatedManagedObject(pWaveManagedObjectCreateContext, pAssociatedContext, associatedClassName, pAssociatedContext->getAssociatedAttributeName(),  pAssociatedContext->getAssociatedAttributeValue());

        if (ObjectId::NullObjectId != childObjectId) 
        {
            trace (TRACE_LEVEL_DEVEL, string ("WaveManagedObjectCreateWorker::isAssociatedManagedObjectToBeCreatedCallback: Adding relationship"));
            addRelationship (parentClassName, associatedClassName, attributeNames[pAssociatedContext->getAttributeIndex()], parentObjectId, childObjectId);
        } 
        else 
        {
            /* Manage Context counter to handle multiple Associations */
            --(*pWaveManagedObjectCreateContext);
            pWaveManagedObjectCreateContext->setErrorInCreatingMO (WAVE_MESSAGE_ERROR);
            pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_ERROR);
            return;
        }
    }
    else 
    {
        /* Manage Context counter to handle multiple Associations */
        --(*pWaveManagedObjectCreateContext);

        //User has set the completion status as SuCCESS but IsAssociatedMOToBeCreated flag is not set, so the final status is ERROR.
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            status = WAVE_MESSAGE_ERROR;
        }

        pWaveManagedObjectCreateContext->setErrorInCreatingMO(status);
        pWaveManagedObjectCreateContext->executeNextStep (status);
        return;
    }

    /* Manage Context counter to handle multiple Associations */
    --(*pWaveManagedObjectCreateContext);

    /* Check to see if we have encountered any error at all, in which case we will error out the last step */
    if (WAVE_MESSAGE_SUCCESS != pWaveManagedObjectCreateContext->getErrorInCreatingMO()) 
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveManagedObjectCreateWorker::isAssociatedManagedObjectToBeCreatedCallback: Error in Creating an Managed Object"));
        pWaveManagedObjectCreateContext->executeNextStep (pWaveManagedObjectCreateContext->getErrorInCreatingMO());
    }
    else 
    {
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

}

void WaveManagedObjectCreateWorker::createUpdateManagedObjectStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    trace (TRACE_LEVEL_DEBUG , "WaveManagedObjectCreateWorker::createUpdateManagedObjectStep entering ");

    UI32               i                         = 0;
    WaveManagedObject *pWaveManagedObject        = NULL;
    vector<Attribute*> attributes                = pWaveManagedObjectCreateContext->getAttributes ();
    UI32               numberOfAttributes        = attributes.size ();

    if (!pWaveManagedObjectCreateContext->getIsDatabaseUpdateRequired())
    {
        trace(TRACE_LEVEL_DEBUG, "WaveManagedObjectCreateWorker::createUpdateManagedObjectStep skipping for Local Service & Global MO");
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }


    pWaveManagedObject = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();

    if (NULL != pWaveManagedObject)
    {
        pWaveManagedObjectCreateContext->addManagedObjectForGarbageCollection (pWaveManagedObject);

        for (i = 0; i < numberOfAttributes; i++)
        {
            Attribute *pClonedAttribute = attributes[i];
            Attribute *pAttribute       = pWaveManagedObject->getAttributeByName (pClonedAttribute->getAttributeName ());
            string     tempString;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if ((pAttribute->getAttributeType() != AttributeType::AttributeTypeObjectId) && (pAttribute->getAttributeType () != AttributeType::AttributeTypeObjectIdVector))
            {

                pClonedAttribute->toString (tempString);
    
                pAttribute->fromString (tempString);
            }
        }

        if (false == pWaveManagedObjectCreateContext->isNeedToCreateManagedObject ())
        {
            updateWaveManagedObject (pWaveManagedObject);
        }
        else
        {
            WaveObjectManagerCreateWaveManagedObjectMessage *pMessage = dynamic_cast<WaveObjectManagerCreateWaveManagedObjectMessage *> (pWaveManagedObjectCreateContext->getPPrismMessage()); 
            WaveManagedObjectAsynchronousCreateContext      *pContext = dynamic_cast<WaveManagedObjectAsynchronousCreateContext *> (pWaveManagedObjectCreateContext->getPPrismAsynchronousContext ());

            /* Save the object Id in the message to be sent to caller */
            if (NULL != pContext)
            {
                pContext->setObjectId (pWaveManagedObject->getObjectId ());
            }
            else if (NULL != pMessage)
            {
                pWaveManagedObject->createWaveManagedObject ();
                pMessage->setObjectId (pWaveManagedObject->getObjectId());
            }
            else
            {
                prismAssert (false, __FILE__, __LINE__);
            }
        }

        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}


void WaveManagedObjectCreateWorker::createUpdateNewAttributeInManagedObject (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    vector<Attribute*> attributes                = pWaveManagedObjectCreateContext->getAttributes ();
    WaveManagedObject *pWaveManagedObject        = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();

    UpdateNewAttributeInManagedObjectContext *pUpdateNewAttributeInManagedObjectContext = new UpdateNewAttributeInManagedObjectContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveManagedObjectCreateWorker::createUpdateNewAttributeInManagedObjectCallback), pWaveManagedObjectCreateContext);

    pUpdateNewAttributeInManagedObjectContext->setAttributeVector (pWaveManagedObjectCreateContext->getAttributes ());

    pWaveManagedObject->addNewAttributeToBeUpdatedForCreate (pUpdateNewAttributeInManagedObjectContext);
}


//If the plugin developer wants to add an attribute which is not passed through CLI directly but must be created 
//along with the current create request this method should be overridden for all such requirements
void WaveManagedObjectCreateWorker::createUpdateNewAttributeInManagedObjectCallback (UpdateNewAttributeInManagedObjectContext *pUpdateNewAttributeInManagedObjectContext)
{
    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pUpdateNewAttributeInManagedObjectContext->getPCallerContext ());
    WaveManagedObject              *pWaveManagedObject              = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();
    ResourceId                      status                          = pUpdateNewAttributeInManagedObjectContext->getCompletionStatus ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createUpdateNewAttributeInManagedObjectCallback adding new attribute Fails");
        pWaveManagedObjectCreateContext->executeNextStep (status);
        return;
    }

    vector<string>   newAttributeNames     = pUpdateNewAttributeInManagedObjectContext->getNewAttributeNames ();
    UI32             numberOfNewAttributes = newAttributeNames.size ();

    for (UI32 i = 0; i < numberOfNewAttributes; i++)
    {
        //add each attribute in the attribute vector of Main Context
        Attribute *pAttribute = pWaveManagedObject->getAttributeByName (newAttributeNames[i]);
    
        Attribute *pClonedAttribute = pAttribute->clone ();
 
        //If any child MO has to be created as part of the Parent MO creation with default value put that in a seperate vector
        //This will be handled in seperate step "createUpdateChildHardwareStep" in the Linear Sequencer for Child MO Creation 
        if ((pAttribute->getAttributeType () != AttributeType::AttributeTypeCompositionVector) && (pAttribute->getAttributeType () != AttributeType::AttributeTypeComposition))
        {
            pWaveManagedObjectCreateContext->addToAttributeVector (pClonedAttribute);
        }
        else
        {
            pWaveManagedObjectCreateContext->addToChildAttributeVector (pClonedAttribute);
        }
    }

    pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveManagedObjectCreateWorker::createPostUpdateManagedObjectStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectCreateWorker::createPostUpdateManagedObjectStep : Entering ...");

    if(!pWaveManagedObjectCreateContext->getIsDatabaseUpdateRequired()) 
    {
        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createPostUpdateManagedObjectStep skipping for  Local Service Global MO");
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;       
    }


    PrismAsynchronousContext *pPrismAsynchronousContext    = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveManagedObjectCreateWorker::createPostUpdateManagedObjectCallBack), pWaveManagedObjectCreateContext);
    WaveManagedObject        *pOperateOnWaveManagedObject  = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject   ();
    ResourceId                status                       = WAVE_MESSAGE_SUCCESS;

    if (NULL != pOperateOnWaveManagedObject)
    {
        // check if it is composition then update composition
        UI32                childUserTag                = pWaveManagedObjectCreateContext->getChildUserTag ();

        if (0 != childUserTag)
        {
            ObjectId            parentObjectId              = pWaveManagedObjectCreateContext->getParentObjectId ();
            WaveManagedObject  *pParentWaveManagedObject    = queryManagedObject (parentObjectId);
            ObjectId            childObjectId               = pOperateOnWaveManagedObject->getObjectId ();
            string              parentClassName             = OrmRepository::getTableNameById (parentObjectId.getClassId ());
            string              childClassName              = OrmRepository::getTableNameById (childObjectId.getClassId ());

            prismAssert (NULL != pParentWaveManagedObject, __FILE__, __LINE__);

            Attribute *pAttribute       = pParentWaveManagedObject->getAttributeByUserTag (childUserTag);

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            string     compositionName  = pAttribute->getAttributeName ();

            if (isManagedClassSupported (parentClassName))
            {
                addRelationship  (parentClassName, childClassName, compositionName, parentObjectId, childObjectId);
            }
            else
            {
                status = updateRelationship (pWaveManagedObjectCreateContext, parentClassName, childClassName, compositionName, parentObjectId, childObjectId);
            }

            pWaveManagedObjectCreateContext->addManagedObjectForGarbageCollection (pParentWaveManagedObject);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                pWaveManagedObjectCreateContext->executeNextStep (status);
				delete pPrismAsynchronousContext;
                return;
            }

            pOperateOnWaveManagedObject->setOwnerManagedObjectId (parentObjectId);

        }

        pOperateOnWaveManagedObject->createPostUpdateForOperateOnWaveManagedObject (pPrismAsynchronousContext);
        
    }
}

void WaveManagedObjectCreateWorker::createPostUpdateManagedObjectCallBack (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createPostUpdateManagedObjectCallBack : Entering ...");

    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                      status                          = pPrismAsynchronousContext->getCompletionStatus ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createPostUpdateManagedObjectCallBack completion status " + FrameworkToolKit::localize (status));
    }

    delete pPrismAsynchronousContext;

    pWaveManagedObjectCreateContext->executeNextStep (status);

}

void WaveManagedObjectCreateWorker::createPreUpdateHardwareStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    if (pWaveManagedObjectCreateContext->getIsBackendUpdateRequired())
    {
        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createPreUpdateHardwareStep skipping for local service");
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createPreUpdateHardwareStep : Entering ...");

    PrismAsynchronousContext *pPrismAsynchronousContext     = new PrismAsynchronousContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveManagedObjectCreateWorker::createPreUpdateHardwareStepCallback), pWaveManagedObjectCreateContext);
    WaveManagedObject        *pOperateOnWaveManagedObject   = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject   ();

    if (NULL != pOperateOnWaveManagedObject)
    {
        pOperateOnWaveManagedObject->preCreateHardwareStepForOperateOnWaveManagedObject (pPrismAsynchronousContext);
    }
	else
	{
		delete pPrismAsynchronousContext;
		pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_ERROR);
	}
    
    return;
}

void WaveManagedObjectCreateWorker::createPreUpdateHardwareStepCallback (PrismAsynchronousContext *pPrismAsynchronousContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createPreUpdateHardwareStepCallBack : Entering ...");

    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pPrismAsynchronousContext->getPCallerContext ());
    ResourceId                      status                          = pPrismAsynchronousContext->getCompletionStatus ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createPreUpdateHardwareStepCallback completion status " + FrameworkToolKit::localize (status));
    }

    delete pPrismAsynchronousContext;

    pWaveManagedObjectCreateContext->executeNextStep (status);
}

void WaveManagedObjectCreateWorker::createUpdateHardwareStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    if (!pWaveManagedObjectCreateContext->getIsBackendUpdateRequired())
    {
        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createUpdateHardwareStep : skipping for Global Service");      
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;  
    }

    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createUpdateHardwareStep : Entering ...for local service");

    WaveManagedObject          *pOperateOnWaveManagedObject  = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject   ();

    if (NULL != pOperateOnWaveManagedObject)
    {
        GetHardwareConfigurationDetailsForCreateContext *pGetHardwareConfigurationDetailsForCreateContext = new GetHardwareConfigurationDetailsForCreateContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveManagedObjectCreateWorker::createUpdateHardwareStepCallBack), pWaveManagedObjectCreateContext); 

        pGetHardwareConfigurationDetailsForCreateContext->setAttributeNames      (pWaveManagedObjectCreateContext->getAttributeNames ());
        pGetHardwareConfigurationDetailsForCreateContext->setAttributeValues     (pWaveManagedObjectCreateContext->getAttributeValues ());
        pGetHardwareConfigurationDetailsForCreateContext->setAttributeVector     (pWaveManagedObjectCreateContext->getAttributes ());

        pOperateOnWaveManagedObject->getHardwareConfigurationDetailsForCreate (pGetHardwareConfigurationDetailsForCreateContext);
    }
    else
    {
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_ERROR);
    }
}

void WaveManagedObjectCreateWorker::createUpdateHardwareStepCallBack (GetHardwareConfigurationDetailsForCreateContext *pGetHardwareConfigurationDetailsForCreateContext)
{
    UnifiedClientCreateMessage     *pMessage                        = NULL;
    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pGetHardwareConfigurationDetailsForCreateContext->getPCallerContext ());
    ResourceId                      status                          = pGetHardwareConfigurationDetailsForCreateContext->getCompletionStatus ();
    WaveManagedObject              *pOperateOnWaveManagedObject     = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject   ();
    string                          clientNameToSend;
   
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createUpdateHardwareStepCallBack: entering .........");
              
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createUpdateHardwareStepCallBack: failed to get the hardware details");

        pWaveManagedObjectCreateContext->executeNextStep (status);
        return;
    }

    if (pGetHardwareConfigurationDetailsForCreateContext->getIsUpdateBackend ())
    {
        if (NULL != pOperateOnWaveManagedObject)
        {
            pGetHardwareConfigurationDetailsForCreateContext->setManagedObjectName (pOperateOnWaveManagedObject->getObjectClassName ());

            //Populate the key attributes of an MO for the backend if the key is NOT already present
            populateKeyAttributesIfNotPresent (pGetHardwareConfigurationDetailsForCreateContext);

            //Populate the parent key attributes in case of composition. In case of Composition updation of the parent key alone is sufficient as part of child MO update/delete to the backend

            populateParentKeyAttributes (pGetHardwareConfigurationDetailsForCreateContext);

            //Populate the realated attributes if they are not modified
            //This is required by the backend to use common layer.
            clientNameToSend = getClientNameToSendAndPopulateRelatedAttributes (pGetHardwareConfigurationDetailsForCreateContext);
            prismAssert ("" != clientNameToSend, __FILE__, __LINE__);

            pMessage = new UnifiedClientCreateMessage (pGetHardwareConfigurationDetailsForCreateContext);

            pMessage->setConfigReplayInProgressFlag (pWaveManagedObjectCreateContext->getConfigReplayInProgressFlag());

            trace (TRACE_LEVEL_INFO, "Sending to Client " + clientNameToSend);

            status = sendSynchronouslyToWaveClient (clientNameToSend, pMessage);

            if (status == WAVE_MESSAGE_SUCCESS)
            {
                status = pMessage->getCompletionStatus ();
                
                WaveObjectManagerCreateWaveManagedObjectMessage *pCreateMessage = reinterpret_cast<WaveObjectManagerCreateWaveManagedObjectMessage *> (pWaveManagedObjectCreateContext->getPPrismMessage());
                if (pCreateMessage != NULL)
                {
                    pCreateMessage->setWarningResourceId (pMessage->getWarningResourceId());   
                }
                pWaveManagedObjectCreateContext->setAttributeNamesFromBackend (pMessage->getAttributeNamesFromBackend ());
                pWaveManagedObjectCreateContext->setAttributeValuesFromBackend (pMessage->getAttributeValuesFromBackend ());
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string("WaveManagedObjectCreateWorker::createUpdateHardwareStepCallBack : Error while sending Message to client " + clientNameToSend + " with status : " + FrameworkToolKit::localize (status)));
            }

            if (0 != pMessage->getWarningResourceId())
            {
                trace (TRACE_LEVEL_INFO, string("WaveManagedObjectCreateWorker::createUpdateHardwareStepCallBack : WarningResourceId = " ) + FrameworkToolKit::localize (pMessage->getWarningResourceId()));
            }

            delete pMessage;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createUpdateHardwareStep : Could not get the Operate on Managed Object");
            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectCreateWorker::createUpdateHardwareStep : message NOT sent to backend");
    }

    pWaveManagedObjectCreateContext->executeNextStep (status);
}


// In case there is a composed MO that needs to be created as part of the Parent Mo creation this step will be invoked  
void WaveManagedObjectCreateWorker::createUpdateHardwareStepForChild (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    ResourceId  status  = WAVE_MESSAGE_SUCCESS;

    if(!pWaveManagedObjectCreateContext->getIsDatabaseUpdateRequired ())
    {   
        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createUpdateHardwareStepForChild skipping for  Local Service Global MO");
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    trace (TRACE_LEVEL_DEBUG, string("WaveManagedObjectCreateWorker::createUpdateHardwareStepForChild : for service id :") + getServiceId());
    // Check if the child attribute vector is empty (ie the parent does not contain any child composed Mo to be created) if so return
    vector <Attribute *> childAttributeVector = pWaveManagedObjectCreateContext->getChildAttributeVector ();

    for (UI32 i = 0 ; i< childAttributeVector.size (); i++)
    {
        Attribute *pComposedAttribute  = childAttributeVector[i];

        vector<WaveManagedObject *> *pChildWaveManagedObjectVector = pComposedAttribute->getComposedManagedObject ();

        WaveManagedObject *pOperateOnWaveManagedObject = (*pChildWaveManagedObjectVector)[0];

        if (NULL != pOperateOnWaveManagedObject)
        {   
            trace (TRACE_LEVEL_DEVEL, string("WaveManagedObjectCreateWorker::createUpdateHardwareStepForChild : operating on the composed chlild MO name: %d ")+ pOperateOnWaveManagedObject->getName() );
            pOperateOnWaveManagedObject->prepareForSerialization();
           
            GetHardwareConfigurationDetailsForCreateContext *pGetHardwareConfigurationDetailsForCreateContext = new GetHardwareConfigurationDetailsForCreateContext (pOperateOnWaveManagedObject, this, reinterpret_cast<PrismAsynchronousCallback> (&WaveManagedObjectCreateWorker::createUpdateHardwareStepForChildCallBack), pWaveManagedObjectCreateContext);

            pOperateOnWaveManagedObject->getHardwareConfigurationDetailsForCreate (pGetHardwareConfigurationDetailsForCreateContext);
           
            status = pGetHardwareConfigurationDetailsForCreateContext->getCompletionStatus ();
            delete pGetHardwareConfigurationDetailsForCreateContext;
            
            if( WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createUpdateHardwareStepForChild: Could not send the child MO to backend ");
                break;
            }
        }
        else
        {   
            trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createUpdateHardwareStepForChild : Could not get the Operate on Managed Object");
            status = WAVE_MESSAGE_ERROR;
            break;
        }
    }    

    pWaveManagedObjectCreateContext->executeNextStep (status);
}


void WaveManagedObjectCreateWorker::createUpdateHardwareStepForChildCallBack (GetHardwareConfigurationDetailsForCreateContext *pGetHardwareConfigurationDetailsForCreateContext)
{
    UnifiedClientCreateMessage     *pMessage                        = NULL;
    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pGetHardwareConfigurationDetailsForCreateContext->getPCallerContext ());
    ResourceId                      status                          = pGetHardwareConfigurationDetailsForCreateContext->getCompletionStatus ();
    WaveManagedObject              *pOperateOnWaveManagedObject     = pGetHardwareConfigurationDetailsForCreateContext->getPOperateOnWaveManagedObject ();
    string                          clientNameToSend;
    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectCreateWorker::createUpdateHardwareStepforchildcallback: entering ......");
              
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createUpdateHardwareStepForChildCallBack: failed to get the hardware details");
        pWaveManagedObjectCreateContext->executeNextStep (status);
        return;
    }

    if (pGetHardwareConfigurationDetailsForCreateContext->getIsUpdateBackend ())
    {
        if (NULL != pOperateOnWaveManagedObject)
        {
            pGetHardwareConfigurationDetailsForCreateContext->setManagedObjectName (pOperateOnWaveManagedObject->getObjectClassName ());

            //Populate the key attributes of an MO for the backend if the key is NOT already present
            populateKeyAttributesIfNotPresent (pGetHardwareConfigurationDetailsForCreateContext);

            //adding parent Key for the child
            pGetHardwareConfigurationDetailsForCreateContext->addToAttributeVector ((pWaveManagedObjectCreateContext->getAttributes())[0]); 

            //Populate the parent key attributes in case of composition. In case of Composition updation of the parent key alone is sufficient as part of child MO update/delete to the backend
            populateParentKeyAttributes (pGetHardwareConfigurationDetailsForCreateContext);

            //adding Parent MO name
            vector<string> parentMoNames;
            pGetHardwareConfigurationDetailsForCreateContext->getParentManagedObjectNames (parentMoNames);
            parentMoNames.insert(parentMoNames.begin(), pWaveManagedObjectCreateContext->getManagedObjectClassNameNeedToBeCreated ());
            pGetHardwareConfigurationDetailsForCreateContext->setParentManagedObjectNames (parentMoNames);

            //Populate the realated attributes if they are not modified
            //This is required by the backend to use common layer.
            clientNameToSend = getClientNameToSendAndPopulateRelatedAttributes (pGetHardwareConfigurationDetailsForCreateContext);
            prismAssert ("" != clientNameToSend, __FILE__, __LINE__);

            pMessage = new UnifiedClientCreateMessage (pGetHardwareConfigurationDetailsForCreateContext);

            trace (TRACE_LEVEL_INFO, "Sending to Client " + clientNameToSend);
            status = sendSynchronouslyToWaveClient (clientNameToSend, pMessage);

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                status = pMessage->getCompletionStatus ();
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string("WaveManagedObjectCreateWorker::createUpdateHardwareStepForChildCallBack : Error while sending Message to client " + clientNameToSend + " with status : " + FrameworkToolKit::localize (status)));
            }

            if (0 != pMessage->getWarningResourceId())
            {
                trace (TRACE_LEVEL_INFO, string("WaveManagedObjectCreateWorker::createUpdateHardwareStepForChildCallBack : WarningResourceId = " ) + FrameworkToolKit::localize (pMessage->getWarningResourceId()));
            }
            
            delete pMessage;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createUpdateHardwareStepForChildCallBack : Could not get the Operate on Managed Object");
            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectCreateWorker::createUpdateHardwareStepforchildcallback : message NOT sent to backend");
    }
    
    pGetHardwareConfigurationDetailsForCreateContext->setCompletionStatus (status);
}


void WaveManagedObjectCreateWorker::createPostUpdateHardwareStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    if (!pWaveManagedObjectCreateContext->getIsBackendUpdateRequired())
    {
        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createPostUpdateHardwareStep : skipping for Global Service");        
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }   

    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::createPostUpdateHardwareStep : Entering ...");

    pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

void WaveManagedObjectCreateWorker::createAttributesPostUpdateHardwareStep (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    if (pWaveManagedObjectCreateContext->getIsBackendUpdateRequired())
    {
        trace(TRACE_LEVEL_DEVEL,"WaveManagedObjectCreateWorker::createAttributesPostUpdateHardwareStep ..");
 
        vector<string>     attributeNamesFromBackend  = pWaveManagedObjectCreateContext->getAttributeNamesFromBackend  ();
        vector<string>     attributeValuesFromBackend = pWaveManagedObjectCreateContext->getAttributeValuesFromBackend ();
        WaveManagedObject *pWaveManagedObject         = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();
 
        prismAssert (attributeNamesFromBackend.size() == attributeValuesFromBackend.size(), __FILE__, __LINE__);
        prismAssert (pWaveManagedObject != NULL, __FILE__, __LINE__);
 
        for (UI32 i = 0; i < attributeNamesFromBackend.size(); i++)
        {
            Attribute *pAttribute = pWaveManagedObject->getAttributeByName (attributeNamesFromBackend[i]);
 
            prismAssert (pAttribute != NULL, __FILE__, __LINE__);
 
            pAttribute->fromString (attributeValuesFromBackend[i]);
        }
    }
 
    pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

WaveManagedObject *WaveManagedObjectCreateWorker::createManagedObject (const string &className)
{
    WaveObjectManager *pCurrentWorkerWaveObjectManager = getPWaveObjectManager ();
    WaveObjectManager *pWaveObjectManagerFromClassName = WaveObjectManager::getOwnerForManagedClass (className);

    if (pCurrentWorkerWaveObjectManager != pWaveObjectManagerFromClassName)
    {
        trace (TRACE_LEVEL_ERROR, "WaveManagedObjectCreateWorker::createManagedObject Trying to create Managed Object Owned by different ObjectManager");
        prismAssert (false, __FILE__, __LINE__);
        return (NULL);
    }

    WaveManagedObject *pWaveManagedObject = pCurrentWorkerWaveObjectManager->createManagedObjectInstanceWrapper (className);

	prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

    pWaveManagedObject->prepareForSerialization ();

    return (pWaveManagedObject);

}

ResourceId WaveManagedObjectCreateWorker::updateRelationship (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext, const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId)
{
    // This function should be used when parent MO is owned by this currently working object manager.
    // create new message to send to other node
    // put all the field in the message, need to create message in such a way that, receiver can use it for updating MO and backend.
    // Make sure we use nested transaction for updating parent MO. It means, when receiver received this message, Object Id of child is not commited in the database.

    // send parent attibute name, parent attribute value as received from WyserEa g/w which is the keyvalue of child (this MO).

    vector<string>     attributeNames            = pWaveManagedObjectCreateContext->getAttributeNames ();
    vector<string>     attributeValues           = pWaveManagedObjectCreateContext->getAttributeValues ();
    string             attributeValue;
    WaveObjectManager *pWaveObjectManager        = WaveObjectManager::getOwnerForManagedClass (parentClassName);

    WaveObjectManagerUpdateRelationshipMessage  *pWaveObjectManagerUpdateRelationshipMessage = new WaveObjectManagerUpdateRelationshipMessage (pWaveObjectManager->getServiceId ());
    
    pWaveObjectManagerUpdateRelationshipMessage->setRelationshipVariables (parentClassName, childClassName, relationshipName, parentObjectId, childObjectId);

    pWaveObjectManagerUpdateRelationshipMessage->setIsAddingRelationship (true);

    WaveMessageStatus status = sendSynchronously (pWaveObjectManagerUpdateRelationshipMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (status);
    }
    else
    {
        return (pWaveObjectManagerUpdateRelationshipMessage->getCompletionStatus ());
    }
}

void WaveManagedObjectCreateWorker::commitWaveManagedObjectBeforeUpdatingBackend (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::commitWaveManagedObjectBeforeUpdatingBackend : Entering ...");    
 
    if (!pWaveManagedObjectCreateContext->getIsDatabaseUpdateRequired())
    {
        trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::commitWaveManagedObjectBeforeUpdatingBackend skipping for  Local Service Global MO");
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }

    WaveManagedObject *pWaveManagedObject        = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();
    vector<Attribute*> attributes                = pWaveManagedObjectCreateContext->getAttributes ();

    bool updateHardwareBeforeManagedObject       = pWaveManagedObject->updateHardwareBeforeWaveManagedObject (attributes);

    if (false == updateHardwareBeforeManagedObject)
    {
        prismLinearSequencerCommitTransactionStep (pWaveManagedObjectCreateContext);
    }
    else
    {
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

}

void WaveManagedObjectCreateWorker::commitWaveManagedObjectAfterUpdatingBackend (WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::commitWaveManagedObjectAfterUpdatingBackend : Entering ...");

    if (!pWaveManagedObjectCreateContext->getIsDatabaseUpdateRequired())
    {
        trace (TRACE_LEVEL_DEVEL , "WaveManagedObjectCreateWorker::commitWaveManagedObjectAfterUpdatingBackendskipping for Local Service & Global MO");
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        return;
    }  
        
    WaveManagedObject *pWaveManagedObject        = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject ();
    vector<Attribute*> attributes                = pWaveManagedObjectCreateContext->getAttributes ();

    bool updateHardwareBeforeManagedObject       = pWaveManagedObject->updateHardwareBeforeWaveManagedObject (attributes);

    if (true == updateHardwareBeforeManagedObject)
    {
        prismLinearSequencerCommitTransactionStep (pWaveManagedObjectCreateContext);
    }
    else
    {
        pWaveManagedObjectCreateContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

}

void WaveManagedObjectCreateWorker::populateParentKeyAttributes (GetHardwareConfigurationDetailsForCreateContext *pGetHardwareConfigurationDetailsForCreateContext)
{
    trace (TRACE_LEVEL_DEVEL, "WaveManagedObjectCreateWorker::populateParentKeyAttributes : Entering ...");
    
    vector<string>              parentMONames;
    vector<WaveManagedObject *> parentMO;

    WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pGetHardwareConfigurationDetailsForCreateContext->getPCallerContext ());

    WaveManagedObject   *pOperateOnWaveManagedObject = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject   ();

    // Update the parent key attributes to be sent to the backend.
    while (ObjectId::NullObjectId != pOperateOnWaveManagedObject->getOwnerManagedObjectId())
    {       
        //Get Parent Managed Object
        pOperateOnWaveManagedObject = queryManagedObject (pOperateOnWaveManagedObject->getOwnerManagedObjectId());
        //add Parent MO Name into the vector.
        parentMONames.push_back(pOperateOnWaveManagedObject->getObjectClassName());
        parentMO.push_back(pOperateOnWaveManagedObject);
            
        vector<string>            keyNames;
        vector <string>::iterator keyElement;
        //Get Key Names for the Managed Class.
        OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (pOperateOnWaveManagedObject->getObjectClassName (), keyNames);
            
        for (keyElement = keyNames.begin (); keyElement != keyNames.end (); keyElement++)
        {   
            Attribute *attr =  pOperateOnWaveManagedObject->getAttributeByName(*keyElement);
            
            prismAssert (NULL != attr, __FILE__, __LINE__);
 
            Attribute *pClonedAttribute = attr->clone ();
 
            prismAssert (NULL != pClonedAttribute, __FILE__, __LINE__);
 
            //add attrivute value to the vector.
            pGetHardwareConfigurationDetailsForCreateContext->addToAttributeVector (pClonedAttribute);
        }
    }
    pGetHardwareConfigurationDetailsForCreateContext->setParentManagedObjectNames (parentMONames); 
    pWaveManagedObjectCreateContext->addManagedObjectsForGarbageCollection (parentMO); 
}

void WaveManagedObjectCreateWorker::populateKeyAttributesIfNotPresent (GetHardwareConfigurationDetailsForCreateContext *pGetHardwareConfigurationDetailsForCreateContext)
{
    
    vector <Attribute *> attributeVector;
    bool                 attributeFound              = false;
    
    trace (TRACE_LEVEL_DEBUG, string("WaveManagedObjectCreateWorker::populateKeyAttributesIfNotPresent entering ........................ ") );

    // This is to check if this is a call for child MO creation which is compose in of parent MO. In such case where a child MO has to be created the child MO will be set in the 
    //GetHardwareConfigurationDetailsForCreateContext and can be retrived form there. In case it is not iw will return NULL and we can fall back to Parent MO creation logic.  
    WaveManagedObject   *pOperateOnWaveManagedObject = pGetHardwareConfigurationDetailsForCreateContext->getPOperateOnWaveManagedObject   ();

    if (NULL == pOperateOnWaveManagedObject)
    {
        WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pGetHardwareConfigurationDetailsForCreateContext->getPCallerContext ());
        pOperateOnWaveManagedObject = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject   ();
        attributeVector             = pWaveManagedObjectCreateContext->getAttributes ();
    }
    
    // Update the parent key attributes to be sent to the backend.

    vector <string> keyNames;
    vector <string>::iterator keyElement;

    OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (pOperateOnWaveManagedObject->getObjectClassName (), keyNames);

    for (keyElement = keyNames.begin (); keyElement != keyNames.end (); keyElement++)
    {
        trace (TRACE_LEVEL_DEVEL, string("WaveManagedObjectCreateWorker::populateKeyAttributesIfNotPresent keyelement = ")+ *keyElement);
        for (UI32 attributeVectorIterator = 0; attributeVectorIterator < attributeVector.size (); attributeVectorIterator++)
        {
            if (*keyElement == (attributeVector [attributeVectorIterator])->getAttributeName ())
            {
                attributeFound = true;
                break;
            }
        }

        if (!attributeFound)
        {

            Attribute* pKeyAttribute = pOperateOnWaveManagedObject->getAttributeByName (*keyElement);

            if(pKeyAttribute == NULL)
            {
                trace (TRACE_LEVEL_FATAL, string("WaveManagedObjectCreateWorker::populateKeyAttributesIfNotPresent pKeyAttribute is null ")+ *keyElement);
                prismAssert (false, __FILE__, __LINE__);
            }    


             Attribute *pClonedAttribute = NULL;

             if ((pKeyAttribute->getAttributeType () == AttributeType::AttributeTypeObjectId) || (pKeyAttribute->getAttributeType () == AttributeType::AttributeTypeObjectIdVector))
             {
                    /* Get the class name of the associated object */
         
                    AttributeObjectIdAssociation *pAttributeObjectIdAssociation = dynamic_cast<AttributeObjectIdAssociation *> (pKeyAttribute);
                    string                        associatedClassName           = pAttributeObjectIdAssociation->getAssociatedTo ();
                    map<string, Attribute*>       associatedKeyNamesAndTypesMap;
                    vector <string>               associatedKeyNames;
                    Attribute                    *pResultAttribute;
         
                    /* Get the Keys (or combination thereof) used for this class */
         
                    associatedKeyNamesAndTypesMap = OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (associatedClassName, associatedKeyNames);
                    pResultAttribute = associatedKeyNamesAndTypesMap[associatedKeyNames[0]];
         
                    Attribute *pAssociatedAttribute = Attribute::getAttributeFromAttributeType((pResultAttribute->getAttributeType()).getAttributeTypeResourceId());
 
                    /* We are only supporting MOs with a single primary key */
                    pAssociatedAttribute->setAttributeName(pResultAttribute->getAttributeName());
         
                    prismAssert (associatedKeyNames.size () == 1, __FILE__, __LINE__);
 
                    pClonedAttribute = pAssociatedAttribute->clone ();
             }
             else
             {
                pClonedAttribute = pKeyAttribute->clone ();
             }
                
            prismAssert (NULL != pClonedAttribute, __FILE__, __LINE__);

            pGetHardwareConfigurationDetailsForCreateContext->addToAttributeVector (pClonedAttribute);
        }

    }


}

string WaveManagedObjectCreateWorker::getClientNameToSendAndPopulateRelatedAttributes (GetHardwareConfigurationDetailsForCreateContext *pGetHardwareConfigurationDetailsForCreateContext)
{

    WaveManagedObject   *pOperateOnWaveManagedObject = pGetHardwareConfigurationDetailsForCreateContext->getPOperateOnWaveManagedObject   ();

    if (NULL == pOperateOnWaveManagedObject)
    {   
        WaveManagedObjectCreateContext *pWaveManagedObjectCreateContext = reinterpret_cast<WaveManagedObjectCreateContext *> (pGetHardwareConfigurationDetailsForCreateContext->getPCallerContext ());
        pOperateOnWaveManagedObject = pWaveManagedObjectCreateContext->getPOperateOnWaveManagedObject   ();
    }

    map <string, UI32>              attributeNamesGroupCodeMap      = pGetHardwareConfigurationDetailsForCreateContext->getAttributeNamesGroupCodeMap ();
    map <UI32, string>              groupCodeBackendNamesMap        = pGetHardwareConfigurationDetailsForCreateContext->getGroupCodeBackendNamesMap ();
    map <string, string>            attributeNameBackendNameMap     = pGetHardwareConfigurationDetailsForCreateContext->getAttributeNameBackendNameMap ();
    vector <Attribute *>            attributeVector                 = pGetHardwareConfigurationDetailsForCreateContext->getAttributes ();
    vector<string>                  attributeNames                  = pGetHardwareConfigurationDetailsForCreateContext->getAttributeNames();
    vector <string>                 attributeNamesGroup;
    UI32                            groupCode                       = 0;
    bool                            attributeFound                  = false;
    string                          clientName;
    string                          attributeName;

    //Get the last attribute in the vector
    if (0 < attributeNames.size ())
    {
        if (!(attributeNames.back ()).empty ())
        {
            attributeName = attributeNames.back ();
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string("WaveManagedObjectCreateWorker::getClientNameToSendAndPopulateRelatedAttributes: Could not find any element in the attributeNames vector"));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        prismAssert (0 != attributeVector.size (), __FILE__, __LINE__);
        attributeName = ((attributeVector) [0])->getAttributeName ();
    }

    trace (TRACE_LEVEL_DEVEL, string("WaveManagedObjectCreateWorker::getClientNameToSendAndPopulateRelatedAttributes: AttributeName = ") + attributeName     );

    /*//Find the backend Name corresponding to the attribute
    //If we find an entry in this map it indicates that the plugin does not want grouping and hence we will only get the client Name and return
    if (attributeNameBackendNameMap.end () != attributeNameBackendNameMap.find (attributeName))
    {
        clientName = attributeNameBackendNameMap [attributeName];
        trace (TRACE_LEVEL_DEVEL, string("WaveManagedObjectCreateWorker::getClientNameToSendAndPopulateRelatedAttributes: ClientName = ") + clientName);

        //set the groupCode to default value
        pGetHardwareConfigurationDetailsForCreateContext->setGroupCode (0);

        return (clientName);
    }*/

    //Find the groupCode corresponding to the attribute in the map
    if (attributeNamesGroupCodeMap.end () != attributeNamesGroupCodeMap.find (attributeName))
    {

        groupCode = attributeNamesGroupCodeMap [attributeName];

        trace (TRACE_LEVEL_DEVEL, string("WaveManagedObjectCreateWorker::getClientNameToSendAndPopulateRelatedAttributes: groupCode = ") + groupCode);

        //set the groupCode
        pGetHardwareConfigurationDetailsForCreateContext->setGroupCode (groupCode);

    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string("WaveManagedObjectCreateWorker::getClientNameToSendAndPopulateRelatedAttributes: Could not find the groupCode corresponding to the attributeName = ") + attributeName);
        prismAssert (false, __FILE__, __LINE__);
    }

    //Check if the attribute names in the group are already present in the vector of Attributes, which indicates those attributes are modified
    //For the attributes which are not found in the vector of Attributes, populate from MO queried from the DB.

    for (UI32 attributeNamesGroupIterator = 0; attributeNamesGroupIterator < attributeNamesGroup.size (); attributeNamesGroupIterator++)
    {
        for (UI32 attributeVectorIterator = 0; attributeVectorIterator < attributeVector.size (); attributeVectorIterator++)
        {
            if (attributeNamesGroup [attributeNamesGroupIterator] == (attributeVector [attributeVectorIterator])->getAttributeName ())
            {
                attributeFound = true;
                break;
            }
        }

        if (!attributeFound)
        {
            Attribute* pAttribute = pOperateOnWaveManagedObject->getAttributeByName (attributeNamesGroup [attributeNamesGroupIterator]);

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            Attribute *pClonedAttribute = pAttribute->clone ();

            prismAssert (NULL != pClonedAttribute, __FILE__, __LINE__);

            pGetHardwareConfigurationDetailsForCreateContext->addToAttributeVector (pClonedAttribute);

        }
        else
        {
            attributeFound = false;
        }
    }

    //Return the client name to send based on the groupCode.
    if (groupCodeBackendNamesMap.end () != groupCodeBackendNamesMap.find (groupCode))
    {
        clientName = groupCodeBackendNamesMap [groupCode];
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string("WaveManagedObjectCreateWorker::getClientNameToSendAndPopulateRelatedAttributes: Could not find the backendName corresponding to the groupCode = ") + groupCode);
        prismAssert (false, __FILE__, __LINE__);
    }

    return (clientName);
}

}
