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


#ifndef COMPANYSERVICECOMPANYMANAGEDOBJECT_H
#define COMPANYSERVICECOMPANYMANAGEDOBJECT_H


#include "DcmObjectModel/DcmManagedObject.h"
#include "CompanyService/Global/CompanyServiceBoardOfDirectorManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalEmployeeManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalGroupManagedObject.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"



using namespace WaveNs;


namespace DcmNs{

class CompanyServiceObjectManager;
class CompanyServiceLocalEmployeeManagedObject;
class CompanyServiceCityManagedObject;
class CompanyServiceLocalGroupManagedObject;
class CompanyServiceStockExchangeManagedObject;
class CompanyServiceBoardOfDirectorManagedObject;


class CompanyServiceCompanyManagedObject : virtual public DcmManagedObject
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            CompanyServiceCompanyManagedObject                      (CompanyServiceObjectManager *pCompanyServiceObjectManager);
                                                            CompanyServiceCompanyManagedObject                      (CompanyServiceObjectManager *pCompanyServiceObjectManager,
                                                                                                                const string &companyname,
                                                                                                                const vector<WaveManagedObjectPointer<CompanyServiceLocalEmployeeManagedObject> > &employee,
                                                                                                                const ObjectId &location,
                                                                                                                const UI32 &region,
                                                                                                                const string &region_EMEA_country,
                                                                                                                const string &region_AsiaPac_country,
                                                                                                                const string &region_Americas_country,
                                                                                                                const vector<WaveManagedObjectPointer<CompanyServiceLocalGroupManagedObject> > &group,
                                                                                                                const vector<WaveManagedObjectPointer<CompanyServiceBoardOfDirectorManagedObject> > &boardOfDirector);
                static string                               getClassName                                            ();
                virtual void                                setupKeys                                               ();
                virtual bool                                getPluginDetailsForDistribution                         (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag);
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
                virtual                                     ~CompanyServiceCompanyManagedObject                     ();
                void                                        setcompanyname                                          (const string &companyname);
                string                                      getcompanyname                                          ();
                void                                        setemployee                                             (const vector<WaveManagedObjectPointer<CompanyServiceLocalEmployeeManagedObject> > &employee);
                vector<WaveManagedObjectPointer<CompanyServiceLocalEmployeeManagedObject> > getemployee                                             ();
                void                                        setlocation                                             (const ObjectId &location);
                ObjectId                                    getlocation                                             ();
                void                                        setregion                                               (const UI32 &region);
                UI32                                        getregion                                               ();
                void                                        setregion_EMEA_country                                  (const string &region_EMEA_country);
                string                                      getregion_EMEA_country                                  ();
                void                                        setregion_AsiaPac_country                               (const string &region_AsiaPac_country);
                string                                      getregion_AsiaPac_country                               ();
                void                                        setregion_Americas_country                              (const string &region_Americas_country);
                string                                      getregion_Americas_country                              ();
                void                                        setgroup                                                (const vector<WaveManagedObjectPointer<CompanyServiceLocalGroupManagedObject> > &group);
                vector<WaveManagedObjectPointer<CompanyServiceLocalGroupManagedObject> > getgroup                                                ();
                void                                        setstockExchange                                        (const ObjectId &stockExchange);
                ObjectId                                    getstockExchange                                        ();
                void                                        setboardOfDirector                                      (const vector<WaveManagedObjectPointer<CompanyServiceBoardOfDirectorManagedObject> > &boardOfDirector);
                vector<WaveManagedObjectPointer<CompanyServiceBoardOfDirectorManagedObject> > getboardOfDirector                                      ();
    private   :
    protected :
    public    :
        string                                          m_companyname;
        vector<WaveManagedObjectPointer<CompanyServiceLocalEmployeeManagedObject> > m_employee;
        ObjectId                                        m_location;
        UI32                                            m_region;
        string                                          m_region_EMEA_country;
        string                                          m_region_AsiaPac_country;
        string                                          m_region_Americas_country;
        vector<WaveManagedObjectPointer<CompanyServiceLocalGroupManagedObject> > m_group;
        ObjectId                                        m_stockExchange;
        vector<WaveManagedObjectPointer<CompanyServiceBoardOfDirectorManagedObject> > m_boardOfDirector;
};
 
}
#endif
