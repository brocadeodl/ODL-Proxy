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
 *   Author : nbharat                                                     *
 **************************************************************************/

#ifndef LICENSESERVICEFILEADDMESSAGE_H
#define LICENSESERVICEFILEADDMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LicenseServiceFileAddMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LicenseServiceFileAddMessage ();
            LicenseServiceFileAddMessage (const string &proto,const string &user,const string &host,const string &password,const string &srcFilePath,const string &msgStatus);
            virtual    ~LicenseServiceFileAddMessage ();
            void setProto(const string &proto);
            string getProto()  const;
            void setUser(const string &user);
            string getUser()  const;
            void setHost(const string &host);
            string getHost()  const;
            void setPassword(const string &password);
            string getPassword()  const;
            void setSrcFilePath(const string &srcFilePath);
            string getSrcFilePath()  const;
            void setMsgStatus(const string &msgStatus);
            string getMsgStatus()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_proto;
            string  m_user;
            string  m_host;
            string  m_password;
            string  m_srcFilePath;
            string  m_msgStatus;
    };
}
#endif                                            //LICENSESERVICEFILEADDMESSAGE_H
