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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Utils/StreamingSocket.h"
#include "Framework/Utils/ClientStreamingSocket.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Shell/ShellDebug.h"
#include "WaveResourceIds.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/Redundancy/RedundancyOptimizerBase.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "Framework/Utils/PrismMutex.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalGetObjectIdMessages.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Messaging/MessageTracker/MessageTracker.h"
#include "Framework/ObjectModel/ObjectTracker/ObjectTracker.h"
#include "Framework/ObjectModel/ReservedWaveLocalObjectManager.h"
#include "Framework/Zeroize/ZeroizeForFIPSSecureDelete.h"
#include "ManagementInterface/ManagementInterfaceObjectManager.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Persistence/PersistenceObjectManagerMessages.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerLiveSyncEnableMessages.h"
#include "Cluster/MultiPartition/Global/MultiPartitionMessages.h"
#include "File/Local/FilePushFileMessage.h"
#include "File/Local/FileLocalMessages.h"
#include "Framework/Types/Uri.h"
#include "Framework/Messaging/Remote/InterLocationMessageReceiverObjectManager.h"
#include "Framework/Messaging/HaPeer/HaPeerMessageReceiverObjectManager.h"

#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

namespace WaveNs
{

static bool                        s_isRunningAsADaemon                                  = true;
static string                      s_processInitialWorkingDirectory                      ("");

static SI32                        s_managementInterfaceReceiverPort                     = 9110;
static SI32                        s_managementInterfaceClientReceiverPort               = 9710;

static TraceLevel                  s_defaultTraceLevel                                   = TRACE_LEVEL_INFO;
static TraceLevel                  s_defaultTraceLevelForClient                          = TRACE_LEVEL_ERROR;

static WaveManagementInterfaceRole s_waveManagementInterfaceRole                         = WAVE_MGMT_INTF_ROLE_SERVER;

static UI32                        s_numberOfCpus                                        = 0;
static PrismMutex                  s_numberOfCpusMutex;

static bool                        s_useFileServiceForFileTransfer                       = true;

static bool                        s_detectSchemaChange                                  = true;

static bool                        s_isConfigFileReplayGoingToBeDone                     = false;
static PrismMutex                  s_isConfigFileReplayGoingToBeDoneMutex;

static SI32                        s_httpInterfaceReceiverPort                           = 2301;

static SI32                        s_haInterfaceReceiverPort                             = 3516;

static SI32                        s_haPeerInterfaceReceiverPort                         = 0;

static bool                        s_handleDBCorruption                                  = true;

static string                      s_dbBackupFileOnFwdl                                  ("");

static ofstream                    s_consoleStream;
static PrismMutex                  s_consoleStreamMutex;

static bool                        s_rewriteStartupSchemaOnNodeReady       = false;
static PrismMutex                  s_rewriteStartupSchemaOnNodeReadyMutex;

static SI32                        s_commandLineInterfaceReceiverPort                    = 1101;

static SI32                        s_fwdlCase                                            = 0;

static UI32                        s_haId                                                = 0;

static bool                        s_schemaConversionDisabledByUser                      = false;

static bool                        s_isConfigurationCompatibilityCheckRequiredForCluster = false;

static bool                        s_isAbruptReboot                        = false;

static bool                        s_syncFailureNotified                                 = false;
static PrismMutex                  s_syncFailureNotifiedMutex;

static ResourceId                  s_waveStartMode;
static PrismMutex                  s_waveStartModeMutex;

static SI32                        s_messageBrokerPort                                   = 19110;
static SI32                        s_messageBrokerClientPort                             = 19710;

void FrameworkToolKit::initialize ()
{
    initializeEnvironment ();

    (void) getNumberOfCpus ();

    s_processInitialWorkingDirectory = getProcessCurrentWorkingDirectory ();

    // Initialize the Wave Resource Ids.
    WaveNs::initializeWaveResourceIds ();
}

const bool FrameworkToolKit::getIsRunningAsADaemon ()
{
    return (s_isRunningAsADaemon);
}

const bool FrameworkToolKit::isFrameworkReadyToBoot ()
{
    return (PrismFrameworkObjectManager::getIsFrameworkReadyToBoot ());
}

void FrameworkToolKit::setIsRunningAsADaemon (const bool &isRunningAsADaemon)
{
    s_isRunningAsADaemon = isRunningAsADaemon;
}

const string FrameworkToolKit::getAPublicIpAddressForThisMachine ()
{
    return (StreamingSocket::getAPublicIpAddressForThisMachine ());
}

const string FrameworkToolKit::getLoopBackIpAddressForThisMachine ()
{
    return ("127.0.0.1");
}

const string FrameworkToolKit::getIpAddressForInterface (const string &interfaceName)
{
    return (StreamingSocket::getIpAddressForInterface (interfaceName));
}

LocationBase *FrameworkToolKit::getPThisLocation ()
{
    LocationBase              *pThisLocation = NULL;
    PrismFrameworkObjectManager *pTemp         = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        pThisLocation = pTemp->m_pThisLocation;
    }

    return (pThisLocation);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name : getMyLocationId
//Objective     : This method is to get the local node's location-id. The output of this method is same  //
//              same as of FrameworkToolKit::getThisLocationId () which is widely used and should be     //
//              used. The two methods differ in case of surrogation and on surrogating node. In case of  //
//              message being handled on surrogating node, FrameworkToolKit::getThisLocationId() will    //
//              return the location-id of a disconnected node for which local node is surrogating        //
//              whereas this method FrameworkToolKit::getMyLocationId () will return the local node's    //
//              location-id only.                                                                        //
//Output        : None                                                                                   //
//Output        : location-id (UI32)                                                                     //
//      Note    : This method was necessary for send/sendSynchronously where the surrogating node is     //
//              cluster primary and the message is targetted for Primary. Please, avoid using this method//
//              as this will have adverse effect in case of surrogation unless you are sure about the    //
//              usage.                                                                                   //
//              usage.                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

const UI32 FrameworkToolKit::getMyLocationId ()
{
    LocationId         locationid                           = 0;

    LocationBase *pThisLocation = getPThisLocation ();
    if (NULL != pThisLocation)
    {
        locationid = ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getLocationId ());
    }
    return (locationid);
}

const UI32 FrameworkToolKit::getThisLocationId ()
{
    LocationId         locationid                           = getMyLocationId ();
    WaveManagementInterfaceRole waveManagementInterfaceRole = getManagementInterfaceRole ();

    if (WAVE_MGMT_INTF_ROLE_SERVER == waveManagementInterfaceRole)
    {
    WaveObjectManager *pWaveObjectManagerForCurrentThread   = PrismThread::getWaveObjectManagerForCurrentThread ();

    if ((PrismThread::getSelf ()) != (PrismThread::getPrismThreadForServiceId (ReservedWaveLocalObjectManager::getPrismServiceId ()))->getId ())
    {
        PrismMessage *pPrismMessage = pWaveObjectManagerForCurrentThread->getPInputMesage ();
        if (NULL != pPrismMessage && (true == pPrismMessage->getIsMessageBeingSurrogatedFlag ()))
        {
            locationid = pPrismMessage->getSurrogatingForLocationId ();
        }
    }
    }

    return (locationid);
}

const SI32 FrameworkToolKit::getThisLocationSlotId ()
{
    SI32           status = 0;
    string         cmdToGetSlotId("/sbin/sin | grep \"Slot No\" | cut -d':' -f2");
    vector<string> output;
    SI32           slotId = -1;

    status = FrameworkToolKit::systemCommandOutput (cmdToGetSlotId.c_str(),output);

    if (status == 0)
    {
        if ( (output.size () > 0) && ('\0' != (output[0].c_str())[0]) )
        {
            slotId = atoi (output[0].c_str ());
        }
    }

    return slotId;
}

const string FrameworkToolKit::getThisLocationIpAddress ()
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getIpAddress ());
    }
    else
    {
        return (string (""));
    }
}

const SI32 FrameworkToolKit::getThisLocationPort ()
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getPort ());
    }
    else
    {
        return (0);
    }
}

const LocationRole FrameworkToolKit::getThisLocationRole ()
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getRole ());
    }
    else
    {
        return (LOCATION_STAND_ALONE);
    }
}

const LocationId FrameworkToolKit::getClusterPrimaryLocationId ()
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getClusterPrimaryLocationId ());
    }
    else
    {
        // FIXME : sagar : replace 0 with NullLocationId
        return (0);
    }
}

const string FrameworkToolKit::getClusterPrimaryIpAddress ()
{
    LocationId clusterPrimaryLocationId = getClusterPrimaryLocationId ();

    return (getIpAddressForLocationId (clusterPrimaryLocationId));
}

const SI32 FrameworkToolKit::getClusterPrimaryPort ()
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getClusterPrimaryPort ());
    }
    else
    {
        return (0);
    }

}

const string FrameworkToolKit::getServiceNameById (const PrismServiceId &id)
{
    return (PrismThread::getPrismServiceNameForServiceId (id));
}

const PrismServiceId FrameworkToolKit::getServiceIdByName (const string &serviceName)
{
    PrismServiceId prismServiceId = 0;
    WaveManagementInterfaceRole waveManagementInterfaceRole = getManagementInterfaceRole ();

    if ((WAVE_MGMT_INTF_ROLE_SERVER == waveManagementInterfaceRole) || (WAVE_MGMT_INTF_ROLE_CLI == waveManagementInterfaceRole))
    {
        prismServiceId = PrismThread::getPrismServiceIdForServiceName (serviceName);
    }
    else
    {
        prismServiceId = WaveClientTransportObjectManager::getPrismServiceId ();
    }

    trace (TRACE_LEVEL_DEBUG, "FrameworkToolKit::getServiceIdByName : Service Name : " + serviceName + string (" :" ) + prismServiceId);

    return (prismServiceId);
}

LocationId FrameworkToolKit::getLocationIdForIpAddressAndPort (const string &ipAddress, const SI32 &port)
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getLocationIdForIpAddressAndPort (ipAddress, port));
    }
    else
    {
        // FIXME : sagar : replace 0 with NullLocationId
        return (0);
    }
}

string FrameworkToolKit::getIpAddressForLocationId (const LocationId &locationId)
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getIpAddressForLocationId (locationId));
    }
    else
    {
        return ("");
    }
}

SI32 FrameworkToolKit::getPortForLocationId (const LocationId &locationId)
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return ((PrismFrameworkObjectManager::getInstance ())->m_pThisLocation->getPortForLocationId (locationId));
    }
    else
    {
        return (0);
    }
}

const bool FrameworkToolKit::isThisALocation ()
{
    LocationId thisLocationId = getThisLocationId ();

    if (0 == thisLocationId)
    {
        return (false);
    }
    else
    {
        return ((0 == (thisLocationId & 0xFF)) ? true : false);
    }
}

