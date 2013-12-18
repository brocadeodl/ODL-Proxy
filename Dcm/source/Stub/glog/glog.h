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
 * Copyright (c) 2000-2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 *
 * glog.h
 *
 * Generic Logger definitions for users.
 */
#ifndef __GLOG_H__
#define __GLOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <glog/glog_modules.h>

/* Log file size (unit - Mbyte) */
#define GLOG_FILESIZE_MIN	1
#define GLOG_FILESIZE_MAX	4
#define GLOG_FILESIZE_DEFAULT	1

/* Log level */
#define GLOG_EMERG		0
#define GLOG_ALERT		1
#define GLOG_CRIT		2
#define GLOG_ERR		3
#define GLOG_WARNING		4
#define GLOG_NOTICE		5
#define GLOG_INFO		6
#define GLOG_DEBUG		7
#define GLOG_LEVEL_MASK	0xf
#define IS_VALID_LEVEL(x)	(((x) >= GLOG_EMERG) && ((x) <= GLOG_DEBUG))

/* Log control */
#define GLOG_LEVEL_ONLY		0x00
#define GLOG_ON			0x10
#define GLOG_OFF		0x20
#define GLOG_HALT		0x30
#define GLOG_RESUME		0x40
#define GLOG_CONTROL_MASK	0xf0

#define GLOG_EMERG_STR		"EMERG"
#define GLOG_ALERT_STR		"ALERT"
#define GLOG_CRIT_STR		"CRIT"
#define GLOG_ERR_STR		"ERROR"
#define GLOG_WARNING_STR	"WARNING"
#define GLOG_NOTICE_STR		"NOTICE"
#define GLOG_INFO_STR		"INFO"
#define GLOG_DEBUG_STR		"DEBUG"

/* Log file operation */
#define GLOG_O_READ		0
#define GLOG_O_CREAT		0100
#define GLOG_O_EXCL		0200
#define GLOG_O_TRUNC		01000

/* Destination */
#define GLOG_DEST_FILE		0x0001
#define GLOG_DEST_CONSOLE	0x0002
#define GLOG_DEST_TELNET	0x0004
#define GLOG_DEST_SYSLOG	0x0008

#define GLOG_DEST_OFF_SHIFT	8
#define GLOG_DEST_OFF_MASK	0xff00
#define GLOG_DEST_FILE_OFF	(GLOG_DEST_FILE << GLOG_DEST_OFF_SHIFT)
#define GLOG_DEST_CONSOLE_OFF	(GLOG_DEST_CONSOLE << GLOG_DEST_OFF_SHIFT)
#define GLOG_DEST_TELNET_OFF	(GLOG_DEST_TELNET << GLOG_DEST_OFF_SHIFT)
#define GLOG_DEST_SYSLOG_OFF	(GLOG_DEST_SYSLOG << GLOG_DEST_OFF_SHIFT)

#define IS_VALID_DEST(x) 	((x == GLOG_DEST_FILE)||\
				(x == GLOG_DEST_CONSOLE)||\
				(x == GLOG_DEST_TELNET)||\
				(x == GLOG_DEST_SYSLOG))

#define GLOG_DEST_DEBUG		(GLOG_DEST_CONSOLE)
#define GLOG_DEST_DEFAULT	(GLOG_DEST_FILE)
#define GLOG_DEST_ALL		(0x1f)

/* ignore argument */
#define GLOG_IGNORE		(-1)

/*
 * Generic logger handle structure
 */
typedef struct glog_handle {
	struct glog_handle	*hd_next;
} glog_handle_t;

/*
 * Generic logger record structure
 */

/* caller supplys timestamp (date) */
#define GLOG_RECORD_NO_TIMESTAMP	0
#define GLOG_RECORD_HAS_TIMESTAMP	1

/* record message attribute */
#define GLOG_RECORD_ATTR_TYPE_MASK	0x000f
#define GLOG_RECORD_ATTR_TYPE_TEXT	0x0001
#define GLOG_RECORD_ATTR_TYPE_BINARY	0x0002

#define GLOG_RECORD_ATTR_FRAG_MASK	0x00f0
#define GLOG_RECORD_ATTR_FRAG_NONE	0x0000
#define GLOG_RECORD_ATTR_FRAG_BEGIN	0x0010
#define GLOG_RECORD_ATTR_FRAG_DATA	0x0020
#define GLOG_RECORD_ATTR_FRAG_END	0x0040

#define GLOG_RECORD_ATTR_TYPE_TEXT_STR	"T"
#define GLOG_RECORD_ATTR_TYPE_BINARY_STR "B"
#define GLOG_RECORD_ATTR_FRAG_NONE_STR	"FN"
#define GLOG_RECORD_ATTR_FRAG_BEGIN_STR	"FB"
#define GLOG_RECORD_ATTR_FRAG_DATA_STR	"FD"
#define GLOG_RECORD_ATTR_FRAG_END_STR	"FE"

#define GLOG_RECORD_ATTR_DEFAULT \
	(GLOG_RECORD_ATTR_TYPE_TEXT|GLOG_RECORD_ATTR_FRAG_NONE)

