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




#include "Bgp/Local/BgpLocalAggregateAttributeCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAggregateAttributeCommonManagedObjectBase::BgpLocalAggregateAttributeCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    m_advertiseMap(""),
    m_asSetFlag(false),
    m_attributeMap(""),
    m_summaryOnlyFlag(false),
    m_suppressMap("")
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
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalAggregateAttributeCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    addPersistableAttributeForCommonBase (new AttributeString (&m_advertiseMap, true, "", "advertiseMap",bgp_advertise_map));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_asSetFlag, true, "false", "asSetFlag",bgp_as_set));
    addPersistableAttributeForCommonBase (new AttributeString (&m_attributeMap, true, "", "attributeMap",bgp_attribute_map));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_summaryOnlyFlag, true, "false", "summaryOnlyFlag",bgp_summary_only));
    addPersistableAttributeForCommonBase (new AttributeString (&m_suppressMap, true, "", "suppressMap",bgp_suppress_map));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAggregateAttributeCommonManagedObjectBase:: setupAttributesForCreate ()
{
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_advertiseMap, true, "", "advertiseMap",bgp_advertise_map));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_asSetFlag, true, "false", "asSetFlag",bgp_as_set));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_attributeMap, true, "", "attributeMap",bgp_attribute_map));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_summaryOnlyFlag, true, "false", "summaryOnlyFlag",bgp_summary_only));
    addPersistableAttributeForCommonBaseCreate (new AttributeString (&m_suppressMap, true, "", "suppressMap",bgp_suppress_map));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAggregateAttributeCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalAggregateAttributeCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAggregateAttributeCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAggregateAttributeCommonManagedObjectBase::~BgpLocalAggregateAttributeCommonManagedObjectBase()
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
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::setadvertiseMap:This function is used to set the value of member m_advertiseMap
 * @uBreezeClose
 */
void BgpLocalAggregateAttributeCommonManagedObjectBase::setadvertiseMap (const string &advertiseMap)
{
    m_advertiseMap = advertiseMap;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::getadvertiseMap:This function is used to get the value of member m_advertiseMap
 * @uBreezeClose
 */
string BgpLocalAggregateAttributeCommonManagedObjectBase::getadvertiseMap ()
{
    return (m_advertiseMap);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::setasSetFlag:This function is used to set the value of member m_asSetFlag
 * @uBreezeClose
 */
void BgpLocalAggregateAttributeCommonManagedObjectBase::setasSetFlag (const bool &asSetFlag)
{
    m_asSetFlag = asSetFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::getasSetFlag:This function is used to get the value of member m_asSetFlag
 * @uBreezeClose
 */
bool BgpLocalAggregateAttributeCommonManagedObjectBase::getasSetFlag ()
{
    return (m_asSetFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::setattributeMap:This function is used to set the value of member m_attributeMap
 * @uBreezeClose
 */
void BgpLocalAggregateAttributeCommonManagedObjectBase::setattributeMap (const string &attributeMap)
{
    m_attributeMap = attributeMap;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::getattributeMap:This function is used to get the value of member m_attributeMap
 * @uBreezeClose
 */
string BgpLocalAggregateAttributeCommonManagedObjectBase::getattributeMap ()
{
    return (m_attributeMap);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::setsummaryOnlyFlag:This function is used to set the value of member m_summaryOnlyFlag
 * @uBreezeClose
 */
void BgpLocalAggregateAttributeCommonManagedObjectBase::setsummaryOnlyFlag (const bool &summaryOnlyFlag)
{
    m_summaryOnlyFlag = summaryOnlyFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::getsummaryOnlyFlag:This function is used to get the value of member m_summaryOnlyFlag
 * @uBreezeClose
 */
bool BgpLocalAggregateAttributeCommonManagedObjectBase::getsummaryOnlyFlag ()
{
    return (m_summaryOnlyFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::setsuppressMap:This function is used to set the value of member m_suppressMap
 * @uBreezeClose
 */
void BgpLocalAggregateAttributeCommonManagedObjectBase::setsuppressMap (const string &suppressMap)
{
    m_suppressMap = suppressMap;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAggregateAttributeCommonManagedObjectBase::getsuppressMap:This function is used to get the value of member m_suppressMap
 * @uBreezeClose
 */
string BgpLocalAggregateAttributeCommonManagedObjectBase::getsuppressMap ()
{
    return (m_suppressMap);
}



}

