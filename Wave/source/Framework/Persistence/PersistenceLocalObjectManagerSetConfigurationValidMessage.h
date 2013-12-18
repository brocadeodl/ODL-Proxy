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
 *   Author : Anil ChannaveeraSetty                                        *
 ***************************************************************************/

#ifndef PERSISTENCELOCALOBJECTMANAGERSETCONFIGURATIONVALIDMESSAGE_H
#define PERSISTENCELOCALOBJECTMANAGERSETCONFIGURATIONVALIDMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace std;

namespace WaveNs
{

class PersistenceLocalObjectManagerSetConfigurationValidMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        PersistenceLocalObjectManagerSetConfigurationValidMessage (const bool &validity, const string &schemaFileName = "");
                        PersistenceLocalObjectManagerSetConfigurationValidMessage ();
        virtual        ~PersistenceLocalObjectManagerSetConfigurationValidMessage ();

                void    setConfigurationValidity                          (const bool &validity);
                bool    getConfigurationValidity                          () const;

                void    setSchemaFile                                     (const string &schemaFileName);
                string  getSchemaFile                                     () const;
    // Now the data members

    private :
        bool            m_isConfigurationValid;
        string          m_schemaFile;

    protected :
    public :
};

}

#endif // PERSISTENCELOCALOBJECTMANAGERSETCONFIGURATIONVALIDMESSAGE_H
