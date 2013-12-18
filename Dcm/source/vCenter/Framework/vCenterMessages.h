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
 *   Author : sharis                                                     *
 **************************************************************************/

#ifndef VCENTERMESSAGES_H
#define VCENTERMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

// ************* vCenterCreateVcenterMessage ************
    class vCenterCreateVcenterMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            vCenterCreateVcenterMessage ();
            vCenterCreateVcenterMessage (const string &id,
                const string &ipaddress,
                const string &username,
                const string &password,
                const bool &activate);
            virtual    ~vCenterCreateVcenterMessage ();
            void setId(const string &id);
            string getId()  const;
            bool getIdFlag()  const;
            void setIpaddress(const string &ipaddress);
            string getIpaddress()  const;
            bool getIpaddressFlag()  const;
            void setUsername(const string &username);
            string getUsername()  const;
            bool getUsernameFlag()  const;
            void setPassword(const string &password);
            string getPassword()  const;
            bool getPasswordFlag()  const;
            void setActivate(const bool &activate);
            bool getActivate()  const;
            bool getActivateFlag()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_id;
            string  m_ipaddress;
            string  m_username;
            string  m_password;
            bool  m_activate;
            bool m_id_flag;
            bool m_ipaddress_flag;
            bool m_username_flag;
            bool m_password_flag;
            bool m_activate_flag;
    };

// ************* vCenterUpdateVcenterMessage ************
    class vCenterUpdateVcenterMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            vCenterUpdateVcenterMessage ();
            vCenterUpdateVcenterMessage (const string &id,
                const string &ipaddress,
                const string &username,
                const string &password,
                const bool &activate);
            virtual    ~vCenterUpdateVcenterMessage ();
            void setId(const string &id);
            string getId()  const;
            bool getIdFlag()  const;
            void setIpaddress(const string &ipaddress);
            string getIpaddress()  const;
            bool getIpaddressFlag()  const;
            void setUsername(const string &username);
            string getUsername()  const;
            bool getUsernameFlag()  const;
            void setPassword(const string &password);
            string getPassword()  const;
            bool getPasswordFlag()  const;
            void setActivate(const bool &activate);
            bool getActivate()  const;
            bool getActivateFlag()  const;
            void setInterval(const UI32 &interval);
            UI32 getInterval()  const;
            bool getIntervalFlag()  const;
            void setIgnoreValue(const UI32 &ignoreValue);
            UI32 getIgnoreValue()  const;
            bool getIgnoreValueFlag() const;
            void setAlways(const bool &always);
            bool getAlways()  const;
            bool getAlwaysFlag()  const;


// Now the data members

        private:
        protected:
        public:
            string  m_id;
            string  m_ipaddress;
            string  m_username;
            string  m_password;
            bool  m_activate;
            UI32  m_interval;
            UI32  m_ignore_value;
            bool m_always;
            bool m_id_flag;
            bool m_ipaddress_flag;
            bool m_username_flag;
            bool m_password_flag;
            bool m_activate_flag;
            bool m_interval_flag;
            bool m_ignore_value_flag;
            bool m_always_flag;

    };

// ************* vCenterDeleteVcenterMessage ************
    class vCenterDeleteVcenterMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            vCenterDeleteVcenterMessage ();
            vCenterDeleteVcenterMessage (const string &id);
            virtual    ~vCenterDeleteVcenterMessage ();
            void setId(const string &id);
            string getId()  const;
            bool getIdFlag()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_id;
            bool m_id_flag;
    };

}
#endif                                            // VCENTERMESSAGES_H
