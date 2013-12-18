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
 *   Copyright (C) 2006 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Regression/RegressionPrepareMessage.h"

namespace WaveNs
{

RegressionPrepareMessage::RegressionPrepareMessage (PrismServiceId id)
    : PrismMessage (id, WAVE_OBJECT_MANAGER_PREPARE_FOR_REGRESSION)
{
}

RegressionPrepareMessage::RegressionPrepareMessage ()
    : PrismMessage (0, WAVE_OBJECT_MANAGER_PREPARE_FOR_REGRESSION)
{
}

RegressionPrepareMessage::~RegressionPrepareMessage ()
{
}

void RegressionPrepareMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeStringVector (&m_inputStrings, "inputStrings"));
}

void RegressionPrepareMessage::addInputString (const string &inputString)
{
    m_inputStrings.push_back (inputString);
}

vector<string> &RegressionPrepareMessage::getInputStrings ()
{
    return (m_inputStrings);
}

}