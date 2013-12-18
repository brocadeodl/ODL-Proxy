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
 *   Copyright (C) 2008-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Mithun B S                                                   *
 ***************************************************************************/

#ifndef DATABASEOBJECTMANAGERGETDBSCHEMAINFOMESSAGE_H
#define DATABASEOBJECTMANAGERGETDBSCHEMAINFOMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/ObjectRelationalMapping/DatabaseSchema.h"

using namespace std;

namespace WaveNs
{

class WaveManagedObject;
class WaveObjectManager;

class DatabaseObjectManagerGetDBSchemaInfoMessage : public PrismMessage
{
    private :
    protected :
    public :
                                             DatabaseObjectManagerGetDBSchemaInfoMessage ();
        virtual                             ~DatabaseObjectManagerGetDBSchemaInfoMessage ();

                void                         getDatabaseSchema                              (DatabaseSchema &databaseSchema);
                void                         setDatabaseSchema                              (const DatabaseSchema &databaseSchema);
    // Now the data members

    private :
        DatabaseSchema m_databaseSchema;

    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGERGETDBSCHEMAINFOMESSAGE_H
