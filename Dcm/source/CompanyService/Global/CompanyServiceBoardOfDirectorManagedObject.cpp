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




#include "CompanyService/Global/CompanyServiceBoardOfDirectorManagedObject.h"
#include "CompanyService/Global/CompanyServiceTypes.h"
#include "CompanyService/Global/CompanyServiceObjectManager.h"
#include "CompanyService.h"
#include "CompanyService/Global/CompanyServiceCityManagedObject.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceBoardOfDirectorManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceBoardOfDirectorManagedObject::CompanyServiceBoardOfDirectorManagedObject(CompanyServiceObjectManager *pCompanyServiceObjectManager)
    :PrismElement  (pCompanyServiceObjectManager ),
    PrismPersistableObject(CompanyServiceBoardOfDirectorManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceObjectManager),
    DcmManagedObject (pCompanyServiceObjectManager),
    m_boardOfDirectorname("")
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject:: setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeString (&m_boardOfDirectorname, true, "", "boardOfDirectorname",companyservice_boardOfDirectorname));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_boardOfDirectorlocation, "boardOfDirectorlocation", CompanyServiceCityManagedObject::getClassName(), true,companyservice_boardOfDirectorlocation));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceBoardOfDirectorManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject:: setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeString (&m_boardOfDirectorname, true, "", "boardOfDirectorname",companyservice_boardOfDirectorname));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_boardOfDirectorlocation, "boardOfDirectorlocation", CompanyServiceCityManagedObject::getClassName(), true,companyservice_boardOfDirectorlocation));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceBoardOfDirectorManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("boardOfDirectorname");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceBoardOfDirectorManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
CompanyServiceBoardOfDirectorManagedObject::CompanyServiceBoardOfDirectorManagedObject(CompanyServiceObjectManager *pCompanyServiceObjectManager,
                                                                                                                const string &boardOfDirectorname,
                                                                                                                const ObjectId &boardOfDirectorlocation)
    :PrismElement  (pCompanyServiceObjectManager ),
    PrismPersistableObject(CompanyServiceBoardOfDirectorManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceObjectManager),
    DcmManagedObject (pCompanyServiceObjectManager),
    m_boardOfDirectorname(boardOfDirectorname),
    m_boardOfDirectorlocation(boardOfDirectorlocation)
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceBoardOfDirectorManagedObject::getClassName()
{
    return ("CompanyServiceBoardOfDirectorManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceBoardOfDirectorManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceBoardOfDirectorManagedObject::~CompanyServiceBoardOfDirectorManagedObject()
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
 * Brief:CompanyServiceBoardOfDirectorManagedObject::setboardOfDirectorname:This function is used to set the value of member m_boardOfDirectorname
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::setboardOfDirectorname (const string &boardOfDirectorname)
{
    m_boardOfDirectorname = boardOfDirectorname;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceBoardOfDirectorManagedObject::getboardOfDirectorname:This function is used to get the value of member m_boardOfDirectorname
 * @uBreezeClose
 */
string CompanyServiceBoardOfDirectorManagedObject::getboardOfDirectorname ()
{
    return (m_boardOfDirectorname);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceBoardOfDirectorManagedObject::setboardOfDirectorlocation:This function is used to set the value of member m_boardOfDirectorlocation
 * @uBreezeClose
 */
void CompanyServiceBoardOfDirectorManagedObject::setboardOfDirectorlocation (const ObjectId &boardOfDirectorlocation)
{
    m_boardOfDirectorlocation = boardOfDirectorlocation;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceBoardOfDirectorManagedObject::getboardOfDirectorlocation:This function is used to get the value of member m_boardOfDirectorlocation
 * @uBreezeClose
 */
ObjectId CompanyServiceBoardOfDirectorManagedObject::getboardOfDirectorlocation ()
{
    return (m_boardOfDirectorlocation);
}



}

