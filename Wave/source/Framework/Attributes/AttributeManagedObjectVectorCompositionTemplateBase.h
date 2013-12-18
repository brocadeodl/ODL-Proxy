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

#ifndef ATTRIBUTEMANAGEDOBJECTVECTORCOMPOSITIONTEMPLATEBASE_H
#define ATTRIBUTEMANAGEDOBJECTVECTORCOMPOSITIONTEMPLATEBASE_H

#include "Framework/Attributes/Attribute.h"
#if 0
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "WaveResourceIdEnums.h"
#endif

namespace WaveNs
{

class AttributeManagedObjectVectorCompositionTemplateBase : public Attribute
{
    private :
    protected :
                                                                       AttributeManagedObjectVectorCompositionTemplateBase  (AttributeType &attributeType, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational);
                                                                       AttributeManagedObjectVectorCompositionTemplateBase  (const AttributeManagedObjectVectorCompositionTemplateBase &attribute);
                AttributeManagedObjectVectorCompositionTemplateBase   &operator =                                           (const AttributeManagedObjectVectorCompositionTemplateBase &attribute);

    public :
        virtual void                                                   setDisableValidations                                (const bool disableValidations);
        virtual bool                                                   getDisableValidations                                () const;
        virtual void                                                   getCValue                                            (WaveCValue *pCValue);

        virtual void                                                   storeRelatedObjectVector                             (const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects);

    // Now the data members

    private :
        bool          m_disableValidations;
};

}

#endif // ATTRIBUTEMANAGEDOBJECTVECTORCOMPOSITIONTEMPLATEBASE_H

