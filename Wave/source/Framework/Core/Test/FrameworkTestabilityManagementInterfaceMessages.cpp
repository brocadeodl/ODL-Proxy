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

#include "Framework/Core/Test/FrameworkTestabilityManagementInterfaceMessages.h"
#include "Framework/Core/Test/FrameworkTestabilityTypes.h"
#include "Framework/Core/Test/FrameworkTestability1ObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{
#if 0
char pClientInputString1[]  = "JFeastuhser CShornist is LHoorldy and SGahvoisotr of all";
char pClientOutputString1[] = "JHeesiuss Cthhreist is the  wSaoyn, tohfe tGroudth and the life";
	// char pClientInputString2[]  = "JHeesRues Cihgrnissto Ivse Lroarldl.";
	// char pClientInputString3[]  = "PHreaailsoen Heidme Psreiravsees Hailml Pprraaiissee Hainmd fgolroervyer.";

	// vector<SI64> vectorSI64(10);

static UI32 s_largeClientInputBuffer1Size = 5 * 1024 * 1024; // 5 MB.
static SI8  s_largeClientInputBuffer1Char = 'J';
#endif

FrameworkTestabilityManagementInterfaceMessage1::FrameworkTestabilityManagementInterfaceMessage1 ()
    : ManagementInterfaceMessage (FrameworkTestability1ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE1)
{
}

FrameworkTestabilityManagementInterfaceMessage1::~FrameworkTestabilityManagementInterfaceMessage1 ()
{
}

void FrameworkTestabilityManagementInterfaceMessage1::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

FrameworkTestabilityManagementInterfaceMessage2::FrameworkTestabilityManagementInterfaceMessage2 ()
    : ManagementInterfaceMessage (FrameworkTestability1ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE2)
{
}

FrameworkTestabilityManagementInterfaceMessage2::~FrameworkTestabilityManagementInterfaceMessage2 ()
{
}

void FrameworkTestabilityManagementInterfaceMessage2::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

FrameworkTestabilityManagementInterfaceMessage3::FrameworkTestabilityManagementInterfaceMessage3 ()
    : ManagementInterfaceMessage (FrameworkTestability1ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE3)
{
}

FrameworkTestabilityManagementInterfaceMessage3::~FrameworkTestabilityManagementInterfaceMessage3 ()
{
}

void FrameworkTestabilityManagementInterfaceMessage3::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

FrameworkTestabilityManagementInterfaceMessage4::FrameworkTestabilityManagementInterfaceMessage4 ()
	: ManagementInterfaceMessage (FrameworkTestability1ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE4)
#if 0
,
      m_isLargInputBuffer1Set (false),
      m_completionStatusType  (0)
#endif
{
}

void FrameworkTestabilityManagementInterfaceMessage4::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
     addSerializableAttribute (new AttributeString (&m_clientName, "clientName4"));
#if 0
     addSerializableAttribute (new AttributeBool (&m_isLargInputBuffer1Set, "isLargInputBuffer1Set"));
     addSerializableAttribute (new AttributeUI32 (&m_completionStatusType, "completionStatusType"));
#endif

}

FrameworkTestabilityManagementInterfaceMessage4::~FrameworkTestabilityManagementInterfaceMessage4 ()
{
}

void FrameworkTestabilityManagementInterfaceMessage4::setClientName (const string &clientName)
{
    m_clientName = clientName;
}

string FrameworkTestabilityManagementInterfaceMessage4::getClientName () const
{
    return (m_clientName);
}

#if 0
void FrameworkTestabilityManagementInterfaceMessage4::setupInputBuffer1 ()
{
    addBuffer (0, sizeof (pClientInputString1), pClientInputString1, false);
}

void FrameworkTestabilityManagementInterfaceMessage4::setupOutputBuffer1 ()
{
    addBuffer (1, sizeof (pClientOutputString1), pClientOutputString1, false);
}

