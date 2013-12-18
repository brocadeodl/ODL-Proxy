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
 *   Author : James Chen                                                   *
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
//*#include "RAS/Local/RASLocalObjectManager.h"
//*#include "RAS/Global/RASObjectManager.h"
#include "RAS/Local/RASVCSLocalObjectManager.h"
#include "RAS/Global/RASVCSObjectManager.h"
#include "ClientInterface/RAS/RASClientMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

namespace DcmNs
{

RASClientMessage::RASClientMessage ()
		//*: ManagementInterfaceMessage (RASObjectManager::getClassName (), RAS_VCSLOGGING)
		   : ManagementInterfaceMessage (RASVCSObjectManager::getClassName (), RAS_VCSLOGGING)
{
}

RASClientMessage::RASClientMessage (const UI32 &opcode)
		//*: ManagementInterfaceMessage (RASLocalObjectManager::getClassName (), opcode)
		   : ManagementInterfaceMessage (RASVCSLocalObjectManager::getClassName (), opcode)
{
}

RASClientMessage::~RASClientMessage ()
{
}

void  RASClientMessage::setupAttributesForSerialization()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32(&m_mappedId,               "mappedId"));
     addSerializableAttribute (new AttributeString(&m_data,                 "data"));
     addSerializableAttribute (new AttributeString(&m_vcslogString,         "vcslogString"));
     addSerializableAttribute (new AttributeUI8(&m_hdr_ver,                 "hdr_ver"));
     addSerializableAttribute (new AttributeUI8(&m_hdr_magic,               "hdr_magic"));
     addSerializableAttribute (new AttributeUI16(&m_hdr_evt_len,            "hdr_evt_len"));
     addSerializableAttribute (new AttributeUI8(&m_evt_hdr_ver,             "evt_hdr_ver"));
     addSerializableAttribute (new AttributeUI8(&m_evt_hdr_magic,           "evt_hdr_magic"));
     addSerializableAttribute (new AttributeUI16(&m_evt_hdr_evt_len,        "evt_hdr_evt_len"));
     addSerializableAttribute (new AttributeSI32(&m_evt_service_id,         "evt_service_id"));
     addSerializableAttribute (new AttributeSI32(&m_evt_instance,           "evt_instance"));
     addSerializableAttribute (new AttributeSI32(&m_evt_comp_id,            "evt_comp_id"));
     addSerializableAttribute (new AttributeString(&m_evt_comp_name,        "evt_comp_name"));
     addSerializableAttribute (new AttributeSI32(&m_evt_flags,              "evt_flags"));
     addSerializableAttribute (new AttributeSI32(&m_evt_objid,              "evt_objid"));
     addSerializableAttribute (new AttributeString(&m_evt_file,             "evt_file"));
     addSerializableAttribute (new AttributeString(&m_evt_func,             "evt_func"));
     addSerializableAttribute (new AttributeUI32(&m_evt_line,               "evt_line"));
     addSerializableAttribute (new AttributeSI32(&m_evt_local_time_sec,     "evt_local_time_sec"));
     addSerializableAttribute (new AttributeSI32(&m_evt_local_time_usec,    "evt_local_time_usec"));
     addSerializableAttribute (new AttributeSI32(&m_evt_time_sec,           "evt_time_sec"));
     addSerializableAttribute (new AttributeSI32(&m_evt_time_usec,          "evt_time_usec"));
     addSerializableAttribute (new AttributeSI32(&m_evt_global_seq,         "evt_global_seq"));	
     addSerializableAttribute (new AttributeSI32(&m_evt_external_seq,       "evt_external_seq"));
     addSerializableAttribute (new AttributeUI16(&m_evt_iq_drop,            "evt_iq_drop"));
     addSerializableAttribute (new AttributeUI16(&m_evt_rq_drop,            "evt_rq_drop"));
     addSerializableAttribute (new AttributeUI32Vector(&m_evt_arg_offset,   "evt_arg_offset"));
     addSerializableAttribute (new AttributeUI32Vector(&m_evt_arg,          "evt_arg"));
     addSerializableAttribute (new AttributeSI32(&m_msgdef_msg_id,          "msgdef_msg_id"));
     addSerializableAttribute (new AttributeSI32(&m_msgdef_attributes,      "msgdef_attributes"));
     addSerializableAttribute (new AttributeSI32(&m_msgdef_msg_class,       "msgdef_msg_class"));
     addSerializableAttribute (new AttributeSI32(&m_msgdef_severity,        "msgdef_severity"));
     addSerializableAttribute (new AttributeString(&m_msgdef_msg_value,     "msgdef_msg_value"));
     addSerializableAttribute (new AttributeString(&m_msgdef_message,       "msgdef_message"));
     addSerializableAttribute (new AttributeSI32(&m_msgdef_arg_type,        "msgdef_arg_type"));
     addSerializableAttribute (new AttributeSI32(&m_msgdef_arg_num,         "msgdef_arg_num"));
     addSerializableAttribute (new AttributeString(&m_msgdef_cat_name,      "msgdef_cat_name"));
     addSerializableAttribute (new AttributeString(&m_swname,               "swname"));
}
 
