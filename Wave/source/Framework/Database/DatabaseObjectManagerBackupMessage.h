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

#ifndef DATABASEOBJECTMANAGERBACKUPMESSAGE_H
#define DATABASEOBJECTMANAGERBACKUPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class DatabaseObjectManagerBackupMessage : public PrismMessage
{
    private :
    protected :
    public :
                        DatabaseObjectManagerBackupMessage (const string &backupFileName, const bool &pauseTheService = true);
        virtual        ~DatabaseObjectManagerBackupMessage ();

                string  getBackupFileName                  () const;
                void    setBackupFileName                  (const string &backupFileName);
                bool    getPauseTheService                 () const;
                void    setBackupAllSchema                 (const bool isNeeded);
                bool    getBackupAllSchema                 () const;
                bool    getDataOnlyBackup                  () const;
                void    setDataOnlyBackup                  (const bool dataOnlyBackup);

    // Noe the data members

    private :
        string m_backupFileName;
        bool   m_pauseTheService;
        bool   m_backupAllSchemas;
        bool   m_dataOnlyBackup;

    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGERBACKUPMESSAGE_H
