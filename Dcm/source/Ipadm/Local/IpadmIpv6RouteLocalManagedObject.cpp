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
 *   Author :  pprakash                                                    *
 **************************************************************************/

#include "string"
#include "Ipadm/Local/IpadmIpv6RouteLocalManagedObject.h"
#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "vcs.h"
#include "Framework/Types/IpV6Address.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeEnum.h"
namespace DcmNs
{

    IpadmIpv6RouteLocalManagedObject::IpadmIpv6RouteLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager)
        : PrismElement  (pIpadmLocalObjectManager),
        PrismPersistableObject (IpadmIpv6RouteLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pIpadmLocalObjectManager),
        DcmManagedObject (pIpadmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pIpadmLocalObjectManager)
    {
			m_routeType = 0;
    }

//    IpadmIpv6RouteLocalManagedObject::IpadmIpv6RouteLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager,const IpV6AddressNetworkMask &dest,const IpV6Address &nhIp, const string &type)
//      IpadmIpv6RouteLocalManagedObject::IpadmIpv6RouteLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager,const IpV6AddressNetworkMask &dest,const IpV6Address &nhIp, const UI32 &type)
      IpadmIpv6RouteLocalManagedObject::IpadmIpv6RouteLocalManagedObject (IpadmLocalObjectManager *pIpadmLocalObjectManager,const IpV6AddressNetworkMask &dest,const IpV6Address &nhIp, const UI32 &type, const UI32 &management)
        : PrismElement  (pIpadmLocalObjectManager),
        PrismPersistableObject (IpadmIpv6RouteLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pIpadmLocalObjectManager),
        DcmManagedObject (pIpadmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pIpadmLocalObjectManager),
	m_dest (dest),
	m_nhIp (nhIp),
	m_routeType(type),
	m_management(management)
   {
       m_routeType = 0;
    }
    IpadmIpv6RouteLocalManagedObject::~IpadmIpv6RouteLocalManagedObject ()
    {
    }
	void IpadmIpv6RouteLocalManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
	{
	 trace (TRACE_LEVEL_DEBUG, "******** IpadmIpv6Route  == IpadmIpv6RouteLocalManagedObject::loadOperationalData *********");
	}
    
    string  IpadmIpv6RouteLocalManagedObject::getClassName()
    {
        return ("IpadmIpv6RouteLocalManagedObject");
    }

    void IpadmIpv6RouteLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("dest");
        //keyName.push_back("nhIp");
        setUserDefinedKeyCombination (keyName);
    }

    void  IpadmIpv6RouteLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV6AddressNetworkMask(&m_dest,"dest",ip_forward_dest));
        addPersistableAttribute (new AttributeIpV6Address(&m_nhIp,"nhIp",ip_forward_next_hop));
        addPersistableAttribute (new AttributeUI32(&m_routeType,"routeType"));
//	addPersistableAttribute (new AttributeEnum((UI32 *) (&m_management),"management",ip_forward_management));
    }

    void  IpadmIpv6RouteLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV6AddressNetworkMask(&m_dest,"dest",ip_forward_dest));
        addPersistableAttributeForCreate  (new AttributeIpV6Address(&m_nhIp,"nhIp",ip_forward_next_hop));
        addPersistableAttributeForCreate (new AttributeUI32(&m_routeType,"routeType"));
//	addPersistableAttributeForCreate (new AttributeEnum((UI32 *) (&m_management),"management",ip_forward_management));
    }

	UI32 IpadmIpv6RouteLocalManagedObject::getCase (const UI32 &userChoiceWyserTag)
	{
		trace (TRACE_LEVEL_DEBUG, "IpadmIpv6RouteLocalManagedObject::getCase: Entering ...");

		if (MGMNT_ROUTE_NEXTHOP == m_routeType)
		{
			trace (TRACE_LEVEL_DEBUG,"config choice = MGMNT_ROUTE_NEXTHOP ");
			return ip_forward_ipv6_hop;
		}
/* Commenting this choice for Hydra release
		else if (MGMNT_ROUTE_INTERFACE == m_routeType)
		{
			trace (TRACE_LEVEL_DEBUG,"config choice = MGMNT_ROUTE_INTERFACE ");
			return ip_forward_management_name;
		}
*/
		trace (TRACE_LEVEL_DEBUG,"invalid choice");
		return 0;
	}

    void  IpadmIpv6RouteLocalManagedObject::setDest(const IpV6AddressNetworkMask &dest)
    {
        m_dest  =  dest;
    }

    IpV6AddressNetworkMask  IpadmIpv6RouteLocalManagedObject::getDest() const
    {
        return (m_dest);
    }

    void  IpadmIpv6RouteLocalManagedObject::setNhIp(const IpV6Address &nhIp)
    {
        m_nhIp  =  nhIp;
    }

    IpV6Address  IpadmIpv6RouteLocalManagedObject::getNhIp() const
    {
        return (m_nhIp);
    }
    
    void  IpadmIpv6RouteLocalManagedObject::setRouteType(const UI32 &type)
    {
        m_routeType  = (const UI32) type;
    }

    UI32  IpadmIpv6RouteLocalManagedObject::getRouteType() const
    {
        return (m_routeType);
    }
    void  IpadmIpv6RouteLocalManagedObject::setManagement(const UI32 &management)
    {
      	m_management = (const UI32) management;
    }

    UI32  IpadmIpv6RouteLocalManagedObject::getManagement() const
    {
        return (m_management);
    }
    

}
