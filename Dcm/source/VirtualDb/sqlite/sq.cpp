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
#include "discover/vsphere/Debug.h"

#include "sq.h"

#include "Framework/Utils/TraceUtils.h"
#include "DcmCore/DcmToolKit.h"

#define SQL_QUERY_LEN_MAX   256
#define SQL_BUF_MAX_SIZE    256
#define SQLITE_DBCONFIG_ENABLE_FKEY     1002

static __thread sqlite3_stmt *pstmt_get    =    NULL;
static __thread  int          bFinished_get =    1;

int SQ_opendb1 (const char *dbfile, sqlite3 **sqlite, int pagesize, int maxpages, int fk_enable)
{
//#ifndef _X86_COMPILE_
        char    *errmsg = NULL;
        int     sqlerr = 0;
        sqlite3 *sq = NULL;
        char    pragma[256] = {0};
        if (dbfile == NULL) return 0;
        if ((sqlerr = sqlite3_open(dbfile, &sq) != SQLITE_OK)) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s:%s", dbfile, sqlite3_errmsg(sq));
                sqlite3_close(sq);
                return -1;
        }
        memset(pragma, 0, sizeof(pragma));
        snprintf(pragma, sizeof(pragma), "PRAGMA page_size=%d", 8192);
        if ((sqlerr = sqlite3_exec(sq, pragma, NULL, 0, &errmsg)) != SQLITE_OK) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Error in pragma statement %s \n", errmsg);
                sqlite3_free(errmsg);
                return -1;
        }
        if ((sqlerr = sqlite3_exec(sq, "PRAGMA synchronous=OFF", NULL, 0, &errmsg)) != SQLITE_OK) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Error in pragma statement %s \n", errmsg);
                sqlite3_free(errmsg);
                return -1;
        }
        if ((sqlerr = sqlite3_exec(sq, "PRAGMA temp_store=MEMORY", NULL, 0, &errmsg)) != SQLITE_OK) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Error in pragma statement %s \n", errmsg);
                sqlite3_free(errmsg);
                return -1;
        }
        if ((sqlerr = sqlite3_exec(sq, "PRAGMA cache_size=8000", NULL, 0, &errmsg)) != SQLITE_OK) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Error in pragma statement %s \n", errmsg);
                sqlite3_free(errmsg);
                return -1;
        }
        //enable/disable foreign keys
/*        int ret = 0;
        sqlite3_db_config(sq, SQLITE_DBCONFIG_ENABLE_FKEY, fk_enable, &ret);
        printf("fk is %d", ret);*/

        *sqlite = sq;
//#endif
        return 1;
}

void resetQuery()
{
	pstmt_get    =    NULL;
	bFinished_get =    1;
}

int SQ_closedb1(sqlite3 **sqlite)
{
//#ifndef _X86_COMPILE_
    if (*sqlite != NULL) {
    	sqlite3_close(*sqlite);
    	*sqlite = NULL;
    }
//#endif        
	pstmt_get    =    NULL;
	bFinished_get =    1;
    return 1;
}


/* Every call to this function returns one row of the query.
 * The first time this function is called for a query, the
 * query will be evaluated by calling sqlite3_prepare. The
 * rest of the calls return one row of the result till the
 * last row is returned.
 */
