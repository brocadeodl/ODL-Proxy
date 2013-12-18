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
 *   Author : krao                                                     *
 **************************************************************************/

#ifndef VRRPGLOBALUPDATEVRRPGLOBALCONFIGMESSAGE_H
#define VRRPGLOBALUPDATEVRRPGLOBALCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class VRRPGlobalUpdateVRRPGlobalConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            VRRPGlobalUpdateVRRPGlobalConfigMessage ();
            VRRPGlobalUpdateVRRPGlobalConfigMessage (const bool &config_disable, const bool &enable, const bool &vrrpe_enable);
            virtual    ~VRRPGlobalUpdateVRRPGlobalConfigMessage ();
            void setConfigDisable(const bool &config_disable);
            bool getConfigDisable()  const;
            void setVrrpEnable(const bool &enable);
            bool getVrrpEnable()  const;
            void setVrrpeEnable(const bool &vrrpe_enable);
            bool getVrrpeEnable()  const;

// Now the data members

        private:
        protected:
        public:
            bool  m_config_disable;
            bool  m_vrrp_enable;
            bool  m_vrrpe_enable;
    };
}
#endif                                            //VRRPGLOBALUPDATEVRRPGLOBALCONFIGMESSAGE_H
