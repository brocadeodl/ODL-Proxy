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

/**
 *  Copyright (c) 2010 - present by Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Module Name: GenericClientMessage.cpp
 *
 *  Author: suw
 *
 *  Description: implement client format and sending reply messages
 */
#include <stdarg.h>
#include <stdio.h>
#include <sstream>
#include "GenericClientMessage.h"

using namespace std;
// using namespace WaveNs;

void*
GenericClientWriterInit()
{
	generic_wyser_msg_reply_t* msg = new generic_wyser_msg_reply_t;
	bzero(msg, sizeof(*msg));
	return msg;
}

void GenericClientWriterWriteAP(void* handle, const char* fmt, va_list ap)
{
	generic_wyser_msg_reply_t* msg = (generic_wyser_msg_reply_t*)handle;
	string* out;
	char buf[256];

	if (msg->contents == NULL) {
		out = new string();
		// prismAssert(out, __FILE__, __LINE__);
		msg->contents = out;
	}
	else
		out = reinterpret_cast<string*>(msg->contents);

	vsnprintf(buf, sizeof(buf), fmt, ap);

	for (unsigned int i = 0; i < strlen(buf); ++i) {
		if (buf[i] == '%')
			out->append(1, '%');
		out->append(1, buf[i]);
	}
	return;
}


void GenericClientWriterWrite(void* handle, const char* fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	GenericClientWriterWriteAP(handle, fmt, ap);
	va_end (ap);
	return;
}

void* GetCharFromWriteMsg(void *handle, int *len)
{	
    generic_wyser_msg_reply_t* msg = (generic_wyser_msg_reply_t*)handle;
    char *data = NULL;

    *len = 0;
    if (msg == 0) return NULL;
    string* out = reinterpret_cast<string*>(msg->contents);

	if (out) {
        *len = out->size() + 1;
        data = (char *) calloc (1, *len);
		strncpy(data,out->c_str(),out->size());
		data [*len - 1] = '\0';
	}

    return data;
}

void deleteGenericClientWriterHandle(void *handle)
{
    generic_wyser_msg_reply_t* msg = (generic_wyser_msg_reply_t*)handle;
    if (msg) {
        string* out = reinterpret_cast<string*>(msg->contents);
        if (out) {
            delete out;
        }
        delete msg;
    }
}

namespace DcmNs {
using namespace WaveNs;

void GenericClientWriterLoad(generic_wyser_msg_reply_t *msg,
		DcmManagementInterfaceMessage& msgObj)
{
    if (msg == 0) return;
    string* out = reinterpret_cast<string*>(msg->contents);
	msg->contents = 0;

	msg->num_records = !!out; // convert ptr to bool
	msgObj.addBuffer(0, sizeof(*msg), msg, false);
	if (out) {
		msgObj.addBuffer(1, out->size(), out->c_str(), false);
		delete out;
	}
    return;
}
}
