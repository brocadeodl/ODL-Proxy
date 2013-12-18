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

#ifndef YANGDISPLAYCONFIGURATIONCONTEXT_H
#define YANGDISPLAYCONFIGURATIONCONTEXT_H

#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Modeling/XPath/WaveXPathExpressionElement.h"

#include <stdio.h>
#include <vector>
#include <map>
#include <vector>

using namespace std;

namespace WaveNs
{

class WaveManagedObject;
class Attribute;
class YangElement;
class YangList;
class YangUserInterface;

class WaveYangMemberElementFilterInformation;

class YangDisplayConfigurationContext : public WaveXPathExpressionEvaluationContext
{
    private :

        class YangElementDisplayState
        {
            public:
                                            YangElementDisplayState                     (YangElementDisplayState *pPreviousYangElementDisplayState, const bool &isSuppressedMode, const bool &isCompactSyntax);
                                           ~YangElementDisplayState                     ();

                YangElementDisplayState    *getPreviousYangElementDisplayState          () const;
                bool                        getIsSuppressedMode                         () const;
                bool                        getIsCompactSyntax                          () const;

                string                      getPrefixString                             () const;                      // includes parent string if required.
                void                        setPrefixString                             (const string &prefixString);  // for self.

                bool                        getNewLinePendingInHierarchy                () const;
                bool                        getAndResetPendingNewLine                   ();
                bool                        getNewLinePending                           () const;
                void                        setNewLinePending                           ();

                void                        setOutputWasPrinted                         ();
                bool                        getOutputWasPrinted                         () const;

                void                        setPendingOutputString                      (const string &pendingOutputString, const UI32 &depth, const string &newLineString);
                string                      getAndResetPendingOutputStringInHierarchy   ();
                void                        resetPendingOutputString                    ();

                void                        setNewLinePendingInPendingOutputString      ();
                bool                        getNewLinePendingInPendingOutputString      () const;
                void                        resetNewLinePendingInPendingOutputString    ();

                void                        debugPrint                                  () const;
                bool                        getIsOutputPending                          () const;

            private:
                YangElementDisplayState    *m_pPreviousYangElementDisplayState;
                bool                        m_isSuppressedMode;         // parameter
                bool                        m_isCompactSyntax;          // parameter
                string                      m_parentPrefixString;       // cache
                string                      m_prefixString;             // state
                bool                        m_newLinePendingInParent;   // cache
                bool                        m_newLinePending;           // state
                bool                        m_outputWasPrinted;         // state
                string                      m_pendingOutputString;      // state
                bool                        m_newLinePendingInPendingOutputString;
                bool                        m_isOutputPending;
        };

        class ConfigLevel
        {
            public:
                                                    ConfigLevel                                     (vector<WaveManagedObject *> &pWaveManagedObjects, YangList *pYangList);
                                                   ~ConfigLevel                                     ();

                WaveManagedObject                  *getPWaveManagedObjectForConfigurationSegment    (const string &configurationSegmentName, bool &isKnown) const;
                void                                addPWaveManagedObjectForConfigurationSegment    (const string &configurationSegmentName, WaveManagedObject *pWaveManagedObject);

                void                                incrementCurrentIndex                           ();

                void                                getKeyAttributesAtCurrentIndex                  (vector<Attribute *> &pKeyAttributes) const;

                void                                getObjectIdForManagedObject                     (const string &managedObjectClassName, bool &found, ObjectId &objectId) const;

                void                                setNextBatchOfWaveManagedObjects                (vector<WaveManagedObject *> &pWaveManagedObjects, YangList *pYangList);

            private:
                vector<WaveManagedObject *>         m_pWaveManagedObjects;
                YangList                           *m_pYangList;
                UI32                                m_currentIndex;
                map<string, WaveManagedObject *>    m_configurationSegmentToPWaveManagedObjectMap;
        };

