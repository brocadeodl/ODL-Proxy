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
 *   Author : pasu                                                     *
 **************************************************************************/

#include "ClientInterface/Ssm/SsmMacAclRuleMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Framework/Types/MacAddress2.h"
#include "Framework/Utils/TraceUtils.h"
namespace DcmNs
{

    SsmMacAclRuleMessage::SsmMacAclRuleMessage ()
        : DcmManagementInterfaceMessage (SSMMACACLRULE),
        SsmAssocRuleToPolicyMsgPayload (this)
    {
    }

    SsmMacAclRuleMessage::SsmMacAclRuleMessage (const string &policyName, const UI32 &policyType, 
		const UI64 &ruleid,const UI32 &action,const string &srcMac, const string &srcMacMask, 
		const AddrType &srcType, const string &dstMac, const string &dstMacMask, const AddrType &dstType, 
		const string &appType, const EtherType &appTypeEnum, const UI16 &appTypeInt, const bool &isCount, 
		const bool &isLog, const UI32 &opcode, const UI8 &userEnteredSeqid)
        : DcmManagementInterfaceMessage (SSMMACACLRULE),
        SsmAssocRuleToPolicyMsgPayload (this, policyName, policyType, ruleid, action, 
			srcMac, srcMacMask, srcType, dstMac, dstMacMask, dstType, appType, appTypeEnum, 
			appTypeInt, isCount, isLog, opcode, userEnteredSeqid)
    {
    }

    SsmMacAclRuleMessage::~SsmMacAclRuleMessage ()
    {
    }

