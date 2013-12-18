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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Anil C                                                        *
 ***************************************************************************/

#include "ManagementInterface/ClientInterface/UnifiedClientGetDataFromClientMessage.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

UnifiedClientGetDataFromClientMessage::UnifiedClientGetDataFromClientMessage ()
    : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), UNIFIEDCLIENTGETDATA)
{
    m_commandCode = 0;
    m_commandType = 0;
    m_enablePaginate = false;
    m_numberOfRecordsRequested = 0;
    m_numberOfCommandStrings = 0;
}

UnifiedClientGetDataFromClientMessage::UnifiedClientGetDataFromClientMessage ( UI32 numberOfRecordsRequested, UI32 commandCode, UI32 commandType )
    : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), UNIFIEDCLIENTGETDATA),
      m_numberOfRecordsRequested (numberOfRecordsRequested), 
      m_commandCode (commandCode),
      m_commandType (commandType),  
      m_numberOfCommandStrings (0)
{
    m_enablePaginate = false;
}

UnifiedClientGetDataFromClientMessage::~UnifiedClientGetDataFromClientMessage ()
{
}

void UnifiedClientGetDataFromClientMessage::setupAttributesForSerialization ()
{   
    ManagementInterfaceMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeUI32 (&m_commandCode, "commandCode"));
    addSerializableAttribute (new AttributeUI32 (&m_commandType, "commandType"));
    addSerializableAttribute (new AttributeBool (&m_enablePaginate,"enablePaginate")); 
    addSerializableAttribute (new AttributeUI32 (&m_numberOfRecordsRequested, "numberOfRecordsRequested"));
    addSerializableAttribute (new AttributeSI32 (&m_numberOfCommandStrings, "numberOfCommandStrings"));
    addSerializableAttribute (new AttributeStringVector (&m_commandStrings, "commandStrings"));
}

void UnifiedClientGetDataFromClientMessage::setEnablePaginate ( const bool &enablePaginate )
{
    m_enablePaginate = enablePaginate ;
}

bool UnifiedClientGetDataFromClientMessage::getEnablePaginate ( ) const
{
    bool enablePaginate = m_enablePaginate;
    return (enablePaginate);
}

void UnifiedClientGetDataFromClientMessage::setNumberOfRecordsRequested ( const UI32 &numberOfRecordsRequested )
{
    m_numberOfRecordsRequested = numberOfRecordsRequested ;
}

UI32 UnifiedClientGetDataFromClientMessage::getNumberOfRecordsRequested ( ) const
{
    UI32 numberOfRecordsRequested = m_numberOfRecordsRequested ;
    return ( numberOfRecordsRequested );
}

void UnifiedClientGetDataFromClientMessage::setCommandCode ( const UI32 &commandCode )
{       
    m_commandCode = commandCode ;
}       
        
UI32 UnifiedClientGetDataFromClientMessage::getCommandCode ( ) const
{
    UI32 commandCode = m_commandCode ;
    return ( commandCode );
}

void UnifiedClientGetDataFromClientMessage::setCommandType ( const UI32 &commandType )
{
    m_commandType = commandType ;
}

UI32 UnifiedClientGetDataFromClientMessage::getCommandType ( ) const
{
    UI32 commandType = m_commandType ;
    return ( commandType );
}

void UnifiedClientGetDataFromClientMessage::setNumberOfCommandStrings (const SI32 numberOfCommandStrings)
{
    m_numberOfCommandStrings = numberOfCommandStrings;
}

SI32 UnifiedClientGetDataFromClientMessage::getNumberOfCommandStrings () const
{
    return (m_numberOfCommandStrings);
}

void UnifiedClientGetDataFromClientMessage::setCommandStrings (const vector<string> &commandStrings)
{
    m_commandStrings = commandStrings;
}

vector<string> UnifiedClientGetDataFromClientMessage::getCommandStrings () const
{
    return (m_commandStrings);
}

