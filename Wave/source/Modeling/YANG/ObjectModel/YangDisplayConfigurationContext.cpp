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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka  / Mandar Datar                    *
 ***************************************************************************/

#include "Modeling/YANG/ObjectModel/YangDisplayConfigurationContext.h"
#include "Modeling/YANG/ObjectModel/YangList.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"
#include "Framework/Attributes/Attribute.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveLocalManagedObjectBase.h"
#include "Framework/ObjectModel/WaveLocalManagedObject.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"

namespace WaveNs
{

UI32 YangDisplayConfigurationContext::s_defaultBatchSizeForQueryingListInstances = 1000;

// YangElementDisplayState

YangDisplayConfigurationContext::YangElementDisplayState::YangElementDisplayState (YangElementDisplayState *pPreviousYangElementDisplayState, const bool &isSuppressedMode, const bool &isCompactSyntax)
{
    m_pPreviousYangElementDisplayState  = pPreviousYangElementDisplayState;
    m_isSuppressedMode                  = isSuppressedMode;
    m_isCompactSyntax                   = isCompactSyntax;

    if (NULL != m_pPreviousYangElementDisplayState)
    {
        if (true == m_isSuppressedMode)
        {
            m_parentPrefixString = m_pPreviousYangElementDisplayState->getPrefixString ();
        }

        m_newLinePendingInParent = m_pPreviousYangElementDisplayState->getNewLinePendingInHierarchy ();

        m_isOutputPending        = m_pPreviousYangElementDisplayState->m_isOutputPending;
    }
    else
    {
        // m_parentPrefixString        = "";
        m_newLinePendingInParent    = false;
        m_isOutputPending           = false;
    }

    // m_prefixString   = "";
    m_newLinePending    = false;
    m_outputWasPrinted  = false;
    // m_pendingOutput  = "";

    m_newLinePendingInPendingOutputString = false;
}

YangDisplayConfigurationContext::YangElementDisplayState::~YangElementDisplayState ()
{
}

YangDisplayConfigurationContext::YangElementDisplayState *YangDisplayConfigurationContext::YangElementDisplayState::getPreviousYangElementDisplayState () const
{
    return m_pPreviousYangElementDisplayState;
}

bool YangDisplayConfigurationContext::YangElementDisplayState::getIsSuppressedMode () const
{
    return m_isSuppressedMode;
}

bool YangDisplayConfigurationContext::YangElementDisplayState::getIsCompactSyntax () const
{
    return m_isCompactSyntax;
}

string YangDisplayConfigurationContext::YangElementDisplayState::getPrefixString () const
{
    if (true == m_isSuppressedMode)
    {
        return (StringUtils::joinStrings (m_parentPrefixString, m_prefixString));
    }

    return "";
}

void YangDisplayConfigurationContext::YangElementDisplayState::setPrefixString (const string &prefixString)
{
    m_prefixString = prefixString;
}

bool YangDisplayConfigurationContext::YangElementDisplayState::getNewLinePendingInHierarchy () const
{
    bool newLinePending = false;

    if ((true == m_newLinePendingInParent) || (true == m_newLinePending))
    {
        newLinePending = true;
    }

    return newLinePending;
}

bool YangDisplayConfigurationContext::YangElementDisplayState::getAndResetPendingNewLine ()
{
    bool newLinePending = getNewLinePendingInHierarchy ();

    if (true == m_newLinePendingInParent)
    {
        prismAssert (NULL != m_pPreviousYangElementDisplayState, __FILE__, __LINE__);

        bool resetStatus = m_pPreviousYangElementDisplayState->getAndResetPendingNewLine ();
        prismAssert (true == resetStatus, __FILE__, __LINE__);

        m_newLinePendingInParent = false;
    }

    if (true == m_newLinePending)
    {
        m_newLinePending = false;
    }

    return newLinePending;
}

bool YangDisplayConfigurationContext::YangElementDisplayState::getNewLinePending () const
{
    return m_newLinePending;
}

void YangDisplayConfigurationContext::YangElementDisplayState::setNewLinePending ()
{
    m_newLinePending = true;
}

void YangDisplayConfigurationContext::YangElementDisplayState::setOutputWasPrinted ()
{
    m_outputWasPrinted = true;
}

bool YangDisplayConfigurationContext::YangElementDisplayState::getOutputWasPrinted () const
{
    return m_outputWasPrinted;
}

void YangDisplayConfigurationContext::YangElementDisplayState::setPendingOutputString (const string &pendingOutputString, const UI32 &depth ,const string &newLineString)
{
    if ((NULL != m_pPreviousYangElementDisplayState) && (true == m_pPreviousYangElementDisplayState->m_isSuppressedMode))
    {
        string outputStringWithPrefix = StringUtils::joinStrings (m_pPreviousYangElementDisplayState->getPrefixString (), pendingOutputString);
        m_pendingOutputString = outputStringWithPrefix;
    }
    else
    {
        m_pendingOutputString = pendingOutputString;
    }

    if (false == m_isOutputPending)
    {
        bool newLinePending = getNewLinePendingInHierarchy ();

        if (false == newLinePending)
        {
            string spaceString ((size_t) depth, ' ');

            m_pendingOutputString = spaceString + m_pendingOutputString;
        }
    }
    else
    {
        bool newLinePending = getNewLinePendingInPendingOutputString ();

        if (false == newLinePending)
        {
            string spaceString ((size_t) depth, ' ');

            m_pendingOutputString = spaceString + m_pendingOutputString;
        }
    }

    string newLinePlaceHolder;

    //if (false == m_isCompactSyntax)
    if (false == m_isSuppressedMode)
    {
        //newLinePlaceHolder = getNewLineString ();
        newLinePlaceHolder = newLineString;
    }
    else
    {
        m_newLinePendingInPendingOutputString = true;
    }

    m_pendingOutputString += newLinePlaceHolder;

    m_isOutputPending = true;
}

void YangDisplayConfigurationContext::YangElementDisplayState::resetPendingOutputString ()
{
    m_pendingOutputString.clear ();

    m_newLinePendingInPendingOutputString = false;

    m_isOutputPending = false;
}

string YangDisplayConfigurationContext::YangElementDisplayState::getAndResetPendingOutputStringInHierarchy ()
{
    string pendingOutputString;

    if (NULL != m_pPreviousYangElementDisplayState)
    {
        m_pPreviousYangElementDisplayState->resetNewLinePendingInPendingOutputString ();
        pendingOutputString = m_pPreviousYangElementDisplayState->getAndResetPendingOutputStringInHierarchy ();
    }

    resetNewLinePendingInPendingOutputString ();

    pendingOutputString += m_pendingOutputString;

    m_pendingOutputString.clear ();

    m_isOutputPending = false;

    return pendingOutputString;
}

void YangDisplayConfigurationContext::YangElementDisplayState::setNewLinePendingInPendingOutputString ()
{
    m_newLinePendingInPendingOutputString = true;
}

bool YangDisplayConfigurationContext::YangElementDisplayState::getNewLinePendingInPendingOutputString () const
{
    if (true == m_newLinePendingInPendingOutputString)
    {
        return true;
    }

    if (NULL != m_pPreviousYangElementDisplayState)
    {
        return (m_pPreviousYangElementDisplayState->getNewLinePendingInPendingOutputString ());
    }

    return false;
}

void YangDisplayConfigurationContext::YangElementDisplayState::resetNewLinePendingInPendingOutputString ()
{
    m_newLinePendingInPendingOutputString = false;
}

void YangDisplayConfigurationContext::YangElementDisplayState::debugPrint () const
{
    tracePrintf (TRACE_LEVEL_INFO, "--");
    tracePrintf (TRACE_LEVEL_INFO, "                       isSuppressedMode [%u]", m_isSuppressedMode);
    tracePrintf (TRACE_LEVEL_INFO, "                        isCompactSyntax [%u]", m_isCompactSyntax);
    tracePrintf (TRACE_LEVEL_INFO, "                     parentPrefixString [%s]", m_parentPrefixString.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, "                           prefixString [%s]", m_prefixString.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, "                 newLinePendingInParent [%u]", m_newLinePendingInParent);
    tracePrintf (TRACE_LEVEL_INFO, "                         newLinePending [%u]", m_newLinePending);
    tracePrintf (TRACE_LEVEL_INFO, "                       isSuppressedMode [%u]", m_outputWasPrinted);
    tracePrintf (TRACE_LEVEL_INFO, "                    pendingOutputString [%s]", m_pendingOutputString.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, "    newLinePendingInPendingOutputString [%u]", m_newLinePendingInPendingOutputString);
}

bool YangDisplayConfigurationContext::YangElementDisplayState::getIsOutputPending () const
{
    return m_isOutputPending;
}

// ConfigLevel

YangDisplayConfigurationContext::ConfigLevel::ConfigLevel (vector<WaveManagedObject *> &pWaveManagedObjects, YangList *pYangList)
    : m_pWaveManagedObjects     (pWaveManagedObjects),
      m_pYangList               (pYangList),
      m_currentIndex            (0)
{
    prismAssert (false == m_pWaveManagedObjects.empty (), __FILE__, __LINE__);

    prismAssert (NULL != m_pYangList, __FILE__, __LINE__);
}

YangDisplayConfigurationContext::ConfigLevel::~ConfigLevel ()
{
    map<string, WaveManagedObject *>::iterator itr = m_configurationSegmentToPWaveManagedObjectMap.begin ();
    map<string, WaveManagedObject *>::iterator end = m_configurationSegmentToPWaveManagedObjectMap.end   ();

    for (; end != itr; itr++)
    {
        delete itr->second;
    }

    m_configurationSegmentToPWaveManagedObjectMap.clear ();

    UI32 numberOfWaveManagedObjects = m_pWaveManagedObjects.size ();

    for (UI32 i = 0; i < numberOfWaveManagedObjects; i++)
    {
        delete m_pWaveManagedObjects[i];
    }
}

WaveManagedObject *YangDisplayConfigurationContext::ConfigLevel::getPWaveManagedObjectForConfigurationSegment (const string &configurationSegmentName, bool &isKnown) const
{
    if (m_pYangList->getConfigurationSegmentName () == configurationSegmentName)
    {
        isKnown = true;

        return m_pWaveManagedObjects[m_currentIndex];
    }

    WaveManagedObject *pWaveManagedObject = NULL;

    map<string, WaveManagedObject *>::const_iterator itr = m_configurationSegmentToPWaveManagedObjectMap.find (configurationSegmentName);
    map<string, WaveManagedObject *>::const_iterator end = m_configurationSegmentToPWaveManagedObjectMap.end  ();

    if (end != itr)
    {
        pWaveManagedObject = itr->second;

        isKnown = true;
    }
    else
    {
        isKnown = false;
    }

    return pWaveManagedObject;
}

void YangDisplayConfigurationContext::ConfigLevel::addPWaveManagedObjectForConfigurationSegment (const string &configurationSegmentName, WaveManagedObject *pWaveManagedObject)
{
    prismAssert (m_pYangList->getConfigurationSegmentName () != configurationSegmentName, __FILE__, __LINE__);

    map<string, WaveManagedObject *>::const_iterator itr = m_configurationSegmentToPWaveManagedObjectMap.find (configurationSegmentName);
    map<string, WaveManagedObject *>::const_iterator end = m_configurationSegmentToPWaveManagedObjectMap.end  ();

    prismAssert (end == itr, __FILE__, __LINE__);

    m_configurationSegmentToPWaveManagedObjectMap[configurationSegmentName] = pWaveManagedObject;
}

void YangDisplayConfigurationContext::ConfigLevel::incrementCurrentIndex ()
{
    UI32 numberOfWaveManagedObjects = m_pWaveManagedObjects.size ();
    prismAssert (m_currentIndex < numberOfWaveManagedObjects, __FILE__, __LINE__);

    map<string, WaveManagedObject *>::iterator itr = m_configurationSegmentToPWaveManagedObjectMap.begin ();
    map<string, WaveManagedObject *>::iterator end = m_configurationSegmentToPWaveManagedObjectMap.end   ();

    for (; end != itr; itr++)
    {
        delete itr->second;
    }

    m_configurationSegmentToPWaveManagedObjectMap.clear ();

    m_currentIndex++;
}

void YangDisplayConfigurationContext::ConfigLevel::getKeyAttributesAtCurrentIndex (vector<Attribute *> &pKeyAttributes) const
{
    ConfigurationSegmentInformation *pConfigurationSegmentInformation = m_pYangList->getPConfigurationSegmentInformation ();
    prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

    const vector<string> &userDefinedKeyCombination = pConfigurationSegmentInformation->getUserDefinedKeyCombination ();
    UI32 numberOfAttributeNamesInUserDefinedKeyCombination = userDefinedKeyCombination.size ();

    WaveManagedObject *pWaveManagedObject = m_pWaveManagedObjects[m_currentIndex];
    prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

    for (UI32 i = 0; i < numberOfAttributeNamesInUserDefinedKeyCombination; i++)
    {
        Attribute *pAttribute = pWaveManagedObject->getAttributeByName (userDefinedKeyCombination[i]);
        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

#if 0
        Attribute *pClonedAttribute = pAttribute->clone ();
        prismAssert (NULL != pClonedAttribute, __FILE__, __LINE__);

        pKeyAttributes.push_back (pClonedAttribute);
#endif

        pKeyAttributes.push_back (pAttribute);
    }
}

void YangDisplayConfigurationContext::ConfigLevel::getObjectIdForManagedObject (const string &managedObjectClassName, bool &found, ObjectId &objectId) const
{
    WaveManagedObject *pWaveManagedObject = m_pWaveManagedObjects[m_currentIndex];
    prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

    string availableManagedObjectClassName = pWaveManagedObject->getObjectClassName ();

    if (availableManagedObjectClassName == managedObjectClassName)
    {
        found = true;

        objectId = pWaveManagedObject->getObjectId ();
    }
    else
    {
        if (true == OrmRepository::isATableNameInHierarchy (availableManagedObjectClassName, managedObjectClassName))
        {
            found = true;

            objectId = pWaveManagedObject->getObjectId ();
        }
        else
        {
            // check reverse relationship -- keys should be same ?
            trace (TRACE_LEVEL_DEBUG, string ("YangDisplayConfigurationContext::ConfigLevel::getObjectIdForManagedObject : not found : ") + managedObjectClassName + string (", available : ") + availableManagedObjectClassName);
        }
    }
}

void YangDisplayConfigurationContext::ConfigLevel::setNextBatchOfWaveManagedObjects (vector<WaveManagedObject *> &pWaveManagedObjects, YangList *pYangList)
{
    // Assert checks begin [

    // pYangList should match with the YangList for which this (i.e. last) ConfigLevel was created.

    prismAssert (m_pYangList->getConfigurationSegmentName () == pYangList->getConfigurationSegmentName (), __FILE__, __LINE__);
    prismAssert (true == m_configurationSegmentToPWaveManagedObjectMap.empty (), __FILE__, __LINE__);
    prismAssert (false == pWaveManagedObjects.empty (), __FILE__, __LINE__);

    UI32 numberOfWaveManagedObjects = m_pWaveManagedObjects.size ();

    prismAssert (m_currentIndex == numberOfWaveManagedObjects, __FILE__, __LINE__);

    // Assert checks end   ]

    // Delete previously queried WaveManagedObjects.

    for (UI32 i = 0; i < numberOfWaveManagedObjects; i++)
    {
        delete m_pWaveManagedObjects[i];
    }

    // Set new set of WaveManagedObjects.

    m_currentIndex = 0;

    m_pWaveManagedObjects = pWaveManagedObjects;
}

// YangDisplayConfigurationContext

YangDisplayConfigurationContext::YangDisplayConfigurationContext ()
    : WaveXPathExpressionEvaluationContext  (),
      m_pFile                           (stdout),
      m_pManagedObjectsToDisplay        (NULL),
      m_configurationSchema             (OrmRepository::getWaveCurrentSchema ()),
      m_depth                           (0),
      m_pLastYangElementDisplayState    (NULL),
      m_currentIndexForCliTokens        (0),
      m_currentCommandKeyIndex          (0),
      m_configFilterType                (WAVE_YANG_CONFIG_READ_FILTER_NONE),
      m_ownerWaveNodeObjectIdFilterIsSet(false),
      m_anyOutputDisplayed              (false)
{
    m_firstOutputIndex = 0;
    m_printDefaultValuesFlag = false;
    m_batchSizeForQueryingListInstances = YangDisplayConfigurationContext::s_defaultBatchSizeForQueryingListInstances;

    m_pYangUserInterface                = NULL;
    m_newLineString                     = "\n";
}

YangDisplayConfigurationContext::~YangDisplayConfigurationContext ()
{
    if (NULL != m_pFile)
    {
        fclose (m_pFile);

        m_pFile = NULL;
    }

    prismAssert (true == m_keyValueVector.empty (), __FILE__, __LINE__);
    prismAssert (0 == m_currentIndexForCliTokens, __FILE__, __LINE__);

    map<string, WaveManagedObject *>::iterator itr = m_topLevelConfigurationSegmentToPWaveManagedObjectMap.begin ();
    map<string, WaveManagedObject *>::iterator end = m_topLevelConfigurationSegmentToPWaveManagedObjectMap.end ();

    for (; end != itr; itr++)
    {
        WaveManagedObject *pWaveManagedObject = itr->second;

        if (NULL != pWaveManagedObject)
        {
            delete pWaveManagedObject;
        }
    }

    m_topLevelConfigurationSegmentToPWaveManagedObjectMap.clear ();
}

FILE *YangDisplayConfigurationContext::getPFile () const
{
    return (m_pFile);
}

void YangDisplayConfigurationContext::setPFile (FILE *pFile)
{
    m_pFile = pFile;
}

vector<WaveManagedObject *> *YangDisplayConfigurationContext::getPManagedObjectsToDisplay ()
{
    return (m_pManagedObjectsToDisplay);
}

void YangDisplayConfigurationContext::setPManagedObjectsToDisplay (vector<WaveManagedObject *> *pManagedObjectsToDisplay)
{
    m_pManagedObjectsToDisplay = pManagedObjectsToDisplay;
}

void YangDisplayConfigurationContext::setConfigurationSchema (const string &configurationSchema)
{
    m_configurationSchema = configurationSchema;
}

string YangDisplayConfigurationContext::getConfigurationSchema () const
{
    return m_configurationSchema;
}

void YangDisplayConfigurationContext::incrementDepth ()
{
    m_depth++;

    prismAssert (0 != m_depth, __FILE__, __LINE__);
}

void YangDisplayConfigurationContext::decrementDepth ()
{
    prismAssert (0 != m_depth, __FILE__, __LINE__);

    m_depth--;
}

string YangDisplayConfigurationContext::getSpacePrefixString () const
{
    return string (m_depth, ' ');
}

void YangDisplayConfigurationContext::printOutput (const string &outputStringForDisplay)
{
    if ((0 < getNumberOfRemainingTokens ()) && (false == m_printingAllowed.back ()))
    {
        UI32 currentIndex = m_partialOutputBuffer.size () - 1;

        if ((false == outputStringForDisplay.empty ()) && (0 == m_firstOutputIndex))
        {
            m_firstOutputIndex = currentIndex;
        }

        prismAssert (0 < m_partialOutputBuffer.size (), __FILE__, __LINE__);

        (m_partialOutputBuffer.back ()).append (outputStringForDisplay);
    }
    else 
    {
        if (0 < m_partialOutputBuffer.size ())
        {
            UI32 i = 0;

            for (vector<bool>::iterator itr = m_printingAllowed.begin (); itr != m_printingAllowed.end (); itr++, i++)
            {
                if (i < m_firstOutputIndex)
                {
                    continue;
                }

                if (false == (*itr))
                {
                    printOutputToCliSessionBuffered (m_partialOutputBuffer[i]);
                    (m_partialOutputBuffer[i]).clear ();

                    //if (i != m_firstOutputIndex)
                    {
                        (*itr) = true;
                    }
                }
            }
        }

        printOutputToCliSessionBuffered (outputStringForDisplay);

        setAnyOutputDisplayed ();
    }
}

void YangDisplayConfigurationContext::printCustomTemplateConfigOutput (const string &outputStringForDisplay)
{
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    printPendingOutput ();

    bool pendingNewLine = pYangElementDisplayState->getAndResetPendingNewLine ();

    if (true == pendingNewLine)
    {
        printString ("");
    }

    printOutput (outputStringForDisplay);
}

void YangDisplayConfigurationContext::printOutputToCliSessionBuffered (const string &outputStringForDisplay)
{
    m_finalOutputBuffered.append (outputStringForDisplay);

    UI32 totalSizeOfOutputContents = m_finalOutputBuffered.size ();

    if (255 < totalSizeOfOutputContents)
    {
        printOutputToCliSessionWrapper (m_finalOutputBuffered);

        m_finalOutputBuffered.clear ();
    }
}

void YangDisplayConfigurationContext::flushBufferedOutput ()
{
    if (0 < m_finalOutputBuffered.size ())
    {
        printOutputToCliSessionWrapper (m_finalOutputBuffered);

        m_finalOutputBuffered.clear ();
    }
}

void YangDisplayConfigurationContext::addSlotForPartialOutput ()
{
    m_partialOutputBuffer.push_back ("");
    m_printingAllowed.push_back (false);
}

void YangDisplayConfigurationContext::printOutputToCliSessionWrapper (const string &outputStringForDisplay)
{
    unlockYangUserInterface ();     // ] lock is already held. (immediately after 'this' is created.)

    printOutputToCliSession (outputStringForDisplay);

    lockYangUserInterface ();       // [ get the lock. (Now, this thread will again start traversing YangUserInterface tree.)
}

// virtual
void YangDisplayConfigurationContext::printOutputToCliSession (const string &outputStringForDisplay)
{
}

void YangDisplayConfigurationContext::discardContentsOfPartialOutputBuffer ()
{
    prismAssert (0 < m_partialOutputBuffer.size (), __FILE__, __LINE__);

    if (m_firstOutputIndex == (m_partialOutputBuffer.size () - 1))
    {
        m_firstOutputIndex = 0;
    }

    m_partialOutputBuffer.pop_back ();
    m_printingAllowed.pop_back ();
}

void YangDisplayConfigurationContext::printString (const string &outputStringForDisplay, const bool &printNewLine)
{
    if (true == outputStringForDisplay.empty ())
    {
        if (true == printNewLine)
        {
            printOutput (getNewLineString ());
        }
        return;
    }

    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();

    if (NULL != pYangElementDisplayState)
    {
        bool newLinePending = pYangElementDisplayState->getNewLinePendingInHierarchy ();

        if (false == newLinePending)
        {
            string spaceString ((size_t) m_depth, ' ');

            printOutput (spaceString);
        }
    }
    else
    {
        string spaceString ((size_t) m_depth, ' ');

        printOutput (spaceString);
    }

    string newLinePlaceHolder = "";

    if (true == printNewLine)
    {
        newLinePlaceHolder = getNewLineString ();

        if (NULL != pYangElementDisplayState)
        {
            pYangElementDisplayState->getAndResetPendingNewLine ();
        }
    }
    else
    {
        if (NULL != pYangElementDisplayState)
        {
            pYangElementDisplayState->setNewLinePending ();
        }
    }

    printOutput (outputStringForDisplay + newLinePlaceHolder);
}

void YangDisplayConfigurationContext::printNewLineIfRequired ()
{
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    bool newLinePending = pYangElementDisplayState->getAndResetPendingNewLine ();

    if (true == newLinePending)
    {
        printString ("");
    }
}

void YangDisplayConfigurationContext::printSeparator ()
{
    printNewLineIfRequired ();
    printString ("!");
}

void YangDisplayConfigurationContext::setCliString (const string &cliString)
{
    m_cliString = cliString;

    StringUtils::tokenize (cliString, m_cliTokens);

    // for (vector<string>::iterator itr = m_cliTokens.begin (); itr != m_cliTokens.end (); itr++)
    // {
    //     trace (TRACE_LEVEL_INFO, string ("YangDisplayConfigurationContext::setCliString : token [") + (*itr) + string ("]"));
    // }
}

string YangDisplayConfigurationContext::advanceToNextToken ()
{
    UI32 numberOfCliTokens = m_cliTokens.size ();

    prismAssert (m_currentIndexForCliTokens < numberOfCliTokens, __FILE__, __LINE__);

    string tokenString = m_cliTokens[m_currentIndexForCliTokens];

    m_currentIndexForCliTokens++;

    // trace (TRACE_LEVEL_INFO, string ("YangDisplayConfigurationContext::advanceToNextToken : token [") + tokenString + string ("]"));

    return tokenString;
}

void YangDisplayConfigurationContext::decrementTokenPosition ()
{
    prismAssert (0 != m_currentIndexForCliTokens, __FILE__, __LINE__);
    m_currentIndexForCliTokens--;
}

UI32 YangDisplayConfigurationContext::getNumberOfRemainingTokens () const
{
    UI32 numberOfTokens = m_cliTokens.size ();

    prismAssert (m_currentIndexForCliTokens <= numberOfTokens, __FILE__, __LINE__);

    return (numberOfTokens - m_currentIndexForCliTokens);
}

void YangDisplayConfigurationContext::querySynchronously (WaveManagedObjectSynchronousQueryContext &waveManagedObjectSynchronousQueryContext, const string &managedObjectClassName, vector<WaveManagedObject *> &pWaveManagedObjects, const bool &disableFilters) const
{
    if (false == disableFilters)
    {
        if (true == getIsOwnerWaveNodeObjectIdFilterSet ())
        {
            if (true == OrmRepository::isALocalManagedObject (managedObjectClassName))
            {
                waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (getOwnerWaveNodeObjectIdFilter (), "ownerWaveNodeObjectId"));
            }
        }
    }