                    void                querySynchronously                          (WaveManagedObjectSynchronousQueryContext &waveManagedObjectSynchronousQueryContext, const string &managedObjectClassName, vector<WaveManagedObject *> &pWaveManagedObjects, const bool &disableFilters = false) const;
                    ConfigLevel *       getLastConfigLevel                          ();
                    void                getKeyAttributesAtConfigLevel               (const UI32 &indexInConfigLevels, vector<Attribute *> &pKeyAttributes) const;
                    void                getManagedObjectIdForLastLevelInHierarchy   (YangElement *pYangElement, ObjectId &ownerObjectId) const;
                    void                getObjectIdForManagedObjectAtLevel          (const UI32 &indexInConfigLevels, const string &managedObjectClassName, bool &found, ObjectId &objectId) const;
                    void                addPWaveManagedObjectForTopLevelConfigurationSegment    (const string &configurationSegmentName, WaveManagedObject *pWaveManagedObject);
                    YangElementDisplayState *getLastYangElementDisplayState                     () const;

                    ResourceId               addAndAttributesForKeyValues                       (WaveManagedObjectSynchronousQueryContext &waveManagedObjectSynchronousQueryContext, const string &managedObjectClassName, const vector<Attribute *> &pKeyAttributes, const bool &applyNodeSpecificFilter) const;
                    ResourceId               getObjectIdOfAssocociatedManagedObjectWithKey      (const string &keyValue, AttributeObjectIdAssociation *pAssociationAttribute, const bool &applyNodeSpecificFilter) const;

                    void                    debugPrintErrorInformation                          (WaveManagedObjectSynchronousQueryContext &waveManagedObjectSynchronousQueryContext, vector<WaveManagedObject *> &pWaveManagedObjects) const;

                    void                    lockYangUserInterface                               () const;
                    void                    unlockYangUserInterface                             () const;
                    void                    printOutputToCliSessionWrapper                      (const string &outputStringForDisplay);

    protected :
    public :
        typedef enum
        {
            WAVE_YANG_CONFIG_READ_FILTER_NONE,
            WAVE_YANG_CONFIG_READ_FILTER_LOCAL,
            WAVE_YANG_CONFIG_READ_FILTER_GLOBAL
        } WaveYangConfigReadFilterType;

                                     YangDisplayConfigurationContext ();
        virtual                     ~YangDisplayConfigurationContext ();

        FILE                        *getPFile                        () const;
        void                         setPFile                        (FILE *pFile);
        vector<WaveManagedObject *> *getPManagedObjectsToDisplay     ();
        void                         setPManagedObjectsToDisplay     (vector<WaveManagedObject *> *pManagedObjectsToDisplay);

                    void                setConfigurationSchema              (const string &configurationSchema);
                    string              getConfigurationSchema              () const;

                    void                getPWaveManagedObjects              (YangList *pYangList, vector<WaveManagedObject *> &pWaveManagedObjects, const UI32 &pageNumber);
                    void                getPWaveManagedObject               (YangElement *pYangElement, WaveManagedObject *&pWaveManagedObject, const bool &disableFilters);

                    void                addConfigurationLevel               (vector<WaveManagedObject *> &pWaveManagedObjects, YangList *pYangList);
                    void                addNextBatchOfWaveManagedObjects    (vector<WaveManagedObject *> &pWaveManagedObjects, YangList *pYangList);
                    void                incrementIndexForLastConfigLevel    ();
                    void                deleteLastConfigLevel               ();

                    void                setOwnerWaveNodeObjectId            (const ObjectId &ownerWaveNodeObjectId);
                    void                resetOwnerWaveNodeObjectId          ();
                    ObjectId            getOwnerWaveNodeObjectId            () const;

                    void                setOwnerPartitionObjectId           (const ObjectId &ownerPartitionObjectId);
                    void                resetOwnerPartitionObjectId         ();
                    ObjectId            getOwnerPartitionObjectId           () const;

                    void                addYangElementDisplayState          (const bool &isSuppressedMode, const bool &isCompactSyntax);
                    void                removeLastYangElementDisplayState   ();

