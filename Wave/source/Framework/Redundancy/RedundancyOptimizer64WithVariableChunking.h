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

#ifndef REDUNDANCYOPTIMIZER64WITHVARIABLECHUNKING_H
#define REDUNDANCYOPTIMIZER64WITHVARIABLECHUNKING_H

#include <string>
#include <map>
#include "Framework/Utils/FixedSizeBuffer.h"
#include "Mathematics/Z2/Z2.h"
#include "Framework/Redundancy/RedundancyOptimizer64.h"

using namespace std;

namespace WaveNs
{

class RedundancyOptimizer64WithVariableChunking : public RedundancyOptimizer64
{
    private :
    protected :
    public :
                       RedundancyOptimizer64WithVariableChunking ();
        virtual       ~RedundancyOptimizer64WithVariableChunking ();

        virtual void   optimizeFile                              (const string &fileName);

                UI32   getMinimumChunkSize                       () const;
                void   setMinimumChunkSize                       (const UI32 &minimumChunkSize);
                UI32   getMaximumChunkSize                       () const;
                void   setMaximumChunkSize                       (const UI32 &maximumChunkSize);
                UI64   getMagicFingerPrint                       () const;
                void   setMagicFingerPrint                       (const UI64 &magicFingerPrint);
                UI32   getMagicFingerPrintSize                   () const;
                void   setMagicFingerPrintSize                   (const UI32 &magicFingerPrintSize);
                UI64   getMagicFingerPrintBitMask                () const;
                void   setMagicFingerPrintBitMask                (const UI64 &magicFingerPrintBitMask);

        virtual void   print                                     ();

    // Now the data members

    private :
        UI32 m_minimumChunkSize;
        UI32 m_maximumChunkSize;
        UI64 m_magicFingerPrint;
        UI32 m_magicFingerPrintSize;
        UI64 m_magicFingerPrintBitMask;

        UI64 m_anchorFingerPrintMatches;
        UI64 m_maximumChunkSizeMatches;
        UI64 m_residueMatches;

    protected :
    public :
};

}

#endif // REDUNDANCYOPTIMIZER64WITHVARIABLECHUNKING_H
