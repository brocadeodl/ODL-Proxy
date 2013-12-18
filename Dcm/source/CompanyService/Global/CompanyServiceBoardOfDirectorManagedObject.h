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


#ifndef COMPANYSERVICEBOARDOFDIRECTORMANAGEDOBJECT_H
#define COMPANYSERVICEBOARDOFDIRECTORMANAGEDOBJECT_H


#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/Attributes.h"



using namespace WaveNs;


namespace DcmNs{

class CompanyServiceObjectManager;
class CompanyServiceCityManagedObject;


class CompanyServiceBoardOfDirectorManagedObject : virtual public DcmManagedObject
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            CompanyServiceBoardOfDirectorManagedObject              (CompanyServiceObjectManager *pCompanyServiceObjectManager);
                                                            CompanyServiceBoardOfDirectorManagedObject              (CompanyServiceObjectManager *pCompanyServiceObjectManager,
                                                                                                                const string &boardOfDirectorname,
                                                                                                                const ObjectId &boardOfDirectorlocation);
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
                virtual                                     ~CompanyServiceBoardOfDirectorManagedObject             ();
                void                                        setboardOfDirectorname                                  (const string &boardOfDirectorname);
                string                                      getboardOfDirectorname                                  ();
                void                                        setboardOfDirectorlocation                              (const ObjectId &boardOfDirectorlocation);
                ObjectId                                    getboardOfDirectorlocation                              ();
    private   :
    protected :
    public    :
        string                                          m_boardOfDirectorname;
        ObjectId                                        m_boardOfDirectorlocation;
};
 
}
#endif