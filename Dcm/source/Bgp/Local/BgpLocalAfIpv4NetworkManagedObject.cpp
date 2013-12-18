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
 *   Copyright (C) 2013-2018 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/




#include "Bgp/Local/BgpLocalAfIpv4NetworkManagedObject.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAfIpv4NetworkManagedObject::BgpLocalAfIpv4NetworkManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAfIpv4NetworkManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    m_networkWeight(65536),
    m_networkbackDoorFlag(false),
    m_networkRouteMapName("")
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeIpV4AddressNetworkMask (&m_networkIpv4AddressPrefixMask, "networkIpv4AddressPrefixMask",bgp_network_ipv4_address));
    addPersistableAttribute (new AttributeUI32 (&m_networkWeight, 65536, "networkWeight",bgp_network_weight));
    addPersistableAttribute (new AttributeBool (&m_networkbackDoorFlag, true, "false", "networkbackDoorFlag",bgp_backdoor));
    addPersistableAttribute (new AttributeString (&m_networkRouteMapName, true, "", "networkRouteMapName",bgp_network_route_map));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeIpV4AddressNetworkMask (&m_networkIpv4AddressPrefixMask, "networkIpv4AddressPrefixMask",bgp_network_ipv4_address));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_networkWeight, 65536, "networkWeight",bgp_network_weight));
    addPersistableAttributeForCreate (new AttributeBool (&m_networkbackDoorFlag, true, "false", "networkbackDoorFlag",bgp_backdoor));
    addPersistableAttributeForCreate (new AttributeString (&m_networkRouteMapName, true, "", "networkRouteMapName",bgp_network_route_map));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("networkIpv4AddressPrefixMask");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
BgpLocalAfIpv4NetworkManagedObject::BgpLocalAfIpv4NetworkManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const IpV4AddressNetworkMask &networkIpv4AddressPrefixMask,
                                                                                                                const UI32 &networkWeight,
                                                                                                                const bool &networkbackDoorFlag,
                                                                                                                const string &networkRouteMapName)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAfIpv4NetworkManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    m_networkIpv4AddressPrefixMask(networkIpv4AddressPrefixMask),
    m_networkWeight(networkWeight),
    m_networkbackDoorFlag(networkbackDoorFlag),
    m_networkRouteMapName(networkRouteMapName)
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfIpv4NetworkManagedObject::getClassName()
{
    return ("BgpLocalAfIpv4NetworkManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfIpv4NetworkManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAfIpv4NetworkManagedObject::~BgpLocalAfIpv4NetworkManagedObject()
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
 * Brief:BgpLocalAfIpv4NetworkManagedObject::setnetworkIpv4AddressPrefixMask:This function is used to set the value of member m_networkIpv4AddressPrefixMask
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::setnetworkIpv4AddressPrefixMask (const IpV4AddressNetworkMask &networkIpv4AddressPrefixMask)
{
    m_networkIpv4AddressPrefixMask = networkIpv4AddressPrefixMask;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::getnetworkIpv4AddressPrefixMask:This function is used to get the value of member m_networkIpv4AddressPrefixMask
 * @uBreezeClose
 */
IpV4AddressNetworkMask BgpLocalAfIpv4NetworkManagedObject::getnetworkIpv4AddressPrefixMask ()
{
    return (m_networkIpv4AddressPrefixMask);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::setnetworkWeight:This function is used to set the value of member m_networkWeight
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::setnetworkWeight (const UI32 &networkWeight)
{
    m_networkWeight = networkWeight;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::getnetworkWeight:This function is used to get the value of member m_networkWeight
 * @uBreezeClose
 */
UI32 BgpLocalAfIpv4NetworkManagedObject::getnetworkWeight ()
{
    return (m_networkWeight);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::setnetworkbackDoorFlag:This function is used to set the value of member m_networkbackDoorFlag
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::setnetworkbackDoorFlag (const bool &networkbackDoorFlag)
{
    m_networkbackDoorFlag = networkbackDoorFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::getnetworkbackDoorFlag:This function is used to get the value of member m_networkbackDoorFlag
 * @uBreezeClose
 */
bool BgpLocalAfIpv4NetworkManagedObject::getnetworkbackDoorFlag ()
{
    return (m_networkbackDoorFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::setnetworkRouteMapName:This function is used to set the value of member m_networkRouteMapName
 * @uBreezeClose
 */
void BgpLocalAfIpv4NetworkManagedObject::setnetworkRouteMapName (const string &networkRouteMapName)
{
    m_networkRouteMapName = networkRouteMapName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4NetworkManagedObject::getnetworkRouteMapName:This function is used to get the value of member m_networkRouteMapName
 * @uBreezeClose
 */
string BgpLocalAfIpv4NetworkManagedObject::getnetworkRouteMapName ()
{
    return (m_networkRouteMapName);
}



}

