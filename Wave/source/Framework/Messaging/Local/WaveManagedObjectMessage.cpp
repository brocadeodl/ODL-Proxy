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

#include "Framework/Messaging/Local/WaveManagedObjectMessage.h"

namespace WaveNs
{

WaveManagedObjectMessage::WaveManagedObjectMessage (PrismServiceId serviceCode, UI32 operationCode)
    : PrismMessage (serviceCode, operationCode)
{
}

WaveManagedObjectMessage::WaveManagedObjectMessage (PrismServiceId serviceCode, UI32 operationCode, const ObjectId &waveManagedObjectId)
    : PrismMessage          (serviceCode, operationCode),
      m_waveManagedObjectId (waveManagedObjectId)
{
}

WaveManagedObjectMessage::~WaveManagedObjectMessage ()
{
}

void WaveManagedObjectMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeObjectId (&m_waveManagedObjectId, "waveManagedObjectId"));
}

}
