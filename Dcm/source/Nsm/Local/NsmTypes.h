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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef NSMTYPESH_H
#define NSMTYPESH_H

namespace DcmNs
{

    typedef enum
    {
        NSMCREATEPHYINTF = 100 ,
        NSMUPDATEPHYINTF,
        NSMDELETEPHYINTF,
        NSMADDPHYINTFTOVLANINTF,
        NSMDELETEPHYINTFTOVLANINTF,
        NSMCREATEVLANINTFSP,
        NSMUPDATEVLANINTFSP,
        NSMDELETEVLANINTFSP,
        NSMCREATEPOINTFSP,
        NSMUPDATEPOINTFSP,
        NSMDELETEPOINTFSP,
        NSMADDPOTOVLANINTFSP,
        NSMDELETEPOTOVLANINTFSP,
        NSMADDPHYINTFTOPOINTF,
        NSMDELETEPHYINTFTOPOINTF,
        NSMCREATEPHYSTPCONFIG,
        NSMDELETEPHYSTPCONFIG,
        NSMUPDATEPHYSTPCONFIG,
        NSMCREATEPHYVLANSTPCONFIG,
        NSMDELETEPHYVLANSTPCONFIG,
        NSMUPDATEPHYVLANSTPCONFIG,
        NSMUPDATEPHYINSTANCESTPCONFIG,
        NSMCREATEPOSTPCONFIGSP,
        NSMDELETEPOSTPCONFIGSP,
        NSMUPDATEPOSTPCONFIGSP,
        NSMCREATEPOVLANSTPCONFIGSP,
        NSMDELETEPOVLANSTPCONFIGSP,
        NSMUPDATEPOVLANSTPCONFIGSP,
        NSMUPDATEPOINSTANCESTPCONFIGSP,
		NSMADDVLANCLASSIFIERTOPHY,
        NSMDELETEVLANCLASSIFIERTOPO,
        NSMCREATEVLANCLASSIFIERRULESP,
        NSMDELETEVLANCLASSIFIERRULESP,
        NSMUPDATEVLANCLASSIFIERGROUPSP,
        NSMDELETEVLANCLASSIFIERGROUPSP,
        NSMADDVLANCLASSIFIERTOPOSP,
        NSMDELETEVLANCLASSIFIERTOPOSP,
        NSMASSOCMACACLPOLICYPOVLANINTFSP,
        NSMASSOCMACACLPOLICYPHYINTF,
        /* IP Acl*/
		NSMASSOCIPACLPOLICYPOVLANINTFSP,
		NSMASSOCIPACLPOLICYPHYINTF,
        NSMLOCALASSOCIPACLPOLICYVEINTF,
		/* PBR */
		NSMLOCALPBRVEINTF,
		NSMLOCALPBRPHYINTF,
		NSMLOCALPBRPOINTF,
		IPPOLICYDELETEPBR,
		/* PBR */
        NSMUPDATELACPCONFIGSP,
		LACPLOCALSHOW,
		CONFIGMGMTLOCALSHOW,
		DOT1X_LOCAL_SHOW_ALL,
        DOT1X_LOCAL_REAUTHENTICATE_ALL,
        DOT1X_LOCAL_TEST_ALL,
		DOT1X_LOCAL_CLEAR_ALL,
		NSMLOCALFCOEVLAN,
	  	NSMDOT1XUPDATECONFIGSP,
	  	NSMDOT1XUPDATEPORTCONFIG,
		NSMLOCALCONFIGQOSATTRIBUTES,
		NSMQOSPOCONFIGSP,
		NSMQOSPFCCONFIGSP,
		NSMQOSPFCCONFIG,
		QOSCOSPROFILEANDTHRESHOLDCONFIG,
        QOSPOPROFILEANDTHRESHOLDSP,
        NSMLLDPINTFCONFSET,
		NSMLLDPINTERNAL,
		NSMDELETEBRIDGEXSTPCONFIG,
		NSMEVENT,
		NSMLOCALSHOWINTERFACE,
        NSMUPDATEIGMPSVLANLOCALCONFIG,
        NSMUPDATEIGMPSSGVLANLOCALCONFIG,
        NSMUPDATEIGMPSMRTRVLANLOCALCONFIG,
		NSMCREATEINTIPCONFIGSP,
		NSMCREATEINTSECIPLOCALCONFIGSP,
		NSMDELETEINTSECIPLOCALCONFIGSP,
		RMONCREATERMONCOLLECTIONSTATISTICS,
		RMONDELETERMONCOLLECTIONSTATISTICS,
        RMONCREATERMONCOLLECTIONHISTORY,
        RMONDELETERMONCOLLECTIONHISTORY,
		NSMLOCALPORTMEDIASHOWINTERFACE,
		NSMLOCALDEBUGCONFIG,
        NSMUPDATEPOPRIMARYLINK,
		NSMLOCALSHOWVLANCLASSIFIER,
		NSM_LOCAL_CLEAR_MSG,
		LACPLOCALCLEAR,
		NSMLOCALECMPCONFIG,
		NSMLOCALSHOWECMP,
		NSMQOSCOSTHRESHOLDCONFIG,
        NSMUPDATEBULKPHYINTF,
		NSMLOCALOSPFINTERFACECONFIG,
        NSMLOCALPIMINTFMSG,
		NSMLOCALPHYIPMSG,
        NSMLOCALINTFLOOPBACKMSG,
        NSMLOCALNODEIPMSG,
  		NSMLOCALPOLICYMAP,      
        POLICER_LOCAL_SHOW_ALL,
		POLICER_LOCAL_CLEAR_ALL,
        IGMPPOSVIINTFLOCALCONFIG,
        IGMPSGINTFLOCALCONFIG,
		IGMPPHYINTFLOCALCONFIG,
		ELDLOCALSHOW,
		ELDLOCALCLEAR,
		ELDGLOBALSETRXLIM,
		NSMLOCALVLAGCONFIG,
		NSMLOCALSHOWVLAG,
    	NSMLOCALRESERVEDVLAN,
		NSMSTORMCONTROLINTFSET,
		NSMLOCALDESTINATIONMIRRORPORT,
		NSMPORTSECURITYCONFIGMSG,
		NSMPORTSECURITYSTICKYMACMSG,
        NSMPHYINTFVRFCONF,
        NSMVEINTFVRFCONF,
		NSMPVLANTRUNKASSOCTOPHY,
		NSMPVLANTRUNKASSOCTOPOSP,
		NSMPVLANMAPPINGTOPHY,
		NSMPVLANMAPPINGTOPOSP,

        // Udld
        NSM_UDLD_INTF_CONFIG,
        NSM_UDLD_RESET_ALL,

        NSM_DELETE_MSTP_INST_CONFIG,    // Delete port instance config on MSTI delete/vlan membership change
		CONNECTOR_CONFIG_LOCAL_MESSAGE,

        NSM_SLOT_OFFLINE_MSG,
        INTERFACE_STATE_REPLAY_MSG,
		DHCPRELINTFLOCALCONFIG,
		SWITCHPORT_VLAN_CONFIG_LOCAL_MESSAGE,
		MAC_GROUP_CONFIG_LOCAL_MESSAGE,
        PROVISIONING_INFO,
		GVLAN_ENABLE_LOCAL_MESSAGE,
        NSM_LOCAL_VLAN_PROVISION_MSG,

		NSM_LOCAL_MESSAGE_NUMBER  // Leave this line at the last
    }NSM;

typedef enum
{
	LACP_TIMEOUT_DEFAULT,
    LACP_TIMEOUT_SHORT,
    LACP_TIMEOUT_LONG
} LacpTimeout;

typedef enum
{
    PVLAN_MAPPING_ADD=1,
    PVLAN_MAPPING_DEL=2
} PVlanMappingOper;

typedef enum
{
	LINE_SPEED_AUTO,
	LINE_SPEED_1000,
	LINE_SPEED_10000,
	LINE_SPEED_40000
}LineSpeed;

typedef enum
{
	LONG_DISTANCE_200=200,
	LONG_DISTANCE_2000=2000,
	LONG_DISTANCE_5000=5000,
	LONG_DISTANCE_10000=10000,
	LONG_DISTANCE_30000=30000
}LongDistance;

typedef enum
{
  CONFIGURE_INTERFACE_QOS = 0,
  DELETE_INTERFACE_QOS
} QOSOpCodes;

typedef enum
{
  CFGREPLAY_DEFAULT = 0,
  CFGREPLAY_POSTBOOT,
  CFGREPLAY_ISL_TO_EDGE,
  CFGREPLAY_MAX
} CfgReplayModes;

typedef enum
{
    CONNECTOR_CONFIG_SET_NAME = 0,
	CONNECTOR_CONFIG_DEL_NAME,
    CONNECTOR_CONFIG_SET_BREAKOUT,
    CONNECTOR_CONFIG_DEL_BREAKOUT,
    CONNECTOR_CONFIG_BULK_UPDATE
} ConnectorConfigCmdCode;

}
#endif                                            //NSMTYPESH_H