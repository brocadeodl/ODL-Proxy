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




#include "CompanyService/Global/CompanyServiceCityManagedObject.h"
#include "CompanyService/Global/CompanyServiceTypes.h"
#include "CompanyService/Global/CompanyServiceObjectManager.h"
#include "CompanyService.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceCityManagedObject::CompanyServiceCityManagedObject(CompanyServiceObjectManager *pCompanyServiceObjectManager)
    :PrismElement  (pCompanyServiceObjectManager ),
    PrismPersistableObject(CompanyServiceCityManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceObjectManager),
    DcmManagedObject (pCompanyServiceObjectManager),
    m_cityname("")
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
 * Brief:CompanyServiceCityManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject:: setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeString (&m_cityname, true, "", "cityname",companyservice_cityname));
    addPersistableAttribute (new AttributeUI32 (&m_citycode, "citycode",companyservice_citycode));
    addPersistableAttribute (new AttributeUI64 (&m_population, "population",companyservice_population));
    addPersistableAttribute (new AttributeBool (&m_ismetro, "ismetro",companyservice_ismetro));
    addPersistableAttribute (new AttributeIpV4Address (&m_cityIpV4Address, "cityIpV4Address",companyservice_cityIpV4Address));
    addPersistableAttribute (new AttributeIpV4AddressNetworkMask (&m_cityIpV4AddressNetworkMask, "cityIpV4AddressNetworkMask",companyservice_cityIpV4AddressNetworkMask));
    addPersistableAttribute (new AttributeIpVxAddress (&m_cityIpV6Address, "cityIpV6Address",companyservice_cityIpV6Address));
    addPersistableAttribute (new AttributeIpV6AddressNetworkMask (&m_cityIpV6AddressNetworkMask, "cityIpV6AddressNetworkMask",companyservice_cityIpV6AddressNetworkMask));
    addPersistableAttribute (new AttributeIpVxAddress (&m_cityIpVxAddress, "cityIpVxAddress",companyservice_cityIpVxAddress));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject:: setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeString (&m_cityname, true, "", "cityname",companyservice_cityname));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_citycode, "citycode",companyservice_citycode));
    addPersistableAttributeForCreate (new AttributeUI64 (&m_population, "population",companyservice_population));
    addPersistableAttributeForCreate (new AttributeBool (&m_ismetro, "ismetro",companyservice_ismetro));
    addPersistableAttributeForCreate (new AttributeIpV4Address (&m_cityIpV4Address, "cityIpV4Address",companyservice_cityIpV4Address));
    addPersistableAttributeForCreate (new AttributeIpV4AddressNetworkMask (&m_cityIpV4AddressNetworkMask, "cityIpV4AddressNetworkMask",companyservice_cityIpV4AddressNetworkMask));
    addPersistableAttributeForCreate (new AttributeIpVxAddress (&m_cityIpV6Address, "cityIpV6Address",companyservice_cityIpV6Address));
    addPersistableAttributeForCreate (new AttributeIpV6AddressNetworkMask (&m_cityIpV6AddressNetworkMask, "cityIpV6AddressNetworkMask",companyservice_cityIpV6AddressNetworkMask));
    addPersistableAttributeForCreate (new AttributeIpVxAddress (&m_cityIpVxAddress, "cityIpVxAddress",companyservice_cityIpVxAddress));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("cityname");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
