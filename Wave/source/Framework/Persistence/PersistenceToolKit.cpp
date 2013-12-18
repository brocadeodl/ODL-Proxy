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

#include "Framework/Persistence/PersistenceToolKit.h"
#include "Framework/Persistence/ConfigurationManagedObject.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "WaveResourceIdEnums.h"

namespace WaveNs
{

ConfigurationManagedObject *PersistenceToolKit::m_pConfigurationManagedObject       = NULL;
PrismMutex                  PersistenceToolKit::m_configurationManagedObjectMutex;

void PersistenceToolKit::setConfigurationManagedObject (ConfigurationManagedObject* &pConfigurationManagedObject)
{
    m_configurationManagedObjectMutex.lock ();
    
    m_pConfigurationManagedObject = pConfigurationManagedObject; 

    m_configurationManagedObjectMutex.unlock ();
}

void PersistenceToolKit::clearConfigurationManagedObject ()
{
    m_configurationManagedObjectMutex.lock ();
    
    delete (m_pConfigurationManagedObject);

    m_pConfigurationManagedObject = NULL;
    
    m_configurationManagedObjectMutex.unlock ();
}

ConfigurationManagedObject *PersistenceToolKit::getConfigurationManagedObject ()
{
    m_configurationManagedObjectMutex.lock ();

    ConfigurationManagedObject *pConfigurationManagedObject = m_pConfigurationManagedObject;

    m_configurationManagedObjectMutex.unlock ();

    return (pConfigurationManagedObject);
}

ResourceId PersistenceToolKit::getCachedConfigurationManagedObjectInformation (UI64 &configurationTransactionId, DateTime &configurationTime, DateTime &lastestUpdateTime)
{
    ResourceId status = FRAMEWORK_SUCCESS;

    m_configurationManagedObjectMutex.lock ();

    if (NULL != m_pConfigurationManagedObject)
    {
        configurationTransactionId  = m_pConfigurationManagedObject->getConfigurationNumber ();
        configurationTime           = m_pConfigurationManagedObject->getConfigurationTime ();
        lastestUpdateTime           = m_pConfigurationManagedObject->getLatestUpdateTime ();

        status = FRAMEWORK_SUCCESS;
    }
    else
    {
        status = WAVE_PERSISTENCE_IN_MEMORY_CONFIGURATION_MANAGED_OBJECT_NOT_FOUND;
    }

    m_configurationManagedObjectMutex.unlock ();

    return (status);
}

ResourceId PersistenceToolKit::getConfigurationManagedObjectInformation (UI64 &configurationTransactionId, DateTime &configurationTime, DateTime &latestUpdateTime)
{
    vector<WaveManagedObject *>    *pResults                    = NULL;
    ConfigurationManagedObject     *pConfigurationManagedObject = NULL; 
    ResourceId                      status                      = WAVE_MESSAGE_SUCCESS;

    // Query for the Global MO "ConfigurationManagedObject" 

    pResults = WaveObjectManagerToolKit::querySynchronously (ConfigurationManagedObject::getClassName ());
    prismAssert (NULL != pResults, __FILE__, __LINE__);

    if (NULL != pResults)
    {
        prismAssert (0 == (pResults->size ()) || 1 == (pResults->size ()), __FILE__, __LINE__);

        if (1 == pResults->size ())
        {
            // Get the configuration number from queried result

            pConfigurationManagedObject = dynamic_cast<ConfigurationManagedObject *> ((*pResults)[0]);
            prismAssert (NULL != pConfigurationManagedObject, __FILE__, __LINE__);

            configurationTransactionId  = pConfigurationManagedObject->getConfigurationNumber ();
            configurationTime           = pConfigurationManagedObject->getConfigurationTime ();
            latestUpdateTime            = pConfigurationManagedObject->getLatestUpdateTime ();

            status = WAVE_MESSAGE_SUCCESS;
        }
        else if (0 == pResults->size ())
        {
            // The query return an empty result.  In this case get the cached configuration managed object information.

            status = PersistenceToolKit::getCachedConfigurationManagedObjectInformation (configurationTransactionId, configurationTime, latestUpdateTime);

            if (FRAMEWORK_SUCCESS == status)
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "PersistenceToolKit::getConfigurationManagedObjectInformation : Query for the global configuration managed object returned an empty result.  Also, the cached configuration managed object information returned with status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            status = WAVE_PERSISTENCE_CONFIGURATION_MANAGED_OBJECT_QUERY_FAILED;
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
    }
    else
    {
        status = WAVE_PERSISTENCE_CONFIGURATION_MANAGED_OBJECT_QUERY_FAILED;
    }

    return (status);
}

ResourceId PersistenceToolKit::getConfigurationTransactionId (UI64 &configurationTransactionId)
{
    DateTime    tempConfigurationTime           ("1970-01-01 00:00:00.0+0");
    DateTime    tempLatestUpdateTime            ("1970-01-01 00:00:00.0+0");

    ResourceId status = PersistenceToolKit::getConfigurationManagedObjectInformation (configurationTransactionId, tempConfigurationTime, tempLatestUpdateTime);

    return (status);
}

ResourceId PersistenceToolKit::getConfigurationTime (DateTime &configurationTime)
{
    UI64        tempConfigurationTransactionId  = 0;
    DateTime    tempLatestUpdateTime            ("1970-01-01 00:00:00.0+0");

    ResourceId status = PersistenceToolKit::getConfigurationManagedObjectInformation (tempConfigurationTransactionId, configurationTime, tempLatestUpdateTime);

    return (status);
}

ResourceId PersistenceToolKit::getLatestUpdateTime (DateTime &latestUpdateTime)
{
    UI64        tempConfigurationTransactionId  = 0;
    DateTime    tempConfigurationTime           ("1970-01-01 00:00:00.0+0");

    ResourceId status = PersistenceToolKit::getConfigurationManagedObjectInformation (tempConfigurationTransactionId, tempConfigurationTime, latestUpdateTime);

    return (status);
}

}