    unlockYangUserInterface ();     // ] lock is already held. (immediately after 'this' is created.)

    vector<WaveManagedObject *> *pResults = WaveObjectManagerToolKit::querySynchronously (&waveManagedObjectSynchronousQueryContext);

    lockYangUserInterface ();       // [ get the lock. (Now, this thread will again start traversing YangUserInterface tree.)

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    pResults->swap (pWaveManagedObjects);
    delete pResults;
}

YangDisplayConfigurationContext::ConfigLevel *YangDisplayConfigurationContext::getLastConfigLevel ()
{
    UI32 numberOfConfigLevels = m_pConfigLevels.size ();
    prismAssert (0 < numberOfConfigLevels, __FILE__, __LINE__);

    return m_pConfigLevels[numberOfConfigLevels - 1];
}

void YangDisplayConfigurationContext::getKeyAttributesAtConfigLevel (const UI32 &indexInConfigLevels, vector<Attribute *> &pKeyAttributes) const
{
    UI32 numberOfConfigLevels = m_pConfigLevels.size ();
    prismAssert (indexInConfigLevels < numberOfConfigLevels, __FILE__, __LINE__);

    ConfigLevel *pConfigLevel = m_pConfigLevels[indexInConfigLevels];
    prismAssert (NULL != pConfigLevel, __FILE__, __LINE__);

    pConfigLevel->getKeyAttributesAtCurrentIndex (pKeyAttributes);
}

