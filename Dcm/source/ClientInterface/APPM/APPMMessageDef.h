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
 *   Author : Kunjan Kumar Verma, Shivanand                                *
 **************************************************************************/

#ifndef __APPM_MESSAGE_DEF_H__
#define __APPM_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define MAX_NO_PROFILES  751
#define MAX_WYSEREA_PROFILE_NAME_LEN 128 
#define MAX_WYSEREA_ACCESS_GROUP_NAME_LEN 64
#define MAX_INTERFACE_NAME_LEN 64
#define MAC_ADDR_STR_LEN 18
#define QOS_PROFILE_MAP_NAME_LENGTH 64

#define APPM_DEFAULT_VLAN 1
#define APPM_INVALID_VLAN 0
#define APPM_MAX_CTAG_ID  4096

#define AMPP_PLUGIN_LOCAL_VERSION   0
#define AMPP_PLUGIN_GLOBAL_VERSION  0
#define AMPP_OPERATIONAL_DATA_PAGE_SIZE 1000 

/*
 * SUCCESS/FAILURE Macros.
 */

#define APPM_DCM_SUCCESS 0
#define APPM_DCM_ERROR  1

/* 
 * Default Values.
 */
#define APPM_QOS_WYSEREA_DEFAULT_COS 0
#define APPM_QOS_WYSEREA_DEFAULT_TRUST 0
#define APPM_QOS_WYSEREA_DEFAULT_FLOWCONTROL 2

#define APPM_NSM_NATIVE_VLAN_DEFAULT  0 
#define DEFAULT_PORT_PROFILE_DOMAIN_NAME "default"
#define DEFAULT_PORT_PROFILE_NAME "default"
#define DEFAULT_VLAN_PORT_PROFILE_NAME "UpgradedVlanProfile"

//Management Cluster
#define APPM_INVALID_LOCATION_ID 0
#define APPM_INVALID_RBRIDGE_ID  0

//Pagination related constants
#define startingInterfaceName  "#START#"
#define startingMAC  "0000.0000.0000"


/* Message Structs */
typedef struct portProfileMsg_t_ {
    unsigned char opCode;
    unsigned char cmdCode;
    char          profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
    unsigned int  profileId;
    unsigned char isActivated;
	unsigned char isAllow;
    char          profileMacStr[MAC_ADDR_STR_LEN];
	unsigned char isFloodingRestricted;
} portProfileMsg_t;

typedef struct portProfileDomainMessage_t_ {
    uint32_t      opCode;
    uint32_t      cmdCode;
    char          portProfileDomainName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
    char          portProfileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
} portProfileDomainMessage_t;


typedef struct portProfile_t_ {
    char          portProfileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
} portProfile_t;

typedef struct portProfileDomain_t_ {
    char          portProfileDomainName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
} portProfileDomain_t;

/* Below struct is used to mention the ifname and its associated
   port-profiles.*/
typedef struct interfaceToPortProfileAssociation_t_ {
    uint32_t                cmdCode;
    uint32_t                ifType;
    char                    ifName[MAX_INTERFACE_NAME_LEN + 1];
    portProfileDomain_t     portProfileDomain;
    uint32_t      	        numberOfPortProfiles;
    portProfile_t           portProfiles[1];
} interfaceToPortProfileAssociation_t;

/* Below struct is used to mention the ifname and its associated
port-profile-domain.*/
typedef struct interfaceToPortProfileDomainAssociation_t_ {
	uint32_t			numberOfPortProfileDomains;
	char 				ifName[MAX_INTERFACE_NAME_LEN + 1];
	portProfileDomain_t portProfileDomains[1];
} interfaceToPortProfileDomainAssociation_t;

/* Message Structs */
typedef struct portProfileMacMsg_t_
{
char    profileMacsStr[MAC_ADDR_STR_LEN];
unsigned char macErrorFlag;
} portProfileMacMsg_t;

typedef struct portProfileBulkMsg_t_ {
    unsigned char opCode;
    unsigned char cmdCode;
    char          profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
    unsigned int  profileId;
    unsigned char isActivated;
    unsigned char isAllow;
    unsigned int  macCount;
    unsigned char  allSuccessFlag;
    portProfileMacMsg_t macListAndFlags[1];
} portProfileBulkMsg_t;

