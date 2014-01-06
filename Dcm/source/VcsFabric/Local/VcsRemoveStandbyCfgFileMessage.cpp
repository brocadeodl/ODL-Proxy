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

/**************************************************************************
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Kuai Yu                                                      *
 ***************************************************************************/


#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsRemoveStandbyCfgFileMessage.h"
#include "Framework/Attributes/Attributes.h"

namespace DcmNs
{

VcsRemoveStandbyCfgFileMessage::VcsRemoveStandbyCfgFileMessage ()
    : WaveServiceIndependentMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_REMOVE_CFG_FILE_SERVICE_INDEPENDENT)
{
}

VcsRemoveStandbyCfgFileMessage::VcsRemoveStandbyCfgFileMessage (const string &distributionMode)
    : WaveServiceIndependentMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_REMOVE_CFG_FILE_SERVICE_INDEPENDENT),
    m_distributionMode (distributionMode)
{
}

VcsRemoveStandbyCfgFileMessage::~VcsRemoveStandbyCfgFileMessage ()
{   
}

void VcsRemoveStandbyCfgFileMessage::setupAttributesForSerialization ()
{
    WaveServiceIndependentMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString     (&m_distributionMode, "distributionMode"));
}

const string& VcsRemoveStandbyCfgFileMessage::getDistributionMode () const
{
    return (m_distributionMode);
}

void VcsRemoveStandbyCfgFileMessage::setDistributionMode (const string &distributionMode)
{
    m_distributionMode = distributionMode;
}

}