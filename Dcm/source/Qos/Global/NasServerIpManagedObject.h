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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef NASSERVERIPMANAGEDOBJECT_H
#define NASSERVERIPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Qos/Global/NasServerIpVlanManagedObject.h"
#include "Qos/Global/NasServerIpVrfManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class NasServerIpManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            NasServerIpManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager);
            NasServerIpManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager, const IpV4AddressNetworkMask serverIpAddressPrefix);
            virtual    ~NasServerIpManagedObject ();
			virtual void  setupKeys ();
            static string getClassName();
			void setServerIpAddressPrefix(const IpV4AddressNetworkMask &serverIpAddressPrefix);
			IpV4AddressNetworkMask  getServerIpAddressPrefix();
			vector<WaveManagedObjectPointer<NasServerIpVlanManagedObject> > getVlans() const;
			vector<WaveManagedObjectPointer<NasServerIpVrfManagedObject> > getVrfs() const;
			NasServerIpVlanManagedObject* getVlanServerConfig(const UI32 &vlanNumber);
			NasServerIpVrfManagedObject* getVrfServerConfig(const string &vrfName);
			


// Now the data members

        private:
        protected:
        public:
			IpV4AddressNetworkMask m_serverIpAddressPrefix;
			vector<WaveManagedObjectPointer<NasServerIpVlanManagedObject> >  m_vlans;
            vector<WaveManagedObjectPointer<NasServerIpVrfManagedObject> >  m_vrfs;

    };
}
#endif                                            //NASSERVERIPMANAGEDOBJECT_H
