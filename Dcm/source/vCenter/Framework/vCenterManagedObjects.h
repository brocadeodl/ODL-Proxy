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

#ifndef VCENTERMANAGEDOBJECTS_H
#define VCENTERMANAGEDOBJECTS_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class vCenterObjectManager;

// ************* VcenterManagedObject ************
    class VcenterManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            VcenterManagedObject (vCenterObjectManager *pvCenterObjectManager);
            VcenterManagedObject (vCenterObjectManager *pvCenterObjectManager,const string &id,
                const string &ipaddress,
                const string &username,
                const string &password,
                const bool &activate);
            virtual    ~VcenterManagedObject ();
            static string getClassName();
            void setId(const string &id);
            string getId()  const;
            void setIpaddress(const string &ipaddress);
            string getIpaddress()  const;
            void setUsername(const string &username);
            string getUsername()  const;
            void setPassword(const string &password);
            string getPassword()  const;
            void setActivate(const bool &activate);
            bool getActivate()  const;
            void setInterval(const UI32 &interval);
            UI32 getInterval()  const;
            void setIgnoreValue(const UI32 &ignoreValue);
            UI32 getIgnoreValue()  const;
            void setAlways(const bool &always);
            bool getAlways() const;

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
            UI32    m_ignore_value;
            bool    m_always;

    };

}
#endif                                            // VCENTERMANAGEDOBJECTS_H
