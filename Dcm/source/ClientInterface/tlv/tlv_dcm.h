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

#ifndef TLV_DCM_H 
#define TLV_DCM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Common TLV */
#define DCM_TLV_BOOK_KEEP 1 
/* BGP TLVs */
#define BGP_HEADER 2 
#define BGP_DATA  3 
/* PIM TLVs */
#define PIM_HEADER 4
#define PIM_DATA 5
/* NSM TLVs */
#define NSM_HEADER 6
#define NSM_DATA 7

typedef struct dcm_tlv_t_
{
	unsigned int type;
	unsigned int length;
	char	value[1];
} __attribute__((packed)) dcm_tlv_t;

int dcmTlvBufferInit (char *output);
dcm_tlv_t* createTLV (unsigned int type, unsigned int length, char* value);
int appendTLVToBuffer (char *buffer, unsigned int type, unsigned int length, char* value);
int addTLVToBuffer (char *buffer, dcm_tlv_t *new_tlv);
dcm_tlv_t* getTLVFromBuffer(char* buffer, int pos);
dcm_tlv_t* getCopyOfTLV (dcm_tlv_t* tlv);
void freeTLV(dcm_tlv_t* tlv);
dcm_tlv_t* moveToNextTLVInBuffer (char* buffer, dcm_tlv_t* curr_tlv);
int getTLVCountInBuffer(char* buffer); 
int setTLVCountInBuffer(char* buffer, int count); 

#ifdef __cplusplus
}
#endif

#endif // TLV_DCM_H
