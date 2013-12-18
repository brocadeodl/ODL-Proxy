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
 *   Copyright (C) 2010-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : James Chen                                                   *
 *   Description:                                                          *    
 *       This header file contains RAS-LOG related stub routines           *
 *       and structures                                                    *    
 ***************************************************************************/

#ifndef __RASLOGM_H__
#define __RASLOGM_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
typedef struct {
	char ver;
	char magic;
	short evt_len;		/* evt len */
} rasevt_hdr_t;
//typedef int raslog_msg_id_t;
#define raslog_msg_id_t int
#define MAX_MESSAGE_VALUE_LEN	9
#define	MAX_MESSAGE_LEN		128
#define	RASLOG_TIME_LEN		21
#define RAS_CAT_NAME_LEN 8
#define RAS_CAT_NAME "FOS"
#define RASLOG_TIME_FMT "%Y/%m/%d-%H:%M:%S"
#define RASLOG_OK 0
#define RASLOG_ALL_MSGS         0
#define RASLOG_EXT_MSGS         2

#ifndef MYSWITCH
#define MYSWITCH                1
#endif

#define RLMD_ATTR_AUDIT			2
#define RLMD_ATTR_LOG			4
#define RLMD_ATTR_FFDC          8
#define RLMD_ATTR_VCS           16
#define RLMD_ATTR_DCE           32
#define	RLMD_ATTR_CFFDC 		64
#define RLMD_ATTR_MAX           RLMD_ATTR_CFFDC
#define RLMD_IS_FFDC(msgid)     (msgid & RLMD_ATTR_FFDC)
#define REF_CLASS_SIZE 8
#define RASLOG_NUM_TYPES        2
#define RASLOG_MAX_APP_TYPE		2
#define RASLOG_MSGS_PER_TYPE    2048

#define RASEVT_NOFLAGS	0x00000000	/* Normal usage */
#define RASEVT_FRCLOG	0x00000001	/* Force duplicate message logging */
#define RASEVT_NO_SEEPROM	0x00000002	/* Don't store in SEEPROM */
#define RASEVT_NOTRAP	0x00000004	/* Don't generate an SNMP trap */
#define RASEVT_NODIAG	0x00000008	/* Don't display while in diag mode */
#define RASEVT_FORCE_SEEPROM	0x00000010	/* override default filter */
#define RASEVT_IGNORE_FFDC	0x00000020	/* ignore AP FFDC */
#define RASEVT_LOCALONLY	0x00000040	/* don't allow msg to leave switch */

#define RASLOG_INTERNAL     0x10000000
#define RLMD_IS_INTERNAL(msgid) (msgid & RASLOG_INTERNAL)
#define RLS_GET_INSTANCE_ALL(evtp)  RASEVT_EVT_GET_INSTANCE_ALL(&(evtp)->evt)
#define RASEVT_EVT_GET_INSTANCE_ALL(evtp)       ((evtp)->instance)

#define RLS_GET_COMP_ID_ALL(evtp)       RASEVT_EVT_GET_COMP_ID_ALL(&(evtp)->evt)
#define RASEVT_EVT_GET_COMP_ID_ALL(evtp)        ((evtp)->comp_id)

#define RLS_SET_INSTANCE_ALL(evtp, value) \
		RASEVT_EVT_SET_INSTANCE_ALL(&(evtp)->evt, value)
#define RASEVT_EVT_SET_INSTANCE_ALL(evtp, value) ((evtp)->instance = value)

#define RLS_SET_COMP_ID_ALL(evtp, value) \
		RASEVT_EVT_SET_COMP_ID_ALL(&(evtp)->evt, value)
#define RASEVT_EVT_SET_COMP_ID_ALL(evtp, value) ((evtp)->comp_id = value)
#define RASEVT_SET_USERNAME(evtp, value)    \
		RASAUDIT_SET_USERNAME(&(evtp)->audit, value)
#define RASAUDIT_SET_USERNAME(audit, value) \
		(strncpy((audit)->username, value, RASEVT_USERNAME_LEN))
