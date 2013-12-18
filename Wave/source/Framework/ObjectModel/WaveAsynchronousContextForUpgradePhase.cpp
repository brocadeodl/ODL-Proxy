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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForUpgradePhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

namespace WaveNs
{

FirmwareVersion::FirmwareVersion ()
{

}

FirmwareVersion::FirmwareVersion (const string &majorNumber, const string &minorNumber, const string &patchString, const string &description)
    :   m_majorNumber (majorNumber),
        m_minorNumber (minorNumber),
        m_patchString (patchString),
        m_description (description)
{

}

FirmwareVersion::~FirmwareVersion ()
{

}

bool FirmwareVersion::isSameVersion (const string &versionString)
{
    return (false);
}

WaveAsynchronousContextForUpgradePhase::WaveAsynchronousContextForUpgradePhase (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : WaveAsynchronousContextForBootPhases (pCaller, pCallback, pCallerContext)
{
}

WaveAsynchronousContextForUpgradePhase::~WaveAsynchronousContextForUpgradePhase ()
{
}

/*
vector<string> WaveAsynchronousContextForUpgradePhase::getNewManagedObjects () const
{
    return (m_newManagedObjects);
}

void WaveAsynchronousContextForUpgradePhase::setNewManagedObjects (const vector<string> &newManagedObjects)
{
    m_newManagedObjects = newManagedObjects;
}

vector<string> WaveAsynchronousContextForUpgradePhase::getChangedManagedObjects () const
{
    return (m_changedManagedObjects);
}

void WaveAsynchronousContextForUpgradePhase::setChangedManagedObjects (const vector<string> &changedManagedObjects)
{
    m_changedManagedObjects = changedManagedObjects;
}

bool WaveAsynchronousContextForUpgradePhase::checkFromVersionString (const string &versionString)
{
    return (m_fromVersion.isSameVersion (versionString));
}
*/

OMSpecificSchemaChangeInfoForUpgrade* WaveAsynchronousContextForUpgradePhase::getSchemaChangeInfo () const
{                          
    return (m_schemaChangeInfo);
}

void WaveAsynchronousContextForUpgradePhase::setSchemaChangeInfo (OMSpecificSchemaChangeInfoForUpgrade* pSchemaChangeInfo)
{   
    m_schemaChangeInfo = pSchemaChangeInfo;
}

bool WaveAsynchronousContextForUpgradePhase::checkToVersionString (const string &versionString)
{
    return (m_toVersion.isSameVersion (versionString));
}


}
