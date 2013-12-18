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

#ifndef VCENTERLOCALMESSAGES_H
#define VCENTERLOCALMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

// ************* vCenterVcenterMessage ************
    class vCenterVcenterMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            vCenterVcenterMessage ();
            vCenterVcenterMessage (const string &id,
                const string &ipaddress,
                const string &username,
                const string &password,
                const bool &activate);
            virtual    ~vCenterVcenterMessage ();
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
            bool getDeleteFlag() const;
            void setDeleteFlag(const bool val) ;

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
            bool m_vc_delete;
    };

}
#endif                                            // VCENTERLOCALMESSAGES_H
