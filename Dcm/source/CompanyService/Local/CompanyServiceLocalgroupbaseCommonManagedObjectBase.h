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


#ifndef COMPANYSERVICELOCALGROUPBASECOMMONMANAGEDOBJECTBASE_H
#define COMPANYSERVICELOCALGROUPBASECOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Attributes/Attributes.h"



using namespace WaveNs;


namespace DcmNs{
class CompanyServiceLocalEmployeeManagedObject;
class CompanyServiceCityManagedObject;


class CompanyServiceLocalgroupbaseCommonManagedObjectBase : virtual public CommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            CompanyServiceLocalgroupbaseCommonManagedObjectBase     (PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~CompanyServiceLocalgroupbaseCommonManagedObjectBase    ();
                void                                        setgroupEmployee                                        (const ObjectId &groupEmployee);
                ObjectId                                    getgroupEmployee                                        ();
                void                                        setgroupLocation                                        (const ObjectId &groupLocation);
                ObjectId                                    getgroupLocation                                        ();
    private   :
    protected :
    public    :
        ObjectId                                        m_groupEmployee;
        ObjectId                                        m_groupLocation;
};
 
}
#endif