void YangDisplayConfigurationContext::getManagedObjectIdForLastLevelInHierarchy (YangElement *pYangElement, ObjectId &ownerObjectId) const
{
    prismAssert (NULL != pYangElement, __FILE__, __LINE__);

    ConfigurationSegmentInformation *pConfigurationSegmentInformation = pYangElement->getPConfigurationSegmentInformation ();
    prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

    UI32 numberOfManagedObjectClassNamesInOrmHierarchy = pConfigurationSegmentInformation->getNumberOfManagedObjectClassNamesInOrmHierarchy ();

    UI32 indexInConfigLevels                = 0;
    bool addOwnerWaveNodeObjectIdFilter     = false;
    bool addOwnerPartitionObjectIdFilter    = false;

    UI32 i = 0;

    bool composedInAutoAssociationParentManagedObject = false;

    if ((0 < numberOfManagedObjectClassNamesInOrmHierarchy) && (WaveConfigurationSegmentMap::getManagedObjectClassNameForNodeSpecificList () == pConfigurationSegmentInformation->getParentManagedObjectClassNameAtIndex (numberOfManagedObjectClassNamesInOrmHierarchy - 1)))
    {
        bool found  = false;
        getObjectIdForManagedObjectAtLevel (indexInConfigLevels, WaveConfigurationSegmentMap::getManagedObjectClassNameForNodeSpecificList (), found, ownerObjectId);

        composedInAutoAssociationParentManagedObject = true;

        i++;
    }
    else if ((0 < numberOfManagedObjectClassNamesInOrmHierarchy) && (WaveConfigurationSegmentMap::getManagedObjectClassNameForMultiPartitionList () == pConfigurationSegmentInformation->getParentManagedObjectClassNameAtIndex (numberOfManagedObjectClassNamesInOrmHierarchy - 1)))
    {
        bool found  = false;
        getObjectIdForManagedObjectAtLevel (1, WaveConfigurationSegmentMap::getManagedObjectClassNameForMultiPartitionList (), found, ownerObjectId);

        composedInAutoAssociationParentManagedObject = true;

        i++;
    }

    if (true == pYangElement->getIsInNodeSpecificTree ())
    {
        indexInConfigLevels++;

        if (false == composedInAutoAssociationParentManagedObject)
        {
            addOwnerWaveNodeObjectIdFilter = true;
        }

        if (true == pYangElement->getIsInMultiPartitionTree ())
        {
            indexInConfigLevels++;

            if (false == composedInAutoAssociationParentManagedObject)
            {
                addOwnerPartitionObjectIdFilter = true;
            }
        }

        addOwnerWaveNodeObjectIdFilter = true;
    }

    UI32    numberOfConfigLevels        = m_pConfigLevels.size ();

    if ((indexInConfigLevels >= numberOfConfigLevels) && (0 != numberOfConfigLevels) && (0 != indexInConfigLevels))
    {
        return;
    }

    // Special exception to match.

    UI32 iterationNumber = 0;

    for (; i < numberOfManagedObjectClassNamesInOrmHierarchy; i++, iterationNumber++)
    {
        string  parentManagedObjectClassName    = pConfigurationSegmentInformation->getParentManagedObjectClassNameAtIndex (i);
        bool    isSingleton                     = pConfigurationSegmentInformation->getParentIsSingletonAtIndex (i);

        if ((false == isSingleton) && (0 != numberOfConfigLevels))
        {
            bool found = false;

            getObjectIdForManagedObjectAtLevel (indexInConfigLevels, parentManagedObjectClassName, found, ownerObjectId);

            if (true == found)
            {
                if (true == addOwnerWaveNodeObjectIdFilter)
                {
                    addOwnerWaveNodeObjectIdFilter = false;
                }

                if (true == addOwnerPartitionObjectIdFilter)
                {
                    addOwnerPartitionObjectIdFilter = false;
                }

                indexInConfigLevels++;

                continue;
            }
        }

        WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (parentManagedObjectClassName);
        waveManagedObjectSynchronousQueryContext.setSchemaToQuery (getConfigurationSchema ());
        waveManagedObjectSynchronousQueryContext.setLoadCompositions (false);
        waveManagedObjectSynchronousQueryContext.addSelectFieldForObjectId ("objectId");

        if ((0 == iterationNumber) && (1 == numberOfManagedObjectClassNamesInOrmHierarchy))
        {
            // pass. : special case to match behaviour.
        }
        else
        {
            waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (ownerObjectId, "ownerManagedObjectId"));
        }

        if (true == addOwnerWaveNodeObjectIdFilter)
        {
            addOwnerWaveNodeObjectIdFilter = false;

            ObjectId ownerWaveNodeObjectId = getOwnerWaveNodeObjectId ();

            waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (ownerWaveNodeObjectId, "ownerWaveNodeObjectId"));

            if (true == pConfigurationSegmentInformation->getPartitionSupported ())
            {
                ObjectId ownerPartitionObjectId;

                if (true == addOwnerPartitionObjectIdFilter)
                {
                    addOwnerPartitionObjectIdFilter = false;
                    ownerPartitionObjectId          = getOwnerPartitionObjectId ();
                }

                waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (ownerPartitionObjectId, "ownerPartitionManagedObjectId"));
            }
        }

        if (false == isSingleton)
        {
            vector<Attribute *> pKeyAttributes;
            getKeyAttributesAtConfigLevel (indexInConfigLevels, pKeyAttributes);

            ResourceId addAndAttributesForKeyValuesStatus = addAndAttributesForKeyValues (waveManagedObjectSynchronousQueryContext, parentManagedObjectClassName, pKeyAttributes, pYangElement->getIsInNodeSpecificTree ());

            if (WAVE_MESSAGE_SUCCESS != addAndAttributesForKeyValuesStatus)
            {
                ownerObjectId = ObjectId::NullObjectId;

                return;
            }

            indexInConfigLevels++;
        }

        if (0 == i) // last ?
        {
            // pConfigurationSegmentInformation->applyCustomQueryFilterFunction (waveManagedObjectSynchronousQueryContext);
        }

        vector<WaveManagedObject *> pParentWaveManagedObjects;

        querySynchronously (waveManagedObjectSynchronousQueryContext, parentManagedObjectClassName, pParentWaveManagedObjects);

        UI32 numberOfPParentWaveManagedObjects = pParentWaveManagedObjects.size ();

        if (0 == numberOfPParentWaveManagedObjects)
        {
            ownerObjectId = ObjectId::NullObjectId;
            break;
        }
        else if (1 == numberOfPParentWaveManagedObjects)
        {
            WaveManagedObject *pParentWaveManagedObject = pParentWaveManagedObjects[0];
            prismAssert (NULL != pParentWaveManagedObject, __FILE__, __LINE__);

            ownerObjectId = pParentWaveManagedObject->getObjectId ();

            delete pParentWaveManagedObject;
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("YangDisplayConfigurationContext::getManagedObjectIdForLastLevelInHierarchy : multiple managed objects found. (") + pYangElement->getYangName () + string (":") + pYangElement->getName () + string (" : ") + pYangElement->getCliTargetNodeName () + string ("). Details -"));
            debugPrintErrorInformation (waveManagedObjectSynchronousQueryContext, pParentWaveManagedObjects);
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void YangDisplayConfigurationContext::getObjectIdForManagedObjectAtLevel (const UI32 &indexInConfigLevels, const string &managedObjectClassName, bool &found, ObjectId &objectId) const
{
    UI32 numberOfConfigLevels = m_pConfigLevels.size ();
    prismAssert (indexInConfigLevels < numberOfConfigLevels, __FILE__, __LINE__);

    ConfigLevel *pConfigLevel = m_pConfigLevels[indexInConfigLevels];
    prismAssert (NULL != pConfigLevel, __FILE__, __LINE__);

    pConfigLevel->getObjectIdForManagedObject (managedObjectClassName, found, objectId);
}