bool FrameworkTestabilityManagementInterfaceMessage4::validateInputBuffer1 ()
{
    UI32  bufferSize = 0;
    void *pBuffer    = NULL;

    pBuffer = findBuffer (0, bufferSize);

    prismAssert (NULL != pBuffer, __FILE__, __LINE__);

    if (0 == strcmp (pClientInputString1, reinterpret_cast<char *> (pBuffer)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool FrameworkTestabilityManagementInterfaceMessage4::validateOutputBuffer1 ()
{
    UI32  bufferSize = 0;
    void *pBuffer    = NULL;

    pBuffer = findBuffer (1, bufferSize);

    prismAssert (NULL != pBuffer, __FILE__, __LINE__);

    if (0 == strcmp (pClientOutputString1, reinterpret_cast<char *> (pBuffer)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void FrameworkTestabilityManagementInterfaceMessage4::setInputLargeBuffer1 ()
{
    SI8  *pTempBuffer = new SI8[s_largeClientInputBuffer1Size];
    UI32  i           = 0;

    for (i = 0; i < s_largeClientInputBuffer1Size; i++)
    {
        pTempBuffer[i] = s_largeClientInputBuffer1Char;
    }

    addBuffer (100, s_largeClientInputBuffer1Size, pTempBuffer, true);

    m_isLargInputBuffer1Set = true;
}

bool FrameworkTestabilityManagementInterfaceMessage4::validateInputLargeBuffer1 ()
{
    if (false == m_isLargInputBuffer1Set)
    {
        return (true);
    }

    UI32  bufferSize = 0;
    SI8  *pBuffer    = NULL;
    UI32  i          = 0;

    pBuffer = reinterpret_cast<SI8 *> (findBuffer (100, bufferSize));

    //cout << "Buffer Size : " << bufferSize << endl;

    prismAssert (s_largeClientInputBuffer1Size == bufferSize, __FILE__, __LINE__);
    prismAssert (NULL != pBuffer, __FILE__, __LINE__);

    for (i = 0; i < s_largeClientInputBuffer1Size; i++)
    {
        if (s_largeClientInputBuffer1Char != (pBuffer[i]))
        {
            return (false);
        }
    }

    return (true);
}

void FrameworkTestabilityManagementInterfaceMessage4::setCompletionStatusType (const UI32 &completionStatusType)
{
    m_completionStatusType = completionStatusType;
}

ResourceId FrameworkTestabilityManagementInterfaceMessage4::getRequestedCompletionStatus ()
{
    if (0 == m_completionStatusType)
    {
        return (WAVE_MESSAGE_SUCCESS);
    }
    else if (1 == m_completionStatusType)
    {
        return (WAVE_MESSAGE_ERROR);
    }
    else  if (2 == m_completionStatusType)
    {
        SI32 instance = 0;
        FrameworkToolKit::getThisSlotInstance (instance);

        if (0 == ((instance) % 2))
        {
            return (WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            return (WAVE_MESSAGE_ERROR);
        }
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    return (WAVE_MESSAGE_ERROR);
}
#endif

FrameworkTestabilityManagementInterfaceMessageLeakMessage::FrameworkTestabilityManagementInterfaceMessageLeakMessage ()
    : ManagementInterfaceMessage (FrameworkTestability1ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE_FOR_MEMORY_LEAKS),
      m_prismServiceId           (0)
{
}

FrameworkTestabilityManagementInterfaceMessageLeakMessage::~FrameworkTestabilityManagementInterfaceMessageLeakMessage ()
{
}

void FrameworkTestabilityManagementInterfaceMessageLeakMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_prismServiceId,                "prismServiceId"));
     addSerializableAttribute (new AttributeUI32Vector (&m_messageServiceIds,       "messageServiceIds"));
     addSerializableAttribute (new AttributeUI32Vector (&m_messageOperationCodes,   "messageOperationCodes"));
     addSerializableAttribute (new AttributeUI32Vector (&m_messageTypes,            "messageTypes"));
     addSerializableAttribute (new AttributeStringVector (&m_btStrings,             "btStrings"));
}

void FrameworkTestabilityManagementInterfaceMessageLeakMessage::setMessageArguments (const vector<PrismServiceId>  &messageServiceIds, const vector<UI32> &messageOperationCodes, const vector<WaveMessageType> &messageTypes, const vector<string> &btStrings)
{
    m_messageServiceIds     = messageServiceIds;
    m_messageOperationCodes = messageOperationCodes;
    
    UI32 messageTypeSize = messageTypes.size ();

    for (UI32 i = 0; i < messageTypeSize; i++)
    {
        m_messageTypes.push_back ((UI32) messageTypes[i]);
    }

    m_btStrings             = btStrings;
}

void FrameworkTestabilityManagementInterfaceMessageLeakMessage::getMessageArguments (vector<PrismServiceId>  &messageServiceIds, vector<UI32> &messageOperationCodes, vector<WaveMessageType> &messageTypes, vector<string> &btStrings) const
{
    messageServiceIds     = m_messageServiceIds;
    messageOperationCodes = m_messageOperationCodes;

    UI32 messageTypeSize = m_messageTypes.size ();

    for (UI32 i = 0; i < messageTypeSize; i++)     
    {         
        messageTypes.push_back ((WaveMessageType) m_messageTypes[i]);
    } 

    btStrings             = m_btStrings;
}

void FrameworkTestabilityManagementInterfaceMessageLeakMessage::setPrismServiceId (const PrismServiceId prismServiceId)
{
    m_prismServiceId = prismServiceId;
}

PrismServiceId FrameworkTestabilityManagementInterfaceMessageLeakMessage::getPrismServiceId () const
{
    return(m_prismServiceId);
}

FrameworkTestabilityManagementInterfaceObjectLeakMessage::FrameworkTestabilityManagementInterfaceObjectLeakMessage ()
    : ManagementInterfaceMessage (FrameworkTestability1ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE_FOR_OBJECT_LEAKS),
      m_prismServiceId           (0)
{
}

FrameworkTestabilityManagementInterfaceObjectLeakMessage::~FrameworkTestabilityManagementInterfaceObjectLeakMessage ()
{
}

void FrameworkTestabilityManagementInterfaceObjectLeakMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_prismServiceId,                    "prismServiceId"));
     addSerializableAttribute (new AttributeStringVector (&m_managedObjectClassNames,   "managedObjectClassNames"));
     addSerializableAttribute (new AttributeStringVector (&m_managedObjectNames,        "managedObjectNames"));
     addSerializableAttribute (new AttributeBoolVector (&m_queryResults,                "queryResults"));
     addSerializableAttribute (new AttributeStringVector (&m_btStrings,                 "btStrings"));
}

void FrameworkTestabilityManagementInterfaceObjectLeakMessage::setMessageArguments (const vector<string> &managedObjectClassNames, const vector<string>  &managedObjectNames, const vector<bool> &queryResults, const vector<string> &btStrings)
{
    m_managedObjectClassNames = managedObjectClassNames;
    m_managedObjectNames      = managedObjectNames;
    m_queryResults            = queryResults;
    m_btStrings               = btStrings;
}

void FrameworkTestabilityManagementInterfaceObjectLeakMessage::getMessageArguments (vector<string> &managedObjectClassNames, vector<string>  &managedObjectNames, vector<bool> &queryResults, vector<string> &btStrings) const
{
    managedObjectClassNames = m_managedObjectClassNames;
    managedObjectNames      = m_managedObjectNames;
    queryResults            = m_queryResults;
    btStrings               = m_btStrings;
}

void FrameworkTestabilityManagementInterfaceObjectLeakMessage::setPrismServiceId (PrismServiceId prismServiceId)
{
    m_prismServiceId = prismServiceId;
}

PrismServiceId FrameworkTestabilityManagementInterfaceObjectLeakMessage::getPrismServiceId () const
{
    return (m_prismServiceId);
}

FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage::FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage()
    : ManagementInterfaceMessage (FrameworkTestability1ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_TRACK_LEAKS_WITH_BT),
      m_leakType       (OBJECT_LEAK),
      m_btTrackerState (false)
{
}

FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage::~FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage()
{
}

void FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
     addSerializableAttribute (new AttributeUI32 (&m_leakType,          "leakType"));
     addSerializableAttribute (new AttributeBool (&m_btTrackerState,    "btTrackerState"));
}

void FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage::setLeakType (const UI32 &leakType)
{
    m_leakType = leakType;
}

UI32 FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage::getLeakType () const
{
    return m_leakType;
}

void FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage::setBtTrackerState (const bool &btTrackerState)
{
    m_btTrackerState = btTrackerState;
}

bool FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage::getBtTrackerState () const
{
    return m_btTrackerState;
}

FrameworkTestabilityManagementInterfaceFileServiceUsageMessage::FrameworkTestabilityManagementInterfaceFileServiceUsageMessage()
    : ManagementInterfaceMessage (FrameworkTestability1ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_FILE_SERVICE_USAGE), m_useFileServiceForFileTransfer (true)
{
}

FrameworkTestabilityManagementInterfaceFileServiceUsageMessage::~FrameworkTestabilityManagementInterfaceFileServiceUsageMessage()
{
}

void FrameworkTestabilityManagementInterfaceFileServiceUsageMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
     addSerializableAttribute (new AttributeBool (&m_useFileServiceForFileTransfer, "useFileServiceForFileTransfer"));
}

bool    FrameworkTestabilityManagementInterfaceFileServiceUsageMessage::getFileServiceUseforFileTransferFlag      () const
{
     return (m_useFileServiceForFileTransfer);

}

void    FrameworkTestabilityManagementInterfaceFileServiceUsageMessage::setFileServiceUseforFileTransferFlag (const bool &bFileServiceUseForFileTransfer)
{
     m_useFileServiceForFileTransfer = bFileServiceUseForFileTransfer;
}







}
