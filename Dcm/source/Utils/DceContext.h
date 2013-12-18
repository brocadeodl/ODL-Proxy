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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Author : Ganesh Sindigi
 *
 *   Base class for Dce contexts with caching support.
 ***************************************************************************/

#ifndef DCECONTEXT_H_
#define DCECONTEXT_H_

#include <vector>
#include "DataCache.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs {
    class DceContext {
    public:
        DceContext();
        virtual ~DceContext();
        ObjectId    getWaveManagedObjectId                () const;
        void        setWaveManagedObjectId                (const ObjectId &waveManagedObjectId);
        DataCache    *getCache() const;

        virtual void addManagedObjectsForGarbageCollection (const vector<WaveManagedObject *> &mos){}
        virtual void addManagedObjectForGarbageCollection  (WaveManagedObject *mo){}

    // Data Members
    private:
        ObjectId    m_waveManagedObjctId;
        DataCache*   const m_dceCache ;
    };


}



#endif /* DCECONTEXT_H_ */
