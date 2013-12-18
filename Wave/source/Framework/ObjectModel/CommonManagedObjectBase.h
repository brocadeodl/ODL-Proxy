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
 *   Author : Awanish Kumar Trivedi                                        *
 ***************************************************************************/

#ifndef COMMONMANAGEDOBJECTBASE_H
#define COMMONMANAGEDOBJECTBASE_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include <string>

namespace WaveNs
{

class CommonManagedObjectBase 
{
    private :
    protected :
        virtual void                          setupAttributesForPersistence             ();
        virtual void                          setupAttributesForCreate                  ();

    public :
                                              CommonManagedObjectBase                         (PrismPersistableObject *pPrismPersistableObject);
        virtual                              ~CommonManagedObjectBase                         ();
        void                                  addPersistableAttributeForCommonBase            (Attribute *pAttribute);
        void                                  addPersistableAttributeForCommonBaseCreate      (Attribute *pAttribute);

    // Now the data members

    private :
    protected :
    public :
                PrismPersistableObject       *m_pPrismPersistableObject;

    private :
    protected :
    public :

};

}

#endif // COMMONMANAGEDOBJECTBASE_H
