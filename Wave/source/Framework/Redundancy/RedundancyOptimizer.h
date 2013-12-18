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

#ifndef REDUNDANCYOPTIMIZER_H
#define REDUNDANCYOPTIMIZER_H

#include <string>
#include <map>
#include "Framework/Utils/FixedSizeBuffer.h"
#include "Mathematics/Z2/Z2.h"
#include "Framework/Redundancy/RedundancyOptimizerBase.h"

using namespace std;

namespace WaveNs
{

class RedundancyOptimizer : public RedundancyOptimizerBase
{
    private :
        void addUniqueChunk (const string &fingerPrint, char *pBuffer, const UI64 &bufferLength);

    protected :
    public :
                       RedundancyOptimizer     ();
        virtual       ~RedundancyOptimizer     ();

        virtual void   optimizeFile            (const string &fileName);

        virtual void   print                   ();

        virtual void   destroyUniqueChunks     ();

    // Now the data members

    private :
        map<string, FixedSizeBuffer *> m_uniqueChunks;
        map<string, UI64>              m_uniqueChunkCounts;

    protected :
    public :
};

}

#endif // REDUNDANCYOPTIMIZER_H