const bool FrameworkToolKit::isThisASubLocation ()
{
    LocationId thisLocationId = getThisLocationId ();

    if (0 == thisLocationId)
    {
        return (false);
    }
    else
    {
        return ((0 != (thisLocationId & 0xFF)) ? true : false);
    }
}

const bool FrameworkToolKit::isAKnownLocation (const string &ipAddress, const SI32 &port)
{
    if (0 != (getLocationIdForIpAddressAndPort (ipAddress, port)))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

const bool FrameworkToolKit::isAKnownLocation (const LocationId &locationId)
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return (pThisLocation->isAKnownLocation (locationId));
    }
    else
    {
        return (false);
    }
}

const bool FrameworkToolKit::isAConnectedLocation (const LocationId &locationId)
{
    LocationBase *pThisLocation = getPThisLocation ();

    if (NULL != pThisLocation)
    {
        return (pThisLocation->isAConnectedLocation (locationId));
    }
    else
    {
        return (false);
    }
}

void FrameworkToolKit::getKnownRemoteLocations (vector<LocationId> &remoteLocationsVector)
{
    PrismFrameworkObjectManager *pTemp = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        pTemp->getKnownRemoteLocations (remoteLocationsVector);
    }
}

void FrameworkToolKit::getKnownLocations (vector<LocationId> &knownLocationsVector)
{
    PrismFrameworkObjectManager *pTemp = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        pTemp->getKnownLocations (knownLocationsVector);
    }
}

void FrameworkToolKit::getConnectedLocations (vector<LocationId> &connectedLocationsVector)
{
    PrismFrameworkObjectManager *pTemp = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        pTemp->getConnectedLocations (connectedLocationsVector);
    }
}

void FrameworkToolKit::getKnownSubLocations (vector<LocationId> &knownSubLocationsVector)
{
    PrismFrameworkObjectManager *pTemp = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        pTemp->getKnownSubLocations (knownSubLocationsVector);
    }
}

const bool FrameworkToolKit::isAnIpAddressForThisMachine (const string &ipAddress)
{
    if (("" == ipAddress) || ((getLoopBackIpAddressForThisMachine ()) == ipAddress))
    {
        return (true);
    }

    string interfaceName = PrismFrameworkObjectManager::getEthernetInterfaceForThisLocation();
    if ("" == interfaceName)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkToolKit::isAnIpAddressForThisMachine: No specific Ethernet Interface Assigned");
        return (false);
    }

    string interfaceIpAddress = StreamingSocket::getIpAddressForInterface (interfaceName);
    if ("" == interfaceIpAddress)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkToolKit::isAnIpAddressForThisMachine: failed to fetch IP Address for Ethernet Interface \"" + interfaceName + "\"");
        string interfaceState = getStateForInterface(interfaceName);
        trace (TRACE_LEVEL_INFO, string("FrameworkToolKit::isAnIpAddressForThisMachine: :getStateForInterface ") + interfaceName + string(" : ") + interfaceState);
        return (false);
    }

    if (interfaceIpAddress.compare (ipAddress) == 0)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

const string &FrameworkToolKit::getProcessInitialWorkingDirectory ()
{
    return (s_processInitialWorkingDirectory);
}

const string FrameworkToolKit::getProcessCurrentWorkingDirectory ()
{

    string currentWorkingDirectory ("");

    char *pTempBuffer = new char[4096];

    prismAssert (NULL != pTempBuffer, __FILE__, __LINE__);

    if (NULL != pTempBuffer)
    {
        if (NULL != prismGetCwd (pTempBuffer, 4096))
        {
        	currentWorkingDirectory = pTempBuffer;
        }

        delete[] pTempBuffer;
    }
    return currentWorkingDirectory;
}

const string FrameworkToolKit::getTraceClientNameById (const TraceClientId traceClientId)
{
    TraceObjectManager *pTraceObjectManager = TraceObjectManager::getInstance ();

    if (NULL == pTraceObjectManager)
    {
        return ("");
    }

    return (pTraceObjectManager->getTraceClientName (traceClientId));
}
const bool FrameworkToolKit::isAResourceId (ResourceId resourceId)
{
    return (WaveNs::isAResourceId (resourceId));
}

const bool FrameworkToolKit::isAResourceEnum (ResourceEnum resourceEnum)
{
    return (WaveNs::isAResourceEnum (resourceEnum));
}

const string FrameworkToolKit::localize (const ResourceId &resourceId)
{
    return (WaveNs::localizeResourceId (resourceId));
}

const string FrameworkToolKit::localize (const ResourceId &resourceId, const vector <Attribute *> &arguments)
{
    UI32    i = 0;
    string  resourceString = localize (resourceId);
    string  findString;
    size_t  position = 0;
    char    buffer [4];
    string  attributeString;

    for (i = 0; i < arguments.size (); i++)
    {
        snprintf (buffer, 4, "%d", i+1);
        findString = string ("%") + string (buffer);
        position = resourceString.find(findString, position);
        if (string::npos == position)
        {
            prismAssert (false, __FILE__, __LINE__);
        }
        arguments[i]->toString (attributeString);
        resourceString.replace (position, findString.length (), attributeString.c_str ());
        position = position + attributeString.length ();
    }

    return (resourceString);
}

const string FrameworkToolKit::localizeToSourceCodeEnum (ResourceId resourceId)
{
    return (WaveNs::localizeToSourceCodeEnum (resourceId));
}

const ResourceId FrameworkToolKit::localizeToSourceCodeResourceId (const string &sourceCodeEnum)
{
    return (WaveNs::localizeToSourceCodeResourceId (sourceCodeEnum));
}

void FrameworkToolKit::getResourceIdsForResourceEnum (const ResourceEnum &resourceEnum, vector<ResourceId> &resourcesVector)
{
    vector<unsigned long int> rv;
    UI32                      numberOfResources = 0;
    UI32                      i                 = 0;

    WaveNs::localizeResourceEnum (resourceEnum, rv);
    numberOfResources = rv.size ();

    for (i = 0; i < numberOfResources; i++)
    {
        resourcesVector.push_back (rv[i]);
    }
}

UI32 FrameworkToolKit::printNumberOfCpus (UI32 argc, vector<string> argv)
{
    trace (TRACE_LEVEL_INFO, string ("Number Of Cpus : ") + getNumberOfCpus ());

    return (0);
}

UI32 FrameworkToolKit::debugResourceId (UI32 argc, vector<string> argv)
{
    if (argc > 1)
    {
        string     resourceIdString = argv[1].c_str ();
        ResourceId resourceId       = 0;

        if (((resourceIdString.length ()) > 2) && ('0' == resourceIdString[0]) && (('x' == resourceIdString[1]) || ('X' == resourceIdString[1])))
        {
            resourceId = strtoul (resourceIdString.c_str (), NULL, 16);
        }
        else
        {
            resourceId = strtoul (resourceIdString.c_str (), NULL, 10);
        }

        if (true == isAResourceId (resourceId))
        {
            printf ("%lu : ResourceId (0x%08lX) : \"%s\"\n", (unsigned long int) resourceId, (unsigned long int) resourceId, (localize (resourceId)).c_str ());
        }
        else if (true == isAResourceEnum (resourceId))
        {
            vector<ResourceId> resourcesVector;
            UI32               numberOfResources = 0;
            UI32               i                 = 0;

            getResourceIdsForResourceEnum (resourceId, resourcesVector);
            numberOfResources = resourcesVector.size ();

            printf ("|-->%010lu : ResourceEnum (0x%08lX)\n", (unsigned long int) resourceId, (unsigned long int) resourceId);

            for (i = 0; i < numberOfResources; i++)
            {
                printf ("    |-->%010lu : ResourceId (0x%08lX) : \"%s\"\n", (unsigned long int) (resourcesVector[i]), (unsigned long int) (resourcesVector[i]), (localize (resourcesVector[i])).c_str ());
            }
        }
        else
        {
            printf ("N O T  A  K N O W N  R E S O U R C E  I D / E N U M\n");
        }
    }
    else
    {
        cout << "Must supply a resource id/enumeration as an argument." << endl;
    }

    return (0);
}

UI32 FrameworkToolKit::debugAllResourceEnums (UI32 argc, vector<string> argv)
{
    vector<unsigned long int> allResourceEnums;
    UI32                      numberofResourceEnums = 0;
    UI32                      i                     = 0;

    getAllResourceEnums (allResourceEnums);
    numberofResourceEnums = allResourceEnums.size ();

    for (i = 0; i < numberofResourceEnums; i++)
    {
        vector<string> arguments;

        arguments.push_back ("debugResourceId");
        arguments.push_back (string ("") + allResourceEnums[i]);

        debugResourceId (2, arguments);
    }

    return (0);
}

void FrameworkToolKit::registerDebugShellEntries ()
{
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::printNumberOfCpus),                   "ncpu");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::debugResourceId),                     "localize");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::debugAllResourceEnums),               "localizeallresourceenums");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::debugSavePrismConfiguration),         "saveprismconfig");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::printServices),                       "printservices");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::printServices),                       "ps");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::printRemoteLocations),                "printremotelocations");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::assertTest),                          "assert");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::sigSegvTest),                         "sigsegv");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::debugMessageLeak),                    "dumpmessageleak");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::debugObjectLeak),                     "dumpobjectleak");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::debugObjectLeakAll),                  "dumpobjectleakall");
    addDebugFunction ((ShellCmdFunction) (&FrameworkToolKit::analyzeUri),                          "analyzeuri");

    addDebugFunction ((ShellCmdFunction) (&RedundancyOptimizerBase::optimizePathDebugShellEntree), "reduce");
}

const SI32 FrameworkToolKit::getPrismDefaultTcpPort ()
{
    return (3016);
}

const SI32 FrameworkToolKit::getPrismDefaultDatabaseTcpPort ()
{
    return (9013);
}

const ResourceId FrameworkToolKit::savePrismConfiguration (const bool &syncToStandby)
{
    PrismFrameworkObjectManager *pTemp = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        printf ("Saving Wave Configuration...\n");
        return (pTemp->saveConfiguration (syncToStandby));
    }
    else
    {
        printf ("Cannot Save Configuration at this time.  Wave Framework is not available.\n");
        return (0);
    }
}

const ResourceId FrameworkToolKit::getPrismConfigurationValidity (bool &validity)
{

    PrismFrameworkObjectManager *pTemp = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        validity = pTemp->getPrismConfigurationValidity ();
        return FRAMEWORK_SUCCESS;
    }
    else
    {
        printf ("Cannot get configuration validity at this time.  Wave Framework is not available.\n");
        return FRAMEWORK_ERROR;
    }
}

const ResourceId FrameworkToolKit::changePrismConfigurationValidity( const bool &validity )
{
    PrismFrameworkObjectManager *pTemp = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        return (pTemp->changePrismConfigurationValidity ( validity) );
    }
    else
    {
        printf ("Cannot Invalidate Configuration at this time.  Wave Framework is not available.\n");
        return (0);
    }
}


