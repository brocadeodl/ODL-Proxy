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

#ifndef DATABASEOBJECTMANAGERGETLATESTINSTANCEIDFORTABLEWORKER_H
#define DATABASEOBJECTMANAGERGETLATESTINSTANCEIDFORTABLEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class DatabaseObjectManagerGetLatestInstanceIdForTableMessage;

class DatabaseObjectManagerGetLatestInstanceIdForTableWorker : public WaveWorker
{
    private :
        void getLatestInstanceIdForTableMessageHandler (DatabaseObjectManagerGetLatestInstanceIdForTableMessage *pDatabaseObjectManagerGetLatestInstanceIdForTableMessage);

    protected :
    public :
         DatabaseObjectManagerGetLatestInstanceIdForTableWorker (WaveObjectManager *pWaveObjectManager);
        ~DatabaseObjectManagerGetLatestInstanceIdForTableWorker ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGERGETLATESTINSTANCEIDFORTABLEWORKER_H