void YangDisplayConfigurationContext::getPWaveManagedObjects (YangList *pYangList, vector<WaveManagedObject *> &pWaveManagedObjects, const UI32 &pageNumber)
{
    ConfigurationSegmentInformation *pConfigurationSegmentInformation = pYangList->getPConfigurationSegmentInformation ();
    prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

    string managedObjectClassName = pConfigurationSegmentInformation->getManagedObjectClassName ();

#if 0
    // Filter with behaviour similar to existing.

    if (true == isConfigFilterApplicable (pYangList))
    {
        return;
    }
#endif

    WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (managedObjectClassName);
    waveManagedObjectSynchronousQueryContext.setSchemaToQuery (getConfigurationSchema ());
    waveManagedObjectSynchronousQueryContext.setLoadCompositions (false);

    UI32 numberOfManagedObjectClassNamesInOrmHierarchy = pConfigurationSegmentInformation->getNumberOfManagedObjectClassNamesInOrmHierarchy ();

    if (0 != numberOfManagedObjectClassNamesInOrmHierarchy)
    {
        ObjectId ownerObjectId;

        if (0 != numberOfManagedObjectClassNamesInOrmHierarchy)
        {
            getManagedObjectIdForLastLevelInHierarchy (pYangList, ownerObjectId);
        }

        waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (&ownerObjectId, "ownerManagedObjectId"));
    }
    else
    {
        if (true == pYangList->getIsInNodeSpecificTree ())
        {
            ObjectId ownerWaveNodeObjectId = getOwnerWaveNodeObjectId ();

            waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (ownerWaveNodeObjectId, "ownerWaveNodeObjectId"));

            if (true == pConfigurationSegmentInformation->getPartitionSupported ())
            {
                ObjectId ownerPartitionObjectId = getOwnerPartitionObjectId ();

                waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (ownerPartitionObjectId, "ownerPartitionManagedObjectId"));
            }
        }
    }

    vector<string> userInputKeyValues;

    getCurrentCommnadKey (userInputKeyValues);

    UI32 numberOfUserInputKeys = userInputKeyValues.size ();

    if (0 != numberOfUserInputKeys)
    {
        const vector<string> &keyStringVector = pConfigurationSegmentInformation->getUserDefinedKeyCombination ();

        for (UI32 i = 0; i < numberOfUserInputKeys; i++)
        {
            //check if the key contains any regex characters like '*'
            string keyValue         = userInputKeyValues[i];
            string keyAttributeName = keyStringVector[i];
                 
            size_t found = keyValue.find_first_of ("*[(|");

            if (found != string::npos)
            {
                //replace characters not supported by postgress by the one supported 
                StringUtils::replaceAllInstancesOfInputCharWith (keyValue, '*', '%');

                Attribute *atterString = new AttributeString (keyValue, keyAttributeName);

                waveManagedObjectSynchronousQueryContext.addAndAttribute (atterString, "SIMILAR TO");
            }
            else
            {
                string inputKeyValueWithProperKeyType;

                ResourceId keyWithTypeStatus = pYangList->getStringRepresentationForKeyValueAtIndex (i, keyValue, inputKeyValueWithProperKeyType);

                if (WAVE_MESSAGE_SUCCESS != keyWithTypeStatus)
                {
                    trace (TRACE_LEVEL_ERROR, string ("YangDisplayConfigurationContext::getPWaveManagedObjects [") + keyAttributeName + string ("]"));
                    return;
                }

                Attribute *pAttribute = pConfigurationSegmentInformation->getClonedAttributeForKey (keyAttributeName);
                prismAssert (NULL != pAttribute, __FILE__, __LINE__);

                AttributeObjectIdAssociation *pAttributeObjectIdAssociation = dynamic_cast<AttributeObjectIdAssociation *> (pAttribute);

                if (NULL == pAttributeObjectIdAssociation)
                {
                    ResourceId loadFromPlainStringStatus = pAttribute->loadFromPlainString (inputKeyValueWithProperKeyType);

                    if (WAVE_MESSAGE_SUCCESS != loadFromPlainStringStatus)
                    {
                        delete pAttribute;
                        return;
                    }

                }
                else
                {
                    ResourceId associatedObjectIdQueryStatus = getObjectIdOfAssocociatedManagedObjectWithKey (inputKeyValueWithProperKeyType, pAttributeObjectIdAssociation, pYangList->getIsInNodeSpecificTree ());

                    if (WAVE_MESSAGE_SUCCESS != associatedObjectIdQueryStatus)
                    {
                        delete pAttribute;
                        return;
                    }
                }

                waveManagedObjectSynchronousQueryContext.addAndAttribute (pAttribute);
            }
        }
    }

    bool customOrderFieldApplied = pConfigurationSegmentInformation->applyAddCustomOrderFieldFunction (waveManagedObjectSynchronousQueryContext);

    if (false == customOrderFieldApplied)
    {
        const vector<string> &userDefinedKeyCombination = pConfigurationSegmentInformation->getUserDefinedKeyCombination ();

        UI32 numberOfAttributeNamesInUserDefinedKeyCombination = userDefinedKeyCombination.size ();

        bool ascendingOrder = true;

        for (UI32 i = 0; i < numberOfAttributeNamesInUserDefinedKeyCombination; i++)
        {
            if (AttributeType::AttributeTypeObjectId != pConfigurationSegmentInformation->getAttributeTypeForAttributeInKeyCombination (userDefinedKeyCombination[i]))
            {
                waveManagedObjectSynchronousQueryContext.addOrderField (userDefinedKeyCombination[i], ascendingOrder);
            }
        }
    }

    pConfigurationSegmentInformation->applySetNTupleSortingFunction (waveManagedObjectSynchronousQueryContext);
    pConfigurationSegmentInformation->applyCustomQueryFilterFunction (waveManagedObjectSynchronousQueryContext);

    if (0 != getBatchSizeForQueryingListInstances ())
    {
        waveManagedObjectSynchronousQueryContext.addLimitForQueryResults (getBatchSizeForQueryingListInstances (), pageNumber * (getBatchSizeForQueryingListInstances ()));
    }

    querySynchronously (waveManagedObjectSynchronousQueryContext, managedObjectClassName, pWaveManagedObjects);
}