ResourceId FrameworkToolKit::debugSavePrismConfiguration (UI32 argc, vector<string> argv)
{
    return (savePrismConfiguration ());
}

const bool FrameworkToolKit::isALocalService (const PrismServiceId &prismServiceId)
{
    return (WaveLocalObjectManager::isALocalService (prismServiceId));
}

const bool FrameworkToolKit::isPrimaryLocation ()
{
    return ((LOCATION_PRIMARY == (getThisLocationRole ())) || (LOCATION_PRIMARY_UNCONFIRMED == (getThisLocationRole ())) || (LOCATION_SECONDARY_UNCONFIRMED == (getThisLocationRole ())));
}

const bool FrameworkToolKit::isStandAloneLocation ()
{
    return (LOCATION_STAND_ALONE == (getThisLocationRole ()));
}

ResourceId FrameworkToolKit::printServices (UI32 argc, vector<string> argv)
{
    vector<PrismServiceId> prismServiceIds;
    UI32                   numberOfPrismServices;
    UI32                   i;

    PrismThread::getListOfServiceIds (prismServiceIds);
    numberOfPrismServices = prismServiceIds.size ();

    for (i = 0; i < numberOfPrismServices; i++)
    {
        printf ("%5u:%50s:%10s:%10s\n", prismServiceIds[i], (FrameworkToolKit::getServiceNameById (prismServiceIds[i])).c_str (), WaveObjectManager::isServiceEnabled (prismServiceIds[i]) ? "Enabled" : "Disabled", isALocalService(prismServiceIds[i]) ? "Local" : "Global");
    }

    return (0);
}

ResourceId FrameworkToolKit::printRemoteLocations (UI32 argc, vector<string> argv)
{
    vector<LocationId> knownRemoteLocationIds;
    UI32               numberOfKnownRemoteLocations = 0;
    UI32               i                            = 0;
    LocationId         tempLocationId               = 0;
    string             tempIpAddress;
    SI32               tempPort                     = 0;

    getKnownRemoteLocations (knownRemoteLocationIds);

    numberOfKnownRemoteLocations = knownRemoteLocationIds.size ();

    printf ("Total Number of Known Remote Locations : %u\n", numberOfKnownRemoteLocations);

    for (i = 0; i < numberOfKnownRemoteLocations; i++)
    {
        tempLocationId = knownRemoteLocationIds[i];
        tempIpAddress  = getIpAddressForLocationId (tempLocationId);
        tempPort       = getPortForLocationId (tempLocationId);

        printf ("%10u : %15s : %8d\n", tempLocationId, tempIpAddress.c_str (), tempPort);
    }

    printf ("\n");

    return (0);
}

ResourceId FrameworkToolKit::assertTest (UI32 argc, vector<string> argv)
{
    prismAssert (false, __FILE__, __LINE__);

    return (0);
}

ResourceId FrameworkToolKit::sigSegvTest (UI32 argc, vector<string> argv)
{
    int a = 0;

    *(&a) = *((int *) (NULL));

    return (0);
}

ResourceId FrameworkToolKit::debugMessageLeak (UI32 argc, vector<string> argv)
{
    if (argc < 2)
    {
        tracePrintf (TRACE_LEVEL_ERROR, "FrameworkToolKit::debugMessageLeak : Please specify a Prism Service Id");
    }
    else
    {
        vector<PrismServiceId>  messageServiceIds;
        vector<UI32>            messageOperationCodes;
        vector<string>          btStrings;
        vector<WaveMessageType> messageTypes;
        PrismServiceId          prismServiceId         = atoi ((argv[1]).c_str ());
        UI32                    numberOfLeakedMessages = 0;
        UI32                    i                      = 0;

        MessageTracker::getMessages (prismServiceId, messageServiceIds, messageOperationCodes, messageTypes, btStrings);

        numberOfLeakedMessages = messageServiceIds.size ();

        trace (TRACE_LEVEL_INFO, "Leaked Message Summary For Service : \'" + getServiceNameById (prismServiceId) + "\'");

        tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "%-7s %-6s %-50s %-16s %-16s", "_______", "______", "__________________________________________________", "________________", "________________");
        tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "%-7s %-6s %-50s %-16s %-16s", "   #   ", " S ID ", "                   Service Name                   ", "     OpCode     ", "      Type      ");
        tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "%-7s %-6s %-50s %-16s %-16s", "_______", "______", "__________________________________________________", "________________", "________________");

        for (i = 0; i < numberOfLeakedMessages; i++)
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, false, "%07u %06u %50s %-16u %-16s", i, messageServiceIds[i], (getServiceNameById (messageServiceIds[i])).c_str (), messageOperationCodes[i], (localize (messageTypes[i])).c_str ());
        }

        tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "%-7s %-6s %-50s %-16s %-16s", "_______", "______", "__________________________________________________", "________________", "________________");

        if (0 < numberOfLeakedMessages)
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, false, "Total Number Of Message / Event Leaks found : %u", numberOfLeakedMessages);
        }
        else
        {
            tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "NO Message / Event Leaks Found.");
        }
    }

    return (0);
}

ResourceId FrameworkToolKit::debugObjectLeak (UI32 argc, vector<string> argv)
{
    if (argc < 2)
    {
        tracePrintf (TRACE_LEVEL_ERROR, "FrameworkToolKit::debugObjectLeak : Please specify a Prism Service Id");
    }
    else
    {
        vector<string>  managedObjectClassNames;
        vector<string>  managedObjectNames;
        vector<bool>    queryResults;
        vector<string>  btStrings;
        PrismServiceId  prismServiceId           = atoi ((argv[1]).c_str ());
        UI32            numberOfLeakedObjects    = 0;
        UI32            i                        = 0;

        ObjectTracker::getObjects (prismServiceId, managedObjectClassNames, managedObjectNames, queryResults, btStrings);

        numberOfLeakedObjects = managedObjectClassNames.size ();

        trace (TRACE_LEVEL_INFO, "Leaked Object Summary For Service : \'" + getServiceNameById (prismServiceId) + "\'");

        for (i = 0; i < numberOfLeakedObjects; i++)
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, false, "%07u %-50s %-50s %-5s", i + 1, managedObjectClassNames[i].c_str (),managedObjectNames[i].c_str (), queryResults[i] ? "TRUE" : "FALSE");
        }

        if (0 < numberOfLeakedObjects)
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, false, "Total Number Of Object Leaks found : %u", numberOfLeakedObjects);
        }
        else
        {
            tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "NO Object Leaks Found.");
        }
    }

    return (0);
}

ResourceId FrameworkToolKit::debugObjectLeakAll (UI32 argc, vector<string> argv)
{
    vector<string>         serviceNames;
    vector<PrismServiceId> serviceIds;
    UI32                   numberOfServices = 0;
    UI32                   i                = 0;

    getServices (serviceNames, serviceIds);

    numberOfServices = serviceIds.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        vector<string> tempArgv;

        tempArgv.push_back ("dumpobjectleak");
        tempArgv.push_back (string ("") + serviceIds[i]);

        debugObjectLeak (tempArgv.size (), tempArgv);
    }

    return (0);
}

ResourceId FrameworkToolKit::analyzeUri (UI32 argc, vector<string> argv)
{
    if (argc >= 2)
    {
        string uriString = argv[1];

        Uri uri;

        uri.fromString (uriString);

        uri.print ();
    }

    return (0);
}

string FrameworkToolKit::getPrismConnectionPassPhrase ()
{
    return (string ("PmrsiisrmPHello8PmrsiisrmPHello9"));
}

string FrameworkToolKit::getPrismHaConnectionPassPhrase ()
{
    return (string ("PmrsiisrmPHello3PmrsiisrmPHello3"));
}

const SI32 FrameworkToolKit::getManagementInterfaceReceiverPort ()
{
    return (s_managementInterfaceReceiverPort);
}

void FrameworkToolKit::setManagementInterfaceReceiverPort (const SI32 &managementInterfaceReceiverPort)
{
    s_managementInterfaceReceiverPort = managementInterfaceReceiverPort;
}

const SI32 FrameworkToolKit::getManagementInterfaceClientReceiverPort ()
{
    return (s_managementInterfaceClientReceiverPort);
}

void FrameworkToolKit::setManagementInterfaceClientReceiverPort (const SI32 &managementInterfaceClientReceiverPort)
{
    s_managementInterfaceClientReceiverPort = managementInterfaceClientReceiverPort;
}

const string FrameworkToolKit::getDatabaseBackupFileName ()
{
    return ("PrismDatabase.pgd");
}

const string FrameworkToolKit::getDatabaseBackupFileName2 ()
{
    return ("PrismDatabase.previous.pgd");
}

const string FrameworkToolKit::getDatabaseBackupFileName3 ()
{
    return ("PrismDatabase.primary.backup.pgd");
}

const string FrameworkToolKit::getDatabaseBackupFileForFwdl ()
{
    return (s_dbBackupFileOnFwdl);
}

void FrameworkToolKit::setDatabaseBackupFileForFwdl (const string &dbDumpFile)
{
    s_dbBackupFileOnFwdl = dbDumpFile;
}

ObjectId FrameworkToolKit::getObjectIdForSlotNumber (const UI32 slotNumber, const LocationId locationId)
{
    ClusterLocalGetSlotObjectIdMessage *pClusterLocalGetSlotObjectIdMessage = new ClusterLocalGetSlotObjectIdMessage (slotNumber, locationId);

    ObjectId retSlotObjectId;

    ResourceId status = WaveObjectManagerToolKit::sendSynchronously (pClusterLocalGetSlotObjectIdMessage);

    if (status != WAVE_MESSAGE_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkToolKit::getObjectIdForSlotNumber : ClusterLocalGetSlotObjectIdMessage failed ");
    }
    else
    {
        status = pClusterLocalGetSlotObjectIdMessage->getCompletionStatus ();
        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_DEBUG, "FrameworkToolKit::getObjectIdForSlotNumber : ClusterLocalGetSlotObjectIdMessage completion status is not success ");
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "FrameworkToolKit::getObjectIdForSlotNumber : Got Slot ObjectId ");
            retSlotObjectId = pClusterLocalGetSlotObjectIdMessage->getSlotObjectId ();
        }
    }

    delete pClusterLocalGetSlotObjectIdMessage;

    return (retSlotObjectId);
}

ObjectId FrameworkToolKit::createDummySlot (const UI32 slotNumber)
{
    ObjectId slotObjectId = ObjectId::NullObjectId;

    PrismFrameworkObjectManager *pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();

    if (NULL != pPrismFrameworkObjectManager)
    {
        pPrismFrameworkObjectManager->createDummySlot (slotNumber);

        //Get the slotObjectId of the dummy slot created
        slotObjectId =  getObjectIdForSlotNumber (slotNumber);
    }

    return (slotObjectId);
}

