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

#ifndef QUERYUTILS_H
#define QUERYUTILS_H

#include <string>
#include "Framework/ObjectRelationalMapping/OrmRepository.h"

using namespace std;

namespace WaveNs
{

class WaveManagedObjectSynchronousQueryContext;

class QueryUtils
{
    private :
    protected :
    public :
        static  vector<WaveManagedObject *> *querySynchronouslyByName (const string &managedClassName, const string &managedObjectName, const string &schema = OrmRepository::getWaveCurrentSchema ());   
        static  vector<WaveManagedObject *> *querySynchronously (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext);

    // Now the data members
    
    private :
    protected :
    public :
};

}

#endif //QUERYUTILS_H
