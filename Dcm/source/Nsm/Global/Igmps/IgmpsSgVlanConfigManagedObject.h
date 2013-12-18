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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#ifndef IGMPSSGVLANCONFIGMANAGEDOBJECT_H
#define IGMPSSGVLANCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class IgmpsSgVlanConfigManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            IgmpsSgVlanConfigManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            IgmpsSgVlanConfigManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const IpV4Address &groupAddress,const UI32 &interface,const UI32 &ifType,const string &ifName);
            virtual    ~IgmpsSgVlanConfigManagedObject ();
            static string getClassName();
            void setGroupAddress(const IpV4Address &groupAddress);
            IpV4Address getGroupAddress()  const;
            void setInterface(const UI32 &interface);
            UI32 getInterface()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;

// Now the data members

        private:
        protected:
        public:
            IpV4Address  m_groupAddress;
            UI32  m_interface;
            UI32  m_ifType;
            string  m_ifName;
    };
}
#endif                                            //IGMPSSGVLANCONFIGMANAGEDOBJECT_H
