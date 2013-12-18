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

/**********************************************************************************************
 *  @file : DatabaseObjectManagerCalculateDbCksumWorker.h                                     *
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.                                  *
 *   All rights reserved.                                                                     *
 *   Description : Declares a class DatabaseObjectManagerCalculateDbCksumWorker.              *
 *                  The handler handles a message DatabaseObjectManagerCalculateDbCksumMessage*
 *                  where it calculates the checksum for local database and reply.            *
 *   Author : Pritee Ghosh                                                                    *
 *   Date   : 02/22/2011                                                                      *
 **********************************************************************************************/

#ifndef DATABASEOBJECTMANAGERCALCULATEDBCKSUMWORKER_H
#define DATABASEOBJECTMANAGERCALCULATEDBCKSUMWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class WaveObjectManager;

class DatabaseObjectManagerCalculateDbCksumMessage;

class DatabaseObjectManagerCalculateDbCksumWorker : public WaveWorker
{
    private :
            virtual PrismMessage  *createMessageInstance                       (const UI32 &operationCode);
    protected :
    public :
                                   DatabaseObjectManagerCalculateDbCksumWorker (WaveObjectManager *pWaveObjectManager);
            virtual               ~DatabaseObjectManagerCalculateDbCksumWorker ();

                    void           calculateDBCksumMessageHandler              (DatabaseObjectManagerCalculateDbCksumMessage *pDatabaseObjectManagerCalculateDbCksumMessage);


    // Now the data members

    private :
    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGERCALCULATEDBCKSUMWORKER_H
