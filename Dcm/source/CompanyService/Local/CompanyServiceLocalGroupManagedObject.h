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


#ifndef COMPANYSERVICELOCALGROUPMANAGEDOBJECT_H
#define COMPANYSERVICELOCALGROUPMANAGEDOBJECT_H


#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalGroupmanagerManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalSubgroupManagedObject.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"

#include "CompanyService/Local/CompanyServiceLocalgroupbaseCommonManagedObjectBase.h"


using namespace WaveNs;


namespace DcmNs{

class CompanyServiceLocalObjectManager;
class CompanyServiceLocalSubgroupManagedObject;
class CompanyServiceLocalGroupmanagerManagedObject;


class CompanyServiceLocalGroupManagedObject : virtual public DcmLocalManagedObject ,virtual public CompanyServiceLocalgroupbaseCommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            CompanyServiceLocalGroupManagedObject                   (CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager);
                                                            CompanyServiceLocalGroupManagedObject                   (CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager,
                                                                                                                const vector<WaveManagedObjectPointer<CompanyServiceLocalSubgroupManagedObject> > &subgroup,
                                                                                                                const string &groupname,
                                                                                                                const WaveManagedObjectPointer<CompanyServiceLocalGroupmanagerManagedObject> &groupmanager);
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
                virtual                                     ~CompanyServiceLocalGroupManagedObject                  ();
                void                                        setsubgroup                                             (const vector<WaveManagedObjectPointer<CompanyServiceLocalSubgroupManagedObject> > &subgroup);
                vector<WaveManagedObjectPointer<CompanyServiceLocalSubgroupManagedObject> > getsubgroup                                             ();
                void                                        setgroupname                                            (const string &groupname);
                string                                      getgroupname                                            ();
                void                                        setgroupmanager                                         (const WaveManagedObjectPointer<CompanyServiceLocalGroupmanagerManagedObject> &groupmanager);
                WaveManagedObjectPointer<CompanyServiceLocalGroupmanagerManagedObject> getgroupmanager                                         ();
    private   :
    protected :
    public    :
        vector<WaveManagedObjectPointer<CompanyServiceLocalSubgroupManagedObject> > m_subgroup;
        string                                          m_groupname;
        WaveManagedObjectPointer<CompanyServiceLocalGroupmanagerManagedObject> m_groupmanager;
};
 
}
#endif
