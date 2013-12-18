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
 *   Author : jvenkata                                                     *
 **************************************************************************/

#ifndef INTERFACENODESPECIFICCONFIGUREMESSAGE_H
#define INTERFACENODESPECIFICCONFIGUREMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class InterfaceNodeSpecificConfigureMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            InterfaceNodeSpecificConfigureMessage ();
            InterfaceNodeSpecificConfigureMessage (const SI32 &vlanClass,const SI32 &ethernetClass);
            virtual    ~InterfaceNodeSpecificConfigureMessage ();
            void setNsVlan (const SI32 &nsVlan);
            SI32 getNsVlan ()  const;
            void setNsEthernet (const SI32 &nsEthernet);
            SI32 getNsEthernet ()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_nsVlan;
            SI32  m_nsEthernet;
    };
}
#endif                                            //INTERFACECLASSCONFIGUREINTERFACECLASSMESSAGE_H
