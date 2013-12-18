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

#ifndef PRISMTESTOBJECTMANAGER_H
#define PRISMTESTOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Regression/RegressionTestMessage.h"
#include "Regression/RegressionPrepareMessage.h"
#include "Regression/RegressionPrepareMessage2.h"
#include <map>
#include <vector>
#include <string>

namespace WaveNs
{

class PrismTestObjectManager : public WaveLocalObjectManager
{
    private :
        virtual PrismMessage *createMessageInstance (const UI32 &operationCode);

    protected :
                     PrismTestObjectManager              (const string &prismTestObjectManagerName, const UI32 &stackSize = 0, const vector<UI32> *pCpuAffinityVector = NULL);
        virtual void testRequestHandler                  (RegressionTestMessage *pMessage) = 0;
                void prepareForRegressionRequestHandler  (RegressionPrepareMessage *pMessage);
                void prepareForRegressionRequestHandler2 (RegressionPrepareMessage2 *pMessage);
              string getTestParameterValue (const string &inputKeyString);

    public :

    // Now the data members

    private :
    protected :
        map<string, string> m_testParameters;
        vector<string> m_regressionInput;

    public :
};

}

#endif // PRISMTESTOBJECTMANAGER_H