    void  SsmMacAclRuleMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		SsmAssocRuleToPolicyMsgPayload::setupAttributesForSerialization();
    }

    unsigned char *SsmMacAclRuleMessage::macAddressfromStringtoBytes (const string srcDstMac) 
	{
         MacAddress2 temp_macAddress (srcDstMac);
         unsigned char *macaddress_bytesform = (unsigned char *)calloc (1,MAC_ADDR_SIZE);
		 int j = 0;
         for (int i = 0 ; i < 3; i++)
         {
             j = i + i;
             macaddress_bytesform[j] = (UI8) (temp_macAddress[i] / ((UI16) 256));
             macaddress_bytesform[j + 1] = (UI8) (temp_macAddress[i] % ((UI16) 256));
         }
		 return macaddress_bytesform;
    }

	const void *SsmMacAclRuleMessage::getCStructureForInputs ()
	{
        
        ssmAcl_dcmd_l2_rule_t *pInput = (ssmAcl_dcmd_l2_rule_t *)calloc (1, sizeof(ssmAcl_dcmd_l2_rule_t));
		
		
		unsigned char *src_macaddress_bytesform 		= NULL;
		unsigned char *dst_macaddress_bytesform 		= NULL;
		unsigned char *src_macaddress_mask_bytesform 	= NULL; 
		unsigned char *dst_macaddress_mask_bytesform 	= NULL; 
		
		if (m_userEnteredSeqid) {
			pInput->user_entered_sequence_number 	= m_userEnteredSeqid; 
			((pInput->rule).generic).seq_num        = m_ruleid;
		} 
        
        (((pInput->rule).generic).results).forwarding = (ssmAcl_forwarding_result)m_action;
        
		if (m_isCount)
            (((pInput->rule).generic).results).supplemental |= SSMACL_SUPPLEMENTAL_RESULT_COUNT_E;
			
		if (m_isLog) 
            (((pInput->rule).generic).results).supplemental |= SSMACL_SUPPLEMENTAL_RESULT_LOG_E;
			

        if ((m_srcType == ADDR_TYPE_ANY) && (strcmp (m_srcMac.c_str (),"any") == 0)) {
			((pInput->rule).src_mac).valid = SSMACL_KEY_ELEMENT_ANY;
		} else if ((m_srcType == ADDR_TYPE_HOST) && (strcmp (m_srcMac.c_str (),"any") != 0)) {
			((pInput->rule).src_mac).valid = SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_ADDR_MASK_HOST;
			src_macaddress_bytesform 		= macAddressfromStringtoBytes (m_srcMac);
			memcpy (((pInput->rule).src_mac).addr, src_macaddress_bytesform, MAC_ADDR_SIZE);
			memset(((pInput->rule).src_mac).mask, 0xFF, MAC_ADDR_SIZE);
		} else if ((strcmp (m_srcMac.c_str (),"any") != 0) && (strcmp (m_srcMac.c_str (),"") != 0)) {
			((pInput->rule).src_mac).valid 	= SSMACL_KEY_ELEMENT_VALID;
			src_macaddress_bytesform 		= macAddressfromStringtoBytes (m_srcMac);
			src_macaddress_mask_bytesform 	= macAddressfromStringtoBytes (m_srcMacMask);
			memcpy (((pInput->rule).src_mac).addr, src_macaddress_bytesform, MAC_ADDR_SIZE);
			memcpy (((pInput->rule).src_mac).mask, src_macaddress_mask_bytesform, MAC_ADDR_SIZE);
		}
		
		
        if (m_policyType == SSMACL_TYPE_L2_EXT_E) {
			if ((m_dstType == ADDR_TYPE_ANY) && (strcmp (m_dstMac.c_str (),"any") == 0)) {
				((pInput->rule).dst_mac).valid = SSMACL_KEY_ELEMENT_ANY;
			} else if ((m_dstType == ADDR_TYPE_HOST) && (strcmp (m_dstMac.c_str (),"any") != 0)) {
				((pInput->rule).dst_mac).valid 	= SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_ADDR_MASK_HOST;
				dst_macaddress_bytesform = macAddressfromStringtoBytes (m_dstMac);
				memcpy (((pInput->rule).dst_mac).addr, dst_macaddress_bytesform, MAC_ADDR_SIZE);
				memset(((pInput->rule).dst_mac).mask, 0xFF, MAC_ADDR_SIZE);
			} else if ((strcmp (m_dstMac.c_str (),"any") != 0) && (strcmp (m_dstMac.c_str (),"") != 0)) {
				((pInput->rule).dst_mac).valid 	= SSMACL_KEY_ELEMENT_VALID;
				dst_macaddress_bytesform 		= macAddressfromStringtoBytes (m_dstMac);
				dst_macaddress_mask_bytesform 	= macAddressfromStringtoBytes (m_dstMacMask);
				memcpy (((pInput->rule).dst_mac).addr, dst_macaddress_bytesform, MAC_ADDR_SIZE);
				memcpy (((pInput->rule).dst_mac).mask, dst_macaddress_mask_bytesform, MAC_ADDR_SIZE);
			}

			if (strcmp (m_appType.c_str (), "appTypeInt") == 0) {
				(pInput->rule).ether_type.value = m_appTypeInt; 
				(pInput->rule).ether_type.valid = SSMACL_KEY_ELEMENT_VALID;
				(pInput->rule).ether_type.mask	= 0xFFFF;
			} else if (strcmp (m_appType.c_str (), "appTypeEnum") == 0) {
				(pInput->rule).ether_type.value = (uint16_t) m_appTypeEnum;
				(pInput->rule).ether_type.valid = SSMACL_KEY_ELEMENT_VALID;
				(pInput->rule).ether_type.mask  = 0xFFFF;
			} else if (strcmp (m_appType.c_str (), "") == 0)
				(pInput->rule).ether_type.valid = 0;
		
		}
			
		
        if (src_macaddress_mask_bytesform)
			free (src_macaddress_mask_bytesform);
		if (dst_macaddress_mask_bytesform)
			free (dst_macaddress_mask_bytesform);
		if (src_macaddress_bytesform)
			free (src_macaddress_bytesform);
		if (dst_macaddress_bytesform)
			free (dst_macaddress_bytesform);
		
		strncpy (pInput->acl_name, m_policyName.c_str (), SSMACL_NAME_SIZE);
        pInput->opcode = (ssmAcl_dcmd_opcode_t) m_opcode;
        
		return (pInput);
	}

	void SsmMacAclRuleMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        if (NULL != pOutputCStructure) {
            m_ruleid = *((unsigned long long *)pOutputCStructure);
        }
	}

}
