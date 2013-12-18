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




#include "Bgp/Local/BgpLocalRedistributeCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalRedistributeCommonManagedObjectBase::BgpLocalRedistributeCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    m_redistributeConnectedFlag(false),
    m_unicastMetric(0),
    m_redistributeRouteMapName(""),
    m_redistributeOspfFlag(false),
    m_ospfMatchInternal(false),
    m_ospfMatchExternalOne(false),
    m_ospfMatchExternalTwo(false),
    m_ospfMetric(0),
    m_ospfRouteMapName(""),
    m_redistributeStaticFlag(false),
    m_unicastStaticMetric(0),
    m_staticRouteMapName("")
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
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    addPersistableAttributeForCommonBase (new AttributeBool (&m_redistributeConnectedFlag, true, "false", "redistributeConnectedFlag",bgp_redistribute_connected));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_unicastMetric, 0, "unicastMetric",bgp_unicast_metric));
    addPersistableAttributeForCommonBase (new AttributeString (&m_redistributeRouteMapName, true, "", "redistributeRouteMapName",bgp_redistribute_route_map));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_redistributeOspfFlag, true, "false", "redistributeOspfFlag",bgp_redistribute_ospf));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_ospfMatchInternal, true, "false", "ospfMatchInternal",bgp_ospf_internal));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_ospfMatchExternalOne, true, "false", "ospfMatchExternalOne",bgp_ospf_external1));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_ospfMatchExternalTwo, true, "false", "ospfMatchExternalTwo",bgp_ospf_external2));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_ospfMetric, 0, "ospfMetric",bgp_ospf_metric));
    addPersistableAttributeForCommonBase (new AttributeString (&m_ospfRouteMapName, true, "", "ospfRouteMapName",bgp_ospf_route_map));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_redistributeStaticFlag, true, "false", "redistributeStaticFlag",bgp_redistribute_static));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_unicastStaticMetric, 0, "unicastStaticMetric",bgp_unicast_static_metric));
    addPersistableAttributeForCommonBase (new AttributeString (&m_staticRouteMapName, true, "", "staticRouteMapName",bgp_static_route_map));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase:: setupAttributesForCreate ()
{
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_redistributeConnectedFlag, true, "false", "redistributeConnectedFlag",bgp_redistribute_connected));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_unicastMetric, 0, "unicastMetric",bgp_unicast_metric));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_redistributeRouteMapName, true, "", "redistributeRouteMapName",bgp_redistribute_route_map));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_redistributeOspfFlag, true, "false", "redistributeOspfFlag",bgp_redistribute_ospf));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_ospfMatchInternal, true, "false", "ospfMatchInternal",bgp_ospf_internal));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_ospfMatchExternalOne, true, "false", "ospfMatchExternalOne",bgp_ospf_external1));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_ospfMatchExternalTwo, true, "false", "ospfMatchExternalTwo",bgp_ospf_external2));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_ospfMetric, 0, "ospfMetric",bgp_ospf_metric));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_ospfRouteMapName, true, "", "ospfRouteMapName",bgp_ospf_route_map));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_redistributeStaticFlag, true, "false", "redistributeStaticFlag",bgp_redistribute_static));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_unicastStaticMetric, 0, "unicastStaticMetric",bgp_unicast_static_metric));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_staticRouteMapName, true, "", "staticRouteMapName",bgp_static_route_map));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalRedistributeCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalRedistributeCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalRedistributeCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalRedistributeCommonManagedObjectBase::~BgpLocalRedistributeCommonManagedObjectBase()
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
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setredistributeConnectedFlag:This function is used to set the value of member m_redistributeConnectedFlag
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setredistributeConnectedFlag (const bool &redistributeConnectedFlag)
{
    m_redistributeConnectedFlag = redistributeConnectedFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getredistributeConnectedFlag:This function is used to get the value of member m_redistributeConnectedFlag
 * @uBreezeClose
 */
bool BgpLocalRedistributeCommonManagedObjectBase::getredistributeConnectedFlag ()
{
    return (m_redistributeConnectedFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setunicastMetric:This function is used to set the value of member m_unicastMetric
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setunicastMetric (const UI32 &unicastMetric)
{
    m_unicastMetric = unicastMetric;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getunicastMetric:This function is used to get the value of member m_unicastMetric
 * @uBreezeClose
 */
UI32 BgpLocalRedistributeCommonManagedObjectBase::getunicastMetric ()
{
    return (m_unicastMetric);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setredistributeRouteMapName:This function is used to set the value of member m_redistributeRouteMapName
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setredistributeRouteMapName (const string &redistributeRouteMapName)
{
    m_redistributeRouteMapName = redistributeRouteMapName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getredistributeRouteMapName:This function is used to get the value of member m_redistributeRouteMapName
 * @uBreezeClose
 */
string BgpLocalRedistributeCommonManagedObjectBase::getredistributeRouteMapName ()
{
    return (m_redistributeRouteMapName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setredistributeOspfFlag:This function is used to set the value of member m_redistributeOspfFlag
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setredistributeOspfFlag (const bool &redistributeOspfFlag)
{
    m_redistributeOspfFlag = redistributeOspfFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getredistributeOspfFlag:This function is used to get the value of member m_redistributeOspfFlag
 * @uBreezeClose
 */
bool BgpLocalRedistributeCommonManagedObjectBase::getredistributeOspfFlag ()
{
    return (m_redistributeOspfFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setospfMatchInternal:This function is used to set the value of member m_ospfMatchInternal
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setospfMatchInternal (const bool &ospfMatchInternal)
{
    m_ospfMatchInternal = ospfMatchInternal;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getospfMatchInternal:This function is used to get the value of member m_ospfMatchInternal
 * @uBreezeClose
 */
bool BgpLocalRedistributeCommonManagedObjectBase::getospfMatchInternal ()
{
    return (m_ospfMatchInternal);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setospfMatchExternalOne:This function is used to set the value of member m_ospfMatchExternalOne
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setospfMatchExternalOne (const bool &ospfMatchExternalOne)
{
    m_ospfMatchExternalOne = ospfMatchExternalOne;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getospfMatchExternalOne:This function is used to get the value of member m_ospfMatchExternalOne
 * @uBreezeClose
 */
bool BgpLocalRedistributeCommonManagedObjectBase::getospfMatchExternalOne ()
{
    return (m_ospfMatchExternalOne);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setospfMatchExternalTwo:This function is used to set the value of member m_ospfMatchExternalTwo
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setospfMatchExternalTwo (const bool &ospfMatchExternalTwo)
{
    m_ospfMatchExternalTwo = ospfMatchExternalTwo;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getospfMatchExternalTwo:This function is used to get the value of member m_ospfMatchExternalTwo
 * @uBreezeClose
 */
bool BgpLocalRedistributeCommonManagedObjectBase::getospfMatchExternalTwo ()
{
    return (m_ospfMatchExternalTwo);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setospfMetric:This function is used to set the value of member m_ospfMetric
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setospfMetric (const UI32 &ospfMetric)
{
    m_ospfMetric = ospfMetric;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getospfMetric:This function is used to get the value of member m_ospfMetric
 * @uBreezeClose
 */
UI32 BgpLocalRedistributeCommonManagedObjectBase::getospfMetric ()
{
    return (m_ospfMetric);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setospfRouteMapName:This function is used to set the value of member m_ospfRouteMapName
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setospfRouteMapName (const string &ospfRouteMapName)
{
    m_ospfRouteMapName = ospfRouteMapName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getospfRouteMapName:This function is used to get the value of member m_ospfRouteMapName
 * @uBreezeClose
 */
string BgpLocalRedistributeCommonManagedObjectBase::getospfRouteMapName ()
{
    return (m_ospfRouteMapName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setredistributeStaticFlag:This function is used to set the value of member m_redistributeStaticFlag
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setredistributeStaticFlag (const bool &redistributeStaticFlag)
{
    m_redistributeStaticFlag = redistributeStaticFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getredistributeStaticFlag:This function is used to get the value of member m_redistributeStaticFlag
 * @uBreezeClose
 */
bool BgpLocalRedistributeCommonManagedObjectBase::getredistributeStaticFlag ()
{
    return (m_redistributeStaticFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setunicastStaticMetric:This function is used to set the value of member m_unicastStaticMetric
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setunicastStaticMetric (const UI32 &unicastStaticMetric)
{
    m_unicastStaticMetric = unicastStaticMetric;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getunicastStaticMetric:This function is used to get the value of member m_unicastStaticMetric
 * @uBreezeClose
 */
UI32 BgpLocalRedistributeCommonManagedObjectBase::getunicastStaticMetric ()
{
    return (m_unicastStaticMetric);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::setstaticRouteMapName:This function is used to set the value of member m_staticRouteMapName
 * @uBreezeClose
 */
void BgpLocalRedistributeCommonManagedObjectBase::setstaticRouteMapName (const string &staticRouteMapName)
{
    m_staticRouteMapName = staticRouteMapName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalRedistributeCommonManagedObjectBase::getstaticRouteMapName:This function is used to get the value of member m_staticRouteMapName
 * @uBreezeClose
 */
string BgpLocalRedistributeCommonManagedObjectBase::getstaticRouteMapName ()
{
    return (m_staticRouteMapName);
}



}

