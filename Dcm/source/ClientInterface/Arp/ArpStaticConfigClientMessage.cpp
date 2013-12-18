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
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Arp/ArpStaticConfigClientMessage.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    ArpStaticConfigClientMessage::ArpStaticConfigClientMessage (const IpV4Address &IpAddress,const string &MacAdress,const UI32 &InterfaceType,const UI32 &PortChannelValue,const string &GigabitValue,const string &TenGigabitValue,const string &FortyGigabitValue, const UI32 &VlanValue, const bool &opCode)
        : DcmManagementInterfaceMessage (ARPSTATICCONFIGCLIENT),
        m_IpAddress    (IpAddress),
        m_MacAdress    (MacAdress),
        m_InterfaceType    (InterfaceType),
        m_PortChannelValue    (PortChannelValue),
		m_GigabitValue     (GigabitValue),
		m_TenGigabitValue     (TenGigabitValue),
		m_FortyGigabitValue     (FortyGigabitValue),
		m_VlanValue           (VlanValue),
        m_opCode    (opCode)
    {
    }


}