UI32  RASClientMessage::getMappedId() const
{
    return (m_mappedId);
}

void   RASClientMessage::setMappedId(const  UI32 &mappedId)
{
    m_mappedId = mappedId;
}

string  RASClientMessage::getData() const
{
        return (m_data);
}
void  RASClientMessage::setData(const string &Data)
{
        m_data = Data;
}
string RASClientMessage::getVcslogString() const
{
        return (m_vcslogString);
}
void   RASClientMessage::setVcslogString(const string &vcslogString)
{
       m_vcslogString = vcslogString;
}

string RASClientMessage::getSwName() const
{
        return (m_swname);
}
void   RASClientMessage::setSwName(const string &sw_name)
{
       m_swname = sw_name;
}

// Convert CPP msg to CStyle structure
const void *RASClientMessage::getCStructureForInputs ()
{

        // vcslog_t *pInput = new  vcslog_t;
	vcslog_t *pInput = NULL;

	// need assign the vcsmsg field one by one
	// *pInput = m_vcsmsg;

        return (pInput);
}

void RASClientMessage::setAll(const RASClientMessage &src)
{
    m_mappedId			= src.m_mappedId;

    m_hdr_ver 			= src.m_hdr_ver;
    m_hdr_magic 		= src.m_hdr_magic;
    m_hdr_evt_len 		= src.m_hdr_evt_len;
    m_evt_hdr_ver 		= src.m_evt_hdr_ver;;
    m_evt_hdr_magic 		= src.m_evt_hdr_magic;
    m_evt_hdr_evt_len 		= src.m_evt_hdr_evt_len;
    m_evt_service_id 		= src.m_evt_service_id;
    m_evt_instance 		= src.m_evt_instance;
    m_evt_comp_id 		= src.m_evt_comp_id;
    m_evt_comp_name		= src.m_evt_comp_name;
    m_evt_flags 		= src.m_evt_flags;
    m_evt_objid 		= src.m_evt_objid;
    m_evt_file			= src.m_evt_file;
    m_evt_func 			= src.m_evt_func;
    m_evt_line 			= src.m_evt_line;
    m_evt_local_time_sec 	= src.m_evt_local_time_sec;
    m_evt_local_time_usec 	= src.m_evt_local_time_usec;
    m_evt_time_sec 		= src.m_evt_time_sec;
    m_evt_time_usec 		= src.m_evt_time_usec;
    m_evt_global_seq 		= src.m_evt_global_seq;
    m_evt_external_seq 		= src.m_evt_external_seq;
    m_evt_iq_drop 		= src.m_evt_iq_drop;
    m_evt_rq_drop 		= src.m_evt_rq_drop;
    m_evt_arg_offset 		= src.m_evt_arg_offset;
    m_evt_arg 			= src.m_evt_arg;
    m_msgdef_msg_id 		= src.m_msgdef_msg_id;
    m_msgdef_attributes 	= src.m_msgdef_attributes;
    m_msgdef_msg_class 		= src.m_msgdef_msg_class;
    m_msgdef_severity 		= src.m_msgdef_severity;
    m_msgdef_msg_value 		= src.m_msgdef_msg_value;
    m_msgdef_message 		= src.m_msgdef_message;
    m_msgdef_arg_type 		= src.m_msgdef_arg_type;
    m_msgdef_arg_num 		= src.m_msgdef_arg_num;
    m_msgdef_cat_name 		= src.m_msgdef_cat_name;
    m_swname 			= src.m_swname;
    m_swname 			= src.m_swname;
    m_vcslogString 		= src.m_vcslogString;
    m_data 			= src.m_data;
}


