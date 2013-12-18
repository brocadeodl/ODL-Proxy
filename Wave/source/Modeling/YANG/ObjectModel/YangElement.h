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

#ifndef YANGELEMENT_H
#define YANGELEMENT_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/PrismMutex.h"
#include <string>
#include <map>
#include <vector>
#include <set>

#include "xercesc/dom/DOM.hpp"

using namespace std;
using namespace XERCES_CPP_NAMESPACE;

namespace WaveNs
{

class YangModule;
class YangModuleCollection;
class YangDisplayConfigurationContext;
class ConfigurationSegmentInformation;
class WaveYangMemberElementFilterInformation;

class YangElement
{
    private :
                void                   setDefaultValues                        ();

    protected :
                void                   getSelectedChildElementsForData         (const string &tokenString, vector<YangElement *> &selectedChildElementsForData) const;
                void                   getSelectedChildElementsForDataWithExactMatch    (const string &tokenString, vector<YangElement *> &selectedChildElementsForData) const;
                void                   addYangElementForCliToken               (const string &tokenString, YangElement *pYangElement);

    public :
                                       YangElement                             (const string &yangName, const string &name);
        virtual                       ~YangElement                             ();

                string                 getYangName                             () const;
                void                   setYangName                             (const string &yangName);


                string                 getName                                 () const;
                void                   setName                                 (const string &name);

                string                 getDisplayName                          () const;
                void                   setDisplayName                          (const string &displayName);

                string                 getValue                                () const;
                void                   setValue                                (const string &value);
                void                   addValue                                (const string &value);
        virtual void                   loadValue                               (const string &value);

                YangElement           *getPParentElement                       () const;
                void                   setPParentElement                       (YangElement * const pParentElement);

                vector<YangElement *> &getChildElements                        ();

                void                   addChildElement                         (YangElement *pChildElement);
                void                   addChildElements                        (vector<YangElement *> &childElements);
                void                   addChildElementsBefore                  (YangElement *pChildElement, vector<YangElement *> &newChildElements);

                void                   removeChildElement                      (YangElement *pChildElement);
                void                   removeAllChildElementsByYangName        (const string &yangName);
                void                   removeAllChildElements                  ();

                bool                   isAKnownAttribute                       (const string &attributeName) const;
                void                   addAttribute                            (const string &attributeName, const string &attributeValue);
                void                   getAttributeValue                       (const string &attributeName,       string &attributeValue) const;

        virtual void                   loadFromDomElement                      (DOMElement *pDomElement);
        virtual void                   processAttribute                        (const string &attributeName, const string &attributeValue);
        virtual void                   processChildElement                     (YangElement *pYangElement);
        virtual void                   unprocessChildElement                   (YangElement *pYangElement);

        virtual void                   printYin                                (const UI32 &nodeDepth = 0, const bool &printXmlDeclaration = true, FILE *pFile = stdout) const;
        virtual void                   printYinForThisNode                     (const UI32 &nodeDepth = 0, FILE *pFile = stdout) const;
        virtual void                   printYinForThisNodeHeader               (const UI32 &nodeDepth = 0, FILE *pFile = stdout) const;
        virtual void                   printYinForThisNodeValue                (const UI32 &nodeDepth = 0, FILE *pFile = stdout) const;
        virtual void                   printYinForThisNodeFooter               (const UI32 &nodeDepth = 0, FILE *pFile = stdout) const;

        virtual void                   printYinForTargetNode                   (const string &targetNode, FILE *pFile = stdout);

        virtual void                   printYinToFile                          (const string &filePath) const;
        virtual void                   printYinForTargetNodeToFile             (const string &targetNode, const string &filePath);

        virtual void                   printYinToString                        (string &yinString, const UI32 &nodeDepth = 0, const bool &printXmlDeclaration = true) const;
        virtual void                   printYinForThisNodeToString             (string &yinString, const UI32 &nodeDepth = 0) const;
        virtual void                   printYinForThisNodeHeaderToString       (string &yinString, const UI32 &nodeDepth = 0) const;
        virtual void                   printYinForThisNodeValueToString        (string &yinString, const UI32 &nodeDepth = 0) const;
        virtual void                   printYinForThisNodeFooterToString       (string &yinString, const UI32 &nodeDepth = 0) const;

                char                   getAttributeQuotingCharacter            () const;
                void                   setAttributeQuotingCharacter            (const char &attributeQuotingCharatcer);

        virtual YangElement           *clone                                   () const;

