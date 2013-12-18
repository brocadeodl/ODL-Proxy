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
 *  Module Name: GenericClientMessage.h
 *
 *  Author: suw
 *
 *  Description: generic messages sent to and received from client
 */
#ifndef __GENERICCLIENTMESSAGE_H__

// C msg structure shared by both backend and plugin
#define __GENERICCLIENTMESSAGE_H__

#include <stdarg.h>

#define GENERIC_WYSEREA_ARGS_MAX 50
typedef struct {
	// parsing CLI commands in tranditional argc, argv
	unsigned int argc;
	const char* argv[GENERIC_WYSEREA_ARGS_MAX];
	unsigned long long int cookie;
} generic_wyser_msg_t;

typedef struct {
	unsigned long long cookie; // cookie for next query
	int status; // status of the query
	int num_records; // number of records returned
	void* contents; // ptr to record contents
} generic_wyser_msg_reply_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * At client backend, initialize a control for writing output to WyserEa.
 * The return value used shall be used for subsequent write, and shall
 * be returned by backend handler.
 */
void* GenericClientWriterInit();
/**
 * At client backend, write to WyserEa. Handle is the value returned
 * by GenericClientWriterInit()
 */
void GenericClientWriterWriteAP(void* handle, const char* fmt, va_list ap);
void GenericClientWriterWrite(void* handle, const char* fmt, ...);
void* GetCharFromWriteMsg(void* handle, int *len);
void deleteGenericClientWriterHandle(void *handle);

#ifdef __cplusplus
}
#endif

// Generic Message Class
#ifdef __cplusplus
#include <string>
#include <vector>
#include <sstream>
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Utils/TraceUtils.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{
void GenericClientWriterLoad(generic_wyser_msg_reply_t *msg,
		DcmManagementInterfaceMessage& msgObj);

class GenericClientMessage_ : public DcmManagementInterfaceMessage
{
	vector<string> args;
	UI64 nextCookie;

protected:
	void setupAttributesForSerialization() {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeStringVector(&args, "args"));
		addSerializableAttribute(new AttributeUI64(&nextCookie,"nextCookie"));
	}

public:
	GenericClientMessage_(const vector<string>& in, int msg_type):
		DcmManagementInterfaceMessage(msg_type),args(in),nextCookie(0)	{}
	GenericClientMessage_(int msg_type):
		DcmManagementInterfaceMessage(msg_type){}

	virtual ~GenericClientMessage_ ()
	{
		WaveNs::trace(TRACE_LEVEL_DEBUG, "Enter Destructor");
	}

	const void *getCStructureForInputs () {
		generic_wyser_msg_t* msg = new generic_wyser_msg_t;
		bzero(msg, sizeof(*msg));
		msg->argc = args.size();
		for (unsigned int i = 0 ; i< msg->argc; ++i) {
			msg->argv[i] = args[i].c_str();
		}
		msg->cookie = nextCookie;
		return msg;
	}

    void loadOutputsFromCStructure (const void *pOutputCStructure) {
    	DcmNs::GenericClientWriterLoad
    	((generic_wyser_msg_reply_t*)pOutputCStructure, *this);
    	return;
    }

    void getDuplicateInterfaceOutputBuffer (UI32 tag, UI32 &size, void *&pBuffer)
    { pBuffer = findBuffer(tag, size); }

    void setCookie(UI64 in) { nextCookie = in;}
};

template <int MSGTYPE>
class GenericClientMessage : public GenericClientMessage_
{
public:
    GenericClientMessage(const vector<string>& in):
        GenericClientMessage_(in, MSGTYPE) {};
    GenericClientMessage():
        GenericClientMessage_(MSGTYPE) {};
    virtual ~GenericClientMessage()
    {
        WaveNs::trace(TRACE_LEVEL_DEBUG, "Enter Destructor");
    }
};

} // namespace

#endif // __cplusplus

#endif // __GENERICCLIENTMESSAGE_H__

