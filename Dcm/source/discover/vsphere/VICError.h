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
 * VICError.h
 *
 *  Created on: Apr 11, 2012
 *      Author: rmadapur
 */

#ifndef VICERROR_H_
#define VICERROR_H_


#define VIC_WSCLIENT_SUCCESS 0
#define VIC_WSCLIENT_CURL_ERROR -1001
#define VIC_WSCLIENT_CURL_NEGATIVE_RESPONSE_ERROR -1002
#define VIC_WSCLIENT_CURL_FAULT_RESPONSE_ERROR -1003
#define VIC_WSCLIENT_CURL_XML_PARSE_ERROR -1004
#define VIC_WSCLIENT_CURL_EMPTY_RESPONSE_ERROR -1005
#define VIC_WSCLIENT_CURL_CURLE_COULDNT_CONNECT -1006
#define VIC_WSCLIENT_CURL_CURLE_TIMEDOUT -1007

#endif /* VICERROR_H_ */
