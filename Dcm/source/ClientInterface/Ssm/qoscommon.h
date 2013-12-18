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

#ifndef __QOSCOMMON_H
#define __QOSCOMMON_H

#include <stdio.h>
#include <unistd.h>
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

#define STR32 32
#define MAP_NAME_MAX_STR	65
#define QOS_OUT_OF_RANGE_VALUE		0xFFFFFFFF
#ifndef QOS_EANVIL
  #define QOS_EANVIL
#endif
#define QOS_SHOW_NUMBER_OF_RECORDS 1
#define TMC_FLAT_CHIP_MAX 48

typedef struct QoSCeeMapMsg_ {
	char ceeMapName[MAP_NAME_MAX_STR];
	int precedence;
	int configReplay;
} QosCeeMapMsg_t;

typedef struct QoSCeeRemapMsg_ {
    char ceeMapName[MAP_NAME_MAX_STR];
    int remapPriority;
	int remapFabricP;
    int configReplay;
} QosCeeRemapMsg_t;

typedef struct QoSPriorityGroupMsg_ {
	char ceeMapName[MAP_NAME_MAX_STR];
	char pgid[8];
	int weight;
	int pfc;
    int configReplay;
} QosPriorityGroupMsg_t;

typedef struct QosPriorityMapMsg_ {
	char ceeMapName[MAP_NAME_MAX_STR];
	char pgid0[8];
	char pgid1[8];
    char pgid2[8];
    char pgid3[8];
    char pgid4[8];
    char pgid5[8];
    char pgid6[8];
    char pgid7[8];
    int configReplay;
} QosPriorityMapMsg_t;

typedef struct QosCosToCosMsg_ {
    char cosToCosMutationMapName[MAP_NAME_MAX_STR];
  	int cos0MutatedCosValue; 
  	int cos1MutatedCosValue; 
  	int cos2MutatedCosValue; 
  	int cos3MutatedCosValue; 
  	int cos4MutatedCosValue; 
  	int cos5MutatedCosValue; 
  	int cos6MutatedCosValue; 
  	int cos7MutatedCosValue; 
} QosCosToCosMsg_t;

typedef struct QosCosToTrafficClassMsg_ {
    char cosToTrafficClassMapName[MAP_NAME_MAX_STR];
  	int cos0MappedTrafficClassValue; 
  	int cos1MappedTrafficClassValue; 
  	int cos2MappedTrafficClassValue; 
  	int cos3MappedTrafficClassValue; 
  	int cos4MappedTrafficClassValue; 
  	int cos5MappedTrafficClassValue; 
  	int cos6MappedTrafficClassValue; 
  	int cos7MappedTrafficClassValue; 
} QosCosToTrafficClassMsg_t;

typedef enum McastOpcode_ {
	MCAST_THRESHOLD = 0,
	MCAST_RATELIMIT,
	MCAST_RATE_BURST
} McastOpcode_t;

typedef enum SWBDPLATFORM_ {
	MERCURY_PLATFORM = 1,
	CALLISTO_PLATFORM,
	CASTOR_PLATFORM
} swbd_platform_t;

typedef enum SsmQosClearCounterOpcode_ {
	SSM_QOS_CLEAR_COUNTERS_ALL = 0,
	SSM_QOS_CLEAR_COUNTERS_INTERFACE
}SsmQosClearCounterOpcode_t;		

typedef struct QosRcvQueueMulticastMsg_ {
  	int rateLimit;
	int burst;
	int thresholdTrafficClass0;
	int thresholdTrafficClass1;
	int thresholdTrafficClass2;
	int thresholdTrafficClass3;
	int thresholdTrafficClass4;
	int thresholdTrafficClass5;
	int thresholdTrafficClass6;
	int thresholdTrafficClass7;
	int opCode;
} QosRcvQueueMulticastMsg_t;

typedef struct QosMcastSchedulerMsg_ {
  	int dwrrTrafficClass0;
  	int dwrrTrafficClass1;
  	int dwrrTrafficClass2;
  	int dwrrTrafficClass3;
  	int dwrrTrafficClass4;
  	int dwrrTrafficClass5;
  	int dwrrTrafficClass6;
  	int dwrrTrafficClass7;
} QosMcastSchedulerMsg_t;