typedef enum {
    APPM_VLAN_NOCLASSIFICATION,
    APPM_VLAN_CTAGCLASSIFICATION,
    APPM_VLAN_MACCLASSIFICATION,
    APPM_VLAN_MACGROUPCLASSIFICATION,
    APPM_VLAN_VNICLASSIFICATION
} APPMVlanClassificationType_t;

typedef struct MacAddress_t_ {
    char macAddress[MAC_ADDR_STR_LEN];
} MacAddress_t;

typedef struct MacGroup_t_ {
    uint32_t macGroupId;
} MacGroup_t;

typedef struct Ctag_t_ {
    u_int32_t ctag;
} Ctag_t;

typedef struct Vni_t_ {
    u_int32_t vni;
} Vni_t;

/* The below union corresponds to a vlan classification value */
typedef union APPMVlanClassificationValue_t_ {
    MacAddress_t mac;
    MacGroup_t   macGroup;
    Ctag_t       ctag;
    Vni_t        vni;
} APPMVlanClassificationValue_t;

typedef struct vlanProfileMsg_t_ {
    unsigned char                   opCode;
    unsigned char                   cmdCode;
    char                            profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
    unsigned int                    profileId;
    unsigned char                   m_swModeFlag;
    unsigned char                   m_swMode;
    unsigned char                   m_isAllowedVlanAll;
    unsigned char                   m_isAllowedVlanNone;
    unsigned int                    m_accessVlan;
    char*                           m_vlanRange;
    unsigned int                    m_vlanClassifierGroupId;
    unsigned int                    m_vlanClassifierVlan;
    unsigned int                    m_nativeVlan;
    uint32_t                        trunkVlanId;
    APPMVlanClassificationType_t    vlanClassificationType;    
    uint32_t                        numberOfVlanClassifications;
    APPMVlanClassificationValue_t   vlanClassificationRecords[1];
} vlanProfileMsg_t;

typedef struct secProfileMsg_t_ {
    unsigned char opCode;
    unsigned char cmdCode;
    char          profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
    unsigned int  profileId;
    char          accessGroup[MAX_WYSEREA_ACCESS_GROUP_NAME_LEN + 1];
} secProfileMsg_t;

typedef struct fcoeProfileMsg_t_ {
    unsigned char opCode;
    unsigned char cmdCode;
    char          profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
    unsigned int  profileId;
    unsigned char isFcoePort;
} fcoeProfileMsg_t;

typedef struct qosProfileMsg_t_ {
    char opCode;
    char cmdCode;
    char profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
    char mapName[QOS_PROFILE_MAP_NAME_LENGTH];
    unsigned int cos;
    unsigned int tx;
    unsigned int rx;
    unsigned int  profileId;
} qosProfileMsg_t;

typedef struct AppmShowMsgInpt_t_ {
    unsigned char opCode;
    unsigned char cmdCode;
    uint32_t profileId;
    unsigned int numOfRecords;
    unsigned char fetchModelType; // fetch model value is 0 for CLI get request and 1 for Netconf getNext request.
    char profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
    char lastProfileMac[MAC_ADDR_STR_LEN];
    char ifName[MAX_INTERFACE_NAME_LEN + 1];
    char portProfileDomainName[MAX_WYSEREA_PROFILE_NAME_LEN + 1]; // to get the operational data of a port-profile-domain.
} AppmShowMsgInpt_t;

typedef struct AppmInterfaceMsg_s {
        char ifLongName[MAX_INTERFACE_NAME_LEN + 1];
        unsigned int ifIndex;
        char profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
} AppmInterfaceMsg_t;

typedef struct AppmShowStatusMsg_s {
        char profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
        uint32_t ppId;
        char isActivated[4];
        char profileMac[MAC_ADDR_STR_LEN];
        char ifName[MAX_INTERFACE_NAME_LEN + 1];
} AppmShowStatusMsg_t;


typedef struct AppmProvidePPMsg_s {
        char profileName[MAX_WYSEREA_PROFILE_NAME_LEN + 1];
        uint32_t profileId;
} AppmProvidePPMsg_t;

typedef struct AppmShowMsgOutpt_t_ {
    unsigned int num_records;
    unsigned int record_type;
    unsigned int isMoreFlag;
    union {
        AppmInterfaceMsg_t appmInterfaceMsg[1];
        AppmShowStatusMsg_t appmStatusMsg[1];
		AppmProvidePPMsg_t appmPPList[1];
    } records;
} AppmShowMsgOutpt_t;

