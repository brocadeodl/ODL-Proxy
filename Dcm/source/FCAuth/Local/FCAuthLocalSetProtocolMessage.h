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
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef FCAUTHLOCALSETPROTOCOLMESSAGE_H
#define FCAUTHLOCALSETPROTOCOLMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "FCAuth/Local/AuthProtocolEnumeration.h"

using namespace WaveNs;

namespace DcmNs
{

    class FCAuthLocalSetProtocolMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FCAuthLocalSetProtocolMessage ();
            FCAuthLocalSetProtocolMessage (const AuthProtocolType &AuthType,const AuthProtocolGroup &AuthGroup,const AuthProtocolHash &AuthHash);
            virtual    ~FCAuthLocalSetProtocolMessage ();
            void setAuthType(const AuthProtocolType &AuthType);
            AuthProtocolType getAuthType()  const;
            void setAuthGroup(const AuthProtocolGroup &AuthGroup);
            AuthProtocolGroup getAuthGroup()  const;
            void setAuthHash(const AuthProtocolHash &AuthHash);
            AuthProtocolHash getAuthHash()  const;

// Now the data members

        private:
        protected:
        public:
            AuthProtocolType  m_AuthType;
            AuthProtocolGroup m_AuthGroup;
            AuthProtocolHash  m_AuthHash;
    };
}
#endif                                            //FCAUTHLOCALSETPROTOCOLMESSAGE_H
