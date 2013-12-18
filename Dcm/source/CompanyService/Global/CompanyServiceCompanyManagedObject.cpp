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




#include "CompanyService/Global/CompanyServiceCompanyManagedObject.h"
#include "CompanyService/Global/CompanyServiceTypes.h"
#include "CompanyService/Global/CompanyServiceObjectManager.h"
#include "CompanyService.h"
#include "CompanyService/Global/CompanyServiceCityManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceCompanyManagedObject::CompanyServiceCompanyManagedObject(CompanyServiceObjectManager *pCompanyServiceObjectManager)
    :PrismElement  (pCompanyServiceObjectManager ),
    PrismPersistableObject(CompanyServiceCompanyManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceObjectManager),
    DcmManagedObject (pCompanyServiceObjectManager),
    m_companyname(""),
    m_region(189579809),
    m_region_EMEA_country(""),
    m_region_AsiaPac_country(""),
    m_region_Americas_country("")
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
 * Brief:CompanyServiceCompanyManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject:: setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeString (&m_companyname, true, "", "companyname",companyservice_companyname));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <CompanyServiceLocalEmployeeManagedObject> (&m_employee, "employee", getClassName(),getObjectId(),CompanyServiceLocalEmployeeManagedObject::getClassName(),companyservice_employee));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_location, "location", CompanyServiceCityManagedObject::getClassName(), true,companyservice_location));
    addPersistableAttribute (new AttributeUI32 (&m_region, "region",companyservice_region));
    addPersistableAttribute (new AttributeString (&m_region_EMEA_country, true, "", "region_EMEA_country",companyservice_EMEA_country));
    addPersistableAttribute (new AttributeString (&m_region_AsiaPac_country, true, "", "region_AsiaPac_country",companyservice_AsiaPac_country));
    addPersistableAttribute (new AttributeString (&m_region_Americas_country, true, "", "region_Americas_country",companyservice_Americas_country));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <CompanyServiceLocalGroupManagedObject> (&m_group, "group", getClassName(),getObjectId(),CompanyServiceLocalGroupManagedObject::getClassName(),companyservice_group));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <CompanyServiceBoardOfDirectorManagedObject> (&m_boardOfDirector, "boardOfDirector", getClassName(),getObjectId(),CompanyServiceBoardOfDirectorManagedObject::getClassName(),companyservice_boardOfDirector));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject:: setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeString (&m_companyname, true, "", "companyname",companyservice_companyname));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <CompanyServiceLocalEmployeeManagedObject> (&m_employee, "employee", getClassName(),getObjectId(),CompanyServiceLocalEmployeeManagedObject::getClassName(),companyservice_employee));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_location, "location", CompanyServiceCityManagedObject::getClassName(), true,companyservice_location));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_region, "region",companyservice_region));
    addPersistableAttributeForCreate (new AttributeString (&m_region_EMEA_country, true, "", "region_EMEA_country",companyservice_EMEA_country));
    addPersistableAttributeForCreate (new AttributeString (&m_region_AsiaPac_country, true, "", "region_AsiaPac_country",companyservice_AsiaPac_country));
    addPersistableAttributeForCreate (new AttributeString (&m_region_Americas_country, true, "", "region_Americas_country",companyservice_Americas_country));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <CompanyServiceLocalGroupManagedObject> (&m_group, "group", getClassName(),getObjectId(),CompanyServiceLocalGroupManagedObject::getClassName(),companyservice_group));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <CompanyServiceBoardOfDirectorManagedObject> (&m_boardOfDirector, "boardOfDirector", getClassName(),getObjectId(),CompanyServiceBoardOfDirectorManagedObject::getClassName(),companyservice_boardOfDirector));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("companyname");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