ObjectId FrameworkToolKit::getObjectIdForLocationId (LocationId locationId)
{
    ClusterLocalGetLocationObjectIdMessage *pClusterLocalGetLocationObjectIdMessage = new ClusterLocalGetLocationObjectIdMessage (locationId);

    ObjectId retLocationObjectId;

    ResourceId status = WaveObjectManagerToolKit::sendSynchronously (pClusterLocalGetLocationObjectIdMessage);

    if (status != WAVE_MESSAGE_SUCCESS)
    {
        trace (TRACE_LEVEL_ERROR, "FrameworkToolKit::getObjectIdForLocationId : ClusterLocalGetLocationObjectIdMessage failed ");
    }
    else
    {
        status = pClusterLocalGetLocationObjectIdMessage->getCompletionStatus ();
        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, "FrameworkToolKit::getObjectIdForLocationId : ClusterLocalGetLocationObjectIdMessage completion status is not success ");
        }
        else
        {
            retLocationObjectId = pClusterLocalGetLocationObjectIdMessage->getLocationObjectId ();
        }
    }

    delete pClusterLocalGetLocationObjectIdMessage;

    return (retLocationObjectId);

}

const string FrameworkToolKit::getUniqueString (const string &ipAddress, const UI32 &port)
{
    return (ipAddress + string (":") + port);
}

void FrameworkToolKit::getServices (vector<string> &serviceNames, vector<UI32> &serviceIds)
{
    UI32                   numberOfPrismServices;
    UI32                   i;

    PrismThread::getListOfServiceIds (serviceIds);
    numberOfPrismServices = serviceIds.size ();

    for (i = 0; i < numberOfPrismServices; i++)
    {
        serviceNames.push_back (FrameworkToolKit::getServiceNameById (serviceIds[i]));
    }
}

WaveManagementInterfaceRole FrameworkToolKit::getManagementInterfaceRole ()
{
    return (s_waveManagementInterfaceRole);
}

void FrameworkToolKit::setManagementInterfaceRole (const WaveManagementInterfaceRole &managementInterfaceRole)
{
    s_waveManagementInterfaceRole = managementInterfaceRole;
}

TraceLevel FrameworkToolKit::getDefaultTraceLevel ()
{
    WaveManagementInterfaceRole waveManagementInterfaceRole = getManagementInterfaceRole ();

    if ((WAVE_MGMT_INTF_ROLE_SERVER == waveManagementInterfaceRole) || (WAVE_MGMT_INTF_ROLE_CLI == waveManagementInterfaceRole))
    {
        return (s_defaultTraceLevel);
    }
    else if (WAVE_MGMT_INTF_ROLE_CLIENT == waveManagementInterfaceRole)
    {
        return (s_defaultTraceLevelForClient);
    }

    return (s_defaultTraceLevel);
}

void FrameworkToolKit::setDefaultTraceLevel (const TraceLevel &defaultTraceLevel)
{
    WaveManagementInterfaceRole waveManagementInterfaceRole = getManagementInterfaceRole ();

    if ((WAVE_MGMT_INTF_ROLE_SERVER == waveManagementInterfaceRole) || (WAVE_MGMT_INTF_ROLE_CLI == waveManagementInterfaceRole))
    {
        s_defaultTraceLevel = defaultTraceLevel;
    }
    else if (WAVE_MGMT_INTF_ROLE_CLIENT == waveManagementInterfaceRole)
    {
        setDefaultTraceLevelForClient (defaultTraceLevel);
    }
}

void FrameworkToolKit::setDefaultTraceLevelForClient (const TraceLevel &defaultTraceLevelForClient)
{
    s_defaultTraceLevelForClient = defaultTraceLevelForClient;
}

UI32 FrameworkToolKit::getNumberOfCpus ()
{
    s_numberOfCpusMutex.lock ();

    UI32 numberOfCpus = 0;

    if (0 == s_numberOfCpus)
    {
        long temp = sysconf (_SC_NPROCESSORS_ONLN);

        if (-1 == temp)
        {
            s_numberOfCpus = 1;
        }
        else
        {
            s_numberOfCpus = (UI32) temp;
        }
    }

    numberOfCpus = s_numberOfCpus;

    s_numberOfCpusMutex.unlock ();

    return (numberOfCpus);
}

void FrameworkToolKit::setNumberOfCpus (const UI32 &numberOfCpus)
{
    s_numberOfCpusMutex.lock ();

    s_numberOfCpus = numberOfCpus;

    s_numberOfCpusMutex.unlock ();
}

ObjectId FrameworkToolKit::getThisWaveNodeObjectId ()
{
    if (false == (ClusterLocalObjectManager::getIsInstantiated ()))
    {
        return (ObjectId::NullObjectId);
    }
    else
    {
        return ((ClusterLocalObjectManager::getInstance ())->getThisWaveNodeObjectId ());
    }
}

bool FrameworkToolKit::isIpAddressValid (const string &ipAddress)
{
    sockaddr_in socketAddress;

    SI32 status = inet_pton (AF_INET, ipAddress.c_str (), &socketAddress.sin_addr);

    if (0 >= status)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

ResourceId FrameworkToolKit::checkForExistenceOfOneMO (WaveManagedObjectSynchronousQueryContext *pWaveManagedObjectSynchronousQueryContext, const string &classToQuery)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    trace (TRACE_LEVEL_DEVEL, string ("FrameworkToolKit::checkForExistenceOfOneMO : Entering ..."));
    prismAssert (NULL != pWaveManagedObjectSynchronousQueryContext, __FILE__, __LINE__);

    vector<WaveManagedObject *> *pResults = WaveObjectManagerToolKit::querySynchronously (pWaveManagedObjectSynchronousQueryContext);
    UI32 i = 0;

    if (0 == pResults->size ())
    {
        trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::checkForExistenceOfOneMO : Zero entries found in ") + classToQuery);
        status = WAVE_MESSAGE_ERROR;
    }
    else if (2 == pResults->size ())
    {
        ObjectId object1 = ((*pResults)[0])->getObjectId ();
        ObjectId object2 = ((*pResults)[1])->getObjectId ();
        if (object1 == object2)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::checkForExistenceOfOneMO : Duplicate entries found in ") + classToQuery + ". The ObjectIds are " + object1.toString () + " and " + object2.toString ());
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::checkForExistenceOfOneMO : Two entries found in ") + classToQuery);
            prismAssert (false, __FILE__, __LINE__);
        }

    }
    else if (2 < pResults->size ())
    {
        trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::checkForExistenceOfOneMO : Multiple entries found in ") + classToQuery);
        prismAssert (false, __FILE__, __LINE__);
    }
    for (i = 0; i < pResults->size (); i++)
    {
        delete (*pResults)[i];
    }

    pResults->clear ();
    delete pResults;

    return status;
}

bool FrameworkToolKit::getIsAbruptReboot ()
{
    return (s_isAbruptReboot);
}

void FrameworkToolKit::setIsAbruptReboot (const bool &isAbruptReboot)
{
    s_isAbruptReboot = isAbruptReboot;
}

bool FrameworkToolKit::getIsStartupValid ()
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();

    if (NULL != pTemp)
    {
        return (pTemp->getIsStartupValid ());
    }
    else
    {
        return (false);
    }
}

void FrameworkToolKit::setIsStartupValid (const bool &isValid)
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();

    if (NULL != pTemp)
    {
        pTemp->setIsStartupValid (isValid);
    }
}

string FrameworkToolKit::getStartupFileName ()
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();

    if (NULL != pTemp)
    {
        return (pTemp->getStartupFileName ());
    }
    else
    {
        return (string ());
    }
}

string FrameworkToolKit::getStartupBackupFileName ()
{
    string startupBkFileName = getStartupFileName() + ".bk";
    trace (TRACE_LEVEL_DEBUG, "FrameworkToolKit::getStartupBackupFileName : " + startupBkFileName);

    return (startupBkFileName);
}

bool FrameworkToolKit::isStartupConfigBackupFileExist ()
{
    ifstream backupStartupFileStream((getStartupBackupFileName ()).c_str());

    if(backupStartupFileStream.is_open())
    {
        backupStartupFileStream.close();
        trace(TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::isBackupStartupConfigFileExist: backup for Startup configuration file exists.");
        return (true);
    }

    return (false);
}

void FrameworkToolKit::setStartupFileName (const string &startupFileName)
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();

    if (NULL != pTemp)
    {
        pTemp->setStartupFileName (startupFileName);
    }
}

ResourceId FrameworkToolKit::getStartupFileType ()
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();

    if (NULL != pTemp)
    {
        return (pTemp->getStartupFileType ());
    }
    else
    {
        return (FRAMEWORK_ERROR);
    }
}

void FrameworkToolKit::setStartupFileType (const ResourceId &startupFileType)
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();

    if (NULL != pTemp)
    {
        pTemp->setStartupFileType (startupFileType);
    }
}

LocationId FrameworkToolKit::getLastUsedLocationId ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getLastUsedLocationIdInPrismFrameworkObjectManager ());
}

void FrameworkToolKit::setLastUsedLocationId (const LocationId &locationId)
{
    (PrismFrameworkObjectManager::getInstance ())->setLastUsedLocationIdInPrismFrameworkObjectManager (locationId);
}

SI32 FrameworkToolKit::systemCommandOutput (const string &command, vector<string> &output)
{
    char   psBuffer[128];
    FILE   *pPipe;
    SI32   status = -1000 ;
    cout << "FrameworkToolKit::systemCommandOutput : Executing command " << command << endl << "\r";
    if( (pPipe = popen( command.c_str(), "r" )) == NULL )
    {
        return status;
    }

    output.clear();
    while( !feof( pPipe ) )
    {
        if( fgets( psBuffer, 128, pPipe ) != NULL )
        {
            //printf( psBuffer );
            output.push_back(psBuffer);
        }
    }

    status = pclose( pPipe );
    return (WEXITSTATUS(status));
}

const bool FrameworkToolKit::getSecondaryClusterFormationFlag ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getSecondaryNodeClusterCreationFlag ());
}


void FrameworkToolKit::enableFileServiceUse(void)
{
     s_useFileServiceForFileTransfer = true;
}

void FrameworkToolKit::disableFileServiceUse(void)
{
     s_useFileServiceForFileTransfer = false;
}

const bool FrameworkToolKit::isFileServiceEnabled()
{
    return (s_useFileServiceForFileTransfer);
}

bool FrameworkToolKit::isWarmHaRecoveryPreparationInProgress ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->isWarmHaRecoveryPreparationInProgress ());
}

bool FrameworkToolKit::isActivationCompleted ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getActivationCompleted ());
}

string FrameworkToolKit::getActivationCompletedTimeStamp ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getActivationCompletedTimeStamp ());
}

void FrameworkToolKit::incrementLineCardPostBootCount ()
{
    (PrismFrameworkObjectManager::getInstance ())->incrementLineCardPostBootCount ();
}

