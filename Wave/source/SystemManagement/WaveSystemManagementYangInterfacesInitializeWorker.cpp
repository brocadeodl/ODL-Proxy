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
 *   Author : Jayanth Venkataraman / Mandar Datar                          *
 ***************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "SystemManagement/WaveSystemManagementYangInterfacesInitializeWorker.h"
#include "SystemManagement/WaveSystemManagementRebuildYangInterfacesMessage.h"
#include "SystemManagement/WaveSystemManagementTypes.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"
#include "SystemManagement/SystemManagementToolKit.h"
#include "Modeling/YANG/ObjectModel/YangType.h"

namespace WaveNs
{

WaveSystemManagementYangInterfacesInitializeWorker::WaveSystemManagementYangInterfacesInitializeWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager),
      m_pYangModuleCollection       (NULL),
      m_pYangUserInterface          (NULL),
      m_yinForYangUserInterface     (""),
      m_alreadyInitialized          (false)
{
    addOperationMap (WAVE_SYSTEM_MANAGEMENT_REBUILD_YANG_INTERFACES, reinterpret_cast<PrismMessageHandler> (&WaveSystemManagementYangInterfacesInitializeWorker::rebuildYangInterfacesMessageHandler));
}

WaveSystemManagementYangInterfacesInitializeWorker::~WaveSystemManagementYangInterfacesInitializeWorker ()
{
}

PrismMessage *WaveSystemManagementYangInterfacesInitializeWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case WAVE_SYSTEM_MANAGEMENT_REBUILD_YANG_INTERFACES:
            pPrismMessage = new WaveSystemManagementRebuildYangInterfacesMessage ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("WaveSystemManagementYangInterfacesInitializeWorker::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (pPrismMessage);
}
void WaveSystemManagementYangInterfacesInitializeWorker::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementYangInterfacesInitializeWorker::initializeSystemManagementInterfaceStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementYangInterfacesInitializeWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementYangInterfacesInitializeWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveSystemManagementYangInterfacesInitializeWorker::initializeSystemManagementInterfaceStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    if (false == m_alreadyInitialized)
    {
        YangModuleCollection    *pYangModuleCollection   = new YangModuleCollection ();
        YangUserInterface       *pYangUserInterface      = new YangUserInterface    ();

        buildSystemManagementInterface (pYangModuleCollection, pYangUserInterface, m_yinForYangUserInterface);

        m_alreadyInitialized        = true;
        m_pYangModuleCollection     = pYangModuleCollection;
        m_pYangUserInterface        = pYangUserInterface;
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}