                void                   setPOwnerYangModule                     (YangModule *pOwnerYangModule);
                void                   setPOwnerYangModuleForTree              (YangModule *pOwnerYangModule);

        virtual bool                   considerForComputingUsageCountForGroupingsForProgrammingLanguages () const;

        virtual void                   computeUsageCountForGroupings                        (const YangModuleCollection *pYangModuleCollection);
        virtual void                   computeUsageCountForGroupingsForProgrammingLanguages (const YangModuleCollection *pYangModuleCollection);

        virtual void                   incrementUsageCountForGroupingsInHierarchyForProgrammingLanguages (const YangModuleCollection *pYangModuleCollection, const UI32 &count = 1);

        virtual void                   inlineGroupingUsage                     (const YangModuleCollection *pYangModuleCollection);
        virtual void                   inlineGroupingUsageForRpcs              (const YangModuleCollection *pYangModuleCollection);
        virtual void                   inlineAugmentUsage                      (const YangModuleCollection *pYangModuleCollection, UI32 &numberOfAugmentsResolved, UI32 &numberOfAugmentsNotResolved);

                void                   getAllChildrenByYangName                (const string &yangName, vector<YangElement *> &childElementsByYangName) const;

                void                   getClonedChildren                       (vector<YangElement *> &clonedChildren) const;

                void                   checkIntegrity                          () const;

                YangModule            *getPOwnerYangModule                     () const;

                YangElement           *getYangElementForTargetNode             (const string &targetNode);

                YangElement           *getUniqueYangElementByName              (const string &uniqueYangElementName) const;

        virtual void                   getAllNames                             (set<string> &allNames) const;
        virtual void                   printAllNames                           (FILE *pFile = NULL) const;
        virtual void                   printAllNamesToFile                     (const string &filePath) const;

        virtual bool                   considerSubTreeForHFileForCLanguage     () const;
        virtual bool                   considerForHFileForCLanguage            () const;
        virtual void                   getNameForHFileForCLanguage             (string &actualName, string &processedNameForCLanguage, string &cLanguageValue) const;
        virtual void                   getAllNamesForHFileForCLanguage         (map<string, string> &allNamesForHFileForCLanguage, map<string, string> &allValuesForHFileForCLanguage) const;
        virtual void                   getAllExplicitlySetNamesForHFileForCLanguage (map<string, string> &allNamesForHFileForCLanguage, map<string, string> &allValuesForHFileForCLanguage) const;
        virtual void                   printAllNamesForHFileForCLanguage       (const YangModuleCollection &yangModuleCollection, FILE *pFile = NULL) const;
        virtual void                   printAllNamesToFileForHFileForCLanguage (const string &filePath, const YangModuleCollection &yangModuleCollection, const string &prefix) const;

        virtual void                   generateHFileForCLanguage               (const YangModuleCollection &yangModuleCollection) const;

        virtual string                 getVariableNameInProgrammingLanguage    () const;

        virtual void                   getAllCliTargetNodeNamesForData         (vector<string> &allCliTargetNodeNamesForData) const;
        virtual void                   computeCliTargetNodeNameMapForData      (map<string, vector<YangElement *> > &cliTargetNodeNameMapForData, map<string, vector<YangElement *> > &cliTargetNodeNameMapForDataWithDropNodeName) const;
        virtual void                   printAllCliTargetNodeNamesForData       (FILE *pFile = NULL) const;
        virtual void                   printAllCliTargetNodeNamesForDataToFile (const string &filePath) const;

        virtual void                   updateUserTags                          (const YangModuleCollection *pYangModuleCollection);

                UI32                   getUserTagValue                         () const;
                void                   setuserTagValue                        (const UI32 &userTagValue);

        virtual void                   computeConfigurationSegmentNames        ();
                string                 getConfigurationSegmentName             () const;
                void                   setConfigurationSegmentName             (const string &configurationSegmentName);
                bool                   getIsConfigurationSegmentRoot           () const;
                void                   setIsConfigurationSegmentRoot           (const bool &isConfigurationSegmentRoot);

                bool                   getIsConfiguration                      () const;
                void                   setIsConfiguration                      (const bool &isConfiguration);

                bool                   getIsConfigurationExplicitlySet         () const;
                void                   setIsConfigurationExplicitlySet         (const bool &isConfigurationExplicitlySet);

                UI32                   getNumberOfChildElements                () const;

                YangElement           *getYangElementByTargetNodePath          (const string &targetNode);

