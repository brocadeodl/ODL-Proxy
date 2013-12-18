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

#ifndef COMMANDLINEINTERFACEMESSAGE_H
#define COMMANDLINEINTERFACEMESSAGE_H

#include "Framework/ObjectModel/SerializableObject.h"

namespace WaveNs
{

class CommandLineInterfaceMessage : public SerializableObject
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                                                          CommandLineInterfaceMessage           ();
                                                          CommandLineInterfaceMessage           (const WaveCommandLineInterfaceOperation &commandLineInterfaceOperation, const ResourceId &operationCode);
        virtual                                          ~CommandLineInterfaceMessage           ();

                UI32                                     getMessageIdentifier                   () const;
                void                                     setMessageIdentifier                   (const UI32 &messageIdentifier);

                WaveCommandLineInterfaceOperation        getCommandLineInterfaceOperation       () const;
                void                                     setCommandLineInterfaceOperation       (const WaveCommandLineInterfaceOperation &commandLineInterfaceOperation);

                ResourceId                               getOperationcode                       () const;
                void                                     setOperationcode                       (const ResourceId &operationCode);

                vector<string>                          &getInput                               ();
                void                                     addToInput                             (const string &additionalInput);

                string                                   getTtyName                             () const;
                void                                     setTtyName                             (const string &ttyName);

                WaveCommandLineInterfaceOperationStatus  getCommandLineInterfaceOperationStatus () const;
                void                                     setCommandLineInterfaceOperationStatus (const WaveCommandLineInterfaceOperationStatus &commandLineInterfaceOperationStatus);

                vector<string>                          &getOutput                              ();
                void                                     addToOutput                            (const string &additionalOutput);

                bool                                     getIsLastResponse                      () const;
                void                                     setIsLastResponse                      (const bool &isLastResponse);

                string 									getErrorString() const;
                void 									setErrorString(const string &errorString);

                bool									getIsWyserEaCompatibilityMode() const;
                void 									setIsWyserEaCompatibilityMode(const bool &isWyserEaCompatibilityMode);


    // Now the data members

    private :
        UI32                                    m_messageIdentifier;
        WaveCommandLineInterfaceOperation       m_commandLineInterfaceOperation;
        ResourceId                              m_operationCode;
        vector<string>                          m_input;
        string                                  m_ttyName;
        WaveCommandLineInterfaceOperationStatus m_commandLineInterfaceOperationStatus;
        vector<string>                          m_output;
        bool                                    m_isLastResponse;
        string 									m_errorString;
        bool 									m_isWyserEaCompatibilityMode;

    protected :
    public :
};

}

#endif // COMMANDLINEINTERFACEMESSAGE_H
