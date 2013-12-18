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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef DATABASEOBJECTMANAGEREXECUTEQUERYMESSAGEFORCOUNT_H
#define DATABASEOBJECTMANAGEREXECUTEQUERYMESSAGEFORCOUNT_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "libpq-fe.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class WaveManagedObject;
class WaveObjectManager;

class DatabaseObjectManagerExecuteQueryMessageForCount : public PrismMessage
{
    private :
    protected :
    public :
                                             DatabaseObjectManagerExecuteQueryMessageForCount (const string &className, WaveObjectManager *pWaveObjectManager, const string &sql);
        virtual                             ~DatabaseObjectManagerExecuteQueryMessageForCount ();

                string                       getClassName                                     () const;
                WaveObjectManager           *getPWaveObjectManager                            () const;
                string                       getSql                                           () const;
                UI32                         getCount                                         () const;
                void                         setCount                                         (const UI32 &count);
    // Now the data members

    private :
        string                       m_className;
        WaveObjectManager           *m_pWaveObjectManager;
        string                       m_sql;
        UI32                         m_count;

    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGEREXECUTEQUERYMESSAGEFORCOUNT_H
