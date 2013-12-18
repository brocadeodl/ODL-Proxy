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

        int SQ_opendb1(const char *dbfile, sqlite3 **sqlite, int pagesize, int maxpages, int fk_enable);
        int SQ_closedb1(sqlite3 **sqlite);
        int SQ_getRow1 (sqlite3 *sqlite, const char *table, const char *cols, const char *where, const char *order,  char **item, int n, int index, const char *groupby);
		int SQ_setRow1(sqlite3 *sqlite, const char *table, const char *cols, const char *vals);
		int SQ_deleteRow1(sqlite3 *sqlite, const char *table, const char *where);
		int SQ_updateRow1(sqlite3 *sqlite, const char *table, const char *setColVals, const char *where);
		int SQ_deleteAllEntries(sqlite3 *sqlite, const char *table);
		void resetQuery();

}
