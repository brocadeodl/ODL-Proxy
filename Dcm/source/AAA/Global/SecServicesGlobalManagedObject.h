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
 *   Author : Balaji. M                                                    *
 **************************************************************************/

#if 0
#ifndef SECSERVICESGLOBALMANAGEDOBJECT_H
#define SECSERVICESGLOBALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{
    class AAAGlobalObjectManager;

    class TelnetServerGlobalManagedObject : virtual public DcmManagedObject
    {
        public:
            TelnetServerGlobalManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
            TelnetServerGlobalManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const bool &disableFlag);
            virtual    ~TelnetServerGlobalManagedObject ();
            static string getClassName();
            void setDisableFlag(const bool &disableFlag);
            bool getDisableFlag()  const;
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();

        // Now the data members
        private:
            bool  m_disableFlag;
    };

    class SSHServerGlobalManagedObject : virtual public DcmManagedObject
    {
        public:
            SSHServerGlobalManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
            SSHServerGlobalManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,
                    const bool &disableFlag);
            virtual    ~SSHServerGlobalManagedObject ();
            static string getClassName();
            void setDisableFlag(const bool &disableFlag);
            bool getDisableFlag()  const;
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();

        // Now the data members
        private:
            bool  m_disableFlag;
    };

    class SSHServerListManagedObject : virtual public DcmManagedObject
    {
        public:
            SSHServerListManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
            SSHServerListManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,
                    const SSHKeyExchangeProtocol &protocol);
            virtual    ~SSHServerListManagedObject ();
            static string getClassName();
            void setKeyExchangeProtocol(const SSHKeyExchangeProtocol &protocol);
            SSHKeyExchangeProtocol getKeyExchangeProtocol()  const;
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();

        // Now the data members
        private:
            SSHKeyExchangeProtocol m_protocol;
    };
}
#endif                                    //SECSERVICESGLOBALMANAGEDOBJECT_H
#endif