UI32 FrameworkToolKit::getNumberOfLineCardPostBootCurrentlyRunning ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getNumberOfLineCardPostBootCurrentlyRunning ());
}

void FrameworkToolKit::decrementLineCardPostBootCount ()
{
    (PrismFrameworkObjectManager::getInstance ())->decrementLineCardPostBootCount ();
}

bool FrameworkToolKit::isPostBootStarted ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getPostBootStarted ());
}

string FrameworkToolKit::getPostBootStartTimeStamp()
{
     return ((PrismFrameworkObjectManager::getInstance ())->getPostBootStartedTimeStamp ());
}

bool FrameworkToolKit::isPostBootCompleted ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getPostBootCompleted ());
}


string FrameworkToolKit::getPostBootCompleteTimeStamp ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getPostBootCompletedTimeStamp ());
}


bool FrameworkToolKit::isConfigReplayStarted ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getConfigReplayStarted ());
}

string FrameworkToolKit::getConfigReplayStartedTimeStamp ()
{

    return ((PrismFrameworkObjectManager::getInstance ())->getConfigReplayStartedTimeStamp ());

}

bool FrameworkToolKit::isConfigReplayCompleted ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getConfigReplayCompleted ());
}


string   FrameworkToolKit::getConfigReplayCompletedTimeStamp ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getConfigReplayCompletedTimeStamp ());
}

bool FrameworkToolKit::isConfigReplayInProgress ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getConfigReplayInProgress ());
}


void FrameworkToolKit::setFileReplayStarted (bool fileReplayStarted)
{
    ((PrismFrameworkObjectManager::getInstance ())->setFileReplayStarted (fileReplayStarted));
}

void FrameworkToolKit::setFileReplayCompleted (bool fileReplayCompleted)
{
    ((PrismFrameworkObjectManager::getInstance ())->fileReplayCompletedProcessing (fileReplayCompleted));
}

bool FrameworkToolKit::isFileReplayInProgress ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getFileReplayInProgress ());
}


bool FrameworkToolKit::isNodeReadyForAllCommands ()
{
      return ((PrismFrameworkObjectManager::getInstance ())->getNodeReadyForAllCommands ());
}

bool FrameworkToolKit::isNodeZeroized ()
{

      return ((PrismFrameworkObjectManager::getInstance ())->isNodeZeroized ());
}

ResourceId FrameworkToolKit::getDbConversionStatus ()
{
      return ((PrismFrameworkObjectManager::getInstance ())->getDbConversionStatus ());
}

string FrameworkToolKit::getNodeReadyForAllCommandsTimeStamp ()
{

    return ((PrismFrameworkObjectManager::getInstance ())->getNodeReadyForAllCommandsTimeStamp ());

}

void FrameworkToolKit::rollbackCfgFile ()
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();
    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    pTemp->rollbackCfgFile ();
}
bool FrameworkToolKit::getIsDbRestoreIncomplete ()
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();
    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    return (pTemp->getIsDBRestoreIncomplete ());
}

void FrameworkToolKit::setIsDbRestoreIncomplete (bool isValid)
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();
    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    pTemp->setIsDBRestoreIncomplete (isValid);
}

// Following method is not called from prismAssert.
void FrameworkToolKit::obtainBtString (const int &startFrame, const int &numberOfFrames, string &backTraceString)
{
    prismAssert ((startFrame >= 0) && (numberOfFrames > 0), __FILE__, __LINE__);

    const UI32    BTMAXDEPTH       = 40;

    void   **pBackTraceBuffer      = new void *[BTMAXDEPTH];    // Need to free before returning. (1)
    prismAssert (NULL != pBackTraceBuffer, __FILE__, __LINE__);

    int      numberOfSymbols       = backtrace (pBackTraceBuffer, BTMAXDEPTH);
    prismAssert ((0 <= numberOfSymbols) && (40 >= numberOfSymbols), __FILE__, __LINE__);

    char   **pBackTraceSymbols     = backtrace_symbols (pBackTraceBuffer, numberOfSymbols); // pBackTraceSymbols - need to free before returning. (2)
    prismAssert (NULL != pBackTraceSymbols, __FILE__, __LINE__);

    string   tempBackTraceString   = "";
    char    *pDemangleOutPutBuffer = NULL;
    int      demangleStatus        = 0;
    size_t   begin                 = 0;
    size_t   end                   = 0;

    for (int i = startFrame; i < numberOfSymbols; i++)
    {
        if (i > (startFrame + numberOfFrames))
        {
            break;
        }

        tempBackTraceString = pBackTraceSymbols[i];

        begin = tempBackTraceString.find_first_of ('(');

        if (begin != string::npos)
        {
            end   = tempBackTraceString.find_first_of ('+', begin + 1);

            if (end != string::npos)
            {
                // char* __cxa_demangle(const char* __mangled_name, char* __output_buffer, size_t* __length, int* __status)
                // if __output_buffer is NULL, then it is malloc'ed by __cxa_demangle internally. (3)

                pDemangleOutPutBuffer = abi::__cxa_demangle ((tempBackTraceString.substr (begin + 1, end - begin - 1)).c_str (), NULL, NULL, &demangleStatus);

                if (0 == demangleStatus)
                {
                    backTraceString += '\n';
                    backTraceString += pDemangleOutPutBuffer;

                    // demangleStatus is 0, So, pDemangleOutPutBuffer should be free'ed. It contains nul terminated contents.
                    free (pDemangleOutPutBuffer); // free pDemangleOutPutBuffer. i.e. return value of __cxa_demangle. (3)
                }
            }
        }
    }

    free (pBackTraceSymbols);   // free pBackTraceSymbols. i.e. return value of backtrace_symbols. (2)
    delete[] pBackTraceBuffer;  // free pBackTraceBuffer. (allocated and passed to backtrace (..)) (1)
}

string FrameworkToolKit::getSymbolicLinkToStartupFile ()
{

    string filePath     = "/etc/fabos/Dcmd/startupFile";
    char   buffer [512];
    int    returnValue  = -1;
    string returnString;

    returnValue = readlink (filePath.c_str (), buffer, (sizeof (buffer) - 1));

    if (returnValue > -1)
    {
        buffer [returnValue] = '\0';

        string tempString = buffer;

        vector<string>  firstLevelToken;

        tokenize (tempString, firstLevelToken, '/');

        prismAssert (0 < firstLevelToken.size (), __FILE__, __LINE__);

        returnString  = firstLevelToken [firstLevelToken.size () - 1];
    }

    return (returnString);
}

bool FrameworkToolKit::isSchemaConversionDisabledByUser ()
{
    return (s_schemaConversionDisabledByUser);
}

void FrameworkToolKit::updateSchemaConversionDisabledByUser ()
{
    s_schemaConversionDisabledByUser = false;
    const string fileToDisableSchemaConversion = getProcessInitialWorkingDirectory () + "/disableschemaconversion";
    trace (TRACE_LEVEL_INFO, "FrameworkToolKit::updateSchemaConversionDisabledByUser: checking for presence of file [" + fileToDisableSchemaConversion + "]");

    ifstream fileStream (fileToDisableSchemaConversion.c_str());

    if (fileStream.is_open())
    {
        trace (TRACE_LEVEL_INFO, "FrameworkToolKit::updateSchemaConversionDisabledByUser: user has disabled the schema conversion.");
        s_schemaConversionDisabledByUser = true;

        fileStream.close();

        string deleteFileCmd = "/bin/rm -rf " + fileToDisableSchemaConversion;
        vector<string>  output;
        SI32 cmdStatus = FrameworkToolKit::systemCommandOutput (deleteFileCmd, output );

        if (0 != cmdStatus)
        {
            if (0 < (output.size ()))
            {
                trace (TRACE_LEVEL_ERROR, string("PrismFrameworkConfigurationWorker::createLockFileForConfigurationFile : deleteFileCmd = ")+ deleteFileCmd + " failed with error message : " + output[0]);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("PrismFrameworkConfigurationWorker::createLockFileForConfigurationFile : deleteFileCmd = ")+ deleteFileCmd + " failed.");
            }
        }
    }
}

void FrameworkToolKit::setDetectSchemaChange (const string &setSchemaChange)
{
    if ("false" == setSchemaChange)
    {
        s_detectSchemaChange = false;
    }
    else
    {
        s_detectSchemaChange = true;
    }
}

bool FrameworkToolKit::getDetectSchemaChange ()
{
    return (s_detectSchemaChange);
}

void FrameworkToolKit::secureDeleteDir(const string &dirName)
{
	ZeroizeForFIPSSecureDelete::cleanDeleteDir(dirName);
}

void FrameworkToolKit::secureClearFile(const string &fileName, bool removeOnClear)
{
	ZeroizeForFIPSSecureDelete::secureClearFile(fileName,removeOnClear);
}


void FrameworkToolKit::getFullyConnectedLocations (vector<LocationId> &connectedLocationsVector)
{
    PrismFrameworkObjectManager *pTemp = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        pTemp->getFullyConnectedLocations (connectedLocationsVector);
    }
}

void FrameworkToolKit::disconnectFromAllInstanceClients (const string &clientIpAddress)
{

    (ManagementInterfaceObjectManager::getInstance ())->disconnectFromClient (clientIpAddress);

}

void FrameworkToolKit::getConnectedInstances (vector<SI32> &connectedInstanceVector)
{
    (PrismFrameworkObjectManager::getInstance ())->getInstances (connectedInstanceVector);
}

void FrameworkToolKit::getConnectedInstancesAndSubInstances (vector<SI32> &connectedInstanceVector, vector<SI32> &connectedSubInstanceVector)
{
    (PrismFrameworkObjectManager::getInstance ())->getInstancesAndSubInstances (connectedInstanceVector, connectedSubInstanceVector);
}

SI32 FrameworkToolKit::getThisSlotInstance ()
{
    return ((PrismFrameworkObjectManager::getInstance ())->getThisSlotInstance ());
}

SI32 FrameworkToolKit::getHttpInterfaceReceiverPort ()
{
    return (s_httpInterfaceReceiverPort);
}

SI32 FrameworkToolKit::getHaInterfaceReceiverPort ()
{
    return (s_haInterfaceReceiverPort);
}

SI32 FrameworkToolKit::getHaPeerInterfaceReceiverPort ()
{
    return (s_haPeerInterfaceReceiverPort);
}

void FrameworkToolKit::setHttpInterfaceReceiverPort (const WaveNs::SI32& httpInterfaceReceiverPort)
{
    s_httpInterfaceReceiverPort = httpInterfaceReceiverPort;
}

void FrameworkToolKit::setHaInterfaceReceiverPort (const WaveNs::SI32& haInterfaceReceiverPort)
{
    s_haInterfaceReceiverPort = haInterfaceReceiverPort;
}

void FrameworkToolKit::setHaPeerInterfaceReceiverPort (const WaveNs::SI32& haPeerInterfaceReceiverPort)
{
    s_haPeerInterfaceReceiverPort = haPeerInterfaceReceiverPort;
}


