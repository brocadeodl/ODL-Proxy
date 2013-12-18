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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <iostream>
#include "Framework/Types/Types.h"
#include "Framework/Redundancy/RedundancyOptimizerBase.h"

using namespace std;
using namespace WaveNs;

int main (int argc, char *argv[])
{
    SI32           i;
    vector<string> argumentVector;

    for (i = 0; i < argc; i++)
    {
        argumentVector.push_back (argv[i]);
    }

    RedundancyOptimizerBase::optimizePathDebugShellEntree (argc, argumentVector);

    return (0);
}