#define RASEVT_GET_INSTANCE_ALL(evtp)     RASEVT_EVT_GET_INSTANCE_ALL(&(evtp)->evt)
#define RASEVT_EVT_GET_INSTANCE_ALL(evtp)       ((evtp)->instance)
#define RAS_INSTANCE_VCS        0x00004000
#define RASEVT_SET_INSTANCE_ALL(evtp, value) \
		RASEVT_EVT_SET_INSTANCE_ALL(&(evtp)->evt, value)
#define RASEVT_EVT_SET_INSTANCE_ALL(evtp, value) ((evtp)->instance = value)
typedef struct rlmd_msgdef {
	raslog_msg_id_t msg_id;	/* message id */
	int 	attributes; /* attributes of the message */
	int 	msg_class; /* attributes of the message */
	int	severity;	/* severity level */
	char	msg_value[MAX_MESSAGE_VALUE_LEN+1];	/* msg string */
	char	message[MAX_MESSAGE_LEN + 1];	/* msg format string */
	int	arg_type;	/* argument type */
	int	arg_num;	/* number of argument */
	char 	cat_name[RAS_CAT_NAME_LEN];
} rlmd_msgdef_t;

#define RLMD_SET_ARG_NUM(msgdef, value) ((msgdef)->arg_num = value)
#define RLMD_GET_ARG_NUM(msgdef) ((msgdef)->arg_num)

/* get argument type */
#define RLMD_SET_ARG_TYPE(msgdef, value) ((msgdef)->arg_type = value)
#define RLMD_GET_ARG_TYPE_ALL(msgdef) ((msgdef)->arg_type)

#define RLMD_GET_ARG_TYPE(msgdef, idx) (((msgdef)->arg_type >> \
(idx * RLMD_ARG_TYPE)) & RLMD_ARG_MASK)

#define RLMD_SET_CLASS(msgdef, value) ((msgdef)->msg_class = value)
#define RLMD_GET_CLASS(msgdef) ((msgdef)->msg_class)

#define RLMD_SET_ATTR(msgdef, value) ((msgdef)->attributes = value)
#define RLMD_GET_ATTR(msgdef) ((msgdef)->attributes)
#define RLMD_ADD_ATTR(msgdef, attribute) ((msgdef)->attributes |= attribute)
#define RLMD_REMOVE_ATTR(msgdef, attribute) ((msgdef)->attributes &= ~attribute)

#define RLMD_SET_SEVERITY(msgdef, value) ((msgdef)->severity =  value)
#define RLMD_GET_SEVERITY(msgdef) ((msgdef)->severity)

#define RLMD_COPY_MSG_ID_STRING(dest, src) \
   memcpy((void *)(dest)->msg_value, (void *)(src)->msg_value, \
   MAX_MESSAGE_VALUE_LEN+1)
#define RLMD_GET_MSG_ID_STRING(msgdef) ((msgdef)->msg_value)

#define RLMD_COPY_MSG_FMT_STRING(dest, src) \
  memcpy((void *)(dest)->message, (void *)(src)->message, MAX_MESSAGE_LEN + 1)
#define RLMD_GET_MSG_FMT_STRING(msgdef) ((msgdef)->message)

#define RLMD_SET_MSG_ID(msgdef, value) ((msgdef)->msg_id = value)
#define RLMD_GET_MSG_ID(msgdef) ((msgdef)->msg_id)
#define RLMD_SET_CATNAME(msgdef, name) \
  (strncpy((msgdef)->cat_name, name, RAS_CAT_NAME_LEN - 1))
#define RLMD_GET_CATNAME(msgdef) ((msgdef)->cat_name)

/* check to see if this is a valid msg id */
#define RLMD_VALID_MSG_ID(msgdef) ((msgdef)->msg_id != 0)

