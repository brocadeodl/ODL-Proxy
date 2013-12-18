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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "Framework/Types/WorldWideName.h"
//#include "brocade-rbridge.h"
#include "vcs.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "brocade-logical-chassis.h"

namespace DcmNs
{

VcsNodeLocalManagedObject::VcsNodeLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (VcsNodeLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager),
      m_switchPort               (0),
      m_SWBDModel                (-1),
      m_fwVersion                (""),
      m_switchname               (""),
      m_swmodelName              (""),
      m_principalPriority        (0)
{
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
    setSwitchDeleteAfterFailoverFlag (false);
    m_domainId = 0;
    m_inVcs = false;
    m_isPrincipal = false;
    m_locationId = 0;
    m_switchState = 0;
    m_vcsId = 0;
    m_vcsMode = false;
    m_willingMode = false;
    m_xactionId = 0;
    m_vcsSlotLocalManagedObjects.clear ();
}

VcsNodeLocalManagedObject::~VcsNodeLocalManagedObject ()
{
}

string VcsNodeLocalManagedObject::getClassName ()
{
    return ("VcsNodeLocalManagedObject");
}

void VcsNodeLocalManagedObject::setSerialNumber(const string &serial_number)
{
    m_serialNumber = serial_number;
}

void VcsNodeLocalManagedObject::setDomainId(const UI32 &domain_id)
{
    m_domainId = domain_id;
}
void VcsNodeLocalManagedObject::setVcsId(const UI32 &vcs_id)
{
    m_vcsId = vcs_id;
}

void VcsNodeLocalManagedObject::setFcfMac(const MacAddress &fcf_mac)
{
    m_fcfMac = fcf_mac;
}
void VcsNodeLocalManagedObject::setSwitchMac(const MacAddress &switch_mac)
{
    m_switchMac = switch_mac;
}
void VcsNodeLocalManagedObject::setVcsMode(const bool &mode)
{
    m_vcsMode = mode;
}
void VcsNodeLocalManagedObject::setInVcs(const bool &in_vcs)
{
    m_inVcs = in_vcs;
}
void VcsNodeLocalManagedObject::setIsPrincipal(const bool &is_principal)
{
    m_isPrincipal = is_principal;
}
void VcsNodeLocalManagedObject::setWillingMode (const bool &willingMode)
{
	m_willingMode = willingMode;
}

void VcsNodeLocalManagedObject::setPublicIpAddress (const vector<string> &publicIpAddress)
{
    m_publicIpAddress = publicIpAddress;
}

void VcsNodeLocalManagedObject::setLocationId (const LocationId &locationId)
{
    m_locationId = locationId;
}

void VcsNodeLocalManagedObject::setSwitchState (const UI32 &switchState)
{
    m_switchState = switchState;
}

void    VcsNodeLocalManagedObject::setSwitchDeleteAfterFailoverFlag (const bool &switchDeleteAfterFailoverFlag) 
{
    m_switchDeleteAfterFailover = switchDeleteAfterFailoverFlag;
}

void VcsNodeLocalManagedObject::setSwitchPort (SI32 switchPort)
{
    m_switchPort = switchPort;
}

void VcsNodeLocalManagedObject::setSwitchWWN (const WorldWideName &switchWWN)
{
    m_switchWWN = switchWWN;
}

void VcsNodeLocalManagedObject::setSWBDModel (int SWBDModel)
{
    m_SWBDModel = SWBDModel;
}

void VcsNodeLocalManagedObject::setFwVersion (const string &fwVersion)
{
    m_fwVersion = fwVersion;
}


void VcsNodeLocalManagedObject::setSwitchname (const string &switchname)
{
    m_switchname = switchname;
}

void VcsNodeLocalManagedObject::setSwmodelName (const string &swmodelName)
{
    m_swmodelName = swmodelName;
}

void VcsNodeLocalManagedObject::setPrincipalPriority (const UI32 principalPriority)
{
    m_principalPriority = principalPriority;
}

UI32 VcsNodeLocalManagedObject::getVcsId() const
{
    return (m_vcsId);
}

UI32 VcsNodeLocalManagedObject::getMappedId() const
{
    // MappedId has been depricated so we return domainId
    return (m_domainId);
}

UI32 VcsNodeLocalManagedObject::getDomainId() const
{
    return (m_domainId);
}

MacAddress VcsNodeLocalManagedObject::getSwitchMac() const
{
    return (m_switchMac);
}

string VcsNodeLocalManagedObject::getSerialNumber() const
{
    return (m_serialNumber);
}

bool VcsNodeLocalManagedObject::getVcsMode() const
{
    return (m_vcsMode);
}

MacAddress VcsNodeLocalManagedObject::getFcfMac() const
{
    return (m_fcfMac);
}

bool VcsNodeLocalManagedObject::getIsPrincipal () const
{
	return m_isPrincipal;
}

bool VcsNodeLocalManagedObject::getWillingMode () const
{
	return m_willingMode;
}

bool VcsNodeLocalManagedObject::getInVcs () const
{
	return m_inVcs;
}

vector<string> VcsNodeLocalManagedObject::getPublicIpAddress () const
{
    return m_publicIpAddress;
}

LocationId VcsNodeLocalManagedObject::getLocationId () const
{
    return m_locationId;
}

void VcsNodeLocalManagedObject::addVcsSlotLocalManagedObject       (VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject)
{
    m_vcsSlotLocalManagedObjects.push_back (WaveManagedObjectPointer<VcsSlotLocalManagedObject> (pVcsSlotLocalManagedObject));
}


void VcsNodeLocalManagedObject::removeVcsSlotLocalManagedObject (VcsSlotLocalManagedObject *pVcsSlotLocalManagedObject )
{
    ObjectId vcsSlotMOObjectId = pVcsSlotLocalManagedObject->getObjectId ();
    vector<WaveManagedObjectPointer<VcsSlotLocalManagedObject> >::iterator element    = m_vcsSlotLocalManagedObjects.begin ();
    vector<WaveManagedObjectPointer<VcsSlotLocalManagedObject> >::iterator endElement = m_vcsSlotLocalManagedObjects.end ();
    bool   nodeFound  = false;

    while (element != endElement)
    {
        if (vcsSlotMOObjectId == (*element)->getObjectId())
        {
            nodeFound = true;
            break;
        }

        element++;
    }

    if (true == nodeFound)
    {
        m_vcsSlotLocalManagedObjects.erase (element);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "VcsNodeLocalManagedObject::removeVcsSlotLocalManagedObject: vcsSlotManagedObject object to remove not found");
    }
    
}

