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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PERSISTENCEEXECUTETRANSACTIONCONTEXT_H
#define PERSISTENCEEXECUTETRANSACTIONCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/Types/Types.h"
#include "Framework/Persistence/ConfigurationManagedObject.h"
#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class PersistenceObjectManagerExecuteTransactionMessage;

class PersistenceExecuteTransactionContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                                      PersistenceExecuteTransactionContext        (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps, const string &sql);
                                                      //PersistenceExecuteTransactionContext        (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);

        virtual                                ~PersistenceExecuteTransactionContext        ();

                vector<LocationId>             &getCurrentLocationIds                       ();
                map<LocationId, ResourceId>    &getTransactionStatusForCurrentLocations     ();

                UI32                            getCurrentTransactionId                     () const;
                void                            setCurrentTransactionId                     (const UI32 &currentTransactionId);
                string                          getSourceSchema                             () const;
                void                            setSourceSchema                             (const string &sourceSchema);
                string                          getDestSchema                               () const;
                void                            setDestSchema                               (const string &destSchema);
                string                          getStartupFileName                          () const;
                void                            setStartupFileName                          (const string &startupFileName);
                ResourceId                      getStartupFileType                          () const;
                void                            setStartupFileType                          (const ResourceId &startupFileType);
                bool                            getIsStartupValid                           () const;
                void                            setIsStartupValid                           (const bool &isStartupValid);
                bool                            getIsLocalFileCopy                          () const;
                void                            setIsLocalFileCopy                          (const bool &isLocalFileCopy);
                void                            setConfigurationManagedObject               (ConfigurationManagedObject *pConfigurationManagedObject);
                ConfigurationManagedObject     *getConfigurationManagedObject               ();
                vector<string>                 &getxPathStringsVectorForTimestampUpdate     ();
                map<string, DateTime>          &getxPathStringToOriginalDatetimestampMap    ();
                const string                         &getSql                                      () const;
                      //void                            setSql                                      (const string &sql);
                bool                            getIsConfigurationChange                    () const;
                void                            setIsConfigurationChange                    (const bool &isConfigurationChange);
                bool                            getIsConfigurationTimeChange                () const;
                void                            setIsConfigurationTimeChange                (const bool &isConfigurartionTimeChange);
                PrismServiceId                  getSenderServiceCode                        () const;
                void                            setSenderServiceCode                        (const PrismServiceId &senderServiceCode);
                LocationId                      getSenderLocationId                         () const;
                void                            setSenderLocationId                         (const LocationId &senderLocationId);
  
                void                            setConfigNamesForInsert                     (const vector<string> &configNamesForInsert);
                vector<string>                  getConfigNamesForInsert                     () const;
                void                            setConfigValuesForInsert                    (const vector<string> &configValuesForInsert);
                vector<string>                  getConfigValuesForInsert                    () const;
                void                            setConfigTypesForInsert                     (const vector<ResourceId> &configTypesForInsert);
                vector<ResourceId>              getConfigTypesForInsert                     () const;

                void                            setConfigNamesForUpdate                     (const vector<string> &configNamesForUpdate);
                vector<string>                  getConfigNamesForUpdate                     () const;
                void                            setConfigValuesForUpdate                    (const vector<string> &configValuesForUpdate);
                vector<string>                  getConfigValuesForUpdate                    () const;
                void                            setConfigTypesForUpdate                     (const vector<ResourceId> &configTypesForUpdate);
                vector<ResourceId>              getConfigTypesForUpdate                     () const;
  
  
    // now the data members

    private :
        vector<LocationId>          m_currentLocationIds;
        map<LocationId, ResourceId> m_transactionStatusForCurrentLocations;
        UI32                        m_currentTransactionId;
        string                      m_sourceSchema;
        string                      m_destSchema;
        string                      m_startupFileName;
        ResourceId                  m_startupFileType;
        bool                        m_isStartupValid;
        bool                        m_isLocalFileCopy;
        ConfigurationManagedObject *m_pConfigurationManagedObject;
        vector<string>              m_xPathStringsVectorForTimestampUpdate;
        map<string, DateTime>       m_xPathStringToOriginalDatetimestampMap;
        const string                      &m_sql;
        bool                        m_isConfigurationChange;
        bool                        m_isConfigurationTimeChange;
        PrismServiceId              m_senderServiceCode;
        LocationId                  m_senderLocationId;

        vector<string>              m_configNamesForInsert;
        vector<string>              m_configValuesForInsert;
        vector<ResourceId>          m_configTypesForInsert;

        vector<string>              m_configNamesForUpdate;
        vector<string>              m_configValuesForUpdate;
        vector<ResourceId>          m_configTypesForUpdate;

    protected :
    public :
};

}

#endif // PERSISTENCEEXECUTETRANSACTIONCONTEXT_H
