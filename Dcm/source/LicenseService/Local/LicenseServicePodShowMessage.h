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

#ifndef LICENSESERVICEPODSHOWMESSAGE_H
#define LICENSESERVICEPODSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LicenseServicePodShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LicenseServicePodShowMessage ();
            LicenseServicePodShowMessage (const vector<string> &podstr,const vector<UI32> &max_DPOD_Ports,const vector<UI32> &num_DPOD_Licenses,const vector<UI32> &max_DPOD_provisioned,const vector<UI32> &reserved_ports,const vector<UI32> &released_ports);
            virtual    ~LicenseServicePodShowMessage ();
            void setPodstr(const vector<string> &podstr);
            vector<string> getPodstr()  const;
            void setMax_DPOD_Ports(const vector<UI32> &max_DPOD_Ports);
            vector<UI32> getMax_DPOD_Ports()  const;  
            void setNum_DPOD_Licenses(const vector<UI32> &num_DPOD_Licenses);
            vector<UI32> getNum_DPOD_Licenses()  const;  
            void setMax_DPOD_provisioned(const vector<UI32> &max_DPOD_provisioned);
            vector<UI32> getMax_DPOD_provisioned()  const;  
            void setReserved_ports(const vector<UI32> &reserved_ports);
            vector<UI32> getReserved_ports()  const;  
            void setReleased_ports(const vector<UI32> &released_ports);
            vector<UI32> getReleased_ports()  const;  

// Now the data members

        private:
        protected:
        public:
            vector<string>  m_podstr;
            vector<UI32>  m_max_DPOD_Ports;
            vector<UI32>  m_num_DPOD_Licenses;
            vector<UI32>  m_max_DPOD_provisioned;
            vector<UI32>  m_reserved_ports;
            vector<UI32>  m_released_ports;
    };
}
#endif                                            //LICENSESERVICEPODSHOWMESSAGE_H
