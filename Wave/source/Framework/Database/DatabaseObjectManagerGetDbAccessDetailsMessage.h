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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DATABASEOBJECTMANAGERGETDBACCESSDETAILSMESSAGE_H
#define DATABASEOBJECTMANAGERGETDBACCESSDETAILSMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class DatabaseObjectManagerGetDbAccessDetailsMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        DatabaseObjectManagerGetDbAccessDetailsMessage ();
        virtual        ~DatabaseObjectManagerGetDbAccessDetailsMessage ();

                string  getDatabaseName                                () const;
                void    setDatabaseName                                (const string &databaseName);

                UI32    getPort                                        () const;
                void    setPort                                        (const UI32 &port);

    // Now the Data Members

    private :
        string m_databaseName;
        UI32   m_port;

    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGERGETDBACCESSDETAILSMESSAGE_H