const void *UnifiedClientGetDataFromClientMessage::getCStructureForInputs ()
{
    UnifiedClientMessageInput_t *pInput  = new UnifiedClientMessageInput_t ();
    pInput->commandString = NULL;
    pInput->inputBuffer = NULL;
    pInput->inputBufferSize = 0;
    
    pInput->numberOfRecordsRequested = m_numberOfRecordsRequested;
    pInput->groupCode = m_commandCode;
    pInput->commandType = m_commandType;

    pInput->commandStringSize = m_numberOfCommandStrings;

    if (0 < m_numberOfCommandStrings)
    {
        //Allocate memory for all commandString
        pInput->commandString = new const char *[m_numberOfCommandStrings];
        prismAssert (NULL != pInput->commandString, __FILE__, __LINE__);

        for (SI32 j = 0; j < m_numberOfCommandStrings; j++)
        {
            pInput->commandString[j] = m_commandStrings[j].c_str();
        }
    }
    else
    {
        pInput->commandString = NULL;
    }

    UI32 size = 0;
    vector<UI32> bufferTags;
    getBufferTags(bufferTags); 

    if ( 1 < bufferTags.size() )
    {
        trace (TRACE_LEVEL_FATAL, string ("UnifiedClientGetDataFromClientMessage::getCStructureForInputs :: Input cannot have more than one buffer. Assert here"));
        prismAssert (false, __FILE__, __LINE__);
    }
    else if ( 0 != bufferTags.size()) 
    {
        pInput->inputBuffer = transferBufferToUser(bufferTags[0],size);
        pInput->inputBufferSize = size;
        removeAllBuffers();
    }
    else
    {
        //trace (TRACE_LEVEL_ERROR, string ("UnifiedClientGetDataFromClientMessage::getCStructureForInputs :: Buffer is NULL "));
    }
    return (pInput);
}

void UnifiedClientGetDataFromClientMessage::loadOutputsFromCStructure (const void *pOutputCStructure )
{
    if ( NULL == pOutputCStructure )
    {
        m_enablePaginate = false;
        return ;
    }

    UnifiedClientMessageOutput_t *pResult = (UnifiedClientMessageOutput_t *)pOutputCStructure;
    prismAssert ( NULL != pResult, __FILE__, __LINE__);
    
    m_enablePaginate = ((pResult->moreRecords == 1)? true : false);

    if ( NULL != pResult->outputBuffer )
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        status = addBuffer(pResult->bufferTag,pResult->outputBufferSize,pResult->outputBuffer, false);
        free( (char*) (pResult->outputBuffer) );
        free (pResult);
        if ( WAVE_MESSAGE_SUCCESS != status) 
        {
            trace (TRACE_LEVEL_FATAL, string ("UnifiedClientGetDataFromClientMessage::loadOutputsFromCStructure :: addBuffer failed with error " + FrameworkToolKit::localize(status)));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        //trace (TRACE_LEVEL_ERROR, string ("UnifiedClientGetDataFromClientMessage::loadOutputsFromCStructure :: Buffer is NULL ")); 
    }
    
}

void UnifiedClientGetDataFromClientMessage::deleteCStructureForInputs (const void *pInputStruct)
{
    if ( NULL != pInputStruct )
    {
        UnifiedClientMessageInput_t *pUnifiedClientMessageInput_t = (UnifiedClientMessageInput_t *)pInputStruct;
        prismAssert ( NULL != pUnifiedClientMessageInput_t, __FILE__, __LINE__);

        if ( 0 < pUnifiedClientMessageInput_t->commandStringSize )
        {
            delete [] (pUnifiedClientMessageInput_t->commandString);
            pUnifiedClientMessageInput_t->commandString = NULL;
        }

        if (NULL != pUnifiedClientMessageInput_t->inputBuffer)
        {
            void *pBuffer = (const_cast<void*> ( pUnifiedClientMessageInput_t->inputBuffer ));
            delete[] (reinterpret_cast<UI8 *> ( pBuffer ));
            pBuffer = NULL;
            pUnifiedClientMessageInput_t->inputBuffer = NULL;
            pUnifiedClientMessageInput_t->inputBufferSize = 0;
        }
        else
        {
            //trace (TRACE_LEVEL_ERROR, string ("UnifiedClientGetDataFromClientMessage::deleteCStructureForInputs : Buffer is NULL "));
        }
        
        delete (pUnifiedClientMessageInput_t);
        pInputStruct = NULL;
    }
}

}