typedef struct QosSchedulerMsg_ {
	int strict_priority;
  	int dwrrTrafficClass0;
  	int dwrrTrafficClass1;
  	int dwrrTrafficClass2;
  	int dwrrTrafficClass3;
  	int dwrrTrafficClass4;
  	int dwrrTrafficClass5;
  	int dwrrTrafficClass6;
  	int dwrrTrafficClass7;
} QosSchedulerMsg_t;

typedef enum QosInterfaceConfigCmdcode_ {
	CONFIGURE_INTERFACE_QOS = 0,
	DELETE_INTERFACE_QOS
} QosInterfaceConfigCmdcode_t;

typedef enum QosInterfaceConfigOpcode_ {
	QOS_INTERFACE_DEFAULT_COS = 0,
	QOS_INTERFACE_LONG_DISTANCE,
	QOS_INTERFACE_COS_THRESHOLD,
	QOS_INTERFACE_ANY_MODE_CMD_MAX = QOS_INTERFACE_COS_THRESHOLD, 
	QOS_INTERFACE_TRUST_COS,
	QOS_INTERFACE_FLOW_CONTROL_TX,
	QOS_INTERFACE_FLOW_CONTROL_RX,
	QOS_INTERFACE_FLOW_CONTROL_BOTH,
	QOS_INTERFACE_COS_MUTATION,
	QOS_INTERFACE_COS_TRAFFIC_CLASS,
	QOS_INTERFACE_DSCP_MUTATION,
	QOS_INTERFACE_DSCP_TRAFFIC_CLASS,
	QOS_INTERFACE_DSCP_COS,
	QOS_INERFACE_PFC_FLOW_CONTROL_COS,
	QOS_INTERFACE_PFC_FLOW_CONTROL_TX,
	QOS_INTERFACE_PFC_FLOW_CONTROL_RX,
	QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH,
    QOS_INTERFACE_PRIORITY_TAG,
	QOS_INTERFACE_TRUST_DSCP,
	QOS_INTERFACE_COS_PROFILE,
	QOS_INTERFACE_TAIL_DROP,
	QOS_INTERFACE_SCH_CFG,
	QOS_INTERFACE_MLS_CMD_MAX = QOS_INTERFACE_SCH_CFG,
	QOS_INTERFACE_CEE_MAP,
	QOS_INTERFACE_CEE_CMD_MAX = QOS_INTERFACE_CEE_MAP,
	QOS_INTERFACE_SERVICE_POLICY,
	QOS_INTERFACE_MQC_CMD_MAX,
	QOS_INTERFACE_CMD_MAX
}QosInterfaceConfigOpcode_t;

typedef enum QosShowCmdOpCode_s {
	QOSSHOWQOSRCVQUEUEINTERFACE,
	QOSSHOWQOSQUEUEINTERFACE,
	QOSSHOWQOSFLOWCONTROLINTERFACE,
	QOSSHOWQOSINTERFACE,
	QOSSHOWQOSRCVQUEUEMULTICASTINTERFACE,
	QOSSHOWQOSMAPS,
	QOSGETSHOWQOSMAPS,
	QOSSHOWCOSMUTATIONMAP,
	QOSSHOWCOSTRAFFICCLASSMAP,
	QOSSHOWDSCPMUTATIONMAP,
	QOSSHOWDSCPTRAFFICCLASSMAP,
	QOSSHOWDSCPCOSMAP,
	QOSGETSHOWQOSREDPROFILES,
	QOSSHOWQOSREDPROFILES,
	QOSSHOWQOSREDSTATS,
	QOSSHOW_QOS_INGQUEUE_INTERFACE,
	QOSSHOW_QOS_INGBUFF_INTERFACE,
	QOSSHOW_QOS_EGRQUEUE_INTERFACE,
	QOSSHOW_QOS_EGRBUFF_INTERFACE,
	QOSSHOW_QOS_PLC_INGQUEUE_INTERFACE,
	QOSSHOW_QOS_PLC_INGBUFF_INTERFACE,
	QOSSHOW_QOS_PLC_EGRQUEUE_INTERFACE,
	QOSSHOW_QOS_PLC_EGRBUFF_INTERFACE,
    QOSSHOW_QOS_CHIP_INFO,
    QOSSHOW_QOS_TMC_INGQUEUE_INTERFACE,
    QOSSHOW_QOS_TMC_INGBUFF_INTERFACE,
    QOSSHOW_QOS_TMC_EGRQUEUE_INTERFACE,
    QOSHSOW_QOS_TMC_EGRBUFF_INTERFACE,
    QOSSHOW_QOS_ING_TMCBUFF_INTERFACE,
    QOSSHOW_QOS_ING_TMCQUEUE_INTERFACE,
    QOSSHOW_QOS_EGR_TMCBUFF_INTERFACE,
    QOSSHOW_QOS_VOQ_INTERFACE,
    QOSSHOW_DROP_REASON_INTERFACE,
	QOSSHOWCMDMAX //last one
}QosShowCmdOpCode_t;

