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




#include "Bgp/Local/BgpLocalAfCommonAttributesCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfCommonAttributesCommonManagedObjectBase::BgpLocalAfCommonAttributesCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalMaxiumPathCommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalMultiPathCommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalAfBasicAttributesCommonManagedObjectBase(pPrismPersistableObject),
    m_clientToClientReflectionFlag(true),
    m_choiceDampening(0),
    m_dampeningFlag(false),
    m_dampeningRouteMapName(""),
    m_defaultMetric(0),
    m_nextHopEnableDefault(false),
    m_tableMapRouteMapName(""),
    m_updateTime(31)
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
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    BgpLocalAfBasicAttributesCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttributeForCommonBase (new AttributeBool (&m_clientToClientReflectionFlag, true, "true", "clientToClientReflectionFlag",bgp_client_to_client_reflection));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_choiceDampening, "choiceDampening",bgp_ch_dampening_source));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_dampeningFlag, true, "false", "dampeningFlag",bgp_dampening_flag));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_dampeningHalfTime, "dampeningHalfTime",bgp_half_time));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_dampeningReuseValue, "dampeningReuseValue",bgp_reuse_value));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_dampeningStartSuppressTime, "dampeningStartSuppressTime",bgp_start_suppress_time));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_dampeningMaxSuppressTime, "dampeningMaxSuppressTime",bgp_max_suppress_time));
    addPersistableAttributeForCommonBase (new AttributeString (&m_dampeningRouteMapName, true, "", "dampeningRouteMapName",bgp_dampening_route_map));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_defaultMetric, 0, "defaultMetric",bgp_default_metric));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_nextHopEnableDefault, true, "false", "nextHopEnableDefault",bgp_next_hop_enable_default));
    addPersistableAttributeForCommonBase (new AttributeString (&m_tableMapRouteMapName, true, "", "tableMapRouteMapName",bgp_table_map_route_map));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_updateTime, 31, "updateTime",bgp_update_time));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase:: setupAttributesForCreate ()
{
    BgpLocalAfBasicAttributesCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_clientToClientReflectionFlag, true, "true", "clientToClientReflectionFlag",bgp_client_to_client_reflection));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_choiceDampening, "choiceDampening",bgp_ch_dampening_source));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_dampeningFlag, true, "false", "dampeningFlag",bgp_dampening_flag));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_dampeningHalfTime, "dampeningHalfTime",bgp_half_time));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_dampeningReuseValue, "dampeningReuseValue",bgp_reuse_value));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_dampeningStartSuppressTime, "dampeningStartSuppressTime",bgp_start_suppress_time));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_dampeningMaxSuppressTime, "dampeningMaxSuppressTime",bgp_max_suppress_time));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_dampeningRouteMapName, true, "", "dampeningRouteMapName",bgp_dampening_route_map));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_defaultMetric, 0, "defaultMetric",bgp_default_metric));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_nextHopEnableDefault, true, "false", "nextHopEnableDefault",bgp_next_hop_enable_default));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_tableMapRouteMapName, true, "", "tableMapRouteMapName",bgp_table_map_route_map));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_updateTime, 31, "updateTime",bgp_update_time));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfCommonAttributesCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalAfCommonAttributesCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfCommonAttributesCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfCommonAttributesCommonManagedObjectBase::~BgpLocalAfCommonAttributesCommonManagedObjectBase()
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
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setclientToClientReflectionFlag:This function is used to set the value of member m_clientToClientReflectionFlag
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setclientToClientReflectionFlag (const bool &clientToClientReflectionFlag)
{
    m_clientToClientReflectionFlag = clientToClientReflectionFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getclientToClientReflectionFlag:This function is used to get the value of member m_clientToClientReflectionFlag
 * @uBreezeClose
 */
bool BgpLocalAfCommonAttributesCommonManagedObjectBase::getclientToClientReflectionFlag ()
{
    return (m_clientToClientReflectionFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setchoiceDampening:This function is used to set the value of member m_choiceDampening
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setchoiceDampening (const UI32 &choiceDampening)
{
    m_choiceDampening = choiceDampening;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getchoiceDampening:This function is used to get the value of member m_choiceDampening
 * @uBreezeClose
 */
UI32 BgpLocalAfCommonAttributesCommonManagedObjectBase::getchoiceDampening ()
{
    return (m_choiceDampening);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningFlag:This function is used to set the value of member m_dampeningFlag
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningFlag (const bool &dampeningFlag)
{
    m_dampeningFlag = dampeningFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningFlag:This function is used to get the value of member m_dampeningFlag
 * @uBreezeClose
 */
bool BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningFlag ()
{
    return (m_dampeningFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningHalfTime:This function is used to set the value of member m_dampeningHalfTime
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningHalfTime (const UI32 &dampeningHalfTime)
{
    m_dampeningHalfTime = dampeningHalfTime;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningHalfTime:This function is used to get the value of member m_dampeningHalfTime
 * @uBreezeClose
 */
UI32 BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningHalfTime ()
{
    return (m_dampeningHalfTime);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningReuseValue:This function is used to set the value of member m_dampeningReuseValue
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningReuseValue (const UI32 &dampeningReuseValue)
{
    m_dampeningReuseValue = dampeningReuseValue;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningReuseValue:This function is used to get the value of member m_dampeningReuseValue
 * @uBreezeClose
 */
UI32 BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningReuseValue ()
{
    return (m_dampeningReuseValue);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningStartSuppressTime:This function is used to set the value of member m_dampeningStartSuppressTime
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningStartSuppressTime (const UI32 &dampeningStartSuppressTime)
{
    m_dampeningStartSuppressTime = dampeningStartSuppressTime;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningStartSuppressTime:This function is used to get the value of member m_dampeningStartSuppressTime
 * @uBreezeClose
 */
UI32 BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningStartSuppressTime ()
{
    return (m_dampeningStartSuppressTime);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningMaxSuppressTime:This function is used to set the value of member m_dampeningMaxSuppressTime
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningMaxSuppressTime (const UI32 &dampeningMaxSuppressTime)
{
    m_dampeningMaxSuppressTime = dampeningMaxSuppressTime;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningMaxSuppressTime:This function is used to get the value of member m_dampeningMaxSuppressTime
 * @uBreezeClose
 */
UI32 BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningMaxSuppressTime ()
{
    return (m_dampeningMaxSuppressTime);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningRouteMapName:This function is used to set the value of member m_dampeningRouteMapName
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setdampeningRouteMapName (const string &dampeningRouteMapName)
{
    m_dampeningRouteMapName = dampeningRouteMapName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningRouteMapName:This function is used to get the value of member m_dampeningRouteMapName
 * @uBreezeClose
 */
string BgpLocalAfCommonAttributesCommonManagedObjectBase::getdampeningRouteMapName ()
{
    return (m_dampeningRouteMapName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setdefaultMetric:This function is used to set the value of member m_defaultMetric
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setdefaultMetric (const UI32 &defaultMetric)
{
    m_defaultMetric = defaultMetric;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getdefaultMetric:This function is used to get the value of member m_defaultMetric
 * @uBreezeClose
 */
UI32 BgpLocalAfCommonAttributesCommonManagedObjectBase::getdefaultMetric ()
{
    return (m_defaultMetric);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setnextHopEnableDefault:This function is used to set the value of member m_nextHopEnableDefault
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setnextHopEnableDefault (const bool &nextHopEnableDefault)
{
    m_nextHopEnableDefault = nextHopEnableDefault;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getnextHopEnableDefault:This function is used to get the value of member m_nextHopEnableDefault
 * @uBreezeClose
 */
bool BgpLocalAfCommonAttributesCommonManagedObjectBase::getnextHopEnableDefault ()
{
    return (m_nextHopEnableDefault);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::settableMapRouteMapName:This function is used to set the value of member m_tableMapRouteMapName
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::settableMapRouteMapName (const string &tableMapRouteMapName)
{
    m_tableMapRouteMapName = tableMapRouteMapName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::gettableMapRouteMapName:This function is used to get the value of member m_tableMapRouteMapName
 * @uBreezeClose
 */
string BgpLocalAfCommonAttributesCommonManagedObjectBase::gettableMapRouteMapName ()
{
    return (m_tableMapRouteMapName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::setupdateTime:This function is used to set the value of member m_updateTime
 * @uBreezeClose
 */
void BgpLocalAfCommonAttributesCommonManagedObjectBase::setupdateTime (const UI32 &updateTime)
{
    m_updateTime = updateTime;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfCommonAttributesCommonManagedObjectBase::getupdateTime:This function is used to get the value of member m_updateTime
 * @uBreezeClose
 */
UI32 BgpLocalAfCommonAttributesCommonManagedObjectBase::getupdateTime ()
{
    return (m_updateTime);
}



}

