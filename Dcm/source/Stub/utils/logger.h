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
 *    Copyright (c) 2001-2010 Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Module name: logger.h
 *
 *    Description: Library implementing logger utility functions
 *
 *
 */

#ifndef __LOG_MOD_H__
#define __LOG_MOD_H__

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_DEF_MOD			(0)		/* default module id used by Logger */
#define LOG_FW_HIST			(1) 	/* Firmware version history log	*/
#define LOG_INCR_FWDL_PKG_HIST  (2) /* Incremental fwdl package history log */
#define LOG_MAX_ID			(LOG_INCR_FWDL_PKG_HIST) /* Replace this with new ID */

#ifdef __cplusplus
}
#endif
#endif // __LOG_MOD_H__ 

#ifndef __LOGGER_H__
#define __LOGGER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <strings.h>

#define MAX_LOG_FNAME       64
#define LOG_TIME_LEN        34
#define LOG_MSG_LEN         256

/* Logger error codes */

#define LOG_SUCCESS             (0)
#define LOG_ERROR               (-1)


/* Logger flags */
#define LOG_NOFLAG              (0)
#define LOG_SYNC_MNT_FLAG       (1)

#define SWNAME_SIZE 10

/* Structure at the beginning of the status file */
typedef struct {
    int count;
    int magic;
    unsigned int flag;
    int modid;
    int reserved2;
} logger_hdr_t;

/*  Structure for storing log record */
typedef struct  {
    pid_t           pid;      /* Pid */
    int             slotno;                 /* slot number */
    char            swname[SWNAME_SIZE];    /* switch name */
    char            time[LOG_TIME_LEN]; /* timestamp */
    char            msg[LOG_MSG_LEN];   /* msg */
} logger_record_t;

#define HFHDR_SIZE          sizeof (logger_hdr_t)
#define LOG_REC_SIZE        sizeof (logger_record_t)
#define FUP_MISC_SWBDSHIFT  16

/* Prototypes */
static int logger_init(int modid, unsigned int flag)
{
    return (1);
}

static int  logger_exit(int log_handle)
{
    return (LOG_SUCCESS);
}

static int  logger_fsync(int log_handle)
{   
    return (LOG_SUCCESS);
}

static int logger_get_nth_record (int log_handle, int rec_num, logger_record_t* rec)
{
    rec->pid = rec_num;
    rec->slotno = rec_num;
    strncpy(rec->swname, "sw0", sizeof (rec->swname));
    strncpy(rec->time, "20:55:22 May 04, 20112", sizeof (rec->time));
    strncpy(rec->msg, "v3.0.0_nos_simulator_110219_0500", sizeof (rec->msg)); 

    return (LOG_SUCCESS);
}

#ifdef __cplusplus
}
#endif

#endif // __LOGGER_H__    