// Convert CStyle structure to  RASClientMessage CPP msg
void RASClientMessage::getCStructure(vcslog_t *vcslog)
{
    rls_msg_t *evtp = &vcslog->vcsmsg;

    m_hdr_ver 			= RLS_GET_VER(evtp);
    m_hdr_magic 		= RLS_GET_MAGIC(evtp);

    // trace (TRACE_LEVEL_INFO, string (" RASClientMessage::getCStructure VER=")
    //	   + m_hdr_ver + string(" MAGIC=") + m_hdr_magic);
    
    m_hdr_evt_len 		= RLS_GET_EVT_LEN(evtp);
    m_evt_hdr_ver 		= RASEVT_EVT_GET_VER(RLS_GET_EVT(evtp));
    m_evt_hdr_magic 		= RASEVT_EVT_GET_MAGIC(evtp);
    m_evt_hdr_evt_len 		= RASEVT_EVT_GET_EVT_LEN(evtp);
    m_evt_service_id 		= RLS_GET_SVC_ID(evtp);
    m_evt_instance 		= RLS_GET_INSTANCE_ALL(evtp);
    m_evt_comp_id 		= RLS_GET_COMP_ID_ALL(evtp);
    m_evt_comp_name.assign((const char *)RLS_GET_COMP_NAME(evtp));
    m_evt_flags 		= RLS_GET_FLAGS(evtp);
    m_evt_objid 		= RLS_GET_OBJID(evtp);
    m_evt_file.assign((const char *)RLS_GET_FILENAME(evtp));
    m_evt_func.assign((const char *)RLS_GET_FUNC(evtp));
    m_evt_line 			= RLS_GET_LINENUM(evtp);
    m_evt_local_time_sec 	= RLS_GET_SRCTIMESEC(evtp);
    m_evt_local_time_usec 	= RLS_GET_SRCTIMEUSEC(evtp);
    m_evt_time_sec 		= RLS_GET_TIMESEC(evtp);
    m_evt_time_usec 		= RLS_GET_TIMEUSEC(evtp);
    m_evt_global_seq 		= RLS_GET_GSEQ(evtp);
    m_evt_external_seq 		= RLS_GET_ESEQ(evtp);
    m_evt_iq_drop 		= RLS_GET_IQDROP(evtp);
    m_evt_rq_drop 		= RLS_GET_RQDROP(evtp);

    m_evt_arg_offset.clear();
    UI32 i;
    for (i = 0; i < RLMO_MAX_MSG_ARGS / 4; i++) {
	void *pTemp = &((RLS_GET_EVT(evtp))->arg_offset[i*4]);
	UI32 *pTempUI32 = static_cast<UI32 *> (pTemp);
	m_evt_arg_offset.push_back(*pTempUI32);
    }

    m_evt_arg.clear();
    for (i = 0; i < RLMO_MSG_ARG_LEN / 4; i++) {
	void *pTemp = &((RLS_GET_EVT(evtp))->arg[i*4]);
    UI32 *pTempUI32 = static_cast<UI32 *> (pTemp);
	m_evt_arg.push_back(*pTempUI32);
    }
    // tracePrintf(TRACE_LEVEL_DEBUG, "RASClientMessage::setCStructure=%d", m_evt_arg_offset.size());
    m_msgdef_msg_id 		= RLS_GET_MSG_ID(evtp);
    m_msgdef_attributes 	= RLMD_GET_ATTR(RLS_GET_MSGDEF(evtp));
    m_msgdef_msg_class 		= RLMD_GET_CLASS(RLS_GET_MSGDEF(evtp));
    m_msgdef_severity 		= RLS_GET_SEVERITY(evtp);
    m_msgdef_msg_value.assign((const char *)RLMD_GET_MSG_ID_STRING(RLS_GET_MSGDEF(evtp)));
    m_msgdef_message.assign((const char *)RLMD_GET_MSG_FMT_STRING(RLS_GET_MSGDEF(evtp)));
    m_msgdef_arg_type 		= RLMD_GET_ARG_TYPE_ALL(RLS_GET_MSGDEF(evtp));
    m_msgdef_arg_num 		= RLS_GET_ARG_NUM(evtp);
    m_msgdef_cat_name.assign((const char *)RLS_GET_CATNAME(evtp));
    m_swname.assign((const char *)RLS_GET_SWNAME(evtp));
    m_vcslogString.assign((const char *)RLS_GET_SWNAME(evtp));
    m_data.assign((const char *)RLS_GET_SWNAME(evtp));

    // trace (TRACE_LEVEL_INFO, string(" RASClientMessage::getCStructure sw=") 
    //	   + m_swname + string(" VER=") + m_hdr_ver + string(" MAGIC=") + m_hdr_magic);
}


