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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef MANAGEMENTINTERFACECLIENTTESTMESSAGES_H
#define MANAGEMENTINTERFACECLIENTTESTMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class ManagementInterfaceClientTestMessage1 : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 ManagementInterfaceClientTestMessage1 ();
        virtual ~ManagementInterfaceClientTestMessage1 ();

    // now the data members

    private :
    protected :
    public :
};

class ManagementInterfaceClientTestMessage2 : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 ManagementInterfaceClientTestMessage2 ();
        virtual ~ManagementInterfaceClientTestMessage2 ();

    // now the data members

    private :
    protected :
    public :
};

class ManagementInterfaceClientTestMessage3 : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 ManagementInterfaceClientTestMessage3 ();
        virtual ~ManagementInterfaceClientTestMessage3 ();

    // now the data members

    private :
    protected :
    public :
};

class ManagementInterfaceClientTestMessage4 : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 ManagementInterfaceClientTestMessage4 ();
        virtual ~ManagementInterfaceClientTestMessage4 ();
                void   setupInputBuffer1            ();
                void   setupOutputBuffer1           ();
                bool   validateInputBuffer1         ();
                bool   validateOutputBuffer1        ();
                void   setInputLargeBuffer1         ();
                bool   validateInputLargeBuffer1    ();

                void        setCompletionStatusType      (const UI32 &completionStatusType);
                ResourceId  getRequestedCompletionStatus ();
    // Now the data members

    private :
        bool m_isLargInputBuffer1Set;
        UI32 m_completionStatusType;

    protected :
    public :
};

}

#endif // MANAGEMENTINTERFACECLIENTTESTMESSAGES_H
