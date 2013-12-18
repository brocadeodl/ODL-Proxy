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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author :  Balaji. M                                                   *
 **************************************************************************/
#if 0
#ifndef SECSERVICESGLOBALCONFIGMESSAGE_H
#define SECSERVICESGLOBALCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class TelnetServerGlobalConfigMessage : public PrismMessage
    {
        public:
            TelnetServerGlobalConfigMessage ();
            TelnetServerGlobalConfigMessage (const bool &disableFlag);
            virtual    ~TelnetServerGlobalConfigMessage ();
            void setDisableFlag(const bool &disableFlag);
            bool getDisableFlag()  const;
            
        protected:
            virtual void setupAttributesForSerialization();

        // Now the data members
        private:
            bool  m_disableFlag;
    };
    
    class SSHServerGlobalConfigMessage : public PrismMessage
    {
        public:
            SSHServerGlobalConfigMessage ();
            SSHServerGlobalConfigMessage (const bool &disableFlag);
            virtual    ~SSHServerGlobalConfigMessage ();
            void setDisableFlag(const bool &disableFlag);
            bool getDisableFlag()  const;

        protected:
            virtual void setupAttributesForSerialization();

        // Now the data members
        private:
            bool  m_disableFlag;
    };

    class SSHServerListGlobalAddMessage : public PrismMessage
    {
        public:
            SSHServerListGlobalAddMessage ();
            SSHServerListGlobalAddMessage (const SSHKeyExchangeProtocol &protocol);
            virtual    ~SSHServerListGlobalAddMessage ();
            void setKeyExchangeProtocol (const SSHKeyExchangeProtocol &protocol);
            SSHKeyExchangeProtocol getKeyExchangeProtocol()  const;

        protected:
            virtual void setupAttributesForSerialization();

        // Now the data members
        private:
            SSHKeyExchangeProtocol m_protocol;
    };

    class SSHServerListGlobalDeleteMessage : public PrismMessage
    {
        public:
            SSHServerListGlobalDeleteMessage ();
            SSHServerListGlobalDeleteMessage (const SSHKeyExchangeProtocol &protocol);
            virtual    ~SSHServerListGlobalDeleteMessage ();
            void setKeyExchangeProtocol (const SSHKeyExchangeProtocol &protocol);
            SSHKeyExchangeProtocol getKeyExchangeProtocol()  const;

        protected:
            virtual void setupAttributesForSerialization();

        // Now the data members
        private:
            SSHKeyExchangeProtocol m_protocol;
    };
}
#endif                  // SECSERVICESGLOBALCONFIGMESSAGE_H
#endif
