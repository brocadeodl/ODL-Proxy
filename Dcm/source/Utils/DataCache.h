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
 *   Provides Caching support. Adds the given key-value pair to the cache.
 *   When the Cache is destructed, corresponding value's destructor is invoked.
 *   Behavior is undefined if the type of value asked is different from what's
 *   being put!
 ***************************************************************************/

#ifndef DATACACHE_H_
#define DATACACHE_H_

#include <iostream>
#include <map>
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/ObjectId.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs {
    class CacheValue {
    public:
        virtual ~CacheValue() {
        }
    };

    class DataCache {
    private:
        map<string, CacheValue*> contents;
    public:
        DataCache();
        /**
         * All the elements in the cache are dropped; corresponding destructors are
         * called and then they are removed from the cache.
         */

        ~DataCache();

        CacheValue* getCacheValue(string key) const;
        void put(string key, CacheValue* value);

        string getStringValue(string key) const;
        void put(string key, string value);

        int getIntValue(string key) const;
        void put(string key, int value);

        //Methods to set and get the object ids.
        ObjectId getObjectId(string key) const;
        void put(string key, ObjectId objectId);

        WaveManagedObject* getWaveManagedObject(string key) const;
        void put(string key, WaveManagedObject* value, bool deleteOnEvict =
                false);

        /**
         * Removes the element specified by key, calls removed element's destructor.
         */

        void remove(string key);

        /**
         * Removes all the elements, calls removed element's destructor.
         */

        void clear();

        /**
         * Returns number of elements in this cache.
         */
        size_t size();

    };

}

#endif // DATACACHE_H_
