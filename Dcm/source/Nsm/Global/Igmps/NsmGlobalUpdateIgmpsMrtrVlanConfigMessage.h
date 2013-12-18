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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#ifndef NSMGLOBALUPDATEIGMPSMRTRVLANCONFIGMESSAGE_H
#define NSMGLOBALUPDATEIGMPSMRTRVLANCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalUpdateIgmpsMrtrVlanConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalUpdateIgmpsMrtrVlanConfigMessage ();
            NsmGlobalUpdateIgmpsMrtrVlanConfigMessage (const UI32 &ifType,const string &ifName,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const bool &negation);
            virtual    ~NsmGlobalUpdateIgmpsMrtrVlanConfigMessage ();
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId()  const;
            void setVlanIfName(const string &vlanIfName);
            string getVlanIfName()  const;
            void setVlanIfIndex(const UI64 &vlanIfIndex);
            UI64 getVlanIfIndex()  const;
            void setNegation(const bool &negation);
            bool getNegation()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_ifType;
            string  m_ifName;
            UI32  m_vlanId;
            string  m_vlanIfName;
            UI64  m_vlanIfIndex;
            bool  m_negation;
    };
}
#endif                                            //NSMGLOBALUPDATEIGMPSMRTRVLANCONFIGMESSAGE_H
