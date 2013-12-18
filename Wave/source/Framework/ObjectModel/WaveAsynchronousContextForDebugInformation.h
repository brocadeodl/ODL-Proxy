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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef WAVEASYNCHRONOUSCONTEXTFORDEBUGINFORMATION_H
#define WAVEASYNCHRONOUSCONTEXTFORDEBUGINFORMATION_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismAsynchronousContext.h"

namespace WaveNs
{

class WaveAsynchronousContextForDebugInformation : public PrismAsynchronousContext
{
    private :
    protected :
    public :
                                    WaveAsynchronousContextForDebugInformation  (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL);
        virtual                    ~WaveAsynchronousContextForDebugInformation  ();

                void                setDebugInformation                         (const string &debugInformation);
                const   string &    getDebugInformation                         () const;

    // Now the data members

    private :
                        string      m_debugInformation;

    protected :
    public :
};

}

#endif // WAVEASYNCHRONOUSCONTEXTFORDEBUGINFORMATION_H
