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
 *   Author : jvenkata                                                     *
 **************************************************************************/

#ifndef INTERFACEPRIORITYCONFIGUREINTERFACEPRIORITYMESSAGE_H
#define INTERFACEPRIORITYCONFIGUREINTERFACEPRIORITYMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class InterfacePriorityConfigureInterfacePriorityMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            InterfacePriorityConfigureInterfacePriorityMessage ();
            InterfacePriorityConfigureInterfacePriorityMessage (const SI32 &vlanPriority,const SI32 &ethernetPriority);
            virtual    ~InterfacePriorityConfigureInterfacePriorityMessage ();
            void setVlanPriority(const SI32 &vlanPriority);
            SI32 getVlanPriority()  const;
            void setEthernetPriority(const SI32 &ethernetPriority);
            SI32 getEthernetPriority()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_vlanPriority;
            SI32  m_ethernetPriority;
    };
}
#endif                                            //INTERFACEPRIORITYCONFIGUREINTERFACEPRIORITYMESSAGE_H
