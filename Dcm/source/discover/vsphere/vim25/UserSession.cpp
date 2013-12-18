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
 * UserSession.cpp
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#include "UserSession.h"

namespace DcmNs {

    UserSession::UserSession() {
        // TODO Auto-generated constructor stub

    }

    string UserSession::getFullName() const {
        return fullName;
    }

    string UserSession::getKey() const {
        return key;
    }

    string UserSession::getLocale() const {
        return locale;
    }

    string UserSession::getMessageLocale() const {
        return messageLocale;
    }

    string UserSession::getUserName() const {
        return userName;
    }

    bool UserSession::isExtensionSession() const {
        return extensionSession;
    }

    void UserSession::setExtensionSession(bool _extensionSession) {
        this->extensionSession = _extensionSession;
    }

    void UserSession::setFullName(string _fullName) {
        this->fullName = _fullName;
    }

    void UserSession::setKey(string _key) {
        this->key = _key;
    }

    void UserSession::setLocale(string _locale) {
        this->locale = _locale;
    }

    void UserSession::setMessageLocale(string _messageLocale) {
        this->messageLocale = _messageLocale;
    }

    void UserSession::setUserName(string _userName) {
        this->userName = _userName;
    }

    UserSession::~UserSession() {

    }

    void UserSession::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "userName", userName);
            XMLUtil::extractString(childNode, "key", key);
            XMLUtil::extractString(childNode, "fullName", fullName);
            XMLUtil::extractString(childNode, "locale", locale);
            XMLUtil::extractString(childNode, "loginTime", loginTime);
            XMLUtil::extractString(childNode, "lastActiveTime", lastActiveTime);
            XMLUtil::extractString(childNode, "messageLocale", messageLocale);
            XMLUtil::extractBoolean(childNode, "extensionSession",
                    extensionSession);
            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
