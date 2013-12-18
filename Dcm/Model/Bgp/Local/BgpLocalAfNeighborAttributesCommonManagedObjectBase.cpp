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




#include "Bgp/Local/BgpLocalAfNeighborAttributesCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfNeighborAttributesCommonManagedObjectBase::BgpLocalAfNeighborAttributesCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    m_activateFlag(false),
    m_filterListDirectionInAclName(""),
    m_filterListdirectionInAclNameFlag(false),
    m_filterListDirectionOutAclName(""),
    m_filterListdirectionOutAclNameFlag(false),
    m_maxiumPrefixLimit(0),
    m_maxiumPrefixthreshold(0),
    m_maxiumPrefixTeardownFlag(false),
    m_defaultOriginateStatus(false),
    m_defaultOriginateRouteMapName(""),
    m_prefixListInName(""),
    m_prefixListInFlag(false),
    m_prefixListOutName(""),
    m_prefixListOutFlag(false),
    m_routeMapNameIn(""),
    m_routeMapNameOut(""),
    m_routeReflectorClientFlag(false),
    m_sendCommunityBoth(false),
    m_sendCommunityExtended(false),
    m_sendCommunityStandard(false),
    m_sendCommunityStatus(false),
    m_unSuppressRouteMapName(""),
    m_weight(0)
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
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    addPersistableAttributeForCommonBase (new AttributeBool (&m_activateFlag, true, "false", "activateFlag",bgp_activate));
    addPersistableAttributeForCommonBase (new AttributeString (&m_filterListDirectionInAclName, true, "", "filterListDirectionInAclName",bgp_filter_list_direction_in_acl_name));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_filterListdirectionInAclNameFlag, true, "false", "filterListdirectionInAclNameFlag",bgp_filter_list_name_direction_in));
    addPersistableAttributeForCommonBase (new AttributeString (&m_filterListDirectionOutAclName, true, "", "filterListDirectionOutAclName",bgp_filter_list_direction_out_acl_name));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_filterListdirectionOutAclNameFlag, true, "false", "filterListdirectionOutAclNameFlag",bgp_filter_list_name_direction_out));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_maxiumPrefixLimit, 0, "maxiumPrefixLimit",bgp_max_prefix_limit));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_maxiumPrefixthreshold, 0, "maxiumPrefixthreshold",bgp_threshold));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_maxiumPrefixTeardownFlag, true, "false", "maxiumPrefixTeardownFlag",bgp_teardown));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_defaultOriginateStatus, true, "false", "defaultOriginateStatus",bgp_default_originate_status));
    addPersistableAttributeForCommonBase (new AttributeString (&m_defaultOriginateRouteMapName, true, "", "defaultOriginateRouteMapName",bgp_default_originate_route_map));
    addPersistableAttributeForCommonBase (new AttributeString (&m_prefixListInName, true, "", "prefixListInName",bgp_prefix_list_direction_in_prefix_name));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_prefixListInFlag, true, "false", "prefixListInFlag",bgp_prefix_list_direction_in));
    addPersistableAttributeForCommonBase (new AttributeString (&m_prefixListOutName, true, "", "prefixListOutName",bgp_prefix_list_direction_out_prefix_name));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_prefixListOutFlag, true, "false", "prefixListOutFlag",bgp_prefix_list_direction_out));
    addPersistableAttributeForCommonBase (new AttributeString (&m_routeMapNameIn, true, "", "routeMapNameIn",bgp_neighbor_route_map_name_direction_in));
    addPersistableAttributeForCommonBase (new AttributeString (&m_routeMapNameOut, true, "", "routeMapNameOut",bgp_neighbor_route_map_name_direction_out));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_routeReflectorClientFlag, true, "false", "routeReflectorClientFlag",bgp_route_reflector_client));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_sendCommunityBoth, true, "false", "sendCommunityBoth",bgp_both));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_sendCommunityExtended, true, "false", "sendCommunityExtended",bgp_extended));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_sendCommunityStandard, true, "false", "sendCommunityStandard",bgp_standard));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_sendCommunityStatus, true, "false", "sendCommunityStatus",bgp_send_community_status));
    addPersistableAttributeForCommonBase (new AttributeString (&m_unSuppressRouteMapName, true, "", "unSuppressRouteMapName",bgp_map_name));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_weight, 0, "weight",bgp_af_nei_weight));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase:: setupAttributesForCreate ()
{
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_activateFlag, true, "false", "activateFlag",bgp_activate));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_filterListDirectionInAclName, true, "", "filterListDirectionInAclName",bgp_filter_list_direction_in_acl_name));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_filterListdirectionInAclNameFlag, true, "false", "filterListdirectionInAclNameFlag",bgp_filter_list_name_direction_in));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_filterListDirectionOutAclName, true, "", "filterListDirectionOutAclName",bgp_filter_list_direction_out_acl_name));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_filterListdirectionOutAclNameFlag, true, "false", "filterListdirectionOutAclNameFlag",bgp_filter_list_name_direction_out));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_maxiumPrefixLimit, 0, "maxiumPrefixLimit",bgp_max_prefix_limit));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_maxiumPrefixthreshold, 0, "maxiumPrefixthreshold",bgp_threshold));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_maxiumPrefixTeardownFlag, true, "false", "maxiumPrefixTeardownFlag",bgp_teardown));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_defaultOriginateStatus, true, "false", "defaultOriginateStatus",bgp_default_originate_status));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_defaultOriginateRouteMapName, true, "", "defaultOriginateRouteMapName",bgp_default_originate_route_map));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_prefixListInName, true, "", "prefixListInName",bgp_prefix_list_direction_in_prefix_name));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_prefixListInFlag, true, "false", "prefixListInFlag",bgp_prefix_list_direction_in));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_prefixListOutName, true, "", "prefixListOutName",bgp_prefix_list_direction_out_prefix_name));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_prefixListOutFlag, true, "false", "prefixListOutFlag",bgp_prefix_list_direction_out));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_routeMapNameIn, true, "", "routeMapNameIn",bgp_neighbor_route_map_name_direction_in));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_routeMapNameOut, true, "", "routeMapNameOut",bgp_neighbor_route_map_name_direction_out));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_routeReflectorClientFlag, true, "false", "routeReflectorClientFlag",bgp_route_reflector_client));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_sendCommunityBoth, true, "false", "sendCommunityBoth",bgp_both));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_sendCommunityExtended, true, "false", "sendCommunityExtended",bgp_extended));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_sendCommunityStandard, true, "false", "sendCommunityStandard",bgp_standard));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_sendCommunityStatus, true, "false", "sendCommunityStatus",bgp_send_community_status));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_unSuppressRouteMapName, true, "", "unSuppressRouteMapName",bgp_map_name));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_weight, 0, "weight",bgp_af_nei_weight));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfNeighborAttributesCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalAfNeighborAttributesCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfNeighborAttributesCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfNeighborAttributesCommonManagedObjectBase::~BgpLocalAfNeighborAttributesCommonManagedObjectBase()
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
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setactivateFlag:This function is used to set the value of member m_activateFlag
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setactivateFlag (const bool &activateFlag)
{
    m_activateFlag = activateFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getactivateFlag:This function is used to get the value of member m_activateFlag
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getactivateFlag ()
{
    return (m_activateFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setfilterListDirectionInAclName:This function is used to set the value of member m_filterListDirectionInAclName
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setfilterListDirectionInAclName (const string &filterListDirectionInAclName)
{
    m_filterListDirectionInAclName = filterListDirectionInAclName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getfilterListDirectionInAclName:This function is used to get the value of member m_filterListDirectionInAclName
 * @uBreezeClose
 */
string BgpLocalAfNeighborAttributesCommonManagedObjectBase::getfilterListDirectionInAclName ()
{
    return (m_filterListDirectionInAclName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setfilterListdirectionInAclNameFlag:This function is used to set the value of member m_filterListdirectionInAclNameFlag
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setfilterListdirectionInAclNameFlag (const bool &filterListdirectionInAclNameFlag)
{
    m_filterListdirectionInAclNameFlag = filterListdirectionInAclNameFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getfilterListdirectionInAclNameFlag:This function is used to get the value of member m_filterListdirectionInAclNameFlag
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getfilterListdirectionInAclNameFlag ()
{
    return (m_filterListdirectionInAclNameFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setfilterListDirectionOutAclName:This function is used to set the value of member m_filterListDirectionOutAclName
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setfilterListDirectionOutAclName (const string &filterListDirectionOutAclName)
{
    m_filterListDirectionOutAclName = filterListDirectionOutAclName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getfilterListDirectionOutAclName:This function is used to get the value of member m_filterListDirectionOutAclName
 * @uBreezeClose
 */
string BgpLocalAfNeighborAttributesCommonManagedObjectBase::getfilterListDirectionOutAclName ()
{
    return (m_filterListDirectionOutAclName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setfilterListdirectionOutAclNameFlag:This function is used to set the value of member m_filterListdirectionOutAclNameFlag
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setfilterListdirectionOutAclNameFlag (const bool &filterListdirectionOutAclNameFlag)
{
    m_filterListdirectionOutAclNameFlag = filterListdirectionOutAclNameFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getfilterListdirectionOutAclNameFlag:This function is used to get the value of member m_filterListdirectionOutAclNameFlag
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getfilterListdirectionOutAclNameFlag ()
{
    return (m_filterListdirectionOutAclNameFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setmaxiumPrefixLimit:This function is used to set the value of member m_maxiumPrefixLimit
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setmaxiumPrefixLimit (const UI32 &maxiumPrefixLimit)
{
    m_maxiumPrefixLimit = maxiumPrefixLimit;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getmaxiumPrefixLimit:This function is used to get the value of member m_maxiumPrefixLimit
 * @uBreezeClose
 */
UI32 BgpLocalAfNeighborAttributesCommonManagedObjectBase::getmaxiumPrefixLimit ()
{
    return (m_maxiumPrefixLimit);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setmaxiumPrefixthreshold:This function is used to set the value of member m_maxiumPrefixthreshold
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setmaxiumPrefixthreshold (const UI32 &maxiumPrefixthreshold)
{
    m_maxiumPrefixthreshold = maxiumPrefixthreshold;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getmaxiumPrefixthreshold:This function is used to get the value of member m_maxiumPrefixthreshold
 * @uBreezeClose
 */
UI32 BgpLocalAfNeighborAttributesCommonManagedObjectBase::getmaxiumPrefixthreshold ()
{
    return (m_maxiumPrefixthreshold);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setmaxiumPrefixTeardownFlag:This function is used to set the value of member m_maxiumPrefixTeardownFlag
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setmaxiumPrefixTeardownFlag (const bool &maxiumPrefixTeardownFlag)
{
    m_maxiumPrefixTeardownFlag = maxiumPrefixTeardownFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getmaxiumPrefixTeardownFlag:This function is used to get the value of member m_maxiumPrefixTeardownFlag
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getmaxiumPrefixTeardownFlag ()
{
    return (m_maxiumPrefixTeardownFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setdefaultOriginateStatus:This function is used to set the value of member m_defaultOriginateStatus
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setdefaultOriginateStatus (const bool &defaultOriginateStatus)
{
    m_defaultOriginateStatus = defaultOriginateStatus;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getdefaultOriginateStatus:This function is used to get the value of member m_defaultOriginateStatus
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getdefaultOriginateStatus ()
{
    return (m_defaultOriginateStatus);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setdefaultOriginateRouteMapName:This function is used to set the value of member m_defaultOriginateRouteMapName
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setdefaultOriginateRouteMapName (const string &defaultOriginateRouteMapName)
{
    m_defaultOriginateRouteMapName = defaultOriginateRouteMapName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getdefaultOriginateRouteMapName:This function is used to get the value of member m_defaultOriginateRouteMapName
 * @uBreezeClose
 */
string BgpLocalAfNeighborAttributesCommonManagedObjectBase::getdefaultOriginateRouteMapName ()
{
    return (m_defaultOriginateRouteMapName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setprefixListInName:This function is used to set the value of member m_prefixListInName
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setprefixListInName (const string &prefixListInName)
{
    m_prefixListInName = prefixListInName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getprefixListInName:This function is used to get the value of member m_prefixListInName
 * @uBreezeClose
 */
string BgpLocalAfNeighborAttributesCommonManagedObjectBase::getprefixListInName ()
{
    return (m_prefixListInName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setprefixListInFlag:This function is used to set the value of member m_prefixListInFlag
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setprefixListInFlag (const bool &prefixListInFlag)
{
    m_prefixListInFlag = prefixListInFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getprefixListInFlag:This function is used to get the value of member m_prefixListInFlag
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getprefixListInFlag ()
{
    return (m_prefixListInFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setprefixListOutName:This function is used to set the value of member m_prefixListOutName
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setprefixListOutName (const string &prefixListOutName)
{
    m_prefixListOutName = prefixListOutName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getprefixListOutName:This function is used to get the value of member m_prefixListOutName
 * @uBreezeClose
 */
string BgpLocalAfNeighborAttributesCommonManagedObjectBase::getprefixListOutName ()
{
    return (m_prefixListOutName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setprefixListOutFlag:This function is used to set the value of member m_prefixListOutFlag
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setprefixListOutFlag (const bool &prefixListOutFlag)
{
    m_prefixListOutFlag = prefixListOutFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getprefixListOutFlag:This function is used to get the value of member m_prefixListOutFlag
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getprefixListOutFlag ()
{
    return (m_prefixListOutFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setrouteMapNameIn:This function is used to set the value of member m_routeMapNameIn
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setrouteMapNameIn (const string &routeMapNameIn)
{
    m_routeMapNameIn = routeMapNameIn;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getrouteMapNameIn:This function is used to get the value of member m_routeMapNameIn
 * @uBreezeClose
 */
string BgpLocalAfNeighborAttributesCommonManagedObjectBase::getrouteMapNameIn ()
{
    return (m_routeMapNameIn);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setrouteMapNameOut:This function is used to set the value of member m_routeMapNameOut
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setrouteMapNameOut (const string &routeMapNameOut)
{
    m_routeMapNameOut = routeMapNameOut;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getrouteMapNameOut:This function is used to get the value of member m_routeMapNameOut
 * @uBreezeClose
 */
string BgpLocalAfNeighborAttributesCommonManagedObjectBase::getrouteMapNameOut ()
{
    return (m_routeMapNameOut);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setrouteReflectorClientFlag:This function is used to set the value of member m_routeReflectorClientFlag
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setrouteReflectorClientFlag (const bool &routeReflectorClientFlag)
{
    m_routeReflectorClientFlag = routeReflectorClientFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getrouteReflectorClientFlag:This function is used to get the value of member m_routeReflectorClientFlag
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getrouteReflectorClientFlag ()
{
    return (m_routeReflectorClientFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setsendCommunityBoth:This function is used to set the value of member m_sendCommunityBoth
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setsendCommunityBoth (const bool &sendCommunityBoth)
{
    m_sendCommunityBoth = sendCommunityBoth;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getsendCommunityBoth:This function is used to get the value of member m_sendCommunityBoth
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getsendCommunityBoth ()
{
    return (m_sendCommunityBoth);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setsendCommunityExtended:This function is used to set the value of member m_sendCommunityExtended
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setsendCommunityExtended (const bool &sendCommunityExtended)
{
    m_sendCommunityExtended = sendCommunityExtended;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getsendCommunityExtended:This function is used to get the value of member m_sendCommunityExtended
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getsendCommunityExtended ()
{
    return (m_sendCommunityExtended);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setsendCommunityStandard:This function is used to set the value of member m_sendCommunityStandard
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setsendCommunityStandard (const bool &sendCommunityStandard)
{
    m_sendCommunityStandard = sendCommunityStandard;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getsendCommunityStandard:This function is used to get the value of member m_sendCommunityStandard
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getsendCommunityStandard ()
{
    return (m_sendCommunityStandard);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setsendCommunityStatus:This function is used to set the value of member m_sendCommunityStatus
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setsendCommunityStatus (const bool &sendCommunityStatus)
{
    m_sendCommunityStatus = sendCommunityStatus;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getsendCommunityStatus:This function is used to get the value of member m_sendCommunityStatus
 * @uBreezeClose
 */
bool BgpLocalAfNeighborAttributesCommonManagedObjectBase::getsendCommunityStatus ()
{
    return (m_sendCommunityStatus);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setunSuppressRouteMapName:This function is used to set the value of member m_unSuppressRouteMapName
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setunSuppressRouteMapName (const string &unSuppressRouteMapName)
{
    m_unSuppressRouteMapName = unSuppressRouteMapName;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getunSuppressRouteMapName:This function is used to get the value of member m_unSuppressRouteMapName
 * @uBreezeClose
 */
string BgpLocalAfNeighborAttributesCommonManagedObjectBase::getunSuppressRouteMapName ()
{
    return (m_unSuppressRouteMapName);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::setweight:This function is used to set the value of member m_weight
 * @uBreezeClose
 */
void BgpLocalAfNeighborAttributesCommonManagedObjectBase::setweight (const UI32 &weight)
{
    m_weight = weight;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfNeighborAttributesCommonManagedObjectBase::getweight:This function is used to get the value of member m_weight
 * @uBreezeClose
 */
UI32 BgpLocalAfNeighborAttributesCommonManagedObjectBase::getweight ()
{
    return (m_weight);
}



}

