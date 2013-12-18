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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef SYSTEMGROUPMANAGEDOBJECT_H
#define SYSTEMGROUPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerObjectManager;
    class SystemGroupManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            SystemGroupManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager);
            SystemGroupManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager,const string &contact,const string &location,const string &sysDescr);
            virtual    ~SystemGroupManagedObject ();
            static string getClassName();
            void setContact(const string &contact);
            string getContact()  const;
            void setLocation(const string &location);
            string getLocation()  const;
            void setSysDescr(const string &sysDescr);
            string getSysDescr()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_contact;
            string  m_location;
            string  m_sysDescr;
    };
}
#endif                                            //SYSTEMGROUPMANAGEDOBJECT_H
