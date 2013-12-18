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
 *   Author : Adarsh
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Dot1X/Dot1XShowMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    Dot1XShowMessage::Dot1XShowMessage ()
        : DcmManagementInterfaceMessage (DOT1X_SHOW_ALL)
    {
    }

    Dot1XShowMessage::Dot1XShowMessage (const string &Interface, const string &ifName, const UI32 &cmdCode, const UI32 &mappedId)
        : DcmManagementInterfaceMessage (DOT1X_SHOW_ALL),
        m_interface (Interface),
        m_ifName    (ifName),
        m_cmdCode   (cmdCode),
		m_mappedId    (mappedId)                                
    {
    }

    Dot1XShowMessage::~Dot1XShowMessage ()
    {
    }

    void  Dot1XShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  Dot1XShowMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  Dot1XShowMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  Dot1XShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  Dot1XShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  Dot1XShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  Dot1XShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	void  Dot1XShowMessage::setMappedId(const UI32 &mappedId)
	{
	  m_mappedId  =  mappedId;
	}

	UI32  Dot1XShowMessage::getMappedId() const
	{
	  return (m_mappedId);
	}

      void Dot1XShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
      {
          pBuffer = findBuffer(bufferNum, size);
      }


    void Dot1XShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *Dot1XShowMessage::getCStructureForInputs ()
    {
        Dot1XShow_InputMsg_t *pInput = new Dot1XShow_InputMsg_t;
		trace (TRACE_LEVEL_DEBUG, "Entering Dot1XShowMessage:: getCStructureForInputs:- " + m_cmdCode);

        strncpy (pInput->if_name, m_ifName.c_str (), 64);
		pInput->if_name[63] = '\0';
        strncpy (pInput->interface , m_interface.c_str (), 64);
		pInput->interface[63] = '\0';
        pInput->cmdCode     = m_cmdCode;

        return (pInput);
    }

	void Dot1XShowMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
	    dot1x_mgmt_buffer_num_t  buff_num;
        Dot1X_show_msg_t *msg = ((Dot1X_show_msg_t *)pOutputCStructure);
		trace (TRACE_LEVEL_DEBUG, "Entering Dot1XShowMessage:: loadOutputsFromCStructure:- " + m_cmdCode);
		if (msg == 0) {
		  trace (TRACE_LEVEL_DEBUG, "Entering Dot1XShowMessage:: loadOutputsFromCStructure:- Message NULL");
		  return;
		}
		int num_records = msg->num_records;
		Dot1X_show_dot1x_output_msg_t *infoptr = msg->info;
		int num_radius = infoptr->num_radius;

		trace (TRACE_LEVEL_DEBUG, "Entering Dot1XShowMessage::loadOutputsFromCStructure\n");

		memset(&buff_num, 0, sizeof(buff_num));
		buff_num.buff.mapped_id = this->getMappedId();
		buff_num.buff.tag = DOT1X_SHOW_ALL;

		switch (msg->record_type) {
		  case DOT1XSHOW: {
				if (num_records > 1) {
				  addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t) + (num_records - 1) * sizeof (dot1x_config_Radius_t) ), pOutputCStructure, false);
				}else{
				  addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t)), pOutputCStructure, false);
				}
				break;
			}
		  case DOT1XSHOWALL: {
				if ((num_records > 1) && (num_radius > 1))
 	       		  addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t)+ ((num_radius -1) * sizeof(dot1x_config_Radius_t)) + ((num_records - 1) * sizeof (Dot1X_show_interface_output_msg_t))), pOutputCStructure, false);
				else if((num_records > 1) && (num_radius <= 1))
				  addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t) + (num_records - 1) * sizeof (Dot1X_show_interface_output_msg_t) ), pOutputCStructure, false);
				else if((num_radius > 1) && (num_records <= 1))
				  addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t) + (num_radius - 1) * sizeof (dot1x_config_Radius_t) ), pOutputCStructure, false);  
				else
				  addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t)), pOutputCStructure, false);
				break;
			}
		  case DOT1XSHOWSTATS: {
        		addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t)+ ((num_records - 1) * sizeof (Dot1X_show_stats_output_msg_t))), pOutputCStructure, false);
				break;
			}
		  case DOT1XSHOWSESSIONINFO: {
        		addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t)+ ((num_records - 1) * sizeof (Dot1X_show_session_info_output_msg_t))), pOutputCStructure, false);
				break;
			}
		  case DOT1XSHOWINTERFACE: {
        		addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t)+ ((num_records - 1) * sizeof (Dot1X_show_interface_output_msg_t))), pOutputCStructure, false);
				break;
			}
		  case DOT1XSHOWDIAG: {
        		addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t)+ ((num_records - 1) * sizeof (Dot1X_show_diag_output_msg_t))), pOutputCStructure, false);
				break;
			}
          case DOT1XSHOWINTERFACE_ALL: {
				if (num_records > 1) {
				  addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t)+ ((num_records - 1) * sizeof (Dot1X_show_interface_output_msg_t))), pOutputCStructure, false);
				}
				else {
				  addBuffer (buff_num.num, (sizeof(Dot1X_show_msg_t)), pOutputCStructure, false);
				}
                break;
            }
			default :
				break;
		}
	}
}