void VcsNodeLocalManagedObject::removeAllVcsSlotLocalManagedObjects () 
{
    m_vcsSlotLocalManagedObjects.clear ();
}

UI32   VcsNodeLocalManagedObject::getSwitchState () const
{
    return m_switchState;
}

bool VcsNodeLocalManagedObject::getSwitchDeleteAfterFailover () const
{
    return m_switchDeleteAfterFailover;
}

SI32 VcsNodeLocalManagedObject::getSwitchPort () const
{
    return m_switchPort;
}

WorldWideName VcsNodeLocalManagedObject::getSwitchWWN () const
{
    return m_switchWWN;
}

SI32 VcsNodeLocalManagedObject::getSWBDModel () const
{
    return m_SWBDModel;
}

string VcsNodeLocalManagedObject::getFwVersion () const
{
    return m_fwVersion;
}

string VcsNodeLocalManagedObject::getSwitchname () const
{
    return m_switchname;
}

string VcsNodeLocalManagedObject::getSwmodelName () const
{
    return m_swmodelName;
}

UI32 VcsNodeLocalManagedObject::getPrincipalPriority () const
{
    return m_principalPriority;
}

void VcsNodeLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_serialNumber, "serialNumber"));
    addPersistableAttribute (new AttributeUI32   (&m_vcsId,        "vcsId"));
    addPersistableAttribute (new AttributeUI32   (&m_domainId,     "domainId", rbridge_rbridge_id));
    addPersistableAttribute (new AttributeMacAddress (&m_fcfMac,       "fcfMac"));
    addPersistableAttribute (new AttributeMacAddress (&m_switchMac,    "switchMac"));
    addPersistableAttribute (new AttributeBool   (&m_vcsMode,      "vcsMode"));
    addPersistableAttribute (new AttributeBool   (&m_inVcs,        "inVcs"));
    addPersistableAttribute (new AttributeBool   (&m_isPrincipal,  "isPrincipal"));
    addPersistableAttribute (new AttributeUI64   (&m_xactionId,    "xactionId"));
    addPersistableAttribute (new AttributeBool   (&m_willingMode,      "willingMode"));
    addPersistableAttribute (new AttributeStringVector (&m_publicIpAddress, "publicIpAddress"));
    addPersistableAttribute (new AttributeUI32 (&m_locationId, "locationId"));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition<VcsSlotLocalManagedObject> (&m_vcsSlotLocalManagedObjects, "slotManagedObjects", getClassName (), getObjectId (), VcsSlotLocalManagedObject::getClassName ()));
    addPersistableAttribute (new AttributeBool (&m_switchDeleteAfterFailover, "switchDeleteAfterFailover"));
    addPersistableAttribute (new AttributeSI32 (&m_switchPort, "switchPort"));
    addPersistableAttribute (new AttributeWorldWideName (&m_switchWWN, "switchWWN"));
    addPersistableAttribute (new AttributeSI32   (&m_SWBDModel, "SWBDModel", rbridge_swbd_number));
    addPersistableAttribute (new AttributeString (&m_fwVersion, "fwVersion"));
    addPersistableAttribute (new AttributeString (&m_switchname, "switchname"));
    addPersistableAttribute (new AttributeString (&m_swmodelName, "swmodelName"));
    addPersistableAttribute (new AttributeUI32   (&m_principalPriority, 0, "principalPriority", brocade_logical_chassis_principal_priority));
}

void VcsNodeLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString (&m_serialNumber, "serialNumber"));
    addPersistableAttributeForCreate (new AttributeUI32   (&m_vcsId,        "vcsId"));
    addPersistableAttributeForCreate (new AttributeUI32   (&m_domainId,     "domainId", rbridge_rbridge_id));
    addPersistableAttributeForCreate (new AttributeMacAddress (&m_fcfMac,       "fcfMac"));
    addPersistableAttributeForCreate (new AttributeMacAddress (&m_switchMac,    "switchMac"));
    addPersistableAttributeForCreate (new AttributeBool   (&m_vcsMode,      "vcsMode"));
    addPersistableAttributeForCreate (new AttributeBool   (&m_inVcs,        "inVcs"));
    addPersistableAttributeForCreate (new AttributeBool   (&m_isPrincipal,  "isPrincipal"));
    addPersistableAttributeForCreate (new AttributeUI64   (&m_xactionId,    "xactionId"));
    addPersistableAttributeForCreate (new AttributeBool   (&m_willingMode,  "willingMode"));
    addPersistableAttributeForCreate (new AttributeStringVector (&m_publicIpAddress, "publicIpAddress"));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_locationId, "locationId"));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<VcsSlotLocalManagedObject> (&m_vcsSlotLocalManagedObjects, "slotManagedObjects", getClassName (), getObjectId (), VcsSlotLocalManagedObject::getClassName ()));
    addPersistableAttributeForCreate (new AttributeBool   (&m_switchDeleteAfterFailover, "switchDeleteAfterFailover"));
    addPersistableAttributeForCreate (new AttributeSI32   (&m_switchPort, "switchPort"));
    addPersistableAttributeForCreate (new AttributeWorldWideName (&m_switchWWN, "switchWWN"));
    addPersistableAttributeForCreate (new AttributeSI32   (&m_SWBDModel, "SWBDModel", rbridge_swbd_number));
    addPersistableAttributeForCreate (new AttributeString (&m_fwVersion, "fwVersion"));
    addPersistableAttributeForCreate (new AttributeString (&m_switchname, "switchname"));
    addPersistableAttributeForCreate (new AttributeString (&m_swmodelName, "swmodelName"));
    addPersistableAttributeForCreate (new AttributeUI32   (&m_principalPriority, 0, "principalPriority", brocade_logical_chassis_principal_priority));

    vector<string > keyName;
    keyName.push_back ("domainId");
    setUserDefinedKeyCombination (keyName);
}

}
