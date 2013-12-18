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

#include <netinet/in.h>

#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "DcmCore/DcmToolKit.h"

#include <algorithm>

using namespace std;

#define DCE_CLUSTER_VALDN_DATA_INITIAL_SIZE  (sizeof(dce_cluster_validation_data_t) + 1024)

#define DCE_CLUSTER_VALDN_DATA_INCR_SIZE    (1024)

#define DCE_CLUSTER_VALDN_GET_TLV_LEN(len) (len + 2 * sizeof(uint32_t))

namespace DcmNs {

dce_cluster_validation_data_t *
dce_cluster_validation_collect_init (uint32_t version)
{
    dce_cluster_validation_data_t   *data_p = NULL;

    data_p = (dce_cluster_validation_data_t *) 
                  malloc(DCE_CLUSTER_VALDN_DATA_INITIAL_SIZE);
    if (data_p == NULL) {
        return NULL;
    }

    memset(data_p, 0, DCE_CLUSTER_VALDN_DATA_INITIAL_SIZE);

    data_p->version = version;
    data_p->size = sizeof(dce_cluster_validation_data_t) - 
                   sizeof(dce_cluster_validation_data_record_t);
    data_p->max_size = DCE_CLUSTER_VALDN_DATA_INITIAL_SIZE;

    return data_p;
}

uint32_t
dce_cluster_validation_collect_add_record (dce_cluster_validation_data_t **data_pp,
                                           uint32_t type, uint32_t len,
                                           uint8_t *val_p)
{
    dce_cluster_validation_data_t           *new_data_p = NULL;
    dce_cluster_validation_data_record_t    *rec_p = NULL;
    
    if (data_pp == NULL) {
        return DCE_CLUSTER_UTIL_INVALID_ARG;
    }
    
    if (DCE_CLUSTER_VALDN_GET_TLV_LEN(len) + (*data_pp)->size > 
        (*data_pp)->max_size) {
        new_data_p = (dce_cluster_validation_data_t *)realloc(*data_pp,
                             (*data_pp)->max_size + DCE_CLUSTER_VALDN_DATA_INCR_SIZE);
        if (new_data_p == NULL) {
            return DCE_CLUSTER_UTIL_INVALID_ARG;
        }

        new_data_p->max_size = (*data_pp)->max_size + DCE_CLUSTER_VALDN_DATA_INCR_SIZE;
        *data_pp = new_data_p;
    }

    rec_p = (dce_cluster_validation_data_record_t *)
                        (((uint8_t *)(*data_pp)) + (*data_pp)->size);
    
    rec_p->type = htonl(type);
    rec_p->len = htonl(len);
    memcpy(rec_p->val, val_p, len);
    (*data_pp)->size += DCE_CLUSTER_VALDN_GET_TLV_LEN(len);
    (*data_pp)->num_records++;

    return DCE_CLUSTER_UTIL_SUCCESS;
}

ResourceId
dce_cluster_validation_collect_done (WaveObjectManagerCollectValidationDataMessage *pMessage,
                                     dce_cluster_validation_data_t *data_p)
{
    UI32    size = 0;
    if (pMessage == NULL || data_p == NULL) {
        return DCE_CLUSTER_UTIL_INVALID_ARG;
    }

    size = data_p->size;
    
    data_p->size = htonl(data_p->size);
    data_p->version = htonl(data_p->version);
    data_p->num_records = htonl(data_p->num_records);
    //data_p->cap_bits = htonll(data_p->cap_bits);

    pMessage->setValidationDetails(data_p, size, true);

    return DCE_CLUSTER_UTIL_SUCCESS;
}

ResourceId
dce_cluster_validation_parse (dce_cluster_validation_data_t *data_p,
                              uint32_t *version_p, uint64_t *cap_bits_p,
                              uint32_t *num_records_p)
{
    if (data_p == NULL || version_p == NULL || 
        num_records_p == NULL || cap_bits_p == NULL) {
        return DCE_CLUSTER_UTIL_INVALID_ARG;
    }

    *version_p = ntohl(data_p->version);
    //*cap_bits_p = ntohll(data_p->cap_bits);
    *num_records_p = ntohl(data_p->num_records);

    return DCE_CLUSTER_UTIL_SUCCESS;
}

ResourceId
dce_cluster_validation_get_record (dce_cluster_validation_data_t *data_p,
                                   uint32_t record_index,
                                   uint32_t *type_p,
                                   uint32_t *len_p,
                                   uint8_t **val_pp)
{
    uint32_t                                i = 0;
    dce_cluster_validation_data_record_t    *rec_p;
    uint32_t                                num_records = 0;
    bool                                    found = false;

    num_records = ntohl(data_p->num_records);

    rec_p = &(data_p->rec_list[0]);

    for (i = 0; i < num_records; i++) {

        if (i == record_index) {
            found = true;
            break;
        }

        rec_p = (dce_cluster_validation_data_record_t *)
                    (((uint8_t *)(rec_p)) + 
                     DCE_CLUSTER_VALDN_GET_TLV_LEN(ntohl(rec_p->len)));
    }

    if (found) {
        *type_p = ntohl(rec_p->type);
        *len_p = ntohl(rec_p->len);
        *val_pp = (uint8_t *)(rec_p->val);
        return DCE_CLUSTER_UTIL_SUCCESS;
    }

    return DCE_CLUSTER_UTIL_REC_NOT_FOUND;
}

void
ClusterGetErrorStringWithArgsForFailedLocation(WaveSendToClusterContext  *ctx_p, string &errString,vector< vector <Attribute *> > &ErrArguments)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<UI32> succeededSwIds;
	vector <Attribute *> temp;
    UI32 swId = 0;
    string FailedswIdString;
    string SucceededswIdString;
    string errorString;
    map<UI32,string> FailedswIderrStrmap;

