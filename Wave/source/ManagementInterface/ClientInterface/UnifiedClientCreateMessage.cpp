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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 **************************************************************************/

#include "ManagementInterface/ClientInterface/UnifiedClientCreateMessage.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

UnifiedClientCreateMessage::UnifiedClientCreateMessage ()
    : UnifiedClientUpdateMessage (UNIFIEDCLIENTCREATE)
{
}

UnifiedClientCreateMessage::UnifiedClientCreateMessage (const WaveManagedObjectAsynchronousPluginContext *pWaveManagedObjectAsynchronousPluginContext)
   : UnifiedClientUpdateMessage (UNIFIEDCLIENTCREATE, pWaveManagedObjectAsynchronousPluginContext)
{
}

UnifiedClientCreateMessage::~UnifiedClientCreateMessage ()
{
}

void UnifiedClientCreateMessage::setupAttributesForSerialization()
{
    UnifiedClientUpdateMessage::setupAttributesForSerialization ();
}

const void *UnifiedClientCreateMessage::getCStructureForInputs ()
{
    UnifiedClientMessage_t *pInput                  = (UnifiedClientMessage_t *) (UnifiedClientUpdateMessage::getCStructureForInputs ()); 

    return (pInput);
}

void UnifiedClientCreateMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
{
    UnifiedClientUpdateMessage::loadOutputsFromCStructure (pOutputCStructure);
}


}
