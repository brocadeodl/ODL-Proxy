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
 * FileUtils.h
 *
 *  Created on: Jun 19, 2012
 *      Author: rmadapur
 */

#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#ifdef _X86_COMPILE_
#define DBFILE_ORIG     ((const char *) "/vobs/projects/springboard/fabos/src/vcs/dcm/source/VirtualDb/dc.db")
#else
#define DBFILE_ORIG     ((const char *) "/fabos/webtools/htdocs/ws/config/dc.db")
#endif
#include <string>
#include <set>

#define VCENTER_DIR  "/tmp/vCenter/"
#define MC_VCENTER_DIR  "/var/tmp/vCenter/"
#define MNT_MSC_VCENTER_DIR "/mnt/tmp/vcenter"
#define DB(vcenter) VCENTER_DIR + vcenter + ".db"
#define MC_DB(vcenter) MC_VCENTER_DIR + vcenter + ".db"
#define MNT_MC_DB(vcenter) MNT_MSC_VCENTER_DIR + vcenter + ".db"
#define DB_NEW(vcenter) VCENTER_DIR + vcenter + ".db.new"
#define MC_DB_NEW(vcenter) MC_VCENTER_DIR + vcenter + ".db.new"
#define MNT_MC_DB_NEW(vcenter) MNT_MSC_VCENTER_DIR + vcenter + ".db.new"
#define DB_MINI_NEW(vcenter) VCENTER_DIR + vcenter + ".db.mini.new"

using namespace std;
namespace DcmNs {


    class FileUtils {
        public:
            FileUtils();
            virtual ~FileUtils();
           static  int copyfile(const char *src,const char *dst);
           static int fileexists(const char *fname);
           static int mkDir(const char *dirName);
           static int remove(const char *fileName);
           static string getDBLocation(string vcenter);
           static string getDBNewLocation(string vcenter);
		   static string getPersistentDBLocation(string vcenter);


           static bool isvCenterInFabricCluster();
           static bool isvCenterInManagementCluster();
           static bool isvCenterInStandalone();
           static int fetchvCenterNamesFromFileSystem(string dirName,std::set<string> &vcenterList);
           static void  deleteVcenterPersistentDBFiles(string vcenterName);
           static void deleteVcenterTemporaryDBFiles(string vcenterName);
           static string getDBDirectoryLocation();
           static int removeDBFiles(string Dir);
    };

} /* namespace DcmNs */
#endif /* FILEUTILS_H_ */
