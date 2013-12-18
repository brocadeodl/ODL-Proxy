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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "WyserEaGateway/Virtual/sq.h"

#include "Framework/Utils/TraceUtils.h"
#include "DcmCore/DcmToolKit.h"

#define SQL_QUERY_LEN_MAX   256
#define SQL_BUF_MAX_SIZE    256

int initialized_ = 0;

sqlite3 *sqlite_r = NULL;
sqlite3 *sqlite_dbnew_r = NULL;
const char * const dbfile_r = "/tmp/dc.db";
const char * const dbnewfile_r = "/tmp/dc.db.new";


int SQ_opendb(const char *dbfile, sqlite3 **sqlite, int pagesize, int maxpages)
{
#ifndef _X86_COMPILE_
        char    *errmsg = NULL;
        int     sqlerr = 0;
        sqlite3 *sq = NULL;
        char    pragma[256] = {0};
        if (dbfile == NULL) return 0;
        if ((sqlerr = sqlite3_open(dbfile, &sq) != SQLITE_OK)) {
                printf("%s:%s\n", dbfile, sqlite3_errmsg(sq));
                sqlite3_close(sq);
                return -1;
        }
        memset(pragma, 0, sizeof(pragma));
        snprintf(pragma, sizeof(pragma), "PRAGMA page_size=%d", pagesize);
        if ((sqlerr = sqlite3_exec(sq, pragma, NULL, 0, &errmsg)) != SQLITE_OK) {
                printf("Error in pragma statement %s \n", errmsg);
                sqlite3_free(errmsg);
                return -1;
        }
        if ((sqlerr = sqlite3_exec(sq, "PRAGMA synchronous=OFF", NULL, 0, &errmsg)) != SQLITE_OK) {
                printf("Error in pragma statement %s \n", errmsg);
                sqlite3_free(errmsg);
                return -1;
        }
        if ((sqlerr = sqlite3_exec(sq, "PRAGMA temp_store=MEMORY", NULL, 0, &errmsg)) != SQLITE_OK) {
                printf("Error in pragma statement %s \n", errmsg);
                sqlite3_free(errmsg);
                return -1;
        }
        if ((sqlerr = sqlite3_exec(sq, "PRAGMA cache_size=819200", NULL, 0, &errmsg)) != SQLITE_OK) {
                printf("Error in pragma statement %s \n", errmsg);
                sqlite3_free(errmsg);
                return -1;
        }
        *sqlite = sq;
#endif
        return 1;
}
int SQ_closedb()
{
#ifndef _X86_COMPILE_
        if (sqlite_r != NULL) {
                sqlite3_close(sqlite_r);
                sqlite_r = NULL;
        }
#endif
        return 1;
}
int SQ_init()
{
#ifndef _X86_COMPILE_
        int      ret = 0;
        SQ_closedb();
        if ((ret = SQ_opendb(dbfile_r, &sqlite_r, 32768, 10)) == -1) {
                perror(dbfile_r);
                SQ_closedb();
                return 0;
        }
        initialized_ = 1;
#endif
        return 1;
}

int SQ_closeDbnew()
{
#ifndef _X86_COMPILE_
        if (sqlite_dbnew_r != NULL) {
                sqlite3_close(sqlite_dbnew_r);
                sqlite_dbnew_r = NULL;
        }
#endif
        return 1;
}
int SQ_initDbnew()
{
#ifndef _X86_COMPILE_
        int      ret = 0;
        SQ_closeDbnew();
        if ((ret = SQ_opendb(dbnewfile_r, &sqlite_dbnew_r, 32768, 10)) == -1) {
                perror(dbnewfile_r);
                SQ_closeDbnew();
                return 0;
        }
        initialized_ = 1;
#endif
        return 1;
}