void YangDisplayConfigurationContext::getPWaveManagedObject (YangElement *pYangElement, WaveManagedObject *&pWaveManagedObject, const bool &disableFilters)
{
    prismAssert (NULL != pYangElement, __FILE__, __LINE__);

    ConfigurationSegmentInformation *pConfigurationSegmentInformation = pYangElement->getPConfigurationSegmentInformation ();
    prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

    string managedObjectClassName = pConfigurationSegmentInformation->getManagedObjectClassName ();

#if 0
    // Filter with behaviour similar to existing.

    if (false == disableFilters)
    {
        if (true == isConfigFilterApplicable (pYangElement))
        {
            return;
        }
    }
#endif

    UI32            numberOfConfigLevels        = m_pConfigLevels.size ();
    ConfigLevel    *pConfigLevel                = NULL;
    string          configurationSegmentName    = pYangElement->getConfigurationSegmentName ();

    if (0 < numberOfConfigLevels)
    {
        pConfigLevel = getLastConfigLevel ();
        prismAssert (NULL != pConfigLevel, __FILE__, __LINE__);

        bool    isKnown                     = false;

        pWaveManagedObject = pConfigLevel->getPWaveManagedObjectForConfigurationSegment (configurationSegmentName, isKnown);

        if (true == isKnown)
        {
            return;
        }
    }
    else
    {
        map<string, WaveManagedObject *>::iterator itr = m_topLevelConfigurationSegmentToPWaveManagedObjectMap.find (configurationSegmentName);
        map<string, WaveManagedObject *>::iterator end = m_topLevelConfigurationSegmentToPWaveManagedObjectMap.end ();

        if (end != itr)
        {
            pWaveManagedObject = itr->second;

            return;
        }
    }

    UI32 numberOfManagedObjectClassNamesInOrmHierarchy = pConfigurationSegmentInformation->getNumberOfManagedObjectClassNamesInOrmHierarchy ();

    ObjectId ownerObjectId;

    if (0 != numberOfManagedObjectClassNamesInOrmHierarchy)
    {
        getManagedObjectIdForLastLevelInHierarchy (pYangElement, ownerObjectId);
    }

    WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (managedObjectClassName);
    waveManagedObjectSynchronousQueryContext.setSchemaToQuery (getConfigurationSchema ());
    waveManagedObjectSynchronousQueryContext.setLoadCompositions (false);

    if (0 != numberOfManagedObjectClassNamesInOrmHierarchy)
    {
        waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (&ownerObjectId, "ownerManagedObjectId"));
    }
    else
    {
        UI32 numberOfConfigLevelsToBeProcessed = m_pConfigLevels.size ();

        if (true == pYangElement->getIsInNodeSpecificTree ())
        {
            numberOfConfigLevelsToBeProcessed--;

            ObjectId ownerWaveNodeObjectId = getOwnerWaveNodeObjectId ();

            waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (ownerWaveNodeObjectId, "ownerWaveNodeObjectId"));

            if (true == pConfigurationSegmentInformation->getPartitionSupported ())
            {
                ObjectId ownerPartitionObjectId = getOwnerPartitionObjectId ();

                waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (ownerPartitionObjectId, "ownerPartitionManagedObjectId"));
            }
        }

        if (0 < numberOfConfigLevelsToBeProcessed)
        {
            const vector<string> &userDefinedKeyCombination = pConfigurationSegmentInformation->getUserDefinedKeyCombination ();

            UI32 numberOfAttributeNamesInUserDefinedKeyCombination = userDefinedKeyCombination.size ();

            vector<Attribute *> pKeyAttributes;
            getKeyAttributesAtConfigLevel (numberOfConfigLevelsToBeProcessed - 1, pKeyAttributes);
            UI32 numberOfKeyAttributes = pKeyAttributes.size ();

            // Few elements under a list dont have keys !, the top level lists are like singlton... Do not assert for now.
            // prismAssert (numberOfAttributeNamesInUserDefinedKeyCombination == numberOfKeyAttributes, __FILE__, __LINE__);

            if (numberOfAttributeNamesInUserDefinedKeyCombination == numberOfKeyAttributes)
            {
                    ResourceId addAndAttributesForKeyValuesStatus = addAndAttributesForKeyValues (waveManagedObjectSynchronousQueryContext, managedObjectClassName, pKeyAttributes, pYangElement->getIsInNodeSpecificTree ());

                    if (WAVE_MESSAGE_SUCCESS != addAndAttributesForKeyValuesStatus)
                    {
                        return;
                    }
            }
        }

        pConfigurationSegmentInformation->applyCustomQueryFilterFunction (waveManagedObjectSynchronousQueryContext);
    }

    vector<WaveManagedObject *> pWaveManagedObjects;
    querySynchronously (waveManagedObjectSynchronousQueryContext, managedObjectClassName, pWaveManagedObjects, disableFilters);

    UI32 numberOfResults = pWaveManagedObjects.size ();

    if (1 == numberOfResults)
    {
        pWaveManagedObject = pWaveManagedObjects[0];
    }
    else if (0 == numberOfResults)
    {
        pWaveManagedObject = NULL;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("YangDisplayConfigurationContext::getPWaveManagedObject : multiple managed objects found. (") + pYangElement->getYangName () + string (":") + pYangElement->getName () + string (" : ") + pYangElement->getCliTargetNodeName () + string ("). Details -"));
        debugPrintErrorInformation (waveManagedObjectSynchronousQueryContext, pWaveManagedObjects);
        prismAssert (false, __FILE__, __LINE__);
    }

    if (0 < numberOfConfigLevels)
    {
        pConfigLevel->addPWaveManagedObjectForConfigurationSegment (configurationSegmentName, pWaveManagedObject);
    }
    else
    {
        addPWaveManagedObjectForTopLevelConfigurationSegment (configurationSegmentName, pWaveManagedObject);
    }
}

void YangDisplayConfigurationContext::addPWaveManagedObjectForTopLevelConfigurationSegment (const string &configurationSegmentName, WaveManagedObject *pWaveManagedObject)
{
    map<string, WaveManagedObject *>::iterator itr = m_topLevelConfigurationSegmentToPWaveManagedObjectMap.find (configurationSegmentName);
    map<string, WaveManagedObject *>::iterator end = m_topLevelConfigurationSegmentToPWaveManagedObjectMap.end ();

    prismAssert (end == itr, __FILE__, __LINE__);

    m_topLevelConfigurationSegmentToPWaveManagedObjectMap[configurationSegmentName] = pWaveManagedObject;
}

void YangDisplayConfigurationContext::setOwnerWaveNodeObjectId (const ObjectId &ownerWaveNodeObjectId)
{
    m_ownerWaveNodeObjectId = ownerWaveNodeObjectId;
}

void YangDisplayConfigurationContext::resetOwnerWaveNodeObjectId ()
{
    m_ownerWaveNodeObjectId = ObjectId::NullObjectId;
}

ObjectId YangDisplayConfigurationContext::getOwnerWaveNodeObjectId () const
{
    return m_ownerWaveNodeObjectId;
}

void YangDisplayConfigurationContext::setOwnerPartitionObjectId (const ObjectId &ownerPartitionObjectId)
{
    m_ownerPartitionObjectId = ownerPartitionObjectId;
}

void YangDisplayConfigurationContext::resetOwnerPartitionObjectId ()
{
    m_ownerPartitionObjectId = ObjectId::NullObjectId;
}

ObjectId YangDisplayConfigurationContext::getOwnerPartitionObjectId () const
{
    return m_ownerPartitionObjectId;
}

void YangDisplayConfigurationContext::addConfigurationLevel (vector<WaveManagedObject *> &pWaveManagedObjects, YangList *pYangList)
{
    ConfigLevel *pConfigLevel = new ConfigLevel (pWaveManagedObjects, pYangList);
    prismAssert (NULL != pConfigLevel, __FILE__, __LINE__);

    m_pConfigLevels.push_back (pConfigLevel);
}

void YangDisplayConfigurationContext::addNextBatchOfWaveManagedObjects (vector<WaveManagedObject *> &pWaveManagedObjects, YangList *pYangList)
{
    ConfigLevel *pConfigLevel = getLastConfigLevel ();
    prismAssert (NULL != pConfigLevel, __FILE__, __LINE__);

    pConfigLevel->setNextBatchOfWaveManagedObjects (pWaveManagedObjects, pYangList);
}

void YangDisplayConfigurationContext::deleteLastConfigLevel ()
{
    UI32 numberOfConfigLevels = m_pConfigLevels.size ();
    prismAssert (0 < numberOfConfigLevels, __FILE__, __LINE__); // default config level should always be present.

    ConfigLevel *pConfigLevel = m_pConfigLevels.back ();
    m_pConfigLevels.pop_back ();

    delete pConfigLevel;
}

void YangDisplayConfigurationContext::incrementIndexForLastConfigLevel ()
{
    ConfigLevel *pConfigLevel = getLastConfigLevel ();
    prismAssert (NULL != pConfigLevel, __FILE__, __LINE__);

    pConfigLevel->incrementCurrentIndex ();
}

void YangDisplayConfigurationContext::addYangElementDisplayState (const bool &isSuppressedMode, const bool &isCompactSyntax)
{
    YangElementDisplayState *pYangElementDisplayState = new YangElementDisplayState (m_pLastYangElementDisplayState, isSuppressedMode, isCompactSyntax);
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    m_pLastYangElementDisplayState = pYangElementDisplayState;
}

YangDisplayConfigurationContext::YangElementDisplayState *YangDisplayConfigurationContext::getLastYangElementDisplayState () const
{
    return m_pLastYangElementDisplayState;
}

void YangDisplayConfigurationContext::removeLastYangElementDisplayState ()
{
    prismAssert (NULL != m_pLastYangElementDisplayState, __FILE__, __LINE__);

    YangElementDisplayState *pPreviousYangElementDisplayState = m_pLastYangElementDisplayState->getPreviousYangElementDisplayState ();

    if (true == m_pLastYangElementDisplayState->getNewLinePending ())
    {
        if (NULL != pPreviousYangElementDisplayState)
        {
            pPreviousYangElementDisplayState->setNewLinePending ();
        }
        else
        {
            printString ("");
        }
    }

    if (true == m_pLastYangElementDisplayState->getOutputWasPrinted ())
    {
        if (NULL != pPreviousYangElementDisplayState)
        {
            pPreviousYangElementDisplayState->setOutputWasPrinted ();
        }
        else
        {
            prismAssert (false == m_pLastYangElementDisplayState->getIsOutputPending (), __FILE__, __LINE__);

            // printPendingOutput ();
        }
    }

    delete m_pLastYangElementDisplayState;

    m_pLastYangElementDisplayState = pPreviousYangElementDisplayState;
}

void YangDisplayConfigurationContext::printNoConfigString (const string &outputString)
{
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    printPendingOutput ();

    bool pendingNewLine = pYangElementDisplayState->getAndResetPendingNewLine ();

    if (true == pendingNewLine)
    {
        printString ("");
    }

    if (true == pYangElementDisplayState->getIsSuppressedMode ())
    {
        printString (StringUtils::joinStrings ("no", StringUtils::joinStrings (pYangElementDisplayState->getPrefixString (), outputString)));
    }
    else
    {
        printString (StringUtils::joinStrings ("no", outputString));
    }
}

