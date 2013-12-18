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

#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceMessage.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

CommandLineInterfaceMessage::CommandLineInterfaceMessage ()
    : SerializableObject                    (),
      m_commandLineInterfaceOperation       (WAVE_CLI_OPERATION_REQUEST),
      m_commandLineInterfaceOperationStatus (WAVE_CLI_OPERATION_ERROR),
      m_isLastResponse                      (true),
	  m_errorString                         (""),
	  m_isWyserEaCompatibilityMode			(false)
{
}

CommandLineInterfaceMessage::CommandLineInterfaceMessage (const WaveCommandLineInterfaceOperation &commandLineInterfaceOperation, const ResourceId &operationCode)
    : SerializableObject                    (),
      m_commandLineInterfaceOperation       (commandLineInterfaceOperation),
      m_operationCode                       (operationCode),
      m_commandLineInterfaceOperationStatus (WAVE_CLI_OPERATION_ERROR),
      m_isLastResponse                      (true),
      m_errorString                         (""),
      m_isWyserEaCompatibilityMode			(false)
{
}

CommandLineInterfaceMessage::~CommandLineInterfaceMessage ()
{
}

void CommandLineInterfaceMessage::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32         (&m_messageIdentifier,                           "messageIdentifier"));
    addSerializableAttribute (new AttributeResourceId   ((UI32 *) &m_commandLineInterfaceOperation,      "commandLineInterfaceOperation"));
    addSerializableAttribute (new AttributeResourceId   ((UI32 *) &m_operationCode,                      "operationCode"));
    addSerializableAttribute (new AttributeStringVector (&m_input,                                       "input"));
    addSerializableAttribute (new AttributeString       (&m_ttyName,                                      "ttyName"));
    addSerializableAttribute (new AttributeResourceId   ((UI32 *) &m_commandLineInterfaceOperationStatus, "commandLineInterfaceOperationStatus"));
    addSerializableAttribute (new AttributeStringVector (&m_output,                                      "output"));
    addSerializableAttribute (new AttributeBool         (&m_isLastResponse,                              "isLastResponse"));
    addSerializableAttribute (new AttributeString         (&m_errorString,                              "errorString"));
    addSerializableAttribute (new AttributeBool         (&m_isWyserEaCompatibilityMode,                   "isWyserEaCompatibilityMode"));
}

UI32 CommandLineInterfaceMessage::getMessageIdentifier () const
{
    return (m_messageIdentifier);
}

void CommandLineInterfaceMessage::setMessageIdentifier (const UI32 &messageIdentifier)
{
    m_messageIdentifier = messageIdentifier;
}

WaveCommandLineInterfaceOperation CommandLineInterfaceMessage::getCommandLineInterfaceOperation () const
{
    return (m_commandLineInterfaceOperation);
}

void CommandLineInterfaceMessage::setCommandLineInterfaceOperation (const WaveCommandLineInterfaceOperation &commandLineInterfaceOperation)
{
    m_commandLineInterfaceOperation = commandLineInterfaceOperation;
}

ResourceId CommandLineInterfaceMessage::getOperationcode () const
{
    return (m_operationCode);
}

void CommandLineInterfaceMessage::setOperationcode (const ResourceId &operationCode)
{
    m_operationCode = operationCode;
}

vector<string> &CommandLineInterfaceMessage::getInput ()
{
    return (m_input);
}

void CommandLineInterfaceMessage::addToInput (const string &additionalInput)
{
    m_input.push_back (additionalInput);
}

string CommandLineInterfaceMessage::getTtyName () const
{
    return (m_ttyName);
}

void CommandLineInterfaceMessage::setTtyName (const string &ttyName)
{
    m_ttyName = ttyName;
}

WaveCommandLineInterfaceOperationStatus CommandLineInterfaceMessage::getCommandLineInterfaceOperationStatus () const
{
    return (m_commandLineInterfaceOperationStatus);
}

void CommandLineInterfaceMessage::setCommandLineInterfaceOperationStatus (const WaveCommandLineInterfaceOperationStatus &commandLineInterfaceOperationStatus)
{
    m_commandLineInterfaceOperationStatus = commandLineInterfaceOperationStatus;
}

vector<string> &CommandLineInterfaceMessage::getOutput ()
{
    return (m_output);
}

void CommandLineInterfaceMessage::addToOutput (const string &additionalOutput)
{
    m_output.push_back (additionalOutput);
}

bool CommandLineInterfaceMessage::getIsLastResponse () const
{
    return (m_isLastResponse);
}

void CommandLineInterfaceMessage::setIsLastResponse (const bool &isLastResponse)
{
    m_isLastResponse = isLastResponse;
}

string CommandLineInterfaceMessage::getErrorString() const
{
    return (m_errorString);
}

void CommandLineInterfaceMessage::setErrorString (const string &errorString)
{
	m_errorString = errorString;
}

bool CommandLineInterfaceMessage::getIsWyserEaCompatibilityMode() const
{
    return (m_isWyserEaCompatibilityMode);
}

void CommandLineInterfaceMessage::setIsWyserEaCompatibilityMode (const bool &isWyserEaCompatibilityMode)
{
	m_isWyserEaCompatibilityMode = isWyserEaCompatibilityMode;
}

}
