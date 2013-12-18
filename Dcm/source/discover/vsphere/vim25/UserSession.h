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

/*
 * UserSession.h
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#ifndef USERSESSION_H_
#define USERSESSION_H_
#include <string>
#include "discover/vsphere/vim25/WSObject.h"
#include <libxml/tree.h>
#include <libxml/xpath.h>
using namespace std;
namespace DcmNs {

    /**
     * A class informing about UserSession
     */
    class UserSession: public WSObject {
        public:
            /*
             * A unique identifier for this session, also known as the session ID.
             */
            string key;
            string userName;
            string fullName;
            string loginTime;
            string lastActiveTime;
            string locale;
            string messageLocale;
            bool extensionSession;
            string getFullName() const;
            string getKey() const;
            string getLocale() const;
            string getMessageLocale() const;
            string getUserName() const;
            bool isExtensionSession() const;
            void setExtensionSession(bool extensionSession);
            void setFullName(string fullName);
            void setKey(string key);
            void setLocale(string locale);
            void setMessageLocale(string messageLocale);
            void setUserName(string userName);
            UserSession();
            virtual ~UserSession();
            void deSerialize(xmlNodePtr node);

    };

} /* namespace DcmNs */
#endif /* USERSESSION_H_ */
