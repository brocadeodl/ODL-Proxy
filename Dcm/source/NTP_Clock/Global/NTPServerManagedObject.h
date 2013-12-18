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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef NTPSERVERMANAGEDOBJECT_H
#define NTPSERVERMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

    class NTPClockGlobalObjectManager;
    class NTPServerManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
			virtual void setupKeys();
        public:
            NTPServerManagedObject (NTPClockGlobalObjectManager *pNTPClockGlobalObjectManager);
            NTPServerManagedObject (NTPClockGlobalObjectManager *pNTPClockGlobalObjectManager,const IpVxAddress &server, const UI32 &Key);
            virtual    ~NTPServerManagedObject ();
            static string getClassName();
            void setIndex(const UI32 &index);
            UI32 getIndex()  const;
            void setServer(const IpVxAddress &server);
            IpVxAddress getServer()  const;
            void setKey(const UI32 &Key);
            UI32 getKey() const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_index;
            IpVxAddress  m_server;
            UI32  m_Key;
    };
}
#endif                                            //NTPSERVERMANAGEDOBJECT_H