int SQ_getRow1(sqlite3 *sqlite, const char *table, const char *cols, const char *where, const char *order, char **items, int n, int index, const char *groupby)
{
//#ifndef _X86_COMPILE_
        int                 rc = 0, col = 0, retry = 0;
        char                q[SQL_QUERY_LEN_MAX] = {0};
        const char          *colval = NULL;
        int                 colcnt = 0;
        //printf("SQ getRow..\n");

            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"bFinished is %d", bFinished_get);
        if (bFinished_get) {
            // if we are done with the previous query, evaluate the new one
            snprintf(q, SQL_QUERY_LEN_MAX, "SELECT %s FROM %s ", cols, table);
            if (where != NULL) {
                char where_clause[SQL_QUERY_LEN_MAX] = {0};
                snprintf(where_clause, SQL_QUERY_LEN_MAX, "WHERE %s ", where);
                strncat(q, where_clause, SQL_QUERY_LEN_MAX-strlen(q));
            }

            if (order != NULL) {
                char order_clause[SQL_QUERY_LEN_MAX] = {0};
                snprintf(order_clause, SQL_QUERY_LEN_MAX, "ORDER BY %s ASC ", order);
                strncat(q, order_clause, SQL_QUERY_LEN_MAX-strlen(q));
            }

            if (groupby!= NULL) {
                char groupby_clause[SQL_QUERY_LEN_MAX] = {0};
                snprintf(groupby_clause, SQL_QUERY_LEN_MAX, "GROUP BY %s ", groupby);
                strncat(q, groupby_clause, SQL_QUERY_LEN_MAX-strlen(q));
            }

            if (index != NO_INDEX_IS_SET) {
                char limit_clause[SQL_QUERY_LEN_MAX] = {0};
                snprintf(limit_clause, SQL_QUERY_LEN_MAX, " LIMIT %d OFFSET %d", PAGE_SIZE, index);
                strncat(q, limit_clause, SQL_QUERY_LEN_MAX-strlen(q));
            }


            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"This is the query %s",q);

            if ((rc = sqlite3_prepare(sqlite, q, -1, &pstmt_get, 0)) != SQLITE_OK) {
                //VIC_PLUG_LOG(VIC_TRACE_INFO,"Error %s \n", sqlite3_errmsg(sqlite));
                //VIC_PLUG_LOG(VIC_TRACE_INFO,"SQL : %s \n", q);
                //VIC_PLUG_LOG(VIC_TRACE_INFO,"Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return 0;
            }
            bFinished_get = 0;
        }
//        printf("next step \n");
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","in next step ");

        // return the results one row at a time
        rc = sqlite3_step(pstmt_get);
        col = 0;
        switch (rc) {
        case SQLITE_DONE:
            bFinished_get = 1;
            break;
        case SQLITE_ROW:
            colcnt = sqlite3_column_count(pstmt_get);
            while (col < colcnt && col < n) {
                colval = (const char *)sqlite3_column_text(pstmt_get, col);
                if (colval == NULL)
                        colval = "-";
//                printf("colval=%s\n", colval);
                items[col] = strdup(colval);
//                printf("colval is %s\n", colval);
                col++;
            }
            break;
        case SQLITE_BUSY:
            if (retry++ >= 4) {
                    bFinished_get = 1;
                    //sleep(1);
                    break;
            }
            break;
        default:
            bFinished_get = 1;
            break;
        }

        //VIC_PLUG_LOG(VIC_TRACE_INFO," 1 bFinished is %d", bFinished_get);
        if (bFinished_get) {

            // all the rows for the current query are returned
            sqlite3_finalize(pstmt_get);
//            printf("query is done\n");
            return 0;
        }   
 
        // there are still more rows to return
//        printf("query is not done\n");
        return 1;
//#else
//        return 0;
//#endif
}

int SQ_setRow1(sqlite3 *sqlite, const char *table, const char *cols, const char *vals)
{
//#ifndef _X86_COMPILE_
        int          rc = 0;
		int retry = 0;
        sqlite3_stmt *pstmt    =    NULL;
        int          bFinished =    0;
        char         q[SQL_QUERY_LEN_MAX] = {0};

  // start sqlite transaction block
  sqlite3_exec(sqlite, "BEGIN", 0, 0, 0);
/*
  // execute all the sql statements
  for(int i = 0; i < 4; i++)
  {
    int rc = sqlite3_exec(db, pSQL[i], callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);

      // rollback all update/insert to sqlite
      sqlite3_exec(db, "ROLLBACK", 0, 0, 0);

      break; // break the loop if error occur
    }
  }

*/
		snprintf(q, SQL_QUERY_LEN_MAX, "INSERT INTO %s (%s) VALUES (%s)", table, cols, vals);

        if ((rc = sqlite3_prepare(sqlite, q, -1, &pstmt, 0)) != SQLITE_OK) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Error %s \n", sqlite3_errmsg(sqlite));
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return -1;
        }
        while (!bFinished) {
                rc = sqlite3_step(pstmt);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"sqlite step : rc %d", rc);
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

  // commit all to sqlite
  sqlite3_exec(sqlite, "COMMIT", 0, 0, 0);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"This is the query %s",q);
        sqlite3_finalize(pstmt);

        return (rc);
