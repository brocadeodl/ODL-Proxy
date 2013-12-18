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




#include "Bgp/Local/BgpLocalAfBasicAttributesCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfBasicAttributesCommonManagedObjectBase::BgpLocalAfBasicAttributesCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalMaxiumPathCommonManagedObjectBase(pPrismPersistableObject),
    BgpLocalMultiPathCommonManagedObjectBase(pPrismPersistableObject),
    m_alwaysPropogateFlag(false),
    m_defaultInformationOriginateFlag(false),
    m_ribRouteLimit(0)
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
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalAfBasicAttributesCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    BgpLocalMaxiumPathCommonManagedObjectBase::setupAttributesForPersistence();
    BgpLocalMultiPathCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttributeForCommonBase (new AttributeBool (&m_alwaysPropogateFlag, true, "false", "alwaysPropogateFlag",bgp_always_propagate));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_defaultInformationOriginateFlag, true, "false", "defaultInformationOriginateFlag",bgp_default_information_originate));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_ribRouteLimit, 0, "ribRouteLimit",bgp_rib_route_limit));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfBasicAttributesCommonManagedObjectBase:: setupAttributesForCreate ()
{
    BgpLocalMaxiumPathCommonManagedObjectBase::setupAttributesForCreate();
    BgpLocalMultiPathCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_alwaysPropogateFlag, true, "false", "alwaysPropogateFlag",bgp_always_propagate));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_defaultInformationOriginateFlag, true, "false", "defaultInformationOriginateFlag",bgp_default_information_originate));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_ribRouteLimit, 0, "ribRouteLimit",bgp_rib_route_limit));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfBasicAttributesCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalAfBasicAttributesCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfBasicAttributesCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalAfBasicAttributesCommonManagedObjectBase::~BgpLocalAfBasicAttributesCommonManagedObjectBase()
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
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase::setalwaysPropogateFlag:This function is used to set the value of member m_alwaysPropogateFlag
 * @uBreezeClose
 */
void BgpLocalAfBasicAttributesCommonManagedObjectBase::setalwaysPropogateFlag (const bool &alwaysPropogateFlag)
{
    m_alwaysPropogateFlag = alwaysPropogateFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase::getalwaysPropogateFlag:This function is used to get the value of member m_alwaysPropogateFlag
 * @uBreezeClose
 */
bool BgpLocalAfBasicAttributesCommonManagedObjectBase::getalwaysPropogateFlag ()
{
    return (m_alwaysPropogateFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase::setdefaultInformationOriginateFlag:This function is used to set the value of member m_defaultInformationOriginateFlag
 * @uBreezeClose
 */
void BgpLocalAfBasicAttributesCommonManagedObjectBase::setdefaultInformationOriginateFlag (const bool &defaultInformationOriginateFlag)
{
    m_defaultInformationOriginateFlag = defaultInformationOriginateFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase::getdefaultInformationOriginateFlag:This function is used to get the value of member m_defaultInformationOriginateFlag
 * @uBreezeClose
 */
bool BgpLocalAfBasicAttributesCommonManagedObjectBase::getdefaultInformationOriginateFlag ()
{
    return (m_defaultInformationOriginateFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase::setribRouteLimit:This function is used to set the value of member m_ribRouteLimit
 * @uBreezeClose
 */
void BgpLocalAfBasicAttributesCommonManagedObjectBase::setribRouteLimit (const UI32 &ribRouteLimit)
{
    m_ribRouteLimit = ribRouteLimit;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfBasicAttributesCommonManagedObjectBase::getribRouteLimit:This function is used to get the value of member m_ribRouteLimit
 * @uBreezeClose
 */
UI32 BgpLocalAfBasicAttributesCommonManagedObjectBase::getribRouteLimit ()
{
    return (m_ribRouteLimit);
}



}

