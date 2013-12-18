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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vikas B Dharwadkar                                           *
 *                                                                         *
 *   This header file contains DCE Cluster utility definitions/APIs.       *
 ***************************************************************************/
#ifndef DCE_CLUSTER_UTILS_H
#define DCE_CLUSTER_UTILS_H

#include "Framework/Core/PrismFrameworkMessages.h"

using namespace WaveNs;

namespace DcmNs {

typedef struct dce_cluster_validation_data_record_ {
    uint32_t    type;
    uint32_t    len;
    uint8_t     val[1];
} dce_cluster_validation_data_record_t;

typedef struct dce_cluster_validation_data_ {
    uint32_t                                size;
    uint32_t                                max_size;
    uint32_t                                version;
    uint32_t                                num_records;
    uint64_t                                cap_bits;
    dce_cluster_validation_data_record_t    rec_list[1];
} dce_cluster_validation_data_t;


#define DCE_CLUSTER_VALDN_DATA_SET_CAP_BITS(_data, _cap_bmp) \
{ \
    (_data).cap_bits = (_cap_bmp); \
}

#define DCE_CLUSTER_VALDN_DATA_RESET_CAP_BITS(_data) \
{ \
    (_data).cap_bits = 0; \
}

#define DCE_CLUSTER_VALDN_DATA_ADD_CAP(_data, _cap) \
{ \
   (_data).cap_bits |= ( 1 << (_cap)); \
}

#define DCE_CLUSTER_VALDN_DATA_REM_CAP(_data, _cap) \
{ \
    (_data).cap_bits &= ~(1 << (_cap)); \
}

#define DCE_CLUSTER_VALDN_DATA_ADD_CAP_BIT(_data, _cap_bit) \
{ \
    (_data).cap_bits |= (_cap_bit); \
}

#define DCE_CLUSTER_VALDN_DATA_REM_CAP_BIT(_data, _cap_bit) \
{ \
    (_data).cap_bits &= ~(_cap_bit); \
}

#define PLUG_ASSERT(exp) \
    prismAssert ((exp), __FILE__, __LINE__);

#define PLUG_ASSERT_PTR(exp) \
    prismAssert ((exp) != NULL, __FILE__, __LINE__);

#define PLUG_DBG_TRACE() \
        {\
                  char nsm_cpw_debug_buffer[512]; \
                  snprintf(nsm_cpw_debug_buffer, 512,\
                                     "%s:%s:%d: Entered Function", __FILE__, __FUNCTION__, __LINE__);\
                  trace(TRACE_LEVEL_DEBUG, __FUNCTION__);\
                }

#define PLUG_DBG(msg) \
        { \
                    char nsm_cpw_debug_buffer[512]; \
                    snprintf(nsm_cpw_debug_buffer, 512,\
                                         "%s:%s:%d:%s", __FILE__, __FUNCTION__, __LINE__, msg); \
                    trace(TRACE_LEVEL_DEBUG, string(nsm_cpw_debug_buffer)); \
                }

#define PLUG_DBG_ERR(msg) \
        { \
                    char nsm_cpw_debug_buffer[512]; \
                    snprintf(nsm_cpw_debug_buffer, 512,\
                                         "ERROR: %s:%s:%d:%s", __FILE__, __FUNCTION__, __LINE__, msg); \
                    trace(TRACE_LEVEL_ERROR, string(nsm_cpw_debug_buffer)); \
                }

#define PLUG_DBG_ERR_S(msg) \
        { \
                    char nsm_cpw_debug_buffer[512]; \
                    snprintf(nsm_cpw_debug_buffer, 512,\
                                         "ERROR: %s:%s:%d", __FILE__, __FUNCTION__, __LINE__); \
                    trace(TRACE_LEVEL_ERROR, string(nsm_cpw_debug_buffer) + msg ); \
                }

#define PLUG_DBG_S(msg) \
        { \
                    char nsm_cpw_debug_buffer[512]; \
                    snprintf(nsm_cpw_debug_buffer, 512,\
                                         "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
                    trace(TRACE_LEVEL_DEBUG, string(nsm_cpw_debug_buffer) + msg); \
        }

#define PLUG_INFO_S(msg) \
        { \
                    char nsm_cpw_debug_buffer[512]; \
                    snprintf(nsm_cpw_debug_buffer, 512,\
                                         "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
                    trace(TRACE_LEVEL_INFO, string(nsm_cpw_debug_buffer) + msg); \
        }

#define PLUG_TRACE(_tag_, _trace_lvl_, _msg_)                                           \
        {                                                                                   \
                    char _debug_buffer[512];                                                \
                    snprintf(_debug_buffer, 512, "[%s] %s:%s:%d: ",                         \
                        _tag_, __FILE__, __FUNCTION__, __LINE__);                           \
                    trace(_trace_lvl_, string(_debug_buffer) + _msg_);                      \
        }

/*
dce_cluster_validation_collect_init()

Desc    : Initialize the Cluster Validation Data.

Args    : @version : Current version of plugin in this release.

Returns : Pointer the validation data if SUCCESS.
          NULL on failure.
*/
dce_cluster_validation_data_t *
dce_cluster_validation_collect_init (uint32_t version);

/*
dce_cluster_validation_collect_add_record ()
Desc    : Adds a plugin specific record to the validation data.

Args    : @data_p = Double Pointer to the validation data received from
                    the 'dce_cluster_validation_collect_init()' call.
          @type = Plugin specific 'type'. The type should be greater
                  than 64, as first 64 types are reserved for 
                  well-known/future record types.
          @len  = Length of data in bytes.
          @val_p = Pointer to the data.

Returns : DCE_CLUSTER_UTIL_SUCCESS on SUCCESS
          DCE_CLUSTER_UTIL_xxx on FAILURE
*/
ResourceId
dce_cluster_validation_collect_add_record (dce_cluster_validation_data_t **data_p,
                                           uint32_t type, uint32_t len, 
                                           uint8_t * val_p);


/*
dce_cluster_validation_collect_done ()

Desc    : Completes the 'Cluster Validation Data' collection and adds the data
          to the collect validation message.

Args    : @pMessage = Message pointer received in
                      'clusterCreateCollectValidationData()' call.
          @data_p = Cluster validation data recieved from 
                    'dce_cluster_validation_collect_init()' call.

Returns : DCE_CLUSTER_UTIL_SUCCESS on SUCCESS
          DCE_CLUSTER_UTIL_xxx on FAILURE
*/
ResourceId
dce_cluster_validation_collect_done (WaveObjectManagerCollectValidationDataMessage *pMessage,
                                     dce_cluster_validation_data_t *data_p);
/*
dce_cluster_validation_parse()

Desc    : Parses the validation data received from the primary node.

Args    : @data_p = Pointer to the validation data received from the primary node.
          @version_p (OUT) = Contains the version of plugin on the primary node.
          @num_records_p (OUT) = Contains the number of records in the 
                                 validation data.
Returns : DCE_CLUSTER_UTIL_SUCCESS on SUCCESS
          DCE_CLUSTER_UTIL_xxx on FAILURE
*/          
ResourceId
dce_cluster_validation_parse (dce_cluster_validation_data_t *data_p,
                              uint32_t *version_p, uint64_t *cap_bits_p,
                              uint32_t *num_records_p);

/*
dce_cluster_validation_get_record()

Desc    : Get the details of a record in the validation data.

Args    : @data_p = Pointer to the validation data received from the primary node.
          @record_index = Index of record (0 Based)
          @type_p (OUT) = Contains the 'type' of the record.
          @len_p (OUT) = Contains the 'len' of the record in Bytes.
          @val_pp (OUT) = Double Pointer to the value of the data. (Read-only, DO NOT FREE).

Returns : DCE_CLUSTER_UTIL_SUCCESS on SUCCESS
          DCE_CLUSTER_UTIL_xxx on FAILURE
*/          
ResourceId
dce_cluster_validation_get_record (dce_cluster_validation_data_t *data_p,
                                   uint32_t record_index,
                                   uint32_t *type_p,
                                   uint32_t *len_p,
                                   uint8_t **val_pp);

void
ClusterGetErrorStringForFailedLocation(WaveSendToClusterContext  *ctx_p, string &errString);

void
ClusterGetErrorStringWithArgsForFailedLocation(WaveSendToClusterContext  *ctx_p, string &errString,vector< vector <Attribute *> > &ErrArguments);

ResourceId
ClusterGetStatusFromContext(WaveSendToClusterContext  *ctx_p);

ResourceId
ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid);

ResourceId 
WyserEaConfigurationErrorCode(WaveNs::PrismMessage* msg_p, ResourceId status,vector <Attribute *> &errorArguments, bool isPartialSuccessEnabled = true);

void PrintErrorCode(WaveNs::PrismMessage* msg_p, ResourceId &status,vector <Attribute *> &errorArguments,  bool isPartialSuccessEnabled = true);

int  getIfShortName(const char *ifType,const char *ifName, string &shortIfName, LocationId &locationId );

ResourceId
ClusterGetPhase1StatusFromContextForLocation(WaveSendToClusterContext *ctx_p,
                                             LocationId &locationid);

ResourceId
ClusterGetPhase2StatusFromContextForLocation(WaveSendToClusterContext *ctx_p,
                                             LocationId &locationid);

/**
 * Resolve the locations ids for sending L3 show commands.. The command is
 * assumed to be of syntax "<tokens> [ rbridge-id [ <id>|<range>|all ] ]".
 * Decrements the 'argc' value by 2 if the 'rbridge-id' information exists in
 * the command. Otherwise fills the location id of current node in the locations
 * vector.
 */
ResourceId
resolveLocations (int &argc, char **argv, vector<LocationId> &locations);

ResourceId
getLocationIdsForRbridges (const string &rbridges, vector<LocationId> &locations);

ResourceId
parseVrfToken (int &argc, char **argv, string  &vrfName);

ResourceId
resolveLocationsAndVrf(int &argc, char **argv, vector<LocationId> &locations, string &vrfName);

}
#endif
