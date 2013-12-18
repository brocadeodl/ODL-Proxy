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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef YANGMODULECOLLECTION_H
#define YANGMODULECOLLECTION_H

#include "Modeling/YANG/ObjectModel/YangModule.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

namespace WaveNs
{

class YangUserInterface;

class YangModuleCollection
{
    private :
    protected :
    public :
                              YangModuleCollection                     ();
                             ~YangModuleCollection                     ();

        bool                  isAKnownYangModuleByName                 (YangModule *pYangModule) const;
        bool                  isAKnownYangModuleByName                 (const string &yangModuleName) const;
        bool                  isAKnownYangModuleByPrefix               (YangModule *pYangModule) const;
        bool                  isAKnownYangModuleByPrefix               (const string &yangModulePrefix) const;
        void                  addYangModule                            (YangModule *pYangModule);
        void                  addYangModules                           (vector<YangModule *> yangModules);
        void                  loadModulesFromDirectory                 (const string &directoryPath);
        void                  loadModuleFromFile                       (const string &filePath);
        void                  loadModulesFromDirectoriesAndFiles       (const vector<string> &directoryPaths, const vector<string> &yinFileNames);
        void                  clearAllModules                          ();
        void                  removeAllModules                         ();

        void                  getAllKnownModules                       (vector<string> &allKnownYangModules);
        YangModule           *getYangModuleByName                      (const string &yangModuleName) const;
        YangModule           *getYangModuleByPrefix                    (const string &yangModulePrefix) const;

        YangModuleCollection *clone                                    () const;

        void                  transferAllModulesToUserInterface        (YangUserInterface *pYangUSerInterface);

        void                  computeUsageCountForGroupings            ();
        void                  computeUsageCountForGroupingsForProgrammingLanguages ();

        void                  inlineGroupingUsage                      ();
        void                  inlineGroupingUsageForRpcs               ();
        void                  inlineAugmentUsage                       ();

        void                  removeAllUnusedGroupings                 ();
        void                  removeAllGroupings                       ();

        void                  checkIntegrity                           () const;

        void                  printYinForQualifiedYangPath             (const string &qualifiedYangPath, FILE *pFile = NULL) const;
        void                  printYinForQualifiedYangPathToFile       (const string &qualifiedYangPath, const string &filePath) const;
        void                  printYinForPrefixQualifiedYangPath       (const string &prefixQualifiedYangPath, FILE *pFile = NULL) const;
        void                  printYinForPrefixQualifiedYangPathToFile (const string &prefixQualifiedYangPath, const string &filePath) const;

        void                  getTargetNodeDetails                     (const string &qualifiedYangPath, string &moduleName, string &targetNodePath) const;

        void                  printYinForAllModules                    (FILE *pFile) const;
        void                  printYinForAllModulesToFile              (const string &filePath) const;
        void                  printYinForModule                        (const string &moduleName, FILE *pFile) const;
        void                  printYinForModuleToFile                  (const string &moduleName, const string &filePath) const;

        void                  getAllNames                              (set<string> &allNames) const;
        void                  printAllNames                            (FILE *pFile = NULL) const;
        void                  printAllNamesToFile                      (const string &filePath) const;

        void                  getAllCliTargetNodeNamesForData          (vector<string> &allCliTargetNodeNamesForData) const;
        void                  printAllCliTargetNodeNamesForData        (FILE *pFile = NULL) const;
        void                  printAllCliTargetNodeNamesForDataToFile  (const string &filePath) const;

        void                  loadUserTagsFromFile                     (const string &userTagsFilePath);
        void                  addUserTag                               (const string &userTagName, const UI32 &userTagValue);
        UI32                  getUserTagValueByName                    (const string &userTagName) const;
        void                  updateModulesWithUserTags                ();

        void                  computeConfigurationSegmentNames         ();

        YangElement          *getYangElementByQualifiedYangPath        (const string &qualifiedYangPath) const;
        YangElement          *getYangElementByPrefixQualifiedYangPath  (const string &prefixQualifiedYangPath) const;

        void                  computeCliTargetNodeNames                    ();
        void                  markNodeSpecificAndPartitionBaseYangElements ();
        void                  inlineTypedef                                ();
        void                  processTypeInformations                      ();

        UI32                  getNumberOfAllChildYangElementsInAllModules  () const;

        void                  setOriginalModuleNameSpaceUriInAllModules    ();

        void                  generateHFilesForCLanguageForAllModules      () const;

    // now the data members

    private :
        vector<YangModule *>      m_yangModules;
        map<string, YangModule *> m_yangModulesMap;
        map<string, YangModule *> m_yangModulesMapByPrefix;

        map<string, UI32>         m_userTagsMap;

    protected :
    public :
};

}

#endif // YANGMODULECOLLECTION_H