    /* Get the Location Ids of the switches to which the original message was sent and record them */
    vector<LocationId> sendLocations = ctx_p->getLocationsToSendToForPhase1();
    for (UI32 i = 0; i < sendLocations.size(); i++)
	{
		temp.clear(); 
		if( ErrArguments.size() > i )
        	if(!(ErrArguments[i].empty()))
                temp = ErrArguments[i];
		trace (TRACE_LEVEL_INFO, string("size  of temp = ") + temp.size() );
		
        swId = DcmToolKit::getMappedIdFromLocationId(sendLocations[i]);

        status = ctx_p->getSendStatusForPhase1(sendLocations[i]);
        if (status == WAVE_MESSAGE_SUCCESS)
        {
            status = ctx_p->getFrameworkStatusForPhase1(sendLocations[i]);
            if (status == FRAMEWORK_SUCCESS)
            {
                status = ctx_p->getCompletionStatusForPhase1(sendLocations[i]);
                if (status != WAVE_MESSAGE_SUCCESS)
                {                                                                                                                       
					if (status == WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE)
						continue;
					trace (TRACE_LEVEL_ERROR, string(": Backend operation failed on rbridge-id ") + swId + string(" plugin with error - ") + FrameworkToolKit::localize (status,temp));
					errorString = "\t " + string("rbridge-id ") + swId + string(" reason: ") + FrameworkToolKit::localize (status,temp);
					FailedswIderrStrmap.insert(make_pair(swId,errorString));
                }
                else 
				{
                    trace (TRACE_LEVEL_INFO, string(": Backend operation succeeded on rbridge-id ") + swId + string(" plugin with success - ") + FrameworkToolKit::localize (status,temp));
                    succeededSwIds.push_back(swId);
                }
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("Framework error occured during delegation of backend operation to local plugin on rbridge-id '") + swId + string("' with reason code - ") + FrameworkToolKit::localize(status,temp));
                errorString = "\t " + string("rbridge-id ") + swId + string(" reason: ") + FrameworkToolKit::localize (status,temp);
                FailedswIderrStrmap.insert(make_pair(swId,errorString));
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("Sending original local message to rbridge-id '") + swId + string("' failed with reason code - ") + FrameworkToolKit::localize (status,temp));
            errorString = "\t " + string("rbridge-id ") + swId + string(" reason: ") + FrameworkToolKit::localize (status,temp);
            FailedswIderrStrmap.insert(make_pair(swId,errorString));
        }
    }

	UI32 first=1;
    UI32 k;
    map<UI32,string>::iterator iter;
    if (FailedswIderrStrmap.size()) 
	{
    	for( iter = FailedswIderrStrmap.begin(); iter != FailedswIderrStrmap.end(); ++iter ) 
		{
        	if (first) 
			{
        		FailedswIdString = string("")+iter->first;
		        errorString = iter->second + string (".");
        		first = 0;
        	}
        	else 
			{
            	FailedswIdString +=string(", ")+iter->first;
	            errorString += "\n" + iter->second + string (".");
    	    }
    	}
    	sort (succeededSwIds.begin(),succeededSwIds.end());
	    for (k = 0; k < succeededSwIds.size(); k++) 
		{
        	if (k == 0) 
			{
            	SucceededswIdString = string(" ")+ succeededSwIds[k];
        	}
	        else 
			{
            	SucceededswIdString +=string(", ")+ succeededSwIds[k];
        	}
    	}
        errString = FailedswIdString + string (". Succeeded on Node(es):") + SucceededswIdString + string (". ") + "\n" + errorString ;
    }
    else
        errString = string("");
}

