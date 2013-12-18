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
 *   Copyright (C) 2008-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "VcsFabric/Local/VcsFabricLocalSerializableObject.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeBitMap.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

using namespace WaveNs;

namespace DcmNs
{

VcsFabricLocalSwitchInfoObject::VcsFabricLocalSwitchInfoObject ()
    :   SerializableObject              (),
        m_switchDomainId                (0),
        m_switchVcsId                   (0),
        m_switchClusterMgmtPort         (0),
        m_switchInCluster               (false),
        m_switchIsPrincipal             (false),
        m_nodeState                     (0),
        m_locationRole                  (0),
        m_switchIsUnconfirmPrincipal    (false),
        m_myPrincipalDomainId           (0),
        m_switchLocationId              (256)
{
    m_dcaDomainsBitMap.resize (max_domain_list - 1);
    m_dcaDomainsBitMap.reset (); 
}

VcsFabricLocalSwitchInfoObject::VcsFabricLocalSwitchInfoObject  (const VcsFabricLocalSwitchInfoObject &vcsFabricLocalSwitchInfoObject)
    : SerializableObject ()
{
    *this = vcsFabricLocalSwitchInfoObject;
}

VcsFabricLocalSwitchInfoObject::~VcsFabricLocalSwitchInfoObject ()
{
}

LocationId  VcsFabricLocalSwitchInfoObject::getSwitchLocationId () const
{
    return (m_switchLocationId);
}

void VcsFabricLocalSwitchInfoObject::setSwitchLocationId (const LocationId &locationId)
{
    m_switchLocationId = locationId;
}

UI32 VcsFabricLocalSwitchInfoObject::getPrincipalSwitchDomainId () const
{
    return (m_myPrincipalDomainId);
}

void VcsFabricLocalSwitchInfoObject::setPrincipalSwitchDomainId (const UI32 &domainId)
{
    m_myPrincipalDomainId = domainId;
}

bool VcsFabricLocalSwitchInfoObject::getIsSwitchUnconfirmPrincipal () const
{
    return (m_switchIsUnconfirmPrincipal);
}

void VcsFabricLocalSwitchInfoObject::setIsSwitchUnconfirmPrincipal (const bool &isPrincipal)
{
    m_switchIsUnconfirmPrincipal = isPrincipal;
}

UI32 VcsFabricLocalSwitchInfoObject::getLocationRole () const
{
    return (m_locationRole);
}

void VcsFabricLocalSwitchInfoObject::setLocationRole (const UI32 &locationRole)
{
    m_locationRole = locationRole;
}

UI32 VcsFabricLocalSwitchInfoObject::getNodeState () const
{
    return (m_nodeState);
}

void VcsFabricLocalSwitchInfoObject::setNodeState (const UI32 &nodeState)
{
    m_nodeState = nodeState;
}

BitMap VcsFabricLocalSwitchInfoObject::getDcaBitMap () const
{
    return (m_dcaDomainsBitMap);
}

void VcsFabricLocalSwitchInfoObject::setDcaBitMap (const BitMap &dcaBitMap)
{
    m_dcaDomainsBitMap = dcaBitMap;
}

bool VcsFabricLocalSwitchInfoObject::getIsPrincipal () const
{
    return (m_switchIsPrincipal);
}

void VcsFabricLocalSwitchInfoObject::setIsPrincipal (const bool &isPrincipal)
{
    m_switchIsPrincipal = isPrincipal;
}

bool VcsFabricLocalSwitchInfoObject::getIsSwitchInCluster () const
{
    return (m_switchInCluster);
}

void VcsFabricLocalSwitchInfoObject::setIsSwitchInCluster (const bool &inCluster)
{
    m_switchInCluster = inCluster;
}

vector<string> VcsFabricLocalSwitchInfoObject::getSwitchPublicIpAddress () const
{
    return (m_switchPublicIpAddress);
}

void VcsFabricLocalSwitchInfoObject::setSwitchPublicIpAddress (const vector<string> &ipAddress)
{
    m_switchPublicIpAddress = ipAddress;
}

UI32 VcsFabricLocalSwitchInfoObject::getSwitchClusterManagementPort () const
{
    return (m_switchClusterMgmtPort);
}

void VcsFabricLocalSwitchInfoObject::setSwitchClusterManagementPort (const UI32 &port)
{
    m_switchClusterMgmtPort = port;
}

string VcsFabricLocalSwitchInfoObject::getSwitchClusterManagementIpAddress () const
{
    return (m_switchClusterMgmtIpAddress);
}

void VcsFabricLocalSwitchInfoObject::setSwitchClusterManagementIpAddress (const string &ipAddress)
{
    m_switchClusterMgmtIpAddress = ipAddress;
}

UI32 VcsFabricLocalSwitchInfoObject::getSwitchVcsId () const
{
    return (m_switchVcsId);
}

void VcsFabricLocalSwitchInfoObject::setSwitchVcsId (const UI32 &vcsId)
{
    m_switchVcsId = vcsId;
}

string VcsFabricLocalSwitchInfoObject::getSwitchSWVersion () const
{
   return (m_switchSWVersion);
}

void VcsFabricLocalSwitchInfoObject::setSwitchSWVersion (const string &swVersion)
{
    m_switchSWVersion = swVersion;
}

string VcsFabricLocalSwitchInfoObject::getSwitchSerialNumber () const
{
    return (m_switchSerialNumber);
}

void VcsFabricLocalSwitchInfoObject::setSwitchSerialNumber (const string &serialNumber)
{
    m_switchSerialNumber = serialNumber;
}

UI32 VcsFabricLocalSwitchInfoObject::getSwitchDomainId () const
{
    return (m_switchDomainId);
}

void VcsFabricLocalSwitchInfoObject::setSwitchDomainId (const UI32 &domainId)
{
    m_switchDomainId = domainId;
}

vector<SI32>	VcsFabricLocalSwitchInfoObject::getReplacementSwitchSwbd			() const
{
	return m_replacementSwitchSwbd;
}

void  VcsFabricLocalSwitchInfoObject::addReplacementSwitchSwbd					(const SI32 &replacementSwitchSwbd)
{
	m_replacementSwitchSwbd.push_back (replacementSwitchSwbd);
}

vector<WorldWideName>	VcsFabricLocalSwitchInfoObject::getReplacementSwitchWWN 			() const
{
	return m_replacementSwitchWWN;
}

void  VcsFabricLocalSwitchInfoObject::addReplacementSwitchWWN 					(const WorldWideName &replacementSwitchWWN)
{
	m_replacementSwitchWWN.push_back (replacementSwitchWWN);
}

void VcsFabricLocalSwitchInfoObject::setupAttributesForSerialization ()
{
    SerializableObject::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32         (&m_switchDomainId,             "switchDomainId"));
    addSerializableAttribute (new AttributeString       (&m_switchSerialNumber,         "switchSerialNumber"));
    addSerializableAttribute (new AttributeString       (&m_switchSWVersion,            "switchSWVersion"));
    addSerializableAttribute (new AttributeUI32         (&m_switchVcsId,                "switchVcsId"));
    addSerializableAttribute (new AttributeString       (&m_switchClusterMgmtIpAddress, "switchClusterMgmtIpAddress"));
    addSerializableAttribute (new AttributeUI32         (&m_switchClusterMgmtPort,      "switchClusterMgmtPort"));
    addSerializableAttribute (new AttributeStringVector (&m_switchPublicIpAddress,      "switchPublicIpAddress"));
    addSerializableAttribute (new AttributeBool         (&m_switchInCluster,            "switchInCluster"));
    addSerializableAttribute (new AttributeBool         (&m_switchIsPrincipal,          "switchIsPrincipal"));
    addSerializableAttribute (new AttributeBitMap       (&m_dcaDomainsBitMap,           "dcaDomainsBitMap"));  
    addSerializableAttribute (new AttributeUI32         (&m_nodeState,                  "nodeState"));
    addSerializableAttribute (new AttributeUI32         (&m_locationRole,               "locationRole"));
    addSerializableAttribute (new AttributeBool         (&m_switchIsUnconfirmPrincipal, "switchIsUnconfirmPrincipal"));
    addSerializableAttribute (new AttributeUI32         (&m_myPrincipalDomainId,        "myPrincipalDomainId"));
    addSerializableAttribute (new AttributeLocationId   (&m_switchLocationId,           "switchLocationId"));
	addSerializableAttribute (new AttributeSI32Vector   (&m_replacementSwitchSwbd,		"switchModelSwbd"));
	addSerializableAttribute (new AttributeWorldWideNameVector   (&m_replacementSwitchWWN,	"switchWWN"));
	
}

}