typedef struct qos_interface_config_msg_ {
	int cmd_code;
	int op_code;
	int default_cos;
	unsigned char flowcontrol_tx;
	unsigned char flowcontrol_rx;
	char ifname[50];
	char cos_mutation_map[MAP_NAME_MAX_STR];
	char cos_traffic_class_map[MAP_NAME_MAX_STR];
	char dscp_mutation_map[MAP_NAME_MAX_STR];
	char dscp_traffic_class_map[MAP_NAME_MAX_STR];
    char dscp_cos_map[MAP_NAME_MAX_STR];
	char cee_map[MAP_NAME_MAX_STR];
	unsigned char pfc_flowcontrol_tx;
	unsigned char pfc_flowcontrol_rx;
	int pfc_cos;
	int cos_threshold[8];
	unsigned int distance;
	int cos;
	int red_profile;
	int tail_drop;
	int strict_priority;
	int dwrrTrafficClass0;
    int dwrrTrafficClass1;
	int dwrrTrafficClass2;
	int dwrrTrafficClass3;
	int dwrrTrafficClass4;
	int dwrrTrafficClass5;
	int dwrrTrafficClass6;
	int dwrrTrafficClass7;
	char policy_name[MAP_NAME_MAX_STR];
	int  dir;
} qos_interface_config_msg_t;

typedef struct qos_clear_counters_msg_ {
	int cmd_code; 
	char if_name[MAX_STR + 1];
	char if_type_str[MAX_STR + 1];
	int if_type;
} qos_clear_counters_msg_t;

typedef enum
{
	QOSSHOW_QOS_RCVQUEUE_INTERFACE_ALL = 0 ,
	QOSSHOW_QOS_RCVQUEUE_INTERFACE ,
	QOSSHOW_QOS_RCVQUEUE_PORTCHANNEL

#if 0
	// Command codes to support Wolverine QOS Statistics Show Commands -- BEGIN
	QOSSHOW_QOS_INGQUEUE_INTERFACE,
	QOSSHOW_QOS_INGBUFF_INTERFACE,
	QOSSHOW_QOS_EGRQUEUE_INTERFACE,
	QOSSHOW_QOS_EGRBUFF_INTERFACE,
	QOSSHOW_QOS_PLC_INGQUEUE_INTERFACE,
	QOSSHOW_QOS_PLC_INGBUFF_INTERFACE,
	QOSSHOW_QOS_PLC_EGRQUEUE_INTERFACE,
	QOSSHOW_QOS_PLC_EGRBUFF_INTERFACE,
	// Command codes to support Wolverine QOS Statistics Show Commands -- END

	//Command codes to support Hawk QOS Statistics Show Commands -- BEGIN
	QOSSHOW_QOS_CHIP_INFO = 100,

	QOSSHOW_QOS_TMC_INGQUEUE_INTERFACE,
	QOSSHOW_QOS_TMC_INGBUFF_INTERFACE,
	QOSSHOW_QOS_TMC_EGRQUEUE_INTERFACE,
	QOSHSOW_QOS_TMC_EGRBUFF_INTERFACE,

	QOSSHOW_QOS_ING_TMCBUFF_INTERFACE,
	QOSSHOW_QOS_ING_TMCQUEUE_INTERFACE,
	QOSSHOW_QOS_EGR_TMCBUFF_INTERFACE,

	QOSSHOW_QOS_VOQ_INTERFACE,

	QOSSHOW_DROP_REASON_INTERFACE
#endif

	//Command codes to support Hawk QOS Statistics Show Commands -- END

} QosShowQosCmdCode;

typedef struct QosChipPort_Index_ {
	int chip_index;
	int port_index;
}QosChipPort_Index_t;

