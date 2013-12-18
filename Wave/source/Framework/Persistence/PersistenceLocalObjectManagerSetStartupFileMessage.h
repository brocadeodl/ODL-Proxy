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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef PERSISTENCELOCALOBJECTMANAGERSETSTARTUPFILEMESSAGE_H
#define PERSISTENCELOCALOBJECTMANAGERSETSTARTUPFILEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class PersistenceLocalObjectManagerSetStartupFileMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        PersistenceLocalObjectManagerSetStartupFileMessage  (const ResourceId &startupFileType, const string &fileName);
                        PersistenceLocalObjectManagerSetStartupFileMessage  ();
        virtual        ~PersistenceLocalObjectManagerSetStartupFileMessage  ();

                string      getStartupFileName                                  () const;
                void        setStartupFileName                                  (const string &fileName);
                ResourceId  getStartupFileType                                  () const;
                void        setStartupFileType                                  (const ResourceId &startupFileType);

    // Now the data members

    private :
        ResourceId  m_startupFileType; 
        string      m_fileName;

    protected :
    public :
};

}

#endif // PERSISTENCELOCALOBJECTMANAGERSETSTARTUPFILEMESSAGE_H
