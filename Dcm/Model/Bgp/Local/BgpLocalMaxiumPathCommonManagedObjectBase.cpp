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




#include "Bgp/Local/BgpLocalMaxiumPathCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalMaxiumPathCommonManagedObjectBase::BgpLocalMaxiumPathCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    m_loadSharingValue(0),
    m_ebgpLoadSharingValue(0),
    m_ibgpLoadSharingValue(0),
    m_useLoadSharingFlag(false)
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
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalMaxiumPathCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_loadSharingValue, 0, "loadSharingValue",bgp_load_sharing_value));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_ebgpLoadSharingValue, 0, "ebgpLoadSharingValue",bgp_ebgp));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_ibgpLoadSharingValue, 0, "ibgpLoadSharingValue",bgp_ibgp));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_useLoadSharingFlag, true, "false", "useLoadSharingFlag",bgp_use_load_sharing));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalMaxiumPathCommonManagedObjectBase:: setupAttributesForCreate ()
{
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_loadSharingValue, 0, "loadSharingValue",bgp_load_sharing_value));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_ebgpLoadSharingValue, 0, "ebgpLoadSharingValue",bgp_ebgp));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_ibgpLoadSharingValue, 0, "ibgpLoadSharingValue",bgp_ibgp));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_useLoadSharingFlag, true, "false", "useLoadSharingFlag",bgp_use_load_sharing));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalMaxiumPathCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalMaxiumPathCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalMaxiumPathCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalMaxiumPathCommonManagedObjectBase::~BgpLocalMaxiumPathCommonManagedObjectBase()
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
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::setloadSharingValue:This function is used to set the value of member m_loadSharingValue
 * @uBreezeClose
 */
void BgpLocalMaxiumPathCommonManagedObjectBase::setloadSharingValue (const UI32 &loadSharingValue)
{
    m_loadSharingValue = loadSharingValue;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::getloadSharingValue:This function is used to get the value of member m_loadSharingValue
 * @uBreezeClose
 */
UI32 BgpLocalMaxiumPathCommonManagedObjectBase::getloadSharingValue ()
{
    return (m_loadSharingValue);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::setebgpLoadSharingValue:This function is used to set the value of member m_ebgpLoadSharingValue
 * @uBreezeClose
 */
void BgpLocalMaxiumPathCommonManagedObjectBase::setebgpLoadSharingValue (const UI32 &ebgpLoadSharingValue)
{
    m_ebgpLoadSharingValue = ebgpLoadSharingValue;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::getebgpLoadSharingValue:This function is used to get the value of member m_ebgpLoadSharingValue
 * @uBreezeClose
 */
UI32 BgpLocalMaxiumPathCommonManagedObjectBase::getebgpLoadSharingValue ()
{
    return (m_ebgpLoadSharingValue);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::setibgpLoadSharingValue:This function is used to set the value of member m_ibgpLoadSharingValue
 * @uBreezeClose
 */
void BgpLocalMaxiumPathCommonManagedObjectBase::setibgpLoadSharingValue (const UI32 &ibgpLoadSharingValue)
{
    m_ibgpLoadSharingValue = ibgpLoadSharingValue;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::getibgpLoadSharingValue:This function is used to get the value of member m_ibgpLoadSharingValue
 * @uBreezeClose
 */
UI32 BgpLocalMaxiumPathCommonManagedObjectBase::getibgpLoadSharingValue ()
{
    return (m_ibgpLoadSharingValue);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::setuseLoadSharingFlag:This function is used to set the value of member m_useLoadSharingFlag
 * @uBreezeClose
 */
void BgpLocalMaxiumPathCommonManagedObjectBase::setuseLoadSharingFlag (const bool &useLoadSharingFlag)
{
    m_useLoadSharingFlag = useLoadSharingFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMaxiumPathCommonManagedObjectBase::getuseLoadSharingFlag:This function is used to get the value of member m_useLoadSharingFlag
 * @uBreezeClose
 */
bool BgpLocalMaxiumPathCommonManagedObjectBase::getuseLoadSharingFlag ()
{
    return (m_useLoadSharingFlag);
}



}

