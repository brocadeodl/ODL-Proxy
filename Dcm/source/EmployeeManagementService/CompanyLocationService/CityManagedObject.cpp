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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 **************************************************************************/

#include "EmployeeManagementService/CompanyLocationService/CityManagedObject.h"
#include "EmployeeManagementService/CompanyLocationService/CityObjectManager.h"
#include "companyprofile.h"

namespace DcmNs
{

CityManagedObject::CityManagedObject (CityObjectManager *pCityObjectManager)
        : PrismElement           (pCityObjectManager),
          PrismPersistableObject (CityManagedObject::getClassName (), DcmManagedObject::getClassName ()),
          WaveManagedObject      (pCityObjectManager),
          DcmManagedObject       (pCityObjectManager)
{
}

CityManagedObject::~CityManagedObject ()
{
}

string CityManagedObject::getClassName ()
{
    return ("CityManagedObject");
}

void CityManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_cityName, "cityname", companyprofile_cityname));
    addPersistableAttribute (new AttributeUI32UC (&m_cityCode, false, true, 10, "citycode", companyprofile_citycode));
    addPersistableAttribute (new AttributeUI64 (&m_population, "population", companyprofile_population));
    addPersistableAttribute (new AttributeBoolUC (&m_isMetro, false, "true", "false", "ismetro", companyprofile_ismetro));
    addPersistableAttribute (new AttributeIpV4Address (&m_cityIpV4Address, "cityIpV4Address", companyprofile_cityIpV4Address));
    addPersistableAttribute (new AttributeIpV4AddressNetworkMask (&m_cityIpV4AddressNetworkMask, "cityIpV4AddressNetworkMask", companyprofile_cityIpV4AddressNetworkMask)); 
    addPersistableAttribute (new AttributeIpV6Address (&m_cityIpV6Address, "cityIpV6Address", companyprofile_cityIpV6Address)); 
    addPersistableAttribute (new AttributeIpV6AddressNetworkMask (&m_cityIpV6AddressNetworkMask, "cityIpV6AddressNetworkMask", companyprofile_cityIpV6AddressNetworkMask)); 
    addPersistableAttribute (new AttributeIpVxAddress (&m_cityIpVxAddress, "cityIpVxAddress", companyprofile_cityIpVxAddress));                                                                                                                                               
}


void CityManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString (&m_cityName, "cityname", companyprofile_cityname));
    addPersistableAttributeForCreate (new AttributeUI32UC (&m_cityCode, false, true, 10, "citycode", companyprofile_citycode));
    addPersistableAttributeForCreate (new AttributeUI64 (&m_population, "population", companyprofile_population));
    addPersistableAttributeForCreate (new AttributeBoolUC (&m_isMetro,false, "true", "false", "ismetro", companyprofile_ismetro));
    addPersistableAttributeForCreate (new AttributeIpV4Address (&m_cityIpV4Address, "cityIpV4Address", companyprofile_cityIpV4Address));  
    addPersistableAttributeForCreate (new AttributeIpV4AddressNetworkMask (&m_cityIpV4AddressNetworkMask, "cityIpV4AddressNetworkMask", companyprofile_cityIpV4AddressNetworkMask));
    addPersistableAttributeForCreate (new AttributeIpV6Address (&m_cityIpV6Address, "cityIpV6Address", companyprofile_cityIpV6Address));  
    addPersistableAttributeForCreate (new AttributeIpV6AddressNetworkMask (&m_cityIpV6AddressNetworkMask, "cityIpV6AddressNetworkMask", companyprofile_cityIpV6AddressNetworkMask)); 
    addPersistableAttributeForCreate (new AttributeIpVxAddress (&m_cityIpVxAddress, "cityIpVxAddress", companyprofile_cityIpVxAddress));

    vector<string > keyName;
    keyName.push_back ("cityname");
    setUserDefinedKeyCombination (keyName);
}

void CityManagedObject::setCityName (const string &cityName)
{
    m_cityName = cityName;
}

string CityManagedObject::getCityName () const
{
    return (m_cityName);
}

void CityManagedObject::setCityCode (const UI32 cityCode)
{
    m_cityCode.setIsUserConfigured( true );
    m_cityCode.setUI32Value(cityCode);
}

UI32 CityManagedObject::getCityCode () const
{
    return (m_cityCode.getUI32Value ());
}

void CityManagedObject::setPopulation (const UI64 &population)
{
    m_population = population;
}

UI64 CityManagedObject::getPopulation () const
{
    return (m_population);
}

void CityManagedObject::setIsMetro (const bool isMetro)
{
    m_isMetro.setIsUserConfigured( true );
    m_isMetro.setBoolValue(isMetro);
}

bool CityManagedObject::getIsMetro () const
{
    return (m_isMetro.getBoolValue());
}

