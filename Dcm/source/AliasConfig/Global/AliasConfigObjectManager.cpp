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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : cgangwar                                                     *
 ***************************************************************************/

#include "AliasConfig/Global/AliasConfigObjectManager.h"
#include "AliasConfig/Global/WyserCliCommandAliasManagedObject.h"
#include "AliasConfig/Global/WyserEaCliCommandAliasMessage.h"
#include "AliasConfig/Global/WyserEaAliasUserMessage.h"
#include "AliasConfig/Global/WyserAliasUserManagedObject.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Messaging/Local/PrismEvent.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

using namespace WaveNs;

namespace DcmNs
{
const UI32 AliasConfigObjectManager::m_cleanCacheTimerInterval     = 4000;

AliasConfigObjectManager::AliasConfigObjectManager ()
    : WaveObjectManager (getClassName ())
{
    WyserAliasUserManagedObject wyserAliasUserManagedObject (this);
    wyserAliasUserManagedObject.setupOrm ();
    addManagedClass (WyserAliasUserManagedObject::getClassName ());
             
    addOperationMap (WYSEREA_ALIAS_USER, reinterpret_cast<PrismMessageHandler> (&AliasConfigObjectManager::WyserEaUserAliasMessageHandler));
    
    WyserCliCommandAliasManagedObject wyserCliCommandAliasManagedObject (this);
    wyserCliCommandAliasManagedObject.setupOrm ();
    addManagedClass (WyserCliCommandAliasManagedObject::getClassName ());

    addOperationMap (WYSEREA_ALIAS_COMMAND, reinterpret_cast<PrismMessageHandler> (&AliasConfigObjectManager::WyserEaCliCommandAliasMessageHandler));

    WyserEaCliCommandGlobalAliasManagedObject wyserCliCommandGlobalAliasManagedObject (this);
    wyserCliCommandGlobalAliasManagedObject.setupOrm ();
    addManagedClass (WyserEaCliCommandGlobalAliasManagedObject::getClassName ());
 
    addOperationMap (WYSEREA_GLOBAL_ALIAS_COMMAND, reinterpret_cast<PrismMessageHandler> (&AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMessageHandler));

    m_cleanCacheTimerHandle = 0;
    m_startTimer = false;
}

AliasConfigObjectManager::~AliasConfigObjectManager ()
{
}

string AliasConfigObjectManager::getClassName ()
{
    return ("AliasConfigObjectManager");
}

AliasConfigObjectManager *AliasConfigObjectManager::getInstance ()
{
    static AliasConfigObjectManager *pAliasConfigObjectManager = new AliasConfigObjectManager ();

    WaveNs::prismAssert (NULL != pAliasConfigObjectManager, __FILE__, __LINE__);

    return (pAliasConfigObjectManager);
}

PrismServiceId AliasConfigObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

WaveManagedObject *AliasConfigObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if (WyserCliCommandAliasManagedObject::getClassName() == managedClassName)
    {
        pWaveManagedObject = new WyserCliCommandAliasManagedObject(this);
    }
    else if (WyserAliasUserManagedObject::getClassName() == managedClassName)
    {
        pWaveManagedObject = new WyserAliasUserManagedObject(this);
    }
    else if (WyserEaCliCommandGlobalAliasManagedObject::getClassName() == managedClassName)
    {
        pWaveManagedObject = new WyserEaCliCommandGlobalAliasManagedObject(this);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::createManagedObjectInstance : unsupported managed class name : " + managedClassName);
    }
    return pWaveManagedObject;
}


PrismMessage *AliasConfigObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case  WYSEREA_ALIAS_COMMAND :
            pPrismMessage = new WyserEaCliCommandAliasMessage(WYSEREA_ALIAS_COMMAND);
            break;
        
        case  WYSEREA_ALIAS_USER :
           pPrismMessage = new WyserEaAliasUserMessage();
           break;

        case  WYSEREA_GLOBAL_ALIAS_COMMAND:
            pPrismMessage = new WyserEaCliCommandAliasMessage(WYSEREA_GLOBAL_ALIAS_COMMAND);
            break;

        default :
           pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

