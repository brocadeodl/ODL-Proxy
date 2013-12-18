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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef REGRESSIONTESTOBJECTMANAGER_H
#define REGRESSIONTESTOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Regression/RegressionTestEntry.h"
#include <vector>

namespace WaveNs
{

class RegressionTestObjectManagerGetTestServiceEntriesMessage;
class RegressionTestObjectManagerSetTestServiceStateMessage;
class RegressionTestObjectManagerStartRegressionMessage;
class RegressionTestObjectManagerPrepareTestForAServiceMessage;
class RegressionTestObjectManagerRunTestForAServiceMessage;

class RegressionTestObjectManager : public WaveLocalObjectManager
{
    private :
                              RegressionTestObjectManager         ();
                void          getTestServiceEntriesMessageHandler (RegressionTestObjectManagerGetTestServiceEntriesMessage *pMessage);
                void          setTestServiceStateMessageHandler   (RegressionTestObjectManagerSetTestServiceStateMessage *pMessage);
                void          startRegressionMessageHandler       (RegressionTestObjectManagerStartRegressionMessage *pMessage);
                void          prepareAServiceTestMessageHandler   (RegressionTestObjectManagerPrepareTestForAServiceMessage *pMessage);
                void          runAServiceTestMessageHandler       (RegressionTestObjectManagerRunTestForAServiceMessage *pMessage);

        virtual PrismMessage *createMessageInstance               (const UI32 &operationCode);

    protected :
    public :
        static RegressionTestObjectManager *getInstance          ();
        static PrismServiceId               getPrismServiceId    ();
        static string                       getServiceName       ();

        static void                         addToRegressionShell (const PrismServiceId &prismServiceId, const bool &isEnabledByDefault);

    // Now the data members

    private :
        vector<RegressionTestEntry> m_testServiceEntries;

    protected :
    public :

    friend class RegressionTestThread;
};

}

#endif //REGRESSIONTESTOBJECTMANAGER_H
