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
 *   Author : Pritee Kadu                                                  *
 ***************************************************************************/

#ifndef STARTUPSCHEMACHANGEEVENT_H
#define STARTUPSCHEMACHANGEEVENT_H
#include "Framework/Messaging/Local/PrismEvent.h"

namespace WaveNs
{

typedef enum 
{
    RUNNING_SCHEMA = 0,
    DEFAULT_SCHEMA,
    STARTUP_SCHEMA,
    SCHEMA_FILE
}SchemaType;
    
class StartupSchemaChangeEvent : public PrismEvent
{
    private :

    protected :
        virtual void    setupAttributesForSerialization ();

    public :
                        StartupSchemaChangeEvent    ();
        virtual         ~StartupSchemaChangeEvent   ();

        bool            getOperationStatus          () const;
        void            setOperationStatus          (const bool isOpComplete);

        SchemaType      getFromSchema               () const;
        void            setFromSchema               (SchemaType schemaCopiedFrom);

        string          getFromFileSchemaPath       () const;
        void            setFromFileSchemaPath       (const string &fileCopiedFromStartupSchema);

        SchemaType      getToSchema                 () const;
        void            setToSchema                 (SchemaType schemaCopiedTo);

        string          getToFileSchemaPath         () const;
        void            setToFileSchemaPath         (const string &fileCopiedToStartupSchema);

        string          getSchemaName               (const SchemaType schemaEnumValue) const;

    // Now the data members

    private :
        bool            m_operationStatus;          // 0 : Operation STARTED and 1:COMPLETED
        SchemaType      m_fromSchema;               // SchemaType <enum> from which the schema is copied to STARTUP Config
        string          m_pathForFromSchemaFile;    // If operation is copy <file> startup, this will have fileName for Schema.
        
        SchemaType      m_toSchema;                 // SchemaType <enum> from which the schema is copied to STARTUP Config
        string          m_pathForToSchemaFile;      // If operation is copy <file> startup, this will have fileName for Schema.
        
    protected :
    public :
};

}

#endif // STARTUPSCHEMACHANGEEVENT_H
