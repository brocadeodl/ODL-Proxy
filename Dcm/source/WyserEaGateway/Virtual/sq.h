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

#include <sqlite3.h>

extern "C" {
        
        #define NO_INDEX_IS_SET -2
        #define PAGE_SIZE 500

        int SQ_opendb(const char *dbfile, sqlite3 **sqlite, int pagesize, int maxpages);
        int SQ_closedb();
        int SQ_init();
        int SQ_closeDbnew();
        int SQ_initDbnew();
        int SQ_getTitle(char *table, char *cols, char *where, char **item, int n);
        int SQ_getRow (char *table, char *cols, char *where, char **item, int n, int index);
		int SQ_setRow(char *table, char *cols, char *vals);
		int SQ_deleteRow(char *table, char *col, char *val);
		int SQ_setRowDbnew(char *table, char *cols, char *vals);
		int SQ_updateRow(char *table, char **cols, char **vals, int ncols, char *whereCol, char *whereVal);
		int SQ_updateRowDbnew(char *table, char **cols, char **vals, int ncols, char *whereCol, char *whereVal);
        int SQ_updateHostRecord(char *host_mor, char *vmnic, char *iface, int now);
}
