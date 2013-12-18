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
 * FileUtils.cpp
 *
 *  Created on: Jun 19, 2012
 *      Author: rmadapur
 */

#include "discover/utils/FileUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include <stddef.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <set>

using namespace std;

namespace DcmNs {

    FileUtils::FileUtils() {


    }

    FileUtils::~FileUtils() {

    }

    int FileUtils::fileexists(const char *fname) {
        struct stat sb;
        int ret;
        if ((ret = stat(fname, &sb)) == 0) {
            if (sb.st_mode & S_IFREG)
                return 1;
        }
        return 0;
    }

    int FileUtils::copyfile(const char *src, const char *dst) {
        FILE *ifp = NULL, *ofp = NULL;
        char buf[128];
        int n, m;
        ifp = fopen(src, "ro");
        ofp = fopen(dst, "w+");
        if (ifp == NULL || ofp == NULL) {
            if (ofp)
                fclose(ofp);
            if (ifp)
                fclose(ifp);
            return -1;
        }
        while ((n = fread(buf, 1, 128, ifp)) != 0) {
            if ((m = fwrite(buf, 1, n, ofp)) != n) {
                if (ofp) {
                    fclose(ofp);
                }
                if (ifp) {
                    fclose(ifp);
                }
                return -1;
            }
        }
        fclose(ifp);
        fclose(ofp);
        return 0;
    }

    int FileUtils::mkDir(const char *dirName){
        return mkdir(dirName,0777);
    }
    int FileUtils::remove(const char *fileName){
            return unlink(fileName);
    }

    string FileUtils::getDBDirectoryLocation(){
        if(FileUtils::isvCenterInManagementCluster()){
            return MC_VCENTER_DIR;
        }
        return VCENTER_DIR;
    }

    string FileUtils::getDBLocation(string  vcenter){
         string db = DB(vcenter);
         //ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
         if(FileUtils::isvCenterInManagementCluster()){
             db = MC_DB(vcenter);
         }
         return db;
    }

    string FileUtils::getPersistentDBLocation(string vcenter)
    {
            string db = MC_DB(vcenter);
            return db;
    }

    string FileUtils::getDBNewLocation(string  vcenter){

         string db = DB_NEW(vcenter);
                  //ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
                  if(FileUtils::isvCenterInManagementCluster()){
                      db = MC_DB_NEW(vcenter);
                  }
         return db;

    }

    bool FileUtils:: isvCenterInManagementCluster(){
            ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
            ResourceId distributionMode = DcmToolKit::getDistributionMode ();
            return (vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) &&(distributionMode == DCM_CCM);
    }

    bool FileUtils:: isvCenterInFabricCluster(){
        ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
        ResourceId distributionMode = DcmToolKit::getDistributionMode ();
        return (vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) &&(distributionMode ==CCM_ONLY);
    }

    bool FileUtils:: isvCenterInStandalone(){
        ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
        //ResourceId distributionMode = DcmToolKit::getDistributionMode ();
        return (vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE);
    }
    int FileUtils::fetchvCenterNamesFromFileSystem(string dirName,
            std::set<string> &vcenterList) {
        DIR* dir;
        dirent* pdir;

        dir = opendir(dirName.c_str()); // open current directory
        if (dir == NULL) {
            return -1;
        }

        while ((pdir = readdir(dir))!=NULL) {
            string name = pdir->d_name;
            size_t found = name.find(".db");
            if (found != std::string::npos) {
                string substr = name.substr(0, found);
                vcenterList.insert(substr);
            }
        }

        closedir(dir);

        return 0;
    }

    void FileUtils::deleteVcenterPersistentDBFiles(string vcenterName){
        string db = MC_DB(vcenterName);
        if(fileexists(db.c_str())){
            remove(db.c_str());
        }
        string ddNew = MC_DB_NEW(vcenterName);
        if (fileexists(ddNew.c_str())) {
            remove(ddNew.c_str());
        }
        //mnt files
        string mnt_db = MNT_MC_DB(vcenterName);
        if (fileexists(mnt_db.c_str())) {
            remove(mnt_db.c_str());
        }
        string mnt_ddNew = MNT_MC_DB_NEW(vcenterName);
        if (fileexists(mnt_ddNew.c_str())) {
            remove(mnt_ddNew.c_str());
        }

    }
    void FileUtils::deleteVcenterTemporaryDBFiles(string vcenterName){
            string db = DB(vcenterName);
            if(fileexists(db.c_str())){
                remove(db.c_str());
            }
            string ddNew = DB_NEW(vcenterName);
            if (fileexists(ddNew.c_str())) {
                remove(ddNew.c_str());
            }

        }

    int FileUtils:: removeDBFiles(string Dir) {
        DIR* dir;
        dirent* pdir;

        dir = opendir(Dir.c_str()); // open current directory
        if(dir == NULL) {
            return -1;
        }

        while (1) {
            pdir = readdir(dir);
            if (pdir == NULL) {
                break;
            }
            string name = Dir.c_str();
            name += pdir->d_name;
            remove(name.c_str());
        }

        closedir(dir);

        return 0;
    }
} /* namespace DcmNs */