typedef struct QosShow_InputMsg_ {
	char if_name[MAX_STR+1];
	char po_num[MAX_STR+1];
	int slot_num;
	int chip_num;
	unsigned int cmdCode;
	union {
		QosChipPort_Index_t qosInterfaceIndex;
		QosChipPort_Index_t qosFlowControlIndex;
		QosChipPort_Index_t qosRcvQueueIndex;
		QosChipPort_Index_t qosQueueindex;
		QosChipPort_Index_t qosRedStatsIndex;
	}indices;
	int16_t  no_of_records;
	int requestType;
} QosShow_InputMsg_t;
			

typedef struct QosRcvQueueInterfaceMsg_ {
	int inUse;
	int totalBuffer;
	int cosOrTc;
	int cosInuse[8];
	int cosMax[8];
	char ifLongName[MAX_STR+1];
	char packetsDropped[STR32];
	char lagName[MAX_STR+1];
} QosRcvQueueInterfaceMsg_t;

typedef struct QosQueueInterfaceMsg_ {
	char ifLongName[MAX_STR+1];
	char cosRxPackets[8][STR32+1];
	char cosRxBytes[8][STR32+1];
	char cosTxPackets[8][STR32+1];
	char cosTxBytes[8][STR32+1];
	char untag[2][STR32+1];
	char tx_untag[2][STR32+1];
	char lagName[MAX_STR+1];
	swbd_platform_t swbd; /*Platform Mercury or Callisto */
} QosQueueInterfaceMsg_t;

#define QOS_RED_STATS_PKT	0
#define QOS_RED_STATS_BYT	1

typedef struct QosRedIntfPriStatsMsg_ {
	uint8_t	pri;
	int profileId;
	uint64_t green_drop[2];
	uint64_t yellow_drop[2];
	uint64_t red_drop[2];
	uint64_t que_full_drop[2];
} QosRedIntfPriStatsMsg_t;

typedef struct QosRedIntfStatsMsg_ {
	uint8_t isResultValid;
	char ifShortName[STR32];
	uint8_t num_pris;
	QosRedIntfPriStatsMsg_t priStats[8];
	QosRedIntfPriStatsMsg_t portStats;
	swbd_platform_t swbd; /*Platform Mercury or Castor */
} QosRedIntfStatsMsg_t;

typedef struct QosStatsHdr_ {
	char ifShortName[STR32];
	int slot, chip;
} QosStatsHdr_t;

typedef struct QosPlcIngrBufStats_ {
	QosStatsHdr_t hdr;
	uint32_t bpPortBuffers;
	uint32_t bpPortBufferThreshold;
	uint32_t globalSharedBuffers;
	uint32_t globalSharedBufferHiThreshold;
	uint32_t globalSharedBufferLoThreshold;
	uint32_t globalBusyBuffers;
	uint32_t globalBpQueDepth[8];
	uint32_t globalBpQueDepthThreshold[8];
	uint32_t spanQueDepth[8];
	uint32_t spanQueDepthThreshold[8];
	uint32_t sflowMultiReasonQueDepth;
	uint32_t sflowMultiReasonQueDepthThreshold;
	uint8_t globalBpQueDepthLog[8];
	uint8_t spanQueFull[8];
	uint8_t sflowQueFull;
	uint8_t multiSpanQueFull;
} QosPlcIngrBufStats_t;

typedef struct QosPortIngrBufStats_ {
	QosStatsHdr_t hdr;
	uint8_t bpDropLog[8];
	uint8_t tailDropLog[8];
	uint32_t portBusyBuffers;
	uint32_t portBusyBufferThreshold;
	uint32_t portPauseHiThreshold;
	uint32_t portPauseLoThreshold;
	uint32_t portPriBusyBuffers[8];
	uint32_t portPfcHiThreshold[8];
	uint32_t portPfcLoThreshold[8];
	uint32_t portTailDropThreshold[8];
	uint32_t portBpQueDepth[8];
	uint32_t portBpQueDepthThreshold[8];
	uint8_t portQpriWpriMap[8];
	uint8_t portPfcThresholdEn[8];
	uint8_t portPauseThresholdEn;
} QosPortIngrBufStats_t;

typedef struct QosPlcIngrQueStats_ {
	QosStatsHdr_t hdr;
	uint32_t spanQueThreshold[8];
	uint32_t spanQueDepth[8];
	uint8_t  bpDiscardQueStatus;
	uint8_t  bpQueueStatus[18];
	uint32_t bpDequeuePkts;
	uint32_t bpTimeoutPkts;
	uint32_t sflowQueDepth;
	uint32_t sflowQueThreshold;
	uint32_t multiBpReasonQueDepth;
	uint32_t multiBpReasonQueThreshold;
} QosPlcIngrQueStats_t;

