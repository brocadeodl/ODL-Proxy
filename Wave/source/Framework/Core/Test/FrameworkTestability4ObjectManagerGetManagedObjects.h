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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef FRAMEWORKTESTABILITY4OBJECTMANAGERGETMANAGEDOBJECTS_H
#define FRAMEWORKTESTABILITY4OBJECTMANAGERGETMANAGEDOBJECTS_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class FrameworkTestability4ObjectManagerGetManagedObjects : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                                FrameworkTestability4ObjectManagerGetManagedObjects ();
                                FrameworkTestability4ObjectManagerGetManagedObjects(const string &managedObjectClassName, const string &schemaName);
        virtual                ~FrameworkTestability4ObjectManagerGetManagedObjects ();

                string          getManagedObjectClassName                           () const;
                void            setManagedObjectClassName                           (const string &managedObjectClassName);

                string          getSchemaName                                       () const;
                void            setSchemaName                                       (const string &schemaName);

                UI32            getNumberOfManagedObjects                           () const;
                void            setNumberOfManagedObjects                           (const UI32 &numberOfManagedObjects);

                vector<string> &getManagedObjects                                   ();

                bool            getUseRawFormat                                     () const;
                void            setUseRawFormat                                     (const bool &useRawFormat);

    // now the data members

    private :
        string         m_managedObjectClassName;
        string         m_schemaName;
        UI32           m_numberOfManagedObjects;
        vector<string> m_managedObjects;
        bool           m_useRawFormat;

    protected :
    public :
};

}

#endif // FRAMEWORKTESTABILITY4OBJECTMANAGERGETMANAGEDOBJECTS_H
