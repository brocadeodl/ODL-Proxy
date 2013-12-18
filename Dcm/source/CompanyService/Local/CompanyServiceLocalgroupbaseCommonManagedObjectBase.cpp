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
 *   Author : apurva                                                     *
 **************************************************************************/




#include "CompanyService/Local/CompanyServiceLocalgroupbaseCommonManagedObjectBase.h"
#include "CompanyService/Local/CompanyServiceLocalTypes.h"
#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService.h"
#include "CompanyService/Global/CompanyServiceCityManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalEmployeeManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalgroupbaseCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
CompanyServiceLocalgroupbaseCommonManagedObjectBase::CompanyServiceLocalgroupbaseCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
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
 * Brief:CompanyServiceLocalgroupbaseCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void CompanyServiceLocalgroupbaseCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    addPersistableAttributeForCommonBase (new AttributeObjectIdAssociation (&m_groupEmployee, "groupEmployee", CompanyServiceLocalEmployeeManagedObject::getClassName(), true,companyservice_groupEmployee));
    addPersistableAttributeForCommonBase (new AttributeObjectIdAssociation (&m_groupLocation, "groupLocation", CompanyServiceCityManagedObject::getClassName(), true,companyservice_groupLocation));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalgroupbaseCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceLocalgroupbaseCommonManagedObjectBase:: setupAttributesForCreate ()
{
    addPersistableAttributeForCommonBaseCreate (new AttributeObjectIdAssociation (&m_groupEmployee, "groupEmployee", CompanyServiceLocalEmployeeManagedObject::getClassName(), true,companyservice_groupEmployee));
    addPersistableAttributeForCommonBaseCreate (new AttributeObjectIdAssociation (&m_groupLocation, "groupLocation", CompanyServiceCityManagedObject::getClassName(), true,companyservice_groupLocation));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalgroupbaseCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceLocalgroupbaseCommonManagedObjectBase::getClassName()
{
    return ("CompanyServiceLocalgroupbaseCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceLocalgroupbaseCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
CompanyServiceLocalgroupbaseCommonManagedObjectBase::~CompanyServiceLocalgroupbaseCommonManagedObjectBase()
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
 * Brief:CompanyServiceLocalgroupbaseCommonManagedObjectBase::setgroupEmployee:This function is used to set the value of member m_groupEmployee
 * @uBreezeClose
 */
void CompanyServiceLocalgroupbaseCommonManagedObjectBase::setgroupEmployee (const ObjectId &groupEmployee)
{
    m_groupEmployee = groupEmployee;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalgroupbaseCommonManagedObjectBase::getgroupEmployee:This function is used to get the value of member m_groupEmployee
 * @uBreezeClose
 */
ObjectId CompanyServiceLocalgroupbaseCommonManagedObjectBase::getgroupEmployee ()
{
    return (m_groupEmployee);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalgroupbaseCommonManagedObjectBase::setgroupLocation:This function is used to set the value of member m_groupLocation
 * @uBreezeClose
 */
void CompanyServiceLocalgroupbaseCommonManagedObjectBase::setgroupLocation (const ObjectId &groupLocation)
{
    m_groupLocation = groupLocation;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalgroupbaseCommonManagedObjectBase::getgroupLocation:This function is used to get the value of member m_groupLocation
 * @uBreezeClose
 */
ObjectId CompanyServiceLocalgroupbaseCommonManagedObjectBase::getgroupLocation ()
{
    return (m_groupLocation);
}



}