/* Enum Variables */
typedef enum
{
    APPM_CREATE = 0,
    APPM_REMOVE,
    APPM_SET_ELEM,
    APPM_SSM_MSG,
    APPM_QOS_MSG,
    APPM_VCENTER
}AppmOpCode;

typedef enum {
    APPM_PROFILE_DOMAIN_CREATE = 7,
    APPM_PROFILE_DOMAIN_EDIT,
    APPM_PROFILE_DOMAIN_DELETE
}AppmProfileDomainOpCode;

//Do we really need to define an opcode?
//cmd codes are suffice??
typedef enum {
    PORT_PROFILE_DOMAIN_CREATE = 7,
    PORT_PROFILE_DOMAIN_DELETE,
    PORT_PROFILE_ADD_TO_PORT_PROFILE_DOMAIN,
    PORT_PROFILE_DELETE_FROM_PORT_PROFILE_DOMAIN
}PortProfileDomainCmdCode;

typedef enum
{
    PORT_PROFILE_CREATE = 0,
    PORT_PROFILE_DELETE,
    PORT_PROFILE_ACTIVATE,
    PORT_PROFILE_DEACTIVATE,
    PORT_PROFILE_ASSOCIATE,
    PORT_PROFILE_DEASSOCIATE,
	PORT_PROFILE_ALLOW_NONPROFILEDMACS,
	PORT_PROFILE_RESTRICT_FLOODING
}ProfileCmdCode;

typedef enum
{
    VLAN_SW_MODE = 0,
    VLAN_PROFILE_CREATE,
    VLAN_IS_ALLOWED_VLAN_ALL,
    VLAN_SW_MODE_FLAG,
    VLAN_SW_MODE_FLAG_DEL,
    VLAN_IS_ALLOWED_VLAN_NONE,
    VLAN_IS_ALLOWED_VLAN_ADD,
    VLAN_IS_ALLOWED_VLAN_REMOVE,
    VLAN_IS_ALLOWED_VLAN_EXCEPT,
	VLAN_NSM_INTF_DELETE,
    VLAN_ACCESS_VLAN,
	VLAN_ACCESS_VLAN_DEL,
    VLAN_GROUP_ID,
	VLAN_NATIVE_ID_ADD,
	VLAN_NATIVE_ID_DEL,
	VLAN_PROFILE_DEL,
    APPM_VLAN_CLASSIFICATION_SET_MAC_ADDR,
    APPM_VLAN_CLASSIFICATION_DEL_MAC_ADDR,
    APPM_VLAN_CLASSIFICATION_SET_MAC_GROUP,
    APPM_VLAN_CLASSIFICATION_DEL_MAC_GROUP,
    APPM_VLAN_CLASSIFICATION_SET_CTAG,
    APPM_VLAN_CLASSIFICATION_DEL_CTAG,
    APPM_VLAN_CLASSIFICATION_SET_NATIVE_CTAG,
    APPM_VLAN_CLASSIFICATION_DEL_NATIVE_CTAG,
      VLAN_VCENTER_CREATE, //to be removed
    VCENTER_PORT_PROFILE_DELETE,
    VCENTER_PORT_PROFILE_CREATE,
    VCENTER_PORT_PROFILE_ACTIVATE,
    VCENTER_PORT_PROFILE_DEACTIVATE,
    VCENTER_ACCESS_VLAN_PROFILE_CREATE,
    VCENTER_ACCESS_VLAN_PROFILE_UPDATE,
    VCENTER_TRUNK_VLAN_PROFILE_CREATE,
    VCENTER_TRUNK_VLAN_PROFILE_UPDATE,
    VCENTER_VLAN_PROFILE_DELETE,
    VCENTER_ASSOC_MAC,
    VCENTER_ASSOC_MACS,
    VCENTER_DEASSOC_MAC,
    VCENTER_DEASSOC_MACS,
    VCENTER_QOS_DEFAULT_COS_CREATE,
    VCENTER_QOS_DEFAULT_COS_UPDATE,
    VCENTER_QOS_DELETE,
	VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_ALL,
	VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE,
	VCENTER_TRUNK_VLAN_PROFILE_NATIVE_VLAN
}VlanCmdCodeType;

