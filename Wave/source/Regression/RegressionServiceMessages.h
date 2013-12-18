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

#ifndef REGRESSIONSERVICEMESSAGES_H
#define REGRESSIONSERVICEMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/Types.h"
#include "Regression/RegressionTestEntry.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include <vector>

namespace WaveNs
{

class RegressionTestObjectManagerGetTestServiceEntriesMessage : public ManagementInterfaceMessage
{
    private :
        void setNumberOfTestServiceEntries (UI32 numberOfTestServiceEntries);
        void addTestServiceEntry           (const PrismServiceId &prismServiceId, const bool &isServiceEnabled);

    protected :
    public :
                                     RegressionTestObjectManagerGetTestServiceEntriesMessage ();
        virtual                     ~RegressionTestObjectManagerGetTestServiceEntriesMessage ();
        virtual void                 setupAttributesForSerialization                         ();

                UI32                 getNumberOfTestServiceEntries                           ();
                RegressionTestEntry  getTestServiceEntryAt                                   (UI32 index);

    // Now the data members

    private :
        UI32         m_numberOfTestServiceEntries;
        vector<UI32> m_testServiceIds;
        vector<bool> m_testServiceStates;

    protected :
    public:

        friend class RegressionTestObjectManager;
};

class RegressionTestObjectManagerSetTestServiceStateMessage : public PrismMessage
{
    private :
    protected :
    public :
                                RegressionTestObjectManagerSetTestServiceStateMessage ();
                                RegressionTestObjectManagerSetTestServiceStateMessage (PrismServiceId testseviceId, bool isTestEnabled);
        virtual                ~RegressionTestObjectManagerSetTestServiceStateMessage ();
        virtual void            setupAttributesForSerialization                       ();
                PrismServiceId  getTestServiceId                                      ();
                bool            getIsTestEnabled                                      ();

    // Now the data members

    private :
        PrismServiceId m_testServiceId;
        bool           m_isTestEnabled;

    protected :
    public :
};

class RegressionTestObjectManagerStartRegressionMessage : public PrismMessage
{
    private :
    protected :
    public :
                      RegressionTestObjectManagerStartRegressionMessage ();
                      RegressionTestObjectManagerStartRegressionMessage (UI32 numberOfTimesToRunRegression);
        virtual      ~RegressionTestObjectManagerStartRegressionMessage ();
        virtual void  setupAttributesForSerialization                   ();
        UI32          getNumberOfTimesToRunRegression                   ();

    // Now the data members

    private :
        UI32 m_numberOfTimesToRunRegression;

    protected :
    public :
};

class RegressionTestObjectManagerRunTestForAServiceMessage : public ManagementInterfaceMessage
{
    private :
    protected :
    public :
                                RegressionTestObjectManagerRunTestForAServiceMessage ();
                                RegressionTestObjectManagerRunTestForAServiceMessage (PrismServiceId serviceCode);
                                RegressionTestObjectManagerRunTestForAServiceMessage (PrismServiceId serviceCode, UI32 numberOfTimesToRunServiceTest);
        virtual                ~RegressionTestObjectManagerRunTestForAServiceMessage ();
        virtual void            setupAttributesForSerialization                      ();
                PrismServiceId  getServiceCode                                       ();
                UI32            getNumberOfTimesToRunServiceTest                     ();

    // Now the data members

    private :
        PrismServiceId m_serviceCode;
        UI32           m_numberOfTimesToRunServiceTest;

    protected :
    public :
};

class RegressionTestObjectManagerPrepareTestForAServiceMessage : public ManagementInterfaceMessage
{
    private :
    protected :
    public :
                                RegressionTestObjectManagerPrepareTestForAServiceMessage ();
                                RegressionTestObjectManagerPrepareTestForAServiceMessage (PrismServiceId serviceCode);
                                RegressionTestObjectManagerPrepareTestForAServiceMessage (PrismServiceId serviceCode, vector<string> inputStrings);
        virtual                ~RegressionTestObjectManagerPrepareTestForAServiceMessage ();
        virtual void            setupAttributesForSerialization                      ();
                PrismServiceId  getServiceCode                                       ();
                vector<string> &getInputStrings                                      ();

    // Now the data members

    private :
        PrismServiceId m_serviceCode;
        vector<string> m_inputStrings;

    protected :
    public :
};

}

#endif // REGRESSIONSERVICEMESSAGES_H
