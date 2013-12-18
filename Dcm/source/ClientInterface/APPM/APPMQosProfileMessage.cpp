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
 *   Author : Ankur Sharma                                                 *
 **************************************************************************/

#include "ClientInterface/APPM/APPMQosProfileMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"


namespace DcmNs
{

    APPMQosProfileMessage::APPMQosProfileMessage ()
        : DcmManagementInterfaceMessage (APPMQOSPROFILE)
    {
    }

    APPMQosProfileMessage::APPMQosProfileMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const string &qosMapName,const UI32 &qosCos,const UI32 &tx,const UI32 &rx, const UI32 &profileId)
        : DcmManagementInterfaceMessage (APPMQOSPROFILE), 
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_qosMapName    (qosMapName),
        m_qosCos        (qosCos),
        m_tx            (tx),
        m_rx             (rx),
        m_profileId    (profileId)
    {
    }

    APPMQosProfileMessage::~APPMQosProfileMessage ()
    {
    }

    void  APPMQosProfileMessage::setupAttributesForSerialization()
    {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeString(&m_qosMapName,"qosMapName"));
        addSerializableAttribute (new AttributeUI32(&m_qosCos,"qosCos"));
        addSerializableAttribute (new AttributeUI32(&m_tx,"tx"));
        addSerializableAttribute (new AttributeUI32(&m_rx,"rx"));
    }

    void  APPMQosProfileMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMQosProfileMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMQosProfileMessage::setCmdCode(const UI8 cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMQosProfileMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMQosProfileMessage::setQosProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMQosProfileMessage::getQosProfileName() const
    {
        return (m_profileName);
    }

    void  APPMQosProfileMessage::setQosMapName(const string &mapName)
    {
        m_qosMapName  =  mapName;
    }

    string  APPMQosProfileMessage::getQosMapName() const
    {
        return (m_qosMapName);
    }

    void  APPMQosProfileMessage::setQosCos(const UI32 qosCos)
    {
        m_qosCos  =  qosCos;
    }

    UI32  APPMQosProfileMessage::getQosCos() const
    {
        return (m_qosCos);
    }

    void  APPMQosProfileMessage::setQosTx(const UI32 tx)
    {
        m_tx  =  tx;
    }

    UI32  APPMQosProfileMessage::getQosTx() const
    {
        return (m_tx);
    }

    void  APPMQosProfileMessage::setQosRx(const UI32 rx)
    {
        m_rx  =  rx;
    }

    UI32  APPMQosProfileMessage::getQosRx() const
    {
        return (m_rx);
    }
    
    void  APPMQosProfileMessage::setProfileId(const UI32 &profileId){
        m_profileId  =  profileId;
    }
    
    UI32  APPMQosProfileMessage::getProfileId() const {
        return (m_profileId);
    }



    const void *APPMQosProfileMessage::getCStructureForInputs ()
    {
        qosProfileMsg_t *pInput = new qosProfileMsg_t;
       
	    pInput->opCode = m_opCode; 
	    pInput->cmdCode = m_cmdCode; 
        strncpy (pInput->profileName, m_profileName.c_str (), MAX_WYSEREA_PROFILE_NAME_LEN);
		pInput->profileName[MAX_WYSEREA_PROFILE_NAME_LEN] = '\0';
        strncpy (pInput->mapName, m_qosMapName.c_str(), QOS_PROFILE_MAP_NAME_LENGTH);
	    pInput->cos = m_qosCos; 
        
	    pInput->tx = m_tx; 
        pInput->rx = m_rx;

        pInput->profileId = m_profileId;

        return (pInput);
    }

	void APPMQosProfileMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