void WaveSystemManagementYangInterfacesInitializeWorker::rebuildYangInterfacesMessageHandler (WaveSystemManagementRebuildYangInterfacesMessage *pWaveSystemManagementRebuildYangInterfacesMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementYangInterfacesInitializeWorker::rebuildSystemManagementInterfaceStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementYangInterfacesInitializeWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementYangInterfacesInitializeWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveSystemManagementRebuildYangInterfacesMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void WaveSystemManagementYangInterfacesInitializeWorker::rebuildSystemManagementInterfaceStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    if (true == m_alreadyInitialized) 
    {
        YangModuleCollection    *pYangModuleCollection   = new YangModuleCollection ();
        YangUserInterface       *pYangUserInterface      = new YangUserInterface    ();
        
        // Rebuild the yangmodulecollection and yanguserinterface
        buildSystemManagementInterface (pYangModuleCollection, pYangUserInterface, m_yinForYangUserInterface);

        /*  1. Delete the existing m_yangModuleCollection
            2. Do a pointer copy of the local yangModuleCollection to m_yangModuleCollection
            3. Delete the existing m_yangUserInterface
            4. Do a pointer copy of the local yangUserInterface to m_yangUserInterface
        */
        // 1
        delete m_pYangModuleCollection;

        // 2 
        m_pYangModuleCollection = pYangModuleCollection;
            
        // 3
        delete m_pYangUserInterface;

        // 4
        m_pYangUserInterface = pYangUserInterface;
    }

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveSystemManagementYangInterfacesInitializeWorker::buildSystemManagementInterface (YangModuleCollection *yangModuleCollection, YangUserInterface *yangUserInterface, string &yinForYangUserInterface)
{
    trace (TRACE_LEVEL_DEBUG, "WaveSystemManagementYangInterfacesInitializeWorker::buildSystemManagementInterface : Entering ...");

    vector<string> yinPaths          = SystemManagementToolKit::getYinPaths ();
    vector<string> yinFilePaths;
    string         wyserTagsFilePath = SystemManagementToolKit::getWyserTagsFilePath ();

    trace (TRACE_LEVEL_INFO, "Initializing the System Management Interface ...");

    // Read the YIN Modules and initialize them into memory.

    trace (TRACE_LEVEL_INFO, "    Loading All Modules ...");

    yangModuleCollection->loadModulesFromDirectoriesAndFiles (yinPaths, yinFilePaths);

    trace (TRACE_LEVEL_INFO, "    Updating Original Module NameSpace Uri in All Modules ...");

    yangModuleCollection->setOriginalModuleNameSpaceUriInAllModules ();

    // initialize Yang Builtin Types 

    trace (TRACE_LEVEL_INFO, "    Initializing Yang Builtin Types ...");

    YangType::initializeBuiltinTypes();

    // Inline Typedef

    trace (TRACE_LEVEL_INFO, "    In-lining Typedef ...");

    yangModuleCollection->inlineTypedef ();

    // Inline Groupings

    trace (TRACE_LEVEL_INFO, "    In-lining Groupings ...");

    yangModuleCollection->inlineGroupingUsage ();

    // Inline Augments

    trace (TRACE_LEVEL_INFO, "    Inlining Augments ...");

    yangModuleCollection->inlineAugmentUsage ();

    // Remove All groupings

    trace (TRACE_LEVEL_INFO, "    Removing Groupings ...");

    yangModuleCollection->removeAllGroupings ();
    
    trace (TRACE_LEVEL_INFO, "    Computing Configuration Segment Information ...");

    yangModuleCollection->computeConfigurationSegmentNames ();

    trace (TRACE_LEVEL_INFO, "    Loading WYSER Tags from " + wyserTagsFilePath + " ...");

    yangModuleCollection->loadUserTagsFromFile (wyserTagsFilePath);

    trace (TRACE_LEVEL_INFO, "    Updating Modules with WYSER Tag information ...");

    yangModuleCollection->updateModulesWithUserTags ();

    trace (TRACE_LEVEL_INFO, "    Computing Cli Target Node Node Names ...");

    yangModuleCollection->computeCliTargetNodeNames ();
        
    //WaveConfigurationSegmentMap::print ();
#if 0
    trace (TRACE_LEVEL_INFO, "    Cloning Yang Module Collection ...");

    YangModuleCollection *pClonedYangModuleCollection = yangModuleCollection->clone ();

    prismAssert (NULL != pClonedYangModuleCollection, __FILE__, __LINE__);
#endif   
    trace (TRACE_LEVEL_INFO, "    Process Type information like Enum Bitmap etc ... ");

    yangModuleCollection->processTypeInformations ();

    trace (TRACE_LEVEL_INFO, "    Computing Configuration Segment Information for the cloned yangModuleCollection ..");

    yangModuleCollection->computeConfigurationSegmentNames ();

    trace (TRACE_LEVEL_INFO, "    Marking node-specific and multi-partition base Yang Elements ...");

    yangModuleCollection->markNodeSpecificAndPartitionBaseYangElements ();

    trace (TRACE_LEVEL_INFO, "    Transfering Modules from Cloned Yang Module Collection to Yang User Interface ...");

    yangModuleCollection->transferAllModulesToUserInterface (yangUserInterface);

    trace (TRACE_LEVEL_INFO, "    Generating YIN String for YANG User Interface ...");

    yangUserInterface->printYinToString (yinForYangUserInterface);

    trace (TRACE_LEVEL_INFO, "    Computing First Data Element in Hierarchy ...");

    yangUserInterface->computeFirstDataElementInHierarchy ();

    trace (TRACE_LEVEL_INFO, "    Computing configuration Status ...");

    yangUserInterface->computeIsConfiguration ();

    trace (TRACE_LEVEL_INFO, "    Computing Cli Target Node Node Names ...");

    yangUserInterface->computeCliTargetNodeNames ();

    trace (TRACE_LEVEL_INFO, "    Computing If nodes will be shown in configuration tree ...");

    yangUserInterface->computeWillBeShownInConfigurationTree ();

    trace (TRACE_LEVEL_INFO, "    Computing Cli Target Node Name Map ...");

    yangUserInterface->computeCliTargetNodeNameMapForData ();

    yangUserInterface->applyAnnotationSupplement ();

    trace (TRACE_LEVEL_INFO, "    Updating Yang user Interface with WYSER Tag information ...");

    yangUserInterface->updateUserTags (yangModuleCollection);

    trace (TRACE_LEVEL_INFO, "    Preparaing tree of Yang Element Tree for Data ...");

    yangUserInterface->prepareYangElementTreeForData ();

    trace (TRACE_LEVEL_INFO, "    Propagating node-specific and multi-partition information in Yang Element Tree for Data ...");

    yangUserInterface->propagateNodeSpecificAndMultiPartitionInformationInDataElementTree ();

    trace (TRACE_LEVEL_INFO, "    Preparing Type information ...");

    yangUserInterface->prepareTypeInformation ();

    trace (TRACE_LEVEL_INFO, "    Processing hide-in-submode Leafs ...");

    yangUserInterface->collectHideInSubModeLeaf ();

    // trace (TRACE_LEVEL_INFO, "    Propagating compact syntax in list and container ...");

    // yangUserInterface->propagateCompactSyntaxOneLevel ();

    trace (TRACE_LEVEL_INFO, "    Collecting Child Leaf with custom display ...");

    yangUserInterface->collectInformationAboutChildLeafWithCustomDisplay ();

    //delete pClonedYangModuleCollection;
}

YangModuleCollection *WaveSystemManagementYangInterfacesInitializeWorker::getYangModuleCollection ()
{
    return (m_pYangModuleCollection);
}

YangUserInterface *WaveSystemManagementYangInterfacesInitializeWorker::getYangUserInterface ()
{
    return (m_pYangUserInterface);
}

YangUserInterface *WaveSystemManagementYangInterfacesInitializeWorker::getClonedYangUserInterface ()
{
    YangElement *pYangElement = m_pYangUserInterface->clone ();

    WaveNs::prismAssert (NULL != pYangElement, __FILE__, __LINE__);

    YangUserInterface *pYangUserInterface = dynamic_cast<YangUserInterface *> (pYangElement);

    WaveNs::prismAssert (NULL != pYangUserInterface, __FILE__, __LINE__);

    return (pYangUserInterface);
}

string &WaveSystemManagementYangInterfacesInitializeWorker::getYinForYangUserInterface ()
{
    return (m_yinForYangUserInterface);
}

void WaveSystemManagementYangInterfacesInitializeWorker::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "WaveSystemManagementYangInterfacesInitializeWorker::boot : Entering ...");

    WaveConfigurationSegmentMap::cacheIsLocalManagedObjectInformation ();

    YangUserInterface *pYangUserInterface = getYangUserInterface ();
    prismAssert (NULL != pYangUserInterface, __FILE__, __LINE__);

    pYangUserInterface->prepareHierarchyInformationForYangElementsInTreeForData ();

    UI32 numberOfYangElements = pYangUserInterface->getNumberOfAllChildYangElementsInTree ();
    numberOfYangElements++; // include pYangUserInterface itself.
    trace (TRACE_LEVEL_INFO, string ("WaveSystemManagementYangInterfacesInitializeWorker::boot : Number of YangElements in YangUserInterface    [") + numberOfYangElements + string ("]"));

    delete (m_pYangModuleCollection);
    
    m_pYangModuleCollection = NULL;
    m_yinForYangUserInterface = "";

