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

#include "ClientInterface/Ssm/SsmMacAclPolicyBulkMessage.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Framework/Types/MacAddress2.h"
namespace DcmNs
{

    SsmMacAclPolicyBulkMessage::SsmMacAclPolicyBulkMessage ()
        : DcmManagementInterfaceMessage (SSMMACACLBULKPOLICY),
		m_numRules (0)
    {
    }

    /*SsmMacAclPolicyBulkMessage::SsmMacAclPolicyBulkMessage (const string &policyName,const UI32 &policyType, const UI32 opCode)
        : DcmManagementInterfaceMessage (SSMMACACLBULKPOLICY),
        m_policyName    (policyName),
        m_policyType    (policyType),
        m_opCode		(opCode)
    {
    }

    SsmMacAclPolicyBulkMessage::SsmMacAclPolicyBulkMessage (const string &policyName,const string &ifname, const UI32 opCode)
        : DcmManagementInterfaceMessage (SSMMACACLPOLICY),
        m_policyName    (policyName),
        m_ifname		(ifname),
        m_opCode		(opCode)
    {
    }*/

    SsmMacAclPolicyBulkMessage::~SsmMacAclPolicyBulkMessage ()
    {
    }

    void  SsmMacAclPolicyBulkMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_policyName, "policyName"));
        addSerializableAttribute (new AttributeUI32(&m_policyType, "policyType"));
        addSerializableAttribute (new AttributeUI64(&m_numRules, "numRules"));
        addSerializableAttribute (new AttributeUI64Vector(&m_ruleid, "ruleid"));
        addSerializableAttribute (new AttributeUI32Vector(&m_action, "action"));
        addSerializableAttribute (new AttributeStringVector(&m_srcMac, "srcMac"));
        addSerializableAttribute (new AttributeStringVector(&m_srcMacMask, "srcMacMask"));
        addSerializableAttribute (new AttributeStringVector(&m_dstMac, "dstMac"));
        addSerializableAttribute (new AttributeStringVector(&m_dstMacMask, "dstMacMask"));
        addSerializableAttribute (new AttributeStringVector(&m_appType, "appType"));
        addSerializableAttribute (new AttributeBoolVector(&m_isCount, "isCount"));
        addSerializableAttribute (new AttributeBoolVector(&m_isLog, "isLog"));
		addSerializableAttribute (new AttributeUI32Vector(&m_srcType, "srcType"));
		addSerializableAttribute (new AttributeUI32Vector(&m_dstType, "dstType"));
		addSerializableAttribute (new AttributeUI32Vector(&m_appTypeInt, "appTypeInt"));
		addSerializableAttribute (new AttributeUI32Vector(&m_appTypeEnum, "appTypeEnum"));
		addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
    }

    void  SsmMacAclPolicyBulkMessage::setPolicyName(const string &policyName)
    {
        m_policyName = policyName;
    }

    string  SsmMacAclPolicyBulkMessage::getPolicyName() const
    {
        return (m_policyName);
    }

    void  SsmMacAclPolicyBulkMessage::setOpCode(const UI32 &opCode)
    {
        m_opcode  =  opCode;
    }

    UI32  SsmMacAclPolicyBulkMessage::getOpCode() const
    {
        return (m_opcode);
    }

    void  SsmMacAclPolicyBulkMessage::setPolicyType(const UI32 &policyType)
    {
        m_policyType  =  policyType;
    }

    UI32  SsmMacAclPolicyBulkMessage::getPolicyType() const
    {
        return (m_policyType);
    }

    void  SsmMacAclPolicyBulkMessage::setNumRules(const UI64 &rules)
    {
        m_numRules  =  rules;
    }

	void SsmMacAclPolicyBulkMessage::setStdRuleParams(const UI64 &ruleid, const UI32 &action, 
		const string &srcmac, const string &srcmacmask, const bool &iscount, const bool &islog, 
		const UI32 &srcType)
	{
		m_ruleid.push_back(ruleid);
		m_action.push_back(action);
		m_srcMac.push_back(srcmac);
		m_srcMacMask.push_back(srcmacmask);
		m_isCount.push_back(iscount);
		m_isLog.push_back(islog);
		m_srcType.push_back(srcType);
	}
	
	void SsmMacAclPolicyBulkMessage::setExtRuleParams(const UI64 &ruleid, const UI32 &action, 
		const string &srcmac, const string &srcMacMask, const string &dstmac, const string &dstMacMask, 
		const string &appType, const bool &iscount, const bool &islog, const UI32 &srcType, 
		const UI32 &dstType, const UI32 &appTypeInt, const UI32 &appTypeEnum)
	{
		m_ruleid.push_back(ruleid);
		m_action.push_back(action);
		m_srcMac.push_back(srcmac);
		m_srcMacMask.push_back(srcMacMask);
		m_dstMac.push_back(dstmac);
		m_dstMacMask.push_back(dstMacMask);
		m_appType.push_back(appType);
		m_isCount.push_back(iscount);
		m_isLog.push_back(islog);
		m_srcType.push_back(srcType);
		m_dstType.push_back(dstType);
		m_appTypeInt.push_back(appTypeInt);
		m_appTypeEnum.push_back(appTypeEnum);
	}


    UI64  SsmMacAclPolicyBulkMessage::getNumRules() const
    {
        return (m_numRules);
    }

    vector<UI64>  SsmMacAclPolicyBulkMessage::getRuleidVector() const
    {
        return (m_ruleid);
    }


    vector<UI32>  SsmMacAclPolicyBulkMessage::getActionVector() const
    {
        return (m_action);
    }


    vector<string>  SsmMacAclPolicyBulkMessage::getSrcMacVector() const
    {
        return (m_srcMac);
    }


    vector<string>  SsmMacAclPolicyBulkMessage::getSrcMacMaskVector() const
    {
        return (m_srcMacMask);
    }


    vector<string>  SsmMacAclPolicyBulkMessage::getDstMacVector() const
    {
        return (m_dstMac);
    }


    vector<string>  SsmMacAclPolicyBulkMessage::getDstMacMaskVector() const
    {
        return (m_dstMacMask);
    }


    vector<string>  SsmMacAclPolicyBulkMessage::getAppTypeVector() const
    {
        return (m_appType);
    }


    vector<bool>  SsmMacAclPolicyBulkMessage::getIsCountVector() const
    {
        return (m_isCount);
    }


    vector<bool>  SsmMacAclPolicyBulkMessage::getIsLogVector() const
    {
        return (m_isLog);
    }
	
    unsigned char *SsmMacAclPolicyBulkMessage::macAddressfromStringtoBytes (const string srcDstMac)
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

	const void *SsmMacAclPolicyBulkMessage::getCStructureForInputs ()
	{
        ssmAcl_dcmd_l2_rule_bulk_t *pInput 	= NULL;
        string 						pname  	= "";
       	UI64 num 							= m_numRules;
        UI64 i = 0;

        if (num > 0) {
			pInput = (ssmAcl_dcmd_l2_rule_bulk_t *)calloc(1, 
				(sizeof (ssmAcl_dcmd_l2_rule_bulk_t) + (num - 1) * sizeof(ssmAcl_dcmd_l2_rule_t)));
			
			if (pInput == NULL) {
				return (pInput);
			}

        	pInput->numrules 						= num;
        	pname 									= m_policyName;
			
        	strncpy (pInput->acl_name, pname.c_str (), SSMACL_NAME_SIZE);
        	pInput->type 							= (ssmAcl_type) m_policyType;
			/* Fix Me Set Below And Above one As per ssmAcl_dcmd_public.h */	
			pInput->opcode 							= (ssmAcl_dcmd_opcode_t) m_opcode;
        	for (i = 0; i < num; i++) {
				
				unsigned char *src_macaddress_bytesform         = NULL; 
				unsigned char *dst_macaddress_bytesform         = NULL;
				unsigned char *src_macaddress_mask_bytesform    = NULL; 
				unsigned char *dst_macaddress_mask_bytesform    = NULL; 

				(((pInput->rules[i]).rule).generic).seq_num = m_ruleid[i];
				strncpy ((pInput->rules[i]).acl_name, pname.c_str (), SSMACL_NAME_SIZE);
				/* Always 1 in case of postboot or configReply */
				(pInput->rules[i]).user_entered_sequence_number = 1;				

				((((pInput->rules[i]).rule).generic).results).forwarding = (ssmAcl_forwarding_result)m_action[i];
				
				if (m_isCount[i])
					((((pInput->rules[i]).rule).generic).results).supplemental |= SSMACL_SUPPLEMENTAL_RESULT_COUNT_E;

				
				if (m_isLog[i])
					((((pInput->rules[i]).rule).generic).results).supplemental |= SSMACL_SUPPLEMENTAL_RESULT_LOG_E;
				
				if ((m_srcType[i] == ADDR_TYPE_ANY) && (strcmp (m_srcMac[i].c_str (),"any") == 0)) {
					(((pInput->rules[i]).rule).src_mac).valid = SSMACL_KEY_ELEMENT_ANY;
				} else if ((m_srcType[i] == ADDR_TYPE_HOST) && (strcmp (m_srcMac[i].c_str (),"any") != 0)) {
					(((pInput->rules[i]).rule).src_mac).valid = SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_ADDR_MASK_HOST;
					src_macaddress_bytesform        		= macAddressfromStringtoBytes (m_srcMac[i]);
					memcpy ((((pInput->rules[i]).rule).src_mac).addr, src_macaddress_bytesform, MAC_ADDR_SIZE);
					memset((((pInput->rules[i]).rule).src_mac).mask, 0xFF, MAC_ADDR_SIZE);
				} else if ((strcmp (m_srcMac[i].c_str (),"any") != 0) && (strcmp (m_srcMac[i].c_str (),"") != 0)) {
					(((pInput->rules[i]).rule).src_mac).valid  	= SSMACL_KEY_ELEMENT_VALID;
					src_macaddress_bytesform					= macAddressfromStringtoBytes (m_srcMac[i]);
					memcpy ((((pInput->rules[i]).rule).src_mac).addr, src_macaddress_bytesform, MAC_ADDR_SIZE);
					src_macaddress_mask_bytesform				= macAddressfromStringtoBytes (m_srcMacMask[i]);
					memcpy ((((pInput->rules[i]).rule).src_mac).mask, src_macaddress_mask_bytesform, MAC_ADDR_SIZE);
				}
				
				if (m_policyType == SSMACL_TYPE_L2_EXT_E) {
					if ((m_dstType[i] == ADDR_TYPE_ANY) && (strcmp (m_dstMac[i].c_str (),"any") == 0)) {
						((pInput->rules[i]).rule.dst_mac).valid = SSMACL_KEY_ELEMENT_ANY;
					} else if ((m_dstType[i] == ADDR_TYPE_HOST) && (strcmp (m_dstMac[i].c_str (),"any") != 0)) {
						((pInput->rules[i]).rule.dst_mac).valid = SSMACL_KEY_ELEMENT_VALID | SSMACL_KEY_ADDR_MASK_HOST;
						dst_macaddress_bytesform 				= macAddressfromStringtoBytes (m_dstMac[i]);
						memcpy ((((pInput->rules[i]).rule).dst_mac).addr, dst_macaddress_bytesform, MAC_ADDR_SIZE);
						memset((((pInput->rules[i]).rule).dst_mac).mask, 0xFF, MAC_ADDR_SIZE);
					} else if ((strcmp (m_dstMac[i].c_str (),"any") != 0) && (strcmp (m_dstMac[i].c_str (),"") != 0)) {
						((pInput->rules[i]).rule.dst_mac).valid = SSMACL_KEY_ELEMENT_VALID;
						dst_macaddress_bytesform        		= macAddressfromStringtoBytes (m_dstMac[i]);
						memcpy ((((pInput->rules[i]).rule).dst_mac).addr, dst_macaddress_bytesform, MAC_ADDR_SIZE);
						dst_macaddress_mask_bytesform   		= macAddressfromStringtoBytes (m_dstMacMask[i]);
						memcpy ((((pInput->rules[i]).rule).dst_mac).mask, dst_macaddress_mask_bytesform, MAC_ADDR_SIZE);
					}

					if (strcmp (m_appType[i].c_str (), "appTypeInt") == 0) {
						(pInput->rules[i]).rule.ether_type.value = m_appTypeInt[i];
						(pInput->rules[i]).rule.ether_type.valid = SSMACL_KEY_ELEMENT_VALID;
						(pInput->rules[i]).rule.ether_type.mask  = 0xFFFF;
					} else if (strcmp (m_appType[i].c_str (), "appTypeEnum") == 0) {
						(pInput->rules[i]).rule.ether_type.value = (uint16_t) m_appTypeEnum[i];
						(pInput->rules[i]).rule.ether_type.valid = SSMACL_KEY_ELEMENT_VALID;
						(pInput->rules[i]).rule.ether_type.mask  = 0xFFFF;
					} else if (strcmp (m_appType[i].c_str (), "") == 0)
						(pInput->rules[i]).rule.ether_type.valid = 0;
				}

				if (src_macaddress_mask_bytesform)
					free (src_macaddress_mask_bytesform);
				if (dst_macaddress_mask_bytesform)
					free (dst_macaddress_mask_bytesform);
				if (src_macaddress_bytesform)
					free (src_macaddress_bytesform);
				if (dst_macaddress_bytesform)
					free (dst_macaddress_bytesform);

				(pInput->rules[i]).opcode = SSMACL_DCMD_ADD_RULE;
			}

		} else  {
        	pInput = (ssmAcl_dcmd_l2_rule_bulk_t *)calloc(1, (sizeof (ssmAcl_dcmd_l2_rule_bulk_t)));
            if (pInput == NULL) {
				return (pInput);
			}
			
			pInput->numrules                        = num;
            pname                                   = m_policyName;

            strncpy (pInput->acl_name, pname.c_str (), SSMACL_NAME_SIZE);
            pInput->type                            = (ssmAcl_type) m_policyType;
            /* Fix Me Set Below And Above one As per ssmAcl_dcmd_public.h */
            pInput->opcode                          = (ssmAcl_dcmd_opcode_t) m_opcode;

		}
        
        return (pInput);
	}

	void SsmMacAclPolicyBulkMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
