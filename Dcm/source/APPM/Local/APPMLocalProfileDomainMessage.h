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

#ifndef APPMLOCALPROFILEDOMAINMESSAGE_H
#define APPMLOCALPROFILEDOMAINMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs {

    class APPMLocalProfileDomainMessage : public PrismMessage {

        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMLocalProfileDomainMessage ();
            virtual    ~APPMLocalProfileDomainMessage ();
            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            void setPortProfileDomainName(const string &portProfileDomainName);
            string getPortProfileDomainName()  const;
            void setPortProfileName(const string &portProfileName);
            string getPortProfileName()  const;

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;
            string m_portProfileDomainName;
            string  m_portProfileName;
    };
}
#endif                                            //APPMLOCALPROFILEDOMAINMESSAGE_H

