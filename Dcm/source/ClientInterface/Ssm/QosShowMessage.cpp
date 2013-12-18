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
 *   Author : Venkat                                                       *
 **************************************************************************/

#include "ClientInterface/Ssm/QosShowMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

	QosShowMessage::QosShowMessage()
		: DcmManagementInterfaceMessage (QOSSHOWMESSAGE),
		m_ifName     (),
		m_poNum      (),
		m_cmdCode    (QOS_OUT_OF_RANGE_VALUE),
		m_mappedId   (QOS_OUT_OF_RANGE_VALUE),
		m_lineCard   (QOS_OUT_OF_RANGE_VALUE),
		m_portSet    (QOS_OUT_OF_RANGE_VALUE)
    {
    }

    QosShowMessage::QosShowMessage(const string &ifName, const string &poNum, const UI32 &cmdCode, const UI32 &mappedId, const UI32 &lineCard, const UI32 &portSet, const SI32 &chipIndex, const SI32 &portIndex, const UI32 &noOfRecords)
        : DcmManagementInterfaceMessage (QOSSHOWMESSAGE),
        m_ifName     (ifName),
        m_poNum      (poNum),
        m_cmdCode    (cmdCode),
		m_mappedId   (mappedId),
		m_lineCard   (lineCard),
		m_portSet    (portSet),
		m_chipIndex     (chipIndex),
        m_portIndex     (portIndex),
        m_noOfRecords   (noOfRecords)
    {
    }

    QosShowMessage::~QosShowMessage ()
    {
    }

    void  QosShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_poNum,"poNum"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute (new AttributeUI32(&m_lineCard,"lineCard"));
        addSerializableAttribute (new AttributeUI32(&m_portSet,"portSet"));
        addSerializableAttribute (new AttributeSI32(&m_chipIndex,"chipIndex"));
        addSerializableAttribute (new AttributeSI32(&m_portIndex,"portIndex"));
        addSerializableAttribute (new AttributeUI32(&m_noOfRecords,"noOfRecords"));
    }

    void  QosShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  QosShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  QosShowMessage::setPoNum(const string &poNum)
    {
        m_poNum  =  poNum;
    }

    string  QosShowMessage::getPoNum() const
    {
        return (m_poNum);
    }

    void  QosShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  QosShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  QosShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  QosShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void  QosShowMessage::setLineCard(const UI32 &lineCard)
    {
        m_lineCard  =  lineCard;
    }

    UI32  QosShowMessage::getLineCard() const
    {
        return (m_lineCard);
    }

    void  QosShowMessage::setPortSet(const UI32 &portSet)
    {
        m_portSet  =  portSet;
    }

    UI32  QosShowMessage::getPortSet() const
    {
        return (m_portSet);
    }

	void QosShowMessage::setChipIndex(SI32 &chipIndex)
    {
        m_chipIndex = chipIndex;
    }

    SI32 QosShowMessage::getChipIndex() const
    {
        return (m_chipIndex);
    }

    void QosShowMessage::setPortIndex(SI32 &portIndex)
    {
        m_portIndex = portIndex;
    }

    SI32 QosShowMessage::getPortIndex() const
    {
        return (m_portIndex);
    }

    void QosShowMessage::setNoOfRecords(UI32 &noOfRecords)
    {
        m_noOfRecords = noOfRecords;
    }

    UI32 QosShowMessage::getNoOfRecords() const
    {
        return (m_noOfRecords);
    }




    void QosShowMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *QosShowMessage::getCStructureForInputs ()
    {
        QosShow_InputMsg_t *pInput = new QosShow_InputMsg_t;

        strncpy (pInput->if_name, m_ifName.c_str (), MAX_STR);
		pInput->if_name[MAX_STR] = '\0';
        strncpy (pInput->po_num, m_poNum.c_str (), MAX_STR);
		pInput->po_num[MAX_STR] = '\0';
        pInput->cmdCode     = m_cmdCode;
		pInput->slot_num    = m_lineCard;
		pInput->chip_num    = m_portSet;
		pInput->no_of_records = m_noOfRecords;
		pInput->indices.qosInterfaceIndex.chip_index = m_chipIndex;
		pInput->indices.qosInterfaceIndex.port_index = m_portIndex;

        return (pInput);
    }

    void QosShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        QosBufferNum_t  buffNum;
        memset(&buffNum, 0, sizeof(buffNum));

	if (!pOutputCStructure)
		return;

        QosShowMsg_t *msg = ((QosShowMsg_t *)pOutputCStructure);
        int num_records = msg->num_records;

        /* Set QosMessageBufferId */
        switch (msg->record_type)
       	{
            case QOSSHOWQOSRCVQUEUEINTERFACE:
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSSHOWQOSRCVQUEUEINTERFACE;
                addBuffer (buffNum.num, (sizeof(QosShowMsg_t)+ ((num_records - 1) * sizeof (QosRcvQueueInterfaceMsg_t))), pOutputCStructure, false);
                break;
            }
            case QOSSHOWQOSQUEUEINTERFACE:
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSSHOWQOSQUEUEINTERFACE;
                addBuffer (buffNum.num, (sizeof(QosShowMsg_t)+ ((num_records - 1) * sizeof (QosQueueInterfaceMsg_t))), pOutputCStructure, false);
                break;
            }
            case QOSSHOWQOSFLOWCONTROLINTERFACE: 
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSSHOWQOSFLOWCONTROLINTERFACE;
                addBuffer (buffNum.num, (sizeof(QosShowMsg_t)+ ((num_records - 1) * sizeof (QosFlowcontrolInterfaceMsg_t))), pOutputCStructure, false);
	        break;
            }
            case QOSSHOWQOSINTERFACE:
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSSHOWQOSINTERFACE;
                addBuffer (buffNum.num, (sizeof(QosShowMsg_t)+ ((num_records - 1) * sizeof (QosInterfaceMsg_t))), pOutputCStructure, false);
                break;
            }
            case QOSSHOWQOSRCVQUEUEMULTICASTINTERFACE: 
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSSHOWQOSRCVQUEUEMULTICASTINTERFACE;
                addBuffer (buffNum.num, (sizeof(QosShowMsg_t)+ ((num_records - 1) * sizeof (QosRcvQueueMulticastInterfaceMsg_t))), pOutputCStructure, false);
                break;
            }
            case QOSSHOWQOSMAPS: 
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSSHOWQOSMAPS;
                addBuffer (buffNum.num, (sizeof(QosShowMsg_t)+ ((num_records - 1) * sizeof (QosMapsMsg_t))), pOutputCStructure, false);
                break;
            }
            case QOSGETSHOWQOSMAPS: 
            {
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSGETSHOWQOSMAPS;
                addBuffer (buffNum.num, (sizeof(QosShowMsg_t)+ ((num_records - 1) * sizeof (QosMapNamesMsg_t))), pOutputCStructure, false);
				break;
			}
			case QOSSHOWQOSREDPROFILES:
			{
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSSHOWQOSREDPROFILES;
				addBuffer (buffNum.num, (sizeof(QosShowMsg_t) + ((num_records - 1) * sizeof (QosRedProfileMsg_t))), pOutputCStructure, false);
				break;
			}
			case QOSGETSHOWQOSREDPROFILES:
			{
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSGETSHOWQOSREDPROFILES;
				addBuffer (buffNum.num, (sizeof(QosShowMsg_t) + ((num_records - 1) * sizeof (QosRedProfileIdsMsg_t))), pOutputCStructure, false);
				break;
			}
			case QOSSHOWQOSREDSTATS:
			{
                buffNum.buff.mappedId = this->getMappedId();
                buffNum.buff.tag = QOSSHOWQOSREDSTATS;
				addBuffer (buffNum.num, (sizeof(QosShowMsg_t) + ((num_records - 1) * sizeof (QosRedIntfStatsMsg_t))), pOutputCStructure, false);
                break;
            }

			// Command codes to support QOS Wolverine Statistics Show Commands - BEGIN

			case QOSSHOW_QOS_INGQUEUE_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_INGQUEUE_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPortIngrQueStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_INGBUFF_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_INGBUFF_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPortIngrBufStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_EGRQUEUE_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_EGRQUEUE_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPortEgrQueStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_EGRBUFF_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_EGRBUFF_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPortEgrBufStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_PLC_INGQUEUE_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_PLC_INGQUEUE_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPlcIngrQueStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_PLC_INGBUFF_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_PLC_INGBUFF_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPlcIngrBufStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_PLC_EGRQUEUE_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_PLC_EGRQUEUE_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPlcEgrQueStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_PLC_EGRBUFF_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_PLC_EGRBUFF_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPlcEgrBufStats_t))),
						pOutputCStructure, false);
				break;
			}

			// Command codes to support QOS Wolverine Statistics Show Commands - END

			// Command codes to support QOS Hawk Statistics Show Commands - BEGIN
			case QOSSHOW_QOS_CHIP_INFO: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_CHIP_INFO;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosChipPortInfo_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_TMC_INGQUEUE_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_TMC_INGQUEUE_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosTmcIngQueStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_TMC_INGBUFF_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_TMC_INGBUFF_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosTmcIngBufStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_TMC_EGRQUEUE_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_TMC_EGRQUEUE_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosTmcEgrQueStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSHSOW_QOS_TMC_EGRBUFF_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSHSOW_QOS_TMC_EGRBUFF_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosTmcEgrBufStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_ING_TMCBUFF_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_ING_TMCBUFF_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPortTmcIngBufStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_ING_TMCQUEUE_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_ING_TMCQUEUE_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPortTmcIngQueStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_EGR_TMCBUFF_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_EGR_TMCBUFF_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPortTmcEgrBufStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_QOS_VOQ_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_QOS_VOQ_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPortVoqStats_t))),
						pOutputCStructure, false);
				break;
			}

			case QOSSHOW_DROP_REASON_INTERFACE: {
				buffNum.buff.mappedId = this->getMappedId();
				buffNum.buff.tag = QOSSHOW_DROP_REASON_INTERFACE;
				addBuffer(
						buffNum.num,
						(sizeof(QosShowMsg_t)
								+ ((num_records - 1) * sizeof(QosPortDropReasonStats_t))),
						pOutputCStructure, false);
				break;
			}

			// Command codes to support QOS Hawk Statistics Show Commands - END
            default :
                break;
        }
    }
}
