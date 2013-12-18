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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef PERSISTENCEOBJECTMANAGEREXECUTETRANSACTIONASYNCHRONOUSCONTEXT_H
#define PERSISTENCEOBJECTMANAGEREXECUTETRANSACTIONASYNCHRONOUSCONTEXT_H

#include "Framework/Utils/PrismAsynchronousContext.h"

using namespace std;

namespace WaveNs
{

class PersistenceObjectManagerExecuteTransactionAsynchronousContext : public PrismAsynchronousContext
{   
    private :
    protected :
    public :
                                    PersistenceObjectManagerExecuteTransactionAsynchronousContext   (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL);
        virtual                    ~PersistenceObjectManagerExecuteTransactionAsynchronousContext   ();

                string              getSql                                                          () const;
                void                setSql                                                          (const string &sql);
                bool                getIsConfigurationChange                                        () const;
                void                setIsConfigurationChange                                        (const bool &isConfigurationChange);
                bool                getIsConfigurationTimeChange                                    () const;
                void                setIsConfigurationTimeChange                                    (const bool &isConfigurartionTimeChange);
                PrismServiceId      getSenderServiceCode                                            () const;
                void                setSenderServiceCode                                            (const PrismServiceId &senderServiceCode);
                LocationId          getSenderLocationId                                             () const;
                void                setSenderLocationId                                             (const LocationId &senderLocationId);
    
    // Now the data members

    private :
        string                      m_sql;
        bool                        m_isConfigurationChange;
        bool                        m_isConfigurationTimeChange;
        PrismServiceId              m_senderServiceCode;
        LocationId                  m_senderLocationId;

    protected :
    public : 
};  

}

#endif // PERSISTENCEOBJECTMANAGEREXECUTETRANSACTIONASYNCHRONOUSCONTEXT_H

