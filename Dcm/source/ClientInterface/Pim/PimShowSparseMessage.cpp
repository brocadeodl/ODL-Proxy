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
#include "ClientInterface/Pim/PimShowSparseMessage.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"


namespace DcmNs
{

    PimShowSparseMessage::PimShowSparseMessage ()
        : DcmManagementInterfaceMessage (PIM_SHOW_SPARSE)
    {
     trace(TRACE_LEVEL_DEBUG, string("Base PimShowSparseMessage constructor entered:"
			         " Entered...."));
	}

    PimShowSparseMessage::PimShowSparseMessage (const UI32 &cmd, const string  &ifName)
        : DcmManagementInterfaceMessage (PIM_SHOW_SPARSE),
        m_cmd 	   	(cmd),
		m_ifName    (ifName)
    {
     trace(TRACE_LEVEL_DEBUG, string("Advanced PimShowSparseMessage constructor entered:"
			                      " Entered...."));	 
    }

    PimShowSparseMessage::~PimShowSparseMessage ()
    {
    }

    void  PimShowSparseMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmd,"cmd"));
		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
    }

    void  PimShowSparseMessage::setCmdCode(const UI32 &cmd)
    {
        m_cmd  =  cmd;
    }

    UI32  PimShowSparseMessage::getCmdCode() const
    {
        return (m_cmd);
    }

   void  PimShowSparseMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  PimShowSparseMessage::getIfName() const
    {
        return (m_ifName);
    }


    void PimShowSparseMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
		     trace(TRACE_LEVEL_DEBUG, string("PimShowNeighborMessag: getOutputBuffer entered:"
					                      " Entered...."));			 
            pBuffer   =   findBuffer(bufferNum, size);
    }

    const void *PimShowSparseMessage::getCStructureForInputs ()
    {
        pim_show_in_msg  *msg = new pim_show_in_msg;
		msg->cmd_code = m_cmd;
		strncpy (msg->if_name, m_ifName.c_str (), IF_LONG_NAMESIZE_DCM);
        return msg;
    }

	void 
    PimShowSparseMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		pim_show_sparse_out_msg *out_msg_show_sparse = ((pim_show_sparse_out_msg *)pOutputCStructure);
        if (out_msg_show_sparse == 0) 
		{
            //printf("\n Failed to extract data from pOutputCStructure");
			return;
        }
		//printf("\n Going to execute addBuffer");
		//printf("\n size=%d",out_msg_show_sparse->size);
		addBuffer (PIM_SHOW_SPARSE, out_msg_show_sparse->size, pOutputCStructure, false);
		//printf("\n addBuffer done. size=%d",out_msg_show_sparse->size);
		return;
	}
}
