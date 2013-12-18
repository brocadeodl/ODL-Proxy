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




#include "Bgp/Local/BgpLocalAggregateIpv4AddressManagedObject.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateIpv4AddressManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAggregateIpv4AddressManagedObject::BgpLocalAggregateIpv4AddressManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAggregateIpv4AddressManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalAggregateAttributeCommonManagedObjectBase(this)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeIpV4AddressNetworkMask (&m_aggregateIpv4PrefixMask, "aggregateIpv4PrefixMask",bgp_aggregate_ip_prefix));
    BgpLocalAggregateAttributeCommonManagedObjectBase::setupAttributesForPersistence();

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeIpV4AddressNetworkMask (&m_aggregateIpv4PrefixMask, "aggregateIpv4PrefixMask",bgp_aggregate_ip_prefix));
    BgpLocalAggregateAttributeCommonManagedObjectBase::setupAttributesForCreate();

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateIpv4AddressManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("aggregateIpv4PrefixMask");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateIpv4AddressManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
BgpLocalAggregateIpv4AddressManagedObject::BgpLocalAggregateIpv4AddressManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const IpV4AddressNetworkMask &aggregateIpv4PrefixMask)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAggregateIpv4AddressManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalAggregateAttributeCommonManagedObjectBase(this),
    m_aggregateIpv4PrefixMask(aggregateIpv4PrefixMask)
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAggregateIpv4AddressManagedObject::getClassName()
{
    return ("BgpLocalAggregateIpv4AddressManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAggregateIpv4AddressManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAggregateIpv4AddressManagedObject::~BgpLocalAggregateIpv4AddressManagedObject()
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
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::setaggregateIpv4PrefixMask:This function is used to set the value of member m_aggregateIpv4PrefixMask
 * @uBreezeClose
 */
void BgpLocalAggregateIpv4AddressManagedObject::setaggregateIpv4PrefixMask (const IpV4AddressNetworkMask &aggregateIpv4PrefixMask)
{
    m_aggregateIpv4PrefixMask = aggregateIpv4PrefixMask;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateIpv4AddressManagedObject::getaggregateIpv4PrefixMask:This function is used to get the value of member m_aggregateIpv4PrefixMask
 * @uBreezeClose
 */
IpV4AddressNetworkMask BgpLocalAggregateIpv4AddressManagedObject::getaggregateIpv4PrefixMask ()
{
    return (m_aggregateIpv4PrefixMask);
}



}

