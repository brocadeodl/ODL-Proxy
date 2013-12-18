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
 *   Author : pprakash                                                     *
 **************************************************************************/

#ifndef IPADMIpv6ROUTELOCALMANAGEDOBJECT_H
#define IPADMIpv6ROUTELOCALMANAGEDOBJECT_H
#include "string"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Types/IpV6Address.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeEnum.h"
using namespace WaveNs;

namespace DcmNs
{

    class IpadmLocalObjectManager;
    class IpadmIpv6RouteLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void loadOperationalData(WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext);
	virtual void setupKeys();
        public:
            IpadmIpv6RouteLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager);
            IpadmIpv6RouteLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager,const IpV6AddressNetworkMask &dest, const IpV6Address &nhIp, const UI32 &type,const UI32 &management);
//            IpadmIpv6RouteLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager,const IpV6AddressNetworkMask &dest, const IpV6Address &nhIp, const UI32 &type);
            //IpadmIpv6RouteLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager,const IpV6AddressNetworkMask &dest, const IpV6Address &nhIp);
            virtual    ~IpadmIpv6RouteLocalManagedObject ();
            static string getClassName();
			virtual UI32 getCase (const UI32 &userChoiceWyserTag);
            void setDest(const IpV6AddressNetworkMask &dest);
            IpV6AddressNetworkMask getDest()  const;
            void setNhIp(const IpV6Address &nhIp);
            IpV6Address getNhIp()  const;
            void setRouteType(const UI32 &type);
            UI32 getRouteType()  const;
            void setManagement(const UI32 &management);
            UI32 getManagement()  const;

// Now the data members

        private:
        protected:
        public:
            IpV6AddressNetworkMask  m_dest;
            IpV6Address  m_nhIp;
	        UI32 m_routeType;
	        UI32 m_management;
    };
}
#endif
//IPADMROUTELOCALMANAGEDOBJECT_H
