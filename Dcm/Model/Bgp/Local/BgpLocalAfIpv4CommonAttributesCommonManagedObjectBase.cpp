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




#include "Bgp/Local/BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject)
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
 * Brief:BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    addPersistableAttributeForCommonBase (new AttributeManagedObjectVectorComposition <BgpLocalAggregateIpv4AddressManagedObject> (&m_aggregateAddress, "aggregateAddress", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalAggregateIpv4AddressManagedObject::getClassName(),bgp_aggregate_address));
    addPersistableAttributeForCommonBase (new AttributeManagedObjectVectorComposition <BgpLocalAfIpv4NetworkManagedObject> (&m_network, "network", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalAfIpv4NetworkManagedObject::getClassName(),bgp_network));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase:: setupAttributesForCreate ()
{
    addPersistableAttributeForCommonBaseCreate (new AttributeManagedObjectVectorComposition <BgpLocalAggregateIpv4AddressManagedObject> (&m_aggregateAddress, "aggregateAddress", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalAggregateIpv4AddressManagedObject::getClassName(),bgp_aggregate_address));
    addPersistableAttributeForCommonBaseCreate (new AttributeManagedObjectVectorComposition <BgpLocalAfIpv4NetworkManagedObject> (&m_network, "network", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalAfIpv4NetworkManagedObject::getClassName(),bgp_network));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::~BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase()
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
 * Brief:BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::setaggregateAddress:This function is used to set the value of member m_aggregateAddress
 * @uBreezeClose
 */
void BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::setaggregateAddress (const vector<WaveManagedObjectPointer<BgpLocalAggregateIpv4AddressManagedObject> > &aggregateAddress)
{
    m_aggregateAddress = aggregateAddress;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::getaggregateAddress:This function is used to get the value of member m_aggregateAddress
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<BgpLocalAggregateIpv4AddressManagedObject> > BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::getaggregateAddress ()
{
    return (m_aggregateAddress);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::setnetwork:This function is used to set the value of member m_network
 * @uBreezeClose
 */
void BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::setnetwork (const vector<WaveManagedObjectPointer<BgpLocalAfIpv4NetworkManagedObject> > &network)
{
    m_network = network;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::getnetwork:This function is used to get the value of member m_network
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<BgpLocalAfIpv4NetworkManagedObject> > BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::getnetwork ()
{
    return (m_network);
}



}

