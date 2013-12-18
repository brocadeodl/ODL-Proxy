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
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#include "Vrf/Local/VrfLocalMessages.h"
#include "Vrf/Local/VrfLocalTypes.h"
#include "Vrf/Local/VrfLocalObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"

namespace DcmNs
{

VrfLocalAddVrfMessage::VrfLocalAddVrfMessage ()
    : ManagementInterfaceMessage (VrfLocalObjectManager::getServiceName (), VRF_CREATE),
      m_vrfName            ("")
{
    m_vrfIpv4Flag = true;
    m_maxRoutes = RTM_DEFAULT_MAX_ROUTES;
}

VrfLocalAddVrfMessage::VrfLocalAddVrfMessage (const string &vrfName)
    : ManagementInterfaceMessage (VrfLocalObjectManager::getServiceName (), VRF_CREATE),
      m_vrfName            (vrfName)
{
    m_vrfIpv4Flag = true;
    m_maxRoutes = RTM_DEFAULT_MAX_ROUTES;
}

VrfLocalAddVrfMessage::~VrfLocalAddVrfMessage ()
{
}

void VrfLocalAddVrfMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString      (&m_vrfName,    "vrfName"));
     addSerializableAttribute (new AttributeString      (&m_rd,         "rd"));
     addSerializableAttribute (new AttributeIpV4Address (&m_ip,         "ip"));
     addSerializableAttribute (new AttributeUI32        (&m_cmdCode,    "cmdCode"));
     addSerializableAttribute (new AttributeBool        (&m_vrfIpv4Flag,"vrfIpv4Flag"));
     addSerializableAttribute (new AttributeUI32        (&m_maxRoutes,  "maxRoutes"));
}

string VrfLocalAddVrfMessage::getVrfName(void) const
{
    return m_vrfName;
}

void   VrfLocalAddVrfMessage::setVrfName(const string &vrfName)
{
    m_vrfName = vrfName;
}

VrfLocalDeleteVrfMessage::VrfLocalDeleteVrfMessage ()
    : ManagementInterfaceMessage (VrfLocalObjectManager::getServiceName (), VRF_DELETE),
      m_vrfName                  ("")
{
}

VrfLocalDeleteVrfMessage::VrfLocalDeleteVrfMessage (const string &vrfName)
    : ManagementInterfaceMessage (VrfLocalObjectManager::getServiceName (), VRF_DELETE),
      m_vrfName                  (vrfName)
{
}

VrfLocalDeleteVrfMessage::~VrfLocalDeleteVrfMessage ()
{
}

void VrfLocalDeleteVrfMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_vrfName, "vrfName"));
}

string VrfLocalDeleteVrfMessage::getVrfName(void) const
{
    return m_vrfName;
}

void   VrfLocalDeleteVrfMessage::setVrfName(const string &vrfName)
{
    m_vrfName = vrfName;
}

}

