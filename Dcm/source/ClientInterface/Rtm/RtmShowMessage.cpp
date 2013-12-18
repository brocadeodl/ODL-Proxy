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
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rtm/RtmShowMessage.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Rtm/Local/RtmLocalShowMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <netinet/in.h>

#include <stdarg.h>
#include <stdio.h>
#include <sstream>

using namespace std;
// using namespace WaveNs;

void*
RtmClientWriterInit()
{
	rtm_show_out_msg_t* msg = new rtm_show_out_msg_t;
	bzero(msg, sizeof(*msg));
	return msg;
}

void RtmClientWriterWriteAP(void* handle, const char* fmt, va_list ap)
{
	rtm_show_out_msg_t* msg = (rtm_show_out_msg_t*)handle;
	string* out;
	char buf[256];

	if (msg->contents == NULL) {
		out = new string();
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
	msg->buf_is_string = 1;
	msg->cmdCode = RTM_SHOW_MSG_OUTPUT;

	return;
}


void RtmClientWriterWrite(void* handle, const char* fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	RtmClientWriterWriteAP(handle, fmt, ap);
	va_end (ap);
	return;
}


using namespace WaveNs;

namespace DcmNs
{

    RtmShowMessage::RtmShowMessage (RtmLocalShowMessage *pMessage)
        : DcmManagementInterfaceMessage (RTMSHOW)
        , m_argv(pMessage->m_argv)
    {
        m_cmdCode = pMessage->m_cmdCode;
        m_vrfName = pMessage->m_vrfName;
    }

    RtmShowMessage::RtmShowMessage ()
        : DcmManagementInterfaceMessage (RTMSHOW)
    {
        m_vrfName = "";
    }

    RtmShowMessage::~RtmShowMessage ()
    {
    }

    void  RtmShowMessage::setupAttributesForSerialization()
    {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    		addSerializableAttribute (new AttributeStringVector(&m_argv, "argv"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
    }

	void  RtmShowMessage::setArgs(const vector<string>& args)
    {
		m_argv = args;
    }
    void RtmShowMessage::setVrfNAme (const string &vrfName)
    {
        m_vrfName = vrfName;
    }

    string RtmShowMessage::getVrfName () const
    {
        return m_vrfName;
    }


	void RtmShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }

	// Convert info from C++ struct RtmShowMessage populated in the APWorker to a C struct for backend
    const void *RtmShowMessage::getCStructureForInputs ()
    {
		rtm_show_in_msg_t* msg = new rtm_show_in_msg_t;
		bzero(msg, sizeof(*msg));
        int offset =0;

        msg->cmdCode = static_cast<RtmShowCmdCode_t> (m_cmdCode);
        snprintf(msg->vrf_name, RTM_VRF_NAME_STRING_LEN, "%s", m_vrfName.c_str());
		msg->argc = m_argv.size();

        if( ((msg->argc > 2) && !(strncmp(m_argv[msg->argc-2].c_str(), "vrf", 3))) ) { //for vrf cmds
    		for (unsigned int i = 0 ; i< msg->argc-2; ++i) {
	    		msg->argv[i+offset] = m_argv[i].c_str();
                if( i == 2) { // copying "vrf <vrf-id>" after "show ip route/static"
                    msg->argv[i+1] = m_argv[msg->argc-2].c_str();
                    msg->argv[i+2] = m_argv[msg->argc-1].c_str();
                    offset = 2;
                }
		    }
        } else {
    		for (unsigned int i = 0 ; i< msg->argc; ++i) {
	    		msg->argv[i] = m_argv[i].c_str();
		    }
        }    

		return msg;
    }

	// Add C struct returned from backend into buffer so APWorker can process it
	void 
    RtmShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        if (!pOutputCStructure) return;

		rtm_show_out_msg_t *msg = (rtm_show_out_msg_t *)pOutputCStructure;

		if (msg->buf_is_string) {
			string* out = reinterpret_cast<string*>(msg->contents);
			msg->contents = 0;

			msg->num_records = !!out; // convert ptr to bool
			addBuffer(0, sizeof(*msg), msg, false);
			if (out) {
				addBuffer(1, out->size(), out->c_str(), false);
				delete out;
			}
			return;
		} else {
			addBuffer(0, sizeof(*msg), msg, false);
			if (msg->size) {		
				addBuffer(1, msg->size, msg->buf, false);
				free(msg->buf);
			}
		}
	}
}

