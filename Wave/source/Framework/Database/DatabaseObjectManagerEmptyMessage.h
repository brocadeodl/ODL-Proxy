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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DATABASEOBJECTMANAGEREMPTYMESSAGE_H
#define DATABASEOBJECTMANAGEREMPTYMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include <set>

namespace WaveNs
{

class DatabaseObjectManagerEmptyMessage : public ManagementInterfaceMessage
{
    private :
        virtual void        setupAttributesForSerialization ();
    protected :
    public :
                            DatabaseObjectManagerEmptyMessage ();
        virtual            ~DatabaseObjectManagerEmptyMessage ();
                void        setSchemasToBeEmptied             (const set<string> schemasToBeEmptied);
                set<string> getSchemasToBeEmptied             () const;
                void        addToSchemasToBeEmptied           (const string schemaToBeEmptied);

                void        setSqlForEmpty                    (const string &emptySql);
                void        getSqlForEmpty                    (string &emptySql);

                bool        getIsExecuteEmptyDatabase         () const;
                void        setExecuteEmptyDatabase           (const bool &emptyDatabase);

                bool        getIsReturnSqlForEmptyDatabase    () const;
                void        setReturnSqlForEmptyDatabase      (const bool &returnSql);

                UI32        getDatabaseEmptyType              ();
                void        setDatabaseEmptyType              (const UI32 &emptyType);

                bool        getIsSelectivelyEmptyDatabase     () const;
                void        setSelectivelyEmptyDatabase       (const bool &selectivelyEmpty);

    // Now the data members

    private :
        set<string> m_schemasToBeEmptied;
        string      m_sqlForEmpty;
        bool        m_executeEmptyDatabase;    
        bool        m_returnSqlForEmpty;
        bool        m_selectivelyEmpty;
        UI32        m_databaseEmptyType;
                
    protected :
    public :
};

class DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage : public ManagementInterfaceMessage
{
    private :
        virtual void        setupAttributesForSerialization (); 
    protected :
    public :

                            DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage ();
                           ~DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage ();

                void        setEmptyDatabaseAutoDetectionThresholdValue (const UI32 &thresholdValue);
                UI32        getEmptyDatabaseAutoDetectionThresholdValue ();

                void        setNumberOfEntriesInDatabase (const UI32 &numberOfEntries);
                UI32        getNumberOfEntriesInDatabase ();

    private:
                UI32        m_thresholdValue;
                UI32        m_numberOfEntries;
};

}

#endif // DATABASEOBJECTMANAGEREMPTYMESSAGE_H
