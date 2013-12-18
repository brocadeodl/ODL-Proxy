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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include "LicenseService/Local/LicenseServiceLocalObjectManager.h"
#include "LicenseService/Local/LicenseServicePodShowMessage.h"
#include "LicenseService/Local/LicenseServiceTypes.h"

namespace DcmNs
{

    LicenseServicePodShowMessage::LicenseServicePodShowMessage ()
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEPODSHOW)
    {
    }

    LicenseServicePodShowMessage::LicenseServicePodShowMessage (const vector<string> &podstr,const vector<UI32> &max_DPOD_Ports,const vector<UI32> &num_DPOD_Licenses,const vector<UI32> &max_DPOD_provisioned,const vector<UI32> &reserved_ports,const vector<UI32> &released_ports)
        : PrismMessage (LicenseServiceLocalObjectManager::getPrismServiceId (), LICENSESERVICEPODSHOW),
        m_podstr    (podstr),
        m_max_DPOD_Ports    (max_DPOD_Ports),
        m_num_DPOD_Licenses    (num_DPOD_Licenses),
        m_max_DPOD_provisioned    (max_DPOD_provisioned),
        m_reserved_ports    (reserved_ports),
        m_released_ports    (released_ports)
    {
    }

    LicenseServicePodShowMessage::~LicenseServicePodShowMessage ()
    {
    }

    void  LicenseServicePodShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeStringVector(&m_podstr,"podstr"));
        addSerializableAttribute (new AttributeUI32Vector(&m_max_DPOD_Ports,"max_DPOD_Ports"));
        addSerializableAttribute (new AttributeUI32Vector(&m_num_DPOD_Licenses,"num_DPOD_Licenses"));
        addSerializableAttribute (new AttributeUI32Vector(&m_max_DPOD_provisioned,"max_DPOD_provisioned"));
        addSerializableAttribute (new AttributeUI32Vector(&m_reserved_ports,"reserved_ports"));
        addSerializableAttribute (new AttributeUI32Vector(&m_released_ports,"released_ports"));
    }

    void  LicenseServicePodShowMessage::setPodstr(const vector<string> &podstr)
    {
        m_podstr  =  podstr;
    }

    vector<string>  LicenseServicePodShowMessage::getPodstr() const
    {
        return (m_podstr);
    }

    void  LicenseServicePodShowMessage::setMax_DPOD_Ports(const vector<UI32> &max_DPOD_Ports)
    {
        m_max_DPOD_Ports  =  max_DPOD_Ports;
    }

    vector<UI32>  LicenseServicePodShowMessage::getMax_DPOD_Ports() const
    {
        return (m_max_DPOD_Ports);
    }

    void  LicenseServicePodShowMessage::setNum_DPOD_Licenses(const vector<UI32> &num_DPOD_Licenses)
    {
        m_num_DPOD_Licenses  =  num_DPOD_Licenses;
    }

    vector<UI32>  LicenseServicePodShowMessage::getNum_DPOD_Licenses() const
    {
        return (m_num_DPOD_Licenses);
    }

    void  LicenseServicePodShowMessage::setMax_DPOD_provisioned(const vector<UI32> &max_DPOD_provisioned)
    {
        m_max_DPOD_provisioned  =  max_DPOD_provisioned;
    }

    vector<UI32>  LicenseServicePodShowMessage::getMax_DPOD_provisioned() const
    {
        return (m_max_DPOD_provisioned);
    }

    void  LicenseServicePodShowMessage::setReserved_ports(const vector<UI32> &reserved_ports)
    {
        m_reserved_ports  =  reserved_ports;
    }

    vector<UI32>  LicenseServicePodShowMessage::getReserved_ports() const
    {
        return (m_reserved_ports);
    }

    void  LicenseServicePodShowMessage::setReleased_ports(const vector<UI32> &released_ports)
    {
        m_released_ports  =  released_ports;
    }

    vector<UI32>  LicenseServicePodShowMessage::getReleased_ports() const
    {
        return (m_released_ports);
    }

}
