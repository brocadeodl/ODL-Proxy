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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Cluster/Local/WaveNode.h"
#include "Framework/Attributes/AttributeResourceId.h"
#include "Cluster/Local/WaveHaNode.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace WaveNs
{

WaveNode::WaveNode (WaveObjectManager *pWaveObjectManager)
    : PrismElement                      (pWaveObjectManager),
      PrismPersistableObject            (WaveNode::getClassName (), WaveLocalManagedObject::getClassName ()),
      WaveManagedObject                 (pWaveObjectManager),
      WaveLocalManagedObjectBase        (this),
      WaveLocalManagedObject            (pWaveObjectManager),
      m_locationId                      (0),
      m_port                            (0),
      m_hardwareSynchronizationState    (WAVE_NODE_HARDWARE_SYNCHRONIZATION_STATE_UNINITIALIZED),
      m_waveNodeContollerPort           (0)
{
    setOwnerWaveNodeObjectId    (getObjectId ());
    setGenericStatus            (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD);
    setSpecificStatus           (WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_STAND_ALONE);
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
    setThisNodeStartMode (WAVE_NODE_START_AS_NORMAL);
}

WaveNode::WaveNode (WaveObjectManager *pWaveObjectManager, const LocationId &locationId, const string &ipAddress, const UI32 &port)
    : PrismElement                      (pWaveObjectManager),
      PrismPersistableObject            (WaveNode::getClassName (), WaveLocalManagedObject::getClassName ()),
      WaveManagedObject                 (pWaveObjectManager),
      WaveLocalManagedObjectBase        (this),
      WaveLocalManagedObject            (pWaveObjectManager),
      m_locationId                      (locationId),
      m_ipAddress                       (ipAddress),
      m_port                            (port),
      m_hardwareSynchronizationState    (WAVE_NODE_HARDWARE_SYNCHRONIZATION_STATE_UNINITIALIZED),
      m_waveNodeContollerPort           (0)
{
    setOwnerWaveNodeObjectId    (getObjectId ());
    setGenericStatus            (WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD);
    setSpecificStatus           (WAVE_MANAGED_OBJECT_SPECIFIC_STATUS_STAND_ALONE);
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
    setThisNodeStartMode (WAVE_NODE_START_AS_NORMAL);
}

WaveNode::WaveNode (const WaveNode &waveNode)
    : PrismElement               (waveNode.getPWaveObjectManager ()),
      PrismPersistableObject     (WaveNode::getClassName (), WaveLocalManagedObject::getClassName ()),
      WaveManagedObject          (waveNode.getPWaveObjectManager ()),
      WaveLocalManagedObjectBase (this),
      WaveLocalManagedObject     (waveNode.getPWaveObjectManager ())
{
    m_locationId                    = waveNode.m_locationId;
    m_ipAddress                     = waveNode.m_ipAddress;
    m_port                          = waveNode.m_port;
    m_hardwareSynchronizationState  = waveNode.m_hardwareSynchronizationState;
    m_waveNodeControllerIpAddress   = waveNode.m_waveNodeControllerIpAddress;
    m_waveNodeContollerPort         = waveNode.m_waveNodeContollerPort;

    setGenericStatus (waveNode.getGenericStatus ());
    setSpecificStatus (waveNode.getSpecificStatus ());
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
    setThisNodeStartMode (WAVE_NODE_START_AS_NORMAL);
}

WaveNode::~WaveNode ()
{
}

string WaveNode::getClassName ()
{
    return ("WaveNode");
}

void WaveNode::setupAttributesForPersistence ()
{
    WaveLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeLocationId (&m_locationId, "locationId"));
    addPersistableAttribute (new AttributeString     (&m_ipAddress,  "ipAddress"));
    addPersistableAttribute (new AttributeUI32       (&m_port,       "port"));
    addPersistableAttribute (new AttributeResourceId (&m_hardwareSynchronizationState,  "hardwareSynchronizationState"));
    addPersistableAttribute (new AttributeObjectIdVectorAssociation (&m_waveHaNodes,    "waveHaNodes", getClassName (), getObjectId (), WaveHaNode::getClassName ()));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition<WaveSlotLocalManagedObject> (&m_waveSlotLocalManagedObjects, "waveSlotManagedObjects", getClassName (), getObjectId (), WaveSlotLocalManagedObject::getClassName ()));
    addPersistableAttribute (new AttributeIpVxAddress (&m_waveNodeControllerIpAddress,  "waveNodeControllerIpAddress"));
    addPersistableAttribute (new AttributeUI32        (&m_waveNodeContollerPort, "waveNodeContollerPort"));
    addPersistableAttribute (new AttributeResourceId  (&m_waveNodeStartMode, "waveNodeStartMode"));
}

void WaveNode::setupAttributesForCreate ()
{
    WaveLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeLocationId (&m_locationId, "locationId"));
    addPersistableAttributeForCreate (new AttributeString     (&m_ipAddress,  "ipAddress"));
    addPersistableAttributeForCreate (new AttributeUI32       (&m_port,       "port"));
    addPersistableAttributeForCreate (new AttributeResourceId (&m_hardwareSynchronizationState, "hardwareSynchronizationState"));
    addPersistableAttributeForCreate (new AttributeObjectIdVectorAssociation (&m_waveHaNodes,    "waveHaNodes", getClassName (), getObjectId (), WaveHaNode::getClassName ()));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<WaveSlotLocalManagedObject> (&m_waveSlotLocalManagedObjects, "waveSlotManagedObjects", getClassName (), getObjectId (), WaveSlotLocalManagedObject::getClassName ()));
    addPersistableAttributeForCreate (new AttributeIpVxAddress (&m_waveNodeControllerIpAddress,  "waveNodeControllerIpAddress"));
    addPersistableAttributeForCreate (new AttributeUI32        (&m_waveNodeContollerPort, "waveNodeContollerPort"));
    addPersistableAttributeForCreate (new AttributeResourceId  (&m_waveNodeStartMode, "waveNodeStartMode"));
}