// Convert  RASClientMessage CPP msg to CStyle structure
void RASClientMessage::setCStructure(vcslog_t *vcslog)
{
    rls_msg_t *evtp = &vcslog->vcsmsg;

    RLS_SET_VER(evtp, m_hdr_ver);
    RLS_SET_MAGIC(evtp, m_hdr_magic);

    RLS_SET_EVT_LEN(evtp, m_hdr_evt_len);
    RASEVT_EVT_SET_VER(RLS_GET_EVT(evtp), m_evt_hdr_ver);
    RASEVT_EVT_SET_MAGIC(RLS_GET_EVT(evtp), m_evt_hdr_magic);
    RASEVT_EVT_SET_EVT_LEN(RLS_GET_EVT(evtp), m_evt_hdr_evt_len);
    RLS_SET_SVC_ID(evtp, m_evt_service_id);
    RLS_SET_INSTANCE_ALL(evtp, m_evt_instance);
    RLS_SET_COMP_ID_ALL(evtp, m_evt_comp_id);
    RLS_SET_COMP_NAME(evtp, m_evt_comp_name.c_str());
    RLS_SET_FLAGS(evtp, m_evt_flags);
    RLS_SET_OBJID(evtp, m_evt_objid);
    RLS_SET_FILENAME(evtp, m_evt_file.c_str());
    RLS_SET_FUNC(evtp, m_evt_func.c_str());
    RLS_SET_LINENUM(evtp, m_evt_line);
    RLS_SET_SRCTIMESEC(evtp, m_evt_local_time_sec);
    RLS_SET_SRCTIMEUSEC(evtp,  m_evt_local_time_usec);
    RLS_SET_TIMESEC(evtp,  m_evt_time_sec);
    RLS_SET_TIMEUSEC(evtp, m_evt_time_usec);
    RLS_SET_GSEQ(evtp, m_evt_global_seq);
    RLS_SET_ESEQ(evtp,m_evt_external_seq);
    RLS_SET_IQDROP(evtp, m_evt_iq_drop);
    RLS_SET_RQDROP(evtp, m_evt_rq_drop);

    // tracePrintf(TRACE_LEVEL_DEBUG, "RASClientMessage::setCStructure=%d", m_evt_arg_offset.size());
    UI32 i;
    for (i = 0; i < m_evt_arg_offset.size (); i++) {
	void *pTemp = &((RLS_GET_EVT(evtp))->arg_offset[i*4]);
    UI32 *pTempUI32 = static_cast<UI32 *> (pTemp);
	*pTempUI32 = m_evt_arg_offset[i];
    }

    for (i = 0; i < m_evt_arg.size (); i++) {
	void *pTemp = &((RLS_GET_EVT(evtp))->arg[i*4]);
    UI32 *pTempUI32 = static_cast<UI32 *> (pTemp);
	*pTempUI32 = m_evt_arg[i];
    }

    RLMD_SET_MSG_ID(RLS_GET_MSGDEF(evtp), m_msgdef_msg_id);
    RLMD_SET_ATTR(RLS_GET_MSGDEF(evtp), m_msgdef_attributes);
    RLMD_SET_CLASS(RLS_GET_MSGDEF(evtp), m_msgdef_msg_class);
    RLMD_SET_SEVERITY(RLS_GET_MSGDEF(evtp), m_msgdef_severity);
    strncpy((RLS_GET_MSGDEF(evtp))->msg_value,
	   m_msgdef_msg_value.c_str(), MAX_MESSAGE_VALUE_LEN+1);
	strncpy((RLS_GET_MSGDEF(evtp))->message,
	   m_msgdef_message.c_str(), MAX_MESSAGE_LEN + 1);
    RLMD_SET_ARG_TYPE(RLS_GET_MSGDEF(evtp), m_msgdef_arg_type);
    RLMD_SET_ARG_NUM(RLS_GET_MSGDEF(evtp), m_msgdef_arg_num);
    RLMD_SET_CATNAME(RLS_GET_MSGDEF(evtp),m_msgdef_cat_name.c_str());

    RLS_SET_SWNAME(evtp, m_swname.c_str());
}


void RASClientMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
{
}


LCCLIMsg::LCCLIMsg ()
        : DcmManagementInterfaceMessage (LCCLI_MSG)
{
}

LCCLIMsg::LCCLIMsg (const string &Arg, const UI32 &CliCode)
        : DcmManagementInterfaceMessage (LCCLI_MSG),
        m_Arg    (Arg),
        m_CliCode   (CliCode)
{
}

LCCLIMsg::~LCCLIMsg ()
{
}

void  LCCLIMsg::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeUI32(&m_CliCode,"CliCode"));
    addSerializableAttribute (new AttributeString(&m_Arg,"Arg"));
}

void  LCCLIMsg::setArg(const string &Arg)
{
    m_Arg  =  Arg;
}

string  LCCLIMsg::getArg() const
{
        return (m_Arg);
}
    
void  LCCLIMsg::setCliCode(const UI32 &CliCode)
{
        m_CliCode  =  CliCode;
}

UI32  LCCLIMsg::getCliCode() const
{
        return (m_CliCode);
}

void LCCLIMsg::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
						  UI32 &size, void *&pBuffer)
{
        pBuffer   =   findBuffer(bufferNum, size);
}
    
const void *LCCLIMsg::getCStructureForInputs ()
{
        lccli_in_msg_t *pInput = new lccli_in_msg_t;

        strncpy (pInput->argv, m_Arg.c_str (), LCCLI_INPUT__MAX);
        pInput->cli_code    = m_CliCode;

        return (pInput);
}

void LCCLIMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
{
        lccli_out_msg_t *msg = (lccli_out_msg_t *)pOutputCStructure;

	tracePrintf(TRACE_LEVEL_INFO, "LCCLIMsg:data len=%d",  msg->out_buf_len);

	// duplicate data to buf for server to process
	// m->getDuplicateInterfaceOutputBuffer(LCCLI_MSG, size, data);
	// Call addBuffer(...., false) If the data is not included as part of lccli_out_msg_t
	// Expect pOutputCStructure should include lccli_out_msg_t and real data.
        addBuffer (LCCLI_MSG, sizeof(lccli_out_msg_t) + msg->out_buf_len,
		   pOutputCStructure, false);
}
}

