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




#include "CompanyService/Local/CompanyServiceLocalSubgroupManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalTypes.h"
#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalSubgroupManagedObject::CompanyServiceLocalSubgroupManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalSubgroupManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    CommonManagedObjectBase(this),
    CompanyServiceLocalgroupbaseCommonManagedObjectBase(this),
    m_subgroupname("")
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
 * Brief:CompanyServiceLocalSubgroupManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    CompanyServiceLocalgroupbaseCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeString (&m_subgroupname, true, "", "subgroupname",companyservice_subgroupname));
    addPersistableAttribute (new AttributeManagedObjectComposition <CompanyServiceLocalSubgroupmanagerManagedObject> (&m_subgroupmanager, "subgroupmanager",CompanyServiceLocalSubgroupmanagerManagedObject::getClassName(), true,companyservice_subgroupmanager));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    CompanyServiceLocalgroupbaseCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeString (&m_subgroupname, true, "", "subgroupname",companyservice_subgroupname));
    addPersistableAttributeForCreate (new AttributeManagedObjectComposition <CompanyServiceLocalSubgroupmanagerManagedObject> (&m_subgroupmanager, "subgroupmanager",CompanyServiceLocalSubgroupmanagerManagedObject::getClassName(), true,companyservice_subgroupmanager));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("subgroupname");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
CompanyServiceLocalSubgroupManagedObject::CompanyServiceLocalSubgroupManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager,
                                                                                                                const string &subgroupname,
                                                                                                                const WaveManagedObjectPointer<CompanyServiceLocalSubgroupmanagerManagedObject> &subgroupmanager)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalSubgroupManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    CommonManagedObjectBase(this),
    CompanyServiceLocalgroupbaseCommonManagedObjectBase(this),
    m_subgroupname(subgroupname),
    m_subgroupmanager(subgroupmanager)
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
 * Brief:CompanyServiceLocalSubgroupManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceLocalSubgroupManagedObject::getClassName()
{
    return ("CompanyServiceLocalSubgroupManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceLocalSubgroupManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalSubgroupManagedObject::~CompanyServiceLocalSubgroupManagedObject()
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
 * Brief:CompanyServiceLocalSubgroupManagedObject::setsubgroupname:This function is used to set the value of member m_subgroupname
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::setsubgroupname (const string &subgroupname)
{
    m_subgroupname = subgroupname;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::getsubgroupname:This function is used to get the value of member m_subgroupname
 * @uBreezeClose
 */
string CompanyServiceLocalSubgroupManagedObject::getsubgroupname ()
{
    return (m_subgroupname);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::setsubgroupmanager:This function is used to set the value of member m_subgroupmanager
 * @uBreezeClose
 */
void CompanyServiceLocalSubgroupManagedObject::setsubgroupmanager (const WaveManagedObjectPointer<CompanyServiceLocalSubgroupmanagerManagedObject> &subgroupmanager)
{
    m_subgroupmanager = subgroupmanager;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalSubgroupManagedObject::getsubgroupmanager:This function is used to get the value of member m_subgroupmanager
 * @uBreezeClose
 */
WaveManagedObjectPointer<CompanyServiceLocalSubgroupmanagerManagedObject> CompanyServiceLocalSubgroupManagedObject::getsubgroupmanager ()
{
    return (m_subgroupmanager);
}



}

