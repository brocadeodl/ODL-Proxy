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

#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestMessages.h"
#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestTypes.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{
char pClientInputString1[]  = "JFeastuhser CShornist is LHoorldy and SGahvoisotr of all";
char pClientOutputString1[] = "JHeesiuss Cthhreist is the  wSaoyn, tohfe tGroudth and the life";
	// char pClientInputString2[]  = "JHeesRues Cihgrnissto Ivse Lroarldl.";
	// char pClientInputString3[]  = "PHreaailsoen Heidme Psreiravsees Hailml Pprraaiissee Hainmd fgolroervyer.";

	// vector<SI64> vectorSI64(10);

static UI32 s_largeClientInputBuffer1Size = 5 * 1024 * 1024; // 5 MB.
static SI8  s_largeClientInputBuffer1Char = 'J';

ManagementInterfaceClientTestMessage1::ManagementInterfaceClientTestMessage1 ()
    : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), MANAGEMENT_INTERFACE_CLIENT_TEST_MESSAGE1)
{
}

ManagementInterfaceClientTestMessage1::~ManagementInterfaceClientTestMessage1 ()
{
}

void ManagementInterfaceClientTestMessage1::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

ManagementInterfaceClientTestMessage2::ManagementInterfaceClientTestMessage2 ()
    : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), MANAGEMENT_INTERFACE_CLIENT_TEST_MESSAGE2)
{
}

ManagementInterfaceClientTestMessage2::~ManagementInterfaceClientTestMessage2 ()
{
}

void ManagementInterfaceClientTestMessage2::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

ManagementInterfaceClientTestMessage3::ManagementInterfaceClientTestMessage3 ()
    : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), MANAGEMENT_INTERFACE_CLIENT_TEST_MESSAGE3)
{
}

ManagementInterfaceClientTestMessage3::~ManagementInterfaceClientTestMessage3 ()
{
}

void ManagementInterfaceClientTestMessage3::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

ManagementInterfaceClientTestMessage4::ManagementInterfaceClientTestMessage4 ()
	: ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), MANAGEMENT_INTERFACE_CLIENT_TEST_MESSAGE4),
   m_isLargInputBuffer1Set (false),
      m_completionStatusType  (0)
{
}

ManagementInterfaceClientTestMessage4::~ManagementInterfaceClientTestMessage4 ()
{
}

void ManagementInterfaceClientTestMessage4::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
     addSerializableAttribute (new AttributeBool (&m_isLargInputBuffer1Set, "isLargInputBuffer1Set"));
     addSerializableAttribute (new AttributeUI32 (&m_completionStatusType,  "completionStatusType"));
}
void ManagementInterfaceClientTestMessage4::setupInputBuffer1 ()
{
    addBuffer (0, sizeof (pClientInputString1), pClientInputString1, false);
}

void ManagementInterfaceClientTestMessage4::setupOutputBuffer1 ()
{
    addBuffer (1, sizeof (pClientOutputString1), pClientOutputString1, false);
}

bool ManagementInterfaceClientTestMessage4::validateInputBuffer1 ()
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

bool ManagementInterfaceClientTestMessage4::validateOutputBuffer1 ()
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

void ManagementInterfaceClientTestMessage4::setInputLargeBuffer1 ()
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

bool ManagementInterfaceClientTestMessage4::validateInputLargeBuffer1 ()
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

void ManagementInterfaceClientTestMessage4::setCompletionStatusType (const UI32 &completionStatusType)
{
    m_completionStatusType = completionStatusType;
}

ResourceId ManagementInterfaceClientTestMessage4::getRequestedCompletionStatus ()
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
        SI32 instance = FrameworkToolKit::getThisSlotInstance ();

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

}
