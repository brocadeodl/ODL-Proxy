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
 *   Author : skrastog                                                     *
 **************************************************************************/

#ifndef PORTSECOUILOCALMANAGEDOBJECT_H
#define PORTSECOUILOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
    class PortSecOuiLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PortSecOuiLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            PortSecOuiLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            PortSecOuiLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager, const string &allowedMac);
            PortSecOuiLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager, const string &allowedMac);
            virtual    ~PortSecOuiLocalManagedObject ();
            static string getClassName();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setOui(const string &oui);
            string getOui() const;

// Now the data members

        private:
        protected:
        public:
			string m_ifName;
            string m_oui;
    };
}
#endif                                            //PORTSECALLOWEDMACSLOCALMANAGEDOBJECT_H
