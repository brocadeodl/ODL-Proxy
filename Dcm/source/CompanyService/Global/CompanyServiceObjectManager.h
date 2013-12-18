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


#ifndef COMPANYSERVICEOBJECTMANAGER_H
#define COMPANYSERVICEOBJECTMANAGER_H



#include "Framework/ObjectModel/WaveObjectManager.h"


using namespace WaveNs;


namespace DcmNs{

class CompanyServicecompanyWorker;

class CompanyServiceObjectManager : public WaveObjectManager
{
    private   :
                PrismMessage                                *createMessageInstance                                  (const UI32 &operationCode);
                WaveManagedObject                           *createManagedObjectInstance                            (const string &managedClassName);
                                                            CompanyServiceObjectManager                             ();
    protected :
    public    :
                virtual                                     ~CompanyServiceObjectManager                            ();
                static string                               getClassName                                            ();
                static CompanyServiceObjectManager          *getInstance                                            ();
                static PrismServiceId                       getPrismServiceId                                       ();
                virtual void                                setPostbootPhases                                       ();
    private   :
    protected :
    public    :
        CompanyServicecompanyWorker                     *m_pCompanyServicecompanyWorker;
};
 
}
#endif
