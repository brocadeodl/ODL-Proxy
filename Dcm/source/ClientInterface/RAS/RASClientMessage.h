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
 *   Author : Jmaes Chen                                                     *
 **************************************************************************/

#ifndef RASCLIENTMESSAGE_H
#define RASCLIENTMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ClientInterface/RAS/RASClient.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

typedef enum
{
	RAS_VCSLOGGING = 1000,
	RAS_VCSLOGGING_LOCAL,
	LCCLI_MSG
} RASClientMessageTypes;

//*class RASObjectManager;
//*class RASLocalObjectManager;
class RASVCSObjectManager;
class RASVCSLocalObjectManager;

class RASClientMessage : public ManagementInterfaceMessage
{
    private:
	// need to be in private
	virtual const void *getCStructureForInputs    ();
	virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);
	void setupAttributesForSerialization();

    protected:
	// virtual void setupAttributesForSerialization();
    public:
	RASClientMessage ();
	RASClientMessage (const UI32 &opcode);
	virtual    ~RASClientMessage ();
	void setMappedId(const UI32 &mappedId);
	UI32 getMappedId() const;
	string  getData() const;
	void setData(const string &Data);

	void setCStructure (vcslog_t *evtp);
	void getCStructure (vcslog_t *evtp);

	vcslog_t getVcslog() const;
        void setVcslog (const vcslog_t &vcslog);
	string getVcslogString() const;
        void setVcslogString (const string &vcslogString);
	string getSwName() const;
        void setSwName (const string &swName);
	void setAll(const RASClientMessage &src);

// Now the data members

        private:
        protected:
        public:
	UI32    m_mappedId;
	string  m_data;
	// vcslog_t  m_vcslog;
	string	  m_vcslogString;

	// rls_msg_t
	  // rasevt_hdr_t hdr;
	  UI8  m_hdr_ver;
	  UI8  m_hdr_magic;
	  UI16 m_hdr_evt_len;		/* evt len */
	// rasevt_evt_t    evt;
	/* header section */
	  // rasevt_hdr_t hdr;
	  UI8 m_evt_hdr_ver;
	  UI8 m_evt_hdr_magic;
  	  UI16 m_evt_hdr_evt_len;		/* evt len */

	  /* source section, caller info */
	  SI32  m_evt_service_id;	/* service id, setup by raslog_init */
	  SI32  m_evt_instance ;	/* instance number, setup by raslog_init */
	  SI32  m_evt_comp_id;	/* component id, setup by raslog_init */
	  string m_evt_comp_name; // [MAX_COMPNAME_SIZE+1];
	  SI32  m_evt_flags;	/* defined in raslog.h */
	  SI32  m_evt_objid;	/* object ID of event */

	  /* Filename/function name/line # where event was generated */
	  string m_evt_file; //[RLMO_MAX_FILE_LEN];
	  string m_evt_func;  //[RLMO_MAX_FUNC_LEN];

	  UI32  m_evt_line;

	  SI32  m_evt_local_time_sec;
	  SI32  m_evt_local_time_usec;
	
	  /* system wide event information */
	  SI32  m_evt_time_sec;
	  SI32  m_evt_time_usec;
	  SI32  m_evt_global_seq;		/* raslog global sequence number */
	  SI32  m_evt_external_seq;	/* raslog external sequence number */
	  UI16  m_evt_iq_drop;	/* incoming message queue message drop count */
	  UI16  m_evt_rq_drop;	/* receiver message queue message drop count */

	  vector<UI32>	m_evt_arg_offset; // [RLMO_MAX_MSG_ARGS]; /* word offsets of arguments */
	  vector<UI32>	m_evt_arg; 	  //[RLMO_MSG_ARG_LEN];	    /* packed arguments */

	  /* Static message definition from message catalog */
	  //rlmd_msgdef_t msgdef;
	    //raslog_msg_id_t msg_id;	/* message id */
	    SI32   m_msgdef_msg_id;
	    SI32   m_msgdef_attributes; /* attributes of the message */
	    SI32   m_msgdef_msg_class;  /* attributes of the message */
	    SI32   m_msgdef_severity;   /* severity level */
	    string m_msgdef_msg_value;  // [MAX_MESSAGE_VALUE_LEN+1]; /* msg string */
	    string m_msgdef_message;    // [MAX_MESSAGE_LEN + 1];	/* msg format string */
	    SI32   m_msgdef_arg_type;	/* argument type */
	    SI32   m_msgdef_arg_num;	/* number of argument */
	    string m_msgdef_cat_name;  //[RAS_CAT_NAME_LEN];

	    string m_swname;    //[RLS_SWNAME];	/* switchname/chassisname  */
    };


class LCCLIMsg : public DcmManagementInterfaceMessage
{
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LCCLIMsg ();
            LCCLIMsg (const string &Arg, const UI32 &CliCode);
            virtual ~LCCLIMsg ();
            void setArg(const string &Arg);
            string getArg() const;
            void setCliCode(const UI32 &CliCode);
            UI32 getCliCode() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 	*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, 										UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_Arg;
            UI32 m_CliCode;

        public:
};

}
#endif  //RASCLIENTMESSAGE_H
