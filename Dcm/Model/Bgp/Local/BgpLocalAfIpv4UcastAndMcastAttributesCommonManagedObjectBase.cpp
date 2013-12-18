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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/




#include "Bgp/Local/BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalMaxiumPathCommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalMultiPathCommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalAfBasicAttributesCommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalAfCommonAttributesCommonManagedObjectBase(pPrismPersistableObject)
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add any additional handling needed for CommonManagedObjectBase constructor
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add any additional handling needed for CommonManagedObjectBase constructor
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    BgpLocalAfCommonAttributesCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttributeForCommonBase (new AttributeManagedObjectVectorComposition <BgpLocalAfIpv4NeighborIpAddrManagedObject> (&m_addressFamilyNeighborIpAddr, "addressFamilyNeighborIpAddr", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName(),bgp_af_ipv4_neighbor_address));
    addPersistableAttributeForCommonBase (new AttributeManagedObjectVectorComposition <BgpLocalAfIpv4NeighborPeerGrpManagedObject> (&m_addressFamilyNeighborPeerGrp, "addressFamilyNeighborPeerGrp", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName(),bgp_af_ipv4_neighbor_peergroup));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase:: setupAttributesForCreate ()
{
    BgpLocalAfCommonAttributesCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCommonBaseCreate (new AttributeManagedObjectVectorComposition <BgpLocalAfIpv4NeighborIpAddrManagedObject> (&m_addressFamilyNeighborIpAddr, "addressFamilyNeighborIpAddr", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName(),bgp_af_ipv4_neighbor_address));
    addPersistableAttributeForCommonBaseCreate (new AttributeManagedObjectVectorComposition <BgpLocalAfIpv4NeighborPeerGrpManagedObject> (&m_addressFamilyNeighborPeerGrp, "addressFamilyNeighborPeerGrp", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName(),bgp_af_ipv4_neighbor_peergroup));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::~BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase()
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add the Destructor handling for the CommonManagedObjectBase
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add the Destructor handling for the CommonManagedObjectBase
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::setaddressFamilyNeighborIpAddr:This function is used to set the value of member m_addressFamilyNeighborIpAddr
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::setaddressFamilyNeighborIpAddr (const vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborIpAddrManagedObject> > &addressFamilyNeighborIpAddr)
{
    m_addressFamilyNeighborIpAddr = addressFamilyNeighborIpAddr;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::getaddressFamilyNeighborIpAddr:This function is used to get the value of member m_addressFamilyNeighborIpAddr
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborIpAddrManagedObject> > BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::getaddressFamilyNeighborIpAddr ()
{
    return (m_addressFamilyNeighborIpAddr);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::setaddressFamilyNeighborPeerGrp:This function is used to set the value of member m_addressFamilyNeighborPeerGrp
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::setaddressFamilyNeighborPeerGrp (const vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborPeerGrpManagedObject> > &addressFamilyNeighborPeerGrp)
{
    m_addressFamilyNeighborPeerGrp = addressFamilyNeighborPeerGrp;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::getaddressFamilyNeighborPeerGrp:This function is used to get the value of member m_addressFamilyNeighborPeerGrp
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<BgpLocalAfIpv4NeighborPeerGrpManagedObject> > BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::getaddressFamilyNeighborPeerGrp ()
{
    return (m_addressFamilyNeighborPeerGrp);
}



}

