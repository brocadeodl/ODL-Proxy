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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Regression/RegressionServiceMessages.h"
#include "Regression/RegressionTestTypes.h"
#include "Regression/RegressionTestObjectManager.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

RegressionTestObjectManagerGetTestServiceEntriesMessage::RegressionTestObjectManagerGetTestServiceEntriesMessage ()
    : ManagementInterfaceMessage (RegressionTestObjectManager::getServiceName (), REGRESSION_GET_TEST_SERVICE_ENTRIES),
      m_numberOfTestServiceEntries (0)
{
}

RegressionTestObjectManagerGetTestServiceEntriesMessage::~RegressionTestObjectManagerGetTestServiceEntriesMessage ()
{
}

void RegressionTestObjectManagerGetTestServiceEntriesMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_numberOfTestServiceEntries,    "numberOfTestServiceEntries"));
     addSerializableAttribute (new AttributeUI32Vector (&m_testServiceIds,          "testServiceIds"));
     addSerializableAttribute (new AttributeBoolVector (&m_testServiceStates,       "testServiceStates"));
}

void RegressionTestObjectManagerGetTestServiceEntriesMessage::setNumberOfTestServiceEntries (UI32 numberOfTestServiceEntries)
{
    m_numberOfTestServiceEntries = numberOfTestServiceEntries;
}

void RegressionTestObjectManagerGetTestServiceEntriesMessage::addTestServiceEntry (const PrismServiceId &prismServiceId, const bool &isServiceEnabled)
{
    m_testServiceIds.push_back (prismServiceId);
    m_testServiceStates.push_back (isServiceEnabled);
}

UI32 RegressionTestObjectManagerGetTestServiceEntriesMessage::getNumberOfTestServiceEntries ()
{
    return (m_numberOfTestServiceEntries);
}

RegressionTestEntry RegressionTestObjectManagerGetTestServiceEntriesMessage::getTestServiceEntryAt (UI32 index)
{
    if (index < m_numberOfTestServiceEntries)
    {
        prismAssert ((m_testServiceIds.size ()) > index, __FILE__, __LINE__);
        prismAssert ((m_testServiceStates.size ()) > index, __FILE__, __LINE__);

        RegressionTestEntry tempEntry (m_testServiceIds[index], m_testServiceStates[index]);
        return (tempEntry);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
        RegressionTestEntry tempEntry (0, false);
        return (tempEntry);
    }
}

RegressionTestObjectManagerSetTestServiceStateMessage::RegressionTestObjectManagerSetTestServiceStateMessage ()
    : PrismMessage    (RegressionTestObjectManager::getPrismServiceId (), REGRESSION_SET_TEST_SERVICE_STATE),
      m_testServiceId (0),
      m_isTestEnabled (false)
{
}

RegressionTestObjectManagerSetTestServiceStateMessage::RegressionTestObjectManagerSetTestServiceStateMessage (PrismServiceId testServiceId, bool isTestEnabled)
    : PrismMessage (RegressionTestObjectManager::getPrismServiceId (), REGRESSION_SET_TEST_SERVICE_STATE),
      m_testServiceId (testServiceId),
      m_isTestEnabled (isTestEnabled)
{
}

RegressionTestObjectManagerSetTestServiceStateMessage::~RegressionTestObjectManagerSetTestServiceStateMessage ()
{
}

void RegressionTestObjectManagerSetTestServiceStateMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributePrismServiceId (&m_testServiceId, "testServiceId"));
     addSerializableAttribute (new AttributeBool (&m_isTestEnabled,           "isTestEnabled"));
}

PrismServiceId RegressionTestObjectManagerSetTestServiceStateMessage::getTestServiceId ()
{
    return (m_testServiceId);
}

bool RegressionTestObjectManagerSetTestServiceStateMessage::getIsTestEnabled ()
{
    return (m_isTestEnabled);
}

RegressionTestObjectManagerStartRegressionMessage::RegressionTestObjectManagerStartRegressionMessage ()
    : PrismMessage (RegressionTestObjectManager::getPrismServiceId (), REGRESSION_START_REGRESSION),
      m_numberOfTimesToRunRegression (1)
{
}

RegressionTestObjectManagerStartRegressionMessage::RegressionTestObjectManagerStartRegressionMessage (UI32 numberOfTimesToRunRegression)
    : PrismMessage (RegressionTestObjectManager::getPrismServiceId (), REGRESSION_START_REGRESSION),
      m_numberOfTimesToRunRegression (numberOfTimesToRunRegression)
{
}

