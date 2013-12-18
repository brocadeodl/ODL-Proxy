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




#include "Bgp/Local/BgpLocalBasicConfigManagedObject.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalBasicConfigManagedObject::BgpLocalBasicConfigManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalBasicConfigManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    m_vrfName(""),
    m_localAsNum(""),
    m_alwaysCompareMedFlag(false),
    m_asPathIgnoreFlag(false),
    m_capabilityAs4Flag(false),
    m_choiceClusterId(0),
    m_clusterId(0),
    m_compareMedEmptyAsPathFlag(false),
    m_compareRouterIdFlag(false),
    m_defaultLocalPreference(0),
    m_extRoutedistance(0),
    m_intRouteDistance(0),
    m_lclRouteDistance(0),
    m_enforceFirstAsFlag(false),
    m_fastExternalFallOverFlag(false),
    m_installIgpCostFlag(false),
    m_logDampeningDebug(false),
    m_maxAsLimit(301),
    m_medMissingAsWorstFlag(false),
    m_routerBgpModeTimersKeepAliveTime(65536),
    m_routerBgpModeTimerHoldTime(65536)
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
 * Brief:BgpLocalBasicConfigManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeString (&m_vrfName, true, "", "vrfName",bgp_vrf_name));
    addPersistableAttribute (new AttributeString (&m_localAsNum, true, "", "localAsNum",bgp_local_as));
    addPersistableAttribute (new AttributeBool (&m_alwaysCompareMedFlag, true, "false", "alwaysCompareMedFlag",bgp_always_compare_med));
    addPersistableAttribute (new AttributeBool (&m_asPathIgnoreFlag, true, "false", "asPathIgnoreFlag",bgp_as_path_ignore));
    addPersistableAttribute (new AttributeBool (&m_capabilityAs4Flag, true, "false", "capabilityAs4Flag",bgp_as4_enable));
    addPersistableAttribute (new AttributeUI32 (&m_choiceClusterId, "choiceClusterId",bgp_ch_cluster_id));
    addPersistableAttribute (new AttributeUI32 (&m_clusterId, 0, "clusterId",bgp_cluster_id_value));
    addPersistableAttribute (new AttributeIpV4Address (&m_clusterIdIpAddress, "clusterIdIpAddress",bgp_cluster_id_ipv4_address));
    addPersistableAttribute (new AttributeBool (&m_compareMedEmptyAsPathFlag, true, "false", "compareMedEmptyAsPathFlag",bgp_compare_med_empty_aspath));
    addPersistableAttribute (new AttributeBool (&m_compareRouterIdFlag, true, "false", "compareRouterIdFlag",bgp_compare_routerid));
    addPersistableAttribute (new AttributeUI32 (&m_defaultLocalPreference, 0, "defaultLocalPreference",bgp_default_local_preference));
    addPersistableAttribute (new AttributeUI32 (&m_extRoutedistance, 0, "extRoutedistance",bgp_ext_route_distance));
    addPersistableAttribute (new AttributeUI32 (&m_intRouteDistance, 0, "intRouteDistance",bgp_int_route_distance));
    addPersistableAttribute (new AttributeUI32 (&m_lclRouteDistance, 0, "lclRouteDistance",bgp_lcl_route_distance));
    addPersistableAttribute (new AttributeBool (&m_enforceFirstAsFlag, true, "false", "enforceFirstAsFlag",bgp_enforce_first_as));
    addPersistableAttribute (new AttributeBool (&m_fastExternalFallOverFlag, true, "false", "fastExternalFallOverFlag",bgp_fast_external_fallover));
    addPersistableAttribute (new AttributeBool (&m_installIgpCostFlag, true, "false", "installIgpCostFlag",bgp_install_igp_cost));
    addPersistableAttribute (new AttributeBool (&m_logDampeningDebug, true, "false", "logDampeningDebug",bgp_log_dampening_debug));
    addPersistableAttribute (new AttributeUI32 (&m_maxAsLimit, 301, "maxAsLimit",bgp_num_as_in_path));
    addPersistableAttribute (new AttributeBool (&m_medMissingAsWorstFlag, true, "false", "medMissingAsWorstFlag",bgp_med_missing_as_worst));
    addPersistableAttribute (new AttributeUI32UC (&m_routerBgpModeTimersKeepAliveTime, false, true, 65536, "routerBgpModeTimersKeepAliveTime",bgp_keep_alive));
    addPersistableAttribute (new AttributeUI32UC (&m_routerBgpModeTimerHoldTime, false, true, 65536, "routerBgpModeTimerHoldTime",bgp_hold_time));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <BgpLocalNeighborPeerGrpManagedObject> (&m_routerBgpNeighborPeerGrp, "routerBgpNeighborPeerGrp", getClassName(),getObjectId(),BgpLocalNeighborPeerGrpManagedObject::getClassName(),bgp_neighbor_peer_grp));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <BgpLocalNeighborIpxAddrManagedObject> (&m_routerBgpNeighborIpxAddr, "routerBgpNeighborIpxAddr", getClassName(),getObjectId(),BgpLocalNeighborIpxAddrManagedObject::getClassName(),bgp_neighbor_addr));
    addPersistableAttribute (new AttributeManagedObjectComposition <BgpLocalAfIpv4UcastManagedObject> (&m_routerBgpAddressIpv4Uc, "routerBgpAddressIpv4Uc",BgpLocalAfIpv4UcastManagedObject::getClassName(), true,bgp_ipv4_unicast));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeString (&m_vrfName, true, "", "vrfName",bgp_vrf_name));
    addPersistableAttributeForCreate (new AttributeString (&m_localAsNum, true, "", "localAsNum",bgp_local_as));
    addPersistableAttributeForCreate (new AttributeBool (&m_alwaysCompareMedFlag, true, "false", "alwaysCompareMedFlag",bgp_always_compare_med));
    addPersistableAttributeForCreate (new AttributeBool (&m_asPathIgnoreFlag, true, "false", "asPathIgnoreFlag",bgp_as_path_ignore));
    addPersistableAttributeForCreate (new AttributeBool (&m_capabilityAs4Flag, true, "false", "capabilityAs4Flag",bgp_as4_enable));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_choiceClusterId, "choiceClusterId",bgp_ch_cluster_id));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_clusterId, 0, "clusterId",bgp_cluster_id_value));
    addPersistableAttributeForCreate (new AttributeIpV4Address (&m_clusterIdIpAddress, "clusterIdIpAddress",bgp_cluster_id_ipv4_address));
    addPersistableAttributeForCreate (new AttributeBool (&m_compareMedEmptyAsPathFlag, true, "false", "compareMedEmptyAsPathFlag",bgp_compare_med_empty_aspath));
    addPersistableAttributeForCreate (new AttributeBool (&m_compareRouterIdFlag, true, "false", "compareRouterIdFlag",bgp_compare_routerid));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_defaultLocalPreference, 0, "defaultLocalPreference",bgp_default_local_preference));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_extRoutedistance, 0, "extRoutedistance",bgp_ext_route_distance));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_intRouteDistance, 0, "intRouteDistance",bgp_int_route_distance));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_lclRouteDistance, 0, "lclRouteDistance",bgp_lcl_route_distance));
    addPersistableAttributeForCreate (new AttributeBool (&m_enforceFirstAsFlag, true, "false", "enforceFirstAsFlag",bgp_enforce_first_as));
    addPersistableAttributeForCreate (new AttributeBool (&m_fastExternalFallOverFlag, true, "false", "fastExternalFallOverFlag",bgp_fast_external_fallover));
    addPersistableAttributeForCreate (new AttributeBool (&m_installIgpCostFlag, true, "false", "installIgpCostFlag",bgp_install_igp_cost));
    addPersistableAttributeForCreate (new AttributeBool (&m_logDampeningDebug, true, "false", "logDampeningDebug",bgp_log_dampening_debug));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_maxAsLimit, 301, "maxAsLimit",bgp_num_as_in_path));
    addPersistableAttributeForCreate (new AttributeBool (&m_medMissingAsWorstFlag, true, "false", "medMissingAsWorstFlag",bgp_med_missing_as_worst));
    addPersistableAttributeForCreate (new AttributeUI32UC (&m_routerBgpModeTimersKeepAliveTime, false, true, 65536, "routerBgpModeTimersKeepAliveTime",bgp_keep_alive));
    addPersistableAttributeForCreate (new AttributeUI32UC (&m_routerBgpModeTimerHoldTime, false, true, 65536, "routerBgpModeTimerHoldTime",bgp_hold_time));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <BgpLocalNeighborPeerGrpManagedObject> (&m_routerBgpNeighborPeerGrp, "routerBgpNeighborPeerGrp", getClassName(),getObjectId(),BgpLocalNeighborPeerGrpManagedObject::getClassName(),bgp_neighbor_peer_grp));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <BgpLocalNeighborIpxAddrManagedObject> (&m_routerBgpNeighborIpxAddr, "routerBgpNeighborIpxAddr", getClassName(),getObjectId(),BgpLocalNeighborIpxAddrManagedObject::getClassName(),bgp_neighbor_addr));
    addPersistableAttributeForCreate (new AttributeManagedObjectComposition <BgpLocalAfIpv4UcastManagedObject> (&m_routerBgpAddressIpv4Uc, "routerBgpAddressIpv4Uc",BgpLocalAfIpv4UcastManagedObject::getClassName(), true,bgp_ipv4_unicast));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("vrfName");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
