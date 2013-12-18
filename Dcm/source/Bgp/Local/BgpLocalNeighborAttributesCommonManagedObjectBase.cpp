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




#include "Bgp/Local/BgpLocalNeighborAttributesCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalNeighborAttributesCommonManagedObjectBase::BgpLocalNeighborAttributesCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    m_remoteAsNum(""),
    m_description(""),
    m_shutdownFlag(false),
    m_shutdownGenerateRibOutFlag(false),
    m_localAsNum(""),
    m_localAsNoPrependFlag(false),
    m_choiceNextHopSelfType(0),
    m_nextHopSelfStatusFlag(false),
    m_nextHopSelfAlwaysFlag(false),
    m_advertisementInterval(0),
    m_choiceEbgpMultiHopType(0),
    m_ebgpMultiHopFlag(false),
    m_ebgpMultiHopCount(0),
    m_choiceMaxasLimit(0),
    m_asPathNum(301),
    m_maxasDisableFlag(false),
    m_bgpPassword(""),
    m_enforceFirstAsFlag(false),
    m_enforceFirstAsDisableFlag(false),
    m_as4EnableFlag(false),
    m_as4DisableFlag(false),
    m_NeikeepAliveTime(65536),
    m_NeiHoldTime(65536),
    m_choiceUpdateSource(0),
    m_ethInterfaceType(0),
    m_ethInterface(""),
    m_loopInterface(""),
    m_virtualInterface(0),
    m_removePrivateAsFlag(false),
    m_asOverRideFlag(false),
    m_softReconfigurationInboundFlag(false)
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
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    addPersistableAttributeForCommonBase (new AttributeString (&m_remoteAsNum, true, "", "remoteAsNum",bgp_remote_as));
    addPersistableAttributeForCommonBase (new AttributeString (&m_description, true, "", "description",bgp_description));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_shutdownFlag, true, "false", "shutdownFlag",bgp_shutdown_status));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_shutdownGenerateRibOutFlag, true, "false", "shutdownGenerateRibOutFlag",bgp_generate_rib_out));
    addPersistableAttributeForCommonBase (new AttributeString (&m_localAsNum, true, "", "localAsNum",bgp_local_as_value));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_localAsNoPrependFlag, true, "false", "localAsNoPrependFlag",bgp_no_prepend));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_choiceNextHopSelfType, "choiceNextHopSelfType",bgp_ch_next_hop_self_type));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_nextHopSelfStatusFlag, true, "false", "nextHopSelfStatusFlag",bgp_next_hop_self_status));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_nextHopSelfAlwaysFlag, true, "false", "nextHopSelfAlwaysFlag",bgp_next_hop_self_always));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_advertisementInterval, 0, "advertisementInterval",bgp_value));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_choiceEbgpMultiHopType, "choiceEbgpMultiHopType",bgp_ch_ebgp_multihop_type));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_ebgpMultiHopFlag, true, "false", "ebgpMultiHopFlag",bgp_ebgp_multihop_flag));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_ebgpMultiHopCount, 0, "ebgpMultiHopCount",bgp_ebgp_multihop_count));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_choiceMaxasLimit, "choiceMaxasLimit",bgp_ch_maxas_limit));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_asPathNum, 301, "asPathNum",bgp_num_as_in_as_path));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_maxasDisableFlag, true, "false", "maxasDisableFlag",bgp_maxas_limit_disable));
    addPersistableAttributeForCommonBase (new AttributeString (&m_bgpPassword, true, "", "bgpPassword",bgp_password));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_enforceFirstAsFlag, true, "false", "enforceFirstAsFlag",bgp_nei_enforce_first_as));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_enforceFirstAsDisableFlag, true, "false", "enforceFirstAsDisableFlag",bgp_nei_enforce_first_as_disable));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_as4EnableFlag, true, "false", "as4EnableFlag",bgp_neighbor_as4_enable));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_as4DisableFlag, true, "false", "as4DisableFlag",bgp_neighbor_as4_disable));
    addPersistableAttributeForCommonBase (new AttributeUI32UC (&m_NeikeepAliveTime, false, true, 65536, "NeikeepAliveTime",bgp_nei_keep_alive));
    addPersistableAttributeForCommonBase (new AttributeUI32UC (&m_NeiHoldTime, false, true, 65536, "NeiHoldTime",bgp_nei_hold_time));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_choiceUpdateSource, "choiceUpdateSource",bgp_ch_update_source));
    addPersistableAttributeForCommonBase (new AttributeIpV4Address (&m_sipIpv4Address, "sipIpv4Address",bgp_sip_ipv4_address));
    addPersistableAttributeForCommonBase (new AttributeEnum (&m_ethInterfaceType, 0, "ethInterfaceType",bgp_interface_type));
    addPersistableAttributeForCommonBase (new AttributeString (&m_ethInterface, true, "", "ethInterface",bgp_ethernet));
    addPersistableAttributeForCommonBase (new AttributeString (&m_loopInterface, true, "", "loopInterface",bgp_loopback));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_virtualInterface, "virtualInterface",bgp_ve_interface));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_removePrivateAsFlag, true, "false", "removePrivateAsFlag",bgp_remove_private_as));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_asOverRideFlag, true, "false", "asOverRideFlag",bgp_as_override));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_softReconfigurationInboundFlag, true, "false", "softReconfigurationInboundFlag",bgp_inbound));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase:: setupAttributesForCreate ()
{
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_remoteAsNum, true, "", "remoteAsNum",bgp_remote_as));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_description, true, "", "description",bgp_description));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_shutdownFlag, true, "false", "shutdownFlag",bgp_shutdown_status));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_shutdownGenerateRibOutFlag, true, "false", "shutdownGenerateRibOutFlag",bgp_generate_rib_out));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_localAsNum, true, "", "localAsNum",bgp_local_as_value));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_localAsNoPrependFlag, true, "false", "localAsNoPrependFlag",bgp_no_prepend));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_choiceNextHopSelfType, "choiceNextHopSelfType",bgp_ch_next_hop_self_type));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_nextHopSelfStatusFlag, true, "false", "nextHopSelfStatusFlag",bgp_next_hop_self_status));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_nextHopSelfAlwaysFlag, true, "false", "nextHopSelfAlwaysFlag",bgp_next_hop_self_always));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_advertisementInterval, 0, "advertisementInterval",bgp_value));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_choiceEbgpMultiHopType, "choiceEbgpMultiHopType",bgp_ch_ebgp_multihop_type));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_ebgpMultiHopFlag, true, "false", "ebgpMultiHopFlag",bgp_ebgp_multihop_flag));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_ebgpMultiHopCount, 0, "ebgpMultiHopCount",bgp_ebgp_multihop_count));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_choiceMaxasLimit, "choiceMaxasLimit",bgp_ch_maxas_limit));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_asPathNum, 301, "asPathNum",bgp_num_as_in_as_path));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_maxasDisableFlag, true, "false", "maxasDisableFlag",bgp_maxas_limit_disable));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_bgpPassword, true, "", "bgpPassword",bgp_password));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_enforceFirstAsFlag, true, "false", "enforceFirstAsFlag",bgp_nei_enforce_first_as));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_enforceFirstAsDisableFlag, true, "false", "enforceFirstAsDisableFlag",bgp_nei_enforce_first_as_disable));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_as4EnableFlag, true, "false", "as4EnableFlag",bgp_neighbor_as4_enable));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_as4DisableFlag, true, "false", "as4DisableFlag",bgp_neighbor_as4_disable));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32UC (&m_NeikeepAliveTime, false, true, 65536, "NeikeepAliveTime",bgp_nei_keep_alive));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32UC (&m_NeiHoldTime, false, true, 65536, "NeiHoldTime",bgp_nei_hold_time));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_choiceUpdateSource, "choiceUpdateSource",bgp_ch_update_source));
    addPersistableAttributeForCommonBaseCreate (new AttributeIpV4Address (&m_sipIpv4Address, "sipIpv4Address",bgp_sip_ipv4_address));
    addPersistableAttributeForCommonBaseCreate (new AttributeEnum (&m_ethInterfaceType, 0, "ethInterfaceType",bgp_interface_type));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_ethInterface, true, "", "ethInterface",bgp_ethernet));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_loopInterface, true, "", "loopInterface",bgp_loopback));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_virtualInterface, "virtualInterface",bgp_ve_interface));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_removePrivateAsFlag, true, "false", "removePrivateAsFlag",bgp_remove_private_as));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_asOverRideFlag, true, "false", "asOverRideFlag",bgp_as_override));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_softReconfigurationInboundFlag, true, "false", "softReconfigurationInboundFlag",bgp_inbound));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalNeighborAttributesCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalNeighborAttributesCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalNeighborAttributesCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalNeighborAttributesCommonManagedObjectBase::~BgpLocalNeighborAttributesCommonManagedObjectBase()
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
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setremoteAsNum:This function is used to set the value of member m_remoteAsNum
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setremoteAsNum (const string &remoteAsNum)
{
    m_remoteAsNum = remoteAsNum;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getremoteAsNum:This function is used to get the value of member m_remoteAsNum
 * @uBreezeClose
 */
string BgpLocalNeighborAttributesCommonManagedObjectBase::getremoteAsNum ()
{
    return (m_remoteAsNum);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setdescription:This function is used to set the value of member m_description
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setdescription (const string &description)
{
    m_description = description;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getdescription:This function is used to get the value of member m_description
 * @uBreezeClose
 */
string BgpLocalNeighborAttributesCommonManagedObjectBase::getdescription ()
{
    return (m_description);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setshutdownFlag:This function is used to set the value of member m_shutdownFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setshutdownFlag (const bool &shutdownFlag)
{
    m_shutdownFlag = shutdownFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getshutdownFlag:This function is used to get the value of member m_shutdownFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getshutdownFlag ()
{
    return (m_shutdownFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setshutdownGenerateRibOutFlag:This function is used to set the value of member m_shutdownGenerateRibOutFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setshutdownGenerateRibOutFlag (const bool &shutdownGenerateRibOutFlag)
{
    m_shutdownGenerateRibOutFlag = shutdownGenerateRibOutFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getshutdownGenerateRibOutFlag:This function is used to get the value of member m_shutdownGenerateRibOutFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getshutdownGenerateRibOutFlag ()
{
    return (m_shutdownGenerateRibOutFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setlocalAsNum:This function is used to set the value of member m_localAsNum
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setlocalAsNum (const string &localAsNum)
{
    m_localAsNum = localAsNum;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getlocalAsNum:This function is used to get the value of member m_localAsNum
 * @uBreezeClose
 */
string BgpLocalNeighborAttributesCommonManagedObjectBase::getlocalAsNum ()
{
    return (m_localAsNum);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setlocalAsNoPrependFlag:This function is used to set the value of member m_localAsNoPrependFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setlocalAsNoPrependFlag (const bool &localAsNoPrependFlag)
{
    m_localAsNoPrependFlag = localAsNoPrependFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getlocalAsNoPrependFlag:This function is used to get the value of member m_localAsNoPrependFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getlocalAsNoPrependFlag ()
{
    return (m_localAsNoPrependFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setchoiceNextHopSelfType:This function is used to set the value of member m_choiceNextHopSelfType
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setchoiceNextHopSelfType (const UI32 &choiceNextHopSelfType)
{
    m_choiceNextHopSelfType = choiceNextHopSelfType;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getchoiceNextHopSelfType:This function is used to get the value of member m_choiceNextHopSelfType
 * @uBreezeClose
 */
UI32 BgpLocalNeighborAttributesCommonManagedObjectBase::getchoiceNextHopSelfType ()
{
    return (m_choiceNextHopSelfType);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setnextHopSelfStatusFlag:This function is used to set the value of member m_nextHopSelfStatusFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setnextHopSelfStatusFlag (const bool &nextHopSelfStatusFlag)
{
    m_nextHopSelfStatusFlag = nextHopSelfStatusFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getnextHopSelfStatusFlag:This function is used to get the value of member m_nextHopSelfStatusFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getnextHopSelfStatusFlag ()
{
    return (m_nextHopSelfStatusFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setnextHopSelfAlwaysFlag:This function is used to set the value of member m_nextHopSelfAlwaysFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setnextHopSelfAlwaysFlag (const bool &nextHopSelfAlwaysFlag)
{
    m_nextHopSelfAlwaysFlag = nextHopSelfAlwaysFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getnextHopSelfAlwaysFlag:This function is used to get the value of member m_nextHopSelfAlwaysFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getnextHopSelfAlwaysFlag ()
{
    return (m_nextHopSelfAlwaysFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setadvertisementInterval:This function is used to set the value of member m_advertisementInterval
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setadvertisementInterval (const UI32 &advertisementInterval)
{
    m_advertisementInterval = advertisementInterval;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getadvertisementInterval:This function is used to get the value of member m_advertisementInterval
 * @uBreezeClose
 */
UI32 BgpLocalNeighborAttributesCommonManagedObjectBase::getadvertisementInterval ()
{
    return (m_advertisementInterval);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setchoiceEbgpMultiHopType:This function is used to set the value of member m_choiceEbgpMultiHopType
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setchoiceEbgpMultiHopType (const UI32 &choiceEbgpMultiHopType)
{
    m_choiceEbgpMultiHopType = choiceEbgpMultiHopType;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getchoiceEbgpMultiHopType:This function is used to get the value of member m_choiceEbgpMultiHopType
 * @uBreezeClose
 */
UI32 BgpLocalNeighborAttributesCommonManagedObjectBase::getchoiceEbgpMultiHopType ()
{
    return (m_choiceEbgpMultiHopType);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setebgpMultiHopFlag:This function is used to set the value of member m_ebgpMultiHopFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setebgpMultiHopFlag (const bool &ebgpMultiHopFlag)
{
    m_ebgpMultiHopFlag = ebgpMultiHopFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getebgpMultiHopFlag:This function is used to get the value of member m_ebgpMultiHopFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getebgpMultiHopFlag ()
{
    return (m_ebgpMultiHopFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setebgpMultiHopCount:This function is used to set the value of member m_ebgpMultiHopCount
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setebgpMultiHopCount (const UI32 &ebgpMultiHopCount)
{
    m_ebgpMultiHopCount = ebgpMultiHopCount;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getebgpMultiHopCount:This function is used to get the value of member m_ebgpMultiHopCount
 * @uBreezeClose
 */
UI32 BgpLocalNeighborAttributesCommonManagedObjectBase::getebgpMultiHopCount ()
{
    return (m_ebgpMultiHopCount);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setchoiceMaxasLimit:This function is used to set the value of member m_choiceMaxasLimit
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setchoiceMaxasLimit (const UI32 &choiceMaxasLimit)
{
    m_choiceMaxasLimit = choiceMaxasLimit;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getchoiceMaxasLimit:This function is used to get the value of member m_choiceMaxasLimit
 * @uBreezeClose
 */
UI32 BgpLocalNeighborAttributesCommonManagedObjectBase::getchoiceMaxasLimit ()
{
    return (m_choiceMaxasLimit);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setasPathNum:This function is used to set the value of member m_asPathNum
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setasPathNum (const UI32 &asPathNum)
{
    m_asPathNum = asPathNum;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getasPathNum:This function is used to get the value of member m_asPathNum
 * @uBreezeClose
 */
UI32 BgpLocalNeighborAttributesCommonManagedObjectBase::getasPathNum ()
{
    return (m_asPathNum);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setmaxasDisableFlag:This function is used to set the value of member m_maxasDisableFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setmaxasDisableFlag (const bool &maxasDisableFlag)
{
    m_maxasDisableFlag = maxasDisableFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getmaxasDisableFlag:This function is used to get the value of member m_maxasDisableFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getmaxasDisableFlag ()
{
    return (m_maxasDisableFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setbgpPassword:This function is used to set the value of member m_bgpPassword
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setbgpPassword (const string &bgpPassword)
{
    m_bgpPassword = bgpPassword;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getbgpPassword:This function is used to get the value of member m_bgpPassword
 * @uBreezeClose
 */
string BgpLocalNeighborAttributesCommonManagedObjectBase::getbgpPassword ()
{
    return (m_bgpPassword);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setenforceFirstAsFlag:This function is used to set the value of member m_enforceFirstAsFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setenforceFirstAsFlag (const bool &enforceFirstAsFlag)
{
    m_enforceFirstAsFlag = enforceFirstAsFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getenforceFirstAsFlag:This function is used to get the value of member m_enforceFirstAsFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getenforceFirstAsFlag ()
{
    return (m_enforceFirstAsFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setenforceFirstAsDisableFlag:This function is used to set the value of member m_enforceFirstAsDisableFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setenforceFirstAsDisableFlag (const bool &enforceFirstAsDisableFlag)
{
    m_enforceFirstAsDisableFlag = enforceFirstAsDisableFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getenforceFirstAsDisableFlag:This function is used to get the value of member m_enforceFirstAsDisableFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getenforceFirstAsDisableFlag ()
{
    return (m_enforceFirstAsDisableFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setas4EnableFlag:This function is used to set the value of member m_as4EnableFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setas4EnableFlag (const bool &as4EnableFlag)
{
    m_as4EnableFlag = as4EnableFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getas4EnableFlag:This function is used to get the value of member m_as4EnableFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getas4EnableFlag ()
{
    return (m_as4EnableFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setas4DisableFlag:This function is used to set the value of member m_as4DisableFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setas4DisableFlag (const bool &as4DisableFlag)
{
    m_as4DisableFlag = as4DisableFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getas4DisableFlag:This function is used to get the value of member m_as4DisableFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getas4DisableFlag ()
{
    return (m_as4DisableFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setNeikeepAliveTime:This function is used to set the value of member m_NeikeepAliveTime
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setNeikeepAliveTime (const UI32UC &NeikeepAliveTime)
{
    m_NeikeepAliveTime = NeikeepAliveTime;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getNeikeepAliveTime:This function is used to get the value of member m_NeikeepAliveTime
 * @uBreezeClose
 */
UI32UC BgpLocalNeighborAttributesCommonManagedObjectBase::getNeikeepAliveTime ()
{
    return (m_NeikeepAliveTime);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setNeiHoldTime:This function is used to set the value of member m_NeiHoldTime
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setNeiHoldTime (const UI32UC &NeiHoldTime)
{
    m_NeiHoldTime = NeiHoldTime;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getNeiHoldTime:This function is used to get the value of member m_NeiHoldTime
 * @uBreezeClose
 */
UI32UC BgpLocalNeighborAttributesCommonManagedObjectBase::getNeiHoldTime ()
{
    return (m_NeiHoldTime);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setchoiceUpdateSource:This function is used to set the value of member m_choiceUpdateSource
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setchoiceUpdateSource (const UI32 &choiceUpdateSource)
{
    m_choiceUpdateSource = choiceUpdateSource;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getchoiceUpdateSource:This function is used to get the value of member m_choiceUpdateSource
 * @uBreezeClose
 */
UI32 BgpLocalNeighborAttributesCommonManagedObjectBase::getchoiceUpdateSource ()
{
    return (m_choiceUpdateSource);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setsipIpv4Address:This function is used to set the value of member m_sipIpv4Address
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setsipIpv4Address (const IpV4Address &sipIpv4Address)
{
    m_sipIpv4Address = sipIpv4Address;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getsipIpv4Address:This function is used to get the value of member m_sipIpv4Address
 * @uBreezeClose
 */
IpV4Address BgpLocalNeighborAttributesCommonManagedObjectBase::getsipIpv4Address ()
{
    return (m_sipIpv4Address);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setethInterfaceType:This function is used to set the value of member m_ethInterfaceType
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setethInterfaceType (const UI32 &ethInterfaceType)
{
    m_ethInterfaceType = ethInterfaceType;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getethInterfaceType:This function is used to get the value of member m_ethInterfaceType
 * @uBreezeClose
 */
UI32 BgpLocalNeighborAttributesCommonManagedObjectBase::getethInterfaceType ()
{
    return (m_ethInterfaceType);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setethInterface:This function is used to set the value of member m_ethInterface
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setethInterface (const string &ethInterface)
{
    m_ethInterface = ethInterface;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getethInterface:This function is used to get the value of member m_ethInterface
 * @uBreezeClose
 */
string BgpLocalNeighborAttributesCommonManagedObjectBase::getethInterface ()
{
    return (m_ethInterface);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setloopInterface:This function is used to set the value of member m_loopInterface
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setloopInterface (const string &loopInterface)
{
    m_loopInterface = loopInterface;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getloopInterface:This function is used to get the value of member m_loopInterface
 * @uBreezeClose
 */
string BgpLocalNeighborAttributesCommonManagedObjectBase::getloopInterface ()
{
    return (m_loopInterface);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setvirtualInterface:This function is used to set the value of member m_virtualInterface
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setvirtualInterface (const UI32 &virtualInterface)
{
    m_virtualInterface = virtualInterface;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getvirtualInterface:This function is used to get the value of member m_virtualInterface
 * @uBreezeClose
 */
UI32 BgpLocalNeighborAttributesCommonManagedObjectBase::getvirtualInterface ()
{
    return (m_virtualInterface);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setremovePrivateAsFlag:This function is used to set the value of member m_removePrivateAsFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setremovePrivateAsFlag (const bool &removePrivateAsFlag)
{
    m_removePrivateAsFlag = removePrivateAsFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getremovePrivateAsFlag:This function is used to get the value of member m_removePrivateAsFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getremovePrivateAsFlag ()
{
    return (m_removePrivateAsFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setasOverRideFlag:This function is used to set the value of member m_asOverRideFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setasOverRideFlag (const bool &asOverRideFlag)
{
    m_asOverRideFlag = asOverRideFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getasOverRideFlag:This function is used to get the value of member m_asOverRideFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getasOverRideFlag ()
{
    return (m_asOverRideFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::setsoftReconfigurationInboundFlag:This function is used to set the value of member m_softReconfigurationInboundFlag
 * @uBreezeClose
 */
void BgpLocalNeighborAttributesCommonManagedObjectBase::setsoftReconfigurationInboundFlag (const bool &softReconfigurationInboundFlag)
{
    m_softReconfigurationInboundFlag = softReconfigurationInboundFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalNeighborAttributesCommonManagedObjectBase::getsoftReconfigurationInboundFlag:This function is used to get the value of member m_softReconfigurationInboundFlag
 * @uBreezeClose
 */
bool BgpLocalNeighborAttributesCommonManagedObjectBase::getsoftReconfigurationInboundFlag ()
{
    return (m_softReconfigurationInboundFlag);
}



}

