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

#ifndef CLIBLOCKDETAIL
#define CLIBLOCKDETAIL

#include "Framework/Types/Types.h"
#include "Framework/CliBlockManagement/CliBlockContext.h"
#include "Framework/CliBlockManagement/CliBlockServiceIndependentMessage.h"

using namespace std;

namespace WaveNs
{

class CliBlockDetail
{
    private :
    protected :
    public :
                                    CliBlockDetail                      ();
                                    CliBlockDetail                      (PrismServiceId prismServiceId, LocationId originator, const CliBlockContext &cliBlockContext);
                                    CliBlockDetail                      (const CliBlockServiceIndependentMessage &cliBlockServiceIndependentMessage);
                                    CliBlockDetail                      (const CliBlockDetail &detail);
                                   ~CliBlockDetail                      ();
          CliBlockDetail&           operator=                           (const CliBlockDetail &detail);

    const PrismServiceId            getPrismServiceId                   () const;
    const LocationId                getOriginator                       () const;  
    const CliBlockContext&          getCliBlockContext                  () const;
                
           // Now the data members

    private :
            PrismServiceId            m_prismServiceId;
            LocationId                m_originator;
            CliBlockContext           m_cliBlockContext; 

    protected :
    public :

};
}

#endif // CLIBLOCKDETAIL
