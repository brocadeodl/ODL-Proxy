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
 *   Author : krao                                                     *
 **************************************************************************/

#include "Nsm/Local/VRRPWorker/VRRPVcsSessionConfigLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
//#include "Nsm/Local/VRRPWorker/VRRPVirtualIPLocalManagedObject.h"
//#include "Nsm/Local/VRRPWorker/VRRPTrackPortLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

    VRRPVcsSessionConfigLocalManagedObject::VRRPVcsSessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPVcsSessionConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    VRRPVcsSessionConfigLocalManagedObject::VRRPVcsSessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI8 &vrid,const UI16 &advt_int,const UI8 &prio,const bool &enable,const bool &preempt,const UI16 &hold_time,const string &description)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPVcsSessionConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_vrid    (vrid),
        /*
        m_vip    (vip),
        m_track    (track),
        */
        m_prio    (prio),
        m_description    (description)
    {
    }

    VRRPVcsSessionConfigLocalManagedObject::~VRRPVcsSessionConfigLocalManagedObject ()
    {
    }

    string  VRRPVcsSessionConfigLocalManagedObject::getClassName()
    {
        return ("VRRPVcsSessionConfigLocalManagedObject");
    }

    void  VRRPVcsSessionConfigLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI8(&m_vrid,"vrid", vrrp_vrid));
        /*
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<VRRPVirtualIPLocalManagedObject>(&m_vip,"vip", getClassName (), getObjectId (), VRRPVirtualIPLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<VRRPTrackPortLocalManagedObject>(&m_track,"track", getClassName (), getObjectId (), VRRPTrackPortLocalManagedObject::getClassName ()));
        */
        addPersistableAttribute (new AttributeUI8(&m_prio,"prio", vrrp_priority));
        addPersistableAttribute (new AttributeString(&m_description,"description", vrrp_description));
    }

    void  VRRPVcsSessionConfigLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI8(&m_vrid,"vrid", vrrp_vrid));
        /*
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<VRRPVirtualIPLocalManagedObject>(&m_vip,"vip", getClassName (), getObjectId (), VRRPVirtualIPLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<VRRPTrackPortLocalManagedObject>(&m_track,"track", getClassName (), getObjectId (), VRRPTrackPortLocalManagedObject::getClassName ()));
        */
        addPersistableAttributeForCreate  (new AttributeUI8(&m_prio,"prio", vrrp_priority));
        addPersistableAttributeForCreate  (new AttributeString(&m_description,"description", vrrp_description));

        vector<string> keyName;
        keyName.push_back ("vrid");
        setUserDefinedKeyCombination (keyName);
    }

    void  VRRPVcsSessionConfigLocalManagedObject::setVrid(const UI8 &vrid)
    {
        m_vrid  =  vrid;
    }

    UI8  VRRPVcsSessionConfigLocalManagedObject::getVrid() const
    {
        return (m_vrid);
    }
/*
    void  VRRPVcsSessionConfigLocalManagedObject::setVip(const vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> > &vip)
    {
        m_vip  =  vip;
    }

    vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> >  VRRPVcsSessionConfigLocalManagedObject::getVip() const
    {
        return (m_vip);
    }

    void  VRRPVcsSessionConfigLocalManagedObject::settrack(const vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> > &track)
    {
        m_track  =  track;
    }

    vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> >  VRRPVcsSessionConfigLocalManagedObject::gettrack() const
    {
        return (m_track);
    }
*/
    void  VRRPVcsSessionConfigLocalManagedObject::setPrio(const UI8 &prio)
    {
        m_prio  =  prio;
    }

    UI8  VRRPVcsSessionConfigLocalManagedObject::getPrio() const
    {
        return (m_prio);
    }

    void  VRRPVcsSessionConfigLocalManagedObject::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  VRRPVcsSessionConfigLocalManagedObject::getDescription() const
    {
        return (m_description);
    }

}