WaveNode &WaveNode::operator = (const WaveNode &waveNode)
{
    m_locationId                    = waveNode.m_locationId;
    m_ipAddress                     = waveNode.m_ipAddress;
    m_port                          = waveNode.m_port;
    m_hardwareSynchronizationState  = waveNode.m_hardwareSynchronizationState;
    m_waveNodeControllerIpAddress   = waveNode.m_waveNodeControllerIpAddress;
    m_waveNodeContollerPort         = waveNode.m_waveNodeContollerPort;

    return (*this);
}

bool WaveNode::operator == (const WaveNode &waveNode)
{
    if (m_locationId != waveNode.m_locationId)
    {
        return (false);
    }

    if (m_ipAddress != waveNode.m_ipAddress)
    {
        return (false);
    }

    if (m_port != waveNode.m_port)
    {
        return (false);
    }

    if (m_hardwareSynchronizationState != waveNode.m_hardwareSynchronizationState)
    {
        return (false);
    }

    if (m_waveNodeContollerPort != waveNode.m_waveNodeContollerPort)
    {
        return (false);
    }

    if (m_waveNodeControllerIpAddress != waveNode.m_waveNodeControllerIpAddress)
    {
        return (false);
    }

    return (true);
}

bool WaveNode::operator != (const WaveNode &waveNode)
{
    return (!(operator == (waveNode)));
}

LocationId WaveNode::getLocationId () const
{
    return (m_locationId);
}

void WaveNode::setLocationId (const LocationId &locationId)
{
    m_locationId = locationId;
}

string WaveNode::getIpAddress () const
{
    return (m_ipAddress);
}

void WaveNode::setIpAddress (const string &ipAddress)
{
    m_ipAddress = ipAddress;
}

UI32 WaveNode::getPort () const
{
    return (m_port);
}

void WaveNode::setPort (const UI32 &port)
{
    m_port = port;
}

ResourceId WaveNode::getHardwareSynchronizationState () const
{
    return (m_hardwareSynchronizationState);
}

void WaveNode::setHardwareSynchronizationState (ResourceId hardwareSynchronizationState)
{
    m_hardwareSynchronizationState = hardwareSynchronizationState;
}

void WaveNode::addWaveHaNode (const ObjectId &waveHaNodeObjectId)
{
    m_waveHaNodes.push_back (waveHaNodeObjectId);
}

void WaveNode::removeWaveHaNode (const ObjectId &waveHaNodeObjectId)
{
    vector<ObjectId>::iterator element    = m_waveHaNodes.begin ();
    vector<ObjectId>::iterator endElement = m_waveHaNodes.end ();
    bool                       nodeFound  = false;

    while (element != endElement)
    {
        if (waveHaNodeObjectId == (*element))
        {
            nodeFound = true;
            break;
        }

        element++;
    }

    if (true == nodeFound)
    {
        m_waveHaNodes.erase (element);
    }
    else
    {
    }
}

void WaveNode::removeAllHaNodes ()
{
    m_waveHaNodes.clear ();
}
void WaveNode::addWaveSlotLocalManagedObject       (WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject)
{
    m_waveSlotLocalManagedObjects.push_back (WaveManagedObjectPointer<WaveSlotLocalManagedObject> (pWaveSlotLocalManagedObject));
}


void WaveNode::removeWaveSlotLocalManagedObject          (WaveSlotLocalManagedObject *pWaveSlotLocalManagedObject )
{
    ObjectId waveSlotMOObjectId = pWaveSlotLocalManagedObject->getObjectId ();
    vector<WaveManagedObjectPointer<WaveSlotLocalManagedObject> >::iterator element    = m_waveSlotLocalManagedObjects.begin ();
    vector<WaveManagedObjectPointer<WaveSlotLocalManagedObject> >::iterator endElement = m_waveSlotLocalManagedObjects.end ();
    bool                       nodeFound  = false;

    while (element != endElement)
    {
        if (waveSlotMOObjectId == (*element)->getObjectId())
        {
            nodeFound = true;
            break;
        }

        element++;
    }

    if (true == nodeFound)
    {
        m_waveSlotLocalManagedObjects.erase (element);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "WaveNode::removeWaveSlotLocalManagedObject: waveSlotManagedObject object to remove not found");
    }
    
}

void WaveNode::removeAllWaveSlotLocalManagedObjects ()
{
    m_waveSlotLocalManagedObjects.clear ();
}

IpVxAddress WaveNode::getContollerIpAddress () const
{
    return (m_waveNodeControllerIpAddress);
}

void WaveNode::setContollerIpAddress (const IpVxAddress &ipAddress)
{
    m_waveNodeControllerIpAddress = ipAddress;
}

UI32 WaveNode::getControllerPort () const
{
    return (m_waveNodeContollerPort);
}

void WaveNode::setControllerPort (const UI32 &port)
{
    m_waveNodeContollerPort = port;
}

void WaveNode::setThisNodeStartMode (const ResourceId &startMode)
{
    m_waveNodeStartMode = startMode;
}

ResourceId WaveNode::getThisNodeStartMode ()
{
    return (m_waveNodeStartMode);
}


}
