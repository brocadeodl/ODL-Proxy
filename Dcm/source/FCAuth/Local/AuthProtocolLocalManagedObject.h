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

#ifndef AUTHPROTOCOLLOCALMANAGEDOBJECT_H
#define AUTHPROTOCOLLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "FCAuth/Local/AuthProtocolEnumeration.h"

using namespace WaveNs;

namespace DcmNs
{

    class FCAuthLocalObjectManager;
    class AuthProtocolLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            AuthProtocolLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager);
            AuthProtocolLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager,const AuthProtocolType &ProtocolAuthType,const AuthProtocolGroup &ProtocolAuthGroup,const AuthProtocolHash &ProtocolAuthHash);
            virtual    ~AuthProtocolLocalManagedObject ();
            static string getClassName();
            void setProtocolAuthType(const AuthProtocolType &ProtocolAuthType);
            AuthProtocolType getProtocolAuthType()  const;
            void setProtocolAuthGroup(const AuthProtocolGroup
                &ProtocolAuthGroup);
            AuthProtocolGroup getProtocolAuthGroup()  const;
            void setProtocolAuthHash(const AuthProtocolHash &ProtocolAuthHash);
            AuthProtocolHash getProtocolAuthHash()  const;

// Now the data members

        private:
        protected:
        public:
            AuthProtocolType  m_ProtocolAuthType;
            AuthProtocolGroup m_ProtocolAuthGroup;
            AuthProtocolHash  m_ProtocolAuthHash;
    };

}
#endif                                            //AUTHPROTOCOLLOCALMANAGEDOBJECT_H
