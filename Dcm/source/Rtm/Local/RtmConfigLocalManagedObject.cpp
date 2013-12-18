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
 *   Author : hzhu                                                     *
 **************************************************************************/

#include "Rtm/Local/RtmConfigLocalManagedObject.h"
#include "Rtm/Local/RtmLocalObjectManager.h"
#include "Rtm/Local/RtmTypes.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "vcs.h"

namespace DcmNs
{

    RtmConfigLocalManagedObject::RtmConfigLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager)
        : PrismElement  (pRtmLocalObjectManager),
        PrismPersistableObject (RtmConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pRtmLocalObjectManager),
        DcmManagedObject (pRtmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pRtmLocalObjectManager)
    {
        m_nhProto = 1;
        m_nhDefault = false;
        m_nhRecur = UI32UC(RTM_CONFIG_DEFAULT_NH_RECUR);
        m_loadSharing = UI32UC(RTM_CONFIG_DEFAULT_LOADSHARING);
    }

    RtmConfigLocalManagedObject::RtmConfigLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager,const UI8 &nhProto,const bool &nhDefault,const UI32UC &nhRecur,const UI32UC &loadSharing,const IpV4AddressUC &routerID)
        : PrismElement  (pRtmLocalObjectManager),
        PrismPersistableObject (RtmConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pRtmLocalObjectManager),
        DcmManagedObject (pRtmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pRtmLocalObjectManager),
        m_nhProto    (nhProto),
        m_nhDefault    (nhDefault),
        m_nhRecur    (nhRecur),
        m_loadSharing    (loadSharing),
        m_routerID    (routerID)
    {
    }

    RtmConfigLocalManagedObject::~RtmConfigLocalManagedObject ()
    {
    }

    string  RtmConfigLocalManagedObject::getClassName()
    {
        return ("RtmConfigLocalManagedObject");
    }

    void  RtmConfigLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_nhDefault,"nhDefault",rtm_next_hop_enable_default));
        addPersistableAttribute (new AttributeUI32UC(&m_nhRecur,false,false,0,"nhRecur",rtm_next_hop_recursion));
        addPersistableAttribute (new AttributeUI32UC(&m_loadSharing, false, false, 0, "loadSharing",rtm_load_sharing));
        IpV4Address routerID;
        addPersistableAttribute (new AttributeIpV4AddressUC(&m_routerID,false,false,routerID,"routerID",rtm_router_id));
    }

    void  RtmConfigLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_nhDefault,"nhDefault",rtm_next_hop_enable_default));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_nhRecur,false,false,0,"nhRecur",rtm_next_hop_recursion));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_loadSharing,false,false,0,"loadSharing",rtm_load_sharing));
        IpV4Address routerID;
        addPersistableAttributeForCreate  (new AttributeIpV4AddressUC(&m_routerID,false,false,routerID,"routerID",rtm_router_id));
    }

    void  RtmConfigLocalManagedObject::setNhProto(const UI8 &nhProto)
    {
        m_nhProto  =  nhProto;
    }

    UI8  RtmConfigLocalManagedObject::getNhProto() const
    {
        return (m_nhProto);
    }

    void  RtmConfigLocalManagedObject::setNhDefault(const bool &nhDefault)
    {
        m_nhDefault  =  nhDefault;
    }

    bool  RtmConfigLocalManagedObject::getNhDefault() const
    {
        return (m_nhDefault);
    }

    void  RtmConfigLocalManagedObject::setNhRecur(const UI32 &nhRecur, bool userConfig)
    {
		m_nhRecur.setIsUserConfigured(userConfig);
        m_nhRecur.setUI32Value(nhRecur);
    }

    UI32UC  RtmConfigLocalManagedObject::getNhRecur() const
    {
        return (m_nhRecur);
    }

    void  RtmConfigLocalManagedObject::setLoadSharing(const UI32 &loadSharing, bool userConfig)
    {
		m_loadSharing.setIsUserConfigured(userConfig);
		m_loadSharing.setUI32Value(loadSharing);
    }

    UI32UC  RtmConfigLocalManagedObject::getLoadSharing() const
    {
        return (m_loadSharing);
    }

    void  RtmConfigLocalManagedObject::setRouterID(const IpV4Address &routerID, bool userConfig)
    {
		m_routerID.setIsUserConfigured(userConfig);
        m_routerID.setIpV4AddressValue(routerID.toString());
    }

	void  RtmConfigLocalManagedObject::unsetRouterID()
    {
		m_routerID.setIsUserConfigured(false);
    }

    IpV4AddressUC  RtmConfigLocalManagedObject::getRouterID() const
    {
        return (m_routerID);
    }

}