typedef struct QosPortIngrQueStats_ {
	QosStatsHdr_t hdr;
	uint64_t numPktsDequeued[8];
	uint64_t numPktsDropped;
	uint8_t  portCreditAvailable;
	uint8_t  pauseStatus[8];
	uint8_t  portPriCreditAvailable[8];
	uint8_t  portPriQueueStatus[8];
} QosPortIngrQueStats_t;

typedef struct QosPlcEgrBufStats_ {
	QosStatsHdr_t hdr;
	uint32_t globalQueDepth;
	uint32_t globalQueDepthThreshold;
	uint32_t globalBpQueDepth;
	uint32_t globalBpQueDepthThreshold;
	uint32_t globalBpQuePrioDepth[8];
	uint32_t globalBpQuePrioDepthHiThreshold[8];
	uint32_t globalBpQuePrioDepthLoThreshold[8];
	uint8_t globalBpQueTmcBackPressure[8];
} QosPlcEgrBufStats_t;

typedef struct QosPortEgrBufStats_ {
	QosStatsHdr_t hdr;
	uint32_t portPrioQueDepth[8];
	uint32_t portPrioQueHiThreshold[8];
	uint32_t portPrioQueLoThreshold[8];
	uint8_t portPrioBufSharing[8];
	uint8_t portPrioQueBackPressure[8];
} QosPortEgrBufStats_t;

typedef struct QosPlcEgrQueStats_ {
	QosStatsHdr_t hdr;
	uint32_t bpUnicastCurrentRateShapeWt[8];
	uint32_t bpUnicastRateShapeMax[8];
	uint32_t bpUnicastRateShapeWt[8];
	uint32_t bpMulticastCurrentRateShapeWt[8];
	uint32_t bpMulticastRateShapeMax[8];
	uint32_t bpMulticastRateShapeWt[8];
} QosPlcEgrQueStats_t;

typedef struct QosPortEgrQueStats_ {
	QosStatsHdr_t hdr;
	uint8_t rateType[8];
	uint32_t rateShapePeriod;
	uint8_t portDequeEnabled;
	uint32_t portRateShapeMax;
	uint32_t portRateShapeWt;
	uint8_t strictPrioBndry;
	uint8_t portPrioToDrrGrpMap[8];
	uint32_t portDrrGrpWt[8];
	uint32_t portDrrPrioWt[8];
} QosPortEgrQueStats_t;


typedef struct QosPortTmcIngQueConfig_ {
	uint8_t drrMode;
	uint8_t drrPrioAllow;
	uint8_t drrCrdtAllow;
}QosPortTmcIngQueConfig_t;

typedef struct QosPortTmcIngQueStats_ {
	QosStatsHdr_t hdr;
	QosPortTmcIngQueConfig_t portQueConfig;
	uint32_t drrGroup;
	uint16_t flowCredit[8];
}QosPortTmcIngQueStats_t;

typedef struct QosTmcIngQueConfig_ {
	uint8_t mcastRateLimitEnabled;
	uint8_t tmoutLoslessDetectEn;
	uint8_t drrEn;
	uint8_t flushEn;
	uint8_t tmoutScanEn;
	uint8_t fwdEn;
	uint32_t speedupGroup0;
	uint32_t speedupGroup1;
}QosTmcIngQueConfig_t;

typedef struct QosTmcIngQueStats_ {
	QosStatsHdr_t hdr;
	QosTmcIngQueConfig_t queConfig;
	uint16_t chipFlowCredit;
	uint16_t chipFlowCreditPool;
}QosTmcIngQueStats_t;

typedef struct QosTmcEgrQueStats_ {
	QosStatsHdr_t hdr;
	uint32_t discardCount;
	uint32_t eccErrCount;
	uint32_t mcastLinkMemEccAddrErr;
	uint32_t mcastDescEccAddrErr;
	uint32_t mcastQueRatebaseErr;
	uint32_t rateBaseErr;
	uint32_t mcastQueRateCountErr;
	uint32_t rateCountErr;
	uint32_t groupRestoreCrdtBaseAddrErr;
	uint32_t queRestoreCrdtBaseAddrErr;
	uint32_t egidAddrErr;
	uint32_t gpDrrAddrErr;
	uint32_t queDrrAddrErr;
	uint32_t mcastTailEccAddrErr;
	uint32_t ucastTailEccAddrErr;
	uint32_t mcastHeadEccAddrErr;
	uint32_t ucastHeadEccAddrErr;
}QosTmcEgrQueStats_t;

