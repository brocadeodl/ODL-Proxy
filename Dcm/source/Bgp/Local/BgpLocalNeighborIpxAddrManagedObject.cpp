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




#include "Bgp/Local/BgpLocalNeighborIpxAddrManagedObject.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"
#include "Bgp/Local/BgpLocalNeighborPeerGrpManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalNeighborIpxAddrManagedObject::BgpLocalNeighborIpxAddrManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalNeighborIpxAddrManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalNeighborAttributesCommonManagedObjectBase(this)
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add any additional handling needed for ManagedObject constructor
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add any additional handling needed for ManagedObject constructor
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    BgpLocalNeighborAttributesCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeIpVxAddress (&m_neighborIpvXAddress, "neighborIpvXAddress",bgp_router_bgp_neighbor_address));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_associatePeerGroup, "associatePeerGroup", BgpLocalNeighborPeerGrpManagedObject::getClassName(), true,bgp_associate_peer_group));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    BgpLocalNeighborAttributesCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeIpVxAddress (&m_neighborIpvXAddress, "neighborIpvXAddress",bgp_router_bgp_neighbor_address));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_associatePeerGroup, "associatePeerGroup", BgpLocalNeighborPeerGrpManagedObject::getClassName(), true,bgp_associate_peer_group));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("neighborIpvXAddress");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
BgpLocalNeighborIpxAddrManagedObject::BgpLocalNeighborIpxAddrManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const IpVxAddress &neighborIpvXAddress,
                                                                                                                const ObjectId &associatePeerGroup)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalNeighborIpxAddrManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalNeighborAttributesCommonManagedObjectBase(this),
    m_neighborIpvXAddress(neighborIpvXAddress),
    m_associatePeerGroup(associatePeerGroup)
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add any additional handling needed for ManagedObject constructor
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add any additional handling needed for ManagedObject constructor
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalNeighborIpxAddrManagedObject::getClassName()
{
    return ("BgpLocalNeighborIpxAddrManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalNeighborIpxAddrManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalNeighborIpxAddrManagedObject::~BgpLocalNeighborIpxAddrManagedObject()
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add the Destructor handling for the ManagedObject
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add the Destructor handling for the ManagedObject
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::setneighborIpvXAddress:This function is used to set the value of member m_neighborIpvXAddress
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::setneighborIpvXAddress (const IpVxAddress &neighborIpvXAddress)
{
    m_neighborIpvXAddress = neighborIpvXAddress;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::getneighborIpvXAddress:This function is used to get the value of member m_neighborIpvXAddress
 * @uBreezeClose
 */
IpVxAddress BgpLocalNeighborIpxAddrManagedObject::getneighborIpvXAddress ()
{
    return (m_neighborIpvXAddress);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::setassociatePeerGroup:This function is used to set the value of member m_associatePeerGroup
 * @uBreezeClose
 */
void BgpLocalNeighborIpxAddrManagedObject::setassociatePeerGroup (const ObjectId &associatePeerGroup)
{
    m_associatePeerGroup = associatePeerGroup;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborIpxAddrManagedObject::getassociatePeerGroup:This function is used to get the value of member m_associatePeerGroup
 * @uBreezeClose
 */
ObjectId BgpLocalNeighborIpxAddrManagedObject::getassociatePeerGroup ()
{
    return (m_associatePeerGroup);
}



}

