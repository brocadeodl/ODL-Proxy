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

#ifndef DATABASEOBJECTMANAGEREXECUTECOPYSCHEMAMESSAGE_H
#define DATABASEOBJECTMANAGEREXECUTECOPYSCHEMAMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "libpq-fe.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class WaveManagedObject;
class WaveObjectManager;

class DatabaseObjectManagerExecuteCopySchemaMessage : public PrismMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                                              DatabaseObjectManagerExecuteCopySchemaMessage (const string &prepareString, const string &sourceSchema, const string &destSchema);
                                              DatabaseObjectManagerExecuteCopySchemaMessage (const string &sourceSchema, const string &destSchema);
                                              DatabaseObjectManagerExecuteCopySchemaMessage ();
        virtual                              ~DatabaseObjectManagerExecuteCopySchemaMessage ();

                string                        getClassName                                  () const;
                WaveObjectManager           *getPWaveObjectManager                          () const;
                void                         setPrepareString                               (const string &prepareString);
                string                       getPrepareString                               () const;
                void                         setPResults                                    (vector<WaveManagedObject *> *pResults);
                vector<WaveManagedObject *> *getPResults                                    () const;
                string                       getSourceSchema                                () const;
                void                         setSourceSchema                                (const string &sourceSchema);
                string                       getDestSchema                                  () const;
                void                         setDestSchema                                  (const string &destSchema);

    // Now the data members

    private :
        vector<WaveManagedObject *> *m_pResults;
        string                      m_prepareString;
        string                      m_sourceSchema;
        string                      m_destSchema;

    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGEREXECUTECOPYSCHEMAMESSAGE_H
