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

#include "DcmMemUtil.h"
#include "Framework/Utils/PrismMutex.h"
#define max2(x,y) ((x)>(y)?(x):(y))

dcm_mem_stat_t	dcm_mem_stat[DCM_MEM_TYPE_MAX] = {
			   {"LldpClearActionpointWorker", DCM_MEM_TYPE_LLDP_CLEARACTIONPOINTWORKER, 0, 0, 0, 0},
               {"LldpActionpointWorker", DCM_MEM_TYPE_LLDP_ACTIONPOINTWORKER, 0, 0, 0, 0},
               {"LldpDebugActionpointWorker", DCM_MEM_TYPE_LLDP_DEBUGACTIONPOINTWORKER, 0, 0, 0, 0},
               {"LldpLocalLldpDebugMessage", DCM_MEM_TYPE_LLDP_LOCALLLDPDEBUGMESSAGE, 0, 0, 0, 0},
               {"LldpLocalShowMessage", DCM_MEM_TYPE_LLDP_LOCALSHOWMESSAGE, 0, 0, 0, 0},
               {"LldpLocalClearMessage", DCM_MEM_TYPE_LLDP_LOCALCLEARMESSAGE, 0, 0, 0, 0},
			   {"LldpGlobalProfileConfManagedObject", DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFMANAGEDOBJECT, 0, 0, 0, 0},
			   {"LldpGlobalScalarsConfManagedObject", DCM_MEM_TYPE_LLDP_GLOBALSCALARCONFMANAGEDOBJECT, 0, 0, 0, 0},
			   {"LldpGlobalProfileConfSetMessage", DCM_MEM_TYPE_LLDP_GLOBALPROFILECONFSETMESSAGE, 0, 0, 0, 0},
			   {"LldpGlobalScalarsConfSetMessage", DCM_MEM_TYPE_LLDP_GLOBALSCALARSCONFSETMESSAGE, 0, 0, 0, 0}};


namespace DcmNs
{

	   	DcmMemUtil::DcmMemUtil () 
	   	{
	   	}

       	DcmMemUtil::~DcmMemUtil () 
	   	{
		}

			
int DcmMemUtil::memStatUpdate(mem_type_t dcm_mem_type, unsigned int size, bool increment) {
	WaveNs::PrismMutex m_prismMutex;
	if ((int)(dcm_mem_type) >= (int)(DCM_MEM_TYPE_MAX))
		return 0;
	m_prismMutex.lock ();
	if (increment) {	
		dcm_mem_stat[dcm_mem_type].count_of_alloc++  ;
		dcm_mem_stat[dcm_mem_type].size = dcm_mem_stat[dcm_mem_type].size + size;
		dcm_mem_stat[dcm_mem_type].accum_alloc++ ;
		dcm_mem_stat[dcm_mem_type].max_alloc = max2(dcm_mem_stat[dcm_mem_type].accum_alloc,
			dcm_mem_stat[dcm_mem_type].max_alloc);
	}
	else {
		dcm_mem_stat[dcm_mem_type].count_of_alloc-- ;
		dcm_mem_stat[dcm_mem_type].size = dcm_mem_stat[dcm_mem_type].size - size;
		dcm_mem_stat[dcm_mem_type].accum_alloc = 0 ;
	}
	m_prismMutex.unlock();
	return 0;
}
}
		