WaveHaNodeRole FrameworkToolKit::getThisWaveHaNodeRole ()
{
    return ((ClusterLocalObjectManager::getInstance ())->getThisWaveHaNodeRole ());
}

void FrameworkToolKit::setIsConfigFileReplayGoingToBeDone (const bool &isConfigFileReplayGoingToBeDone)
{
    s_isConfigFileReplayGoingToBeDoneMutex.lock ();

    s_isConfigFileReplayGoingToBeDone = isConfigFileReplayGoingToBeDone;

    s_isConfigFileReplayGoingToBeDoneMutex.unlock ();
}

void FrameworkToolKit::setHandleDBCorruptionFlag (const string &handleDbCorruption)
{
    if ("false" == handleDbCorruption)
    {
        s_handleDBCorruption = false;
    }
    else
    {
        s_handleDBCorruption = true;
    }
}

bool FrameworkToolKit::getHandleDBCorruptionFlag ()
{
    return (s_handleDBCorruption);
}

bool FrameworkToolKit::getIsConfigFileReplayGoingToBeDone ()
{
    bool isConfigFileReplayGoingToBeDone = false;

    s_isConfigFileReplayGoingToBeDoneMutex.lock ();

    isConfigFileReplayGoingToBeDone = s_isConfigFileReplayGoingToBeDone;

    s_isConfigFileReplayGoingToBeDoneMutex.unlock ();

    return isConfigFileReplayGoingToBeDone;
}

void FrameworkToolKit::broadcastStartupSchemaChangeEvent (SchemaType fromSchema, string fromSchemaPath, SchemaType toSchema, string toSchemaPath , bool operationStatus)
{
    PersistenceLocalObjectManager *pTemp = PersistenceLocalObjectManager::getInstance ();
    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    pTemp->broadcastSchemaChangeEvent (fromSchema, fromSchemaPath, toSchema, toSchemaPath, operationStatus);
}

ResourceId FrameworkToolKit::addXPathStrings (const vector<string> &xPathStrings)
{
    ResourceId returnStatus = WAVE_MESSAGE_ERROR;

    PersistenceObjectManagerAddXPathStringsMessage persistenceObjectManagerAddXPathStringsMessage (xPathStrings);

    WaveMessageStatus  sendStatus = WaveObjectManagerToolKit::sendSynchronously (&persistenceObjectManagerAddXPathStringsMessage);

    if (WAVE_MESSAGE_SUCCESS == sendStatus)
    {
        returnStatus = persistenceObjectManagerAddXPathStringsMessage.getCompletionStatus ();
    }
    else
    {
        returnStatus = sendStatus;
    }

    return returnStatus;
}

ResourceId FrameworkToolKit::getLastUpdateTimestampsForXPathStrings (vector<string> &xPathStrings, vector<UI64> &lastUpdatedTimestamps)
{
    ResourceId returnStatus = WAVE_MESSAGE_ERROR;

    PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage (xPathStrings);

    WaveMessageStatus  sendStatus = WaveObjectManagerToolKit::sendSynchronously (&persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage);

    if (WAVE_MESSAGE_SUCCESS == sendStatus)
    {
        ResourceId completionStatus = persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            xPathStrings            = persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage.getXPathStrings ();
            lastUpdatedTimestamps   = persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage.getLastUpdatedTimestamps ();
        }

        returnStatus = completionStatus;
    }
    else
    {
        returnStatus = sendStatus;
    }

    return returnStatus;
}

void FrameworkToolKit::initializeConsolePrintFunction ()
{
    s_consoleStreamMutex.lock ();

    if (false == s_consoleStream.is_open ())
    {
        s_consoleStream.open ("/dev/console", ios::out | ios::app);
        if (!(s_consoleStream.is_open()))
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "FrameworkToolKit::consolePrint : s_consoleStream is not open.");
        }
    }

    s_consoleStreamMutex.unlock ();
}

void FrameworkToolKit::consolePrint (const string &stringToBePrinted)
{
    s_consoleStreamMutex.lock ();

    if (s_consoleStream.is_open ())
    {
        // endl is necessary for flushing the stream to out.

        s_consoleStream<<stringToBePrinted<<endl;

        // WaveNs::trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::consolePrint : Message [") + stringToBePrinted + string ("]"));
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "FrameworkToolKit::consolePrint : s_consoleStream is not open.");
    }

    s_consoleStreamMutex.unlock ();
}

void FrameworkToolKit::uninitializeConsolePrintFunction ()
{
    s_consoleStreamMutex.lock ();

    if (s_consoleStream.is_open ())
    {
        s_consoleStream.close ();
    }

    s_consoleStreamMutex.unlock ();
}

bool FrameworkToolKit::getIsSyncDumpCompleted ()
{
    PrismFrameworkObjectManager *pTemp = PrismFrameworkObjectManager::getInstance ();
    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    return (pTemp->getIsSyncDumpCompleted ());
}

void FrameworkToolKit::setRewriteStartupSchemaOnNodeReady ()
{
    s_rewriteStartupSchemaOnNodeReadyMutex.lock ();
    s_rewriteStartupSchemaOnNodeReady = true;
    s_rewriteStartupSchemaOnNodeReadyMutex.unlock ();

    // Following lines are for printing caller details.

    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();
    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    // service name is protected, So, at least print service Id.
    PrismServiceId callerThreadServiceId = pWaveObjectManagerForCurrentThread->getServiceId ();

    trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::setRewriteStartupSchemaOnNodeReady : Called by PrismServiceId [") + callerThreadServiceId + string ("], s_rewriteStartupSchemaOnNodeReady is set to TRUE."));
}

bool FrameworkToolKit::getRewriteStartupSchemaOnNodeReady ()
{
    bool rewriteStartupSchemaOnNodeReady = false;

    s_rewriteStartupSchemaOnNodeReadyMutex.lock ();
    rewriteStartupSchemaOnNodeReady = s_rewriteStartupSchemaOnNodeReady;
    s_rewriteStartupSchemaOnNodeReadyMutex.unlock ();

    return rewriteStartupSchemaOnNodeReady;
}

ResourceId FrameworkToolKit::deleteFilesOnAllNodes ( const vector<string> &filenames, const UI32 &transferFlag, const bool &disconnectFromFailedNode , const vector<LocationId> &locationIds)
{
    size_t numberOfFiles = filenames.size();

    ResourceId status = WAVE_MESSAGE_SUCCESS ;

    if ( 0 == numberOfFiles)
    {
        trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::deleteFilesOnAllNodes : Success since no files have been specified") );
        return ( status );
    }

    //delete files on HA peer. On pizzabox, OR with dual MM where connection to HA peer has failed, we won't enter here
    if (WAVE_MESSAGE_SUCCESS == isConnectedToHaPeerLocation())
    {
        LocationId  haPeerLocationId = FrameworkToolKit::getHaPeerLocationId ();

        if (0 == haPeerLocationId)
        {
            trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::deleteFilesOnAllNodes : FrameworkToolKit::getHaPeerLocationId returned [0]"));
            return (WAVE_MESSAGE_ERROR);
        }

        for ( size_t i = 0; i < numberOfFiles; i++ )
        {
            FileDeleteFileToHaPeerMessage *pMessage = new FileDeleteFileToHaPeerMessage ();
            prismAssert (NULL != pMessage, __FILE__, __LINE__);

            pMessage->setDestinationFileName (filenames[i]);

            status = WaveObjectManagerToolKit::sendSynchronously (pMessage, haPeerLocationId);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::deleteFilesOnAllNodes : Sending failed with status - ") + FrameworkToolKit::localize(status) );
                delete (pMessage) ;
                pMessage = NULL;
                return ( status );
            }
            else
            {
                status = pMessage->getCompletionStatus();
                if ( WAVE_MESSAGE_SUCCESS != status )
                {
                    // do not return on this failure
                    trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::deleteFilesOnAllNodes : Sending failed with Completion status - ") + FrameworkToolKit::localize(status) );
                }
            }

            delete (pMessage) ;
            pMessage = NULL;
        }

    }

    vector<LocationId> connectedLocationsVector;

    if (0 != locationIds.size())
    {
        connectedLocationsVector = locationIds;
    }
    else
    {
        FrameworkToolKit::getFullyConnectedLocations( connectedLocationsVector );
    }

    if ( 0 == connectedLocationsVector.size() )
    {
        trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::deleteFilesOnAllNodes : Success since no nodes are connected") );
        return ( status );
    }

    for ( size_t i = 0; i < numberOfFiles; i++ )
    {
        FileDeleteRequestMessage *pMessage = new FileDeleteRequestMessage(filenames[i], connectedLocationsVector );
        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        pMessage->setFileTransferFlag( transferFlag );
        status = WaveObjectManagerToolKit::sendSynchronously (pMessage);
        //If failure is because a node went off then we should continue. Need to handle this
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::deleteFilesOnAllNodes : Sending failed with status - ") + FrameworkToolKit::localize(status) );
            delete (pMessage) ;
            pMessage = NULL;
            return ( status );
        }
        else
        {
            status = pMessage->getCompletionStatus();
            if ( WAVE_MESSAGE_SUCCESS != status )
            {
                do
                {
                    if ( WAVE_FILE_INVALID_DESTINATION_NODE == status )
                    {
                        // A node has got disconnected and this will be a validation failure
                        // Remove the disconnected node and send again

                        delete (pMessage) ;
                        pMessage = NULL;
                        removeDisconnectedLocationFromVector ( connectedLocationsVector );

                        if ( 0 == connectedLocationsVector.size() )
                        {
                            trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::deleteFilesOnAllNodes : no nodes are connected after removal") );
                            return ( status );
                        }
                        trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::deleteFilesOnAllNodes : Filename - ") + filenames[i] + (", Failed with Completionstatus - ") + FrameworkToolKit::localize(status) + (", retry after removing that node from list. size = ") + connectedLocationsVector.size());

                        pMessage = new FileDeleteRequestMessage(filenames[i], connectedLocationsVector );
                        pMessage->setFileTransferFlag( transferFlag );

                        status = WaveObjectManagerToolKit::sendSynchronously (pMessage);
                        if (WAVE_MESSAGE_SUCCESS != status)
                        {
                            trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::deleteFilesOnAllNodes : Retry. Sending failed with status - ") + FrameworkToolKit::localize(status) );
                            delete (pMessage) ;
                            pMessage = NULL;
                            return ( status );
                        }
                        else
                        {
                            status = pMessage->getCompletionStatus();

                        }
                    }
                    else if ( WAVE_MESSAGE_SUCCESS != status )
                    {
                        trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::deleteFilesOnAllNodes : Filename - ") + filenames[i] + (", Failed with Completionstatus - ") + FrameworkToolKit::localize(status) );
                        vector<ResourceId> nodeStatus = pMessage->getResponseCodeList ();

                        if ( connectedLocationsVector.size() == nodeStatus.size() )
                        {
                            vector<LocationId> succeededLocations;
                            for ( UI32 j = 0; j < nodeStatus.size(); j++ )
                            {
                                if ( WAVE_MESSAGE_SUCCESS != nodeStatus[j] )
                                {
                                    trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::deleteFilesOnAllNodes : Failed for location = ") + connectedLocationsVector[j] + (", disconnect from that node"));
                                    if ( true == disconnectFromFailedNode )
                                    {
                                        (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (connectedLocationsVector[j]);
                                    }
                                }
                                else
                                {
                                    succeededLocations.push_back( connectedLocationsVector[j]);
                                }
                            }
                            connectedLocationsVector.clear();
                            connectedLocationsVector = succeededLocations;
                            succeededLocations.clear();
                        }
                        else
                        {
                            // For now we can continue with this error. Once we know what erros can fall here lets handle it
                            trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::deleteFilesOnAllNodes : Unknown error ") );
                        }
                        //continue with next file
                        break;
                    }
                    else
                    {
                        trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::deleteFilesOnAllNodes : Success after retry for Filename - ") + filenames[i] );
                        break ;
                    }

                }while (1);

                if ( 0 == connectedLocationsVector.size() )
                {
                    trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::deleteFilesOnAllNodes : Retry end. No nodes are connected after removal") );
                    delete (pMessage) ;
                    pMessage = NULL;
                    return ( status );
                }
            }
        }
        delete (pMessage) ;
        pMessage = NULL;
    }
    return (status);
}

