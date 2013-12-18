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

#ifndef DATABASEOBJECTMANAGEREXECUTECOPYSCHEMAWORKER_H
#define DATABASEOBJECTMANAGEREXECUTECOPYSCHEMAWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

typedef enum
{
    ORM_DERIVATIONS_TABLE_TYPE = 0,
    ORM_TABLE_TYPE,
    ORM_AUXILLIARY_TABLE_TYPE,
    ORM_TYPE_END
} OrmTableType;

class WaveObjectManager;

class DatabaseObjectManagerExecuteCopySchemaMessage;

class DatabaseObjectManagerExecuteCopySchemaWorker : public WaveWorker
{
    private :
        virtual PrismMessage *createMessageInstance (const UI32 &operationCode);

    protected :
    public :
                                   DatabaseObjectManagerExecuteCopySchemaWorker (WaveObjectManager *pWaveObjectManager);
        virtual                   ~DatabaseObjectManagerExecuteCopySchemaWorker ();

                void               executeCopySchemaMessageHandler              (DatabaseObjectManagerExecuteCopySchemaMessage *pDatabaseObjectManagerExecuteCopySchemaMessage);


    // Now the data members

    private :
        vector<OrmTableType>     m_tableType;
    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGEREXECUTECOPYSCHEMAWORKER_H