#define MAX_COMPNAME_LEN		16
#define MAX_COMPNAME_SIZE	MAX_COMPNAME_LEN + 1 /* for NUL character */
#define RLMO_MAX_FILE_LEN   16
#define RLMO_MAX_FUNC_LEN   16
#define MSG_ARG_LEN		(256) /* end with '\0' */
#define RLMO_MSG_ARG_LEN    MSG_ARG_LEN
#define RLMO_MAX_MSG_ARGS   16	


#define RASEVT_MSG_INFO_LEN   64      
#define RASEVT_MAX_FILE_LEN   16
#define RASEVT_MAX_FUNC_LEN   16

typedef struct {
	/* header section */
	rasevt_hdr_t hdr;

	/* source section, caller info */
	int service_id;	/* service id, setup by raslog_init */
	int instance ;	/* instance number, setup by raslog_init */
	int comp_id;	/* component id, setup by raslog_init */
	char comp_name[MAX_COMPNAME_SIZE+1];
	int flags;	/* defined in raslog.h */
	int objid;	/* object ID of event */

	/* Filename/function name/line # where event was generated */
	char file[RLMO_MAX_FILE_LEN];
	char func[RLMO_MAX_FUNC_LEN];

	unsigned int line;

	int local_time_sec;
	int local_time_usec;
	
	/* system wide event information */
	int time_sec;
	int time_usec;
	int global_seq;		/* raslog global sequence number */
	int external_seq;	/* raslog external sequence number */
	short iq_drop;	/* incoming message queue message drop count */
	short rq_drop;	/* receiver message queue message drop count */

	/* msg info */
	char arg_offset[RLMO_MAX_MSG_ARGS]; /* word offsets of arguments */
	char arg[RLMO_MSG_ARG_LEN];	    /* packed arguments */

	/* Static message definition from message catalog */
	rlmd_msgdef_t msgdef;
} rasevt_evt_t;
#define RASEVT_EVT_GET_HDR(evtp)	(&(evtp)->hdr)

#define RASEVT_EVT_CPY(evtsrc, evtdest)	(*(evtdest) = *(evtsrc))
#define RASEVT_EVT_SET_VER(evtp, value)	((evtp)->hdr.ver = (char)value)
#define RASEVT_EVT_GET_VER(evtp)    ((evtp)->hdr.ver)
#define RASEVT_EVT_SET_MAGIC(evtp, value) ((evtp)->hdr.magic = (char)value)
#define RASEVT_EVT_GET_MAGIC(evtp)	((char)(evtp)->hdr.magic)
#define RASEVT_EVT_SET_EVT_LEN(evtp, value) ((evtp)->hdr.evt_len = (short)value)
#define RASEVT_EVT_GET_EVT_LEN(evtp)	((evtp)->hdr.evt_len)
#define RASEVT_EVT_SET_SVC_ID(evtp, value) ((evtp)->service_id = value)
#define RASEVT_EVT_GET_SVC_ID(evtp)	    ((evtp)->service_id)
#define RASEVT_EVT_SET_INSTANCE(evtp, value) ((evtp)->instance = value)
#define RASEVT_EVT_GET_INSTANCE(evtp)	    ((evtp)->instance)
#define RASEVT_EVT_SET_COMP_ID(evtp, value) ((evtp)->comp_id = value)
#define RASEVT_EVT_GET_COMP_ID(evtp)        ((evtp)->comp_id)
#define RASEVT_EVT_SET_COMP_NAME(evtp, value) \
            (strncpy((evtp)->comp_name, value, MAX_COMPNAME_LEN))
#define RASEVT_EVT_GET_COMP_NAME(evtp)        ((evtp)->comp_name)
#define RASEVT_EVT_SET_FLAGS(evtp, value)   ((evtp)->flags = value)
#define RASEVT_EVT_GET_FLAGS(evtp)	    ((evtp)->flags)
#define RASEVT_EVT_SET_OBJID(evtp, value)   ((evtp)->objid = value)
#define RASEVT_EVT_GET_OBJID(evtp)	    ((evtp)->objid)
#define RASEVT_EVT_SET_LINENUM(evtp, value) ((evtp)->line = value)
#define RASEVT_EVT_GET_LINENUM(evtp)	    ((evtp)->line)
#define RASEVT_EVT_SET_FILENAME(evtp, value) do { \
         if (strlen(value) >= RASEVT_MAX_FILE_LEN) {  \
	 strncpy((evtp)->file, value, RASEVT_MAX_FILE_LEN); \
	 (evtp)->file[RASEVT_MAX_FILE_LEN - 1] = '\0'; } else \
	 strcpy((evtp)->file, value); } while (0)
