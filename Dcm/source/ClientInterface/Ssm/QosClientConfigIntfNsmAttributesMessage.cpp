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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "ClientInterface/Ssm/QosClientConfigIntfNsmAttributesMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientConfigIntfNsmAttributesMessage::QosClientConfigIntfNsmAttributesMessage() :
	  DcmManagementInterfaceMessage (QOSCLIENTCONFIGINTFNSMATTRIBUTES)
    {
		m_distance = LONG_DISTANCE_200;
    }

    QosClientConfigIntfNsmAttributesMessage::QosClientConfigIntfNsmAttributesMessage(const string &ifName, const SI32 &flowControlTx,const SI32 &flowControlRx, const bool &qosTrust,const bool &qosDscpTrust, const SI32 &defaultCosValue,const SI32 &cmdCode,const SI32 &OpCode) : DcmManagementInterfaceMessage(QOSCLIENTCONFIGINTFNSMATTRIBUTES),
        m_ifName    (ifName),
        m_flowControlTx    (flowControlTx),
        m_flowControlRx    (flowControlRx),
        m_qosTrust    (qosTrust),
	 	m_qosDscpTrust 	(qosDscpTrust),
        m_defaultCosValue    (defaultCosValue),
        m_cmdCode    (cmdCode),
        m_OpCode    (OpCode)
    {
		m_distance = LONG_DISTANCE_200;
    }

    QosClientConfigIntfNsmAttributesMessage::~QosClientConfigIntfNsmAttributesMessage()
    {
    }

    void  QosClientConfigIntfNsmAttributesMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_flowControlTx,"flowControlTx"));
        addSerializableAttribute (new AttributeSI32(&m_flowControlRx,"flowControlRx"));
        addSerializableAttribute (new AttributeBool(&m_qosTrust,"qosTrust"));
		addSerializableAttribute (new AttributeBool(&m_qosDscpTrust,"qosDscpTrust"));
        addSerializableAttribute (new AttributeSI32(&m_defaultCosValue,"defaultCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_OpCode,"OpCode"));
		addSerializableAttribute (new AttributeString(&m_cosToCosMutationMapName,"cosToCosMutationMapName"));
        addSerializableAttribute (new AttributeString(&m_cosToTrafficClassMapName,"cosToTrafficClassMapName"));
        addSerializableAttribute (new AttributeString(&m_dscpToDscpMutationMapName,"dscpToDscpMutationMapName"));
        addSerializableAttribute (new AttributeString(&m_dscpToTrafficClassMapName,"dscpToTrafficClassMapName"));
		addSerializableAttribute (new AttributeString(&m_dscpToCosMapName,"dscpToCosMapName"));
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
		addSerializableAttribute (new AttributeSI32(&m_pfcFlowControlCos,"pfcFlowControlCos"));
        addSerializableAttribute (new AttributeUI32(&m_pfcFlowControlTx,"pfcFlowControlTx"));
        addSerializableAttribute (new AttributeUI32(&m_pfcFlowControlRx,"pfcFlowControlRx"));
		addSerializableAttribute (new AttributeUI32(&m_cos0Threshold, "cos0Threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos1Threshold, "cos1Threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos2Threshold, "cos2Threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos3Threshold, "cos3Threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos4Threshold, "cos4Threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos5Threshold, "cos5Threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos6Threshold, "cos6Threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos7Threshold, "cos7Threshold"));
		addSerializableAttribute (new AttributeEnum((UI32 *)(&m_distance), "Distance"));
		addSerializableAttribute (new AttributeUI32(&m_cos,"cos"));
		addSerializableAttribute (new AttributeUI32(&m_redProfile, "redProfile"));
		addSerializableAttribute (new AttributeUI32(&m_tailDrop, "tailDrop"));

    }

    void  QosClientConfigIntfNsmAttributesMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  QosClientConfigIntfNsmAttributesMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setFlowControlTx(const SI32 &flowControlTx)
    {
        m_flowControlTx  =  flowControlTx;
    }

    SI32  QosClientConfigIntfNsmAttributesMessage::getFlowControlTx() const
    {
        return (m_flowControlTx);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setFlowControlRx(const SI32 &flowControlRx)
    {
        m_flowControlRx  =  flowControlRx;
    }

    SI32  QosClientConfigIntfNsmAttributesMessage::getFlowControlRx() const
    {
        return (m_flowControlRx);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setQosTrust(const bool &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }

    bool  QosClientConfigIntfNsmAttributesMessage::getQosTrust() const
    {
        return (m_qosTrust);
    }

	 void  QosClientConfigIntfNsmAttributesMessage::setQosDscpTrust(const bool &qosDscpTrust)
    {
        m_qosDscpTrust  =  qosDscpTrust;
    }

    bool  QosClientConfigIntfNsmAttributesMessage::getQosDscpTrust() const
    {
        return (m_qosDscpTrust);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setDefaultCosValue(const SI32 &defaultCosValue)
    {
        m_defaultCosValue  =  defaultCosValue;
    }

    SI32  QosClientConfigIntfNsmAttributesMessage::getDefaultCosValue() const
    {
        return (m_defaultCosValue);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  QosClientConfigIntfNsmAttributesMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setOpCode(const SI32 &OpCode)
    {
        m_OpCode  =  OpCode;
    }

    SI32  QosClientConfigIntfNsmAttributesMessage::getOpCode() const
    {
        return (m_OpCode);
    }

	void  QosClientConfigIntfNsmAttributesMessage::setCosToCosMutationMapName(const string &cosToCosMutationMapName)
    {
        m_cosToCosMutationMapName  =  cosToCosMutationMapName;
    }

    string  QosClientConfigIntfNsmAttributesMessage::getCosToCosMutationMapName() const
    {
        return (m_cosToCosMutationMapName);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setCosToTrafficClassMapName(const string &cosToTrafficClassMapName)
    {
        m_cosToTrafficClassMapName  =  cosToTrafficClassMapName;
    }

    string  QosClientConfigIntfNsmAttributesMessage::getCosToTrafficClassMapName() const
    {
        return (m_cosToTrafficClassMapName);
    }
	void  QosClientConfigIntfNsmAttributesMessage::setDscpToDscpMutationMapName(const string &dscpToDscpMutationMapName)
    {
        m_dscpToDscpMutationMapName  =  dscpToDscpMutationMapName;
    }

    string  QosClientConfigIntfNsmAttributesMessage::getDscpToDscpMutationMapName() const
    {
        return (m_dscpToDscpMutationMapName);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setDscpToTrafficClassMapName(const string &dscpToTrafficClassMapName)
    {
        m_dscpToTrafficClassMapName  =  dscpToTrafficClassMapName;
    }

    string  QosClientConfigIntfNsmAttributesMessage::getDscpToTrafficClassMapName() const
    {
        return (m_dscpToTrafficClassMapName);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setDscpToCosMapName(const string &dscpToCosMapName)
    {
        m_dscpToCosMapName  =  dscpToCosMapName;
    }

    string  QosClientConfigIntfNsmAttributesMessage::getDscpToCosMapName() const
    {
        return (m_dscpToCosMapName);
    }
    void  QosClientConfigIntfNsmAttributesMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosClientConfigIntfNsmAttributesMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }
	
	void  QosClientConfigIntfNsmAttributesMessage::setPfcFlowControlCos(const SI32 &pfcFlowControlCos)
    {
        m_pfcFlowControlCos  =  pfcFlowControlCos;
    }

    SI32  QosClientConfigIntfNsmAttributesMessage::getPfcFlowControlCos() const
    {
        return (m_pfcFlowControlCos);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setPfcFlowControlTx(const UI32 &pfcFlowControlTx)
    {
        m_pfcFlowControlTx  =  pfcFlowControlTx;
    }

    UI32  QosClientConfigIntfNsmAttributesMessage::getPfcFlowControlTx() const
    {
        return (m_pfcFlowControlTx);
    }

    void  QosClientConfigIntfNsmAttributesMessage::setPfcFlowControlRx(const UI32 &pfcFlowControlRx)
    {
        m_pfcFlowControlRx  =  pfcFlowControlRx;
    }

    UI32  QosClientConfigIntfNsmAttributesMessage::getPfcFlowControlRx() const
    {
        return (m_pfcFlowControlRx);
    }

	void QosClientConfigIntfNsmAttributesMessage::setCos0Threshold(const UI32 &threshold)
	{
		m_cos0Threshold = threshold;
	}

	UI32 QosClientConfigIntfNsmAttributesMessage::getCos0Threshold() const
	{
		return (m_cos0Threshold);
	}
	void QosClientConfigIntfNsmAttributesMessage::setCos1Threshold(const UI32 &threshold)
	{
		m_cos1Threshold = threshold;
	}

	UI32 QosClientConfigIntfNsmAttributesMessage::getCos1Threshold() const
	{
		return (m_cos1Threshold);
	}
	void QosClientConfigIntfNsmAttributesMessage::setCos2Threshold(const UI32 &threshold)
	{
		m_cos2Threshold = threshold;
	}

	UI32 QosClientConfigIntfNsmAttributesMessage::getCos2Threshold() const
	{
		return (m_cos2Threshold);
	}
	void QosClientConfigIntfNsmAttributesMessage::setCos3Threshold(const UI32 &threshold)
	{
		m_cos3Threshold = threshold;
	}

	UI32 QosClientConfigIntfNsmAttributesMessage::getCos3Threshold() const
	{
		return (m_cos3Threshold);
	}
	void QosClientConfigIntfNsmAttributesMessage::setCos4Threshold(const UI32 &threshold)
	{
		m_cos4Threshold = threshold;
	}

	UI32 QosClientConfigIntfNsmAttributesMessage::getCos4Threshold() const
	{
		return (m_cos4Threshold);
	}
	void QosClientConfigIntfNsmAttributesMessage::setCos5Threshold(const UI32 &threshold)
	{
		m_cos5Threshold = threshold;
	}

	UI32 QosClientConfigIntfNsmAttributesMessage::getCos5Threshold() const
	{
		return (m_cos5Threshold);
	}
	void QosClientConfigIntfNsmAttributesMessage::setCos6Threshold(const UI32 &threshold)
	{
		m_cos6Threshold = threshold;
	}

	UI32 QosClientConfigIntfNsmAttributesMessage::getCos6Threshold() const
	{
		return (m_cos6Threshold);
	}
	void QosClientConfigIntfNsmAttributesMessage::setCos7Threshold(const UI32 &threshold)
	{
		m_cos7Threshold = threshold;
	}

	UI32 QosClientConfigIntfNsmAttributesMessage::getCos7Threshold() const
	{
		return (m_cos7Threshold);
	}

	void QosClientConfigIntfNsmAttributesMessage::setDistance(const LongDistance &distance)
	{
		m_distance = distance;
	}

	LongDistance  QosClientConfigIntfNsmAttributesMessage::getDistance() const
	{
		return (m_distance);
	}
	UI32  QosClientConfigIntfNsmAttributesMessage::getCos() const
	{
		return (m_cos);
	}
	
	void QosClientConfigIntfNsmAttributesMessage::setCos(const UI32 &cos)
	{
		m_cos = cos;
	}

	UI32  QosClientConfigIntfNsmAttributesMessage::getRedProfile() const
    {
        return (m_redProfile);
    }

    void QosClientConfigIntfNsmAttributesMessage::setRedProfile(const UI32 &redProfile)
    {
        m_redProfile = redProfile;
    }

	UI32  QosClientConfigIntfNsmAttributesMessage::getTailDrop() const
    {
        return (m_tailDrop);
    }

    void QosClientConfigIntfNsmAttributesMessage::setTailDrop(const UI32 &tailDrop)
    {
        m_tailDrop = tailDrop;
    }

	const void *QosClientConfigIntfNsmAttributesMessage::getCStructureForInputs ()
    {
	  	qos_interface_config_msg_t *pInput = (qos_interface_config_msg_ *)malloc (sizeof (struct qos_interface_config_msg_));
		strncpy (pInput->ifname, m_ifName.c_str(), 10);
		pInput->cmd_code = m_cmdCode;
		pInput->op_code = m_OpCode; 
		switch (m_OpCode) {
		  case CONFIGURE_INTERFACE_QOS:
			{
				switch (m_cmdCode) {
		  			case QOS_INTERFACE_TRUST_COS:
						break;
					case QOS_INTERFACE_TRUST_DSCP:
                        break;
					case QOS_INTERFACE_DEFAULT_COS:
						pInput->default_cos = m_defaultCosValue;
						break;
					case QOS_INTERFACE_FLOW_CONTROL_TX:
			  			pInput->flowcontrol_tx = m_flowControlTx;
						break;
					case QOS_INTERFACE_FLOW_CONTROL_RX:
			  			pInput->flowcontrol_rx = m_flowControlRx;
						break;
					case QOS_INTERFACE_FLOW_CONTROL_BOTH:
			  			pInput->flowcontrol_tx = m_flowControlTx;
			  			pInput->flowcontrol_rx = m_flowControlRx;
						break;
					case QOS_INTERFACE_COS_THRESHOLD:
						pInput->cos_threshold[0] = m_cos0Threshold;
						pInput->cos_threshold[1] = m_cos1Threshold;
						pInput->cos_threshold[2] = m_cos2Threshold;
						pInput->cos_threshold[3] = m_cos3Threshold;
						pInput->cos_threshold[4] = m_cos4Threshold;
						pInput->cos_threshold[5] = m_cos5Threshold;
						pInput->cos_threshold[6] = m_cos6Threshold;
						pInput->cos_threshold[7] = m_cos7Threshold;
						break;
					case QOS_INTERFACE_COS_MUTATION:
						strncpy(pInput->cos_mutation_map, m_cosToCosMutationMapName.c_str(), MAP_NAME_MAX_STR);
						pInput->cos_mutation_map[MAP_NAME_MAX_STR - 1] = '\0';
						break;
					case QOS_INTERFACE_COS_TRAFFIC_CLASS:
						strncpy(pInput->cos_traffic_class_map, m_cosToTrafficClassMapName.c_str(), MAP_NAME_MAX_STR);
						pInput->cos_traffic_class_map[MAP_NAME_MAX_STR - 1] = '\0';
						break;
				    case  QOS_INTERFACE_DSCP_MUTATION:
                        strncpy(pInput->dscp_mutation_map, m_dscpToDscpMutationMapName.c_str(), MAP_NAME_MAX_STR);
						pInput->dscp_mutation_map[MAP_NAME_MAX_STR - 1] = '\0';
                        break;
                    case QOS_INTERFACE_DSCP_TRAFFIC_CLASS:
                        strncpy(pInput->dscp_traffic_class_map, m_dscpToTrafficClassMapName.c_str(), MAP_NAME_MAX_STR);
						pInput->dscp_traffic_class_map[MAP_NAME_MAX_STR - 1] = '\0';
                        break;	
					case QOS_INTERFACE_DSCP_COS:
						strncpy(pInput->dscp_cos_map, m_dscpToCosMapName.c_str(), MAP_NAME_MAX_STR);
						pInput->dscp_cos_map[MAP_NAME_MAX_STR - 1] = '\0';
                        break;
					case QOS_INTERFACE_CEE_MAP:
						strncpy(pInput->cee_map, m_ceeMapName.c_str(), MAP_NAME_MAX_STR);
						pInput->cee_map[MAP_NAME_MAX_STR - 1] = '\0';
						break;
					case QOS_INTERFACE_PFC_FLOW_CONTROL_TX:
						pInput->pfc_cos = m_pfcFlowControlCos;
						pInput->pfc_flowcontrol_tx = m_pfcFlowControlTx;
						break;
					case QOS_INTERFACE_PFC_FLOW_CONTROL_RX:
						pInput->pfc_cos = m_pfcFlowControlCos;
						pInput->pfc_flowcontrol_rx = m_pfcFlowControlRx;
						break;
					case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
						pInput->pfc_cos = m_pfcFlowControlCos;
						pInput->pfc_flowcontrol_tx = m_pfcFlowControlTx;
						pInput->pfc_flowcontrol_rx = m_pfcFlowControlRx;
						break;
					case QOS_INTERFACE_LONG_DISTANCE:
						pInput->distance = (UI32)m_distance;
						break;
					case QOS_INTERFACE_COS_PROFILE:
						pInput->cos = m_cos;
						pInput->red_profile = m_redProfile;
						break;
					case QOS_INTERFACE_TAIL_DROP:
						pInput->cos = m_cos;
						pInput->tail_drop = m_tailDrop;
						break;
				}	
				break;
			}
		  case DELETE_INTERFACE_QOS:
			{
			  switch(m_cmdCode) {
				case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
					pInput->pfc_cos = m_pfcFlowControlCos;
					break;
				case QOS_INTERFACE_COS_PROFILE:
				case QOS_INTERFACE_TAIL_DROP:
					pInput->cos = m_cos;
				break;
			  }
			  break;
			}
		}
		
        return (pInput);
    }

    void QosClientConfigIntfNsmAttributesMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
