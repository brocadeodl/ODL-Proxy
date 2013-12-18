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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef PERSISTENCELOCALOBJECTMANAGERLIVESYNCENABLEMESSAGES_H
#define PERSISTENCELOCALOBJECTMANAGERLIVESYNCENABLEMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class PersistenceLocalObjectManagerGetLiveSyncEnableMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        PersistenceLocalObjectManagerGetLiveSyncEnableMessage ();
        virtual        ~PersistenceLocalObjectManagerGetLiveSyncEnableMessage ();
                bool    getIsLiveSyncEnabled                                  ();
                void    setIsLiveSyncEnabled                                  (const bool &isLiveSyncEnabled);

    // Now the data members

    private :
        bool m_isLiveSyncEnabled;

    protected :
    public :
};

class PersistenceLocalObjectManagerSetLiveSyncEnableMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        PersistenceLocalObjectManagerSetLiveSyncEnableMessage ();
                        PersistenceLocalObjectManagerSetLiveSyncEnableMessage (const bool &isLiveSyncEnabled);
        virtual        ~PersistenceLocalObjectManagerSetLiveSyncEnableMessage ();
                bool    getIsLiveSyncEnabled                                  ();
                void    setIsLiveSyncEnabled                                  (const bool &isLiveSyncEnabled);

    // Now the data members

    private :
        bool m_isLiveSyncEnabled;

    protected :
    public :
};

}

#endif // PERSISTENCELOCALOBJECTMANAGERLIVESYNCENABLEMESSAGES_H