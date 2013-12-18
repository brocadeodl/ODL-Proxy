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
 *   Author : Mayur Mahajan                                                *
 ***************************************************************************/

#ifndef MOSCHEMAINFOMANAGEDOBJECT_H
#define MOSCHEMAINFOMANAGEDOBJECT_H


namespace WaveNs
{
class MoSchemaInfoManagedObject : public  WaveManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();
    protected :
    public :
                                MoSchemaInfoManagedObject       (WaveObjectManager *pWaveObjectManager);
        virtual                 ~MoSchemaInfoManagedObject      ();
        const string &          getManagedObjectName            () const;
        const vector<string> &  getManagedObjectFieldNames      () const;
        const vector<string> &  getManagedObjectFieldTypes      () const;
        const vector<string> &  getManagedObjectUserDefinedFieldNames       () const;
        const vector<string> &  getManagedObjectUserDefinedFieldTypes       () const;
        const vector<string> &  getManagedObjectExpandedFieldTypes          () const;
              UI32              getManagedObjectClassId         () const;
              UI32              getManagedObjectParentClassId   () const;
       static string            getClassName                    ();
              bool              getIsALocalManagedObject        () const;
       const  string &          getDerivedFromClassName         () const;        
        
    // Now the data members

    private :
        string                                  m_managedObjectName;
        vector<string>                          m_fieldNames;
        vector<string>                          m_fieldTypes;
        UI32                                    m_classId;
        UI32                                    m_parentClassId;
        vector<string>                          m_expandedFieldTypes;
        vector<string>                          m_userDefinedFieldNames;
        vector<string>                          m_userDefinedFieldTypes;
        bool                                    m_isALocalManagedObject;
        string                                  m_derivedFromClassName;

    protected :
    public :
};
}
#endif //MOSCHEMAINFOMANAGEDOBJECT_H

