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




#include "Bgp/Local/BgpLocalAfIpv4NeighborPeerGrpManagedObject.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"
#include "Bgp/Local/BgpLocalNeighborPeerGrpManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAfIpv4NeighborPeerGrpManagedObject::BgpLocalAfIpv4NeighborPeerGrpManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalAfNeighborAttributesCommonManagedObjectBase(this)
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
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    BgpLocalAfNeighborAttributesCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_afIpv4NeighborPeerGrpName, "afIpv4NeighborPeerGrpName", BgpLocalNeighborPeerGrpManagedObject::getClassName(), true,bgp_af_ipv4_neighbor_peergroup_name));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    BgpLocalAfNeighborAttributesCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_afIpv4NeighborPeerGrpName, "afIpv4NeighborPeerGrpName", BgpLocalNeighborPeerGrpManagedObject::getClassName(), true,bgp_af_ipv4_neighbor_peergroup_name));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("afIpv4NeighborPeerGrpName");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
BgpLocalAfIpv4NeighborPeerGrpManagedObject::BgpLocalAfIpv4NeighborPeerGrpManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const ObjectId &afIpv4NeighborPeerGrpName)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalAfNeighborAttributesCommonManagedObjectBase(this),
    m_afIpv4NeighborPeerGrpName(afIpv4NeighborPeerGrpName)
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
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfIpv4NeighborPeerGrpManagedObject::getClassName()
{
    return ("BgpLocalAfIpv4NeighborPeerGrpManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfIpv4NeighborPeerGrpManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAfIpv4NeighborPeerGrpManagedObject::~BgpLocalAfIpv4NeighborPeerGrpManagedObject()
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
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::setafIpv4NeighborPeerGrpName:This function is used to set the value of member m_afIpv4NeighborPeerGrpName
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborPeerGrpManagedObject::setafIpv4NeighborPeerGrpName (const ObjectId &afIpv4NeighborPeerGrpName)
{
    m_afIpv4NeighborPeerGrpName = afIpv4NeighborPeerGrpName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborPeerGrpManagedObject::getafIpv4NeighborPeerGrpName:This function is used to get the value of member m_afIpv4NeighborPeerGrpName
 * @uBreezeClose
 */
ObjectId BgpLocalAfIpv4NeighborPeerGrpManagedObject::getafIpv4NeighborPeerGrpName ()
{
    return (m_afIpv4NeighborPeerGrpName);
}



}