CompanyServiceCityManagedObject::CompanyServiceCityManagedObject(CompanyServiceObjectManager *pCompanyServiceObjectManager,
                                                                                                                const string &cityname,
                                                                                                                const UI32 &citycode,
                                                                                                                const UI64 &population,
                                                                                                                const bool &ismetro,
                                                                                                                const IpV4Address &cityIpV4Address,
                                                                                                                const IpV4AddressNetworkMask &cityIpV4AddressNetworkMask,
                                                                                                                const IpVxAddress &cityIpV6Address,
                                                                                                                const IpV6AddressNetworkMask &cityIpV6AddressNetworkMask,
                                                                                                                const IpVxAddress &cityIpVxAddress)
    :PrismElement  (pCompanyServiceObjectManager ),
    PrismPersistableObject(CompanyServiceCityManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceObjectManager),
    DcmManagedObject (pCompanyServiceObjectManager),
    m_cityname(cityname),
    m_citycode(citycode),
    m_population(population),
    m_ismetro(ismetro),
    m_cityIpV4Address(cityIpV4Address),
    m_cityIpV4AddressNetworkMask(cityIpV4AddressNetworkMask),
    m_cityIpV6Address(cityIpV6Address),
    m_cityIpV6AddressNetworkMask(cityIpV6AddressNetworkMask),
    m_cityIpVxAddress(cityIpVxAddress)
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
 * Brief:CompanyServiceCityManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceCityManagedObject::getClassName()
{
    return ("CompanyServiceCityManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceCityManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceCityManagedObject::~CompanyServiceCityManagedObject()
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
 * Brief:CompanyServiceCityManagedObject::setcityname:This function is used to set the value of member m_cityname
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setcityname (const string &cityname)
{
    m_cityname = cityname;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::getcityname:This function is used to get the value of member m_cityname
 * @uBreezeClose
 */
string CompanyServiceCityManagedObject::getcityname ()
{
    return (m_cityname);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setcitycode:This function is used to set the value of member m_citycode
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setcitycode (const UI32 &citycode)
{
    m_citycode = citycode;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::getcitycode:This function is used to get the value of member m_citycode
 * @uBreezeClose
 */
UI32 CompanyServiceCityManagedObject::getcitycode ()
{
    return (m_citycode);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setpopulation:This function is used to set the value of member m_population
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setpopulation (const UI64 &population)
{
    m_population = population;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::getpopulation:This function is used to get the value of member m_population
 * @uBreezeClose
 */
UI64 CompanyServiceCityManagedObject::getpopulation ()
{
    return (m_population);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setismetro:This function is used to set the value of member m_ismetro
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setismetro (const bool &ismetro)
{
    m_ismetro = ismetro;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::getismetro:This function is used to get the value of member m_ismetro
 * @uBreezeClose
 */
bool CompanyServiceCityManagedObject::getismetro ()
{
    return (m_ismetro);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setcityIpV4Address:This function is used to set the value of member m_cityIpV4Address
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setcityIpV4Address (const IpV4Address &cityIpV4Address)
{
    m_cityIpV4Address = cityIpV4Address;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::getcityIpV4Address:This function is used to get the value of member m_cityIpV4Address
 * @uBreezeClose
 */
IpV4Address CompanyServiceCityManagedObject::getcityIpV4Address ()
{
    return (m_cityIpV4Address);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setcityIpV4AddressNetworkMask:This function is used to set the value of member m_cityIpV4AddressNetworkMask
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setcityIpV4AddressNetworkMask (const IpV4AddressNetworkMask &cityIpV4AddressNetworkMask)
{
    m_cityIpV4AddressNetworkMask = cityIpV4AddressNetworkMask;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::getcityIpV4AddressNetworkMask:This function is used to get the value of member m_cityIpV4AddressNetworkMask
 * @uBreezeClose
 */
IpV4AddressNetworkMask CompanyServiceCityManagedObject::getcityIpV4AddressNetworkMask ()
{
    return (m_cityIpV4AddressNetworkMask);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setcityIpV6Address:This function is used to set the value of member m_cityIpV6Address
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setcityIpV6Address (const IpVxAddress &cityIpV6Address)
{
    m_cityIpV6Address = cityIpV6Address;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::getcityIpV6Address:This function is used to get the value of member m_cityIpV6Address
 * @uBreezeClose
 */
IpVxAddress CompanyServiceCityManagedObject::getcityIpV6Address ()
{
    return (m_cityIpV6Address);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setcityIpV6AddressNetworkMask:This function is used to set the value of member m_cityIpV6AddressNetworkMask
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setcityIpV6AddressNetworkMask (const IpV6AddressNetworkMask &cityIpV6AddressNetworkMask)
{
    m_cityIpV6AddressNetworkMask = cityIpV6AddressNetworkMask;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::getcityIpV6AddressNetworkMask:This function is used to get the value of member m_cityIpV6AddressNetworkMask
 * @uBreezeClose
 */
IpV6AddressNetworkMask CompanyServiceCityManagedObject::getcityIpV6AddressNetworkMask ()
{
    return (m_cityIpV6AddressNetworkMask);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::setcityIpVxAddress:This function is used to set the value of member m_cityIpVxAddress
 * @uBreezeClose
 */
void CompanyServiceCityManagedObject::setcityIpVxAddress (const IpVxAddress &cityIpVxAddress)
{
    m_cityIpVxAddress = cityIpVxAddress;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceCityManagedObject::getcityIpVxAddress:This function is used to get the value of member m_cityIpVxAddress
 * @uBreezeClose
 */
IpVxAddress CompanyServiceCityManagedObject::getcityIpVxAddress ()
{
    return (m_cityIpVxAddress);
}



}

