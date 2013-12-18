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

#ifndef WAVESYSTEMMANAGEMENTGETPOSSIBLECOMPLETIONSMESSAGE_H
#define WAVESYSTEMMANAGEMENTGETPOSSIBLECOMPLETIONSMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class WaveSystemManagementGetPossibleCompletionsMessage: public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        WaveSystemManagementGetPossibleCompletionsMessage               ();
        virtual        ~WaveSystemManagementGetPossibleCompletionsMessage               ();

        void                            addCompletionNameAndInfoOneByOne                (const string &completionName, const string &completionInfo);
        void                            addCompletionNameByVector                       (const vector<pair <string, string> > &completionNames);
        vector<pair<string, string> >   getCompletionNameAndInfo                        () const;

        string          getCommandName                                                  () const;
        void            setCommandName                                                  (const string &commandName);
        string          getToken                                                        () const;
        void            setToken                                                        (const string &token);
 
    // Now the Data Members
    private :
        string          m_token;
        string          m_commandName;
        vector<pair<string, string> >  m_completionNameAndInfo;

    protected :
    public :
};

}

#endif // WAVESYSTEMMANAGEMENTDISPLAYCONFIGURATIONBYTARGETNODENAMEMESSAGE_H