#define ATTR_TYPE(attr) \
	(attr&GLOG_RECORD_ATTR_TYPE_MASK)
#define ATTR_FRAG(attr) \
	(attr&GLOG_RECORD_ATTR_FRAG_MASK)

#define GLOG_RECORD_MSG_LEN		256	/* record message length */
#define GLOG_RECORD_MSG_TOTAL		8192	/* total message */

#define GLOG_RECORD_HDR_LEN		128	/* header string */
#define GLOG_RECORD_MSGBUF_LEN		1024	/* processed msg buffer length */
#define GLOG_RECORD_SWITCH_LEN		32	/* switch name length */
#define GLOG_RECORD_TIMESTAMP_LEN	34	/* timestamep string */

typedef struct glog_record_header {
	pid_t	rh_pid;		/* application process id */
	time_t	rh_time;	/* time stamp */
	short	rh_level;	/* message level */
	short 	rh_attr;	/* attribute type */
	short	rh_len;		/* message length */
	short	rh_frag;	/* Frag tag */
	short	rh_frag_seq;	/* sequence ID */
	short	rh_frag_cnt;	/* number of records */
	short	rh_total;	/* total size */
	short 	rh_slot;	/* slot number */
	char	rh_switch[GLOG_RECORD_SWITCH_LEN]; /* switch name */
} glog_record_header_t;

typedef struct glog_record {
	glog_record_header_t	rc_header;
#define	rc_pid		rc_header.rh_pid
#define	rc_time		rc_header.rh_time
#define	rc_level	rc_header.rh_level
#define	rc_attr		rc_header.rh_attr
#define	rc_len		rc_header.rh_len
#define	rc_frag		rc_header.rh_frag
#define	rc_frag_seq	rc_header.rh_frag_seq
#define	rc_frag_cnt	rc_header.rh_frag_cnt
#define	rc_total	rc_header.rh_total
#define	rc_slot		rc_header.rh_slot
#define	rc_switch	rc_header.rh_switch
	char		rc_msg[GLOG_RECORD_MSG_LEN];
} glog_record_t;

/*
 * Generic logger log structure
 *
 * Handle returned from glog_openlog().
 * Used by APIs:
 * - glog_openlog()
 * - glog_closelog()
 * - glog_seeklog()
 * - glog_readlog()
 *
 * It maintains the offset of record in log file.  Users are able
 * to manipute the offset by calling glog_seeklog().  Calling
 * glog_readlog() returns the record index by current offset.
 */
#define GLOG_SEEK_SET		0
#define GLOG_SEEK_CUR		1
#define GLOG_SEEK_END		2
#define GLOG_SEEK_LAST_NREC	3

typedef struct glog_log {
	int		lg_modid;	/* modid */
	void            *lg_mem;        /* in-core copy */
	off_t		lg_current;	/* current offset, initialize to 0 */
	off_t		lg_start;	/* start offset in log file */
	off_t		lg_count;	/* count of records in log file */
	struct glog_log	*lg_next;	/* next object */
} glog_log_t;

/*
 * glog status
 */
typedef struct _glog_stat {
	struct stat	ls_stat;	/* log file status */
	int		ls_version;	/* glog version */
	int		ls_type;	/* glog type */
	int		ls_size;	/* max log file size */
	int		ls_count;	/* number of records in log file */
	int		ls_count_max;	/* max number of records in log file */
	int		ls_wrapped;	/* number of wrapped around */
} glog_stat_t;


/*
 * Definition of generic logger API
 */
static inline glog_handle_t*
glog_init(int modid, int oflags, int fsize)
{
    return ((glog_handle_t*)1);
}

static inline int
glog_exit(glog_handle_t *log_handle)
{
    return (0);
}

static inline void
glog(glog_handle_t *log_handle, int level, const char *fmt, ...)
{
    /* try to substitute traceprintf in x86 mode */
    switch (level) {
    case GLOG_EMERG:
        break;
    case GLOG_ALERT:
        break;
    case GLOG_CRIT:
        break;
    case GLOG_ERR:
        break;
    case GLOG_WARNING:
        break;
    case GLOG_NOTICE:
        break;
    case GLOG_INFO:
        break;
    case GLOG_DEBUG:
        break;
    default:
        break;
    }
}

#if 0
/* 4.1 */
glog_handle_t *glog_init(int modid, int oflags, int fsize);
int  glog_clear(glog_handle_t *log_handle);
int  glog_config(glog_handle_t *log_handle, int dest, int config);
int  glog_exit(glog_handle_t *log_handle);
int  glog_stat(int modid, glog_stat_t *buf);

/* 4.2 */
void glog(glog_handle_t *log_handle, int level, const char *fmt, ...);
void glog_write(glog_handle_t *log_handle, int level,
	int attribute, char *log_msg, size_t len);

/* 4.3 */
glog_log_t *glog_openlog(int modid, int *entry);
int glog_closelog(glog_log_t *logp);
glog_record_t  *glog_readlog(glog_log_t *logp, glog_record_t *recp);
int glog_seeklog(glog_log_t *logp, off_t offset, int whence);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __GLOG_H__ */
