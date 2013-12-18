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

/*
 *  Copyright (C) 2012-2017 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *  Author: pbalacha 
 *  Description: APIs to manipulate TLV buffer exchanged between Plugin and backend. 
 */

#include "tlv_dcm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * API  		: dcmTlvBufferInit
 * Description  : Used to initialize a tlv buffer.
 *				  'addTLVToBuffer' API should never be called without
 *				  initializing the buffer using this API.
 * Input		: Buffer that needs to be initialized
 * Returns      : success = 0, failure = -1
 */
int dcmTlvBufferInit (char *output)
{
	if (output == NULL)
	{
		return (-1);
	}

  	dcm_tlv_t* p_tlv = (dcm_tlv_t*) output;
	p_tlv->type = DCM_TLV_BOOK_KEEP;
	p_tlv->length = 1;
	p_tlv->value[0] = 0;
	return (0);
}

/*
 * API  		: createTLV 
 * Description  : Creates a TLV. Caller should free the TLV using freeTLV().
 * Input		: Type, length and value 
 * Returns		: success = Non NULL dcm_tlv_t* , failure = NULL
 */
dcm_tlv_t* createTLV (unsigned int type, unsigned int length, char* value)
{
  	dcm_tlv_t* p_tlv;
	p_tlv = (dcm_tlv_t*)malloc (sizeof (dcm_tlv_t) + length - 1);
	if (length == 0 || p_tlv == NULL)
	{
		return (NULL);
	}
	p_tlv->type = type;
	p_tlv->length = length;
	memcpy(p_tlv->value, value, length);
	return (p_tlv);
}
/*
 * API  		: appendTLVToBuffer 
 * Description  : Appends a TLV to buffer. FreeTLV() should not be called
 *                after using this API.
 * Input		: Buffer, Type, length and value 
 * Returns		: success = 0, failure = -1 
 */

int appendTLVToBuffer (char *buffer, unsigned int type, unsigned int length, char* value)
{
  	unsigned int tlv_count = 0, i = 0;
	dcm_tlv_t *curr_tlv = NULL;
	dcm_tlv_t* start_tlv = (dcm_tlv_t*) buffer;

	if (length == 0)
	{
		return(-1);
	}
	tlv_count = start_tlv->value[0];
	curr_tlv = (dcm_tlv_t*) (buffer + sizeof (dcm_tlv_t));

	for (i = 0; i < tlv_count; i++)
	{
		curr_tlv = moveToNextTLVInBuffer (buffer, curr_tlv);
	}
	curr_tlv->type = type;
	curr_tlv->length = length;
	memcpy(curr_tlv->value, value, length);
	start_tlv->value[0] = tlv_count + 1;
	return (0);
}

/*
 * API  		: addTLVToBuffer 
 * Description  : Adds TLV to a buffer.
 *				  Buffer should have been initialized using dcmTlvBufferInit().
 *				  The TLV is not freed by this API. Caller should free it.
 * Input		: Buffer char*, dcm_tlv_t * 
 * Returns		: success = 0, failure = -1 
 */
int addTLVToBuffer (char *buffer, dcm_tlv_t *new_tlv)
{
  	unsigned int tlv_count = 0, i = 0;
	dcm_tlv_t *curr_tlv = NULL;
	dcm_tlv_t *next_tlv = NULL;
	dcm_tlv_t* start_tlv = (dcm_tlv_t*) buffer;

	if (new_tlv->length == 0)
	{
		return(-1);
	}

	tlv_count = start_tlv->value[0];
	curr_tlv = (dcm_tlv_t*) (buffer + sizeof (dcm_tlv_t));

	for (i = 0; i < tlv_count; i++)
	{
		next_tlv = moveToNextTLVInBuffer (buffer, curr_tlv);
		curr_tlv = next_tlv;
	}
	curr_tlv->type = new_tlv->type;
	curr_tlv->length = new_tlv->length;
	memcpy(curr_tlv->value, new_tlv->value, new_tlv->length);
	start_tlv->value[0] = tlv_count + 1;
	return (0);
}

/*
 * API  		: getTLVFromBuffer 
 * Description  : Returns pointer to a TLV from buffer.
 *				  No new memory is allocated for the TLV.
 *                Hence,  Caller should not free the returned TLV pointer.
 * Input		: Buffer char*, int position. First TLV is at position 0(Zero).
 * Returns		: success = Non NULL dcm_tlv_t* , failure = NULL
 */
dcm_tlv_t* getTLVFromBuffer(char* buffer, int pos)
{
  	int i = 0;
	dcm_tlv_t *tlv = (dcm_tlv_t*) buffer;
	dcm_tlv_t *next_tlv = NULL;
	if ((tlv->value[0] == 0) || (pos > tlv->value[0] - 1)) {
	  return (NULL);
	}
	tlv = (dcm_tlv_t*) (buffer + sizeof (dcm_tlv_t)); 
	for (i = 0 ; i < pos ; i++)
	{
		next_tlv = moveToNextTLVInBuffer (buffer, tlv);
		tlv = next_tlv;
	}
	return (tlv);
}

/*
 * API  		: getCopyOfTLV 
 * Description  : Returns a copy of TLV.
 *                Caller should free the returned TLV pointer.
 * Input		: TLV pointer 
 * Returns		: success = Non NULL dcm_tlv_t* , failure = NULL
 */
dcm_tlv_t* getCopyOfTLV (dcm_tlv_t* tlv)
{
  	dcm_tlv_t* p_tlv;
	p_tlv = (dcm_tlv_t*) malloc (sizeof (dcm_tlv_t) + tlv->length - 1);
	if (p_tlv == NULL)
	{
		return (NULL);
	}

	p_tlv->type = tlv->type;
	p_tlv->length = tlv->length;
	memcpy(p_tlv->value, tlv->value, tlv->length);
	return p_tlv;
}

/*
 * API  		: freeTLV 
 * Description  : Frees memory pointed by TLV.
 * Input		: TLV pointer 
 */
void freeTLV(dcm_tlv_t* tlv)
{
	if (tlv) {
		free(tlv);
		tlv = NULL;
	}
}

/*
 * API  		: moveToNextTLVInBuffer 
 * Description  : Returns pointer to next TLV in buffer. This API should not be called by user.
 *				  It is internally called by getTLVFromBuffer().
 * Input		: Buffer char*, dcm_tlv_t * 
 * Returns		: Non NULL dcm_tlv_t* 
 */
dcm_tlv_t* moveToNextTLVInBuffer (char* buffer, dcm_tlv_t* curr_tlv)
{
  	dcm_tlv_t* next_tlv_p;
 	next_tlv_p = (dcm_tlv_t*)((char*)curr_tlv + (sizeof (dcm_tlv_t) + curr_tlv->length - 1));
	return (next_tlv_p);
}

/*
 * API  		: getTLVCountInBuffer 
 * Description  : Returns number of TLVs in TLV buffer.
 * Input		: Buffer char* 
 * Returns		: Integer - Number of TLVs 
 */
int getTLVCountInBuffer(char* buffer)
{
	dcm_tlv_t *tlv = (dcm_tlv_t*) buffer;
	return (tlv->value[0]);
}

/*
 * API  		: setTLVCountInBuffer 
 * Description  : Sets the number of TLVs in TLV buffer.
 * Input		: Buffer char* , int count
 * Returns		: 0 = success , -1 = error
 */
int setTLVCountInBuffer(char* buffer, int count)
{
  	if (buffer == NULL)
	{
		return (-1);
	}
	dcm_tlv_t *tlv = (dcm_tlv_t*) buffer;
	tlv->value[0] = count;
	return (0);
}
