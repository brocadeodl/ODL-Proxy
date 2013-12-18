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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Balaji. M                                               *
 **************************************************************************/

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/ClearSessionsLocalMessage.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "AAA/Local/AAALocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

ClearSessionsLocalMessage::ClearSessionsLocalMessage ()
    : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), CLEARSESSIONSLOCAL)
{
}

ClearSessionsLocalMessage::~ClearSessionsLocalMessage ()
{
}

void ClearSessionsLocalMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
}

}
