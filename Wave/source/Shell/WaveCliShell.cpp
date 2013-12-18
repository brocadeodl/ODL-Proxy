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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Shell/WaveCliShell.h"
#include "Shell/WaveCliRegressionShell.h"
#include "Shell/WaveCliServiceShell.h"
#include "Shell/WaveCliClusterShell.h"
#include "Shell/WaveCliWyserShell.h"
#include "Shell/WaveCliDebugShell.h"
#include "Shell/WaveCliTraceShell.h"
#include "Shell/WaveCliShowShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

WaveCliShell::WaveCliShell (WaveClientSynchronousConnection &connection)
    : WaveShell    ("Wave"),
      m_connection (connection)
{
}

WaveCliShell::WaveCliShell (const string &shellName, WaveClientSynchronousConnection &connection)
    : WaveShell    (shellName),
      m_connection (connection)
{
}

WaveCliShell::~WaveCliShell ()
{
}

WaveCliShell *WaveCliShell::getInstance (WaveClientSynchronousConnection &connection)
{
    static WaveCliShell *pWaveCliShell = NULL;

    if (NULL == pWaveCliShell)
    {
        pWaveCliShell = new WaveCliShell (connection);

        WaveCliRegressionShell *pRegressionShell = new WaveCliRegressionShell (connection);
        WaveCliServiceShell    *pServiceShell    = new WaveCliServiceShell    (connection);
        WaveCliClusterShell    *pClusterShell    = new WaveCliClusterShell    (connection);
        WaveCliWyserShell      *pWyserShell      = new WaveCliWyserShell      (connection);
        WaveCliDebugShell      *pDebugShell      = new WaveCliDebugShell      (connection);
        WaveCliTraceShell      *pTraceShell      = new WaveCliTraceShell      (connection);
        WaveCliShowShell       *pShowShell       = new WaveCliShowShell       (connection);

        pWaveCliShell->addSubShell (pRegressionShell);
        pWaveCliShell->addSubShell (pServiceShell);
        pWaveCliShell->addSubShell (pClusterShell);
        pWaveCliShell->addSubShell (pWyserShell);
        pWaveCliShell->addSubShell (pDebugShell);
        pWaveCliShell->addSubShell (pTraceShell);
        pWaveCliShell->addSubShell (pShowShell);
    }

    return (pWaveCliShell);
}

WaveClientSynchronousConnection &WaveCliShell::getConnection ()
{
    return (m_connection);
}

}
