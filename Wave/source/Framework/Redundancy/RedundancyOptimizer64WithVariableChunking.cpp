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

#include "Framework/Redundancy/RedundancyOptimizer64WithVariableChunking.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

#include <fstream>

using namespace std;

namespace WaveNs
{

RedundancyOptimizer64WithVariableChunking::RedundancyOptimizer64WithVariableChunking ()
    : RedundancyOptimizer64      (),
      m_anchorFingerPrintMatches (0),
      m_maximumChunkSizeMatches  (0),
      m_residueMatches           (0)
{
    UI32 i        = 0;
    UI32 msbIndex = 0;

    m_maximumChunkSize = 0;
    m_minimumChunkSize = 0;
    m_magicFingerPrint        = 0x78; //0x8;

    for (i = 0; i < 64; i++)
    {
        if (m_magicFingerPrint & (0x1ULL << i))
        {
            msbIndex = i;
        }
    }

    m_magicFingerPrintSize    = msbIndex;
    m_magicFingerPrintBitMask = (0x1ULL << msbIndex) - 1;

    if (m_fixedChunksize > 32)
    {
        m_minimumChunkSize = m_fixedChunksize - 16;
        m_maximumChunkSize = m_fixedChunksize + 16;
    }
}

RedundancyOptimizer64WithVariableChunking::~RedundancyOptimizer64WithVariableChunking ()
{
}

UI32 RedundancyOptimizer64WithVariableChunking::getMinimumChunkSize () const
{
    return (m_minimumChunkSize);
}

void RedundancyOptimizer64WithVariableChunking::setMinimumChunkSize (const UI32 &minimumChunkSize)
{
    m_minimumChunkSize = minimumChunkSize;
}

UI32 RedundancyOptimizer64WithVariableChunking::getMaximumChunkSize () const
{
    return (m_maximumChunkSize);
}

void RedundancyOptimizer64WithVariableChunking::setMaximumChunkSize (const UI32 &maximumChunkSize)
{
    m_maximumChunkSize = maximumChunkSize;
}

UI64 RedundancyOptimizer64WithVariableChunking::getMagicFingerPrint () const
{
    return (m_magicFingerPrint);
}

void RedundancyOptimizer64WithVariableChunking::setMagicFingerPrint (const UI64 &magicFingerPrint)
{
    m_magicFingerPrint = magicFingerPrint;
}

UI32 RedundancyOptimizer64WithVariableChunking::getMagicFingerPrintSize () const
{
    return (m_magicFingerPrintSize);
}

void RedundancyOptimizer64WithVariableChunking::setMagicFingerPrintSize (const UI32 &magicFingerPrintSize)
{
    m_magicFingerPrintSize = magicFingerPrintSize;
}

UI64 RedundancyOptimizer64WithVariableChunking::getMagicFingerPrintBitMask () const
{
    return (m_magicFingerPrintBitMask);
}

void RedundancyOptimizer64WithVariableChunking::setMagicFingerPrintBitMask (const UI64 &magicFingerPrintBitMask)
{
    m_magicFingerPrintBitMask = magicFingerPrintBitMask;
}

void RedundancyOptimizer64WithVariableChunking::optimizeFile (const string &fileName)
{
             ifstream  inputFileStream;
             char     *pBuffer                  = NULL; 
             UI64      fingerPrint              = 0;
             UI64      chunkFingerPrint         = 0;
    const    UI32      slidingWindowSize        = m_z2.getSlidingWindowSize ();
             UI32      consumedChunkSize        = 0;
             UI32      bytesConsumedInThisRead  = 0;
             char     *pCurrentBuffer           = NULL;
             char     *pCurrentBufferBeginning  = NULL;
             UI32      readLength               = 0;
             char     *pTempBuffer              = NULL;
    unsigned char      oldestData               = '\0';

    prismAssert (slidingWindowSize <= m_minimumChunkSize, __FILE__, __LINE__);
    prismAssert (slidingWindowSize <= m_fileIoSize, __FILE__, __LINE__);

    inputFileStream.open (fileName.c_str (), ios::binary);

    if (!inputFileStream)
    {
        //trace (TRACE_LEVEL_ERROR, "RedundancyOptimizer64WithVariableChunking::optimizeFile : File (" + fileName + ") cannot be opened to read.");
        //cerr << "RedundancyOptimizer64WithVariableChunking::optimizeFile : File (" + fileName + ") cannot be opened to read." << endl;
        return;
    }
    
    // Allocate a buffer here, just before use.
    pBuffer                  = new char[m_fileIoSize];
    prismAssert (NULL != pBuffer, __FILE__, __LINE__);

    while (! (inputFileStream.eof ()))
    {
        inputFileStream.read (pBuffer, m_fileIoSize);
        readLength = inputFileStream.gcount ();

        pCurrentBuffer = pBuffer;

        pCurrentBufferBeginning  = pCurrentBuffer;
        consumedChunkSize        = 0;
        fingerPrint              = 0;
        chunkFingerPrint         = 0;
        bytesConsumedInThisRead  = 0;

        while (bytesConsumedInThisRead < readLength)
        {
            if (consumedChunkSize < slidingWindowSize)
            {
                oldestData = '\0';
            }
            else
            {
                oldestData = *(pCurrentBuffer - slidingWindowSize);
            }

            m_z2.computeFingerPrintOptimizedFor64 (fingerPrint, oldestData, (unsigned char) pCurrentBuffer[0]);

            consumedChunkSize++;
            pCurrentBuffer++;
            bytesConsumedInThisRead++;

            if (consumedChunkSize >= m_minimumChunkSize)
            {
                if (((fingerPrint & m_magicFingerPrintBitMask) == (m_magicFingerPrint & m_magicFingerPrintBitMask)) || (consumedChunkSize >= m_maximumChunkSize))
                {
                    if ((fingerPrint & m_magicFingerPrintBitMask) == (m_magicFingerPrint & m_magicFingerPrintBitMask))
                    {
                        m_anchorFingerPrintMatches++;
                    }
                    else
                    {
                        m_maximumChunkSizeMatches++;
                    }

                    pTempBuffer = new char[consumedChunkSize];
                    memcpy (pTempBuffer, pCurrentBufferBeginning, consumedChunkSize);

                    m_z2.computeFingerPrintOptimizedFor64 (chunkFingerPrint, pTempBuffer, consumedChunkSize);

                    addUniqueChunk (chunkFingerPrint, pTempBuffer, consumedChunkSize);

                    pCurrentBufferBeginning  = pCurrentBuffer;
                    consumedChunkSize        = 0;
                    fingerPrint              = 0;
                    chunkFingerPrint         = 0;
                }
            }
        }

        if (consumedChunkSize > 0)
        {
            m_residueMatches++;

            pTempBuffer = new char[consumedChunkSize];
            memcpy (pTempBuffer, pCurrentBufferBeginning, consumedChunkSize);

            m_z2.computeFingerPrintOptimizedFor64 (chunkFingerPrint, pTempBuffer, consumedChunkSize);

            addUniqueChunk (chunkFingerPrint, pTempBuffer, consumedChunkSize);

            pCurrentBufferBeginning  = pCurrentBuffer;
            consumedChunkSize        = 0;
            fingerPrint              = 0;
            chunkFingerPrint         = 0;
        }
    }

    delete[] pBuffer;
}

void RedundancyOptimizer64WithVariableChunking::print ()
{
    printf ("Magic Finger Print          : 0x%016llX\n", m_magicFingerPrint);
    printf ("Magic Finger Print Size     : %u\n",        m_magicFingerPrintSize);
    printf ("Magic Finger Print Bit Mask : 0x%016llX\n", m_magicFingerPrintBitMask);
    printf ("Anchor Finger Print Matches : %llu\n",      m_anchorFingerPrintMatches);
    printf ("Maximum Chunk Size Matches  : %llu\n",      m_maximumChunkSizeMatches);
    printf ("Residue Matches             : %llu\n",      m_residueMatches);
}

}
