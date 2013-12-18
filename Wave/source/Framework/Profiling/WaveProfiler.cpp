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

#include "Framework/Profiling/WaveProfiler.h"
#include "Framework/Utils/PrismMutex.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

extern "C"
{

#include <dlfcn.h>

}

extern "C" char s_profileFileName[256];

extern "C" char *cplus_demangle (const char *mangled, int options);

namespace WaveNs
{

string WaveProfiler::m_profileFileName = "WaveProfileData.pwd";

string WaveProfiler::getProfileFileName ()
{
    return (m_profileFileName);
}

void WaveProfiler::setProfileFileName (const string &profileFileName)
{
    m_profileFileName = profileFileName;

    strncpy (s_profileFileName, (WaveProfiler::getProfileFileName ()).c_str (), 255);
    s_profileFileName[255] = '\0';
}

}

using namespace WaveNs;
using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

static void            **s_pProfileFunctions         = NULL;
static void            **s_pProfileFunctionCallSites = NULL;
static bool             *s_pProfileFunctionType      = NULL;
static UI64             *s_pProfileFunctionTimestamp = NULL;
static UI32              s_profileSize               = 10 * 1024 * 1024;
static UI32              s_profileIndex              = 0;
static pthread_mutex_t   s_profileMutex;
static UI32              s_profilingActive           = 0;

       char              s_profileFileName[256];

void __cyg_profile_func_enter (void *pThisFunction, void *pFromCallSite)
{
    if (0 == s_profilingActive)
    {
        return;
    }

    pthread_mutex_lock (&s_profileMutex);
#if 0
    Dl_info symbolInformation;

    if (0 != (dladdr (pThisFunction, &symbolInformation)))
    {
        if (NULL != symbolInformation.dli_sname)
        {
            printf ("%s\n", symbolInformation.dli_sname);
        }
    }
#endif
    if ((NULL != s_pProfileFunctions) && (s_profileIndex < s_profileSize))
    {
        s_pProfileFunctions[s_profileIndex]         = pThisFunction;
        s_pProfileFunctionCallSites[s_profileIndex] = pFromCallSite;
        s_pProfileFunctionType[s_profileIndex]      = true;
        s_pProfileFunctionTimestamp[s_profileIndex] = 0;

        s_profileIndex++;
    }

    if (s_profileIndex == s_profileSize)
    {
        //cout << "Reached Limit" << endl;
    }

    pthread_mutex_unlock (&s_profileMutex);
}

void __cyg_profile_func_exit (void *pThisFunction, void *pFromCallSite)
{
    if (0 == s_profilingActive)
    {
        return;
    }

    pthread_mutex_lock (&s_profileMutex);
#if 0
    Dl_info symbolInformation;

    if (0 != (dladdr (pThisFunction, &symbolInformation)))
    {
        if (NULL != symbolInformation.dli_sname)
        {
            printf ("%s\n", symbolInformation.dli_sname);
        }
    }
#endif
    if ((NULL != s_pProfileFunctions) && (s_profileIndex < s_profileSize))
    {
        s_pProfileFunctions[s_profileIndex]         = pThisFunction;
        s_pProfileFunctionCallSites[s_profileIndex] = pFromCallSite;
        s_pProfileFunctionType[s_profileIndex]      = false;
        s_pProfileFunctionTimestamp[s_profileIndex] = 0;

        s_profileIndex++;
    }

    pthread_mutex_unlock (&s_profileMutex);
}

void main_constructor (void)
{
    if (NULL == getenv ("WAVE_PROFILE"))
    {
        return;
    }

    pthread_mutex_init (&s_profileMutex, NULL);

    s_pProfileFunctions         = (void **) malloc (s_profileSize * sizeof (void *));
    s_pProfileFunctionCallSites = (void **) malloc (s_profileSize * sizeof (void *));
    s_pProfileFunctionType      = (bool *)  malloc (s_profileSize * sizeof (bool));
    s_pProfileFunctionTimestamp = (UI64 *)  malloc (s_profileSize * sizeof (UI64));
    s_profileIndex              = 0;
    s_profilingActive           = 1;
}

void main_destructor (void)
{
    if (NULL == getenv ("WAVE_PROFILE"))
    {
        return;
    }

    if (0 == s_profileIndex)
    {
        return;
    }

    string profileFileName = s_profileFileName;

    ifstream profileFileToRead (profileFileName.c_str ());

    if (false == (profileFileToRead.is_open ()))
    {
        ofstream profileFileToWrite (profileFileName.c_str ());

        profileFileToWrite.close ();
    }
    else
    {
        profileFileToRead.seekg (0, ios::end);
    }

    fstream profileFile (profileFileName.c_str (), ios::in | ios::out);

    profileFile.seekg (0, ios::end);

    UI64    numberOfEntries               = s_profileIndex;
    UI64    i                             = 0;
    Dl_info symbolInformation;
    Dl_info symbolInformationForCallSite;
    int     symbolStatus                  = 0;
    int     symbolForCallSiteStatus       = 0;
    UI64    totalEffectiveNumberOfEntries = 0;
    char   *pDemangledSymbol              = NULL;
    char   *pDemangledSymbolForCallsite   = NULL;

    cout << "Number Of           Profile Entries : " << numberOfEntries << endl;

    for (i = 0; i < numberOfEntries; i++)
    {
        symbolForCallSiteStatus = dladdr (s_pProfileFunctionCallSites[i], &symbolInformationForCallSite);
        symbolStatus            = dladdr (s_pProfileFunctions[i], &symbolInformation);

        if ((0 != symbolStatus)                              &&
            (0 != symbolForCallSiteStatus)                   &&
            (NULL != symbolInformationForCallSite.dli_sname) &&
            (NULL != symbolInformation.dli_sname))
        {
            profileFile << (s_pProfileFunctionType[i] ? "E " : "X ");
            profileFile << s_pProfileFunctionCallSites[i] << " ";
            profileFile << s_pProfileFunctions[i] << " ";

            pDemangledSymbolForCallsite = cplus_demangle (symbolInformationForCallSite.dli_sname, 0);

            if (NULL != pDemangledSymbolForCallsite)
            {
                profileFile << pDemangledSymbolForCallsite << " ";

                free (pDemangledSymbolForCallsite);
            }
            else
            {
                profileFile << symbolInformationForCallSite.dli_sname << " ";
            }

            pDemangledSymbol = cplus_demangle (symbolInformation.dli_sname, 0);

            if (NULL != pDemangledSymbol)
            {
                profileFile << pDemangledSymbol;

                free (pDemangledSymbol);
            }
            else
            {
                profileFile << symbolInformation.dli_sname;
            }

            profileFile << endl;

            totalEffectiveNumberOfEntries++;
        }

        if (0 == (i % 1000))
        {
            cout << "Processed                           : " << i << '\r';
            cout.flush ();
        }
    }

    cout << "Processed                           : " << i << endl;

    profileFile.close ();

    cout << "Number Of Effective Profile Entries : " << totalEffectiveNumberOfEntries << endl;
}

#ifdef __cplusplus
}
#endif