//#else
        return 0;
//#endif
}

int SQ_updateRow1(sqlite3 *sqlite, const char *table, const char *setColVal, const char *where)
{
//#ifndef _X86_COMPILE_
        int          rc,	retry = 0;
        sqlite3_stmt *pstmt    =    NULL;
        int          bFinished =    0;
//        int          bFinished =    0,	i = 1;
        char         q[SQL_QUERY_LEN_MAX] = {0};
		//int comm = sqlite3_get_autocommit(sqlite);
                //VIC_PLUG_LOG(VIC_TRACE_INFO,"Auto Commit commit %d\n", comm);

  sqlite3_exec(sqlite, "BEGIN", 0, 0, 0);
		snprintf(q, SQL_QUERY_LEN_MAX, "UPDATE %s SET %s WHERE %s", table, setColVal, where);

        if ((rc = sqlite3_prepare(sqlite, q, -1, &pstmt, 0)) != SQLITE_OK) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Error %s \n", sqlite3_errmsg(sqlite));
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Prepare Error: %s rc %d\n", __FUNCTION__, rc);
                return (rc);
        }
        while (!bFinished) {
                rc = sqlite3_step(pstmt);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"sqlite step : rc %d", rc);
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

  sqlite3_exec(sqlite, "COMMIT", 0, 0, 0);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"This is the query %s",q);
        sqlite3_finalize(pstmt);
        return rc;
}

int SQ_deleteRow1(sqlite3 *sqlite, const char *table, const char *where)
{
//#ifndef _X86_COMPILE_
        int          rc = 0;
		int retry = 0;
        sqlite3_stmt *pstmt    =    NULL;
        int          bFinished =    0;
        char         q[SQL_QUERY_LEN_MAX] = {0};

  sqlite3_exec(sqlite, "BEGIN", 0, 0, 0);
		snprintf(q, SQL_QUERY_LEN_MAX, "DELETE FROM %s WHERE %s", table, where);

        if ((rc = sqlite3_prepare(sqlite, q, -1, &pstmt, 0)) != SQLITE_OK) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Error %s \n", sqlite3_errmsg(sqlite));
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Prepare Error: %s rc %d\n", __FUNCTION__, rc);
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
    
  sqlite3_exec(sqlite, "COMMIT", 0, 0, 0);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"This is the query %s",q);
        sqlite3_finalize(pstmt);

        return (rc);
}
int SQ_deleteAllEntries(sqlite3 *sqlite, const char *table)
{
//#ifndef _X86_COMPILE_
        int          rc = 0;
                int retry = 0;
        sqlite3_stmt *pstmt    =    NULL;
        int          bFinished =    0;
        char         q[SQL_QUERY_LEN_MAX] = {0};
  sqlite3_exec(sqlite, "BEGIN", 0, 0, 0);

                snprintf(q, SQL_QUERY_LEN_MAX, "DELETE FROM %s", table);

        if ((rc = sqlite3_prepare(sqlite, q, -1, &pstmt, 0)) != SQLITE_OK) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Error %s \n", sqlite3_errmsg(sqlite));
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Prepare Error: %s rc %d\n", __FUNCTION__, rc);
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

  sqlite3_exec(sqlite, "COMMIT", 0, 0, 0);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"This is the query %s",q);
        sqlite3_finalize(pstmt);

        return (rc);
}