void YangDisplayConfigurationContext::printConfigString (const string &outputString, const bool &isCliIncompleteCommand)
{
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    printPendingOutput ();

    string prefixString;

    bool pendingNewLine = pYangElementDisplayState->getNewLinePendingInHierarchy ();

    if (false == pendingNewLine)
    {
        prefixString = pYangElementDisplayState->getPrefixString ();
    }
    else
    {
        /////// if ((false == outputString.empty ()) && (" " != outputString))
        if (false == outputString.empty ())
        {
            printString (" ", false);
        }
    }

    bool newLineRequired = false;

    if ((false == pYangElementDisplayState->getIsCompactSyntax ()) && (false == isCliIncompleteCommand))
    {
        newLineRequired = true;
    }

    string outputStringWithPrefix;

    /////// if (" " != outputString)
    if (false == outputString.empty ())
    {
        outputStringWithPrefix = StringUtils::joinStrings (prefixString, outputString);
    }
    else
    {
        outputStringWithPrefix = prefixString;
    }

    printString (outputStringWithPrefix, newLineRequired);

#if 0
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    printPendingOutput ();

    bool pendingNewLine = pYangElementDisplayState->getNewLinePendingInHierarchy ();

    string outputStringWithPrefix = outputString;

    if (false == pendingNewLine)
    {
        string prefixString = pYangElementDisplayState->getPrefixString ();

        if (false == prefixString.empty ())
        {
            printString (prefixString + " ", false);
        }
    }
    else
    {
        if (true == pYangElementDisplayState->getIsCompactSyntax ())
        {
            printString (" ", false);
        }
        else
        {
            ///// printString ("");
            // outputStringWithPrefix = StringUtils::joinStrings (pYangElementDisplayState->getPrefixString (), outputString);
            printString (" ", false);
        }
    }

    bool newLineRequired = false;

    if ((false == pYangElementDisplayState->getIsCompactSyntax ()) && (false == isCliIncompleteCommand))
    {
        newLineRequired = true;
    }

    printString (outputStringWithPrefix, newLineRequired);
#endif
}

void YangDisplayConfigurationContext::setPrefixString (const string &prefixString)
{
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    pYangElementDisplayState->setPrefixString (prefixString);
}

void YangDisplayConfigurationContext::printPendingOutput ()
{
#if 1
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    bool newLinePendingInPendingOutputString = pYangElementDisplayState->getNewLinePendingInPendingOutputString ();

    string pendingOutput = pYangElementDisplayState->getAndResetPendingOutputStringInHierarchy ();

#if 0
    string newLineString = getNewLineString ();

    if (newLineString.size () <= pendingOutput.size ())
    {
        if (0 != pendingOutput.compare (pendingOutput.size () - newLineString.size (), newLineString.size (), newLineString))
        {
            pYangElementDisplayState->setNewLinePending ();
        }
        else
        {
            pYangElementDisplayState->getAndResetPendingNewLine ();
        }
    }
    else
    {
        pYangElementDisplayState->setNewLinePending ();
    }
#endif

    printOutput (pendingOutput);

    pYangElementDisplayState->setOutputWasPrinted ();

    if (true == newLinePendingInPendingOutputString)
    {
        pYangElementDisplayState->setNewLinePending ();
    }
#endif
}

void YangDisplayConfigurationContext::printHeaderString (const string &outputString)
{
    printPendingOutput ();

    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    string prefixString;

    YangElementDisplayState *pPreviousYangElementDisplayState = pYangElementDisplayState->getPreviousYangElementDisplayState ();

    if (NULL != pPreviousYangElementDisplayState)
    {
        prefixString = pPreviousYangElementDisplayState->getPrefixString ();
    }

    string stringToBePrinted = StringUtils::joinStrings (prefixString, outputString);

    if (false == stringToBePrinted.empty ())
    {
        printString (stringToBePrinted);
    }
}

void YangDisplayConfigurationContext::setPendingOutputString (const string &pendingOutputString)
{
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    pYangElementDisplayState->setPendingOutputString (pendingOutputString, m_depth, getNewLineString());
}

bool YangDisplayConfigurationContext::getOutputWasPrinted () const
{
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    return (pYangElementDisplayState->getOutputWasPrinted ());
}

void YangDisplayConfigurationContext::resetPendingOutputString ()
{
    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();
    prismAssert (NULL != pYangElementDisplayState, __FILE__, __LINE__);

    pYangElementDisplayState->resetPendingOutputString ();
}

void YangDisplayConfigurationContext::insertCommandKey (vector<string> &keyVector)
{
    m_keyValueVector.push_back(keyVector);
}

void YangDisplayConfigurationContext::incrementCommandKeyIndex ()
{
    m_currentCommandKeyIndex++; 
}

void YangDisplayConfigurationContext::setCommandKeyIndex (UI32 index)
{
    prismAssert (false, __FILE__, __LINE__);
    m_currentCommandKeyIndex = index; 
}

void YangDisplayConfigurationContext::decrementCommandKeyIndex ()
{
    m_currentCommandKeyIndex--;
}

void YangDisplayConfigurationContext::removeLastCommandKey ()
{
    prismAssert (false == m_keyValueVector.empty (), __FILE__, __LINE__);
    m_keyValueVector.pop_back ();
}

void YangDisplayConfigurationContext::getCurrentCommnadKey (vector<string> &keyVector)
{
    keyVector = m_keyValueVector[m_currentCommandKeyIndex-1];
}

void YangDisplayConfigurationContext::debugPrintDisplayState () const
{
    vector<string> debugStrings;

    YangElementDisplayState *pYangElementDisplayState = getLastYangElementDisplayState ();

    tracePrintf (TRACE_LEVEL_INFO, "--------------------------------------------------------------------------");

    while (NULL != pYangElementDisplayState)
    {
        pYangElementDisplayState->debugPrint ();
        pYangElementDisplayState = pYangElementDisplayState->getPreviousYangElementDisplayState ();
    }

    tracePrintf (TRACE_LEVEL_INFO, "--------------------------------------------------------------------------");
}

void YangDisplayConfigurationContext::setConfigFilterType (const YangDisplayConfigurationContext::WaveYangConfigReadFilterType &configFilterType)
{
    m_configFilterType = configFilterType;
}

YangDisplayConfigurationContext::WaveYangConfigReadFilterType YangDisplayConfigurationContext::getConfigFilterType () const
{
    return m_configFilterType;
}

bool YangDisplayConfigurationContext::getIsOwnerWaveNodeObjectIdFilterSet () const
{
    return m_ownerWaveNodeObjectIdFilterIsSet;
}

void YangDisplayConfigurationContext::setOwnerWaveNodeObjectIdFilter (const ObjectId &ownerWaveNodeObjectIdFilter)
{
    prismAssert (ObjectId::NullObjectId != ownerWaveNodeObjectIdFilter, __FILE__, __LINE__);

    m_ownerWaveNodeObjectIdFilter       = ownerWaveNodeObjectIdFilter;
    m_ownerWaveNodeObjectIdFilterIsSet  = true;
}

ObjectId YangDisplayConfigurationContext::getOwnerWaveNodeObjectIdFilter () const
{
    prismAssert (true == m_ownerWaveNodeObjectIdFilterIsSet, __FILE__, __LINE__);

    return m_ownerWaveNodeObjectIdFilter;
}

bool YangDisplayConfigurationContext::isConfigFilterApplicable (const YangElement *pYangElement) const
{
    bool isFilterApplicable = false;

    WaveYangConfigReadFilterType configFilterType = getConfigFilterType ();

    if (WAVE_YANG_CONFIG_READ_FILTER_NONE != configFilterType)
    {
        bool isInLocalConfigSubtree = pYangElement->getIsInLocalConfigurationSubtree ();

        if ((true == isInLocalConfigSubtree) && (WAVE_YANG_CONFIG_READ_FILTER_GLOBAL == configFilterType))
        {
            isFilterApplicable = true;
        }
        else if ((false == isInLocalConfigSubtree) && (WAVE_YANG_CONFIG_READ_FILTER_LOCAL == configFilterType))
        {
            isFilterApplicable = true;
        }
    }

    return isFilterApplicable;
}