ResourceId FrameworkToolKit::pushFilesToAllNodes ( const vector<string> &sourceFilenames, const vector<string> &destFilenames, const UI32 &transferFlag, const bool &disconnectFromFailedNode , const vector<LocationId> &locationIds)
{
    prismAssert ( sourceFilenames.size() == destFilenames.size(), __FILE__, __LINE__);

    size_t numberOfFiles = sourceFilenames.size();

    ResourceId status = WAVE_MESSAGE_SUCCESS ;

    if ( 0 == numberOfFiles)
    {
        trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::pushFilesToAllNodes : Success since no files have been specified") );
        return ( status );
    }

    //push files to HA peer. On pizzabox, OR with dual MM where connection to HA peer has failed, we won't enter here
    if (WAVE_MESSAGE_SUCCESS == isConnectedToHaPeerLocation())
    {
        for ( size_t i = 0; i < numberOfFiles; i++ )
        {
            FilePushFileToHaPeerMessage *pMessage = new FilePushFileToHaPeerMessage();
            prismAssert (NULL != pMessage, __FILE__, __LINE__);

            pMessage->setSourceFileName(sourceFilenames[i]);
            pMessage->setDestinationFileName(destFilenames[i]);

            status = WaveObjectManagerToolKit::sendSynchronously (pMessage);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::pushFilesToAllNodes : Sending failed with status - ") + FrameworkToolKit::localize(status) );
                delete (pMessage) ;
                pMessage = NULL;
                return ( status );
            }
            else
            {
                status = pMessage->getCompletionStatus();
                if ( WAVE_MESSAGE_SUCCESS != status )
                {
                    // do not return on this failure
                    trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::pushFilesToAllNodes : Sending failed with Completion status - ") + FrameworkToolKit::localize(status) );
                }
            }

            delete (pMessage) ;
            pMessage = NULL;
        }
    }

    vector<LocationId> connectedLocationsVector;

    if (0 != locationIds.size())
    {
        connectedLocationsVector = locationIds;
    }
    else
    {
        FrameworkToolKit::getFullyConnectedLocations( connectedLocationsVector );
    }

    if ( 0 == connectedLocationsVector.size() )
    {
        trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::pushFilesToAllNodes : Success since no nodes are connected") );
        return ( status );
    }

    for ( size_t i = 0; i < numberOfFiles; i++ )
    {
        FilePushFileMessage *pMessage = new FilePushFileMessage(sourceFilenames[i], destFilenames[i],  FrameworkToolKit::getThisLocationId(), connectedLocationsVector );
        pMessage->setFileTransferFlag( transferFlag );
        status = WaveObjectManagerToolKit::sendSynchronously (pMessage);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::pushFilesToAllNodes : Sending failed with status - ") + FrameworkToolKit::localize(status) );
            delete (pMessage) ;
            pMessage = NULL;
            return ( status );
        }
        else
        {
            status = pMessage->getCompletionStatus();
            if ( WAVE_MESSAGE_SUCCESS != status )
            {
                do
                {
                    if ( WAVE_FILE_INVALID_DESTINATION_NODE == status )
                    {
                        // A node has got disconnected and this will be a validation failure
                        // Remove the disconnected node and send again
                        delete (pMessage) ;
                        pMessage = NULL;
                        removeDisconnectedLocationFromVector ( connectedLocationsVector );

                        if ( 0 == connectedLocationsVector.size() )
                        {
                            trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::pushFilesToAllNodes : no nodes are connected after removal") );
                            return ( status );
                        }

                        trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::pushFilesToAllNodes : Filename - ") + sourceFilenames[i] + (", Failed with Completionstatus - ") + FrameworkToolKit::localize(status) + (", retry after removing that node from list. size = ") + connectedLocationsVector.size());
                        pMessage = new FilePushFileMessage(sourceFilenames[i], destFilenames[i],  FrameworkToolKit::getThisLocationId(), connectedLocationsVector );
                        pMessage->setFileTransferFlag( transferFlag );

                        status = WaveObjectManagerToolKit::sendSynchronously (pMessage);

                        if (WAVE_MESSAGE_SUCCESS != status)
                        {
                            trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::pushFilesToAllNodes : Retry. Sending failed with status - ") + FrameworkToolKit::localize(status) );
                            delete (pMessage) ;
                            pMessage = NULL;
                            return ( status );
                        }
                        else
                        {
                            status = pMessage->getCompletionStatus();

                        }
                    }
                    else if ( WAVE_MESSAGE_SUCCESS != status )
                    {
                        trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::pushFilesToAllNodes : Filename - ") + sourceFilenames[i] + (", Failed with Completionstatus - ") + FrameworkToolKit::localize(status) );
                        vector<ResourceId> nodeStatus = pMessage->getResponseCodeList ();

                        if ( connectedLocationsVector.size() == nodeStatus.size() )
                        {
                            vector<LocationId> succeededLocations;

                            for ( UI32 j = 0; j < nodeStatus.size(); j++ )
                            {
                                if ( WAVE_MESSAGE_SUCCESS != nodeStatus[j] )
                                {
                                    trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::pushFilesToAllNodes : Failed for location = ") + connectedLocationsVector[j] + (", disconnect from that node"));
                                    if ( true == disconnectFromFailedNode )
                                    {
                                        (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (connectedLocationsVector[j]);
                                    }
                                }
                                else
                                {
                                    succeededLocations.push_back( connectedLocationsVector[j]);
                                }
                            }
                            connectedLocationsVector.clear();
                            connectedLocationsVector = succeededLocations;
                            succeededLocations.clear();
                        }
                        else
                        {
                            // For now we can continue with this error. Once we know what erros can fall here lets handle it
                            trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::pushFilesToAllNodes : Unknown error ") );
                        }
                        //continue with next file
                        break;
                    }
                    else
                    {
                        trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::pushFilesToAllNodes : Success after retry for Filename - ") + sourceFilenames[i] );
                        break ;
                    }

                }while (1);

                if ( 0 == connectedLocationsVector.size() )
                {
                    trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::pushFilesToAllNodes : Retry end. No nodes are connected after removal") );
                    delete (pMessage) ;
                    pMessage = NULL;
                    return ( status );
                }

            }
        }

        delete (pMessage) ;
        pMessage = NULL;
    }

    return (status);
}

void FrameworkToolKit::removeDisconnectedLocationFromVector ( vector<LocationId> &connectedLocationsVector )
{
    for ( UI32 j = 0; j < connectedLocationsVector.size();  )
    {
        if ( false == isAConnectedLocation( connectedLocationsVector[j]) )
        {
            trace (TRACE_LEVEL_INFO, string ("FrameworkToolKit::removeDisconnectedLocationFromVector : removing location = ") + connectedLocationsVector[j] );
            connectedLocationsVector.erase ( connectedLocationsVector.begin() + j );
        }
        else
        {
            //Increment index only if its connected
            //IF disconnected the element is removed, continue with same index
            j++;
        }
    }
}

void FrameworkToolKit::getAllFilesInTheDirectory (const string &directoryPath, vector<string> &filePaths, const string &extensionFilter)
{
    filePaths.clear ();

    if ("" == directoryPath)
    {
        return;
    }

    DIR *pDirectory = opendir (directoryPath.c_str ());

    if (NULL != pDirectory)
    {
        struct dirent *pDirectoryEntry = (struct dirent *) malloc (sizeof (struct dirent));
        struct dirent *pResult         = NULL;

        prismAssert (NULL != pDirectoryEntry, __FILE__, __LINE__);

        while (0 == (readdir_r (pDirectory, pDirectoryEntry, &pResult)))
        {
            if (NULL == pResult)
            {
                break;
            }
            else
            {
                struct stat fileStat;
                string      filePathWithDirectory = directoryPath + string ("/") + string (pDirectoryEntry->d_name);

                if (-1 != (stat (filePathWithDirectory.c_str (), &fileStat)))
                {
                    if (S_ISREG (fileStat.st_mode))
                {
                    string filePath = pDirectoryEntry->d_name;

                        if ("" != extensionFilter)
                        {
                    if (true == (StringUtils::endsWith (filePath, extensionFilter)))
                    {
                                filePaths.push_back (filePathWithDirectory);
                            }
                        }
                        else
                        {
                            filePaths.push_back (filePathWithDirectory);
                        }
                    }
                }
            }
        }

        if (NULL != pDirectoryEntry)
        {
            free (pDirectoryEntry);
        }

        closedir (pDirectory);
    }
}

SI32 FrameworkToolKit::getCommandLineInterfaceReceiverPort ()
{
    return (s_commandLineInterfaceReceiverPort);
}

void FrameworkToolKit::setCommandLineInterfaceReceiverPort (const SI32 &commandLineInterfaceReceiverPort)
{
    s_commandLineInterfaceReceiverPort = commandLineInterfaceReceiverPort;
}

string FrameworkToolKit::getHostNameForThisMachine ()
{
    char hostName[128];

    int result = gethostname (hostName, 128);

    if (0 == result)
    {
        return (string (hostName));
    }
    else
    {
        return (string ("-UNKNWON-"));
    }
}


string FrameworkToolKit::getCurrentTtyName ()
{
    char   ttyName[128];
    string ttyNameString;

    SI32 result = ttyname_r (1, ttyName, 128);

    if (0 == result)
    {
        ttyNameString = ttyName;
    }

    return (ttyNameString);
}