RegressionTestObjectManagerStartRegressionMessage::~RegressionTestObjectManagerStartRegressionMessage ()
{
}

void RegressionTestObjectManagerStartRegressionMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_numberOfTimesToRunRegression, "numberOfTimesToRunRegression"));
}

UI32 RegressionTestObjectManagerStartRegressionMessage::getNumberOfTimesToRunRegression ()
{
    return (m_numberOfTimesToRunRegression);
}

RegressionTestObjectManagerRunTestForAServiceMessage::RegressionTestObjectManagerRunTestForAServiceMessage ()
    : ManagementInterfaceMessage (RegressionTestObjectManager::getServiceName (), REGRESSION_RUN_TEST_FOR_A_SERVICE),
      m_serviceCode (0),
      m_numberOfTimesToRunServiceTest (1)
{
}

RegressionTestObjectManagerRunTestForAServiceMessage::RegressionTestObjectManagerRunTestForAServiceMessage (PrismServiceId serviceCode)
    : ManagementInterfaceMessage (RegressionTestObjectManager::getServiceName (), REGRESSION_RUN_TEST_FOR_A_SERVICE),
      m_serviceCode (serviceCode),
      m_numberOfTimesToRunServiceTest (1)
{
}

RegressionTestObjectManagerRunTestForAServiceMessage::RegressionTestObjectManagerRunTestForAServiceMessage (PrismServiceId serviceCode, UI32 numberOfTimesToRunServiceTest)
    : ManagementInterfaceMessage (RegressionTestObjectManager::getServiceName (), REGRESSION_RUN_TEST_FOR_A_SERVICE),
      m_serviceCode (serviceCode),
      m_numberOfTimesToRunServiceTest (numberOfTimesToRunServiceTest)
{
}

RegressionTestObjectManagerRunTestForAServiceMessage::~RegressionTestObjectManagerRunTestForAServiceMessage ()
{
}

void RegressionTestObjectManagerRunTestForAServiceMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributePrismServiceId (&m_serviceCode,         "serviceCode"));
     addSerializableAttribute (new AttributeUI32 (&m_numberOfTimesToRunServiceTest, "numberOfTimesToRunServiceTest"));
}

PrismServiceId RegressionTestObjectManagerRunTestForAServiceMessage::getServiceCode ()
{
    return (m_serviceCode);
}

UI32 RegressionTestObjectManagerRunTestForAServiceMessage::getNumberOfTimesToRunServiceTest ()
{
    return (m_numberOfTimesToRunServiceTest);
}


RegressionTestObjectManagerPrepareTestForAServiceMessage::RegressionTestObjectManagerPrepareTestForAServiceMessage ()
    : ManagementInterfaceMessage (RegressionTestObjectManager::getServiceName (), REGRESSION_PREPARE_TEST_FOR_A_SERVICE),
      m_serviceCode (0)
{
}

RegressionTestObjectManagerPrepareTestForAServiceMessage::RegressionTestObjectManagerPrepareTestForAServiceMessage (PrismServiceId serviceCode)
    : ManagementInterfaceMessage (RegressionTestObjectManager::getServiceName (), REGRESSION_PREPARE_TEST_FOR_A_SERVICE),
      m_serviceCode (serviceCode)
{
}

RegressionTestObjectManagerPrepareTestForAServiceMessage::RegressionTestObjectManagerPrepareTestForAServiceMessage (PrismServiceId serviceCode, vector<string> inputStrings)
    : ManagementInterfaceMessage (RegressionTestObjectManager::getServiceName (), REGRESSION_PREPARE_TEST_FOR_A_SERVICE),
      m_serviceCode (serviceCode),
      m_inputStrings (inputStrings)
{
}

RegressionTestObjectManagerPrepareTestForAServiceMessage::~RegressionTestObjectManagerPrepareTestForAServiceMessage ()
{
}

void RegressionTestObjectManagerPrepareTestForAServiceMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributePrismServiceId (&m_serviceCode, "serviceCode"));
     addSerializableAttribute (new AttributeStringVector (&m_inputStrings,  "inputStrings"));
}

PrismServiceId RegressionTestObjectManagerPrepareTestForAServiceMessage::getServiceCode ()
{
    return (m_serviceCode);
}

vector<string> &RegressionTestObjectManagerPrepareTestForAServiceMessage::getInputStrings ()
{
    return (m_inputStrings);
}

}
