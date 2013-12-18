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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef RMONHISTORYLOCALMANAGEDOBJECT_H
#define RMONHISTORYLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class RmonHistoryLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            RmonHistoryLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            RmonHistoryLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const SI32 &historyIndex,const SI32 &buckets,const SI32 &interval,const string &owner);
            virtual    ~RmonHistoryLocalManagedObject ();
            static string getClassName();
            void setHistoryIndex(const SI32 &historyIndex);
            SI32 getHistoryIndex()  const;
            void setBuckets(const SI32 &buckets);
            SI32 getBuckets()  const;
            void setInterval(const SI32 &interval);
            SI32 getInterval()  const;
            void setOwner(const string &owner);
            string getOwner()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_historyIndex;
            SI32  m_buckets;
            SI32  m_interval;
            string  m_owner;
    };
}
#endif                                            //RMONHISTORYLOCALMANAGEDOBJECT_H