                    void                incrementDepth                      ();
                    void                decrementDepth                      ();

        virtual     void                printOutputToCliSession             (const string &outputStringForDisplay);
                    void                printOutputToCliSessionBuffered     (const string &outputStringForDisplay);
                    void                flushBufferedOutput                 ();
                    void                printOutput                         (const string &outputStringForDisplay);
                    void                printString                         (const string &outputStringForDisplay, const bool &printNewLine = true);
                    void                printSeparator                      ();
                    void                printNewLineIfRequired              ();
                    void                printConfigString                   (const string &outputString, const bool &isCliIncompleteCommand = false);
                    void                printNoConfigString                 (const string &outputString);
                    void                setPrefixString                     (const string &prefixString);
                    void                printHeaderString                   (const string &outputString);
                    void                printCustomTemplateConfigOutput     (const string &outputStringForDisplay);

                    // For container with add mode.
                    void                setPendingOutputString              (const string &pendingOutputString);
                    void                resetPendingOutputString            ();

                    void                printPendingOutput                  ();
                    bool                getOutputWasPrinted                 () const;

                    void                setCliString                        (const string &cliString);
                    string              advanceToNextToken                  ();
                    void                decrementTokenPosition              ();
                    UI32                getNumberOfRemainingTokens          () const;

                    void                insertCommandKey                    (vector<string> &keyVector);
                    void                incrementCommandKeyIndex            ();
                    void                decrementCommandKeyIndex            ();
                    void                getCurrentCommnadKey                (vector<string> &keyVector);
                    void                setCommandKeyIndex                  (UI32 index);
                    void                removeLastCommandKey                ();

                    void                debugPrintDisplayState              () const;

                    void                                setConfigFilterType                         (const WaveYangConfigReadFilterType &configFilterType);
                    WaveYangConfigReadFilterType        getConfigFilterType                         () const;

                    bool                                getIsOwnerWaveNodeObjectIdFilterSet         () const;

                    void                                setOwnerWaveNodeObjectIdFilter              (const ObjectId &ownerWaveNodeObjectIdFilter);
                    ObjectId                            getOwnerWaveNodeObjectIdFilter              () const;

            // virtual function in WaveXPathExpressionEvaluationContext
            virtual WaveXPathExpressionValue            evaluateValueOfLocationPath                 (const WaveLocationPath &locationPath, const YangElement *pContextYangElement);

                    void                                discardContentsOfPartialOutputBuffer        ();
                    void                                addSlotForPartialOutput                     ();

                    bool                                getAnyOutputDisplayed                       () const;
                    void                                setAnyOutputDisplayed                       ();

                    void                                getCachedValuesForAbsoluteXPathLocationPath (const string &locationPathString, WaveXPathExpressionValue &resultValue, bool &valueFound) const;
                    void                                addValueForAbsoluteXPathLocationPath        (const string &locationPathString, const WaveXPathExpressionValue &resultValue);

                    string                              getSpacePrefixString                        () const;

            virtual bool                                isCommandAbortedByUser                          () const;

                    void                                addSetOfUnhiddenHideGroupNames                  (const set<string> &setOfUnhiddenHideGroupNames);
                    void                                addHideGroupNameToSetOfUnhiddenHideGroupNames   (const string &unhiddenHideGroupName);
                    bool                                isHideGroupUnhidden                             (const string &hideGroupName) const;

                    bool                                isConfigFilterApplicable                        (const YangElement *pYangElement) const;

                    void                                setPrintDefaultValuesFlag                       (const bool &printDefaultValuesFlag);
                    bool                                getPrintDefaultValuesFlag                       () const;

                    void                                setPYangUserInterfaceAndLock                    (YangUserInterface *pYangUserInterface);
                    void                                releasePYangUserInterfaceLock                   ();

                    void                                setBatchSizeForQueryingListInstances            (const UI32 &batchSize);
                    UI32                                getBatchSizeForQueryingListInstances            () const;

