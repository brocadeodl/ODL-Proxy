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


#ifndef COMPANYSERVICECITYMANAGEDOBJECT_H
#define COMPANYSERVICECITYMANAGEDOBJECT_H


#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Utils/BigInteger.h"



using namespace WaveNs;


namespace DcmNs{

class CompanyServiceObjectManager;


class CompanyServiceCityManagedObject : virtual public DcmManagedObject
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            CompanyServiceCityManagedObject                         (CompanyServiceObjectManager *pCompanyServiceObjectManager);
                                                            CompanyServiceCityManagedObject                         (CompanyServiceObjectManager *pCompanyServiceObjectManager,
                                                                                                                const string &cityname,
                                                                                                                const UI32 &citycode,
                                                                                                                const UI64 &population,
                                                                                                                const bool &ismetro,
                                                                                                                const IpV4Address &cityIpV4Address,
                                                                                                                const IpV4AddressNetworkMask &cityIpV4AddressNetworkMask,
                                                                                                                const IpVxAddress &cityIpV6Address,
                                                                                                                const IpV6AddressNetworkMask &cityIpV6AddressNetworkMask,
                                                                                                                const IpVxAddress &cityIpVxAddress);
                static string                               getClassName                                            ();
                virtual void                                setupKeys                                               ();
                virtual void                                isAssociatedManagedObjectToBeCreated                    (IsAssociatedManagedObjectToBeCreatedContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForCreate                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForUpdate                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForDelete                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForCreate                (GetHardwareConfigurationDetailsForCreateContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForUpdate                (GetHardwareConfigurationDetailsForUpdateContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForDelete                (GetHardwareConfigurationDetailsForDeleteContext *pContext);
                virtual void                                setHardwareConfigurationDetails                         (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap);
                virtual void                                setPostbootHardwareConfigurationDetails                 (map<UI32, vector<string> > &backendAttributesMap, map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap);
                virtual void                                getHardwareConfigurationDetailsForPostboot              (GetHardwareConfigurationDetailsForPostbootContext *pContext);
                virtual                                     ~CompanyServiceCityManagedObject                        ();
                void                                        setcityname                                             (const string &cityname);
                string                                      getcityname                                             ();
                void                                        setcitycode                                             (const UI32 &citycode);
                UI32                                        getcitycode                                             ();
                void                                        setpopulation                                           (const UI64 &population);
                UI64                                        getpopulation                                           ();
                void                                        setismetro                                              (const bool &ismetro);
                bool                                        getismetro                                              ();
                void                                        setcityIpV4Address                                      (const IpV4Address &cityIpV4Address);
                IpV4Address                                 getcityIpV4Address                                      ();
                void                                        setcityIpV4AddressNetworkMask                           (const IpV4AddressNetworkMask &cityIpV4AddressNetworkMask);
                IpV4AddressNetworkMask                      getcityIpV4AddressNetworkMask                           ();
                void                                        setcityIpV6Address                                      (const IpVxAddress &cityIpV6Address);
                IpVxAddress                                 getcityIpV6Address                                      ();
                void                                        setcityIpV6AddressNetworkMask                           (const IpV6AddressNetworkMask &cityIpV6AddressNetworkMask);
                IpV6AddressNetworkMask                      getcityIpV6AddressNetworkMask                           ();
                void                                        setcityIpVxAddress                                      (const IpVxAddress &cityIpVxAddress);
                IpVxAddress                                 getcityIpVxAddress                                      ();
    private   :
    protected :
    public    :
        string                                          m_cityname;
        UI32                                            m_citycode;
        UI64                                            m_population;
        bool                                            m_ismetro;
        IpV4Address                                     m_cityIpV4Address;
        IpV4AddressNetworkMask                          m_cityIpV4AddressNetworkMask;
        IpVxAddress                                     m_cityIpV6Address;
        IpV6AddressNetworkMask                          m_cityIpV6AddressNetworkMask;
        IpVxAddress                                     m_cityIpVxAddress;
};
 
}
#endif
