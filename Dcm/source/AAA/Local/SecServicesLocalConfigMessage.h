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

#ifndef SECSERVICESLOCALCONFIGMESSAGE_H
#define SECSERVICESLOCALCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include "AAA/Local/AAALocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class TelnetServerLocalConfigMessage : public PrismMessage
    {
        public:
            TelnetServerLocalConfigMessage ();
            TelnetServerLocalConfigMessage (const bool &disableFlag);
            virtual    ~TelnetServerLocalConfigMessage ();
            void setDisableFlag(const bool &disableFlag);
            bool getDisableFlag()  const;

        protected:
            virtual void setupAttributesForSerialization();

        // Now the data members
        private:
            bool  m_disableFlag;
    };
    
    class SSHServerLocalConfigMessage : public PrismMessage
    {
        public:
            SSHServerLocalConfigMessage ();
            SSHServerLocalConfigMessage (const bool &disableFlag);
            virtual    ~SSHServerLocalConfigMessage ();
            void setDisableFlag(const bool &disableFlag);
            bool getDisableFlag()  const;

        protected:
            virtual void setupAttributesForSerialization();

        // Now the data members
        private:
            bool  m_disableFlag;
    };

    class SSHServerListLocalAddMessage : public PrismMessage
    {
        public:
            SSHServerListLocalAddMessage ();
            SSHServerListLocalAddMessage (const SSHLocalKeyExchangeProtocol &protocol);
            virtual    ~SSHServerListLocalAddMessage ();
            void setKeyExchangeProtocol(const SSHLocalKeyExchangeProtocol &protocol);
            SSHLocalKeyExchangeProtocol getKeyExchangeProtocol()  const;

        protected:
            virtual void setupAttributesForSerialization();

        // Now the data members
        private:
            SSHLocalKeyExchangeProtocol m_protocol;
    };

    class SSHServerListLocalDeleteMessage : public PrismMessage
    {
        public:
            SSHServerListLocalDeleteMessage ();
            SSHServerListLocalDeleteMessage (const SSHLocalKeyExchangeProtocol &protocol);
            virtual    ~SSHServerListLocalDeleteMessage ();
            void setKeyExchangeProtocol(const SSHLocalKeyExchangeProtocol &protocol);
            SSHLocalKeyExchangeProtocol getKeyExchangeProtocol()  const;

        protected:
            virtual void setupAttributesForSerialization();

        // Now the data members
        private:
            SSHLocalKeyExchangeProtocol m_protocol;
    };

}
#endif                  // SECSERVICESLOCALCONFIGMESSAGE_H