void 
ClusterGetErrorStringForFailedLocation(WaveSendToClusterContext  *ctx_p, string &errString)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    vector<UI32> succeededSwIds;
    UI32 swId = 0;
    string FailedswIdString;
    string SucceededswIdString;
    string errorString;
    map<UI32,string> FailedswIderrStrmap;


    /* Get the Location Ids of the switches to which the original message was sent and record them */
    vector<LocationId> sendLocations = ctx_p->getLocationsToSendToForPhase1();
    for (UI32 i = 0; i < sendLocations.size(); i++)
    {
        swId = DcmToolKit::getMappedIdFromLocationId(sendLocations[i]);

        status = ctx_p->getSendStatusForPhase1(sendLocations[i]);
        if (status == WAVE_MESSAGE_SUCCESS)
        {
            status = ctx_p->getFrameworkStatusForPhase1(sendLocations[i]);
            if (status == FRAMEWORK_SUCCESS)
            {
                status = ctx_p->getCompletionStatusForPhase1(sendLocations[i]);
                if (status != WAVE_MESSAGE_SUCCESS)
                {
                    if (status == WAVE_MESSAGE_ERROR_REMOTE_LOCATION_UNAVAILABLE)
						continue;
					trace (TRACE_LEVEL_ERROR, string(": Backend operation failed on rbridge-id ") + swId + string(" plugin with error - ") + FrameworkToolKit::localize (status));

                    errorString = "\t " + string("rbridge-id ") + swId + string(" reason: ") + FrameworkToolKit::localize (status);
                    FailedswIderrStrmap.insert(make_pair(swId,errorString));
                }
                else {
                    trace (TRACE_LEVEL_DEVEL, string(": Backend operation succeeded on rbridge-id ") + swId + string(" plugin with success - ") + FrameworkToolKit::localize (status));
                    succeededSwIds.push_back(swId);
                }
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("Framework error occured during delegation of backend operation to local plugin on rbridge-id '") + swId + string("' with reason code - ") + FrameworkToolKit::localize(status));
                    errorString = "\t " + string("rbridge-id ") + swId + string(" reason: ") + FrameworkToolKit::localize (status);
                    FailedswIderrStrmap.insert(make_pair(swId,errorString));
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("Sending original local message to rbridge-id '") + swId + string("' failed with reason code - ") + FrameworkToolKit::localize (status));
            errorString = "\t " + string("rbridge-id ") + swId + string(" reason: ") + FrameworkToolKit::localize (status);
            FailedswIderrStrmap.insert(make_pair(swId,errorString));
        }

    }
    UI32 first=1;
    UI32 k;
    map<UI32,string>::iterator iter;
    if (FailedswIderrStrmap.size()) {
    for( iter = FailedswIderrStrmap.begin(); iter != FailedswIderrStrmap.end(); ++iter ) {
        if (first) {
        FailedswIdString = string("")+iter->first;
        errorString = iter->second + string (".");
        first = 0;
        }
        else {
            FailedswIdString +=string(", ")+iter->first;
            errorString += "\n" + iter->second + string (".");
        }
    }
    sort (succeededSwIds.begin(),succeededSwIds.end());
    for (k = 0; k < succeededSwIds.size(); k++) {
        if (k == 0) {
            SucceededswIdString = string(" ")+ succeededSwIds[k];
        }
        else {
            SucceededswIdString +=string(", ")+ succeededSwIds[k];
        }
    }

        errString = FailedswIdString + string (". Succeeded on Node(s):") + SucceededswIdString + string (". ") + "\n" + errorString ;

    }
    else
        errString = string("");

}

ResourceId 
ClusterGetStatusFromContext(WaveSendToClusterContext  *ctx_p)
{
    LocationId locationid;
    ResourceId sendToClusterCompletionStatus = ctx_p->getCompletionStatus ();

    //Need to remove the below if condition, when framework supports complete error handling.
    //sendToClusterCompletionStatus value will be either
    //WAVE_MESSAGE_SUCCESS, WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES ors WAVE_MESSAGE_ERROR_FAILED_ON_SOME_NODES.
    //And based on the above status, framework propagates error to noscli.

    if (sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES ) {
       sendToClusterCompletionStatus = ClusterGetFirstFailureLocation(ctx_p, locationid);
    }
    return sendToClusterCompletionStatus;
}

