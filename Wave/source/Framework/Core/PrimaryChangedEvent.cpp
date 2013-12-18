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
 *  @file   : PrimaryChangedEvent.cpp                                      *
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Description : This file defines an event PrimaryChangedEvent which is *
 *                 broadcasted from new Primary and all secondaries in a   *
 *                 cluster when old Primary is failing over. This event is *
 *                 specifically for Plug-ins.                              *
 *   Author      : Pritee Ghosh                                            *
 *   Date        : 03/02/2011                                              *
 ***************************************************************************/


#include "Framework/Core/PrimaryChangedEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

PrimaryChangedEvent::PrimaryChangedEvent ()
    : PrismEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_PRIMARY_CHANGED_EVENT),
      m_newPrimaryLocationId (0)
{
}

PrimaryChangedEvent::~PrimaryChangedEvent ()
{
}

void PrimaryChangedEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();
}

LocationId PrimaryChangedEvent::getNewPrimaryLocationId () const
{
    return m_newPrimaryLocationId;
}

void PrimaryChangedEvent::setNewPrimaryLocationId (const LocationId location)
{
    m_newPrimaryLocationId = location;
}
}