typedef struct QosTmcEgrBufStats_ {
	QosStatsHdr_t hdr;
	uint8_t redEnable[8];
	uint32_t redTj0Red[16];
	uint32_t redTj0Yellow[16];
	uint32_t redTj0Green[16];
	uint32_t redDeltajRed[16];
	uint32_t redDeltajYellow[16];
	uint32_t redDeltajGreen[16];
	uint32_t redAlphaRed[16];
	uint32_t redAlphaYellow[16];
	uint32_t redAlphaGreen[16];
	uint16_t creditCounter;
	uint64_t rcvMulticastPktsVoq[8];
	uint64_t rcvMulticastBytesVoq[8];
	uint64_t xmitMulticastPktsVoq[8];
	uint64_t xmitMulticastBytesVoq[8];
	uint32_t runtDropCount;
	uint32_t ccbDropCount;
	uint32_t badShimCrcDropCount;
	uint32_t oobDropCount;
	uint32_t oobTruncCount;
	uint32_t totalDropCount;
	uint32_t rxfFifoParityErrorCount;
	uint32_t shimParityErrorDropCount;
	uint32_t rxfBidPrefetchFailCount;
	uint32_t txfOverszTruncCount;
	uint32_t txfEccErrstagFifoCount;
	uint32_t txfEccErrPayloadCount;
	uint32_t txfEccErrTruncCount;
}QosTmcEgrBufStats_t;

typedef struct QosPortTmcEgrBufStats_ {
	QosStatsHdr_t hdr;
	uint32_t redProfileMap;
	uint32_t voqDepth[8];
	uint64_t dropRedPkts[8];
	uint64_t dropYellowPkts[8];
	uint64_t dropGreenPkts[8];
	uint64_t dropRedBytes[8];
	uint64_t dropYellowBytes[8];
	uint64_t dropGreenBytes[8];
	uint64_t queFullDropPkts[8];
	uint64_t queFullDropBytes[8];
	uint64_t enquePktsVoq[8];
	uint64_t enqueBytesVoq[8];
	uint64_t dequePktsVoq[8];
	uint64_t dequeBytesVoq[8];
}QosPortTmcEgrBufStats_t;

typedef struct QosPortTmcIngBufStats_ {
	QosStatsHdr_t hdr;
	uint8_t portPrioType;
	uint8_t mcPortPrioType;
	uint32_t losslessSharedBuffer;
	uint32_t inputBufferCount[8];
	uint32_t multicastInputBufferCount[8];
	uint32_t multicastSharedBuffer;
	uint32_t bpInputBufferCount[8];
	uint32_t inputTailDropThreshold[8];
	uint32_t multicastTailDropThreshold[8];
	uint32_t bpTailDropThreshold[8];
	uint8_t pfcStatus;
	uint64_t rcvPkts;
	uint64_t rcvBytes;
	uint64_t dropPkts;
	uint64_t dropBytes;
}QosPortTmcIngBufStats_t;

typedef struct QosPortVoqStats_t {
	QosStatsHdr_t hdr;
	uint64_t dropRedPkts;
	uint64_t dropYellowPkts;
	uint64_t dropGreenPkts;
	uint64_t dropRedBytes;
	uint64_t dropYellowBytes;
	uint64_t dropGreenBytes;
	uint64_t queFullDropPkts;
	uint64_t queFullDropBytes;
	uint64_t enquePktsVoq;
	uint64_t enqueBytesVoq;
	uint64_t dequePktsVoq;
	uint64_t dequeBytesVoq;
	uint64_t ecnMarkedPackets;
	uint64_t dropToFlushedPackets;
	uint8_t  tmcChipId;
} QosPortVoqStats_t;

#define QOS_MAX_RSN_STR_LEN 45
#define MAX_DROP_REASON_STATS 128
#define MAX_PORTS_PER_CHIP 24

typedef struct QosChipPortInfo_ {
	uint32_t chipIfindex;
	uint8_t	 chipId;
	uint8_t  tmcChipId;
	uint8_t  numPorts;
	char 	 ports[MAX_PORTS_PER_CHIP][STR32];
} QosChipPortInfo_t;

