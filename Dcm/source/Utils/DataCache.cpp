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
 *   Provides Caching support.
 ***************************************************************************/
#include "DataCache.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs {

    //TODO: Its possible to have a template mechanism so that individual
    // classes can be avoided; Needs some more cleaning and abstraction

    class IntCV: public CacheValue {
    private:
        const int value;
    public:
        IntCV(int val) :
                CacheValue(), value(val) {
        }
        int getValue() const {
            return value;
        }
    };

    class StringCV: public CacheValue {
    private:
        string value;
    public:
        StringCV(string &s) :
                CacheValue(), value(s) {
        }
        ~StringCV() {
        }
        string getValue() const {
            return value;
        }
    };

    class WaveMOCV: public CacheValue {
    private:
        WaveManagedObject* pWaveMO;
        bool deleteOnEvict;
    public:
        WaveMOCV(WaveManagedObject* ptrWaveMO, bool pDeleteOnEvict) :
                CacheValue(), pWaveMO(ptrWaveMO), deleteOnEvict(pDeleteOnEvict) {
        }
        ~WaveMOCV() {
            if (deleteOnEvict) {
                tracePrintf(TRACE_LEVEL_DEBUG, "Deleting pointer to MO...");
                delete pWaveMO;
            }
        }
        WaveManagedObject* getValue() const {
            return pWaveMO;
        }

    };

    class ObjectIdCV: public CacheValue {
        private:
            ObjectId objectIdValue;
        public:
            ObjectIdCV(ObjectId objectId) :
                CacheValue(), objectIdValue(objectId) {
            }

            ~ObjectIdCV() {
            }

            ObjectId getValue() const {
                return objectIdValue;
            }
    };

    DataCache::DataCache() {
    }
    DataCache::~DataCache() {
        clear();
    }

    CacheValue* DataCache::getCacheValue(string key) const {
        tracePrintf(TRACE_LEVEL_DEBUG, "getting value for key : %s", key.c_str());
        map<string, CacheValue*>::const_iterator pos = contents.find(key);
        CacheValue* cv = NULL;
        if (pos != contents.end()) {
            tracePrintf(TRACE_LEVEL_DEBUG,"iterator has values...");
            cv = pos->second;
        }
        return cv;
    }

    string DataCache::getStringValue(string key) const {
        CacheValue* cv = getCacheValue(key);
        string value;
        if (cv) {
            StringCV* scv = static_cast<StringCV*>(cv);
            value = scv->getValue();
        }
        return value;
    }

    void DataCache::put(string key, string value) {
        CacheValue *p = new StringCV(value);
        contents[key] = p;
    }

    WaveManagedObject* DataCache::getWaveManagedObject(string key) const {
        CacheValue* cv = getCacheValue(key);
        WaveManagedObject* value = NULL;
        if (cv) {
            WaveMOCV* waveMOCV = static_cast<WaveMOCV*>(cv);
            value = waveMOCV->getValue();
        }
        return value;
    }

    void DataCache::put(string key, WaveManagedObject* value,
            bool deleteOnEvict) {
        CacheValue *p = new WaveMOCV(value, deleteOnEvict);
        contents[key] = p;
    }

    ObjectId DataCache::getObjectId(string key) const {
        
        CacheValue* cv = getCacheValue(key);
        ObjectId objectId = ObjectId::NullObjectId;
        if(cv) {
            ObjectIdCV* objectIdCV = static_cast<ObjectIdCV*>(cv); 
            objectId = objectIdCV->getValue();
        }

        return objectId;
    }

    void DataCache::put(string key, ObjectId objectId) {
        CacheValue *p = new ObjectIdCV(objectId);
        contents[key] = p;
    }


    int DataCache::getIntValue(string key) const {
        CacheValue* cv = getCacheValue(key);
        int value = -1;
        if (cv) {
            IntCV* icv = static_cast<IntCV*>(cv);
            value = icv->getValue();
        }
        return value;
    }

    void DataCache::put(string key, int value) {
        CacheValue *p = new IntCV(value);
        contents[key] = p;
    }

    void DataCache::remove(string key) {
        CacheValue* cv = getCacheValue(key);
        if (cv) {
            delete cv;
            contents.erase(key);
        }
    }

    void DataCache::clear() {
        for (map<string, CacheValue*>::iterator it = contents.begin();
                it != contents.end(); ++it) {
            delete it->second;
        }
        contents.clear();
    }

    size_t DataCache::size() {
        return contents.size();
    }
}

/*

// Un-comment while testing
using namespace DcmNs;

int main() {
    string key = "key", keyValue = "keyValue";

    DataCache c;
    //c.put("i", pi);
    c.put(key, keyValue);

    string newKey = key + "3";
    cout << "Cache contains " << c.size() << " values." << endl;
    cout << "String Exist Case: Key - " << key << ", value - "
            << c.getStringValue(key) << endl;
    cout << "NonExist Case: Key - " << newKey << ", value - "
            << c.getStringValue(newKey) << endl;
    c.remove(key);
    cout << "PostRemove-Exist Case: Key - " << key << ", value - "
            << c.getStringValue(key) << endl;

    c.put(newKey, 3);
    cout << "Int Exist Case: Key - " << newKey << ", value - "
            << c.getIntValue(newKey) << endl;

    return 0;
}
*/
