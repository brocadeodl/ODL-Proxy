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

#ifndef PORTSECALLOWEDMACSLOCALMANAGEDOBJECT_H
#define PORTSECALLOWEDMACSLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
    class PortSecAllowedMacsLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PortSecAllowedMacsLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            PortSecAllowedMacsLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            PortSecAllowedMacsLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager, const string &allowedMac);
            PortSecAllowedMacsLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager, const string &allowedMac);
            virtual    ~PortSecAllowedMacsLocalManagedObject ();
            static string getClassName();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setAllowedMac(const string &allowedMac);
            string getAllowedMac() const;
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId() const;

// Now the data members

        private:
        protected:
        public:
			string m_ifName;
            string m_allowedMac;
			UI32 m_vlanId;
    };
}
#endif                                            //PORTSECALLOWEDMACSLOCALMANAGEDOBJECT_H
