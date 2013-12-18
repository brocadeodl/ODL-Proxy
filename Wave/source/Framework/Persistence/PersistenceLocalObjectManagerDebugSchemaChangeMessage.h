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

/**************************************************************************************
 *  @file : PersistenceLocalObjectManagerDebugSchemaChangeMessage.h                                 *
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.                          *
 *   All rights reserved.                                                             *
 *   Description : Declares a message Persistence OM handled message used to print    *
 *                 ORM repository information.                                        *
 *   Author : Pritee Ghosh                                                            *
 *   Date   : 12/03/2011                                                              *
 **************************************************************************************/

#ifndef PERSISTENCELOCALOBJECTMANAGERDEBUGSCHEMACHANGEMESSAGE_H
#define PERSISTENCELOCALOBJECTMANAGERDEBUGSCHEMACHANGEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"


namespace WaveNs
{

class PersistenceLocalObjectManagerDebugSchemaChangeMessage : public ManagementInterfaceMessage
{
    private :
                void            setupAttributesForSerialization         ();
    protected :
    public :
                                PersistenceLocalObjectManagerDebugSchemaChangeMessage ();
                                PersistenceLocalObjectManagerDebugSchemaChangeMessage (const vector<string> &arguments);
        virtual                ~PersistenceLocalObjectManagerDebugSchemaChangeMessage ();


        
        const   vector<string> &getCmdArgs                              () const;

                string          getResultPrint                          () const;
                void            setResultPrint                          (const string &result);

    // Now the data members
    private :
                vector<string>  m_cmdArgs;
                string          m_resultPrint;

    protected :
    public :
};

}
#endif // PERSISTENCELOCALOBJECTMANAGERDEBUGSCHEMACHANGEMESSAGE_H
