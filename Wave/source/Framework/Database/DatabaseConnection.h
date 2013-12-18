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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <string>

#include "libpq-fe.h"

using namespace std;

namespace WaveNs
{

class DatabaseConnection
{
    private :
             DatabaseConnection (const string &name, const UI32 &port);

        void lock               ();
        void unlock             ();

    protected :
    public :
                                  ~DatabaseConnection ();

        static DatabaseConnection *getInstance        (const string &name, const UI32 &port);
        static void                deleteInstance     ();

               bool                connect            (const UI32 &maximumNumberOfRetries);
               void                close              ();

               PGconn             *getPConnection     ();

    // Now the data members

    private :
                string              m_name;
                UI32                m_port;
                PGconn              *m_pConnection;
                PrismMutex          m_mutex;

        static  DatabaseConnection *s_pDatabaseConnection; // Singleton connection object

    protected :
    public :
};

}

#endif // DATABASECONNECTION_H
