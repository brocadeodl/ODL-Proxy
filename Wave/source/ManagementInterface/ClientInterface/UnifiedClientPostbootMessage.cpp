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
 *   Author : Anil C                                                       *
 **************************************************************************/

#include "ManagementInterface/ClientInterface/UnifiedClientPostbootMessage.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "Framework/Attributes/AttributeStringVectorVector.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

UnifiedClientPostbootMessage::UnifiedClientPostbootMessage ()
    : UnifiedClientUpdateMessage (UNIFIEDCLIENTPOSTBOOT)
{
}

UnifiedClientPostbootMessage::UnifiedClientPostbootMessage (const GetHardwareConfigurationDetailsForPostbootContext *pContext)
   : UnifiedClientUpdateMessage (UNIFIEDCLIENTPOSTBOOT, pContext)
{
}

UnifiedClientPostbootMessage::~UnifiedClientPostbootMessage ()
{
}

void UnifiedClientPostbootMessage::setupAttributesForSerialization()
{   
    UnifiedClientUpdateMessage::setupAttributesForSerialization ();
}

void UnifiedClientPostbootMessage::initializeFromContext( const WaveManagedObjectAsynchronousPluginContext *pContext )
{
    UnifiedClientUpdateMessage::initializeFromContext( pContext );
}

const void *UnifiedClientPostbootMessage::getCStructureForInputs ()
{  
    UnifiedClientMessage_t *pInput                  = (UnifiedClientMessage_t *) (UnifiedClientUpdateMessage::getCStructureForInputs ()); 
    return (pInput);
}

void UnifiedClientPostbootMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
{
}

}