// handler.
void AliasConfigObjectManager::AliasValidationStep (PrismLinearSequencerContext *ctx_p)
{
     ResourceId status = WAVE_MESSAGE_SUCCESS;
     string managedObjectName;

     WyserEaCliCommandAliasMessage *msg_p = dynamic_cast<WyserEaCliCommandAliasMessage *>(ctx_p->getPPrismMessage ());
     prismAssert (NULL != msg_p, __FILE__, __LINE__);

     if (ALIAS_IS_GLOBAL_OPERATION == msg_p->getAliasType())
     {
        managedObjectName = WyserEaCliCommandGlobalAliasManagedObject::getClassName();
     }
     else
     {
        managedObjectName = WyserCliCommandAliasManagedObject::getClassName();
     }

     if (ALIAS_CREATE_ALIAS == (msg_p->getCmdCode()))
     {
        UI32       count  = 0;     

        WaveManagedObjectSynchronousQueryContext syncQueryCtxtForCount (managedObjectName);

        status = querySynchronouslyForCount (&syncQueryCtxtForCount, count);     

        if (4000 == count)
        {
            ctx_p->executeNextStep (MAX_ALIAS_LIMIT);
            return;
        }
     }

     ctx_p->executeNextStep (status);
}

void AliasConfigObjectManager::WyserEaCliCommandAliasMOStep (PrismLinearSequencerContext *ctx_p)
{
    ResourceId statusForNextStep = WAVE_MESSAGE_ERROR;
    WyserEaCliCommandAliasMessage *msg_p = dynamic_cast<WyserEaCliCommandAliasMessage *>(ctx_p->getPPrismMessage ());
    prismAssert (NULL != msg_p, __FILE__, __LINE__);

    m_startTimer = msg_p->getStartTimer();
    m_aliasType = msg_p->getAliasType();

    WyserAliasUserManagedObject         *UserMO = NULL;
    WyserCliCommandAliasManagedObject   *AliasMO = NULL;
    
    string className = WyserAliasUserManagedObject::getClassName();
    string childClassName = WyserCliCommandAliasManagedObject::getClassName();
    string compositionName = "useralias";           
    
    // Query Database for username
    WaveManagedObjectSynchronousQueryContext syncQueryCtxtU(WyserAliasUserManagedObject::getClassName());
    syncQueryCtxtU.addAndAttribute (new AttributeString (msg_p->getusername(), "username"));
    syncQueryCtxtU.setLoadCompositions(false);
    vector<WaveManagedObject *> *pResultsU = querySynchronously (&syncQueryCtxtU);
    
    switch (msg_p->getCmdCode())
    {
        case ALIAS_CREATE_ALIAS:
        {
            if (NULL == pResultsU)
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Query for WyserEaUserAlias MO returned NULL objects for CreateAlias");
                prismAssert (false, __FILE__, __LINE__);
            }
            else if(1 == pResultsU->size()) //if username is present
            {
                UserMO = dynamic_cast<WyserAliasUserManagedObject *>((*pResultsU)[0]);

                if(NULL == UserMO)
                {
                    trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : dynamic cast to WyserAliasUserManagedObject failed. System failure.");
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    // Query database for aliasname for a particular user.
                    trace (TRACE_LEVEL_DEVEL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : dynamic cast to WyserAliasUserManagedObject Success.");
                    
                    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(WyserCliCommandAliasManagedObject::getClassName());
                    syncQueryCtxt.addAndAttribute (new AttributeString (msg_p->getaliasname(), "aliasname"));
                    syncQueryCtxt.addAndAttribute (new AttributeObjectId(UserMO->getObjectId(), "ownerManagedObjectId"));
                    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

                    ctx_p->addManagedObjectsForGarbageCollection (*pResults);

                    if (NULL == pResults)
                    {
                        trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Query for WyserEaAliasCommand MO returned NULL objects for CreateAlias");
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    else if (0 == pResults->size()) //if alianame not found, create anew one.
                    {
                        AliasMO = new WyserCliCommandAliasManagedObject(AliasConfigObjectManager::getInstance ());
                    
                        if (!((msg_p->getaliasname()).empty()))
                        {
                            AliasMO->setaliasname(msg_p->getaliasname());
                        }
                        if (!((msg_p->getaliasexpansion()).empty()))
                        {
                            AliasMO->setaliasexpansion(msg_p->getaliasexpansion());
                        }
                    
                        AliasMO->setOwnerManagedObjectId(UserMO->getObjectId());
                        addToComposition(className, childClassName,compositionName, UserMO->getObjectId(), AliasMO->getObjectId());
                        statusForNextStep = WAVE_MESSAGE_SUCCESS;

                        ctx_p->addManagedObjectForGarbageCollection (AliasMO);
                    }
                    else if (1 == pResults->size()) //if aliasname found.
                    {
                        //if call is from operation mode, thn update the existing alias.
                        if (msg_p->getOperationMode() == ALIAS_FROM_OPERATIONAL_MODE)
                        {
                            AliasMO = dynamic_cast<WyserCliCommandAliasManagedObject *>((*pResults)[0]);
                     
                            if (NULL == AliasMO)
                            {
                                trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : dynamic cast to WyserCliCommandAliasManagedObject failed. System failure.");
                                prismAssert (false, __FILE__, __LINE__);
                            }
                            else
                            {
                                if (!((msg_p->getaliasname()).empty()))
                                {
                                    AliasMO->setaliasname(msg_p->getaliasname());
                                }
                                if (!((msg_p->getaliasexpansion()).empty()))
                                {
                                    AliasMO->setaliasexpansion(msg_p->getaliasexpansion());
                                }
                            }
                            updateWaveManagedObject (AliasMO);
                            statusForNextStep = WAVE_MESSAGE_SUCCESS;
                        }
                        else //otherwise call is from configure mode, return error.
                        {
                            trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Query for WyserEaAliasCommand return a similar alias");
                            statusForNextStep = DUPLICATE_ALIAS_FOUND;
                        }
                    }
                    else
                    {
                        tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : number of WyserCliCommandAliasManagedObject [%u]. Only one Managed Object Expected.",  pResults->size());
                        prismAssert (false, __FILE__, __LINE__);    
                    }
                    if(NULL != pResults)
                    {
                        delete pResults;        
                    }
                 }
             }
             else if (0 == pResultsU->size()) // if no username found.
             {
                // if call is from operationmode, create the user and then create alias.     
                if (msg_p->getOperationMode() == ALIAS_FROM_OPERATIONAL_MODE)
                {
                    UI32       count  = 0;
 
                    WaveManagedObjectSynchronousQueryContext syncQueryCtxtForCount (WyserAliasUserManagedObject::getClassName());
 
                    statusForNextStep = querySynchronouslyForCount (&syncQueryCtxtForCount, count);
 
                    if (4000 == count)
                    {
                        ctx_p->executeNextStep (MAX_USER_LIMIT);
                        return;
                    }
    
                    UserMO = new WyserAliasUserManagedObject(AliasConfigObjectManager::getInstance ());
          
                    if (NULL == UserMO)
                    {
                        trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : dynamic cast to WyserAliasUserManagedObject failed. System failure.");
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    else
                    {
                        trace (TRACE_LEVEL_DEVEL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : dynamic cast to WyserAliasUserManagedObject Success.");
                        if (!((msg_p->getusername()).empty()))
                        {
                            UserMO->setusername(msg_p->getusername());
                        }             
                    }
                    
                    //Create Alias entry    
                    AliasMO = new WyserCliCommandAliasManagedObject(AliasConfigObjectManager::getInstance ());
                                                                   
                    if (!((msg_p->getaliasname()).empty()))
                    {
                        AliasMO->setaliasname(msg_p->getaliasname());
                    }

                    if (!((msg_p->getaliasexpansion()).empty()))
                    {
                        AliasMO->setaliasexpansion(msg_p->getaliasexpansion());
                    }

                    AliasMO->setOwnerManagedObjectId(UserMO->getObjectId());
                    addToComposition(className, childClassName,compositionName, UserMO->getObjectId(), AliasMO->getObjectId());
                    statusForNextStep = WAVE_MESSAGE_SUCCESS;

                    ctx_p->addManagedObjectForGarbageCollection (AliasMO);
                }
                else
                {     
                    trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : WyserEaAliasCommand MO not found!");
                }
             }
             else
             {
                tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : number of WyserCliCommandAliasManagedObject [%u]. Only one Managed Object Expected.",  pResultsU->size());
                prismAssert (false, __FILE__, __LINE__);
             }
             
             if (NULL != pResultsU)
             {
                if (UserMO != NULL)
                {
                    ctx_p->addManagedObjectForGarbageCollection (UserMO);     
                }
                delete pResultsU; 
             }
        }
        break;
        
        case ALIAS_REMOVE_ALIAS:
        {

            if (NULL == pResultsU)
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Query for WyserEaAliasUser MO returned NULL objects for RemoveAlias");
                prismAssert (false, __FILE__, __LINE__);                
            }
            else if (1 == pResultsU->size())
            {

                UserMO = dynamic_cast<WyserAliasUserManagedObject *>((*pResultsU)[0]);
                
                if(NULL == UserMO)
                {       
                    trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : dynamic cast to WyserAliasUserManagedObject failed. System failure.");
                    prismAssert (false, __FILE__, __LINE__);
                }          
                else
                {
                    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(WyserCliCommandAliasManagedObject::getClassName());
                    syncQueryCtxt.addAndAttribute (new AttributeString (msg_p->getaliasname(), "aliasname"));
                    syncQueryCtxt.addAndAttribute (new AttributeObjectId(UserMO->getObjectId(), "ownerManagedObjectId"));        
                    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
                  
                    ctx_p->addManagedObjectsForGarbageCollection (*pResults);

                    if (NULL == pResults)
                    {
                        trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Query for WyserEaAliasCommand MO returned NULL objects for RemoveAlias");
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    else if (1 == pResults->size())
                    {
                        AliasMO = dynamic_cast<WyserCliCommandAliasManagedObject *>((*pResults)[0]);
                        
                        if (NULL == AliasMO)
                        {
                            trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : dynamic cast to WyserCliCommandAliasManagedObject failed. System failure.");
                            prismAssert (false, __FILE__, __LINE__);
                        }
                        else
                        {
                            deleteFromComposition(className, childClassName,compositionName, UserMO->getObjectId(), AliasMO->getObjectId());
                            statusForNextStep = WAVE_MESSAGE_SUCCESS;
                        }
                    }
                    else if (0 == pResults->size())
                    {
                        trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : WyserEaAliasCommand MO not found!");
                        statusForNextStep = NO_ALIAS_FOUND;
                    }
                    else
                    {
                        tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : number of WyserCliCommandAliasManagedObject [%u]. Only one Managed Object Expected.",  pResults->size());
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    if (NULL != pResults)
                    {
                        delete (pResults);        
                    }
                }
            }  
            else if (0 == pResultsU->size())
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : WyserEaAliasUser MO not found!");
                statusForNextStep = NO_ALIAS_USER_FOUND;
            }
            else
            {
                tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaAliasUser MOStep : number of WyserAliasUserManagedObject  [%u]. Only one Managed Object Expected.",  pResultsU->size());
                prismAssert (false, __FILE__, __LINE__);
            }

            // if deleting last alias entry, remove the corresponding user entry.
            
            UI32 count = 0;

            if (0 != pResultsU->size())
            {
                WaveManagedObjectSynchronousQueryContext syncQueryCtxtR(WyserCliCommandAliasManagedObject::getClassName());
                syncQueryCtxtR.addAndAttribute (new AttributeObjectId(UserMO->getObjectId(), "ownerManagedObjectId"));
                ResourceId queryStatus = querySynchronouslyForCount (&syncQueryCtxtR, count);       

                if (WAVE_MESSAGE_SUCCESS != queryStatus)
                {
                    trace (TRACE_LEVEL_ERROR,"AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Query Failed Reason : " + FrameworkToolKit::localize (queryStatus));        
                }
                if (1 == count)
                {
                    delete (UserMO);       
                }
                else
                {
                    ctx_p->addManagedObjectForGarbageCollection (UserMO);        
                }
            }
            
            if(NULL != pResultsU)
            {        
                delete pResultsU;
            }
        }
        break;

        case ALIAS_UPDATE_ALIAS:
        {
            if (NULL == pResultsU)
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Query for WyserEaAliasUser MO returned NULL objects for UpdateAlias");
                prismAssert (false, __FILE__, __LINE__);                
            }
            else if (1 == pResultsU->size())
            {
                UserMO = dynamic_cast<WyserAliasUserManagedObject *>((*pResultsU)[0]);
                if(NULL == UserMO)
                {
                    trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : dynamic cast to WyserAliasUserManagedObject failed. System failure.");
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(WyserCliCommandAliasManagedObject::getClassName());
                    syncQueryCtxt.addAndAttribute (new AttributeString (msg_p->getaliasname(), "aliasname"));
                    syncQueryCtxt.addAndAttribute (new AttributeObjectId(UserMO->getObjectId(), "ownerManagedObjectId"));        
                    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
                        
                    if (NULL == pResults)
                    {
                        trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Query for WyserEaAliasCommand MO returned NULL objects for UpdateAlias");
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    else if (1 == pResults->size())
                    {
                        AliasMO = dynamic_cast<WyserCliCommandAliasManagedObject *>((*pResults)[0]);
                            
                        if (NULL == AliasMO)
                        {
                            trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : dynamic cast to WyserCliCommandAliasManagedObject failed. System failure.");
                            prismAssert (false, __FILE__, __LINE__);
                        }
                        else
                        {
                            if (!((msg_p->getaliasname()).empty()))
                            {
                                AliasMO->setaliasname(msg_p->getaliasname());
                            }

                            if (!((msg_p->getaliasexpansion()).empty()))
                            {
                                AliasMO->setaliasexpansion(msg_p->getaliasexpansion());
                            }    
                                
                            updateWaveManagedObject (AliasMO);
                            statusForNextStep = WAVE_MESSAGE_SUCCESS;
                        }
                    }
                
                    else if (0 == pResults->size())
                    {
                        trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : WyserEaAliasCommand MO not found!");
                        statusForNextStep = NO_ALIAS_FOUND;
                    }
                    else    
                    {
                        tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : number of WyserCliCommandAliasManagedObject [%u]. Only one Managed Object Expected.",  pResults->size());
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    
                    ctx_p->addManagedObjectForGarbageCollection (AliasMO);
                    
                    if (NULL != pResults)
                    {
                        delete pResults;
                    }
                }
            } 
            else if (0 == pResultsU->size())
            {
                trace (TRACE_LEVEL_DEVEL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : WyserEaAliasUser MO not found!");
            }
            else
            {
                tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaAliasUser MOStep : number of WyserAliasUserManagedObject  [%u]. Only one Managed Object Expected.",  pResultsU->size());
                prismAssert (false, __FILE__, __LINE__);
            }

            if(NULL != pResultsU)
            {
                if (NULL != UserMO)
                {
                    ctx_p->addManagedObjectForGarbageCollection (UserMO);    
                }
                delete pResultsU;    
            }
        } 
        break;

        default :
        {
            tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Unknown commandCode [%u]", msg_p->getCmdCode());
            prismAssert (false, __FILE__,__LINE__);
        }
        break;
    }
    ctx_p->executeNextStep (statusForNextStep);
}


void AliasConfigObjectManager::WyserEaCliCommandAliasMessageHandler(WyserEaCliCommandAliasMessage *msg_p)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::AliasValidationStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::WyserEaCliCommandAliasMOStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::AAACleanCacheTimerStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerFailedStep)
    };
    
    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (msg_p , this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
    pPrismLinearSequencerContext->start ();
}

void AliasConfigObjectManager::UserValidationStep (PrismLinearSequencerContext *ctx_p)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    WyserEaAliasUserMessage *msg_p = dynamic_cast<WyserEaAliasUserMessage *>(ctx_p->getPPrismMessage ());
    prismAssert (NULL != msg_p, __FILE__, __LINE__);
 
    if (ALIAS_CREATE_USER == (msg_p->getCmdCode()))
    {
        UI32       count  = 0;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxtForCount (WyserAliasUserManagedObject::getClassName());
 
        status = querySynchronouslyForCount (&syncQueryCtxtForCount, count);
 
        if (4000 == count)
        {
            ctx_p->executeNextStep (MAX_USER_LIMIT);
            return;
        }
    }
    ctx_p->executeNextStep (status);
}

void AliasConfigObjectManager::WyserEaAliasUserMOStep (PrismLinearSequencerContext *ctx_p)
{
    ResourceId statusForNextStep = WAVE_MESSAGE_ERROR;
    
    WyserEaAliasUserMessage *msg_p = dynamic_cast<WyserEaAliasUserMessage *>(ctx_p->getPPrismMessage ());
    prismAssert (NULL != msg_p, __FILE__, __LINE__);

    m_startTimer = msg_p->getStartTimer(); 

    switch (msg_p->getCmdCode())
    {
        case ALIAS_CREATE_USER:
        {
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(WyserAliasUserManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute (new AttributeString (msg_p->getusername(), "username"));
            syncQueryCtxt.setLoadCompositions(false);
            vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

            if (NULL == pResults)
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaAliasUserMOStep : Query for WyserEaAliasUser MO returned NULL objects");
                prismAssert (false, __FILE__, __LINE__);
            }
            else if (0 == pResults->size())
            {
                WyserAliasUserManagedObject *mo_p = new WyserAliasUserManagedObject(AliasConfigObjectManager::getInstance ());
                    
                if (NULL == mo_p)
                {
                    trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaAliasUserMOStep : dynamic cast to WyserAliasUserManagedObject failed. System failure.");
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    trace (TRACE_LEVEL_DEVEL, "AliasConfigObjectManager::WyserEaAliasUserMOStep : dynamic cast to WyserAliasUserManagedObject Success.");
                    if (!((msg_p->getusername()).empty()))
                    {
                        mo_p->setusername(msg_p->getusername());
                    }

                    statusForNextStep = WAVE_MESSAGE_SUCCESS;
                }

                ctx_p->addManagedObjectForGarbageCollection (mo_p);
            }
            else if (1 == pResults->size())
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaAliasUserMOStep : Query for WyserEaAliasCommand return a similar alias");
                statusForNextStep =   DUPLICATE_ALIAS_FOUND;
            }
            else
            {
                tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaAliasUserMOStep : number of WyserAliasUserManagedObject [%u]. Only one Managed Object Expected.",  pResults->size());
                prismAssert (false, __FILE__, __LINE__);
            }

            if(NULL != pResults)
            {
                delete pResults;
            }
        }

        break;
    
        case ALIAS_REMOVE_USER:
        {
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(WyserAliasUserManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute (new AttributeString (msg_p->getusername(), "username"));
            vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
                    
            if (NULL == pResults)
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaAliasUserMOStep : Query for WyserEaAliasUser MO returned NULL objects");
                prismAssert (false, __FILE__, __LINE__);
            }
            else if (1 == pResults->size())
            {
                WyserAliasUserManagedObject *mo_p = dynamic_cast<WyserAliasUserManagedObject *>((*pResults)[0]);
                        
                if (NULL == mo_p)
                {
                    trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaAliasUser MOStep : dynamic cast to WyserAliasUserManagedObject failed. System failure.");
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    delete(mo_p);
                    statusForNextStep = WAVE_MESSAGE_SUCCESS;
                }
            }
            else if (0 == pResults->size())
            {
                trace (TRACE_LEVEL_DEVEL, "AliasConfigObjectManager::WyserEaAliasUserMOStep : WyserEaAliasUser MO not found!");
            }
            else
            {
                tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaAliasUser MOStep : number of WyserAliasUserManagedObject [%u]. Only one Managed Object Expected.",  pResults->size());
                prismAssert (false, __FILE__, __LINE__);
            }         

            delete(pResults);         
        }  

        break;
     
        default:
        {
            tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandAliasMOStep : Unknown commandCode [%u]", msg_p->getCmdCode());
            prismAssert (false, __FILE__,__LINE__);
        }

        break;
    }     

    ctx_p->executeNextStep (statusForNextStep);
}


void AliasConfigObjectManager::WyserEaUserAliasMessageHandler(WyserEaAliasUserMessage *msg_p)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::UserValidationStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::WyserEaAliasUserMOStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::AAACleanCacheTimerStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerFailedStep)
    };
    
    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (msg_p , this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
    pPrismLinearSequencerContext->start ();
}


