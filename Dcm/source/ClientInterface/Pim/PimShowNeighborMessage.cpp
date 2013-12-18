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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Sree Shankar                                                 *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Pim/PimShowNeighborMessage.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"


namespace DcmNs
{

    PimShowNeighborMessage::PimShowNeighborMessage ()
        : DcmManagementInterfaceMessage (PIM_SHOW_NEIGHBOR)
    {
     trace(TRACE_LEVEL_DEBUG, string("Base PimShowNeighborMessage constructor entered:"
			         " Entered...."));
	}

    PimShowNeighborMessage::PimShowNeighborMessage (const UI32 &cmd)
        : DcmManagementInterfaceMessage (PIM_SHOW_NEIGHBOR),
        m_cmd 	   	(cmd)
    {
     trace(TRACE_LEVEL_DEBUG, string("Advanced PimShowNeighborMessage constructor entered:"
			                      " Entered...."));	 
    }

    PimShowNeighborMessage::~PimShowNeighborMessage ()
    {
    }

    void  PimShowNeighborMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmd,"cmd"));
    }

    void  PimShowNeighborMessage::setCmdCode(const UI32 &cmd)
    {
        m_cmd  =  cmd;
    }

    UI32  PimShowNeighborMessage::getCmdCode() const
    {
        return (m_cmd);
    }

    void PimShowNeighborMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
		     trace(TRACE_LEVEL_DEBUG, string("PimShowNeighborMessag: getOutputBuffer entered:"
					                      " Entered...."));			 
            pBuffer   =   findBuffer(bufferNum, size);
    }

    const void *PimShowNeighborMessage::getCStructureForInputs ()
    {
        pim_show_in_msg  *msg = new pim_show_in_msg;
		msg->cmd_code = m_cmd;//PIM_SHOW_NEIGHBOR_CMD;
        return msg;
    }

	void 
    PimShowNeighborMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		
		if(m_cmd == PIM_SHOW_NEIGHBOR_CMD) 
		{
			pim_show_out_msg_all_ports *msg = ((pim_show_out_msg_all_ports *)pOutputCStructure);			
        	if (msg == 0) 
			{
            	return;
        	}
			addBuffer (PIM_SHOW_NEIGHBOR, msg->size, pOutputCStructure, false);
		}
		else
		if(m_cmd == PIM_SHOW_SPARSE_CMD)
		{
			pim_show_sparse_out_msg *out_msg_show_sparse = ((pim_show_sparse_out_msg *)pOutputCStructure);
        	if (out_msg_show_sparse == 0) 
			{
            	printf("\n Failed to extract data from pOutputCStructure");
				return;
        	}
			printf("\n Going to execute addBuffer");
			printf("\n size=%d",out_msg_show_sparse->size);
			addBuffer (PIM_SHOW_NEIGHBOR, out_msg_show_sparse->size, pOutputCStructure, false);
			printf("\n addBuffer done. size=%d",out_msg_show_sparse->size);
		}
		return;
	}
}