#define RASEVT_EVT_GET_FILENAME(evtp) 	((evtp)->file)
#define RASEVT_EVT_SET_FUNC(evtp, value) 	\
        do { if (strlen(value) >= RASEVT_MAX_FUNC_LEN) {  \
	 strncpy((evtp)->func, value, RASEVT_MAX_FUNC_LEN); \
	 (evtp)->func[RASEVT_MAX_FUNC_LEN - 1] = '\0'; } else \
	 strcpy((evtp)->func, value); } while (0)
#define RASEVT_EVT_GET_FUNC(evtp) 	((evtp)->func)
#define RASEVT_EVT_SET_CAT_NAME(evtp, value) 	\
	 strcpy((evtp)->catalog_name, value)
#define RASEVT_EVT_GET_CAT_NAME(evtp) 	((evtp)->catalog_name)
#define RASEVT_EVT_SET_SRCTIMESEC(evtp, value) \
        ((evtp)->local_time_sec = value)
#define RASEVT_EVT_GET_SRCTIMESEC(evtp)	((evtp)->local_time_sec)
#define RASEVT_EVT_SET_SRCTIMEUSEC(evtp, value) \
        ((evtp)->local_time_usec = value)
#define RASEVT_EVT_GET_SRCTIMEUSEC(evtp) ((evtp)->local_time_usec)
#define RASEVT_EVT_SET_TIMESEC(evtp, value)	((evtp)->time_sec = value)
#define RASEVT_EVT_GET_TIMESEC(evtp)	((evtp)->time_sec)
#define RASEVT_EVT_SET_TIMEUSEC(evtp, value) ((evtp)->time_usec = value)
#define RASEVT_EVT_GET_TIMEUSEC(evtp)	((evtp)->time_usec)
#define RASEVT_EVT_SET_GSEQ(evtp, value) ((evtp)->global_seq = value)
#define RASEVT_EVT_GET_GSEQ(evtp)	((evtp)->global_seq)
#define RASEVT_EVT_SET_ESEQ(evtp, value) ((evtp)->external_seq = value)
#define RASEVT_EVT_GET_ESEQ(evtp)	((evtp)->external_seq)
#define RASEVT_EVT_SET_IQDROP(evtp, value)  ((evtp)->iq_drop = value)
#define RASEVT_EVT_ADD_IQDROP(evtp, value)  ((evtp)->iq_drop += value)
#define RASEVT_EVT_GET_IQDROP(evtp)	((evtp)->iq_drop)
#define RASEVT_EVT_SET_RQDROP(evtp, value)	((evtp)->rq_drop = value)
#define RASEVT_EVT_ADD_RQDROP(evtp, value)	((evtp)->rq_drop += value)
#define RASEVT_EVT_GET_RQDROP(evtp)	((evtp)->rq_drop)
#define RASEVT_EVT_GET_MSG_ID(evtp)   (RLMD_GET_MSG_ID(&(evtp)->msgdef))
#define RASEVT_EVT_SET_MSG_ID(evtp)   (RLMD_SET_MSG_ID(&(evtp)->msgdef))

#define RASEVT_EVT_ARG_COMP(evtsrc, evtdest) \
        (memcmp((evtsrc)->arg, (evtdest)->arg, RASEVT_MSG_ARG_LEN))
#define RASEVT_EVT_SET_ARG_OFFSET_RESET(evtp) \
        bzero((evtp)->arg_offset, RASEVT_MAX_MSG_ARGS)