        virtual void                   displayConfiguration                    (YangDisplayConfigurationContext *pYangDisplayConfigurationContext);
        virtual void                   displayConfiguration1                   (YangDisplayConfigurationContext *pYangDisplayConfigurationContext);
        virtual void                   displayConfiguration2                   (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData);
        virtual void                   displayConfiguration3                   (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData);

        virtual void                   debugPrint                              () const;

        virtual void                   computeCliTargetNodeNames               ();
        virtual void                   computeCliTargetNodeNameForSelf         ();
        virtual void                   computeCliTargetNodeNameForSelfInternal ();

                bool                   getDropNodeNameForCli                   () const;
                void                   setDropNodeNameForCli                   (const bool &dropNodeNameForCli);

                string                 getCliTargetNodeName                    () const;
                void                   setCliTargetNodeName                    (const string &cliTargetNodeName);
                string                 getMostRecentCliTargetNodeName          () const;

        virtual string                 getCliTargetNodeNameForData             () const;

				string                 getCliTargetNodeNameForDropNode         () const;
				void                   setCliTargetNodeNameForDropNode         (const string &cliTargetNodeNameForDropNode);

                bool                   getIsDeprecated                         () const;
                void                   setIsDeprecated                         (const bool &deprecated);

        virtual void                   computeWillBeShownInConfigurationTree   ();
        virtual bool                   meetsCriteriaToBeShownInConfiguration   () const;

                bool                   getWillBeShownInConfigurationTree       () const;
                void                   setWillBeShownInConfigurationTree       (const bool &willBeShownInConfigurationTree);

                bool                   getIsFullCommand                        () const;
                void                   setIsFullCommand                        (const bool &isfullCommand);

                bool                   getIsCliShowNo                          () const;
                void                   setIsCliShowNo                          (const bool &isCliShowNo);

                bool                   getIsCliAddMode                         () const;
                void                   setIsCliAddMode                         (const bool &isCliAddmode);

                bool                   getIsCliSuppressMode                    () const;
                void                   setIsCliSuppressMode                    (const bool &isCliSuppressMode);

                void                   setIsCliRunTemplate                     (const bool  &cliRunTemplate);
                bool                   getIsCliRunTemplate                     () const;

                void                   setIsCliRunTemplateEnter                (const bool  &cliRunTemplateEnter);
                bool                   getIsCliRunTemplateEnter                () const;

                void                   setIsCliFlatListSyntax                  (const bool &cliFlatListSyntax);
                bool                   getIsCliFlatListSyntax                  () const;

              string                   getCliCompactSyntax                     () const;
                void                   setCliCompactSyntax                     ();

              string                   getExposeKeyName                        () const;
                void                   setExposeKeyName                        ();

              string                   getInfo                                 () const;
                void                   setInfo                                 (const string &info);

                void                   setCliSuppressShowMatch                 (const bool  &cliSuppressShowMatch);
                bool                   getCliSuppressShowMatch                 () const;

                void                   setCliSuppressRange                     (const bool  &cliSuppressRange);
                bool                   getCliSuppressRange                     () const;

                void                   setIsCodeName                           (const bool  &isCodeName);
                bool                   getIsCodeName                           () const;

                void                   setCliIncompleteCommand                 (const bool  &cliIncompleteCommand);
                bool                   getCliIncompleteCommand                 () const;

                void                   setCliNoKeyCompletion                   (const bool  &cliNoKeyCompletion);
                bool                   getCliNoKeyCompletion                   () const;

                void                   setCliSuppressShowConfPath              (const bool  &cliSuppressShowConfPath);
                bool                   getCliSuppressShowConfPath              () const;

                void                   setCliNoMatchCompletion                 (const bool  &cliNoMatchCompletion);
                bool                   getCliNoMatchCompletion                 () const;

                void                   setCliSuppressKeySort                   (const bool  &cliSuppressKeySort);
                bool                   getCliSuppressKeySort                   () const;

                bool                   isAtTopLevelInAModule                   () const;

        virtual void                   computeIsConfiguration                  ();
        virtual void                   computeIsConfigurationForSelf           ();
        virtual void                   computeFirstDataElementInHierarchy      ();

                bool                   getIsFirstDataElementInHierarchy        () const;
                void                   setIsFirstDataElementInHierarchy        (const bool &isFirstDataElementInHierarchy);

        virtual void                   setIsConfigurationForEntireTree         (const bool &isConfiguration);

         YangElement                  *getSimilarChildElement                  (YangElement *pYangElement);
                void                   setAnnotationBitMap                     (UI64 childBitMap);
                UI64                   getAnnotationBitMap                     ();
        virtual void                   updateParentAnnotations                 ();