typedef struct QosPortDropReasonEntry_ {
	char reason[QOS_MAX_RSN_STR_LEN];
	uint64_t dropCount;
}QosPortDropReasonEntry_t;

typedef struct QosPortDropReasonStats_ {
	QosStatsHdr_t hdr;
	int num_entries;
	QosPortDropReasonEntry_t dropEntries[MAX_DROP_REASON_STATS];
}QosPortDropReasonStats_t;

typedef struct QosTmcIngBufStats_ {
	QosStatsHdr_t hdr;
	uint32_t lossySharedBuf;
	uint32_t lossyMcSharedBuf;
	uint32_t bpSharedBuf;
	uint64_t rcvMulticastPkts[8];
	uint64_t rcvMulticastBytes[8];
	uint64_t dropMulticastPkts[8];
	uint64_t dropMulticastBytes[8];
	uint32_t bufferedCreditMem[2];
}QosTmcIngBufStats_t;

typedef struct QosFlowcontrolInterfaceMsg_ {
	int mode;
	int pfcNegotiationFlag;
	int txAdmin[8];
	int txOper[8];
	int rxAdmin[8];
	int rxOper[8];
	char ifLongName[MAX_STR+1];
	char txFrames[STR32+1];
    char rxFrames[STR32+1];
	char outputPaused[8][STR32+1];
	char lagName[MAX_STR+1];
    swbd_platform_t swbd; /* Platform Mercury or Callisto */
} QosFlowcontrolInterfaceMsg_t;

typedef struct QosInterfaceMsg_ {
	char ifLongName[MAX_STR+1];
	char provisionMode[8];
    int priorityTag;
	int ssmQosSqpfCeeSet;
	int SsmQosSqpfFcoeCosSet;
	int SsmQosSqpfFcoeProvisionSet;
	char sqpCeeMap[MAP_NAME_MAX_STR];
	int sqpFcoeCos;
	int defaultCos;
	char ssmQosTrustName[14];
	char ssmQosDscpTrustName[14];
	int SsmQosSqpfCosMutationSet;
	int SsmQosSqpfDscpMutationSet;
	int SsmQosSqpfDscpTcSet;
	int SsmQosSqpfDscpCosSet;
	char sqpCosMutationMap[MAP_NAME_MAX_STR];
	char sqpCosTcMap[MAP_NAME_MAX_STR];
	char sqpDscpMutationMap[MAP_NAME_MAX_STR];
	char sqpDscpTcMap[MAP_NAME_MAX_STR];
	char sqpDscpCosMap[MAP_NAME_MAX_STR];
	int mappedCos[8];
	int mappedTc[8];
	int mappedDscp[64];
	int mappedDscpTc[64];
	int mappedDscpCos[64];
	int tailDropThreshold;
	int SsmQosSqpfPauseSet;
	int cosTailDropThreshold[8];
	int flowControlFlag;
	int txEnable[8];
	int rxEnable[8];
	int mcRateLimit;
	int mcBurstLimit;
	int spCount;
	int dwrrWeight[8];
	int distance;
#ifdef QOS_EANVIL
	int mcTailDrop[8];
	int mcDwrrWeight[8];
#else
	int mcTailDrop[4];
	int mcDwrrWeight[4];
#endif
	char lagName[MAX_STR+1];
	int profileId[8];
	int redCos[8];
	swbd_platform_t swbd; /*Platform Mercury or Callisto */
} QosInterfaceMsg_t;

typedef struct QosRcvQueueMulticastInterfaceMsg_ {
	char ifLongName[MAX_STR+1];
	int printTableHeading;
	int qosEanvil;
	int slotIndex;
	int slotChipIndex;
	char portSetStr[8];
	char Tc[8][STR32+1];
	swbd_platform_t swbd;
} QosRcvQueueMulticastInterfaceMsg_t;

typedef struct QosMapsMsg_ {
	char ifShortName[STR32];
} QosMapsMsg_t;

typedef struct QosMapNamesMsg_ {
	char map[MAP_NAME_MAX_STR];
	int mapType;
	uint8_t outboundCos[8];
	uint8_t outboundDscp[64];
} QosMapNamesMsg_t;

typedef struct QosRedProfileIdsMsg_ {
	int profileId;
	int minThreshold;
	int maxThreshold;
	int dropProbability;
}QosRedProfileIdsMsg_t;

typedef struct QosRedProfileMsg_ {
	char ifShortName[STR32];
	unsigned int cos[8];
} QosRedProfileMsg_t;

