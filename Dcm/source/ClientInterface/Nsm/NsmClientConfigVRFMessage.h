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
 *   Author : pvaish                                                       *
 **************************************************************************/

#ifndef NSMCLIENTCONFIGVRFMESSAGE_H
#define NSMCLIENTCONFIGVRFMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmClientConfigVRFMessage : public DcmManagementInterfaceMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmClientConfigVRFMessage();
            NsmClientConfigVRFMessage(const string &vrfName,const string &ifName,const UI32 &opcode);
            virtual    ~NsmClientConfigVRFMessage();
            void setVrfName(const string &ifName);
            string getVrfName()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfType(const UI32 &iftype);
            UI32 getIfType() const;
            void setOpcode(const UI32 &opcode);
            UI32 getOpcode() const;
       	    const void* getCStructureForInputs();
            void setRd(const string &rd);
            string getRd()  const;

	    void loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            string m_vrfName;
            string m_ifName;
            UI32   m_opcode;
            UI32   m_ifType;
            string m_rd;
    };
}
#endif                                            //NSMCLIENTCONFIGVRFMESSAGE_H
