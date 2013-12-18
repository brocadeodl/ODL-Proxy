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




#include "Bgp/Local/BgpLocalNeighborPeerGrpManagedObject.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalNeighborPeerGrpManagedObject::BgpLocalNeighborPeerGrpManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalNeighborPeerGrpManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalNeighborAttributesCommonManagedObjectBase(this),
    m_neighborPeerGrpName(""),
    m_peerGrpFlag(false)
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
 * Brief:BgpLocalNeighborPeerGrpManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    BgpLocalNeighborAttributesCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeString (&m_neighborPeerGrpName, true, "", "neighborPeerGrpName",bgp_router_bgp_neighbor_peer_grp));
    addPersistableAttribute (new AttributeBool (&m_peerGrpFlag, true, "false", "peerGrpFlag",bgp_peer_group_name));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    BgpLocalNeighborAttributesCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeString (&m_neighborPeerGrpName, true, "", "neighborPeerGrpName",bgp_router_bgp_neighbor_peer_grp));
    addPersistableAttributeForCreate (new AttributeBool (&m_peerGrpFlag, true, "false", "peerGrpFlag",bgp_peer_group_name));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("neighborPeerGrpName");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
BgpLocalNeighborPeerGrpManagedObject::BgpLocalNeighborPeerGrpManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const string &neighborPeerGrpName,
                                                                                                                const bool &peerGrpFlag)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalNeighborPeerGrpManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalNeighborAttributesCommonManagedObjectBase(this),
    m_neighborPeerGrpName(neighborPeerGrpName),
    m_peerGrpFlag(peerGrpFlag)
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
 * Brief:BgpLocalNeighborPeerGrpManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalNeighborPeerGrpManagedObject::getClassName()
{
    return ("BgpLocalNeighborPeerGrpManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalNeighborPeerGrpManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalNeighborPeerGrpManagedObject::~BgpLocalNeighborPeerGrpManagedObject()
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
 * Brief:BgpLocalNeighborPeerGrpManagedObject::setneighborPeerGrpName:This function is used to set the value of member m_neighborPeerGrpName
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::setneighborPeerGrpName (const string &neighborPeerGrpName)
{
    m_neighborPeerGrpName = neighborPeerGrpName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject::getneighborPeerGrpName:This function is used to get the value of member m_neighborPeerGrpName
 * @uBreezeClose
 */
string BgpLocalNeighborPeerGrpManagedObject::getneighborPeerGrpName ()
{
    return (m_neighborPeerGrpName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject::setpeerGrpFlag:This function is used to set the value of member m_peerGrpFlag
 * @uBreezeClose
 */
void BgpLocalNeighborPeerGrpManagedObject::setpeerGrpFlag (const bool &peerGrpFlag)
{
    m_peerGrpFlag = peerGrpFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborPeerGrpManagedObject::getpeerGrpFlag:This function is used to get the value of member m_peerGrpFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborPeerGrpManagedObject::getpeerGrpFlag ()
{
    return (m_peerGrpFlag);
}



}