typedef struct QosPaginationInfo_ {
	char  isMore; 
	union {
		QosChipPort_Index_t lastqosInterfaceIndex;
 		QosChipPort_Index_t lastqosFlowControlIndex;
 		QosChipPort_Index_t lastqosRcvQueueIndex;
 		QosChipPort_Index_t lastqosQueueindex;
 		QosChipPort_Index_t lastqosRedStatsIndex;
 	}indices;
}QosPaginationInfo_t;
 
typedef struct QosShowMsg_ {
	unsigned int num_records;
	unsigned int record_type;
	QosPaginationInfo_t PaginationInfo;
	union {
		QosRcvQueueInterfaceMsg_t rcvQueueInterface[1];
		QosQueueInterfaceMsg_t queueInterface[1];
		QosFlowcontrolInterfaceMsg_t flowcontrolInterface[1];
		QosInterfaceMsg_t interface[1];
		QosRcvQueueMulticastInterfaceMsg_t rcvQueueMulticastInterface[1];
		QosMapsMsg_t maps[1];
		QosMapNamesMsg_t mapName[1];
		QosRedProfileIdsMsg_t profileIds[1];
		QosRedProfileMsg_t profileIdIntf[1];
		QosRedIntfStatsMsg_t redIntfStats[1];

		//QOS Interface Statistics Commands : BEGIN
		QosPlcIngrBufStats_t plcIngrBuffStats[1];
		QosPortIngrBufStats_t portIngBuffStats[1];
		QosPlcIngrQueStats_t plcIngrQueueStats[1];
		QosPortIngrQueStats_t portIngrQueueStats[1];
		QosPlcEgrBufStats_t plcEgrBuffStats[1];
		QosPortEgrBufStats_t portEgrBuffStats[1];
		QosPlcEgrQueStats_t plcEgrQueueStats[1];
		QosPortEgrQueStats_t portEgrQueueStats[1];
		//QOS Interface Statistics Commands : END

		//Hawk QOS Statistics Commands : BEGIN
		QosChipPortInfo_t chipInfo[1];
		QosTmcIngQueStats_t tmcIngQueStats[1];
		QosTmcIngBufStats_t tmcIngBufStats[1];
		QosTmcEgrQueStats_t tmcEgrQueStats[1];
		QosTmcEgrBufStats_t tmcEgrBufStats[1];

		QosPortTmcIngBufStats_t portTmcIngBufStats[1];
		QosPortTmcIngQueStats_t portTmcIngQueStats[1];
		QosPortTmcEgrBufStats_t portTmcEgrBufStats[1];

		QosPortVoqStats_t portVoqStats[1];
		QosPortDropReasonStats_t dropReasonStats[1];
		//Hawk QOS Statistics Commands : END
	} records;
} QosShowMsg_t;

typedef union QosBufferNum_ { 
    struct { 
        u_int16_t   mappedId;
        //This is typically QosShowBuf_t for show commands
        u_int16_t   tag;     
    } buff;
    u_int32_t   num;   
} QosBufferNum_t;

typedef struct QosDscpMarkListMsg_ {
	char dscpMapName[MAP_NAME_MAX_STR];
	int toValue;
	uint8_t qosMapType;
	uint32_t pValues[64];
	uint32_t nValues;
} QosDscpMarkListMsg_t;

typedef struct QosDscpCosMapMsg_ {
	char dscpCosMapName[MAP_NAME_MAX_STR];
	int configReplay;
} QosDscpCosMapMsg_t;

typedef struct QosDscpTrafficClassMapMsg_ {
	char dscpTrafficClassMapName[MAP_NAME_MAX_STR];
	int configReplay;
} QosDscpTrafficClassMapMsg_t;

typedef struct QosDscpMutationMapMsg_ {
	char dscpMutationMapName[MAP_NAME_MAX_STR];
	int configReplay;
} QosDscpMutationMapMsg_t;



static inline u_int16_t QosGetBufferTag (u_int32_t buffNum) 
{
    return (((QosBufferNum_t*) &buffNum)->buff.tag);
}

static inline u_int16_t QosGetBufferMappedId (u_int32_t buffNum) 
{
    return (((QosBufferNum_t*) &buffNum)->buff.mappedId);
}

static inline u_int32_t QosGetBufferNum (u_int32_t buffNum) 
{
    return (((QosBufferNum_t*) &buffNum)->num);
}

#endif // __QOSCOMMON_H