                int                    getMaxDepth                             ();

        virtual bool                               isYangElementForData                    () const;
        virtual UI32                               getDataIndex                            () const;
        static  bool                               compareYangElementsByDataIndex          (YangElement *pYangElement1, YangElement *pYangElement2);
                void                               setupYangElementTreeForData             ();
        const   vector<YangElement *>             &getPChildElementsForData                () const;
                void                               debugPrintChildElementInforamtionForData(FILE *pOutputFile, const UI32 &depth) const;

                void                               setIsNodeSpecificBaseList               ();
                bool                               getIsNodeSpecificBaseList               () const;
                bool                               getIsInNodeSpecificTree                 () const;
                void                               setIsMultiPartitionBaseList             ();
                bool                               getIsMultiPartitionBaseList             () const;
                bool                               getIsInMultiPartitionTree               () const;
                void                               propagateNodeSpecificInformation        ();
                void                               propagateMultiPartitionInformation      ();

                void                               prepareHierarchyInformation             ();

                ConfigurationSegmentInformation   *getPConfigurationSegmentInformation      () const;

                void                               setIsHidden                              ();
                bool                               getIsHidden                              (YangDisplayConfigurationContext *pYangDisplayConfigurationContext) const;

        const   set<string>                       &getKnownDisplayNames                     () const;
                void                               prepareSetOfKnownDisplayNames            ();

                void                               refineFirstLevelNonDataChildElements     (const string &childElementYangName, YangElement *pYangElementToBeRefined);
                void                               refineYangElement                        (YangElement *pYangElementToBeRefined);

       virtual  UI32                               absorbeKey                               (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData) const;
       virtual  void                               restoreAbsorbedKeys                      (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, const UI32 &numberOfKeysAbsorbed) const;

       virtual  void                               inlineTypedefFromImportedModule          (const YangModuleCollection *pYangModuleCollection);
       virtual  void                               inlineTypedefInsideModule                ();
                void                               getclonedChildrenForTypedefFromParents   (const string &typedefName , vector<YangElement *> &clonedChildrenForUsedTypedef);
                void                               processTypeInformations                  ();

                UI32                               getNumberOfAllChildYangElementsInTree            () const;
        static  void                               incrementNumberOfInstancesOfYangElementsInSystem ();
        static  void                               decrementNumberOfInstancesOfYangElementsInSystem ();
        static  UI32                               getNumberOfInstancesOfYangElementsInSystem       ();

                void                               setIsCliMultiValue                               ();
                bool                               getIsCliMultiValue                               () const;

                bool                               getCameFromAugment                               () const;
                void                               setCameFromAugment                               ();
                string                             getOriginalModuleNameSpaceUri                    () const;
                void                               setOriginalModuleNameSpaceUri                    (const string &originalModuleNameSpaceUri);
                void                               setOriginalModuleNameSpaceUriForSelf             ();
                void                               setOriginalModuleNameSpaceUriForTree             ();
                void                               rearrangeAugmenterChildElements                  ();
        static  bool                               compareYangElementsByOriginalModuleNameSpaceUri  (YangElement *pYangElement1, YangElement *pYangElement2);

                void                               prepareTypeInformation                           ();
        virtual void                               prepareTypeInformationForSelf                    ();

                void                               setCliHideInSubMode                              ();
                bool                               getCliHideInSubMode                              () const;

                void                               collectHideInSubModeLeaf                         ();
        virtual void                               collectHideInSubModeLeafForSelf                  ();

                void                               propagateCompactSyntaxOneLevel                   ();
        virtual void                               propagateCompactSyntaxOneLevelForSelf            ();

                void                                    collectInformationAboutChildLeafWithCustomDisplay            ();
        virtual void                                    collectInformationAboutChildLeafWithCustomDisplayForSelf     ();

        virtual ResourceId                              doesElementValueMatch                                        (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, const string &matchValue);
        virtual WaveYangMemberElementFilterInformation *collectMemberElementFilterInformation (YangDisplayConfigurationContext *pYangDisplayConfigurationContext) const;

                UI32                                    getMaxNestingDepthForYangLists                               () const;

                bool                                    getIsInLocalConfigurationSubtree                             () const;
                void                                    setIsInLocalConfigurationSubtree                             ();
                void                                    computeIsInLocalConfigSubtree                                ();
                void                                    markEntireSubtreeAsLocalConfig                               ();

