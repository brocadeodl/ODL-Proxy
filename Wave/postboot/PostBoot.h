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

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

typedef enum // bits in bitmap
{
    WAVE_RECOVERY_TYPE_INVALID = 0,
    WAVE_RECOVERY_TYPE_COLD = 1,
    WAVE_RECOVERY_TYPE_WARM = 2
} configReplayRecoveryTypeEnum;

class configReplayTable {

    private:
        void    printHeader             (string filePath);
        string  printServicePushBack    (string serviceName);
    protected:
    public:
                configReplayTable             ();
               ~configReplayTable             ();

        void    printCpp                (string filePath);
        void    printDotH               (string filePath);
        void    addPass                 (string *tablename, int passNumber, vector< pair<string, int> > pass_service_Name);

    // Data Members 
    private:
        // map of <tablename, map of <pass#, vector of <OM name, recovery type enum>>>
        map<string, map<int, vector <pair<string, int> > > >      configReplayMap;
		
    protected:
    public:
};
