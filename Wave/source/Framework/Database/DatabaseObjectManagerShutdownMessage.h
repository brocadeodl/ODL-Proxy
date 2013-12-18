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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef DATABASEOBJECTMANAGERSHUTDOWNMESSAGE_H
#define DATABASEOBJECTMANAGERSHUTDOWNMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class DatabaseObjectManagerShutdownMessage : public ManagementInterfaceMessage
{
    private :
    protected :
    public :
                            DatabaseObjectManagerShutdownMessage    ();
                            DatabaseObjectManagerShutdownMessage    (const string &databaseDirectory);
                            DatabaseObjectManagerShutdownMessage    (const bool &isShutdownOnReboot);
        virtual            ~DatabaseObjectManagerShutdownMessage    ();
        virtual  void       setupAttributesForSerialization         ();

        void                setDatabaseDirectory                    (const string &databaseDirectory);
        string              getDatabaseDirectory                    () const;

        void                setIsShutdownOnReboot                   (const bool &isShutdownOnReboot);
        bool                getIsShutdownOnReboot                   () const;
    // Now the data members

    private :
        string                      m_databaseDirectory;
        bool                        m_isShutdownOnReboot;
    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGEREMPTYMESSAGE_H
