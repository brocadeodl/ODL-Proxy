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

#include "Framework/Types/Types.h"
#include "WaveManagedObjectToolKit.h"
#include "WaveManagedObject.h"
#include "Framework/Attributes/Attribute.h"

namespace WaveNs
{

/// Name
/// WaveManagedObjectToolKit
/// Description
/// No op constructor. Should never be called.
/// Input
/// none
/// Output
/// none
/// Return
/// none  
WaveManagedObjectToolKit::WaveManagedObjectToolKit()
{
}
/// Name
/// ~WaveManagedObjectToolKit
/// Description
/// Destructor. Should never be called.
/// Input
/// none
/// Output
/// none
/// Return
/// none  
WaveManagedObjectToolKit::~WaveManagedObjectToolKit()
{
}

/// Name
/// releaseMemoryOfWaveMOVector
/// Description
/// Helper function to release memory from 
/// transient objects created due to DB query
/// Input
/// Reference to pointer of a vector containing pointers 
/// to Managed Objects 
/// Output
/// none
/// Return
/// none
void WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(vector<WaveManagedObject *>* & pResults)
{
    if (pResults)
    {
        UI32 nNodes = pResults->size();
        for (UI32 i = 0; i < nNodes; i++)
        {
            if ((*pResults)[i])
            {
                delete (*pResults)[i];
            }
        }

        pResults->clear ();
        delete pResults;
        pResults = 0;
    }
    return;
}

void WaveManagedObjectToolKit::releaseMemoryOfAttributeVector(vector<Attribute *> &attrVector )
{
    if ( false ==  attrVector.empty() )
    {   
        UI32 size = attrVector.size();
        for ( UI32 i = 0; i < size; i++)
        {   
            if ( NULL != attrVector[i] )
            {   
                delete ( attrVector[i] );
            }
        }
        attrVector.clear();
    }
    return ;
}

}