void AliasConfigObjectManager::AaaCacheReload (TimerHandle &timerHandle)
{
    trace (TRACE_LEVEL_DEVEL, "AliasConfigObjectManager::AaaCacheReload entering ..");
    
    LocationId                  thisLocationId       = FrameworkToolKit::getThisLocationId();
    vector<LocationId>          connectedLocationIds; 

    FrameworkToolKit::getConnectedLocations (connectedLocationIds);
    connectedLocationIds.push_back(thisLocationId);

    trace (TRACE_LEVEL_DEVEL, string("AliasConfigObjectManager::AaaCacheReload : connectedLocationIds size = ")+connectedLocationIds.size());

    for (unsigned int i = 0; i < connectedLocationIds.size(); i++)
    {
        trace (TRACE_LEVEL_DEVEL, string("AliasConfigObjectManager::AaaCacheReload sending To Location Id = ")+connectedLocationIds[i]);
    }
}


void AliasConfigObjectManager::AAACleanCacheTimerStep(PrismLinearSequencerContext *ctx_p)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, "AliasConfigObjectManager::AAACleanCacheTimerStep");
    
    if(m_startTimer == true)
    {
        if (m_cleanCacheTimerHandle != 0) 
        {
            deleteTimer (m_cleanCacheTimerHandle);
            m_cleanCacheTimerHandle = 0;
        }

        // Start the timer only if there is no timer running.
        startTimer (m_cleanCacheTimerHandle, m_cleanCacheTimerInterval, reinterpret_cast<PrismTimerExpirationHandler> (&AliasConfigObjectManager::AaaCacheReload));
    }
    else
    {
        if (m_cleanCacheTimerHandle != 0)
        {    
            deleteTimer (m_cleanCacheTimerHandle);    
            m_cleanCacheTimerHandle = 0;     
        }
        startTimer (m_cleanCacheTimerHandle, 1, reinterpret_cast<PrismTimerExpirationHandler> (&AliasConfigObjectManager::AaaCacheReload));       
    }
 
    
    ctx_p->executeNextStep (status);
}


void AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMessageHandler(WyserEaCliCommandAliasMessage *msg_p)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::AliasValidationStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::AAACleanCacheTimerStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep>(&AliasConfigObjectManager::prismLinearSequencerFailedStep)
    };
 
    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (msg_p , this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
    pPrismLinearSequencerContext->start ();
}


void AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep (PrismLinearSequencerContext *ctx_p)
{
    trace(TRACE_LEVEL_INFO,"AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep : entering ..");
    
    ResourceId statusForNextStep = WAVE_MESSAGE_ERROR;
    WyserEaCliCommandAliasMessage *msg_p = dynamic_cast<WyserEaCliCommandAliasMessage *>(ctx_p->getPPrismMessage ());
    prismAssert (NULL != msg_p, __FILE__, __LINE__);
 
    m_startTimer = msg_p->getStartTimer();
    m_aliasType = msg_p->getAliasType();
 
    WyserEaCliCommandGlobalAliasManagedObject   *AliasMO = NULL;
 
    switch (msg_p->getCmdCode())
    {
        case ALIAS_CREATE_ALIAS:
        {
            trace(TRACE_LEVEL_INFO,"AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep : Global Alias");
            
            AliasMO = new WyserEaCliCommandGlobalAliasManagedObject(AliasConfigObjectManager::getInstance ());

            if (!((msg_p->getaliasname()).empty()))
            {
                AliasMO->setaliasname(msg_p->getaliasname());
            }

            if (!((msg_p->getaliasexpansion()).empty()))
            {
                AliasMO->setaliasexpansion(msg_p->getaliasexpansion());
            }
 
            statusForNextStep = WAVE_MESSAGE_SUCCESS;

            ctx_p->addManagedObjectForGarbageCollection (AliasMO);        
        }
        break;
 
        case ALIAS_REMOVE_ALIAS:
        {
            WaveManagedObjectSynchronousQueryContext syncQueryCtxtG (WyserEaCliCommandGlobalAliasManagedObject::getClassName());
            syncQueryCtxtG.addAndAttribute (new AttributeString (msg_p->getaliasname(), "aliasname"));
            vector<WaveManagedObject *> *pResultsG = querySynchronously (&syncQueryCtxtG);

            if (NULL == pResultsG)
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep Global : Query for WyserEaGlobalAliasCommand MO returned NULL objects for RemoveAlias");
                prismAssert (false, __FILE__, __LINE__);
            }
            else if (1 == pResultsG->size())
            {
                AliasMO = dynamic_cast<WyserEaCliCommandGlobalAliasManagedObject *>((*pResultsG)[0]);

                if (NULL == AliasMO)
                {
                    trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep Global: dynamic cast to WyserEaCliCommandGlobalAliasManagedObject failed. System failure.");
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    delete (AliasMO);
                    statusForNextStep = WAVE_MESSAGE_SUCCESS;
                }
            }
            else if (0 == pResultsG->size())
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep Global : WyserEaGlobalAliasCommand MO not found!");
                statusForNextStep = NO_ALIAS_FOUND;
            }
            else
            {
                tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep Global : number of WyserEaCliCommandGlobalAliasManagedObject [%u]. Only one Managed Object Expected.",  pResultsG->size());
                prismAssert (false, __FILE__, __LINE__);
            }
            if (NULL != pResultsG)
            {
                delete (pResultsG);
            }         
        }
        break;
 
        case ALIAS_UPDATE_ALIAS:
        {
            WaveManagedObjectSynchronousQueryContext syncQueryCtxtG(WyserEaCliCommandGlobalAliasManagedObject::getClassName());
            syncQueryCtxtG.addAndAttribute (new AttributeString (msg_p->getaliasname(), "aliasname"));
            vector<WaveManagedObject *> *pResultsG = querySynchronously (&syncQueryCtxtG);

            if (NULL == pResultsG)
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep Global : Query for WyserEaGlobalAliasCommand MO returned NULL objects for UpdateAlias");
                prismAssert (false, __FILE__, __LINE__);
            }
            else if (1 == pResultsG->size())
            {
                AliasMO = dynamic_cast<WyserEaCliCommandGlobalAliasManagedObject *>((*pResultsG)[0]);

                if (NULL == AliasMO)
                {
                    trace (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep Global : dynamic cast to WyserEaCliCommandGlobalAliasManagedObject failed. System failure.");
                    prismAssert (false, __FILE__, __LINE__);
                }
                else
                {
                    if (!((msg_p->getaliasname()).empty()))
                    {
                        AliasMO->setaliasname(msg_p->getaliasname());
                    }

                    if (!((msg_p->getaliasexpansion()).empty()))
                    {
                        AliasMO->setaliasexpansion(msg_p->getaliasexpansion());
                    }

                    updateWaveManagedObject (AliasMO);
                    statusForNextStep = WAVE_MESSAGE_SUCCESS;
                }
            }
            else if (0 == pResultsG->size())
            {
                trace (TRACE_LEVEL_ERROR, "AliasConfigObjectManager::WyserEaCliCommandglobalAliasMOStep Global : WyserEaGlobalAliasCommand MO not found!");
                statusForNextStep = NO_ALIAS_FOUND;
            }
            else
            {
                tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep Global : number of WyserEaCliCommandGlobalAliasManagedObject [%u]. Only one Managed Object Expected.",  pResultsG->size());
                prismAssert (false, __FILE__, __LINE__);
            }
            ctx_p->addManagedObjectForGarbageCollection (AliasMO);
        }
        break;
 
        default :
        {
            tracePrintf (TRACE_LEVEL_FATAL, "AliasConfigObjectManager::WyserEaCliCommandGlobalAliasMOStep : Unknown commandCode [%u]", msg_p->getCmdCode());
            prismAssert (false, __FILE__,__LINE__);
        }
        break;
    }
    ctx_p->executeNextStep (statusForNextStep);
}

void AliasConfigObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (),
                        FRAMEWORK_OBJECT_MANAGER_NODES_ADDITION_TO_CLUSTER_COMPLETED_EVENT,
                        reinterpret_cast<PrismEventHandler> (&AliasConfigObjectManager::nodeAddedEventHandler)
            );

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();    
}

void AliasConfigObjectManager::nodeAddedEventHandler (const WaveNewNodesAddedEvent* &pEvent)
{
    trace(TRACE_LEVEL_INFO,"AliasConfigObjectManager::nodeAddedEventHandler .. ");
    
    LocationId                  thisLocationId       = FrameworkToolKit::getThisLocationId();
    vector<LocationId>          connectedLocationIds;
 
    FrameworkToolKit::getConnectedLocations (connectedLocationIds);
    connectedLocationIds.push_back(thisLocationId);
 
    for (unsigned int i = 0; i < connectedLocationIds.size(); i++)
    {
        if (connectedLocationIds[i] == FrameworkToolKit::getThisLocationId())
        {
            continue;
        }
        trace (TRACE_LEVEL_DEVEL, string("AliasConfigObjectManager::nodeAddedEventHandler sending To Location Id = ")+connectedLocationIds[i]);
    }
    
    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

}
