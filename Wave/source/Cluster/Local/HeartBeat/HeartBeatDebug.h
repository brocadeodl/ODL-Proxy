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

#ifndef HEARTBEATDEBUG_H
#define HEARTBEATDEBUG_H


namespace WaveNs
{


class HeartBeatDebug
{
    private :
    protected :
    public :
        virtual        ~HeartBeatDebug  ();
                        HeartBeatDebug  ();

                        void initialize                     ();
                static  UI32 shellExecuteStartHeartBeat     (UI32 argc, vector<string> argv);
                static  UI32 shellExecuteStopHeartBeat      (UI32 argc, vector<string> argv);
                static  UI32 shellExecutePauseHeartBeat     (UI32 argc, vector<string> argv);
                static  UI32 shellExecuteResumeHeartBeat    (UI32 argc, vector<string> argv);
                static  UI32 shellExecuteConfigHeartBeat    (UI32 argc, vector<string> argv);
                static  UI32 shellExecuteShowStatHeartBeat  (UI32 argc, vector<string> argv);
                static  UI32 shellExecuteGetStatHeartBeat   (UI32 argc, vector<string> argv);

    private :
    protected :
    public :
};


}

#endif // HEARTBEATDEBUG_H
