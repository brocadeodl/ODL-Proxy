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




#include "Bgp/Local/BgpLocalAfIpv4NeighborIpAddrManagedObject.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"
#include "Bgp/Local/BgpLocalNeighborIpxAddrManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAfIpv4NeighborIpAddrManagedObject::BgpLocalAfIpv4NeighborIpAddrManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
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
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    BgpLocalAfNeighborAttributesCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_afIpv4NeighborIpAddress, "afIpv4NeighborIpAddress", BgpLocalNeighborIpxAddrManagedObject::getClassName(), true,bgp_af_ipv4_neighbor_address));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    BgpLocalAfNeighborAttributesCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_afIpv4NeighborIpAddress, "afIpv4NeighborIpAddress", BgpLocalNeighborIpxAddrManagedObject::getClassName(), true,bgp_af_ipv4_neighbor_address));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("afIpv4NeighborIpAddress");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
BgpLocalAfIpv4NeighborIpAddrManagedObject::BgpLocalAfIpv4NeighborIpAddrManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const ObjectId &afIpv4NeighborIpAddress)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalAfNeighborAttributesCommonManagedObjectBase(this),
    m_afIpv4NeighborIpAddress(afIpv4NeighborIpAddress)
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
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfIpv4NeighborIpAddrManagedObject::getClassName()
{
    return ("BgpLocalAfIpv4NeighborIpAddrManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfIpv4NeighborIpAddrManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAfIpv4NeighborIpAddrManagedObject::~BgpLocalAfIpv4NeighborIpAddrManagedObject()
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
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::setafIpv4NeighborIpAddress:This function is used to set the value of member m_afIpv4NeighborIpAddress
 * @uBreezeClose
 */
void BgpLocalAfIpv4NeighborIpAddrManagedObject::setafIpv4NeighborIpAddress (const ObjectId &afIpv4NeighborIpAddress)
{
    m_afIpv4NeighborIpAddress = afIpv4NeighborIpAddress;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NeighborIpAddrManagedObject::getafIpv4NeighborIpAddress:This function is used to get the value of member m_afIpv4NeighborIpAddress
 * @uBreezeClose
 */
ObjectId BgpLocalAfIpv4NeighborIpAddrManagedObject::getafIpv4NeighborIpAddress ()
{
    return (m_afIpv4NeighborIpAddress);
}



}

