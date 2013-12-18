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
 **************************************************************************/

#ifndef APPMPORTPROFILEDOMAINMESSAGE_H
#define APPMPORTPROFILEDOMAINMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class APPMPortProfileDomainConfigMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMPortProfileDomainConfigMessage ();
            virtual    ~APPMPortProfileDomainConfigMessage ();
            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            void setPortProfileName(const string &portProfileName);
            string getPortProfileName()  const;
            void setPortProfileDomainName(const string &portProfileDomainName);
            string getPortProfileDomainName() const;

            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;
            string  m_portProfileName;
            string  m_portProfileDomainName;
    };
}
#endif //APPMPORTPROFILEDOMAINMESSAGE_H