void CityManagedObject::setCityIpV4Address (const IpV4Address cityIpV4Address)
{
    m_cityIpV4Address = cityIpV4Address;
}
IpV4Address CityManagedObject::getCityIpV4Address () const
{
    return m_cityIpV4Address;
}
void CityManagedObject::setCityIpV4AddressNetworkMask (const IpV4AddressNetworkMask cityIpV4AddressNetworkMask)
{
    m_cityIpV4AddressNetworkMask = cityIpV4AddressNetworkMask;
}
IpV4AddressNetworkMask CityManagedObject::getCityIpV4AddressNetworkMask () const
{
    return m_cityIpV4AddressNetworkMask;
}
void CityManagedObject::setCityIpV6Address (const IpV6Address cityIpV6Address)
{
    m_cityIpV6Address = cityIpV6Address;
}

IpV6Address  CityManagedObject::getCityIpV6Address () const
{
    return m_cityIpV6Address;
}

void CityManagedObject::setCityIpV6AddressNetworkMask (const IpV6AddressNetworkMask cityIpV6AddressNetworkMask)
{
    m_cityIpV6AddressNetworkMask = cityIpV6AddressNetworkMask;
}

IpV6AddressNetworkMask  CityManagedObject::getCityIpV6AddressNetworkMask () const
{
    return m_cityIpV6AddressNetworkMask;
}
void CityManagedObject::setCityIpVxAddress (const IpVxAddress cityIpVxAddress)
{
    m_cityIpVxAddress = cityIpVxAddress;
}
IpVxAddress CityManagedObject::getCityIpVxAddress () const
{
    return m_cityIpVxAddress;
}