typedef enum
{
    VLAN_SW_MODE_INVALID = 0,
    VLAN_SW_MODE_ACCESS,
//	VLAN_SW_MODE_HYBRID,
    VLAN_SW_MODE_TRUNK
}VlanSwMode;

typedef enum
{
    SEC_PROFILE_CREATE = 0,
    SEC_PROFILE_DELETE,
    MAC_ACCESS_GRP_ADD,
    MAC_ACCESS_GRP_DEL
} SecCmdCode;

typedef enum
{
    FCOE_PROFILE_CREATE = 0,
    FCOE_PROFILE_DELETE,
    FCOE_PROFILE_FCOEPORT_ADD,
    FCOE_PROFILE_FCOEPORT_DEL
} FcoeCmdCode;

typedef enum
    {
       QOS_PROFILE_NO_OP = 0,
	   QOS_PROFILE_CREATE,
       QOS_PROFILE_CEE_MAP ,
       QOS_PROFILE_NO_CEE_MAP,
       QOS_DEFAULT_COS,
       QOS_NO_DEFAULT_COS,
       QOS_TRUST,
       QOS_NO_TRUST,
       QOS_FLOW_CTRL_PFC,
       QOS_NO_FLOW_CTRL_PFC,
       QOS_FLOW_CTRL_ALL,
       QOS_NO_FLOW_CTRL_ALL,
       QOS_COS_TO_COS_MUTATION,
       QOS_NO_COS_TO_COS_MUTATION,
       QOS_COS_TO_TRAFFIC_MAP,
       QOS_NO_COS_TO_TRAFFIC_MAP,
	   QOS_PROFILE_DEL
    } QosCmdCodeType;

typedef enum
{
    APPM_SHOW_UNKNOWN = 0,
    APPM_SHOW_PROFILE_INTERFACE,
    APPM_SHOW_PROFILE_INTERFACE_ALL,
    APPM_SHOW_STATUS_RECORD, // make different structure
    APPM_SHOW_STATUS_ALL,
    APPM_SHOW_STATUS_ACTIVATED,
    APPM_SHOW_STATUS_ASSOCIATED,
    APPM_SHOW_STATUS_APPLIED,
    APPM_SHOW_PROFILE_STATUS_ALL,
    APPM_SHOW_PROFILE_STATUS_ACTIVATED,
    APPM_SHOW_PROFILE_STATUS_ASSOCIATED,
    APPM_SHOW_PROFILE_STATUS_APPLIED,
    //Below command codes are added to handle the pagination for show commands.
    APPM_PROVIDE_ALL_PORT_PROFILES,
    APPM_PROVIDE_ALL_ACTIVE_PORT_PROFILES,
    APPM_PROVIDE_ALL_ASSOCIATED_PORT_PROFILES,
    APPM_PROVIDE_ALL_APPLIED_PORT_PROFILES,
    //Below command codes are added to hanld the port-profile-domain show commands.
    APPM_PROVIDE_ALL_PORT_PROFILES_OF_A_PP_DOMAIN,
    APPM_PROVIDE_ALL_ACTIVE_PORT_PROFILES_OF_A_PP_DOMAIN,
    APPM_PROVIDE_ALL_ASSOCIATED_PORT_PROFILES_OF_A_PP_DOMAIN,
    APPM_PROVIDE_ALL_APPLIED_PORT_PROFILES_OF_A_PP_DOMAIN

} AppmShowCmdCode;

typedef enum{
    APPM_NO_RECORDS = 0,
    APPM_MORE_RECORDS,
    APPM_INVALID_RECORDS
}AppmIsMoreFlag;

//The below enum is used to differentiate between the get and the getNext fetch models.
typedef enum{
	APPM_GET_FETCH_MODEL = 0,
	APPM_GET_NEXT_FETCH_MODEL
}AppmFetchModelType;

typedef union appm_buffer_num_u {
    struct {
        uint16_t   mapped_id;
        uint16_t   tag;
    } buff;
    uint32_t   num;
} appm_buffer_num_t;

static inline uint16_t
appm_get_buffer_tag (uint32_t  buff_num)
{
    return (((appm_buffer_num_t*) &buff_num)->buff.tag);
}

static inline u_int16_t
appm_get_buffer_mapped_id(uint32_t  buff_num)
{
    return (((appm_buffer_num_t*) &buff_num)->buff.mapped_id);
}

#endif    /* __APPM_MESSAGE_DEF_H__ */
