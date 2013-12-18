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
 *   Copyright (C) 2013      Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Kuai Yu                                                      *
 ***************************************************************************/

#ifndef CLIBLOCKCONTEXT
#define CLIBLOCKCONTEXT

#include "Framework/Types/Types.h"

using namespace std;

namespace WaveNs
{

class CliBlockContext
{
    private :
    protected :
    public :
                                    CliBlockContext                      ();
                                    CliBlockContext                      (ResourceId reason);
                                    CliBlockContext                      (const CliBlockContext &context);
                                   ~CliBlockContext                      ();
          CliBlockContext&          operator=                            (const CliBlockContext &context);

    const ResourceId                getReason                           () const;  
          void                      setReason                           (ResourceId reason);
                
           // Now the data members

    private :
            ResourceId              m_reason;

    protected :
    public :

};
}

#endif // CLIBLOCKCONTEXT
