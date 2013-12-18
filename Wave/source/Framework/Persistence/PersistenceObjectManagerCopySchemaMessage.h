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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef PERSISTENCEOBJECTMANAGERCOPYSCHEMAMESSAGE_H
#define PERSISTENCEOBJECTMANAGERCOPYSCHEMAMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class PersistenceObjectManagerCopySchemaMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        PersistenceObjectManagerCopySchemaMessage (const string &sourceSchema, const string &destSchema);
                        PersistenceObjectManagerCopySchemaMessage ();
        virtual        ~PersistenceObjectManagerCopySchemaMessage ();

                void    setSourceSchema                                   (const string &sourceSchema);
                string  getSourceSchema                                   () const;
                void    setDestSchema                                     (const string &destSchema);
                string  getDestSchema                                     () const;

    // Now the data members

    private :
        string m_sourceSchema;
        string m_destSchema;

    protected :
    public :
};

}

#endif // PERSISTENCEOBJECTMANAGERCOPYSCHEMAMESSAGE_H
