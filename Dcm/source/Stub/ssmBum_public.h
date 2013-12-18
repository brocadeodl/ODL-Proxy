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
 * @file ssmAcl.h
 * @brief Header file containing definitions exposed to all
 * @author Alan Bolton
 * @date 11/22/2011
 *
 */
#ifndef _SSMBUM_PUBLIC_H_
#define _SSMBUM_PUBLIC_H_
/*
* Included Files
*/

/*
* Typedef, enumeration, structure definitions
*/

/*
 * This define is the number of protocols listed in enum ssmBum_protocol and is
 * use for loop controls within BUM files. It is exposed here so that it can be
 * maintained with the enumeration list
 */
#define SSMBUM_NUM_PROTOCOLS_SUPPORTED 3

/**
 * Enumeration to define the BUM protocol types that can be limited
 * This enumeration is used to decide which protocol is being configured.
 */
enum ssmBum_protocol
{
   SSMBUM_PROTOCOL_UNKNOWN_E = 0,
   SSMBUM_PROTOCOL_MIN_E,
   SSMBUM_PROTOCOL_BCAST_E = SSMBUM_PROTOCOL_MIN_E, //broadcast
   SSMBUM_PROTOCOL_MCAST_E, //multicast
   SSMBUM_PROTOCOL_UUCAST_E,//unknown unicast
   SSMBUM_PROTOCOL_MAX_E
};

/**
 * Enumeration to define the action that is to be used on the BUM configuration
 *
 * When BUM storm control is enabled rate limiting will always be performed
 * (the default action). The user can "monitor" which results in a log message
 * being generated when the configured rate is exceeded. The "shutdown" action
 * results in both a log message being generated and the interface being placed
 * in the administrative "down"state when the configured rate has been exceeded.
 */
enum ssmBum_action
{
   SSMBUM_ACTION_UNKNOWN_E = 0,
   SSMBUM_ACTION_LIMIT_ONLY_E,
   SSMBUM_ACTION_MONTIOR_E, // limit and monitor for violations
   SSMBUM_ACTION_SHUTDOWN_E // limit and shutdown when violation is detected
};

/**
 * Enumeration to define the format of the configured rate
 *
 * This type indicates on the format of the user specified rate as either being
 * in a Kilo Bits Per Second or as a precentage of the interfaces bandwidth.
 */
enum ssmBum_rate_format
{
   SSMBUM_RATE_FORMAT_UNKNOWN_E = 0,
   SSMBUM_RATE_FORMAT_BPS_E,
   SSMBUM_RATE_FORMAT_PRECENTAGE_E //percentage of interface bandwidth
};

/**
 * Data Type the configuration of BUM for a certain Protocol on a particular IF
 *
 * The provisioned rate will always be stored in KiloBits per second regardless
 * of how the user entered the data (in KBPS or Precentage). The backend logic
 * converts the specified user input to KBPS.
 */
struct ssmBum_record
{
   unsigned int ifindex;
   enum ssmBum_protocol protocol;
   enum ssmBum_action action;
   enum ssmBum_rate_format user_format;
   unsigned long long provisioned_rate; //detrmined by user_format
};

/**
 * Data Type containing the statistical information for a given Protocol type.
 *
 */
struct ssmBum_counts
{
   unsigned long long conforming;
   unsigned long long violations;
};

/**
 * Data Type representing both configuration and status information.
 *
 */
struct ssmBum_protocol_record
{
   struct ssmBum_record config;
   struct ssmBum_counts status;
};

/**
 * Data Type used in reporting status information from backend to frontend.
 *
 * num_portocols specifies the number of protocols that are contained in the
 * response.
 */
struct ssmBum_intf_record
{
   char if_name[32];
   unsigned char num_protocols;
   struct ssmBum_protocol_record protocol[SSMBUM_NUM_PROTOCOLS_SUPPORTED];
};

/**
 * Data Type used in reporting status information from backend to frontend.
 *
 * num_intf specifies the number of records that are contained in the response
 */
struct ssmBum_status_resp_data
{
   unsigned short num_intf;
   struct ssmBum_intf_record intf[1];
};

/*
* Exported Functions
*/

#endif /*_SSMBUM_PUBLIC_H_*/
