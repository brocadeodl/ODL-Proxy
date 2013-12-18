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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef PERSISTENCEOBJECTMANAGERDELAYEDTRANSACTIONMESSAGES_H
#define PERSISTENCEOBJECTMANAGERDELAYEDTRANSACTIONMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class PersistenceObjectManagerAddDelayedTransactionMessage : public PrismMessage
{
    private :
        void            setupAttributesForSerialization ();

    protected :
    public :
                        PersistenceObjectManagerAddDelayedTransactionMessage    (const string &sql);
                        PersistenceObjectManagerAddDelayedTransactionMessage    ();
        virtual        ~PersistenceObjectManagerAddDelayedTransactionMessage    ();

                string  getSql                                                  () const;
                void    setSql                                                  (const string &sql);


    // Now the data members

    private :
                string  m_sql;

    protected :
    public :
};

}

#endif // PERSISTENCEOBJECTMANAGERDELAYEDTRANSACTIONMESSAGES_H
