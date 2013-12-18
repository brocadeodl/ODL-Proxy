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

#ifndef WAVEPROFILER_H
#define WAVEPROFILER_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <string>

using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

void __cyg_profile_func_enter (void *pThisFunction, void *pFromCallSite) __attribute__ ((no_instrument_function));
void __cyg_profile_func_exit  (void *pThisFunction, void *pFromCallSite) __attribute__ ((no_instrument_function));
void   main_constructor       (void)                                     __attribute__ ((no_instrument_function, constructor));
void   main_destructor        (void)                                     __attribute__ ((no_instrument_function, destructor));

#ifdef __cplusplus
}
#endif

namespace WaveNs
{

class WaveProfiler
{
    private :
    protected :
    public :
        static string getProfileFileName ();
        static void   setProfileFileName (const string &profileFileName);

    // Now the data members

    private :
        static string         m_profileFileName;

    protected :
    public :
};

}

#endif // WAVEPROFILER_H

