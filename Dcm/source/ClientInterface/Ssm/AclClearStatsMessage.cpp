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
 *   Author : Navin                                                       *
 **************************************************************************/

#include "ClientInterface/Ssm/AclClearStatsMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ssmAcl_dcmd_public.h"

namespace DcmNs
{

    AclClearStatsMessage::AclClearStatsMessage() : DcmManagementInterfaceMessage (SSMMACIPACLCLEAR)
    {
    }

    AclClearStatsMessage::AclClearStatsMessage(const string &PolicyName, const string &ifName, const UI32 &opCode, const UI32 &aclType, UI8 detail, const UI32 &direction , const UI32 &mappedId)
        : DcmManagementInterfaceMessage (SSMMACIPACLCLEAR),
        m_PolicyName   (PolicyName),
        m_ifName       (ifName),
        m_opCode       (opCode),
        m_aclType      (aclType),
        m_detail       (detail),
        m_direction    (direction),
		m_mappedId     (mappedId)
    {
    }

    AclClearStatsMessage::~AclClearStatsMessage ()
    {
    }

    void  AclClearStatsMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute(new AttributeString(&m_PolicyName,"PolicyName"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_aclType,"aclType"));
        addSerializableAttribute(new AttributeUI8(&m_detail,"detail"));
        addSerializableAttribute(new AttributeUI32(&m_direction,"direction"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  AclClearStatsMessage::setPolicyName(const string &PolicyName)
    {
        m_PolicyName  =  PolicyName;
    }

    string  AclClearStatsMessage::getPolicyName() const
    {
        return (m_PolicyName);
    }

    void  AclClearStatsMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  AclClearStatsMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  AclClearStatsMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  AclClearStatsMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  AclClearStatsMessage::setAclType(const UI32 &aclType)
    {
        m_aclType = aclType;
    }

    UI32  AclClearStatsMessage::getAclType() const
    {
        return (m_aclType);
    }

    void  AclClearStatsMessage::setDetail(UI8 detail)
    {
        m_detail = detail;
    }

    UI8  AclClearStatsMessage::getDetail() const
    {
        return (m_detail);
    }

    void AclClearStatsMessage::setDirection(const UI32 &direction)
    {
        m_direction = direction;
    }

    UI32  AclClearStatsMessage::getDirection() const
    {
        return (m_direction);
    }

    void  AclClearStatsMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  AclClearStatsMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    const void *AclClearStatsMessage::getCStructureForInputs ()
    {
        ssmAcl_dcmd_acl_show_request_t *pInput = (ssmAcl_dcmd_acl_show_request_t *) calloc (1, sizeof(ssmAcl_dcmd_acl_show_request_t));
        memset(pInput, 0, sizeof(ssmAcl_dcmd_acl_show_request_t));

        strncpy (pInput->acl_name, m_PolicyName.c_str(), SSMACL_NAME_SIZE);
        strncpy (pInput->if_name, m_ifName.c_str(), 64);
        pInput->opcode     = (ssmAcl_dcmd_opcode_t)m_opCode;
		pInput->direction  = (ssmAcl_binding_direction)m_direction;
		pInput->type	   = (ssmAcl_type)m_aclType;
        return (pInput);
    }

}