#define RASEVT_EVT_SET_ARG_OFFSET(evtp, idx, value) \
        ((evtp)->arg_offset[idx] = (char)value)
#define RASEVT_EVT_GET_ARG_OFFSET(evtp, idx) \
        ((unsigned int)((evtp)->arg_offset[idx]))
#define RASEVT_EVT_SET_ARG_RESET(evtp) \
        bzero((evtp)->arg, RASEVT_MSG_ARG_LEN)
#define RASEVT_EVT_COPY_ARG_OFFSET(dest, src) \
        memcpy((dest)->arg_offset, (src)->arg_offset, RLMO_MAX_MSG_ARGS)

#define RASEVT_EVT_COPY_ARG(dest, src) \
	memcpy((dest)->arg, (src)->arg, RLMO_MSG_ARG_LEN)

#define RASEVT_EVT_SET_ARG_STR(evtp, offset, ptr, size) \
        strncpy((char *)&(evtp)->arg[offset], ptr, size)
#define RASEVT_EVT_SET_ARG_STR(evtp, offset, ptr, size) \
        strncpy((char *)&(evtp)->arg[offset], ptr, size)
#define RASEVT_EVT_SET_ARG_1(evtp, offset, value) \
        (*(char *)&(evtp)->arg[offset] = (char)value)
#define RASEVT_EVT_SET_ARG_4(evtp, offset, value) \
        (*(int *)&(evtp)->arg[offset] = value)
#define RASEVT_EVT_SET_ARG_8(evtp, offset, value) \
        (*(double *)&(evtp)->arg[offset] = value)
#define RASEVT_EVT_GET_ARG_STR(evtp, offset) \
        ((char *)&(evtp)->arg[offset])
#define RASEVT_EVT_GET_ARG_4(evtp, offset) \
        (*(int *)&(evtp)->arg[offset])
#define RASEVT_EVT_GET_ARG_8(evtp, offset) \
        (*(double *)&(evtp)->arg[offset])
#define RASEVT_EVT_SET_MSGDEF(evtp, md) (((evtp)->msgdef) = md)
#define RASEVT_EVT_GET_MSGDEF(evtp) (&(evtp)->msgdef)

#define RLS_SWNAME	32
typedef struct rls_msg {
	rasevt_hdr_t hdr;
	rasevt_evt_t    evt;
	char name[RLS_SWNAME];	/* switchname/chassisname  */
} rls_msg_t;

#define RLS_GET_EVT(evtp) (&(evtp)->evt)

#define RLS_SET_SWNAME_OLD(evtp, value)	(strncpy((evtp)->name, value, OLD_SWNAME_SIZE - 1))
#define RLS_SET_SWNAME(evtp, value)	{ \
	strncpy((evtp)->name, value, RLS_SWNAME - 1); \
	(evtp)->name[RLS_SWNAME - 1] = '\0'; \
}

#define RLS_GET_SWNAME(evtp)	((evtp)->name)

#define RLS_SET_VER(evtp, value)	((evtp)->hdr.ver = (char)value)
#define RLS_GET_VER(evtp)    ((evtp)->hdr.ver)
#define RLS_SET_MAGIC(evtp, value) ((evtp)->hdr.magic = (char)value)
#define RLS_GET_MAGIC(evtp)	((char)(evtp)->hdr.magic)
#define RLS_SET_EVT_LEN(evtp, value) ((evtp)->hdr.evt_len = (short)value)
#define RLS_GET_EVT_LEN(evtp)	((evtp)->hdr.evt_len)

#define RLS_SET_SVC_ID(evtp, value)	\
        RASEVT_EVT_SET_SVC_ID(&(evtp)->evt, value)
#define RLS_GET_SVC_ID(evtp)		RASEVT_EVT_GET_SVC_ID(&(evtp)->evt)
#define RLS_SET_INSTANCE(evtp, value) \
        RASEVT_EVT_SET_INSTANCE(&(evtp)->evt, value)
