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

#ifndef VRRPGLOBALCONFIGSPMESSAGE_H
#define VRRPGLOBALCONFIGSPMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class VRRPGlobalConfigSPMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            VRRPGlobalConfigSPMessage ();
            VRRPGlobalConfigSPMessage (const bool &config_disable, const bool &vrrp_enable,const bool &vrrpe_enable);
            virtual    ~VRRPGlobalConfigSPMessage ();
            void setConfigDisable(const bool &config_disable);
            bool getConfigDisable()  const;
            void setVrrpEnable(const bool &vrrp_enable);
            bool getVrrpEnable()  const;
            void setVrrpeEnable(const bool &vrrpe_enable);
            bool getVrrpeEnable()  const;
            virtual const void *getCStructureForInputs();
            virtual void  loadOutputsFromCStructure(const void *pOutputCStructure);


// Now the data members

        private:
        protected:
        public:
			bool  m_config_disable;
            bool  m_vrrp_enable;
            bool  m_vrrpe_enable;
    };
}
#endif                                            //VRRPGLOBALCONFIGSPMESSAGE_H