BgpLocalBasicConfigManagedObject::BgpLocalBasicConfigManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const string &vrfName,
                                                                                                                const string &localAsNum,
                                                                                                                const bool &alwaysCompareMedFlag,
                                                                                                                const bool &asPathIgnoreFlag,
                                                                                                                const bool &capabilityAs4Flag,
                                                                                                                const UI32 &choiceClusterId,
                                                                                                                const UI32 &clusterId,
                                                                                                                const IpV4Address &clusterIdIpAddress,
                                                                                                                const bool &compareMedEmptyAsPathFlag,
                                                                                                                const bool &compareRouterIdFlag,
                                                                                                                const UI32 &defaultLocalPreference,
                                                                                                                const UI32 &extRoutedistance,
                                                                                                                const UI32 &intRouteDistance,
                                                                                                                const UI32 &lclRouteDistance,
                                                                                                                const bool &enforceFirstAsFlag,
                                                                                                                const bool &fastExternalFallOverFlag,
                                                                                                                const bool &installIgpCostFlag,
                                                                                                                const bool &logDampeningDebug,
                                                                                                                const UI32 &maxAsLimit,
                                                                                                                const bool &medMissingAsWorstFlag,
                                                                                                                const UI32UC &routerBgpModeTimersKeepAliveTime,
                                                                                                                const UI32UC &routerBgpModeTimerHoldTime,
                                                                                                                const vector<WaveManagedObjectPointer<BgpLocalNeighborPeerGrpManagedObject> > &routerBgpNeighborPeerGrp,
                                                                                                                const vector<WaveManagedObjectPointer<BgpLocalNeighborIpxAddrManagedObject> > &routerBgpNeighborIpxAddr,
                                                                                                                const WaveManagedObjectPointer<BgpLocalAfIpv4UcastManagedObject> &routerBgpAddressIpv4Uc)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalBasicConfigManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    m_vrfName(vrfName),
    m_localAsNum(localAsNum),
    m_alwaysCompareMedFlag(alwaysCompareMedFlag),
    m_asPathIgnoreFlag(asPathIgnoreFlag),
    m_capabilityAs4Flag(capabilityAs4Flag),
    m_choiceClusterId(choiceClusterId),
    m_clusterId(clusterId),
    m_clusterIdIpAddress(clusterIdIpAddress),
    m_compareMedEmptyAsPathFlag(compareMedEmptyAsPathFlag),
    m_compareRouterIdFlag(compareRouterIdFlag),
    m_defaultLocalPreference(defaultLocalPreference),
    m_extRoutedistance(extRoutedistance),
    m_intRouteDistance(intRouteDistance),
    m_lclRouteDistance(lclRouteDistance),
    m_enforceFirstAsFlag(enforceFirstAsFlag),
    m_fastExternalFallOverFlag(fastExternalFallOverFlag),
    m_installIgpCostFlag(installIgpCostFlag),
    m_logDampeningDebug(logDampeningDebug),
    m_maxAsLimit(maxAsLimit),
    m_medMissingAsWorstFlag(medMissingAsWorstFlag),
    m_routerBgpModeTimersKeepAliveTime(routerBgpModeTimersKeepAliveTime),
    m_routerBgpModeTimerHoldTime(routerBgpModeTimerHoldTime),
    m_routerBgpNeighborPeerGrp(routerBgpNeighborPeerGrp),
    m_routerBgpNeighborIpxAddr(routerBgpNeighborIpxAddr),
    m_routerBgpAddressIpv4Uc(routerBgpAddressIpv4Uc)
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
 * Brief:BgpLocalBasicConfigManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalBasicConfigManagedObject::getClassName()
{
    return ("BgpLocalBasicConfigManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalBasicConfigManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalBasicConfigManagedObject::~BgpLocalBasicConfigManagedObject()
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
 * Brief:BgpLocalBasicConfigManagedObject::setvrfName:This function is used to set the value of member m_vrfName
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setvrfName (const string &vrfName)
{
    m_vrfName = vrfName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getvrfName:This function is used to get the value of member m_vrfName
 * @uBreezeClose
 */
string BgpLocalBasicConfigManagedObject::getvrfName ()
{
    return (m_vrfName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setlocalAsNum:This function is used to set the value of member m_localAsNum
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setlocalAsNum (const string &localAsNum)
{
    m_localAsNum = localAsNum;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getlocalAsNum:This function is used to get the value of member m_localAsNum
 * @uBreezeClose
 */
string BgpLocalBasicConfigManagedObject::getlocalAsNum ()
{
    return (m_localAsNum);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setalwaysCompareMedFlag:This function is used to set the value of member m_alwaysCompareMedFlag
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setalwaysCompareMedFlag (const bool &alwaysCompareMedFlag)
{
    m_alwaysCompareMedFlag = alwaysCompareMedFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getalwaysCompareMedFlag:This function is used to get the value of member m_alwaysCompareMedFlag
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getalwaysCompareMedFlag ()
{
    return (m_alwaysCompareMedFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setasPathIgnoreFlag:This function is used to set the value of member m_asPathIgnoreFlag
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setasPathIgnoreFlag (const bool &asPathIgnoreFlag)
{
    m_asPathIgnoreFlag = asPathIgnoreFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getasPathIgnoreFlag:This function is used to get the value of member m_asPathIgnoreFlag
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getasPathIgnoreFlag ()
{
    return (m_asPathIgnoreFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setcapabilityAs4Flag:This function is used to set the value of member m_capabilityAs4Flag
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setcapabilityAs4Flag (const bool &capabilityAs4Flag)
{
    m_capabilityAs4Flag = capabilityAs4Flag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getcapabilityAs4Flag:This function is used to get the value of member m_capabilityAs4Flag
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getcapabilityAs4Flag ()
{
    return (m_capabilityAs4Flag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setchoiceClusterId:This function is used to set the value of member m_choiceClusterId
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setchoiceClusterId (const UI32 &choiceClusterId)
{
    m_choiceClusterId = choiceClusterId;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getchoiceClusterId:This function is used to get the value of member m_choiceClusterId
 * @uBreezeClose
 */
UI32 BgpLocalBasicConfigManagedObject::getchoiceClusterId ()
{
    return (m_choiceClusterId);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setclusterId:This function is used to set the value of member m_clusterId
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setclusterId (const UI32 &clusterId)
{
    m_clusterId = clusterId;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getclusterId:This function is used to get the value of member m_clusterId
 * @uBreezeClose
 */
UI32 BgpLocalBasicConfigManagedObject::getclusterId ()
{
    return (m_clusterId);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setclusterIdIpAddress:This function is used to set the value of member m_clusterIdIpAddress
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setclusterIdIpAddress (const IpV4Address &clusterIdIpAddress)
{
    m_clusterIdIpAddress = clusterIdIpAddress;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getclusterIdIpAddress:This function is used to get the value of member m_clusterIdIpAddress
 * @uBreezeClose
 */
IpV4Address BgpLocalBasicConfigManagedObject::getclusterIdIpAddress ()
{
    return (m_clusterIdIpAddress);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setcompareMedEmptyAsPathFlag:This function is used to set the value of member m_compareMedEmptyAsPathFlag
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setcompareMedEmptyAsPathFlag (const bool &compareMedEmptyAsPathFlag)
{
    m_compareMedEmptyAsPathFlag = compareMedEmptyAsPathFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getcompareMedEmptyAsPathFlag:This function is used to get the value of member m_compareMedEmptyAsPathFlag
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getcompareMedEmptyAsPathFlag ()
{
    return (m_compareMedEmptyAsPathFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setcompareRouterIdFlag:This function is used to set the value of member m_compareRouterIdFlag
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setcompareRouterIdFlag (const bool &compareRouterIdFlag)
{
    m_compareRouterIdFlag = compareRouterIdFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getcompareRouterIdFlag:This function is used to get the value of member m_compareRouterIdFlag
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getcompareRouterIdFlag ()
{
    return (m_compareRouterIdFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setdefaultLocalPreference:This function is used to set the value of member m_defaultLocalPreference
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setdefaultLocalPreference (const UI32 &defaultLocalPreference)
{
    m_defaultLocalPreference = defaultLocalPreference;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getdefaultLocalPreference:This function is used to get the value of member m_defaultLocalPreference
 * @uBreezeClose
 */
UI32 BgpLocalBasicConfigManagedObject::getdefaultLocalPreference ()
{
    return (m_defaultLocalPreference);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setextRoutedistance:This function is used to set the value of member m_extRoutedistance
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setextRoutedistance (const UI32 &extRoutedistance)
{
    m_extRoutedistance = extRoutedistance;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getextRoutedistance:This function is used to get the value of member m_extRoutedistance
 * @uBreezeClose
 */
UI32 BgpLocalBasicConfigManagedObject::getextRoutedistance ()
{
    return (m_extRoutedistance);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setintRouteDistance:This function is used to set the value of member m_intRouteDistance
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setintRouteDistance (const UI32 &intRouteDistance)
{
    m_intRouteDistance = intRouteDistance;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getintRouteDistance:This function is used to get the value of member m_intRouteDistance
 * @uBreezeClose
 */
UI32 BgpLocalBasicConfigManagedObject::getintRouteDistance ()
{
    return (m_intRouteDistance);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setlclRouteDistance:This function is used to set the value of member m_lclRouteDistance
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setlclRouteDistance (const UI32 &lclRouteDistance)
{
    m_lclRouteDistance = lclRouteDistance;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getlclRouteDistance:This function is used to get the value of member m_lclRouteDistance
 * @uBreezeClose
 */
UI32 BgpLocalBasicConfigManagedObject::getlclRouteDistance ()
{
    return (m_lclRouteDistance);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setenforceFirstAsFlag:This function is used to set the value of member m_enforceFirstAsFlag
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setenforceFirstAsFlag (const bool &enforceFirstAsFlag)
{
    m_enforceFirstAsFlag = enforceFirstAsFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getenforceFirstAsFlag:This function is used to get the value of member m_enforceFirstAsFlag
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getenforceFirstAsFlag ()
{
    return (m_enforceFirstAsFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setfastExternalFallOverFlag:This function is used to set the value of member m_fastExternalFallOverFlag
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setfastExternalFallOverFlag (const bool &fastExternalFallOverFlag)
{
    m_fastExternalFallOverFlag = fastExternalFallOverFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getfastExternalFallOverFlag:This function is used to get the value of member m_fastExternalFallOverFlag
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getfastExternalFallOverFlag ()
{
    return (m_fastExternalFallOverFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setinstallIgpCostFlag:This function is used to set the value of member m_installIgpCostFlag
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setinstallIgpCostFlag (const bool &installIgpCostFlag)
{
    m_installIgpCostFlag = installIgpCostFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getinstallIgpCostFlag:This function is used to get the value of member m_installIgpCostFlag
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getinstallIgpCostFlag ()
{
    return (m_installIgpCostFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setlogDampeningDebug:This function is used to set the value of member m_logDampeningDebug
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setlogDampeningDebug (const bool &logDampeningDebug)
{
    m_logDampeningDebug = logDampeningDebug;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getlogDampeningDebug:This function is used to get the value of member m_logDampeningDebug
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getlogDampeningDebug ()
{
    return (m_logDampeningDebug);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setmaxAsLimit:This function is used to set the value of member m_maxAsLimit
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setmaxAsLimit (const UI32 &maxAsLimit)
{
    m_maxAsLimit = maxAsLimit;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getmaxAsLimit:This function is used to get the value of member m_maxAsLimit
 * @uBreezeClose
 */
UI32 BgpLocalBasicConfigManagedObject::getmaxAsLimit ()
{
    return (m_maxAsLimit);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setmedMissingAsWorstFlag:This function is used to set the value of member m_medMissingAsWorstFlag
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setmedMissingAsWorstFlag (const bool &medMissingAsWorstFlag)
{
    m_medMissingAsWorstFlag = medMissingAsWorstFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getmedMissingAsWorstFlag:This function is used to get the value of member m_medMissingAsWorstFlag
 * @uBreezeClose
 */
bool BgpLocalBasicConfigManagedObject::getmedMissingAsWorstFlag ()
{
    return (m_medMissingAsWorstFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setrouterBgpModeTimersKeepAliveTime:This function is used to set the value of member m_routerBgpModeTimersKeepAliveTime
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setrouterBgpModeTimersKeepAliveTime (const UI32UC &routerBgpModeTimersKeepAliveTime)
{
    m_routerBgpModeTimersKeepAliveTime = routerBgpModeTimersKeepAliveTime;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getrouterBgpModeTimersKeepAliveTime:This function is used to get the value of member m_routerBgpModeTimersKeepAliveTime
 * @uBreezeClose
 */
UI32UC BgpLocalBasicConfigManagedObject::getrouterBgpModeTimersKeepAliveTime ()
{
    return (m_routerBgpModeTimersKeepAliveTime);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setrouterBgpModeTimerHoldTime:This function is used to set the value of member m_routerBgpModeTimerHoldTime
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setrouterBgpModeTimerHoldTime (const UI32UC &routerBgpModeTimerHoldTime)
{
    m_routerBgpModeTimerHoldTime = routerBgpModeTimerHoldTime;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getrouterBgpModeTimerHoldTime:This function is used to get the value of member m_routerBgpModeTimerHoldTime
 * @uBreezeClose
 */
UI32UC BgpLocalBasicConfigManagedObject::getrouterBgpModeTimerHoldTime ()
{
    return (m_routerBgpModeTimerHoldTime);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setrouterBgpNeighborPeerGrp:This function is used to set the value of member m_routerBgpNeighborPeerGrp
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setrouterBgpNeighborPeerGrp (const vector<WaveManagedObjectPointer<BgpLocalNeighborPeerGrpManagedObject> > &routerBgpNeighborPeerGrp)
{
    m_routerBgpNeighborPeerGrp = routerBgpNeighborPeerGrp;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getrouterBgpNeighborPeerGrp:This function is used to get the value of member m_routerBgpNeighborPeerGrp
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<BgpLocalNeighborPeerGrpManagedObject> > BgpLocalBasicConfigManagedObject::getrouterBgpNeighborPeerGrp ()
{
    return (m_routerBgpNeighborPeerGrp);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setrouterBgpNeighborIpxAddr:This function is used to set the value of member m_routerBgpNeighborIpxAddr
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setrouterBgpNeighborIpxAddr (const vector<WaveManagedObjectPointer<BgpLocalNeighborIpxAddrManagedObject> > &routerBgpNeighborIpxAddr)
{
    m_routerBgpNeighborIpxAddr = routerBgpNeighborIpxAddr;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getrouterBgpNeighborIpxAddr:This function is used to get the value of member m_routerBgpNeighborIpxAddr
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<BgpLocalNeighborIpxAddrManagedObject> > BgpLocalBasicConfigManagedObject::getrouterBgpNeighborIpxAddr ()
{
    return (m_routerBgpNeighborIpxAddr);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::setrouterBgpAddressIpv4Uc:This function is used to set the value of member m_routerBgpAddressIpv4Uc
 * @uBreezeClose
 */
void BgpLocalBasicConfigManagedObject::setrouterBgpAddressIpv4Uc (const WaveManagedObjectPointer<BgpLocalAfIpv4UcastManagedObject> &routerBgpAddressIpv4Uc)
{
    m_routerBgpAddressIpv4Uc = routerBgpAddressIpv4Uc;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBasicConfigManagedObject::getrouterBgpAddressIpv4Uc:This function is used to get the value of member m_routerBgpAddressIpv4Uc
 * @uBreezeClose
 */
WaveManagedObjectPointer<BgpLocalAfIpv4UcastManagedObject> BgpLocalBasicConfigManagedObject::getrouterBgpAddressIpv4Uc ()
{
    return (m_routerBgpAddressIpv4Uc);
}

string BgpLocalBasicConfigManagedObject::customConfigurationDisplay (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext)
{
    string displayString = "bgp";

    if("default" != m_vrfName)
    {   
        displayString = displayString + " vrf " + m_vrfName;
    }

    return displayString;
}


}

