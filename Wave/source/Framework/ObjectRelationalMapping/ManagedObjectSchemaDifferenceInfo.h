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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef MANAGEDOBJECTSCHEMADIFFERENCEINFO_H
#define MANAGEDOBJECTSCHEMADIFFERENCEINFO_H


#include "Framework/Types/Types.h"
#include "Framework/Attributes/Attribute.h"

#include <string>
#include <map>
#include <set>
#include <memory>

namespace WaveNs
{

class Field
{
    private:
    protected:
    public:
                                Field                                   (const string& fieldName, const string& fieldType, const string& parentManagedObjectName);
                               ~Field                                   ();
        const string &          getFieldName                            () const;
        const string &          getFieldType                            () const;
        const string &          getParentManagedObjectName              () const;
              Attribute *       getAttribute                            ();
              void              setAttribute                            (Attribute *pAttribute);
              void              getSqlForField                          (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst);
    
    private:
        string                  m_fieldName;
        string                  m_fieldType;
        string                  m_parentManagedObjectName;
        Attribute*              m_pAttribute;
    protected:
    public:
};

class ManagedObjectSchemaDifferenceInfo
{
    private:
    protected:
    public:
                                ManagedObjectSchemaDifferenceInfo       (const string& managedObjectName, bool isNewManagedObject);
        virtual                ~ManagedObjectSchemaDifferenceInfo       ();
        const string &          getManagedObjectName                    () const;
        bool                    getIsManagedObjectInstancePresentInDb   () const;
        void                    setIsManagedObjectInstancePresentInDb   (bool isManagedObjectInstancePresentInDb);
        bool                    getIsNewManagedObject                   () const;
        void                    addAddedField                           (Field *pField);
        void                    getSqlForAddedFields                    (string &sqlForUpdate);
        UI32                    getNumberOfAddedFields                  () const;
        

    private:
        string                  m_managedObjectName;
        bool                    m_isNewManagedObject;
        bool                    m_isManagedObjectInstancePresentInDb;
        map <string,Field *>    m_addedFields;


    protected:
    public:


};


}

#endif // MANAGEDOBJECTSCHEMADIFFERENCEINFO_H