            static  UI32                                getDefaultBatchSizeForQueryingListInstances     ();

                    string                              getNewLineString                                () const;
                    void                                setNewLineString                                (const string &newLineString);

    // Now the Data Members

    private :
        FILE                        *m_pFile;
        vector<WaveManagedObject *> *m_pManagedObjectsToDisplay;

        string                              m_configurationSchema;

        UI32                                m_depth;
        map<string, WaveManagedObject *>    m_topLevelConfigurationSegmentToPWaveManagedObjectMap;
        vector<ConfigLevel *>               m_pConfigLevels;
        YangElementDisplayState            *m_pLastYangElementDisplayState;

        ObjectId                            m_ownerWaveNodeObjectId;       // node-specific filter.
        ObjectId                            m_ownerPartitionObjectId;      // multi-partition filter.

        string                              m_cliString;
        vector<string>                      m_cliTokens;
        UI32                                m_currentIndexForCliTokens;

        vector<vector<string> >             m_keyValueVector;
        UI32                                m_currentCommandKeyIndex;

        WaveYangConfigReadFilterType        m_configFilterType;

        ObjectId                            m_ownerWaveNodeObjectIdFilter;
        bool                                m_ownerWaveNodeObjectIdFilterIsSet;

        vector<string>                      m_partialOutputBuffer;
        vector<bool>                        m_printingAllowed;
        UI32                                m_firstOutputIndex;
        bool                                m_anyOutputDisplayed;

        string                              m_finalOutputBuffered;

        map<string, WaveXPathExpressionValue>   m_cachedValuesForAbsoluteXPathLocationPaths;

        set<string>                             m_setOfUnhiddenHideGroupNames;  // The hide group present in this set will be exposed to user.

        bool                                    m_printDefaultValuesFlag;       // By-default, this flag will be false.

        YangUserInterface                      *m_pYangUserInterface;                       // read-only.

        UI32                                    m_batchSizeForQueryingListInstances;

        static UI32                             s_defaultBatchSizeForQueryingListInstances;

        string                                  m_newLineString;

    protected :
    public :
};

class WaveCustomCliDisplayConfigurationContext
{
    private:
        YangElement                     *m_pYangElement;
        YangDisplayConfigurationContext *m_pYangDisplayConfigurationContext;

    public:
        YangElement*                     getYangElement                     ();
        void                             setYangElement                     (YangElement* pYangElement);

        YangDisplayConfigurationContext* getYangDisplayConfigurationContext ();
        void                             setYangDisplayConfigurationContext (YangDisplayConfigurationContext *pYangDisplayConfigurationContext);

        string                           getConfigurationSegmentName        ();

        UI32                             getAttributeUserTag                ();

        WaveCustomCliDisplayConfigurationContext                            (YangElement* pYangElement, YangDisplayConfigurationContext* pYangDisplayConfigurationContext );
        ~WaveCustomCliDisplayConfigurationContext                           ();

};

class WaveYangMemberElementFilterInformation
{
    public:
                                        WaveYangMemberElementFilterInformation  (const YangElement *pYangElement, const string &matchValue, WaveYangMemberElementFilterInformation *pNextWaveYangMemberElementFilterInformation);
                                       ~WaveYangMemberElementFilterInformation  ();

                    ResourceId          doesFilterInformationMatch              (YangDisplayConfigurationContext *pYangDisplayConfigurationContext);
                    void                setNumberOfTokensAbsorbed               (const UI32 &numberOfTokensAbsorbed);
                    void                restoreAbsorbedTokens                   (YangDisplayConfigurationContext *pYangDisplayConfigurationContext);


    private:
            const   YangElement                            *m_pYangElement;
                    string                                  m_matchValue;
                    UI32                                    m_numberOfTokensAbsorbed;
                    WaveYangMemberElementFilterInformation *m_pNextWaveYangMemberElementFilterInformation;
};

}

#endif // YANGDISPLAYCONFIGURATIONCONTEXT_H