WaveXPathExpressionValue YangDisplayConfigurationContext::evaluateValueOfLocationPath (const WaveLocationPath &locationPath, const YangElement *pContextYangElement)
{
    // cout << "------------------" << "\r\n";
    // locationPath.debugPrint (0);
    // cout << "------------------" << "\r\n";

    WaveXPathExpressionValue resultValue;

    if (true == locationPath.getIsAbsolute ())
    {
        bool valueFound = false;

        getCachedValuesForAbsoluteXPathLocationPath (locationPath.getXPathStringForAbsolutePath (), resultValue, valueFound);

        if (true == valueFound)
        {
            return resultValue;
        }
    }

    const YangElement *pRelativeRootYangElement = NULL;

    UI32 numberOfForwardSteps = locationPath.getNumberOfForwardSteps ();

    if (true == locationPath.getIsAbsolute ())
    {
        pRelativeRootYangElement = pContextYangElement;

        YangElement *pRelativeRootYangElementParent = pContextYangElement->getPParentElement ();

        while (NULL != pRelativeRootYangElementParent)
        {
            pRelativeRootYangElement = pRelativeRootYangElementParent;

            pRelativeRootYangElementParent = pRelativeRootYangElement->getPParentElement ();
        }

        const YangUserInterface *pYangUserInterface = dynamic_cast<const YangUserInterface *> (pRelativeRootYangElement);
        prismAssert (NULL != pYangUserInterface, __FILE__, __LINE__);

        prismAssert (0 < numberOfForwardSteps, __FILE__, __LINE__);

        string firstForwardStep;
        string firstForwardStepNamespaceString;

        locationPath.getForwardStepNameAtIndex (0, firstForwardStep, firstForwardStepNamespaceString);

        pRelativeRootYangElement = pYangUserInterface->getParentYangModuleForGrandChildYangElementByName (firstForwardStep, firstForwardStepNamespaceString);
    }
    else
    {
        pRelativeRootYangElement = pContextYangElement;

        UI32 numberOfStepsToReachRelativeRoot = locationPath.getNumberOfStepsToReachRelativeRoot ();

        for (UI32 i = 0; i < numberOfStepsToReachRelativeRoot; i++)
        {
            pRelativeRootYangElement = pRelativeRootYangElement->getPParentElement ();
            prismAssert (NULL != pRelativeRootYangElement, __FILE__, __LINE__);
        }
    }

    prismAssert (NULL != pRelativeRootYangElement, __FILE__, __LINE__);

    const YangElement *pYangElementForStep = pRelativeRootYangElement;

    for (UI32 i = 0; i < numberOfForwardSteps; i++)
    {
        string forwardStepName;
        string forwardStepNamespaceString;

        locationPath.getForwardStepNameAtIndex (i, forwardStepName, forwardStepNamespaceString);

        prismAssert (false == forwardStepName.empty (), __FILE__, __LINE__);

        pYangElementForStep = pYangElementForStep->getUniqueYangElementByName (forwardStepName); // TODO : add namespace based search.
        prismAssert (NULL != pYangElementForStep, __FILE__, __LINE__);
    }

    prismAssert (NULL != pYangElementForStep, __FILE__, __LINE__);

    // cout << "Found [" << pYangElementForStep->getYangName () << ":" << pYangElementForStep->getName () << "]" << "\r\n";

    const YangDataElement *pYangDataElement = dynamic_cast<const YangDataElement *> (pYangElementForStep);
    prismAssert (NULL != pYangDataElement, __FILE__, __LINE__);

    WaveManagedObject *pWaveManagedObject = NULL;

    YangDataElement *pYangDataElementNonConst = const_cast<YangDataElement *> (pYangDataElement);
    bool disableFilters = true;
    getPWaveManagedObject (pYangDataElementNonConst, pWaveManagedObject, disableFilters);

    if (NULL != pWaveManagedObject)
    {
        string      valueString;
        ResourceId  valueType   = WAVE_MESSAGE_ERROR;

        if (WAVE_MESSAGE_SUCCESS == pYangDataElementNonConst->getValueString (this, pWaveManagedObject, valueString, valueType))
        {
            resultValue.setValueFromString (valueString);
        }
        else
        {
            resultValue.setValueFromString ("");
        }
    }
    else
    {
        resultValue.setValueFromString ("");
    }

    if (true == locationPath.getIsAbsolute ())
    {
        addValueForAbsoluteXPathLocationPath (locationPath.getXPathStringForAbsolutePath (), resultValue);
    }

    return resultValue;
}

bool YangDisplayConfigurationContext::getAnyOutputDisplayed () const
{
    return m_anyOutputDisplayed;
}

void YangDisplayConfigurationContext::setAnyOutputDisplayed ()
{
    m_anyOutputDisplayed = true;
}

void YangDisplayConfigurationContext::setPrintDefaultValuesFlag (const bool &printDefaultValuesFlag)
{
    m_printDefaultValuesFlag = printDefaultValuesFlag;
}

bool YangDisplayConfigurationContext::getPrintDefaultValuesFlag () const
{
    return m_printDefaultValuesFlag;
}

// map<string, WaveXPathExpressionValue>   m_cachedValuesForAbsoluteXPathLocationPaths;
void YangDisplayConfigurationContext::getCachedValuesForAbsoluteXPathLocationPath  (const string &locationPathString, WaveXPathExpressionValue &resultValue, bool &valueFound) const
{
    prismAssert (false == locationPathString.empty (), __FILE__, __LINE__);

    valueFound = false;

    map<string, WaveXPathExpressionValue>::const_iterator itr = m_cachedValuesForAbsoluteXPathLocationPaths.find (locationPathString);
    map<string, WaveXPathExpressionValue>::const_iterator end = m_cachedValuesForAbsoluteXPathLocationPaths.end ();

    if (end != itr)
    {
        resultValue = itr->second;
        valueFound = true;
    }
}

void YangDisplayConfigurationContext::addValueForAbsoluteXPathLocationPath (const string &locationPathString, const WaveXPathExpressionValue &resultValue)
{
    // Assertion checks begin   [

    prismAssert (false == locationPathString.empty (), __FILE__, __LINE__);

    map<string, WaveXPathExpressionValue>::const_iterator itr = m_cachedValuesForAbsoluteXPathLocationPaths.find (locationPathString);
    map<string, WaveXPathExpressionValue>::const_iterator end = m_cachedValuesForAbsoluteXPathLocationPaths.end ();

    prismAssert (end == itr, __FILE__, __LINE__);

    // Assertion checks end     ]

    m_cachedValuesForAbsoluteXPathLocationPaths[locationPathString] = resultValue;
}

ResourceId YangDisplayConfigurationContext::addAndAttributesForKeyValues (WaveManagedObjectSynchronousQueryContext &waveManagedObjectSynchronousQueryContext, const string &managedObjectClassName, const vector<Attribute *> &pKeyAttributes, const bool &applyNodeSpecificFilter) const
{
    vector<string> userDefinedKeyCombination;
    map<string, Attribute*> userDefinedKeyCombinationWithTypes = OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (managedObjectClassName, userDefinedKeyCombination);

    UI32 numberOfAttributeNamesInUserDefinedKeyCombination  = userDefinedKeyCombination.size ();
    UI32 numberOfKeyAttributesReceived                      = pKeyAttributes.size ();

    prismAssert (numberOfAttributeNamesInUserDefinedKeyCombination == numberOfKeyAttributesReceived, __FILE__, __LINE__);

    for (UI32 i = 0; i < numberOfAttributeNamesInUserDefinedKeyCombination; i++)
    {
        Attribute *pKeyAttributeReceived = pKeyAttributes[i];
        prismAssert (NULL != pKeyAttributeReceived, __FILE__, __LINE__);

        string plainString;
        pKeyAttributeReceived->getPlainString (plainString);

        string keyAttributeNameInTargetManagedObject = userDefinedKeyCombination[i];

        map<string, Attribute*>::const_iterator itr = userDefinedKeyCombinationWithTypes.find (keyAttributeNameInTargetManagedObject);
        map<string, Attribute*>::const_iterator end = userDefinedKeyCombinationWithTypes.end ();

        prismAssert (end != itr, __FILE__, __LINE__);

        Attribute *pKeyAttributeInTargetManagedObject = itr->second;
        prismAssert (NULL != pKeyAttributeInTargetManagedObject, __FILE__, __LINE__);

        Attribute *pClonedAttribute = pKeyAttributeInTargetManagedObject->clone ();
        prismAssert (NULL != pClonedAttribute, __FILE__, __LINE__);

        AttributeObjectIdAssociation *pAttributeObjectIdAssociation = dynamic_cast<AttributeObjectIdAssociation *> (pClonedAttribute);

        if (NULL == pAttributeObjectIdAssociation)
        {
            ResourceId loadFromPlainStringStatus = pClonedAttribute->loadFromPlainString (plainString);

            if (WAVE_MESSAGE_SUCCESS != loadFromPlainStringStatus)
            {
                delete pClonedAttribute;
                return loadFromPlainStringStatus;
            }
        }
        else
        {
            ResourceId associatedObjectIdQueryStatus = getObjectIdOfAssocociatedManagedObjectWithKey (plainString, pAttributeObjectIdAssociation, applyNodeSpecificFilter);

            if (WAVE_MESSAGE_SUCCESS != associatedObjectIdQueryStatus)
            {
                delete pClonedAttribute;
                return associatedObjectIdQueryStatus;
            }
        }

        waveManagedObjectSynchronousQueryContext.addAndAttribute (pClonedAttribute);
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId YangDisplayConfigurationContext::getObjectIdOfAssocociatedManagedObjectWithKey (const string &keyValue, AttributeObjectIdAssociation *pAssociationAttribute, const bool &applyNodeSpecificFilter) const
{
    prismAssert (false == keyValue.empty (), __FILE__, __LINE__);
    prismAssert (NULL != pAssociationAttribute, __FILE__, __LINE__);

    string associatedManagedObjectClassName = pAssociationAttribute->getAssociatedTo ();

    WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (associatedManagedObjectClassName);
    waveManagedObjectSynchronousQueryContext.setSchemaToQuery (getConfigurationSchema ());
    waveManagedObjectSynchronousQueryContext.setLoadCompositions (false);
    waveManagedObjectSynchronousQueryContext.addSelectFieldForObjectId ("objectId");

    vector<string> userDefinedKeyCombination;
    map<string, Attribute*> userDefinedKeyCombinationWithTypes = OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (associatedManagedObjectClassName, userDefinedKeyCombination);

    prismAssert (1 == userDefinedKeyCombination.size (), __FILE__, __LINE__);

    map<string, Attribute*>::const_iterator itrElement  = userDefinedKeyCombinationWithTypes.find (userDefinedKeyCombination[0]);
    map<string, Attribute*>::const_iterator endElement  = userDefinedKeyCombinationWithTypes.end ();

    prismAssert (endElement != itrElement, __FILE__, __LINE__);

    Attribute *pKeyAttribute = itrElement->second;
    prismAssert (NULL != pKeyAttribute, __FILE__, __LINE__);

    Attribute *pClonedKeyAttribute = pKeyAttribute->clone ();
    prismAssert (NULL != pClonedKeyAttribute, __FILE__, __LINE__);

    ResourceId loadFromPlainStringStatus = pClonedKeyAttribute->loadFromPlainString (keyValue);

    if (WAVE_MESSAGE_SUCCESS != loadFromPlainStringStatus)
    {
        delete pClonedKeyAttribute;
        return loadFromPlainStringStatus;
    }

    waveManagedObjectSynchronousQueryContext.addAndAttribute (pClonedKeyAttribute);

    if (true == applyNodeSpecificFilter)
    {
        if (true == OrmRepository::isALocalManagedObject (associatedManagedObjectClassName))
        {
            ObjectId ownerWaveNodeObjectId = getOwnerWaveNodeObjectId ();
            prismAssert (ObjectId::NullObjectId != ownerWaveNodeObjectId, __FILE__, __LINE__);

            waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeObjectId (ownerWaveNodeObjectId, "ownerWaveNodeObjectId"));
        }
    }

    vector<WaveManagedObject *> pAssociatedManagedObjects;
    querySynchronously (waveManagedObjectSynchronousQueryContext, associatedManagedObjectClassName, pAssociatedManagedObjects);

    UI32 numberOfResults = pAssociatedManagedObjects.size ();

    if (0 == numberOfResults)
    {
        return WAVE_MESSAGE_ERROR;
    }
    else if (1 == numberOfResults)
    {
        WaveManagedObject *pAssociatedWaveManagedObject = pAssociatedManagedObjects[0];
        prismAssert (NULL != pAssociatedWaveManagedObject, __FILE__, __LINE__);

        pAssociationAttribute->setValue (pAssociatedWaveManagedObject->getObjectId ());

        delete pAssociatedWaveManagedObject;

        return WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("YangDisplayConfigurationContext::getObjectIdOfAssocociatedManagedObjectWithKey : multiple managed objects found. Details -"));
        debugPrintErrorInformation (waveManagedObjectSynchronousQueryContext, pAssociatedManagedObjects);
        prismAssert (false, __FILE__, __LINE__);
        return WAVE_MESSAGE_ERROR;
    }
}