#define RLS_GET_INSTANCE(evtp)	RASEVT_EVT_GET_INSTANCE(&(evtp)->evt)
#define RLS_SET_COMP_ID(evtp, value) \
        RASEVT_EVT_SET_COMP_ID(&(evtp)->evt, value)
#define RLS_GET_COMP_ID(evtp)    	RASEVT_EVT_GET_COMP_ID(&(evtp)->evt)
#define RLS_SET_COMP_NAME(evtp, value) \
        RASEVT_EVT_SET_COMP_NAME(&(evtp)->evt, value)
#define RLS_GET_COMP_NAME(evtp)    RASEVT_EVT_GET_COMP_NAME(&(evtp)->evt)
#define RLS_SET_FLAGS(evtp, value) \
        RASEVT_EVT_SET_FLAGS(&(evtp)->evt, value)
#define RLS_GET_FLAGS(evtp)		RASEVT_EVT_GET_FLAGS(&(evtp)->evt)
#define RLS_SET_OBJID(evtp, value) \
        RASEVT_EVT_SET_OBJID(&(evtp)->evt, value)
#define RLS_GET_OBJID(evtp)		RASEVT_EVT_GET_OBJID(&(evtp)->evt)
#define RLS_SET_LINENUM(evtp, value) \
        RASEVT_EVT_SET_LINENUM(&(evtp)->evt, value)
#define RLS_GET_LINENUM(evtp)	RASEVT_EVT_GET_LINENUM(&(evtp)->evt)

#define RLS_SET_FILENAME(evtp, value) \
        RASEVT_EVT_SET_FILENAME(&(evtp)->evt, value)
#define RLS_GET_FILENAME(evtp) 	RASEVT_EVT_GET_FILENAME(&(evtp)->evt)
#define RLS_SET_FUNC(evtp, value) 	\
        RASEVT_EVT_SET_FUNC(&(evtp)->evt, value)

#define RLS_GET_FUNC(evtp) 		RASEVT_EVT_GET_FUNC(&(evtp)->evt)
#define RLS_SET_SRCTIMESEC(evtp, value) \
        RASEVT_EVT_SET_SRCTIMESEC(&(evtp)->evt, value)
#define RLS_GET_SRCTIMESEC(evtp)   RASEVT_EVT_GET_SRCTIMESEC(&(evtp)->evt)
#define RLS_SET_SRCTIMEUSEC(evtp, value) \
        RASEVT_EVT_SET_SRCTIMEUSEC(&(evtp)->evt, value)
#define RLS_GET_SRCTIMEUSEC(evtp) RASEVT_EVT_GET_SRCTIMEUSEC(&(evtp)->evt)

#define RLS_SET_TIMESEC(evtp, value) RASEVT_EVT_SET_TIMESEC(&(evtp)->evt, value)
#define RLS_GET_TIMESEC(evtp)	RASEVT_EVT_GET_TIMESEC(&(evtp)->evt)
#define RLS_SET_TIMEUSEC(evtp, value)\
        RASEVT_EVT_SET_TIMEUSEC(&(evtp)->evt, value)
#define RLS_GET_TIMEUSEC(evtp)	RASEVT_EVT_GET_TIMEUSEC(&(evtp)->evt)
#define RLS_SET_GSEQ(evtp, value) \
        RASEVT_EVT_SET_GSEQ(&(evtp)->evt, value)
#define RLS_GET_GSEQ(evtp)	RASEVT_EVT_GET_GSEQ(&(evtp)->evt)
#define RLS_SET_ESEQ(evtp, value)	\
        RASEVT_EVT_SET_ESEQ(&(evtp)->evt, value)
#define RLS_GET_ESEQ(evtp)	RASEVT_EVT_GET_ESEQ(&(evtp)->evt)
#define RLS_SET_IQDROP(evtp, value) \
        RASEVT_EVT_SET_IQDROP(&(evtp)->evt, value)
#define RLS_ADD_IQDROP(evtp, value) \
        RASEVT_EVT_ADD_IQDROP(&(evtp)->evt, value)
