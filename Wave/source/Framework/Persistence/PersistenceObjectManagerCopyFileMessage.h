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

#ifndef PERSISTENCEOBJECTMANAGERCOPYFILEMESSAGE_H
#define PERSISTENCEOBJECTMANAGERCOPYFILEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class PersistenceObjectManagerCopyFileMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        PersistenceObjectManagerCopyFileMessage (const ResourceId &startupFileType, const string &startupFileName = "", const bool &isLocalFileCopy = false);
                        PersistenceObjectManagerCopyFileMessage ();
        virtual        ~PersistenceObjectManagerCopyFileMessage ();

                void        setStartupFileName                     (const string &startupFileName);
                string      getStartupFileName                     () const;
                void        setStartupFileType                     (const ResourceId &startupFileType);
                ResourceId  getStartupFileType                     () const;
                bool        getIsLocalFileCopy                     () const;
                void        setIsLocalFileCopy                     (const bool &isLocalFileCopy);

    // Now the data members

    private :
        ResourceId  m_startupFileType;
        string      m_startupFileName;
        bool        m_isLocalFileCopy;

    protected :
    public :
};

}

#endif // PERSISTENCEOBJECTMANAGERCOPYFILEMESSAGE_H
