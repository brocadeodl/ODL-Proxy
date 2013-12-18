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

#ifndef SSM_PLUG_UTILS_H 
#define SSM_PLUG_UTILS_H 


#define SSM_PLUG_ASSERT(exp) \
	        prismAssert ((exp), __FILE__, __LINE__);

#define SSM_PLUG_ASSERT_PTR(exp) \
	        prismAssert ((exp) != NULL, __FILE__, __LINE__);

#define SSM_PLUG_DBG_TRACE() \
	    {\
			      char debug_buffer[512]; \
			      snprintf(debug_buffer, 512,\
					                 "%s:%s(%d): Entered Function", __FILE__, __FUNCTION__, __LINE__);\
			      trace(TRACE_LEVEL_DEBUG, __FUNCTION__);\
			    }

#define SSM_PLUG_DBG_LVL(msg) \
	    { \
			        char debug_buffer[512]; \
			        snprintf(debug_buffer, 512,\
						                 "%s:%s(%d) - %s", __FILE__, __FUNCTION__, __LINE__, msg); \
			        trace(TRACE_LEVEL_DEBUG, string(debug_buffer)); \
			    }

#define SSM_PLUG_ERR_LVL(msg) \
	    { \
			        char debug_buffer[512]; \
			        snprintf(debug_buffer, 512,\
						                 "%s:%s(%d) - %s", __FILE__, __FUNCTION__, __LINE__, msg); \
			        trace(TRACE_LEVEL_ERROR, string(debug_buffer)); \
		}

#define SSM_PLUG_SUCCESS_LVL(msg) \
	    { \
			        char debug_buffer[512]; \
			        snprintf(debug_buffer, 512,\
						                 "%s:%s(%d) - %s", __FILE__, __FUNCTION__, __LINE__, msg); \
			        trace(TRACE_LEVEL_SUCCESS, string(debug_buffer)); \
		}

#define SSM_PLUG_INFO_LVL(msg) \
	    { \
			        char debug_buffer[512]; \
			        snprintf(debug_buffer, 512,\
						                 "%s:%s(%d) - %s", __FILE__, __FUNCTION__, __LINE__, msg); \
			        trace(TRACE_LEVEL_INFO, string(debug_buffer)); \
	    }

#define SSM_PLUG_TRACE_SET_LVL(level, msg) \
		{ \
				char debug_buffer[512]; \
				snprintf(debug_buffer, 512,\
								"%s:%s(%d) - %s", __FILE__, __FUNCTION__, __LINE__, msg); \
				trace(level, string(debug_buffer)); \
		}

#define SSM_PLUG_SUCCESS_LVL_S(msg) \
	    { \
			        char debug_buffer[512]; \
			        snprintf(debug_buffer, 512,\
						                 "%s:%s(%d) - ", __FILE__, __FUNCTION__, __LINE__); \
			        trace(TRACE_LEVEL_SUCCESS, string(debug_buffer) + msg); \
		}

#define SSM_PLUG_INFO_LVL_S(msg) \
	    { \
			        char debug_buffer[512]; \
			        snprintf(debug_buffer, 512,\
						                 "%s:%s(%d) - ", __FILE__, __FUNCTION__, __LINE__); \
			        trace(TRACE_LEVEL_INFO, string(debug_buffer) + msg); \
	    }
#define SSM_PLUG_ERR_LVL_S(msg) \
	    { \
			        char debug_buffer[512]; \
			        snprintf(debug_buffer, 512,\
						                 "ERROR: %s:%s(%d) - ", __FILE__, __FUNCTION__, __LINE__); \
			        trace(TRACE_LEVEL_ERROR, string(debug_buffer) + msg ); \
			    }


#define SSM_PLUG_DBG_LVL_S(msg) \
	    { \
			        char debug_buffer[512]; \
			        snprintf(debug_buffer, 512,\
						                 "%s:%s(%d) - ", __FILE__, __FUNCTION__, __LINE__); \
			        trace(TRACE_LEVEL_DEBUG, string(debug_buffer) + msg); \
	    }



#endif  //SSM_PLUG_UTILS_H