ResourceId
ClusterGetPhase1StatusFromContextForLocation(WaveSendToClusterContext *ctx_p, LocationId &locationid)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    status = ctx_p->getSendStatusForPhase1(locationid);
    if (status != WAVE_MESSAGE_SUCCESS)	{
        return status; 
    }
    status = ctx_p->getFrameworkStatusForPhase1(locationid);
    if (status != FRAMEWORK_SUCCESS) {
        return status;
    }
    status = ctx_p -> getCompletionStatusForPhase1(locationid);
    if (status != WAVE_MESSAGE_SUCCESS) {
        return status;
    }

    return status;
}

ResourceId
ClusterGetPhase2StatusFromContextForLocation(WaveSendToClusterContext *ctx_p, LocationId &locationid)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    status = ctx_p->getSendStatusForPhase2(locationid);
    if (status != WAVE_MESSAGE_SUCCESS)	{
        return status; 
    }
    status = ctx_p->getFrameworkStatusForPhase2(locationid);
    if (status != FRAMEWORK_SUCCESS) {
        return status;
    }
    status = ctx_p -> getCompletionStatusForPhase2(locationid);
    if (status != WAVE_MESSAGE_SUCCESS) {
        return status;
    }

    return status;
}


ResourceId ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid)
{
    vector<LocationId>  locationsToSendToForPhase1 = ctx_p -> getLocationsToSendToForPhase1 ();

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    for(unsigned int i = 0; i < locationsToSendToForPhase1.size(); i++)
    {
        locationid = locationsToSendToForPhase1[i];
        status = ctx_p->getSendStatusForPhase1(locationid);
       	if (status != WAVE_MESSAGE_SUCCESS)	
			return status; 
		status = ctx_p->getFrameworkStatusForPhase1(locationid);
        if (status != FRAMEWORK_SUCCESS)
			return status;
		status = ctx_p -> getCompletionStatusForPhase1(locationid);
        if (status != WAVE_MESSAGE_SUCCESS)
            return status;
    }
	return status;
}

ResourceId WyserEaConfigurationErrorCode(WaveNs::PrismMessage* msg_p, ResourceId status,vector <Attribute *> &errorArguments, bool isPartialSuccessEnabled)
{
    PLUG_DBG_TRACE();
	
	if(isPartialSuccessEnabled)
	{
		if(msg_p != NULL)
			delete msg_p;
		return (DCM_ENABLE_STATUS_PROPAGATION);
	}	
	
    if(WAVE_MESSAGE_SUCCESS == status)
    {
        PLUG_DBG("sendSynchronously:success");
        if(WAVE_MESSAGE_SUCCESS != msg_p -> getCompletionStatus())
        {
            status = msg_p -> getCompletionStatus();
            PLUG_DBG_S(string("error from global plugin") + status);
        }
    }
    else
    {
        PLUG_DBG_S(string("sendSynchronously:failed") + status);
    }

    if(msg_p != NULL)
        delete msg_p;

    return status;
}

void PrintErrorCode(WaveNs::PrismMessage* msg_p, ResourceId &status,vector <Attribute *> &errorArguments, bool isPartialSuccessEnabled) {
	  if(isPartialSuccessEnabled)
		  status = DCM_ENABLE_STATUS_PROPAGATION;
}
/*
  input parameter:
  ifType:vlan/port-channel/tengigabit/gigabit
  ifName:vlanId/portId/{[mappedId]/slot/port}
  size:Max ifName Size
  
  output parameter:
  shortIfName :vlan0.<vlanId>\
               po<poId>\
               te<slot/port>\
               gi<slot/port>
  locationId : if ifName is valid three tupple locationId
               will be filled with locationId if corresponding Node
               else it will be filled with local locationId.

*/ 
               
  


