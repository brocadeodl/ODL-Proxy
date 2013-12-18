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

/*
 * Copyright (c) 2009 by Brocade Communications Systems, Inc.
 * @file qos_bum_dcm_common.h
 * @brief Header file containing definitions exposed to all
 * @date 11/22/2011
 *
 */
#ifndef _QOS_BUM_DCM_COMMON_H_
#define _QOS_BUM_DCM_COMMON_H_

/*
* Typedef, enumeration, structure definitions
*/

/*
 * This define is the number of protocols listed in enum qosBum_protocol and is
 * use for loop controls within BUM files. It is exposed here so that it can be
 * maintained with the enumeration list
 */
#define QOS_BUM_NUM_PROTOCOLS_SUPPORTED 3
#define QOS_BUM_INVALID_LOCATION_ID	0

/**
 * Enumeration to define the BUM protocol types that can be limited
 * This enumeration is used to decide which protocol is being configured.
 */
typedef enum qosBumProtocol_e
{
   QOS_BUM_PROTOCOL_UNKNOWN_E = 0,
   QOS_BUM_PROTOCOL_MIN_E,
   QOS_BUM_PROTOCOL_BCAST_E = QOS_BUM_PROTOCOL_MIN_E, //broadcast
   QOS_BUM_PROTOCOL_MCAST_E, //multicast
   QOS_BUM_PROTOCOL_UUCAST_E,//unknown unicast
   QOS_BUM_PROTOCOL_MAX_E
} qosBumProtocol_t;

/**
 * Enumeration to define the action that is to be used on the BUM configuration
 *
 * When BUM storm control is enabled rate limiting will always be performed
 * (the default action). The user can "monitor" which results in a log message
 * being generated when the configured rate is exceeded. The "shutdown" action
 * results in both a log message being generated and the interface being placed
 * in the administrative "down"state when the configured rate has been exceeded.
 */
typedef enum qosBumAction_e
{
   QOS_BUM_ACTION_UNKNOWN_E = 0,
   QOS_BUM_ACTION_LIMIT_ONLY_E,
   QOS_BUM_ACTION_MONTIOR_E, // limit and monitor for violations
   QOS_BUM_ACTION_SHUTDOWN_E // limit and shutdown when violation is detected
} qosBumAction_t;

/**
 * Enumeration to define the format of the configured rate
 *
 * This type indicates on the format of the user specified rate as either being
 * in a Kilo Bits Per Second or as a precentage of the interfaces bandwidth.
 */
typedef enum qosBumRateFormat_e
{
   QOS_BUM_RATE_FORMAT_UNKNOWN_E = 0,
   QOS_BUM_RATE_FORMAT_BPS_E,
   QOS_BUM_RATE_FORMAT_PRECENTAGE_E //percentage of interface bandwidth
} qosBumRateFormat_t;

/**
 * Data Type the configuration of BUM for a certain Protocol on a particular IF
 *
 * The provisioned rate will always be stored in KiloBits per second regardless
 * of how the user entered the data (in KBPS or Precentage). The backend logic
 * converts the specified user input to KBPS.
 */
typedef struct qosBumRecord_s
{
   unsigned int ifindex;
   qosBumProtocol_t protocol;
   qosBumAction_t action;
   qosBumRateFormat_t user_format;
   unsigned long long provisioned_rate; //detrmined by user_format
} qosBumRecord_t;

/**
 * Data Type containing the statistical information for a given Protocol type.
 *
 */
typedef struct qosBumStats_s
{
   unsigned long long conforming;
   unsigned long long violations;
} qosBumStats_t;

/**
 * Data Type representing both configuration and status information.
 *
 */
typedef struct qosBumProtocolStatsRecord_s
{
   qosBumRecord_t config;
   qosBumStats_t status;
} qosBumProtocolStatsRecord_t;

/**
 * Data Type used in reporting status information from backend to frontend.
 *
 * num_portocols specifies the number of protocols that are contained in the
 * response.
 */
typedef struct qosBumIntfRecord_s
{
   char if_name[32];
   unsigned char num_protocols;
   qosBumProtocolStatsRecord_t protocol[QOS_BUM_NUM_PROTOCOLS_SUPPORTED];
} qosBumIntfRecord_t;

/**
 * Data Type used in reporting status information from backend to frontend.
 *
 * num_intf specifies the number of records that are contained in the response
 */
typedef struct qosBumStatusRespData_s
{
   unsigned short num_intf;
   qosBumIntfRecord_t intf[1];
} qosBumStatusRespData_t;


/* qos dcmd interaction messages */

/**
 * Enumeration of opcodes used by front-end cli to backend module
 */
typedef enum
{
   QOS_BUM_DCMD_ENABLE = 0,
   QOS_BUM_DCMD_DISABLE,
   QOS_BUM_DCMD_SHOW_SYSTEM_IF,
   QOS_BUM_DCMD_SHOW_SYSTEM_BY_PROTOCOL,
   QOS_BUM_DCMD_SHOW_IF,
   QOS_BUM_DCMD_SHOW_IF_BY_PROTOCOL,
   QOS_BUM_DCMD_CLEAR_SYSTEM,
   QOS_BUM_DCMD_CLEAR_SYSTEM_BY_PROTOCOL,
   QOS_BUM_DCMD_CLEAR_IF,
   QOS_BUM_DCMD_CLEAR_IF_BY_PROTOCOL
} qosBumDcmdOpcode_t;

/**
 * Data Type configuration message for enabling/disabling BUM storm control
 */
typedef struct qosBumDcmdConfigMsg_s
{
   qosBumDcmdOpcode_t opcode;
   char if_name[64]; //FIXME what #define do we use here?
   qosBumProtocol_t protocol;
   qosBumAction_t action;
   qosBumRateFormat_t rate_format;
   unsigned long long rate;
} qosBumDcmdConfigMsg_t;

/**
 * Data Type for requesting showing or clearing BUM storm control status
 */
typedef struct qosBumDcmdShowAndClearMSg_s
{
   qosBumDcmdOpcode_t opcode;
   char if_name[64]; //FIXME what #define do we use here?
   qosBumProtocol_t protocol;
} qosBumDcmdShowAndClearMSg_t;

/**
 * Data Type used by backend to report status info to the frontend CLI
 */
typedef struct qosBumDcmdStatusResp_s
{
   qosBumDcmdOpcode_t opcode; //!< Opcode is used to interpret resp buffer
   unsigned char more_elements;
   qosBumStatusRespData_t data;
} qosBumDcmdStatusResp_t;
#endif /*_QOS_BUM_DCM_COMMON_H_*/
