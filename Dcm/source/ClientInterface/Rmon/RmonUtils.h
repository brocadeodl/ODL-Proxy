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

#ifndef _RMON_UTILS_H
#define _RMON_UTILS_H


#define RMON_PLUGIN_ASSERT(exp) \
            prismAssert ((exp), __FILE__, __LINE__);

#define RMON_PLUGIN_TRACE() \
        {\
                  char _debug_buffer[512]; \
                  snprintf(_debug_buffer, 512,\
                                     "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__);\
                  trace(TRACE_LEVEL_DEBUG, string(_debug_buffer));\
                }

#define RMON_PLUGIN_TRACE_DEBUG(msg) \
        {\
                  char _debug_buffer[512]; \
                  snprintf(_debug_buffer, 512,\
                                     "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__);\
                  trace(TRACE_LEVEL_DEBUG, string(_debug_buffer) + msg);\
                }

#define RMON_PLUGIN_TRACE_INFO(msg) \
        { \
                    char _debug_buffer[512]; \
                    snprintf(_debug_buffer, 512,\
                                         "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
                    trace(TRACE_LEVEL_INFO, string(_debug_buffer) + msg); \
                }

#define RMON_PLUGIN_TRACE_ERROR(msg) \
        { \
                    char _debug_buffer[512]; \
                    snprintf(_debug_buffer, 512,\
                                         "ERROR: %s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
                    trace(TRACE_LEVEL_ERROR, string(_debug_buffer) + msg); \
                }

#define RMON_PLUGIN_TRACE_FATAL(msg) \
        { \
                    char _debug_buffer[512]; \
                    snprintf(_debug_buffer, 512,\
                                         "ERROR: %s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
                    trace(TRACE_LEVEL_FATAL, string(_debug_buffer) + msg); \
                }

#define DEFAULT_COMMUNITY 		"__default_community"
#define DEFAULT_DESCRIPTION 	"__default_description"
#define DEFAULT_OWNER 			"RMON_SNMP"

static inline SI32 getEtherStatsIndexFromAlarmOid(string alarmOid)
{
	string  delimiter = ".";
	SI32    index = -1;
	size_t  pos = alarmOid.find_last_of(delimiter);

	if (pos == string::npos) { 
		return index;
	} 
	string etherStatsIdStr = alarmOid.substr(pos + 1);
	index = strtol(etherStatsIdStr.c_str(), NULL, 0);
	if (!index) { 
		index = -1;
	}
	
	return index;
}	

template <class T>
static void cleanUp(map<int, T*> &m) {
    if(!m.empty()) {
        typename map<int, T* >::iterator iter;
        for (iter = m.begin(); iter != m.end(); iter++) {
            if (iter->second)
                delete iter->second;
        }
        m.clear();
     }
}

template <class T>
static void cleanUp(multimap<int, T*> &m) {
    if(!m.empty()) {
        typename multimap<int, T* >::iterator iter;
        for (iter = m.begin(); iter != m.end(); iter++) {
            if (iter->second)
                delete iter->second;
        }
        m.clear();
   }
}


#endif 						// _RMON_UTILS_H
