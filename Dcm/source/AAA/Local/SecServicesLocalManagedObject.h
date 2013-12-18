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

#ifndef SECSERVICESLOCALMANAGEDOBJECT_H
#define SECSERVICESLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

#include "AAA/Local/AAALocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{
    class AAALocalLocalObjectManager;

    class TelnetServerLocalManagedObject : virtual public DcmLocalManagedObject
    {
        public:
            TelnetServerLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager);
            TelnetServerLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager,const bool &disableFlag);
            virtual    ~TelnetServerLocalManagedObject ();
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

    class SSHServerLocalManagedObject : virtual public DcmLocalManagedObject
    {
        public:
            SSHServerLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager);
            SSHServerLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager,
                    const bool &disableFlag);
            virtual    ~SSHServerLocalManagedObject ();
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

    class SSHServerListLocalManagedObject : virtual public DcmLocalManagedObject
    {
        public:
            SSHServerListLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager);
            SSHServerListLocalManagedObject (AAALocalLocalObjectManager *pAAALocalLocalObjectManager,
                    const SSHLocalKeyExchangeProtocol &protocol);
            virtual    ~SSHServerListLocalManagedObject ();
            static string getClassName();
            void setKeyExchangeProtocol(const SSHLocalKeyExchangeProtocol &protocol);
            SSHLocalKeyExchangeProtocol getKeyExchangeProtocol()  const;
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();

        // Now the data members
        private:
            SSHLocalKeyExchangeProtocol m_protocol;
    };
}
#endif                                    //SECSERVICESLOCALMANAGEDOBJECT_H
