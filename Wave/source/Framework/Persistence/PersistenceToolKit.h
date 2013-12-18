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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef PERSISTENCETOOLKIT_H
#define PERSISTENCETOOLKIT_H

#include "Framework/Types/Types.h"
#include "Framework/Types/DateTime.h"
#include "Framework/Utils/PrismMutex.h"

using namespace std;

namespace WaveNs
{

class ConfigurationManagedObject;

class PersistenceToolKit
{
    private :
        static  void                        setConfigurationManagedObject                       (ConfigurationManagedObject* &pConfigurationManagedObject);
        static  ConfigurationManagedObject *getConfigurationManagedObject                       ();
        static  ResourceId                  getCachedConfigurationManagedObjectInformation      (UI64 &configurationTransactionId, DateTime &configurationTime, DateTime &latestUpdateTime);
        static  void                        clearConfigurationManagedObject                     ();

    protected :
    public :
        static  ResourceId                  getConfigurationManagedObjectInformation            (UI64 &configurationTransactionId, DateTime &configurationTime, DateTime &latestUpdateTime);
        static  ResourceId                  getConfigurationTransactionId                       (UI64 &configurationTransactionId);
        static  ResourceId                  getConfigurationTime                                (DateTime &configurationTime);
        static  ResourceId                  getLatestUpdateTime                                 (DateTime &latestUpdateTime);

    // Now the data members

    private :
        static  ConfigurationManagedObject *m_pConfigurationManagedObject;
        static  PrismMutex                  m_configurationManagedObjectMutex;

    protected :
    public :

    friend class PersistenceLocalObjectManager;
    friend class PrismFrameworkObjectManagerInitializeWorker;
};

}

#endif // PERSISTENCETOOLKIT_H
