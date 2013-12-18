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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PERSISTENCELOCALOBJECTMANAGEREXECUTETRANSACTIONMESSAGE_H
#define PERSISTENCELOCALOBJECTMANAGEREXECUTETRANSACTIONMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class PersistenceLocalObjectManagerExecuteTransactionMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                              PersistenceLocalObjectManagerExecuteTransactionMessage (const string &sql, const UI32 &transactionId);
                              PersistenceLocalObjectManagerExecuteTransactionMessage ();
        virtual              ~PersistenceLocalObjectManagerExecuteTransactionMessage ();

                const string &getSql                                                 () const;
                      void    setSql                                                 (const string &sql);
                      bool    getIsCommit                                            () const;
                      void    setIsCommit                                            (const bool &isCommit);
                      bool    getIsRollback                                          () const;
                      void    setIsRollback                                          (const bool &isRollback);
                      bool    getIsPrepare                                           () const;
                      void    setIsPrepare                                           (const bool &isPrepare);
                      UI32    getTransactionId                                       () const;
                      void    setTransactionId                                       (const UI32 &transactionId);
                      bool    getIsConfigurationChange                               () const;
                      void    setIsConfigurationChange                               (const bool &isConfigurationChange);
                      bool    getIsConfigurationTimeChange                           () const;
                      void    setIsConfigurationTimeChange                           (const bool &isConfigurationTimeChange);

                      void    setAnyConfigurationChangeTrackingNumber                (const UI64 &trackingNumber);
                      UI64    getAnyConfigurationChangeTrackingNumber                () const;

    // Now the data members

    private :
        string m_sql;
        bool   m_isCommit;
        bool   m_isRollback;
        UI32   m_transactionId;
        bool   m_isPrepare;
        bool   m_isConfigurationChange;
        bool   m_isConfigurationTimeChange;
        UI64   m_anyConfigurationChangeTrackingNumber;

    protected :
    public :
};

}

#endif // PERSISTENCELOCALOBJECTMANAGEREXECUTETRANSACTIONMESSAGE_H
