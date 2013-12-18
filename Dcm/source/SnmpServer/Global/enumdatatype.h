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

#ifndef ENUMDATATYPE_H
#define ENUMDATATYPE_H

#include <iostream>

using namespace WaveNs;

namespace DcmNs
{
typedef enum {
	ro,
	rw
} Access;

typedef enum {
	v1,
	v2c
} Version;

typedef enum {
	traps,
	informs
} NotifyType;

typedef enum {
	snmpadmin,
	snmpuser
} GroupType;


typedef enum {
	md5,
    sha,
	AuthNone
} AuthType;

typedef enum {
    DES,
    PrivNone,
    AES128
} PrivType;

typedef enum {
	None,
	Debug,
	Info,
	Warning,
	Error,
	Critical
} SevLevel;

}
#endif
