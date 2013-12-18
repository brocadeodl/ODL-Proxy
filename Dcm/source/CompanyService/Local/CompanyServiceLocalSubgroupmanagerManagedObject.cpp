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




#include "CompanyService/Local/CompanyServiceLocalSubgroupmanagerManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalTypes.h"
#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalSubgroupmanagerManagedObject::CompanyServiceLocalSubgroupmanagerManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalSubgroupmanagerManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    m_subgroupmanagername("")
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
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeString (&m_subgroupmanagername, true, "", "subgroupmanagername",companyservice_subgroupmanagername));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeString (&m_subgroupmanagername, true, "", "subgroupmanagername",companyservice_subgroupmanagername));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("subgroupmanagername");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
CompanyServiceLocalSubgroupmanagerManagedObject::CompanyServiceLocalSubgroupmanagerManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager,
                                                                                                                const string &subgroupmanagername)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalSubgroupmanagerManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    m_subgroupmanagername(subgroupmanagername)
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
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceLocalSubgroupmanagerManagedObject::getClassName()
{
    return ("CompanyServiceLocalSubgroupmanagerManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceLocalSubgroupmanagerManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalSubgroupmanagerManagedObject::~CompanyServiceLocalSubgroupmanagerManagedObject()
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
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::setsubgroupmanagername:This function is used to set the value of member m_subgroupmanagername
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupmanagerManagedObject::setsubgroupmanagername (const string &subgroupmanagername)
{
    m_subgroupmanagername = subgroupmanagername;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupmanagerManagedObject::getsubgroupmanagername:This function is used to get the value of member m_subgroupmanagername
 * @uBreezeClose
 */
string CompanyServiceLocalSubgroupmanagerManagedObject::getsubgroupmanagername ()
{
    return (m_subgroupmanagername);
}



}

