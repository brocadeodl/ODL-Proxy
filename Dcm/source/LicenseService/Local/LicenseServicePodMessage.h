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

#ifndef LICENSESERVICEPODMESSAGE_H
#define LICENSESERVICEPODMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LicenseServicePodMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LicenseServicePodMessage ();
            LicenseServicePodMessage (const string &option,const string &interface,const string &swSlotPort);
            virtual    ~LicenseServicePodMessage ();
            void setOption(const string &option);
            string getOption()  const;
            void setInterface(const string &interface);
            string getInterface()  const;
            void setSwSlotPort(const string &swSlotPort);
            string getSwSlotPort()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_option;
            string  m_interface;
            string  m_swSlotPort;
    };
}
#endif                                            //LICENSESERVICEPODMESSAGE_H
