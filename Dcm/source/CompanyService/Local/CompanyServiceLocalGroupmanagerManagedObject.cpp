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




#include "CompanyService/Local/CompanyServiceLocalGroupmanagerManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalTypes.h"
#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalGroupmanagerManagedObject::CompanyServiceLocalGroupmanagerManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalGroupmanagerManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    m_groupmanagername("")
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
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeString (&m_groupmanagername, true, "", "groupmanagername",companyservice_groupmanagername));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeString (&m_groupmanagername, true, "", "groupmanagername",companyservice_groupmanagername));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("groupmanagername");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
CompanyServiceLocalGroupmanagerManagedObject::CompanyServiceLocalGroupmanagerManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager,
                                                                                                                const string &groupmanagername)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalGroupmanagerManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    m_groupmanagername(groupmanagername)
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
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceLocalGroupmanagerManagedObject::getClassName()
{
    return ("CompanyServiceLocalGroupmanagerManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceLocalGroupmanagerManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalGroupmanagerManagedObject::~CompanyServiceLocalGroupmanagerManagedObject()
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
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::setgroupmanagername:This function is used to set the value of member m_groupmanagername
 * @uBreezeClose
 */
void CompanyServiceLocalGroupmanagerManagedObject::setgroupmanagername (const string &groupmanagername)
{
    m_groupmanagername = groupmanagername;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupmanagerManagedObject::getgroupmanagername:This function is used to get the value of member m_groupmanagername
 * @uBreezeClose
 */
string CompanyServiceLocalGroupmanagerManagedObject::getgroupmanagername ()
{
    return (m_groupmanagername);
}



}

