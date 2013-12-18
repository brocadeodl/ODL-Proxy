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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Attributes/AttributeManagedObjectVectorCompositionTemplateBase.h"
#include "Framework/Utils/AssertUtils.h"
#include <vector>

using namespace std;

namespace WaveNs
{
AttributeManagedObjectVectorCompositionTemplateBase::AttributeManagedObjectVectorCompositionTemplateBase (AttributeType &attributeType, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute             (AttributeType::AttributeTypeCompositionVector, attributeName, attributeUserTag, isOperational),
      m_disableValidations  (false)
{
}

AttributeManagedObjectVectorCompositionTemplateBase::AttributeManagedObjectVectorCompositionTemplateBase (const AttributeManagedObjectVectorCompositionTemplateBase &attributeManagedObjectVectorCompositionTemplateBase)
    : Attribute         (attributeManagedObjectVectorCompositionTemplateBase)
{
    m_disableValidations = attributeManagedObjectVectorCompositionTemplateBase.m_disableValidations;
}

AttributeManagedObjectVectorCompositionTemplateBase &AttributeManagedObjectVectorCompositionTemplateBase::operator = (const AttributeManagedObjectVectorCompositionTemplateBase &attributeManagedObjectVectorCompositionTemplateBase)
{
    Attribute::operator = (attributeManagedObjectVectorCompositionTemplateBase);

    m_disableValidations = attributeManagedObjectVectorCompositionTemplateBase.m_disableValidations;

    return (*this);
}

bool AttributeManagedObjectVectorCompositionTemplateBase::getDisableValidations () const
{
    return m_disableValidations;
}

void AttributeManagedObjectVectorCompositionTemplateBase::setDisableValidations (const bool disableValidations)
{
    m_disableValidations = disableValidations;
}

void AttributeManagedObjectVectorCompositionTemplateBase::getCValue (WaveCValue *pCValue)
{
    prismAssert (false, __FILE__, __LINE__); 
}

void AttributeManagedObjectVectorCompositionTemplateBase::storeRelatedObjectVector (const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects)
{
    prismAssert (false, __FILE__, __LINE__);
}

}