#define RLS_GET_IQDROP(evtp)	RASEVT_EVT_GET_IQDROP(&(evtp)->evt)
#define RLS_SET_RQDROP(evtp, value) \
        RASEVT_EVT_SET_RQDROP(&(evtp)->evt, value)
#define RLS_ADD_RQDROP(evtp, value)\
        RASEVT_EVT_ADD_RQDROP(&(evtp)->evt, value)
#define RLS_GET_RQDROP(evtp)	RASEVT_EVT_GET_RQDROP(&(evtp)->evt)
#define RLS_GET_MSG_ID(evtp)	RASEVT_EVT_GET_MSG_ID(&(evtp)->evt)
#define RLS_SET_MSG_ID(evtp)	RASEVT_EVT_SET_MSG_ID(&(evtp)->evt)
#define RLS_ARG_COMP(evtsrc, evtdest) \
        RASEVT_EVT_ARG_COMP((&(evtsrc)->evt), (&(evtdesc)->evt))
#define RLS_SET_ARG_OFFSET_RESET(evtp) \
        RASEVT_EVT_SET_ARG_OFFSET_RESET(&(evtp)->evt)
#define RLS_SET_ARG_OFFSET(evtp, idx, value) \
        RASEVT_EVT_SET_ARG_OFFSET(&(evtp)->evt, idx, value)
#define RLS_GET_ARG_OFFSET(evtp, idx) \
        RASEVT_EVT_GET_ARG_OFFSET(&(evtp)->evt, idx)
#define RLS_SET_ARG_RESET(evtp) \
        RASEVT_EVT_SET_ARG_RESET(&(evtp)->evt)
#define RLS_SET_ARG_STR(evtp, offset, ptr, size) \
        RASEVT_EVT_SET_ARG_STR(&(evtp)->evt, offset, ptr, size)
#define RLS_SET_ARG_4(evtp, offset, value) \
        RASEVT_EVT_SET_ARG_4(&(evtp)->evt, offset, value)
#define RLS_SET_ARG_8(evtp, offset, value) \
        RASEVT_EVT_SET_ARG_8(&(evtp)->evt, offset, value)
#define RLS_GET_ARG_STR(evtp, offset) \
        RASEVT_EVT_GET_ARG_STR(&(evtp)->evt, offset)
#define RLS_GET_ARG_4(evtp, offset) \
        RASEVT_EVT_GET_ARG_4(&(evtp)->evt, offset)
#define RLS_GET_ARG_8(evtp, offset) \
        RASEVT_EVT_GET_ARG_8(&(evtp)->evt, offset)
#define RLS_SET_MSGDEF(evtp, md) \
        RASEVT_EVT_SET_MSGDEF(&(evtp)->evt, md)
#define RLS_GET_MSGDEF(evtp) \
        RASEVT_EVT_GET_MSGDEF(&(evtp)->evt)
#define RLS_GET_ARG_NUM(evtp) \
        RLMD_GET_ARG_NUM(RASEVT_EVT_GET_MSGDEF(&(evtp)->evt))
#define RLS_GET_ARG_TYPE(evtp, idx)\
        RLMD_GET_ARG_TYPE(RASEVT_EVT_GET_MSGDEF(&(evtp)->evt), idx)
#define RLS_GET_SEVERITY(evtp) \
        RLMD_GET_SEVERITY(RASEVT_EVT_GET_MSGDEF(&(evtp)->evt))
#define RLS_GET_MSG_ID_STRING(evtp) \
        RLMD_GET_MSG_ID_STRING(RASEVT_EVT_GET_MSGDEF(&(evtp)->evt))
#define RLS_GET_CATNAME(evtp) \
         RLMD_GET_CATNAME(RASEVT_EVT_GET_MSGDEF(&(evtp)->evt))