void CityManagedObject::validateForDeleteAtThisNode (ValidateForUpdateAtThisNodeContext *pContext)
{
    vector<Attribute*> attributes           = pContext->getAttributes ();
    UI32               numberOfAttributes   = attributes.size ();

    for (UI32 i = 0; i < numberOfAttributes; i++)
    {
        Attribute *pAttribute = attributes[i];

        trace (TRACE_LEVEL_INFO, "CityManagedObject::validateForDeleteAtThisNode attribute name " + pAttribute->getAttributeName ());
    }
    
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void CityManagedObject::getHardwareConfigurationDetailsForCreate (GetHardwareConfigurationDetailsForCreateContext *pContext)
{
    map <string, UI32>              attributeNamesGroupCodeMap;
    map <UI32, string>              groupCodeBackendNamesMap;
    map <UI32, vector <string> >    groupCodeAttributeGroupMap;
    map <string, string>            attributeNameBackendNameMap;

    getAttributeGroupsInformation (attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap, attributeNameBackendNameMap);

    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setAttributeNameBackendNameMap (attributeNameBackendNameMap);

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void CityManagedObject::getHardwareConfigurationDetailsForUpdate (GetHardwareConfigurationDetailsForUpdateContext *pContext)
{
    map <string, UI32>              attributeNamesGroupCodeMap;
    map <UI32, string>              groupCodeBackendNamesMap;
    map <UI32, vector <string> >    groupCodeAttributeGroupMap;
    map <string, string>            attributeNameBackendNameMap;

    getAttributeGroupsInformation (attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap, attributeNameBackendNameMap);

    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setAttributeNameBackendNameMap (attributeNameBackendNameMap);
    
    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void CityManagedObject::getHardwareConfigurationDetailsForDelete (GetHardwareConfigurationDetailsForDeleteContext *pContext)
{
    map <string, UI32>              attributeNamesGroupCodeMap;
    map <UI32, string>              groupCodeBackendNamesMap;
    map <UI32, vector <string> >    groupCodeAttributeGroupMap;
    map <string, string>            attributeNameBackendNameMap;

    getAttributeGroupsInformation (attributeNamesGroupCodeMap, groupCodeBackendNamesMap, groupCodeAttributeGroupMap, attributeNameBackendNameMap);

    pContext->setAttributeNamesGroupCodeMap (attributeNamesGroupCodeMap);
    pContext->setGroupCodeBackendNamesMap (groupCodeBackendNamesMap);
    pContext->setGroupCodeAttributeGroupMap (groupCodeAttributeGroupMap);
    pContext->setAttributeNameBackendNameMap (attributeNameBackendNameMap);

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

void CityManagedObject::getHardwareConfigurationDetailsForPostboot (GetHardwareConfigurationDetailsForPostbootContext *pContext)
{   
    // Generate a backendAttributesMap 
    map<UI32, vector<string> > backendAttributesMap;
    map<UI32, string > groupMap;
    
    //it has group-codes and corresponding type
    map<UI32, bool> groupCodeChoiceFlag;

    // map< choice-group, map < User-tag, vector < case-groups > > >
    //map<UI32, map<UI32, vector<UI32> > > choiceCaseGroup;    

    vector<string> attributeNames;

    attributeNames.push_back("cityname");
    groupMap[21] = "employee_management";
    groupCodeChoiceFlag[21] = false;
    backendAttributesMap[21] = attributeNames;
    attributeNames.clear();

    attributeNames.push_back("citycode");
    attributeNames.push_back("ismetro");    
    groupMap[22] = "employee_management";
    groupCodeChoiceFlag[22] = false;
    backendAttributesMap[22] = attributeNames;

    pContext->setIsUpdateBackend (true);
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    trace (TRACE_LEVEL_DEVEL, string("CityManagedObject::getHardwareConfigurationDetailsForPostboot: Map size is ") + backendAttributesMap.size() + " groupMap size is " + groupMap.size() + " passName is " + (pContext->getPassName()).c_str());
    pContext->setGroupCodeAttributeGroupMap( backendAttributesMap );
    pContext->setGroupCodeBackendNamesMap( groupMap );
    pContext->setGroupCodeChoiceFlag( groupCodeChoiceFlag );
    pContext->callback();

}

void CityManagedObject::getAttributeGroupsInformation (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap, map <string, string> &attributeNameBackendNameMap)
{
    //Populate the Map of attributeNames and corresponding groupCode
    attributeNamesGroupCodeMap ["cityname"] = 1;
    attributeNamesGroupCodeMap ["population"] = 2;
    attributeNamesGroupCodeMap ["ismetro"] = 2;
    attributeNamesGroupCodeMap ["citycode"] = 2;
    attributeNamesGroupCodeMap ["cityIpV4Address"] = 3;
    attributeNamesGroupCodeMap ["cityIpV6Address"] = 3;
    attributeNamesGroupCodeMap ["cityIpVxAddress"] = 3;
    attributeNamesGroupCodeMap ["cityIpV4AddressNetworkMask"] = 3;
    attributeNamesGroupCodeMap ["cityIpV6AddressNetworkMask"] = 3;

    //Populate the Map of groupCodes and backendNames
    groupCodeBackendNamesMap[1] = "employee_management";
    groupCodeBackendNamesMap[2] = "employee_management";
    groupCodeBackendNamesMap[3] = "employee_management";

    //Populate the Map of groupCodes and attributeGroup
    vector <string> attributeNamesVector;

    attributeNamesVector.push_back ("cityname");
    groupCodeAttributeGroupMap[1] = attributeNamesVector;

    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("population");
    attributeNamesVector.push_back ("ismetro");
    attributeNamesVector.push_back ("citycode");
    groupCodeAttributeGroupMap[2] = attributeNamesVector;
  
    attributeNameBackendNameMap["citycode"] = "employee_management";
   
    attributeNamesVector.clear ();
    attributeNamesVector.push_back ("cityIpV4Address");
    attributeNamesVector.push_back ("cityIpV6Address");
    attributeNamesVector.push_back ("cityIpVxAddress");
    attributeNamesVector.push_back ("cityIpV4AddressNetworkMask");
    attributeNamesVector.push_back ("cityIpV6AddressNetworkMask");
    groupCodeAttributeGroupMap[3] = attributeNamesVector;

    attributeNameBackendNameMap["cityIpV4Address"] = "employee_management";
}

bool CityManagedObject::updateHardwareBeforeWaveManagedObject (const vector<Attribute *> &attributeVector)
{

    UI32                numberOfAttributes  = attributeVector.size ();
    Attribute          *pAttribute          = NULL;

    for (UI32 i = 0; i < numberOfAttributes; i++)
    {
        pAttribute = attributeVector [i];

        trace (TRACE_LEVEL_DEBUG, "CityManagedObject::updateHardwareBeforeWaveManagedObject Attribute Name: " + pAttribute->getAttributeName ());

        if ("citycode" == pAttribute->getAttributeName ())
        {
             return (false);
        }
    }

    return (true);
}

void CityManagedObject::addNewAttributeToBeUpdatedForCreate (UpdateNewAttributeInManagedObjectContext *pContext)
{
#if 0    
    vector<Attribute *> attributeVector     = pContext->getAttributes ();
    UI32                numberOfAttribute   = attributeVector.size (); 

    for (UI32 i = 0; i < numberOfAttribute; i++)
    {
        Attribute *pAttribute = attributeVector [i];

        if ("cityname" == pAttribute->getAttributeName ())
        {
            setCityCode (95134);

            pContext->addNewAttributeName ("citycode");
        }
    }
#endif
    pContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pContext->callback ();
}

bool CityManagedObject::getPluginDetailsForDistribution (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag)
{   
    prismServiceId = FrameworkToolKit::getServiceIdByName (CityLocalObjectManager::getClassName ());
    return (true);
}


}