#if 0
    YangModuleCollection *pYangModuleCollection = getYangModuleCollection ();
    prismAssert (NULL != pYangModuleCollection, __FILE__, __LINE__);
    UI32 numberOfYangElementsInYangModuleCollection = pYangModuleCollection->getNumberOfAllChildYangElementsInAllModules ();
    trace (TRACE_LEVEL_INFO, string ("WaveSystemManagementYangInterfacesInitializeWorker::boot : Number of YangElements in YangModuleCollection [") + numberOfYangElementsInYangModuleCollection + string ("]"));
#endif

    UI32 numberOfYangElementsInSystem = YangElement::getNumberOfInstancesOfYangElementsInSystem ();
    trace (TRACE_LEVEL_INFO, string ("WaveSystemManagementYangInterfacesInitializeWorker::boot : Number of YangElements in system               [") + numberOfYangElementsInSystem + string ("]"));

    UI32 sizeOfYangElement = sizeof(YangElement);
    trace (TRACE_LEVEL_INFO, string ("WaveSystemManagementYangInterfacesInitializeWorker::boot : size of YangElement base class object          [") + sizeOfYangElement + string ("]"));

    // Following numbers indicate "max number of managed objects (corresponding to lists) in memory, per user session" ([1] * [2]):
    // (Note : In addition to [1] * [2], in each ConfigLevel (+ top level), there will be few managed objects which correspond to configuration-segments different from that of the YangList for the ConfigLevel.)

    trace (TRACE_LEVEL_INFO, string ("WaveSystemManagementYangInterfacesInitializeWorker::boot : Max Nesting Depth For YangLists                [") + pYangUserInterface->getMaxNestingDepthForYangLists () + string ("]"));                            // [1]
    trace (TRACE_LEVEL_INFO, string ("WaveSystemManagementYangInterfacesInitializeWorker::boot : Default Batch size for querying list instances [") + YangDisplayConfigurationContext::getDefaultBatchSizeForQueryingListInstances () + string ("]"));  // [2]

    prismAssert (numberOfYangElements == numberOfYangElementsInSystem, __FILE__, __LINE__);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

}