#define RASEVT_USERNAME_LEN	41	/* 40 +1 */
#define RASEVT_IPADDR_LEN	64		
#define RASEVT_STR_LEN		16
#define RASEVT_ROLE_LEN		20	
#define RASEVT_AD_LEN		65	/* 64 +1 */
#define RASEVT_EVT_CLASS_STR_LEN  33	/* 32 +1 */

typedef struct {
/* header section */
	rasevt_hdr_t hdr;
	char username[RASEVT_USERNAME_LEN+1];
	char ipaddr[RASEVT_IPADDR_LEN];
	char interface[RASEVT_STR_LEN+1];
	char ad_number[RASEVT_AD_LEN+1];
	char role[RASEVT_ROLE_LEN+1];
	char app_name[RASEVT_STR_LEN+1];
} rasevt_audit_t;

/* event object structure */
typedef struct {
	/* header section */
	rasevt_hdr_t hdr;

	/* generic event info */
	rasevt_evt_t    evt;

	/* Audit specific information */
	rasevt_audit_t 	audit;
} rasevt_t;

typedef struct raslog_filter {
    int severity;   /* severity level and above to keep, everything else */
            /* is filtered out */
    int type; /* RASLOG_ALL_MSGS, RASLOG_INT_MSGS or RASLOG_EXT_MSGS */
    int sw_instance;/* 0, 1 or -1 (all) */
#ifndef __DCM_COMPILATION__
    u_int ref_class[REF_CLASS_SIZE];
#endif
    int attributes; /* log, audit, etc. */
} raslog_filter_t;

typedef struct raslog_return_msgs {
    int msg_num;
    rls_msg_t msgs[RASLOG_MSGS_PER_TYPE * RASLOG_NUM_TYPES * RASLOG_MAX_APP_TYPE];
} raslog_return_msgs_t;

/* TBD: Make dummy Data
#define MAX_EVT_ENTRIES 4
evt_t evt[MAX_EVT_ENTRIES] = {
    { 480, "SNMP-1007", "2011/01/28-23:39:53",
      logical_switch__t3_informational, 1,
      "The last fabric change happened at: Fri Jan 28 23:39:50 2011",
      logical_switch__t4_unknown, "sw0" },

    { 481, "HIL-1206", "2011/01/29-23:39:53",
      logical_switch__t3_error, 1,
      "Fan 2 sensor 1 , speed (1325 RPM) below threshold.",
      logical_switch__t4_unknown, "VDX6720-24" },

    { 482, "EM-1041", "2011/01/30-23:40:53",
      logical_switch__t3_warning, 1,
      "Sensor values for Fan 2: sensor 1 speed = 1325 RPM sensor 2 speed = 3839RPM sensor 3 speed = 3839 RPM    .",
      logical_switch__t4_unknown, "VDX6720-24" },
    { 483, "SEC-1203", "2011/01/26-23:28:55",
      logical_switch__t3_informational, 2,
      "Login information: Login successful via TELNET/SSH/RSH. IP Addr: 10.70.4.105",
      logical_switch__t4_unknown, "sw0" }
};

raslog_return_msgs_t evt = {
    MAX_EVT_ENTRIES,
    {
       { {'1', '100', 64},
         { {'1', '100', 64}, 1, 1, 1, "SNMP", 0x1, 1007, "", "", 1, 6666666, 999999, 5555555, 88888888, 480, 480, "", "", {} }, "sw0" },

       {               },
       {},
       {}
    }
};
*/

static inline int raslog_msg(rasevt_t *new_msg)
{
	return (0);
}

static inline int raslogm_msg2str(rls_msg_t *msg, char *buffer, int size)
{
	return (0);
}

static inline int raslogm_get_log(raslog_return_msgs_t *dest, raslog_filter_t filter)
{
	return (0);
}
static inline int raslogm_get_log_app(raslog_return_msgs_t *dest, raslog_filter_t filter,
		int app_index)
{
	return (0);
}

static inline int raslogm_init(void)
{
	return (0);
}

static inline int raslogm_set_swname(int instance, char *pswname)
{
	return (0);
}

#ifdef __cplusplus
}
#endif

#endif
