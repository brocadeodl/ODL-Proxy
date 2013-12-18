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


#ifndef DCM_MEM_H
#define DCM_MEM_H

#define DCM_MEM_TYPE_STR_MAX_LEN	100
#define	DCMMEMSTATINCREMENT	1
#define DCMMEMSTATDECREMENT	0
/*  ********************
 ********   MAKE SURE YOU ADD YOUR ENU ELEMENT BEFORE DCM_MEM_TYPE_MAX ***********

 * *********************
*/
typedef enum mem_type_{
	DCM_MEM_TYPE_LLDP_CLEARACTIONPOINTWORKER 			= 0,
	DCM_MEM_TYPE_LLDP_ACTIONPOINTWORKER 				= 1,
	DCM_MEM_TYPE_LLDP_DEBUGACTIONPOINTWORKER 			= 2,
	DCM_MEM_TYPE_LLDP_LOCALLLDPDEBUGMESSAGE 			= 3,
	DCM_MEM_TYPE_LLDP_LOCALSHOWMESSAGE 					= 4,
	DCM_MEM_TYPE_LLDP_LOCALCLEARMESSAGE					= 5,
	DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFMANAGEDOBJECT	= 6,
	DCM_MEM_TYPE_LLDP_GLOBALSCALARCONFMANAGEDOBJECT 	= 7,
	DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFSETMESSAGE		= 8,
	DCM_MEM_TYPE_LLDP_GLOBALSCALARSCONFSETMESSAGE		= 9,
	DCM_MEM_TYPE_MAX	 								= 10
}mem_type_t;

typedef struct dcm_mem_stat_{
    char			name[DCM_MEM_TYPE_STR_MAX_LEN + 1];
    mem_type_t		dcm_mem_type;
    unsigned int    count_of_alloc;
    unsigned int    size;
    unsigned int    accum_alloc;
    unsigned int    max_alloc;
}dcm_mem_stat_t;

extern dcm_mem_stat_t dcm_mem_stat[DCM_MEM_TYPE_MAX];


namespace DcmNs
{
    class DcmMemUtil
    {
        private :
        protected :
        public :
             DcmMemUtil ();
			 ~ DcmMemUtil ();

            static int memStatUpdate (mem_type_t ,unsigned int,bool);
        private :

        protected :

        public :
    };
}


#endif 