int getIfShortName(const char *ifType,const char *ifName, string &shortIfName, LocationId &locationId)                 
{                                                                
	PLUG_DBG_S(string("ifType = ") + ifType + string(" ifName = ") + ifName);
	string twoTuppleIfName;                                   
	int ret = -1;                                             
    locationId = 0;
	if (! strncasecmp (ifType, "v", 1)) 
	{                           
		shortIfName = string("vlan0.") +  ifName;                 
	}
	else if (! strncasecmp (ifType, "p", 1)) 
	{                     
		shortIfName = string("po") + ifName;                     
	} 
	else if (! strncasecmp (ifType, "t", 1)) 
	{                 
        locationId = NsmUtils::getLocationId(ifName);    
		ret = NsmUtils::convertThreeTuppleToTwoTupple(ifName, twoTuppleIfName);    
		if(-1 == ret)                                  
		{
			PLUG_DBG_S(string("ifType = ") + ifType + string(" Invalid ifName = ") + ifName + string(" Neither valid three tupple nor valide two tupple"));
			return ret;                                         
		}	
	    shortIfName =  "te" + twoTuppleIfName;                     
	} 
	else if (! strncasecmp (ifType, "g", 1))
	{    
        locationId = NsmUtils::getLocationId(ifName);                 
		ret = NsmUtils::convertThreeTuppleToTwoTupple(ifName, twoTuppleIfName);    
		if(-1 == ret)                                             
		{	
			PLUG_DBG_S(string("ifType = ") + ifType + string(" Invalid ifName = ") + ifName + string(" Neither valid three tupple nor valide two tupple"));
			return ret;                                         
		}	
	    shortIfName =  "gi" + twoTuppleIfName;                     
	}
	else if (! strncasecmp (ifType, "f", 1))
	{    
        locationId = NsmUtils::getLocationId(ifName);                 
		ret = NsmUtils::convertThreeTuppleToTwoTupple(ifName, twoTuppleIfName);    
		if(-1 == ret)                                             
		{	
			PLUG_DBG_S(string("ifType = ") + ifType + string(" Invalid ifName = ") + ifName + string(" Neither valid three tupple nor valide two tupple"));
			return ret;                                         
		}	
	    shortIfName =  "fo" + twoTuppleIfName;                     
	} 
	else 
	{                                                        
		shortIfName = ifName;              
	}                                         
	PLUG_DBG_S(string("short if Name = ") + shortIfName + string(" LocationId = ") + locationId);
	return 0;
} 

/**
 * Resolve the locations ids for sending L3 show commands.. The command is
 * assumed to be of syntax "<tokens> [ rbridge-id [ <id>|<range>|all ] ]".
 * Decrements the 'argc' value by 2 if the 'rbridge-id' information exists in
 * the command. Otherwise fills the location id of current node in the locations
 * vector.
 */
ResourceId
resolveLocations (int &argc, char **argv, vector<LocationId> &locations)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (argc < 2 || strcasecmp(argv[argc-2], "rbridge-id") != 0) {
        locations.push_back(FrameworkToolKit::getThisLocationId());
    } else {
        status = getLocationIdsForRbridges(argv[argc-1], locations);
        if (status == WAVE_MESSAGE_SUCCESS) argc -= 2;
    }

    return status;
}


ResourceId
getLocationIdsForRbridges (const string &rbridges, vector<LocationId> &locations)
{
    LocationId thisLocation = FrameworkToolKit::getThisLocationId();

    if (strcasecmp(rbridges.c_str(), "all") == 0) {
        DcmToolKit::getFullyConnectedLocationsSortedOnDomainId(locations, true);
        return WAVE_MESSAGE_SUCCESS;
    }

    if (rbridges.empty() || !UI32Range::isAValidString(rbridges)) {
        return WRC_VCS_RBRIDGE_INPUT_NOT_IN_CLUSTER;
    }

    vector<UI32> rbridgeIds;
    UI32Range(rbridges).getUI32RangeVector(rbridgeIds);

    for (UI32 i = 0; i < rbridgeIds.size(); ++i) {
        LocationId loc = DcmToolKit::getLocationIdFromMappedId(rbridgeIds[i]);
        if (loc == 0 || (loc != thisLocation && !FrameworkToolKit::isAConnectedLocation(loc)))
            return WRC_VCS_RBRIDGE_INPUT_NOT_IN_CLUSTER;
        locations.push_back(loc);
    }

    return WAVE_MESSAGE_SUCCESS;
}

/**
 * parses vrf token using command line arguments . The command is
 * assumed to be of syntax "<tokens> [ vrf <vrf-name>".
 * Decrements the 'argc' value by 2 if the 'vrf' information exists in
 * the command.Otherwise does nothing.
 */

ResourceId
parseVrfToken (int &argc, char **argv, string  &vrfName)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if ((argc >  2) && (strcasecmp(argv[argc-2], "vrf") == 0)) {
        vrfName = argv[argc-1];
        PLUG_INFO_S(string("vrf name = ") + vrfName);
        argc -=2;
    } 
    return status;
}

ResourceId
resolveLocationsAndVrf(int &argc, char **argv, vector<LocationId> &locations,string &vrfName)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    status = resolveLocations(argc, argv, locations);
    if(status != WAVE_MESSAGE_SUCCESS) {
        return status;
    }
    status = parseVrfToken(argc, argv, vrfName);
    return status;
}

}

