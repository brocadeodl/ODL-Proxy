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
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#ifndef CLUSTERDEBUG_H
#define CLUSTERDEBUG_H

#include "App/AppInterface.h"
#include <string>
#include <vector>

namespace WaveNs
{

class ClusterDebug
{
    private :
    protected :
    public :
                         ClusterDebug  ();
                        ~ClusterDebug  ();
                void     initialize    ();

        static  UI32     shellExecuteCreateClusterDebug     (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteShowStatDebug          (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteLocatHeartBeatDebug    (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteRegisterCallBackDebug  (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteAddNodeDebug           (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteJoinNodeDebug          (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteRemoveNodeDebug        (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteGetRoleDebug           (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteConfigHeartBeatDebug   (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteRemoveClusterDebug     (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteGetClusterConfigDebug  (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteGetClusterInfoDebug    (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteMakeThisNodePrincipal  (UI32 argc, vector<string> argv);
        static  UI32     shellExecuteNodifySecondaryNodeFailure  (UI32 argc, vector<string> argv);
        // Now the data members

    private :

    protected :
    public :
};


}

#endif // CLUSTERDEBUG_H
