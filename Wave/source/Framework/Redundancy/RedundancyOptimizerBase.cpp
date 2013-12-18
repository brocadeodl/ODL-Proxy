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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Redundancy/RedundancyOptimizerBase.h"
#include "Shell/ShellDebug.h"
#include "Framework/Redundancy/RedundancyOptimizer64.h"
#include "Framework/Redundancy/RedundancyOptimizer.h"
#include "Framework/Redundancy/RedundancyOptimizer64WithVariableChunking.h"
#include "Framework/Utils/TraceUtils.h"

#include <limits.h>

namespace WaveNs
{

static RedundancyOptimizerBase *s_pCurrentRedundancyOptimizer = NULL;

RedundancyOptimizerBase::RedundancyOptimizerBase ()
    : m_enableCompression               (false),
      m_storeUniqueChunkData            (false),
      m_resetPerFile                    (false),
      m_printFileNameOnly               (true),
      m_printPerFileStatistics          (true),
      m_globalOriginalSize              (0),
      m_globalTotalNumberOfChunks       (0),
      m_globalTotalNumberOfUniqueChunks (0),
      m_globalOptimizedSize             (0),
      m_globalOptimizedCompressedSize   (0),
      m_originalSize                    (0),
      m_totalNumberOfChunks             (0),
      m_totalNumberOfUniqueChunks       (0),
      m_optimizedSize                   (0),
      m_optimizedCompressedSize         (0),
      m_fileIoSize                      (16 * 1024),
      m_fixedChunksize                  (128)
{
}

RedundancyOptimizerBase::~RedundancyOptimizerBase ()
{
}

UI64 RedundancyOptimizerBase::getOriginalSize () const
{
    return (m_originalSize);
}

UI64 RedundancyOptimizerBase::getTotalNumberOfChunks () const
{
    return (m_totalNumberOfChunks);
}

UI64 RedundancyOptimizerBase::getTotalNumberOfUniqueChunks () const
{
    return (m_totalNumberOfUniqueChunks);
}

UI64 RedundancyOptimizerBase::getOptimizedSize () const
{
    return (m_optimizedSize);
}

bool RedundancyOptimizerBase::getEnableCompression () const
{
    return (m_enableCompression);
}

void RedundancyOptimizerBase::setEnableCompression (const bool &enableCompression)
{
    m_enableCompression = enableCompression;
}

bool RedundancyOptimizerBase::getStoreUniqueChunkData () const
{
    return (m_storeUniqueChunkData);
}

void RedundancyOptimizerBase::setStoreUniqueChunkData (const bool &storeUniqueChunkData)
{
    m_storeUniqueChunkData = storeUniqueChunkData;
}

bool RedundancyOptimizerBase::getResetPerFile () const
{
    return (m_resetPerFile);
}

void RedundancyOptimizerBase::setResetPerFile (const bool &resetPerFile)
{
    m_resetPerFile = resetPerFile;
}

bool RedundancyOptimizerBase::getPrintFileNameOnly () const
{
    return (m_printFileNameOnly);
}

void RedundancyOptimizerBase::setPrintFileNameOnly (const bool &printFileNameOnly)
{
    m_printFileNameOnly = printFileNameOnly;
}

bool RedundancyOptimizerBase::getPrintPerFileStatistics () const
{
    return (m_printPerFileStatistics);
}

void RedundancyOptimizerBase::setPrintPerFileStatistics (const bool &printPerFileStatistics)
{
    m_printPerFileStatistics = printPerFileStatistics;
}

UI64 RedundancyOptimizerBase::getOptimizedCompressedSize () const
{
    return (m_optimizedCompressedSize);
}

UI64 RedundancyOptimizerBase::getFileIoSize () const
{
    return (m_fileIoSize);
}

void RedundancyOptimizerBase::setFileIoSize (const UI64 &fileIoSize)
{
    m_fileIoSize = fileIoSize;
}

UI64 RedundancyOptimizerBase::getFixedChunkSize () const
{
    return (m_fixedChunksize);
}

void RedundancyOptimizerBase::setFixedChunkSize (const UI64 &fixedChunkSize)
{
    m_fixedChunksize = fixedChunkSize;
}

void RedundancyOptimizerBase::printStatistics () const
{
    UI64  numberOfTotalChunks             = m_totalNumberOfChunks;
    UI64  numberOfUniqueChunks            = getTotalNumberOfUniqueChunks ();
    UI64  originalSize                    = getOriginalSize ();
    UI64  optimizedSize                   = getOptimizedSize ();
    UI64  optimizedCompressedSize         = getOptimizedCompressedSize ();
    float redundancyRatio                 = ((double) originalSize) / optimizedSize;
    float redundancyPercentage            = ((double) (originalSize - optimizedSize)) / originalSize * 100;
    float redundancyCompressionPercentage = ((double) (originalSize - optimizedCompressedSize)) / originalSize * 100;

    cout << "Number Of Total  Chunks    : " << numberOfTotalChunks << endl;
    cout << "Number Of Unique Chunks    : " << numberOfUniqueChunks << endl;
    cout << "Original Size              : " << originalSize << endl;
    cout << "Optimized Size             : " << optimizedSize << endl;
    cout << "Optimized Compressed Size  : " << optimizedCompressedSize << endl;
    cout << "Redundancy Ratio           : " << redundancyRatio << endl;
    cout << "Redundancy %               : " << redundancyPercentage << endl;
    cout << "Redundancy + Compression % : " << redundancyCompressionPercentage << endl;
}

void RedundancyOptimizerBase::printStatisticsHorizontal (const string &currentFileName) const
{
    UI64  numberOfTotalChunks             = getTotalNumberOfChunks ();
    UI64  numberOfUniqueChunks            = getTotalNumberOfUniqueChunks ();
    UI64  originalSize                    = getOriginalSize ();
    UI64  optimizedSize                   = getOptimizedSize ();
    UI64  optimizedCompressedSize         = getOptimizedCompressedSize ();
    float redundancyRatio                 = ((double) originalSize) / optimizedSize;
    float redundancyPercentage            = ((double) (originalSize - optimizedSize)) / originalSize * 100;
    float redundancyCompressionPercentage = ((double) (originalSize - optimizedCompressedSize)) / originalSize * 100;

    if (false == m_printFileNameOnly)
    {
        printf ("%15llu│%15llu│%15llu│%15llu│%15llu│%12.5f│%9.5f│%9.5f│%s\n", numberOfTotalChunks, numberOfUniqueChunks, originalSize, optimizedSize, optimizedCompressedSize, redundancyRatio, redundancyPercentage, redundancyCompressionPercentage, currentFileName.c_str ());
    }
    else
    {
        char  fileName[PATH_MAX + 1];
        char *pFileNameOnly       = NULL;

        strncpy (fileName, currentFileName.c_str (), PATH_MAX);
        fileName[PATH_MAX] = '\0';

        pFileNameOnly = rindex (fileName, '/');

        if (NULL == pFileNameOnly)
        {
            printf ("%15llu│%15llu│%15llu│%15llu│%15llu│%12.5f│%9.5f│%9.5f│%s\n", numberOfTotalChunks, numberOfUniqueChunks, originalSize, optimizedSize, optimizedCompressedSize, redundancyRatio, redundancyPercentage, redundancyCompressionPercentage, currentFileName.c_str ());
        }
        else
        {
            printf ("%15llu│%15llu│%15llu│%15llu│%15llu│%12.5f│%9.5f│%9.5f│%s\n", numberOfTotalChunks, numberOfUniqueChunks, originalSize, optimizedSize, optimizedCompressedSize, redundancyRatio, redundancyPercentage, redundancyCompressionPercentage, pFileNameOnly + 1);
        }
    }
}

void RedundancyOptimizerBase::printStatisticsHeader1() const
{
    printf ("%15s %15s %15s %15s %15s %12s %9s %9s %s\n", "Total Chunks", "Unique Chunks", "Size", "O Size", "O + C Size", "R Ratio", "R %", "R + C %", "File Name");
}

void RedundancyOptimizerBase::printStatisticsHeader2 () const
{
    printf ("%15s %15s %15s %15s %15s %12s %9s %9s %s\n", "_______________", "_______________", "_______________", "_______________", "_______________", "____________", "_________", "_________", "_________");
}

void RedundancyOptimizerBase::printStatisticsHeader () const
{
    printStatisticsHeader1 ();
    printStatisticsHeader2 ();
}

void RedundancyOptimizerBase::printGlobalStatisticsHeader1 () const
{
    printf ("%15s %15s %15s %15s %15s %12s %9s %9s\n", "Total Chunks", "Unique Chunks", "Size", "O Size", "O + C Size", "R Ratio", "R %", "R + C %");
}

void RedundancyOptimizerBase::printGlobalStatisticsHeader2 () const
{
    printf ("%15s %15s %15s %15s %15s %12s %9s %9s\n", "_______________", "_______________", "_______________", "_______________", "_______________", "____________", "_________", "_________");
}

void RedundancyOptimizerBase::printGlobalStatisticsHeader () const
{
    printGlobalStatisticsHeader1 ();
    printGlobalStatisticsHeader2 ();
}

void RedundancyOptimizerBase::printGlobalStatisticsHorizontal () const
{
    UI64  numberOfTotalChunks             = m_globalTotalNumberOfChunks;
    UI64  numberOfUniqueChunks            = m_globalTotalNumberOfUniqueChunks;
    UI64  originalSize                    = m_globalOriginalSize;
    UI64  optimizedSize                   = m_globalOptimizedSize;
    UI64  optimizedCompressedSize         = m_globalOptimizedCompressedSize;
    float redundancyRatio                 = ((double) originalSize) / optimizedSize;
    float redundancyPercentage            = ((double) (originalSize - optimizedSize)) / originalSize * 100;
    float redundancyCompressionPercentage = ((double) (originalSize - optimizedCompressedSize)) / originalSize * 100;

    printf ("%15llu %15llu %15llu %15llu %15llu %12.5f %9.5f %9.5f\n", numberOfTotalChunks, numberOfUniqueChunks, originalSize, optimizedSize, optimizedCompressedSize, redundancyRatio, redundancyPercentage, redundancyCompressionPercentage);
}

void RedundancyOptimizerBase::resetCounts ()
{
    m_globalTotalNumberOfChunks       += m_totalNumberOfChunks;
    m_globalTotalNumberOfUniqueChunks += m_totalNumberOfUniqueChunks;
    m_globalOriginalSize              += m_originalSize;
    m_globalOptimizedSize             += m_optimizedSize;
    m_globalOptimizedCompressedSize   += m_optimizedCompressedSize;

    m_totalNumberOfChunks       = 0;
    m_totalNumberOfUniqueChunks = 0;
    m_originalSize              = 0;
    m_optimizedSize             = 0;
    m_optimizedCompressedSize   = 0;
}

void RedundancyOptimizerBase::reset ()
{
    resetCounts ();

    destroyUniqueChunks ();
}

int RedundancyOptimizerBase::fileTreeWalk (const char *pFilePath, const struct stat *pStatBuffer, int typeFlag)
{
    if (FTW_F == typeFlag)
    {
        optimizeFile (pFilePath);

        if (true == m_printPerFileStatistics)
        {
            printStatisticsHorizontal (pFilePath);
        }

        if (true == (getResetPerFile ()))
        {
            reset ();
        }
        else
        {
            resetCounts ();
        }
    }

    return (0);
}

int RedundancyOptimizerBase::fileTreeWalkCallback (const char *pFilePath, const struct stat *pStatBuffer, int typeFlag)
{
    return (s_pCurrentRedundancyOptimizer->fileTreeWalk (pFilePath, pStatBuffer, typeFlag));
}

void RedundancyOptimizerBase::optimizePath (const string &baseDirectoryName)
{
    int result = 0;

    if (true == (getPrintPerFileStatistics ()))
    {
        printStatisticsHeader ();
    }

    s_pCurrentRedundancyOptimizer= this;

    result = ftw (baseDirectoryName.c_str (), fileTreeWalkCallback, 63);

    if (true == (getPrintPerFileStatistics ()))
    {
        printStatisticsHeader2 ();
        cout << endl;
    }

    printGlobalStatisticsHeader ();
    printGlobalStatisticsHorizontal ();
    printGlobalStatisticsHeader2 ();

    if (0 != result)
    {
    	trace (TRACE_LEVEL_ERROR, "RedundancyOptimizerBase::optimizePath : Some errors have occurred.");
    }
}

void RedundancyOptimizerBase::displayUsage (const string &command)
{
    string blankCommand  = command;
    UI32   commandLength = command.length ();
    UI32   i             = 0;

    for (i = 0; i < commandLength; i++)
    {
        blankCommand[i] = ' ';
    }

    cout << endl;
    cout << "USAGE:" << endl;
    cout << endl;
    cout << endl;
    cout << command      << " <-path <Path To Reduce>> [-file <File To Reduce>]" << endl;
    cout << blankCommand << " <-file <File To Reduce>> [-path <Path To Reduce>]" << endl;
    cout << blankCommand << " [-compress <true/false>]" << endl;
    cout << blankCommand << " [-store <true/false>]" << endl;
    cout << blankCommand << " [-resetperfile <true/false>]" << endl;
    cout << blankCommand << " [-filenameonly <true/false>]" << endl;
    cout << blankCommand << " [-perfilestats <true/false>]" << endl;
    cout << blankCommand << " [-chunk <Fixed Chunk Size]" << endl;
    cout << blankCommand << " [-ro64 <true/false]" << endl;
    cout << endl;
    cout << endl;
    cout << "Command Line Options:" << endl;
    cout << endl;
    cout << "    -path         : path               - The Directory path to be used for traversing to optimize redundancy." << endl;
    cout << "                                         This option can be repeated to consider multiple directory trees." << endl;
    cout << endl;
    cout << "    -file         : file               - The file to be processed to optimize redundancy." << endl;
    cout << "                                         This option can be repeated to consider multiple files." << endl;
    cout << endl;
    cout << "    -compress     : true               - Enable compression for the Unique Chunks." << endl;
    cout << "                                         Will run significantly slower.  Uses zlib." << endl;
    cout << "                  : false              - Do not compress the unique chunks." << endl;
    cout << endl;
    cout << "    -store        : true               - Store unique chunks in memory.  Will use significantly more memory when enabled." << endl;
    cout << "                  : false              - Do not store the unique chunks in memory." << endl;
    cout << endl;
    cout << "    -resetperfile : true               - Reset the global unique chunk repository per file." << endl;
    cout << "                                         Redundancy within a file will be eliminated." << endl;
    cout << "                                         But will not eliminate redundancy across files." << endl;
    cout << "                  : false              - Do not reset the global unique chunk store per file." << endl;
    cout << "                                         Redundancy will be eliminated across all files found in the directory tree." << endl;
    cout << endl;
    cout << "    -filenameonly : true               - Print only the file name." << endl;
    cout << "                  : false              - Print fully qualified file name." << endl;
    cout << endl;
    cout << "    -perfilestats : true               - Print per file statistics in order as they are processed." << endl;
    cout << "                                         Global statistics will also be printed at the end." << endl;
    cout << "                  : false              - Do not print per file statistics.  Only global statistics will be printed." << endl;
    cout << endl;
    cout << "    -chunk        : <Fixed Chunk Size> - Size of the fixed chunk in fixed chunking." << endl;
    cout << endl;
    cout << "    -ro64         : true               - Use 64 Bit optimization for Redundancy Optimization." << endl;
    cout << "                                         Can support Polynomials upto 64 degree." << endl;
    cout << "                  : false              - Do not use 64 bit optimization for Redundancy Optimization.  Runs Slower." << endl;
    cout << "                                         No limitation on polynomial degree." << endl;
    cout << endl;

    cout << endl;
    cout << endl;
}

UI32 RedundancyOptimizerBase::optimizePathDebugShellEntree (UI32 argc, vector<string> argv)
{
    UI32           i                        = 0;

    bool           enableCompression        = false;
    bool           storeUniqueChunkData     = false;
    bool           resetPerFile             = false;
    bool           printFileNameOnly        = true;
    bool           printPerFileStatistics   = true;
    UI32           fixedChunkSize           = 128;
    UI32           minimumChunkSize         = fixedChunkSize - 16;
    UI32           maximumChunkSize         = fixedChunkSize + 16;

    bool           useRedundancyOptimizer64 = true;
    bool           useVariableChunking      = false;

    vector<string> pathsToReduce;
    UI32           numberOfPathsToReduce    = 0;
    vector<string> filesToReduce;
    UI32           numberOfFilesToReduce    = 0;

    if (2 == argc)
    {
        if ("-help" == argv[1])
        {
            displayUsage (argv[0]);
            return (0);
        }
    }

    if ((1 != (argc % 2)) || (argc < 3))
    {
        displayUsage (argv[0]);
        return (0);
    }

    for (i = 1; i < argc; i += 2)
    {
        if ("-compress" == argv[i])
        {
            if ("true" == argv[i + 1])
            {
                enableCompression = true;
            }
            else
            {
                enableCompression = false;
            }
        }
        else if ("-store" == argv[i])
        {
            if ("true" == argv[i + 1])
            {
                storeUniqueChunkData = true;
            }
            else
            {
                storeUniqueChunkData = false;
            }
        }
        else if ("-resetperfile" == argv[i])
        {
            if ("true" == argv[i + 1])
            {
                resetPerFile = true;
            }
            else
            {
                resetPerFile = false;
            }
        }
        else if ("-filenameonly" == argv[i])
        {
            if ("true" == argv[i + 1])
            {
                printFileNameOnly = true;
            }
            else
            {
                printFileNameOnly = false;
            }
        }
        else if ("-perfilestats" == argv[i])
        {
            if ("true" == argv[i + 1])
            {
                printPerFileStatistics = true;
            }
            else
            {
                printPerFileStatistics = false;
            }
        }
        else if ("-chunk" == argv[i])
        {
            fixedChunkSize = atoi ((argv[i + 1]).c_str ());
        }
        else if ("-minchunk" == argv[i])
        {
            minimumChunkSize = atoi ((argv[i + 1]).c_str ());
            useVariableChunking = true;
        }
        else if ("-maxchunk" == argv[i])
        {
            maximumChunkSize = atoi ((argv[i + 1]).c_str ());
            useVariableChunking = true;
        }
        else if ("-ro64" == argv[i])
        {
            if ("true" == argv[i + 1])
            {
                useRedundancyOptimizer64 = true;
            }
            else
            {
                useRedundancyOptimizer64 = false;
            }
        }
        else if ("-path" == argv[i])
        {
            pathsToReduce.push_back (argv[i + 1]);
        }
        else if ("-file" == argv[i])
        {
            filesToReduce.push_back (argv[i + 1]);
        }
        else
        {
            cerr << endl;
            cerr << "Unknown command line option : \"" << argv[i] << "\"" << endl;
            cerr << endl;
            displayUsage (argv[0]);
            return (1);
        }
    }

    numberOfPathsToReduce = pathsToReduce.size ();
    numberOfFilesToReduce = filesToReduce.size ();

    if ((0 == (pathsToReduce.size ())) && (0 == (filesToReduce.size ())))
    {
        displayUsage (argv[0]);
        return (1);
    }

    RedundancyOptimizerBase *pRo = NULL;

    if (true == useRedundancyOptimizer64)
    {
        if (true == useVariableChunking)
        {
            RedundancyOptimizer64WithVariableChunking *pRo64wvc = new RedundancyOptimizer64WithVariableChunking;

            pRo64wvc->setMinimumChunkSize (minimumChunkSize);
            pRo64wvc->setMaximumChunkSize (maximumChunkSize);

            pRo = pRo64wvc;
        }
        else
        {
            pRo = new RedundancyOptimizer64;
        }
    }
    else
    {
        pRo = new RedundancyOptimizer;
    }

    pRo->setEnableCompression (enableCompression);
    pRo->setStoreUniqueChunkData (storeUniqueChunkData);
    pRo->setResetPerFile (resetPerFile);
    pRo->setPrintFileNameOnly (printFileNameOnly);
    pRo->setPrintPerFileStatistics (printPerFileStatistics);
    pRo->setFixedChunkSize (fixedChunkSize);

    for (i = 0; i < numberOfPathsToReduce; i++)
    {
        pRo->optimizePath (pathsToReduce[i]);
    }

    if (0 != numberOfFilesToReduce)
    {
        if (true == (pRo->getPrintPerFileStatistics ()))
        {
            cout << endl;
            cout << endl;
            cout << endl;
            pRo->printStatisticsHeader ();
        }
    }

    for (i = 0; i < numberOfFilesToReduce; i++)
    {
        pRo->optimizeFile (filesToReduce[i]);

        if (true == (pRo->getPrintPerFileStatistics ()))
        {
            pRo->printStatisticsHorizontal (filesToReduce[i]);
            pRo->printStatisticsHeader2 ();
        }

        if (true == (pRo->getResetPerFile ()))
        {
            pRo->reset ();
        }
        else
        {
            pRo->resetCounts ();
        }
    }

    if (0 != numberOfFilesToReduce)
    {
        cout << endl;
        cout << endl;
        cout << endl;
        pRo->printGlobalStatisticsHeader ();
        pRo->printGlobalStatisticsHorizontal ();
        pRo->printGlobalStatisticsHeader2 ();
    }

    if (true == useVariableChunking)
    {
        pRo->print ();
    }

    delete pRo;

    return (0);
}

}
