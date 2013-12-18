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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "ClientInterface/Ssm/SsmPbrShowClientMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ssmPbr_dcmd_public.h"

namespace DcmNs
{

	SsmPbrShowClientMessage::SsmPbrShowClientMessage() : DcmManagementInterfaceMessage (SSM_CLIENT_PBR_SHOW)
	{
		m_lastStanza = 0;
	}

	SsmPbrShowClientMessage::~SsmPbrShowClientMessage ()
	{
	}

	void  SsmPbrShowClientMessage::setupAttributesForSerialization()
	{
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute(new AttributeUI32(&m_opCode,"opCode"));
		addSerializableAttribute(new AttributeString(&m_routeMapName,"routeMapName"));
		addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute(new AttributeUI16(&m_ifType,"ifType"));
		addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
		addSerializableAttribute(new AttributeUI16(&m_lastStanza,"lastStanza"));
	}

	string SsmPbrShowClientMessage::getIfName() const
	{
		return m_ifName;
	}

	void SsmPbrShowClientMessage::setIfName(const string &ifName)
	{
		m_ifName = ifName;
	}

	UI16 SsmPbrShowClientMessage::getIfType() const
	{
		return m_ifType;
	}

	void SsmPbrShowClientMessage::setIfType(const UI16 &ifType)
	{
		m_ifType = ifType;
	}

	UI32 SsmPbrShowClientMessage::getOpCode() const
	{
		return m_opCode;
	}

	void SsmPbrShowClientMessage::setOpCode(const UI32 &opCode)
	{
		m_opCode = opCode;
	}

    void  SsmPbrShowClientMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  SsmPbrShowClientMessage::getMappedId() const
    {
        return (m_mappedId);
    }

	string SsmPbrShowClientMessage::getRouteMapName() const
	{
		return m_routeMapName;
	}

	void SsmPbrShowClientMessage::setRouteMapName(const string &routeMapName)
	{
		m_routeMapName = routeMapName;
	}

	void SsmPbrShowClientMessage::setLastStanza(const UI16 &lastStannza) {
		m_lastStanza = lastStannza;
	}

	UI16 SsmPbrShowClientMessage::getLastStanza() const {
		return m_lastStanza;
	}


	const void *SsmPbrShowClientMessage::getCStructureForInputs ()
	{
		ssmPbr_dcmd_show_request_t *pInput = (ssmPbr_dcmd_show_request_t *) calloc (1, sizeof(ssmPbr_dcmd_show_request_t));
		memset(pInput, 0, sizeof(ssmPbr_dcmd_show_request_t));

		trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- getCStructureForInputs 1"));

		strncpy (pInput->rmap_name, m_routeMapName.c_str(), SSMPBR_ROUTE_MAP_NAME_SIZE);

		trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- getCStructureForInputs 2"));

		strncpy (pInput->if_name, m_ifName.c_str(), SSMPBR_INTF_LONG_NAME);

		trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- pInput->if_name :")+pInput->if_name);

		trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- getCStructureForInputs 3"));

		pInput->opcode     = (ssmPbr_dcmd_opcode_t )m_opCode;

		trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- getCStructureForInputs EXIT"));

		if(m_opCode == SSMPBR_DCM_SHOW_BIND_SUMMARY_E) {
			pInput->last_stanza_num = 0;
		} else if(m_opCode == SSMPBR_DCM_SHOW_STATUS_BY_ROUTE_MAP_E || m_opCode == SSMPBR_DCM_SHOW_STATUS_BY_INTERFACE_E) {
			pInput->last_stanza_num = m_lastStanza;
		}

		return (pInput);
	}

	void SsmPbrShowClientMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{

		trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure enter"));

		ssm_mgmt_buffer_num_t  buff_num;
		memset(&buff_num, 0, sizeof(buff_num));

		trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure 1"));

		trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- opcode ->") + m_opCode);

		if( m_opCode == SSMPBR_DCM_SHOW_BIND_SUMMARY_E)
		{
			ssmPbr_dcmd_show_summary_response_t *resp_t = (ssmPbr_dcmd_show_summary_response_t *)pOutputCStructure;

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 2") );

			if (!resp_t)
				return;

			char* tmp_buff = NULL;

			tmp_buff = (char *)(resp_t) + (sizeof(struct ssmPbr_dcmd_show_summary_response) - sizeof(struct ssmPbr_summary_status_resp_data));
			struct ssmPbr_summary_status_resp_data* resp_data = (struct ssmPbr_summary_status_resp_data *)(tmp_buff);

			size_t numOfRec  = (resp_data->num_records == 1) ? 1 : resp_data->num_records;

			buff_num.buff.mapped_id = this->getMappedId();

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- buff_num.buff.mapped_id -> ") +
					buff_num.buff.mapped_id );

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- numOfRec -> ") +
					numOfRec );

