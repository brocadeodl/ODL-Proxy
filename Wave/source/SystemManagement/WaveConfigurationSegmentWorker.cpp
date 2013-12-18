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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "SystemManagement/WaveConfigurationSegmentWorker.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "SystemManagement/WaveConfigurationSegmentWorkerMap.h"

namespace WaveNs
{

WaveConfigurationSegmentWorker::WaveConfigurationSegmentWorker (WaveObjectManager *pWaveObjectManager, const string &configurationSegmentName, const bool isConfigExistValidationRequired)
    :   WaveWorker                  (WaveSystemManagementObjectManager::getInstance ()),
        m_configurationSegmentName      (configurationSegmentName),
        m_isConfigExistValidationRequired    (isConfigExistValidationRequired)
{
    WaveConfigurationSegmentWorkerMap::addConfigurationSegmentWorkerIntoMap (m_configurationSegmentName, this);
}        

WaveConfigurationSegmentWorker::~WaveConfigurationSegmentWorker ()
{
}

void WaveConfigurationSegmentWorker::setConfigurationSegmentName (const string &configurationSegmentName)
{
    m_configurationSegmentName = configurationSegmentName;
}

string WaveConfigurationSegmentWorker::getConfigurationSegmentName () const
{
    return (m_configurationSegmentName);
}

ManagementInterfaceMessage *WaveConfigurationSegmentWorker::createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext)
{
    trace (TRACE_LEVEL_INFO, "WaveConfigurationSegmentWorker::createConfig : Calling the base class function, returning NULL");

    return (NULL);
}

string WaveConfigurationSegmentWorker::getErrorString (ManagementInterfaceMessage* pMessage)
{
    return FrameworkToolKit::localize(pMessage->getCompletionStatus());
}

void WaveConfigurationSegmentWorker::createConfigValidation ()
{
}

ManagementInterfaceMessage *WaveConfigurationSegmentWorker::updateConfig () 
{
    return (NULL);
}

void WaveConfigurationSegmentWorker::updateConfigValidation () 
{
}
        
ManagementInterfaceMessage *WaveConfigurationSegmentWorker::deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext)
{
    return (NULL);
}

void WaveConfigurationSegmentWorker::deleteConfigValidation () 
{
}
        
bool WaveConfigurationSegmentWorker::getIsConfigExistValidationRequired () const
{
    return (m_isConfigExistValidationRequired);
}

void  WaveConfigurationSegmentWorker::setIsConfigExistValidationRequired (const bool isConfigExistValidationRequired)
{
	m_isConfigExistValidationRequired = isConfigExistValidationRequired;
}
}