void FrameworkToolKit::initializeEnvironment ()
{
    setenv ("LANG", "en_US.UTF-8", 1);
}

ResourceId FrameworkToolKit::isConnectedToHaPeerLocation ()
{
    ResourceId returnStatus = WAVE_MESSAGE_ERROR;

    LocationBase *pThisLocation = FrameworkToolKit::getPThisLocation ();
    prismAssert (NULL != pThisLocation, __FILE__, __LINE__);

    ClientStreamingSocket *pClientStreamingSocket = pThisLocation->getClientStreamingSocketForHaPeer ();

    if (NULL != pClientStreamingSocket)
    {
        returnStatus = WAVE_MESSAGE_SUCCESS;
    }

    return returnStatus;
}

LocationId FrameworkToolKit::getHaPeerLocationId ()
{
    LocationId haPeerLocationId = 1;

    return haPeerLocationId;
}

bool FrameworkToolKit::getIsLiveSyncEnabled ()
{
    PersistenceLocalObjectManager *pTemp = PersistenceLocalObjectManager::getInstance ();
    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    return (pTemp->getLiveSyncEnabled ());
}

void FrameworkToolKit::disableLiveSync ()
{
    bool flag = false;
    PersistenceLocalObjectManagerSetLiveSyncEnableMessage *pPersistenceLocalObjectManagerSetLiveSyncEnableMessage = new PersistenceLocalObjectManagerSetLiveSyncEnableMessage (flag);

    ResourceId status = WaveObjectManagerToolKit::sendOneWay (pPersistenceLocalObjectManagerSetLiveSyncEnableMessage);

    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_WARN, string("FrameworkToolKit::Failed to disable live sync : ") + FrameworkToolKit::localize (status));
    }

    return;
}

// This is only used to set syncstate to OUT_OF_SYNC from other plugin other than PrismFrameworkOM
void FrameworkToolKit::setSyncState (const UI32 &syncState)
{
    FrameworkObjectManagerSetSyncStateMessage *pFrameworkObjectManagerSetSyncStateMessage = new FrameworkObjectManagerSetSyncStateMessage (syncState);

    ResourceId status = WaveObjectManagerToolKit::sendOneWay (pFrameworkObjectManagerSetSyncStateMessage);

    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_WARN, string("FrameworkToolKit::setSyncState Failed to set sync state Status : ") + FrameworkToolKit::localize (status));
    }

    return;
}

UI32 FrameworkToolKit::getSyncState ()
{
    return (PrismFrameworkObjectManager::getInstance ()->getSyncState ());
}

void FrameworkToolKit::notifySyncUpdateFailure (const ResourceId &reason)
{
    if (FrameworkToolKit::getSyncFailureNotified () == false)
    {
         if (PrismFrameworkObjectManager::getInstance ()->notifySyncUpdateFailure(reason) == true)
         {
             FrameworkToolKit::setSyncFailureNotified (true);
             FrameworkToolKit::setSyncState(OUT_OF_SYNC);
         }
    }
}

// This is used in the cases to stop live sync after hitting error when it is previously enabled.
void FrameworkToolKit::notifySyncUpdateFailAndStopSync (const ResourceId &reason)
{
    if (FrameworkToolKit::getIsLiveSyncEnabled () == true)
    {
        FrameworkToolKit::disableLiveSync ();
    }

    FrameworkToolKit::notifySyncUpdateFailure (reason);
}

/* Function :
    getFirmwareDownloadCase : returns if the last reboot was -
                                0 : reboot case
                                1 : firmware upgrade case
                               -1 : firmware downgrade case
*/

SI32 FrameworkToolKit::getFirmwareDownloadCase ()
{
    return (s_fwdlCase);
}

void FrameworkToolKit::setFirmwareDownloadCase (const SI32 &fwdlcase)
{
    s_fwdlCase = fwdlcase;
}

ObjectId FrameworkToolKit::getObjectIdForPartition (const string &partitionName)
{
    prismAssert (false == partitionName.empty (), __FILE__, __LINE__);

    ObjectId partitionObjectId;

    MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage multiPartitionObjectManagerGetObjectIdForPartitionNameMessage (partitionName);

    ResourceId sendStatus = WaveObjectManagerToolKit::sendSynchronously (&multiPartitionObjectManagerGetObjectIdForPartitionNameMessage);

    if (WAVE_MESSAGE_SUCCESS != sendStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::getObjectIdForPartition : failed to send MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage for partitionName [") + partitionName + string ("], sendStatus [") + FrameworkToolKit::localizeToSourceCodeEnum (sendStatus) + string ("]"));
        return partitionObjectId;
    }

    ResourceId completionStatus = multiPartitionObjectManagerGetObjectIdForPartitionNameMessage.getCompletionStatus ();

    if (WAVE_MESSAGE_SUCCESS != completionStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("FrameworkToolKit::getObjectIdForPartition : message processing failed for MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage for partitionName [") + partitionName + string ("], completionStatus [") + FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
        return partitionObjectId;
    }

    partitionObjectId = multiPartitionObjectManagerGetObjectIdForPartitionNameMessage.getPartitionObjectId ();

    return partitionObjectId;
}

bool FrameworkToolKit::isConfigurationCompatibilityCheckRequired ()
{
    return (s_isConfigurationCompatibilityCheckRequiredForCluster);
}

void FrameworkToolKit::setConfigurationCompatibilityCheckRequired (const bool &checkRequired)
{
    s_isConfigurationCompatibilityCheckRequiredForCluster = checkRequired;
}

string FrameworkToolKit::getMessageVersionForHaPeer (const string &ipAddress, const SI32 &port)
{
    return (HaPeerMessageReceiverObjectManager::getMessageVersionForHaPeer (ipAddress, port));
}

UI8 FrameworkToolKit::getSerializationTypeForHaPeer (const string &ipAddress, const SI32 &port)
{
    return (HaPeerMessageReceiverObjectManager::getSerializationTypeForHaPeer (ipAddress, port));
}

ResourceId FrameworkToolKit::getHaPeerIpAddressAndPort (string& ip, SI32& port)
{
    ResourceId returnStatus = WAVE_MESSAGE_ERROR;

    LocationBase *pThisLocation = FrameworkToolKit::getPThisLocation ();
    prismAssert (NULL != pThisLocation, __FILE__, __LINE__);

    ClientStreamingSocket *pClientStreamingSocket = pThisLocation->getClientStreamingSocketForHaPeer ();

    if (NULL != pClientStreamingSocket)
    {
        ip      = pClientStreamingSocket->getHost ();
        port    = pClientStreamingSocket->getPort ();

        returnStatus = WAVE_MESSAGE_SUCCESS;
    }

    return returnStatus;
}

string FrameworkToolKit::getMessageVersionForRemoteNode (const string &ipAddress, const SI32 &port)
{
    return (InterLocationMessageReceiverObjectManager::getMessageVersionForRemoteLocation (ipAddress, port));
}

UI8 FrameworkToolKit::getSerializationTypeForRemoteNode (const string &ipAddress, const SI32 &port)
{
    return (InterLocationMessageReceiverObjectManager::getSerializationTypeForRemoteLocation (ipAddress, port));
}

bool FrameworkToolKit::getSyncFailureNotified ()
{
    bool syncFailureNotified;

    s_syncFailureNotifiedMutex.lock ();

    syncFailureNotified = s_syncFailureNotified;

    s_syncFailureNotifiedMutex.unlock ();

    return syncFailureNotified;
}

void FrameworkToolKit::setSyncFailureNotified (const bool &syncFailureNotified)
{
    s_syncFailureNotifiedMutex.lock ();

    s_syncFailureNotified = syncFailureNotified;

    s_syncFailureNotifiedMutex.unlock ();
}

ClientStreamingSocket* FrameworkToolKit::getClientStreamingSocketForRemoteLocation (LocationId locationId)
{
    PrismFrameworkObjectManager *pTemp = NULL;

    if (true == (isFrameworkReadyToBoot ()))
    {
        pTemp = PrismFrameworkObjectManager::getInstance ();
    }

    if (NULL != pTemp)
    {
        return (pTemp->getClientStreamingSocketForRemoteLocation (locationId));
    }

    return (NULL);
}

UI32 FrameworkToolKit::getHaId ()
{
    return (s_haId);
}

void FrameworkToolKit::setHaId (const UI32 &haId)
{
    s_haId = haId;
}

AttributeType FrameworkToolKit::getFieldTypeInATable (const string& tableName, const string& fieldName)
{
    OrmTable* table = (OrmRepository::getInstance ())->getTableByName (tableName);

    prismAssert (NULL != table, __FILE__, __LINE__);

    Attribute* pAttribute = table->getAttributeForFieldNameInManagedObject (fieldName);

    prismAssert (NULL != pAttribute, __FILE__, __LINE__);

    return (pAttribute->getAttributeType ());
}

string FrameworkToolKit::getSqlTypeForAFieldInATable (const string& tableName, const string& fieldName)
{
    OrmTable* pOrmTable = (OrmRepository::getInstance ())->getTableByName (tableName);

    prismAssert (NULL != pOrmTable, __FILE__, __LINE__);

    return (pOrmTable->getSqlTypeForAField (fieldName));

}
void FrameworkToolKit::setStartMode (const string &mode)
{
    ResourceId startMode = WAVE_NODE_START_AS_NORMAL;

    if ("normal" == mode)
    {
        startMode = WAVE_NODE_START_AS_NORMAL;
    }
    else if ("controller" == mode)
    {
        startMode = WAVE_NODE_START_AS_CONTROLLER;
    }
    else if ("client" == mode)
    {
        startMode = WAVE_NODE_START_AS_CLIENT;
    }

    s_waveStartModeMutex.lock ();

    s_waveStartMode = startMode;

    s_waveStartModeMutex.unlock ();
}

ResourceId FrameworkToolKit::getStartMode ()
{
    ResourceId startMode;

    s_waveStartModeMutex.lock ();

    startMode = s_waveStartMode;

    s_waveStartModeMutex.unlock ();

    return (startMode);
}

SI32 FrameworkToolKit::getMessageBrokerPort ()
{
    return (s_messageBrokerPort);
}

void FrameworkToolKit::setMessageBrokerPort (const SI32 &messageBrokerPort)
{
    s_messageBrokerPort = messageBrokerPort;
}

SI32 FrameworkToolKit::getMessageBrokerClientPort ()
{
    return (s_messageBrokerClientPort);
}

void FrameworkToolKit::setMessageBrokerClientPort (const SI32 &messageBrokerClientPort)
{
    s_messageBrokerClientPort = messageBrokerClientPort;
}

string FrameworkToolKit::getPrismConnectionPassPhraseForMessageBroker ()
{
    return (string ("PmrsiisrmPHello1PmrsiisrmPHello1"));
}

}