			for(int k=0; k < resp_data->num_records; k++) {
				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- if_name -> ") +
						resp_data->rmap_rec[k].if_name );

				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- route_map -> ") +
						resp_data->rmap_rec[k].route_map );
			}

//			struct          ssmPbr_summary_status_resp_data     *resp_data_status   = NULL;
//			struct          ssmPbr_route_map_bind_entry   *intfRec           = NULL;
//
//			resp_data_status    =   (struct ssmPbr_summary_status_resp_data *)(&(resp_t->resp_data));
//			resp_data_status    +=  1;
//
//			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 3 "));
//
//			intfRec            =   (struct ssmPbr_route_map_bind_entry *) (resp_data_status);

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 4 "));

			unsigned Totalbuffsize = sizeof(ssmPbr_dcmd_show_summary_response_t) + ((numOfRec) * sizeof (ssmPbr_route_map_bind_entry));

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 5 "));

			addBuffer (buff_num.num, Totalbuffsize, (unsigned char *)pOutputCStructure, false);

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 6 "));

		} else {

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 7 "));

			ssmPbr_dcmd_show_response_t *resp_t = (ssmPbr_dcmd_show_response_t *)pOutputCStructure;

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 8 "));

			if (!resp_t) {
				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- ##### RETURN ###### "));
				return;
			}

			size_t numOfRec     = ((resp_t->resp_data).num_records == 1) ? 1 : (resp_t->resp_data).num_records;

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- numOfRec -> ")+numOfRec);

			buff_num.buff.mapped_id = this->getMappedId();

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- buff_num.buff.mapped_id - >  ")
					+ buff_num.buff.mapped_id);


			for(int k=0; k < resp_t->resp_data.num_records; k++)
			{
				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- if_name -> ") +
						resp_t->resp_data.rmap_rec[k].if_name );

				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- route_map -> ") +
						resp_t->resp_data.rmap_rec[k].route_map );

				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- num_stanzas -> ") +
						resp_t->resp_data.rmap_rec[k].num_stanzas );

				for(int j=0; j< resp_t->resp_data.rmap_rec[k].num_stanzas; j++)
				{
					trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- ip_acl_name -> ") +
							resp_t->resp_data.rmap_rec[k].stanza_rec[j].ip_acl_name );
					trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- seq_num -> ") +
							resp_t->resp_data.rmap_rec[k].stanza_rec[j].seq_num );
				}
			}


			struct          ssmPbr_status_resp_data     *resp_data_status   = NULL;
//			struct          ssmPbr_route_map_status_record   *intf_rec           = NULL;
			unsigned int    intStanzaRecords                                  = 0;

			resp_data_status    =   (struct ssmPbr_status_resp_data *)(&(resp_t->resp_data));

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 10 "));

			for (size_t j = 0; j < numOfRec; j++)
			{

				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 9 "));

//				intf_rec = (struct ssmPbr_route_map_status_record *) (resp_data_status->rmap_rec[j]);

				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- recored -> ")+j);

				unsigned int    numStanza        = 0;
//				char            *status_buff    = NULL;

				if ((resp_data_status->rmap_rec[j].num_stanzas) > 1)
					numStanza = ((resp_data_status->rmap_rec[j].num_stanzas) -1);
				else
					numStanza = 0;

				intStanzaRecords += numStanza;

				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- numStanza -> ")+numStanza);
				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- intStanzaRecords -> ")+intStanzaRecords);

//				status_buff = (char *)(intf_rec) + sizeof (struct ssmPbr_route_map_status_record) +
//						((numStanza) * sizeof (struct ssmPbr_stanza_status_record));
//
//				intf_rec = (struct ssmPbr_route_map_status_record *)(status_buff);
//
				trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 11 "));

			}

			unsigned Totalbuffsize = sizeof(ssmPbr_dcmd_show_response_t) + ((numOfRec) * sizeof (ssmPbr_route_map_status_record))
												+ ((intStanzaRecords) * sizeof (struct ssmPbr_stanza_status_record));

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 12 "));

			addBuffer (buff_num.num, Totalbuffsize, (unsigned char *)pOutputCStructure, false);

			trace(TRACE_LEVEL_DEBUG, string("SsmPbrShowClientMessage -- loadOutputsFromCStructure -- 13 "));
		}

	}
}