void YangDisplayConfigurationContext::addSetOfUnhiddenHideGroupNames (const set<string> &setOfUnhiddenHideGroupNames)
{
    m_setOfUnhiddenHideGroupNames.insert (setOfUnhiddenHideGroupNames.begin (), setOfUnhiddenHideGroupNames.end ());
}

void YangDisplayConfigurationContext::addHideGroupNameToSetOfUnhiddenHideGroupNames (const string &unhiddenHideGroupName)
{
    m_setOfUnhiddenHideGroupNames.insert (unhiddenHideGroupName);
}

bool YangDisplayConfigurationContext::isHideGroupUnhidden (const string &hideGroupName) const
{
    set<string>::const_iterator itrElement = m_setOfUnhiddenHideGroupNames.find (hideGroupName);
    set<string>::const_iterator endElement = m_setOfUnhiddenHideGroupNames.end ();

    if (endElement != itrElement)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void YangDisplayConfigurationContext::debugPrintErrorInformation (WaveManagedObjectSynchronousQueryContext &waveManagedObjectSynchronousQueryContext, vector<WaveManagedObject *> &pWaveManagedObjects) const
{
    waveManagedObjectSynchronousQueryContext.printSqlToDebugError ();

    UI32 numberOfManagedObjects = pWaveManagedObjects.size ();

    trace (TRACE_LEVEL_ERROR, string ("YangDisplayConfigurationContext::debugPrintErrorInformation : numberOfManagedObjects [") + numberOfManagedObjects + string ("]"));

    UI32 numberOfManagedObjectsToPrint = numberOfManagedObjects;

    const UI32 maxNumberOfManagedObjectsToDebugPrint = 5;

    if (maxNumberOfManagedObjectsToDebugPrint < numberOfManagedObjectsToPrint)
    {
        numberOfManagedObjectsToPrint = maxNumberOfManagedObjectsToDebugPrint;

        trace (TRACE_LEVEL_ERROR, string ("YangDisplayConfigurationContext::debugPrintErrorInformation : printing information for first [") + maxNumberOfManagedObjectsToDebugPrint + string ("] results."));
    }

    for (UI32 i = 0; i < numberOfManagedObjectsToPrint; i++)
    {
        WaveManagedObject *pWaveManagedObject = pWaveManagedObjects[i];
        prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

        trace (TRACE_LEVEL_ERROR, string ("Managed Object ") + (i + 1) + string ("."));

        trace (TRACE_LEVEL_ERROR, string ("  objectClassName  [") + pWaveManagedObject->getObjectClassName () + string ("]"));

        trace (TRACE_LEVEL_ERROR, string ("      objectId                       [") + (pWaveManagedObject->getObjectId ()).toString () + string ("]"));
        trace (TRACE_LEVEL_ERROR, string ("      ownerManagedObjectId           [") + (pWaveManagedObject->getOwnerManagedObjectId ()).toString () + string ("]"));

        WaveLocalManagedObjectBase *pWaveLocalManagedObjectBase = dynamic_cast<WaveLocalManagedObjectBase *> (pWaveManagedObject);

        if (NULL != pWaveLocalManagedObjectBase)
        {
            trace (TRACE_LEVEL_ERROR, string ("      ownerWaveNodeObjectId          [") + (pWaveLocalManagedObjectBase->getOwnerWaveNodeObjectId ()).toString () + string ("]"));
            trace (TRACE_LEVEL_ERROR, string ("      waveSlotObjectId               [") + (pWaveLocalManagedObjectBase->getWaveSlotObjectId ()).toString () + string ("]"));
        }

        trace (TRACE_LEVEL_ERROR, string ("      ownerPartitionManagedObjectId  [") + (pWaveManagedObject->getOwnerPartitionManagedObjectId ()).toString () + string ("]"));

        trace (TRACE_LEVEL_ERROR, string ("      keyString                      [") + pWaveManagedObject->getKeyString () + string ("]"));
        trace (TRACE_LEVEL_ERROR, string ("      userDefinedKeyCombinationValue [") + pWaveManagedObject->getUserDefinedKeyCombinationValue () + string ("]"));
    }
}

// virtual
bool YangDisplayConfigurationContext::isCommandAbortedByUser () const
{
    return false;
}

void YangDisplayConfigurationContext::setPYangUserInterfaceAndLock (YangUserInterface *pYangUserInterface)
{
    prismAssert (NULL != pYangUserInterface, __FILE__, __LINE__);
    prismAssert (NULL == m_pYangUserInterface, __FILE__, __LINE__); // It should be set once.

    m_pYangUserInterface = pYangUserInterface;

    lockYangUserInterface ();
}

void YangDisplayConfigurationContext::releasePYangUserInterfaceLock ()
{
    unlockYangUserInterface ();
}

void YangDisplayConfigurationContext::lockYangUserInterface () const
{
    prismAssert (NULL != m_pYangUserInterface, __FILE__, __LINE__);

    m_pYangUserInterface->lock ();
}

void YangDisplayConfigurationContext::unlockYangUserInterface () const
{
    prismAssert (NULL != m_pYangUserInterface, __FILE__, __LINE__);

    m_pYangUserInterface->unlock ();
}

void YangDisplayConfigurationContext::setBatchSizeForQueryingListInstances (const UI32 &batchSize)
{
    m_batchSizeForQueryingListInstances = batchSize;
}

UI32 YangDisplayConfigurationContext::getBatchSizeForQueryingListInstances () const
{
    return m_batchSizeForQueryingListInstances;
}

string YangDisplayConfigurationContext::getNewLineString () const
{
    return m_newLineString;
}

void YangDisplayConfigurationContext::setNewLineString (const string &newLineString)
{
    m_newLineString = newLineString;
}


// static

UI32 YangDisplayConfigurationContext::getDefaultBatchSizeForQueryingListInstances ()
{
    return s_defaultBatchSizeForQueryingListInstances;
}


// WaveCustomCliDisplayConfigurationContext

YangElement* WaveCustomCliDisplayConfigurationContext::getYangElement ()
{
    return m_pYangElement;
}

void WaveCustomCliDisplayConfigurationContext::setYangElement (YangElement* pYangElement)
{
    m_pYangElement = pYangElement;
}

YangDisplayConfigurationContext* WaveCustomCliDisplayConfigurationContext::getYangDisplayConfigurationContext ()
{
    return m_pYangDisplayConfigurationContext;
}

void WaveCustomCliDisplayConfigurationContext::setYangDisplayConfigurationContext (YangDisplayConfigurationContext *pYangDisplayConfigurationContext)
{
    m_pYangDisplayConfigurationContext = pYangDisplayConfigurationContext;
}

string WaveCustomCliDisplayConfigurationContext::getConfigurationSegmentName ()
{
    return m_pYangElement->getConfigurationSegmentName ();
}

UI32 WaveCustomCliDisplayConfigurationContext::getAttributeUserTag ()
{
    return m_pYangElement->getUserTagValue ();
}

WaveCustomCliDisplayConfigurationContext::WaveCustomCliDisplayConfigurationContext (YangElement* pYangElement, YangDisplayConfigurationContext* pYangDisplayConfigurationContext )
{
    m_pYangElement = pYangElement;
    m_pYangDisplayConfigurationContext = pYangDisplayConfigurationContext;
}

WaveCustomCliDisplayConfigurationContext::~WaveCustomCliDisplayConfigurationContext ()
{

}

WaveYangMemberElementFilterInformation::WaveYangMemberElementFilterInformation (const YangElement *pYangElement, const string &matchValue, WaveYangMemberElementFilterInformation *pNextWaveYangMemberElementFilterInformation)
    : m_pYangElement                                (pYangElement),
      m_matchValue                                  (matchValue),
      m_numberOfTokensAbsorbed                      (0),
      m_pNextWaveYangMemberElementFilterInformation (pNextWaveYangMemberElementFilterInformation)
{
}

WaveYangMemberElementFilterInformation::~WaveYangMemberElementFilterInformation ()
{
    prismAssert (0 == m_numberOfTokensAbsorbed, __FILE__, __LINE__);

    if (NULL != m_pNextWaveYangMemberElementFilterInformation)
    {
        delete m_pNextWaveYangMemberElementFilterInformation;
    }
}

ResourceId WaveYangMemberElementFilterInformation::doesFilterInformationMatch (YangDisplayConfigurationContext *pYangDisplayConfigurationContext)
{
    ResourceId returnStatus;

    prismAssert (NULL != m_pYangElement, __FILE__, __LINE__);

    YangElement *pNonConstYangElement = const_cast<YangElement *> (m_pYangElement);

    ResourceId matchStatus = pNonConstYangElement->doesElementValueMatch (pYangDisplayConfigurationContext, m_matchValue);

    if (WAVE_MESSAGE_SUCCESS == matchStatus)
    {
        if (NULL != m_pNextWaveYangMemberElementFilterInformation)
        {
            returnStatus = m_pNextWaveYangMemberElementFilterInformation->doesFilterInformationMatch (pYangDisplayConfigurationContext);
        }
        else
        {
            returnStatus = matchStatus;
        }
    }
    else
    {
        returnStatus = matchStatus;
    }

    return returnStatus;
}

void WaveYangMemberElementFilterInformation::setNumberOfTokensAbsorbed (const UI32 &numberOfTokensAbsorbed)
{
    m_numberOfTokensAbsorbed = numberOfTokensAbsorbed;
}

void WaveYangMemberElementFilterInformation::restoreAbsorbedTokens (YangDisplayConfigurationContext *pYangDisplayConfigurationContext)
{
    if (NULL != m_pNextWaveYangMemberElementFilterInformation)
    {
        m_pNextWaveYangMemberElementFilterInformation->restoreAbsorbedTokens (pYangDisplayConfigurationContext);
    }

    for (UI32 i = 0; i < m_numberOfTokensAbsorbed; i++)
    {
        pYangDisplayConfigurationContext->decrementTokenPosition ();
    }

    m_numberOfTokensAbsorbed = 0;
}

}
