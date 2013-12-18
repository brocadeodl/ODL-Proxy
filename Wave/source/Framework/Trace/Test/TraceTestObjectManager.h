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
 *   Copyright (C) 2006 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef TRACETESTOBJECTMANAGER_H
#define TRACETESTOBJECTMANAGER_H

#include "Framework/Trace/Test/TraceTestContext.h"
#include "Regression/PrismTestObjectManager.h"

namespace WaveNs
{

class TraceTestObjectManager : public PrismTestObjectManager
{
    private :
             TraceTestObjectManager             ();

        void testRequestHandler                 (RegressionTestMessage *pMessage);
        void clearTraceClientsInformationStep   (TraceTestContext *pTraceTestContext);
        void getTraceClientsInformationStep     (TraceTestContext *pTraceTestContext);
        void displayTraceClientsInformationStep (TraceTestContext *pTraceTestContext);
        void getTraceClientTraceLevelStep       (TraceTestContext *pTraceTestContext);
        void setTraceClientTraceLevelStep       (TraceTestContext *pTraceTestContext);
        void resetTraceClientTraceLevelStep     (TraceTestContext *pTraceTestContext);

    protected :
    public :
        virtual                       ~TraceTestObjectManager ();
        static TraceTestObjectManager *getInstance            ();
        static PrismServiceId          getPrismServiceId      ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // TRACETESTOBJECTMANAGER_H
