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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectUpdateContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

WaveManagedObjectUpdateContext::WaveManagedObjectUpdateContext (PrismMessage* pPrismMessage, PrismElement* pPrismElement, PrismLinearSequencerStep* pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pInputWaveManagedObject        (NULL),
      m_pOperateOnWaveManagedObject    (NULL),
      m_childUserTag                   (0),
      m_errorInCreatingMO              (WAVE_MESSAGE_SUCCESS),
      m_isConfigReplayInProgress       (false) 
{
}

WaveManagedObjectUpdateContext::WaveManagedObjectUpdateContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement* pPrismElement, PrismLinearSequencerStep* pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_pInputWaveManagedObject        (NULL),
      m_pOperateOnWaveManagedObject    (NULL),
      m_childUserTag                   (0),
      m_errorInCreatingMO              (WAVE_MESSAGE_SUCCESS),
      m_isConfigReplayInProgress       (false)  
{
}

WaveManagedObjectUpdateContext::~WaveManagedObjectUpdateContext ()
{
    UI32 attributeSize = m_attributes.size ();

    for (UI32 i = 0; i < attributeSize; i++)
    {
        if (NULL != m_attributes[i])
        {
            delete m_attributes[i];
        }
    }

    m_attributes.clear ();

    UI32 numberOfSynchronousContext = m_prismAsynchronousContext.size ();

    for (UI32 i = 0; i < numberOfSynchronousContext; i++)
    {
        PrismAsynchronousContext *pPrismAsynchronousContext = m_prismAsynchronousContext [i];

        if (NULL != pPrismAsynchronousContext)
        {
            delete pPrismAsynchronousContext;
            pPrismAsynchronousContext = NULL;
        }
    }

    m_prismAsynchronousContext.clear ();
}

ObjectId WaveManagedObjectUpdateContext::getInputWaveManagedObjectId () const
{
    return (m_inputWaveManagedObjectId);
}

void WaveManagedObjectUpdateContext::setInputWaveManagedObjectId (const ObjectId& waveManagedObjectId)
{
    m_inputWaveManagedObjectId = waveManagedObjectId;
}

ObjectId WaveManagedObjectUpdateContext::getOperateOnWaveManagedObjectId () const
{
    return (m_operateOnWaveManagedObjectId);
}

void WaveManagedObjectUpdateContext::setOperateOnWaveManagedObjectId (const ObjectId& waveManagedObjectId)
{
    m_operateOnWaveManagedObjectId = waveManagedObjectId;
}

WaveManagedObject*const WaveManagedObjectUpdateContext::getPInputWaveManagedObject () const
{
    return (m_pInputWaveManagedObject);
}

void WaveManagedObjectUpdateContext::setPInputWaveManagedObject (WaveManagedObject * const pWaveManagedObject)
{
    m_pInputWaveManagedObject = pWaveManagedObject;
}

WaveManagedObject*const WaveManagedObjectUpdateContext::getPOperateOnWaveManagedObject () const
{
    return (m_pOperateOnWaveManagedObject);
}

void WaveManagedObjectUpdateContext::setPOperateOnWaveManagedObject (WaveManagedObject * const pWaveManagedObject)
{
    m_pOperateOnWaveManagedObject = pWaveManagedObject;
}

vector<UI32> WaveManagedObjectUpdateContext::getAttributeUserTags () const
{
    return (m_attributeUserTags);
}

void WaveManagedObjectUpdateContext::setAttributeUserTags(const vector< UI32 >& attributeUserTags)
{
    m_attributeUserTags = attributeUserTags;
}

vector<string> WaveManagedObjectUpdateContext::getAttributeNames () const
{
    return (m_attributeNames);
}

void WaveManagedObjectUpdateContext::setAttributeNames (const vector<string> &attributeNames)
{

    m_attributeNames = attributeNames;
}

vector<string> WaveManagedObjectUpdateContext::getAttributeValues () const
{
    return (m_attributeValues);
}

void WaveManagedObjectUpdateContext::setAttributeValues(const vector< string >& attributeValues)
{
    m_attributeValues = attributeValues;
}

vector<Attribute *> WaveManagedObjectUpdateContext::getAttributes () const
{
    return (m_attributes);
}

void WaveManagedObjectUpdateContext::addToAttributeVector (Attribute *pAttribute)
{
    prismAssert (NULL != pAttribute, __FILE__, __LINE__);

    m_attributes.push_back (pAttribute); 
}

ObjectId WaveManagedObjectUpdateContext::getParentObjectId () const
{
    return (m_parentObjectId);
}

void WaveManagedObjectUpdateContext::setParentObjectId (const ObjectId &parentObjectId)
{
    m_parentObjectId = parentObjectId;
}

UI32 WaveManagedObjectUpdateContext::getChildUserTag () const
{
    return (m_childUserTag);
}

void WaveManagedObjectUpdateContext::setChildUserTag (const UI32 &childUserTag)
{
    m_childUserTag = childUserTag;
}

ResourceId WaveManagedObjectUpdateContext::getErrorInCreatingMO () const
{
    return (m_errorInCreatingMO);
}

void WaveManagedObjectUpdateContext::setErrorInCreatingMO (const ResourceId &errorInCreatingMO)
{
    m_errorInCreatingMO = errorInCreatingMO;
}

void WaveManagedObjectUpdateContext::addAysnchronousContextForGarbageCollection   (PrismAsynchronousContext *prismAsynchronousContext)
{
    m_prismAsynchronousContext.push_back (prismAsynchronousContext);
}

void WaveManagedObjectUpdateContext::addManagedObjectForGarbageCollection (WaveManagedObject *pWaveManagedObjectForGarbageCollection)
{
    prismAssert (NULL != pWaveManagedObjectForGarbageCollection, __FILE__, __LINE__);

    PrismAsynchronousContext *pPrismAsynchronousContext = getPPrismAsynchronousContext ();

    if (NULL != pPrismAsynchronousContext)
    {
        pPrismAsynchronousContext->addManagedObjectForGarbageCollection (pWaveManagedObjectForGarbageCollection);
    }
    else
    {
        PrismLinearSequencerContext::addManagedObjectForGarbageCollection (pWaveManagedObjectForGarbageCollection);
    }
}

void WaveManagedObjectUpdateContext::setIsDatabaseUpdateRequired (bool isDatabaseUpdateRequired)
{
    m_isDatabaseUpdateRequired = isDatabaseUpdateRequired;            
}

bool WaveManagedObjectUpdateContext::getIsDatabaseUpdateRequired ()
{
    return (m_isDatabaseUpdateRequired);        
}

void WaveManagedObjectUpdateContext::setIsBackendUpdateRequired (bool isBackendUpdateRequired)
{
    m_isBackendUpdateRequired = isBackendUpdateRequired;
}

bool WaveManagedObjectUpdateContext::getIsBackendUpdateRequired ()
{
    return (m_isBackendUpdateRequired);        
}

void WaveManagedObjectUpdateContext::setParentManagedObjectNames (vector<string> parentManagedObjectNames)
{
     m_parentManagedObjectNames = parentManagedObjectNames;
}
 
vector<string> WaveManagedObjectUpdateContext::getParentManagedObjectNames ()
{
     return (m_parentManagedObjectNames);
}

void WaveManagedObjectUpdateContext::setMessage (PrismMessage *pMessage)
{
    m_pMessage = pMessage;
}

PrismMessage *WaveManagedObjectUpdateContext::getMessage ()
{
    return (m_pMessage);
}

void WaveManagedObjectUpdateContext::setAssociatedFromManagedObjectName (string associatedFromMOName)
{
    m_associatedFromMOName = associatedFromMOName;
}

string WaveManagedObjectUpdateContext::getAssociatedFromManagedObjectName ()
{
    return (m_associatedFromMOName);
}

void WaveManagedObjectUpdateContext::setChoiceUserTagNeedToRemove (const UI32 &choiceUserTagNeedToRemove)
{
    m_choiceUserTagNeedToRemove = choiceUserTagNeedToRemove;
}
 
UI32 WaveManagedObjectUpdateContext::getChoiceUserTagNeedToRemove () const
{
    return (m_choiceUserTagNeedToRemove);
}

void WaveManagedObjectUpdateContext::setConfigReplayInProgressFlag (const bool isConfigReplayInProgress)
{
     m_isConfigReplayInProgress = isConfigReplayInProgress;
}

bool WaveManagedObjectUpdateContext::getConfigReplayInProgressFlag () const
{
    return (m_isConfigReplayInProgress);
}

vector<string> WaveManagedObjectUpdateContext::getAttributeNamesFromBackend () const
{
    return (m_attributeNamesFromBackend);
}
 
void WaveManagedObjectUpdateContext::setAttributeNamesFromBackend (const vector<string> &attributeNamesFromBackend)
{
 
    m_attributeNamesFromBackend = attributeNamesFromBackend;
}
 
vector<string> WaveManagedObjectUpdateContext::getAttributeValuesFromBackend () const
{
    return (m_attributeValuesFromBackend);
}
 
void WaveManagedObjectUpdateContext::setAttributeValuesFromBackend (const vector< string > &attributeValuesFromBackend)
{
    m_attributeValuesFromBackend = attributeValuesFromBackend;
}

}
