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




#include "CompanyService/Local/CompanyServiceLocalEmployeeManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalTypes.h"
#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService.h"
#include "CompanyService/Global/CompanyServiceCityManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalGroupManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalStockOptionManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalEmployeeManagedObject::CompanyServiceLocalEmployeeManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalEmployeeManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    m_employeename("")
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
 * Brief:CompanyServiceLocalEmployeeManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_employeelocation, "employeelocation", CompanyServiceCityManagedObject::getClassName(), true,companyservice_employeelocation));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_employeegroup, "employeegroup", CompanyServiceLocalGroupManagedObject::getClassName(), true,companyservice_employeegroup));
    addPersistableAttribute (new AttributeString (&m_employeename, true, "", "employeename",companyservice_employeename));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_employeelocation, "employeelocation", CompanyServiceCityManagedObject::getClassName(), true,companyservice_employeelocation));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_employeegroup, "employeegroup", CompanyServiceLocalGroupManagedObject::getClassName(), true,companyservice_employeegroup));
    addPersistableAttributeForCreate (new AttributeString (&m_employeename, true, "", "employeename",companyservice_employeename));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("employeename");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
CompanyServiceLocalEmployeeManagedObject::CompanyServiceLocalEmployeeManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager,
                                                                                                                const ObjectId &employeelocation,
                                                                                                                const ObjectId &employeegroup,
                                                                                                                const string &employeename)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalEmployeeManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    m_employeelocation(employeelocation),
    m_employeegroup(employeegroup),
    m_employeename(employeename)
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
 * Brief:CompanyServiceLocalEmployeeManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceLocalEmployeeManagedObject::getClassName()
{
    return ("CompanyServiceLocalEmployeeManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceLocalEmployeeManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalEmployeeManagedObject::~CompanyServiceLocalEmployeeManagedObject()
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
 * Brief:CompanyServiceLocalEmployeeManagedObject::setemployeelocation:This function is used to set the value of member m_employeelocation
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::setemployeelocation (const ObjectId &employeelocation)
{
    m_employeelocation = employeelocation;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::getemployeelocation:This function is used to get the value of member m_employeelocation
 * @uBreezeClose
 */
ObjectId CompanyServiceLocalEmployeeManagedObject::getemployeelocation ()
{
    return (m_employeelocation);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::setemployeegroup:This function is used to set the value of member m_employeegroup
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::setemployeegroup (const ObjectId &employeegroup)
{
    m_employeegroup = employeegroup;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::getemployeegroup:This function is used to get the value of member m_employeegroup
 * @uBreezeClose
 */
ObjectId CompanyServiceLocalEmployeeManagedObject::getemployeegroup ()
{
    return (m_employeegroup);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::setemployeeStockOption:This function is used to set the value of member m_employeeStockOption
 * @uBreezeClose
 */
/*
void CompanyServiceLocalEmployeeManagedObject::setemployeeStockOption (const ObjectId &employeeStockOption)
{
    m_employeeStockOption = employeeStockOption;
}
*/

/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::getemployeeStockOption:This function is used to get the value of member m_employeeStockOption
 * @uBreezeClose
 */
/*
ObjectId CompanyServiceLocalEmployeeManagedObject::getemployeeStockOption ()
{
    return (m_employeeStockOption);
}

*/
/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::setemployeename:This function is used to set the value of member m_employeename
 * @uBreezeClose
 */
void CompanyServiceLocalEmployeeManagedObject::setemployeename (const string &employeename)
{
    m_employeename = employeename;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalEmployeeManagedObject::getemployeename:This function is used to get the value of member m_employeename
 * @uBreezeClose
 */
string CompanyServiceLocalEmployeeManagedObject::getemployeename ()
{
    return (m_employeename);
}



}

