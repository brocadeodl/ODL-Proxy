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
 *   Author : Ankit Jindal												   *
 ***************************************************************************/

#include "APPM/Global/VcenterGetAllPortProfileListMessage.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "APPM/Global/APPMGlobalTypes.h"

namespace DcmNs
{

 VcenterGetAllPortProfileListMessage::VcenterGetAllPortProfileListMessage()
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),VCENTERGETALLPORTPROFILES)
    {
    }

 VcenterGetAllPortProfileListMessage::VcenterGetAllPortProfileListMessage(const vector<string> &portProfiles)
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),VCENTERGETALLPORTPROFILES),
		m_portProfiles    (portProfiles)
    {
    }


    VcenterGetAllPortProfileListMessage::~VcenterGetAllPortProfileListMessage()
    {
    }

	void  VcenterGetAllPortProfileListMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeStringVector(&m_portProfiles,"portProfiles"));
		addSerializableAttribute (new AttributeString(&m_pattern,"pattern"));
	}

    void  VcenterGetAllPortProfileListMessage::setPortProfiles(const vector<string> &portProfiles)
    {
        m_portProfiles  =  portProfiles;
    }

    vector<string>  VcenterGetAllPortProfileListMessage::getPortProfiles() const
    {
        return (m_portProfiles);
    }

    void  VcenterGetAllPortProfileListMessage::setPattern(string pattern)
    {
        m_pattern  =  pattern;
    }

    string  VcenterGetAllPortProfileListMessage::getPattern() 
    {
        return (m_pattern);
    }

}
