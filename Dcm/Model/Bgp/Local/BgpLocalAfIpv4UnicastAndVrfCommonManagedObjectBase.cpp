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




#include "Bgp/Local/BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalRedistributeCommonManagedObjectBase(pPrismPersistableObject),
    m_bgpRedistributeInternalFlag(false)
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
 * Brief:BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    BgpLocalRedistributeCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttributeForCommonBase (new AttributeBool (&m_bgpRedistributeInternalFlag, true, "false", "bgpRedistributeInternalFlag",bgp_bgp_redistribute_internal));
    addPersistableAttributeForCommonBase (new AttributeManagedObjectVectorComposition <BgpLocalIpv4StaticNetworkManagedObject> (&m_staticNetwork, "staticNetwork", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalIpv4StaticNetworkManagedObject::getClassName(),bgp_static_network));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase:: setupAttributesForCreate ()
{
    BgpLocalRedistributeCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_bgpRedistributeInternalFlag, true, "false", "bgpRedistributeInternalFlag",bgp_bgp_redistribute_internal));
    addPersistableAttributeForCommonBaseCreate (new AttributeManagedObjectVectorComposition <BgpLocalIpv4StaticNetworkManagedObject> (&m_staticNetwork, "staticNetwork", m_pPrismPersistableObject->getObjectClassName(),m_pPrismPersistableObject->getObjectId(),BgpLocalIpv4StaticNetworkManagedObject::getClassName(),bgp_static_network));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::~BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase()
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
 * Brief:BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::setbgpRedistributeInternalFlag:This function is used to set the value of member m_bgpRedistributeInternalFlag
 * @uBreezeClose
 */
void BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::setbgpRedistributeInternalFlag (const bool &bgpRedistributeInternalFlag)
{
    m_bgpRedistributeInternalFlag = bgpRedistributeInternalFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::getbgpRedistributeInternalFlag:This function is used to get the value of member m_bgpRedistributeInternalFlag
 * @uBreezeClose
 */
bool BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::getbgpRedistributeInternalFlag ()
{
    return (m_bgpRedistributeInternalFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::setstaticNetwork:This function is used to set the value of member m_staticNetwork
 * @uBreezeClose
 */
void BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::setstaticNetwork (const vector<WaveManagedObjectPointer<BgpLocalIpv4StaticNetworkManagedObject> > &staticNetwork)
{
    m_staticNetwork = staticNetwork;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::getstaticNetwork:This function is used to get the value of member m_staticNetwork
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<BgpLocalIpv4StaticNetworkManagedObject> > BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::getstaticNetwork ()
{
    return (m_staticNetwork);
}



}

