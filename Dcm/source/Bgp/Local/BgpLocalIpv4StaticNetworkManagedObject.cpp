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




#include "Bgp/Local/BgpLocalIpv4StaticNetworkManagedObject.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalIpv4StaticNetworkManagedObject::BgpLocalIpv4StaticNetworkManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalIpv4StaticNetworkManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    m_staticNetworkDistance(0)
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
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeIpV4AddressNetworkMask (&m_staticNetworkAddressPrefix, "staticNetworkAddressPrefix",bgp_static_network_address));
    addPersistableAttribute (new AttributeUI32 (&m_staticNetworkDistance, 0, "staticNetworkDistance",bgp_static_network_distance));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeIpV4AddressNetworkMask (&m_staticNetworkAddressPrefix, "staticNetworkAddressPrefix",bgp_static_network_address));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_staticNetworkDistance, 0, "staticNetworkDistance",bgp_static_network_distance));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("staticNetworkAddressPrefix");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
BgpLocalIpv4StaticNetworkManagedObject::BgpLocalIpv4StaticNetworkManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const IpV4AddressNetworkMask &staticNetworkAddressPrefix,
                                                                                                                const UI32 &staticNetworkDistance)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalIpv4StaticNetworkManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    m_staticNetworkAddressPrefix(staticNetworkAddressPrefix),
    m_staticNetworkDistance(staticNetworkDistance)
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
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalIpv4StaticNetworkManagedObject::getClassName()
{
    return ("BgpLocalIpv4StaticNetworkManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalIpv4StaticNetworkManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalIpv4StaticNetworkManagedObject::~BgpLocalIpv4StaticNetworkManagedObject()
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
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::setstaticNetworkAddressPrefix:This function is used to set the value of member m_staticNetworkAddressPrefix
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::setstaticNetworkAddressPrefix (const IpV4AddressNetworkMask &staticNetworkAddressPrefix)
{
    m_staticNetworkAddressPrefix = staticNetworkAddressPrefix;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::getstaticNetworkAddressPrefix:This function is used to get the value of member m_staticNetworkAddressPrefix
 * @uBreezeClose
 */
IpV4AddressNetworkMask BgpLocalIpv4StaticNetworkManagedObject::getstaticNetworkAddressPrefix ()
{
    return (m_staticNetworkAddressPrefix);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::setstaticNetworkDistance:This function is used to set the value of member m_staticNetworkDistance
 * @uBreezeClose
 */
void BgpLocalIpv4StaticNetworkManagedObject::setstaticNetworkDistance (const UI32 &staticNetworkDistance)
{
    m_staticNetworkDistance = staticNetworkDistance;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalIpv4StaticNetworkManagedObject::getstaticNetworkDistance:This function is used to get the value of member m_staticNetworkDistance
 * @uBreezeClose
 */
UI32 BgpLocalIpv4StaticNetworkManagedObject::getstaticNetworkDistance ()
{
    return (m_staticNetworkDistance);
}



}