int SQ_getTitle(char *table, char *cols, char *where, char **item, int n)
{
#ifndef _X86_COMPILE_
        int          rc = 0, col = 0, colcnt = 0;
        sqlite3_stmt *pstmt    =    NULL;
        char         q[SQL_QUERY_LEN_MAX] = {0};
        const char   *colname = NULL;

        tracePrintf(TRACE_LEVEL_INFO, "%s:%d\n", __FUNCTION__, __LINE__);
        snprintf(q, SQL_QUERY_LEN_MAX, "SELECT %s FROM %s WHERE id = ?", cols, table);
        if ((rc = sqlite3_prepare(sqlite_r, q, -1, &pstmt, 0)) != SQLITE_OK) {
                printf("Error %s \n", sqlite3_errmsg(sqlite_r));
                printf("Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return 0;
        }
        rc = sqlite3_bind_text (pstmt, 1, "Title", -1, SQLITE_STATIC);
        rc = sqlite3_step(pstmt);
        if (rc == SQLITE_ROW) {
            colcnt = sqlite3_column_count(pstmt);
            col = 0;
            while (col < colcnt && col < n) {
                    colname = (const char *)sqlite3_column_text(pstmt, col); 
                    if (colname == NULL)
                            break;
                    if (!strcmp(colname, "Title")) {
                            col++;
                            continue;
                    }
                    if (!strcmp(colname, "Valid"))
                            break;
                    item[col] = strdup(colname);
                    col++;
                    tracePrintf(TRACE_LEVEL_INFO, "coltitle is %s\n", colname);
            }
        }
        sqlite3_finalize(pstmt);
        return col;
#else
        return 0;
#endif
}
/* Every call to this function returns one row of the query.
 * The first time this function is called for a query, the 
 * query will be evaluated by calling sqlite3_prepare. The
 * rest of the calls return one row of the result till the 
 * last row is returned.
 */
int SQ_getRow(char *table, char *cols, char *where, char **items, int n, int index)
{
#ifndef _X86_COMPILE_
        int                 rc = 0, col = 0, retry = 0;
        static sqlite3_stmt *pstmt    =    NULL;
        static int          bFinished =    1;
        char                q[SQL_QUERY_LEN_MAX] = {0};
        const char          *colval = NULL;
        int                 colcnt = 0;

        tracePrintf(TRACE_LEVEL_INFO, "%s:%d index is %d\n", __FUNCTION__, __LINE__, index);

        if (bFinished) {
            // if we are done with the previous query, evaluate the new one
            if (where != NULL) {
                snprintf(q, SQL_QUERY_LEN_MAX, "SELECT %s FROM %s WHERE %s AND id != 'Title' ORDER BY name ASC", cols, table, where);
            } else if (index != NO_INDEX_IS_SET) {
                snprintf(q, SQL_QUERY_LEN_MAX, "SELECT %s FROM %s WHERE id != 'Title' ORDER BY name ASC LIMIT %d OFFSET %d", cols, table, PAGE_SIZE, index); 
            } else {
                snprintf(q, SQL_QUERY_LEN_MAX, "SELECT %s FROM %s WHERE id != 'Title' ORDER BY name ASC", cols, table);
            }
            tracePrintf(TRACE_LEVEL_INFO, "This is the query %s\n", q);
            if ((rc = sqlite3_prepare(sqlite_r, q, -1, &pstmt, 0)) != SQLITE_OK) {
                printf("Error %s \n", sqlite3_errmsg(sqlite_r));
                printf("Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return 0;
            }
            bFinished = 0;
        }

        // return the results one row at a time
        rc = sqlite3_step(pstmt);
        col = 0;
        switch (rc) {
        case SQLITE_DONE:
            bFinished = 1;
            tracePrintf(TRACE_LEVEL_DEBUG, "SQLITE_DONE");
            break;
        case SQLITE_ROW:
            colcnt = sqlite3_column_count(pstmt);
            tracePrintf(TRACE_LEVEL_DEBUG, "colcnt=%d\n", colcnt);
            while (col < colcnt && col < n) {
                colval = (const char *)sqlite3_column_text(pstmt, col); 
                if (colval == NULL)
                        colval = "-";
                tracePrintf(TRACE_LEVEL_DEBUG, "colval=%s\n", colval);
                items[col] = strdup(colval);
                col++;
            }
            break;
        case SQLITE_BUSY:
            tracePrintf(TRACE_LEVEL_INFO, "SQLITE_BUSY");
            if (retry++ >= 4) {
                    bFinished = 1;
                    //sleep(1);
                    break;
            }
            break;
        default: 
            tracePrintf(TRACE_LEVEL_INFO, "default");
            bFinished = 1;
            break;
        }

        if (bFinished) {
            // all the rows for the current query are returned
            sqlite3_finalize(pstmt);
            tracePrintf(TRACE_LEVEL_INFO, "query done\n");
            return 0;
        }
        // there are still more rows to return
        tracePrintf(TRACE_LEVEL_DEBUG, "more rows to return\n");
        return 1;
#else
        return 0;
#endif
}

int SQ_updateHostRecord(char *host_mor, char *vmnic,
                     char *iface, int now)
{
#ifndef _X86_COMPILE_
        int  rc;
        char *errmsg = NULL;
        char *q      = NULL;

        asprintf(&q,
                 "UPDATE vmnics SET createTS='%d',iface='%s' WHERE host_id = '%s' AND name = '%s'",
                  now, iface, host_mor, vmnic);
        tracePrintf(TRACE_LEVEL_INFO, "q=%s\n", q);
        rc = sqlite3_exec(sqlite_r, q, NULL, NULL, &errmsg);
        if (0 != rc)
        {
            // TBDDCMOSS : Please handle this condition.
        }
        free(q);
        if (errmsg) {
            tracePrintf(TRACE_LEVEL_INFO, "Failed Error: %s\n", errmsg);
            sqlite3_free(errmsg);
            return 0;
        }
        return 1;
#else
        return 0;
#endif
}

int SQ_setRow(char *table, char *cols, char *vals)
{
#ifndef _X86_COMPILE_
        int          rc = 0;
		int retry = 0;
        sqlite3_stmt *pstmt    =    NULL;
        int          bFinished =    0;
        char         q[SQL_QUERY_LEN_MAX] = {0};
                
		snprintf(q, SQL_QUERY_LEN_MAX, "INSERT INTO %s (%s) VALUES (%s)", table, cols, vals);

        printf("%s\n", q);
        if ((rc = sqlite3_prepare(sqlite_r, q, -1, &pstmt, 0)) != SQLITE_OK) {
                printf("Error %s \n", sqlite3_errmsg(sqlite_r));
                printf("Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return -1;
        }
        while (!bFinished) { 
                rc = sqlite3_step(pstmt);
                switch (rc) {
                case SQLITE_DONE:
                        bFinished = 1;
                        break;
                case SQLITE_ROW:
                        bFinished = 1;
                        break;
                case SQLITE_BUSY:
                        if (retry++ >= 4) {
                                bFinished = 1;
                                //sleep(1);
                                break;
                        }
                        break;
                default: 
                        bFinished = 1;
                        break;
                }
        }

        sqlite3_finalize(pstmt);
		
        return (rc);
#else
        return 0;
#endif
}

int SQ_updateRow(char *table, char **cols, char **vals, int ncols, char *whereCol, char *whereVal)
{
#ifndef _X86_COMPILE_
        int          rc,	retry = 0;
        sqlite3_stmt *pstmt    =    NULL;
        int          bFinished =    0;
//        int          bFinished =    0,	i = 1;
        char         q[SQL_QUERY_LEN_MAX] = {0};
        char         setColVal[SQL_BUF_MAX_SIZE] = {0};

		char *col = *cols;
		char *val = *vals;

		if (ncols > 0)
			snprintf(setColVal, SQL_BUF_MAX_SIZE, "%s = '%s'", col++, val++);
/*		
		while (i++ < ncols)
			snprintf(setColVal, SQL_BUF_MAX_SIZE, "%s, %s = '%s'", setColVal, col++, val++);
*/              
		snprintf(q, SQL_QUERY_LEN_MAX, "UPDATE %s SET %s WHERE %s = '%s'", table, setColVal, whereCol, whereVal);

        printf("%s\n", q);
        if ((rc = sqlite3_prepare(sqlite_r, q, -1, &pstmt, 0)) != SQLITE_OK) {
                printf("Error %s \n", sqlite3_errmsg(sqlite_r));
                printf("Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return (rc);
        }
        while (!bFinished) { 
                rc = sqlite3_step(pstmt);
                switch (rc) {
                case SQLITE_DONE:
                        bFinished = 1;
                        break;
                case SQLITE_ROW:
                        bFinished = 1;
                        break;
                case SQLITE_BUSY:
                        if (retry++ >= 4) {
                                bFinished = 1;
                                //sleep(1);
                                break;
                        }
                        break;
                default: 
                        bFinished = 1;
                        break;
                }
        }

        sqlite3_finalize(pstmt);
        return rc;
#else
        return 0;
#endif
}

int SQ_setRowDbnew(char *table, char *cols, char *vals)
{
#ifndef _X86_COMPILE_
        int          rc = 0;
		int retry = 0;
        sqlite3_stmt *pstmt    =    NULL;
        int          bFinished =    0;
        char         q[SQL_QUERY_LEN_MAX] = {0};
                
		snprintf(q, SQL_QUERY_LEN_MAX, "INSERT INTO %s (%s) VALUES (%s)", table, cols, vals);

        printf("%s\n", q);
        if ((rc = sqlite3_prepare(sqlite_dbnew_r, q, -1, &pstmt, 0)) != SQLITE_OK) {
                printf("Error %s \n", sqlite3_errmsg(sqlite_dbnew_r));
                printf("Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return -1;
        }
        while (!bFinished) { 
                rc = sqlite3_step(pstmt);
                switch (rc) {
                case SQLITE_DONE:
                        bFinished = 1;
                        break;
                case SQLITE_ROW:
                        bFinished = 1;
                        break;
                case SQLITE_BUSY:
                        if (retry++ >= 4) {
                                bFinished = 1;
                                //sleep(1);
                                break;
                        }
                        break;
                default: 
                        bFinished = 1;
                        break;
                }
        }

        sqlite3_finalize(pstmt);
		
        return (rc);
#else
        return 0;
#endif
}

int SQ_updateRowDbnew(char *table, char **cols, char **vals, int ncols, char *whereCol, char *whereVal)
{
#ifndef _X86_COMPILE_
        int          rc = 0,	retry = 0;
        sqlite3_stmt *pstmt    =    NULL;
        int          bFinished =    0;
//        int          bFinished =    0,	i = 1;
        char         q[SQL_QUERY_LEN_MAX] = {0};
        char         setColVal[SQL_BUF_MAX_SIZE] = {0};

		char *col = *cols;
		char *val = *vals;

		if (ncols > 0)
			snprintf(setColVal, SQL_BUF_MAX_SIZE, "%s = '%s'", col++, val++);
/*		
		while (i++ < ncols)
			snprintf(setColVal, SQL_BUF_MAX_SIZE, "%s, %s = '%s'", setColVal, col++, val++);
*/              
		snprintf(q, SQL_QUERY_LEN_MAX, "UPDATE %s SET %s WHERE %s = '%s'", table, setColVal, whereCol, whereVal);

        printf("%s\n", q);
        if ((rc = sqlite3_prepare(sqlite_dbnew_r, q, -1, &pstmt, 0)) != SQLITE_OK) {
                printf("Error %s \n", sqlite3_errmsg(sqlite_dbnew_r));
                printf("Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return (rc);
        }
        while (!bFinished) { 
                rc = sqlite3_step(pstmt);
                switch (rc) {
                case SQLITE_DONE:
                        bFinished = 1;
                        break;
                case SQLITE_ROW:
                        bFinished = 1;
                        break;
                case SQLITE_BUSY:
                        if (retry++ >= 4) {
                                bFinished = 1;
                                //sleep(1);
                                break;
                        }
                        break;
                default: 
                        bFinished = 1;
                        break;
                }
        }

        sqlite3_finalize(pstmt);
        return rc;
#else
        return 0;
#endif
}

int SQ_deleteRow(char *table, char *col, char *val)
{
#ifndef _X86_COMPILE_
        int          rc = 0;
		int retry = 0;
        sqlite3_stmt *pstmt    =    NULL;
        int          bFinished =    0;
        char         q[SQL_QUERY_LEN_MAX] = {0};
                
		snprintf(q, SQL_QUERY_LEN_MAX, "DELETE FROM %s WHERE %s = '%s'", table, col, val);

        printf("%s\n", q);
        if ((rc = sqlite3_prepare(sqlite_dbnew_r, q, -1, &pstmt, 0)) != SQLITE_OK) {
                printf("Error %s \n", sqlite3_errmsg(sqlite_dbnew_r));
                printf("Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return -1;
        }
        while (!bFinished) { 
                rc = sqlite3_step(pstmt);
                switch (rc) {
                case SQLITE_DONE:
                        bFinished = 1;
                        break;
                case SQLITE_ROW:
                        bFinished = 1;
                        break;
                case SQLITE_BUSY:
                        if (retry++ >= 4) {
                                bFinished = 1;
                                //sleep(1);
                                break;
                        }
                        break;
                default: 
                        bFinished = 1;
                        break;
                }
        }

        sqlite3_finalize(pstmt);
		
        return (rc);
#else
        return 0;
#endif
}