CompanyServiceCompanyManagedObject::CompanyServiceCompanyManagedObject(CompanyServiceObjectManager *pCompanyServiceObjectManager,
                                                                                                                const string &companyname,
                                                                                                                const vector<WaveManagedObjectPointer<CompanyServiceLocalEmployeeManagedObject> > &employee,
                                                                                                                const ObjectId &location,
                                                                                                                const UI32 &region,
                                                                                                                const string &region_EMEA_country,
                                                                                                                const string &region_AsiaPac_country,
                                                                                                                const string &region_Americas_country,
                                                                                                                const vector<WaveManagedObjectPointer<CompanyServiceLocalGroupManagedObject> > &group,
                                                                                                                const vector<WaveManagedObjectPointer<CompanyServiceBoardOfDirectorManagedObject> > &boardOfDirector)
    :PrismElement  (pCompanyServiceObjectManager ),
    PrismPersistableObject(CompanyServiceCompanyManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceObjectManager),
    DcmManagedObject (pCompanyServiceObjectManager),
    m_companyname(companyname),
    m_employee(employee),
    m_location(location),
    m_region(region),
    m_region_EMEA_country(region_EMEA_country),
    m_region_AsiaPac_country(region_AsiaPac_country),
    m_region_Americas_country(region_Americas_country),
    m_group(group),
    m_boardOfDirector(boardOfDirector)
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
 * Brief:CompanyServiceCompanyManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceCompanyManagedObject::getClassName()
{
    return ("CompanyServiceCompanyManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceCompanyManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceCompanyManagedObject::~CompanyServiceCompanyManagedObject()
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
 * Brief:CompanyServiceCompanyManagedObject::setcompanyname:This function is used to set the value of member m_companyname
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setcompanyname (const string &companyname)
{
    m_companyname = companyname;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getcompanyname:This function is used to get the value of member m_companyname
 * @uBreezeClose
 */
string CompanyServiceCompanyManagedObject::getcompanyname ()
{
    return (m_companyname);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setemployee:This function is used to set the value of member m_employee
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setemployee (const vector<WaveManagedObjectPointer<CompanyServiceLocalEmployeeManagedObject> > &employee)
{
    m_employee = employee;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getemployee:This function is used to get the value of member m_employee
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<CompanyServiceLocalEmployeeManagedObject> > CompanyServiceCompanyManagedObject::getemployee ()
{
    return (m_employee);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setlocation:This function is used to set the value of member m_location
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setlocation (const ObjectId &location)
{
    m_location = location;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getlocation:This function is used to get the value of member m_location
 * @uBreezeClose
 */
ObjectId CompanyServiceCompanyManagedObject::getlocation ()
{
    return (m_location);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setregion:This function is used to set the value of member m_region
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setregion (const UI32 &region)
{
    m_region = region;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getregion:This function is used to get the value of member m_region
 * @uBreezeClose
 */
UI32 CompanyServiceCompanyManagedObject::getregion ()
{
    return (m_region);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setregion_EMEA_country:This function is used to set the value of member m_region_EMEA_country
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setregion_EMEA_country (const string &region_EMEA_country)
{
    m_region_EMEA_country = region_EMEA_country;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getregion_EMEA_country:This function is used to get the value of member m_region_EMEA_country
 * @uBreezeClose
 */
string CompanyServiceCompanyManagedObject::getregion_EMEA_country ()
{
    return (m_region_EMEA_country);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setregion_AsiaPac_country:This function is used to set the value of member m_region_AsiaPac_country
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setregion_AsiaPac_country (const string &region_AsiaPac_country)
{
    m_region_AsiaPac_country = region_AsiaPac_country;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getregion_AsiaPac_country:This function is used to get the value of member m_region_AsiaPac_country
 * @uBreezeClose
 */
string CompanyServiceCompanyManagedObject::getregion_AsiaPac_country ()
{
    return (m_region_AsiaPac_country);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setregion_Americas_country:This function is used to set the value of member m_region_Americas_country
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setregion_Americas_country (const string &region_Americas_country)
{
    m_region_Americas_country = region_Americas_country;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getregion_Americas_country:This function is used to get the value of member m_region_Americas_country
 * @uBreezeClose
 */
string CompanyServiceCompanyManagedObject::getregion_Americas_country ()
{
    return (m_region_Americas_country);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setgroup:This function is used to set the value of member m_group
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setgroup (const vector<WaveManagedObjectPointer<CompanyServiceLocalGroupManagedObject> > &group)
{
    m_group = group;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getgroup:This function is used to get the value of member m_group
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<CompanyServiceLocalGroupManagedObject> > CompanyServiceCompanyManagedObject::getgroup ()
{
    return (m_group);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setstockExchange:This function is used to set the value of member m_stockExchange
 * @uBreezeClose
 */
/*
void CompanyServiceCompanyManagedObject::setstockExchange (const ObjectId &stockExchange)
{
    m_stockExchange = stockExchange;
}
*/

/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getstockExchange:This function is used to get the value of member m_stockExchange
 * @uBreezeClose
 */
/*
ObjectId CompanyServiceCompanyManagedObject::getstockExchange ()
{
    return (m_stockExchange);
}

*/
/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::setboardOfDirector:This function is used to set the value of member m_boardOfDirector
 * @uBreezeClose
 */
void CompanyServiceCompanyManagedObject::setboardOfDirector (const vector<WaveManagedObjectPointer<CompanyServiceBoardOfDirectorManagedObject> > &boardOfDirector)
{
    m_boardOfDirector = boardOfDirector;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCompanyManagedObject::getboardOfDirector:This function is used to get the value of member m_boardOfDirector
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<CompanyServiceBoardOfDirectorManagedObject> > CompanyServiceCompanyManagedObject::getboardOfDirector ()
{
    return (m_boardOfDirector);
}



}