        virtual bool                                    isInsideAnRpc                                                () const;

// Now the data members

    private :
        string                                           m_yangName;
        string                                           m_name;
        string                                           m_displayName;
        string                                           m_value;
        YangElement                                     *m_pParentElement;
        vector<YangElement *>                            m_childElements;
        map<string, vector<YangElement *> >              m_childElementsMapByYangName;
        map<string, map<YangElement *, YangElement *> >  m_childElementsMapByName;
        map<string, map<YangElement *, YangElement *> >  m_childElementsMapByDisplayName;
        vector<string>                                   m_attributes;
        map<string, string>                              m_attributesMap;

        char                                             m_attributeQuotingCharacter;

        YangModule                                      *m_pOwnerYangModule;
        YangModule                                      *m_pOriginalOwnerYangModule;

        UI32                                             m_userTagValue;
        string                                           m_configurationSegmentName;
        bool                                             m_isConfigurationSegmentRoot;
        bool                                             m_isConfiguration;
        bool                                             m_isConfigurationExplicitlySet;
        string                                           m_cliTargetNodeName;
        bool                                             m_isDeprecated;
        bool                                             m_willBeShownInConfigurationTree;
        bool                                             m_isFirstDataElementInHierarchy;
        UI64                                             m_annotationBitMap;

        string                                           m_programmingLanguageVariableName;


        struct annotationFlags
        {
            bool        m_isCliShowNo;
            bool        m_isCliAddMode;
            bool        m_isCliSuppressMode;
            string      m_cliCompactSyntax;
            string      m_exposeKeyName;
            string      m_info;
            bool        m_isFullCommand;
            bool        m_dropNodeNameForCli;
            bool        m_cliSuppressRange;
            bool        m_isCodeName;
            bool        m_cliIncompleteCommand;
            bool        m_cliNoKeyCompletion;
            bool        m_cliSuppressShowConfPath;
            bool        m_cliNoMatchCompletion;
            bool        m_cliSuppressKeySort;
            bool        m_cliSuppressShowMatch;
            bool        m_cliMultiValue;
            bool        m_cliHideInSubMode;
            bool        m_cliRunTemplate;
            bool        m_cliRunTemplateEnter;
            bool        m_cliFlatListSyntax;

            // Initialize all the anotation to there appropriate values
            annotationFlags() : m_isCliShowNo                (false),
                                m_isCliAddMode               (false),
                                m_isCliSuppressMode          (false),
                                m_cliCompactSyntax           ("\r\n"),
                                m_exposeKeyName              (""),
                                m_info                       (""),
                                m_isFullCommand              (false),
                                m_dropNodeNameForCli         (false),
                                m_cliSuppressRange           (false),
                                m_isCodeName                 (false),
                                m_cliIncompleteCommand       (false),
                                m_cliNoKeyCompletion         (false),
                                m_cliSuppressShowConfPath    (false),
                                m_cliNoMatchCompletion       (false),
                                m_cliSuppressKeySort         (false),
                                m_cliSuppressShowMatch       (false),
                                m_cliMultiValue              (false),
                                m_cliHideInSubMode           (false),
                                m_cliRunTemplate             (false),
                                m_cliRunTemplateEnter        (false),
                                m_cliFlatListSyntax          (false)
                                { };
        };

        annotationFlags                                     m_annotationFlags;
		string 											    m_cliTargetNodeNameForDropNode;
        vector<YangElement *>                               m_pChildElementsForData;

        bool                                                m_isNodeSpecificBaseList;
        bool                                                m_isMultiPartitionBaseList;
        bool                                                m_underNodeSpecificTree;
        bool                                                m_underMultiPartitionTree;

        ConfigurationSegmentInformation                    *m_pConfigurationSegmentInformation;

        bool                                                m_isHidden;

        set<string>                                         m_knownDisplayNames;
        map<string, vector<YangElement *> >                 m_knownDisplayNamesToChildElementsMap;
        map<string, vector<YangElement *> >                 m_knownCaseInsensitiveDisplayNamesToChildElementsMap; // These two maps can be combined ?

        bool                                                m_cameFromAugment;
        string                                              m_originalModuleNameSpaceUri;

        bool                                                m_isInLocalConfigurationSubtree;

        static  UI32                                        s_numberOfInstancesOfYangElement;
        static  PrismMutex                                  s_numberOfInstancesOfYangElementMutex; // currently, all YangElements get created in one thread.

    protected :
    public :
};

}

#endif // YANGELEMENT_H
