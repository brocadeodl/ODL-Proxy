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

#ifndef PORTSECSTICKYMACSLOCALMANAGEDOBJECT_H
#define PORTSECSTICKYMACSLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
    class PortSecStickyMacsLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PortSecStickyMacsLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            PortSecStickyMacsLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            virtual    ~PortSecStickyMacsLocalManagedObject ();
            static string getClassName();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setStickyMac(const string &stickyMac);
            string getStickyMac() const;
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId() const;

// Now the data members

        private:
        protected:
        public:
			string m_ifName;
            string m_stickyMac;
			UI32 m_vlanId;
    };
}
#endif                                            //PORTSECSTICKYMACSLOCALMANAGEDOBJECT_H
