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
 *   Author : Himanshu VArshney                                            *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectAsynchronousCreateContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

WaveManagedObjectAsynchronousCreateContext::WaveManagedObjectAsynchronousCreateContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : PrismAsynchronousContext (pCaller, pCallback, pCallerContext)
{
}

WaveManagedObjectAsynchronousCreateContext::~WaveManagedObjectAsynchronousCreateContext ()
{

}

vector<string> WaveManagedObjectAsynchronousCreateContext::getAttributeNames () const
{
    return (m_attributeNames);
}

void WaveManagedObjectAsynchronousCreateContext::setAttributeNames (const vector<string> &attributeNames)
{

    m_attributeNames = attributeNames;
}

vector<string> WaveManagedObjectAsynchronousCreateContext::getAttributeValues () const
{
    return (m_attributeValues);
}

void WaveManagedObjectAsynchronousCreateContext::setAttributeValues(const vector< string >& attributeValues)
{
    m_attributeValues = attributeValues;
}

string WaveManagedObjectAsynchronousCreateContext::getManagedObjectClassNameNeedToBeCreated () const
{
    return (m_managedObjectClassName);
}

void WaveManagedObjectAsynchronousCreateContext::setManagedObjectClassNameNeedToBeCreated (const string &managedObjectClassName)
{
    m_managedObjectClassName = managedObjectClassName;
}

ObjectId WaveManagedObjectAsynchronousCreateContext::getObjectId () const
{
    return (m_objectId);
}

void WaveManagedObjectAsynchronousCreateContext::setObjectId (const WaveNs::ObjectId& objectId)
{
    m_objectId = objectId;
}

string WaveManagedObjectAsynchronousCreateContext::getParentAttributeName () const
{
    return (m_parentAttributeName);
}

void WaveManagedObjectAsynchronousCreateContext::setParentAttributeName (const string &attributeName)
{
    m_parentAttributeName = attributeName;
}

}
