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

#ifndef PERSISTENCEOBJECTMANAGEREXECUTETRANSACTIONMESSAGE_H
#define PERSISTENCEOBJECTMANAGEREXECUTETRANSACTIONMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class PersistenceObjectManagerExecuteTransactionMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                              PersistenceObjectManagerExecuteTransactionMessage (const string &sql);
                              PersistenceObjectManagerExecuteTransactionMessage ();
        virtual              ~PersistenceObjectManagerExecuteTransactionMessage ();

                const string &getSql                                            () const;
                      void    setSql                                            (const string &sql);

                void                setConfigNamesForInsert                   (const vector<string> &configNamesForInsert);
                vector<string>      getConfigNamesForInsert                   () const;
                void                setConfigValuesForInsert                  (const vector<string> &configValuesForInsert);
                vector<string>      getConfigValuesForInsert                  () const;
                void                setConfigTypesForInsert                   (const vector<ResourceId> &configTypesForInsert);
                vector<ResourceId>  getConfigTypesForInsert                   () const;

                void                setConfigNamesForUpdate                   (const vector<string> &configNamesForUpdate);
                vector<string>      getConfigNamesForUpdate                   () const;
                void                setConfigValuesForUpdate                  (const vector<string> &configValuesForUpdate);
                vector<string>      getConfigValuesForUpdate                  () const;
                void                setConfigTypesForUpdate                   (const vector<ResourceId> &configTypesForUpdate);
                vector<ResourceId>  getConfigTypesForUpdate                   () const;
    // Now the data members

    private :
        string             m_sql;
        vector<string>     m_configNamesForInsert;
        vector<string>     m_configValuesForInsert;
        vector<ResourceId> m_configTypesForInsert;

        vector<string>     m_configNamesForUpdate;
        vector<string>     m_configValuesForUpdate;
        vector<ResourceId> m_configTypesForUpdate;
    protected :
    public :
};

}

#endif // PERSISTENCEOBJECTMANAGEREXECUTETRANSACTIONMESSAGE_H
