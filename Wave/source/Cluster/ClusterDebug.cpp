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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/LocalClusterConfigObjectManager.h"
#include "Shell/ShellObjectManager.h"
#include "Shell/ShellDebug.h"
#include "ClusterDebug.h"
#include "App/AppDebug.h"
#include "Framework/Utils/TraceUtils.h"
#include "Cluster/ClusterMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"

namespace WaveNs
{

void createClusterReportResult (void * pContext, int status, int nNodes, NodeValidationResult_t secondaryNodes [])
{
    int    i;
    delete [] ((char *) pContext);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "Failed to create Cluster");
        return;
    }

    WaveNs::trace (TRACE_LEVEL_INFO, string ("createClusterReportResult: Number of Secondary nodes: ") +  nNodes);

    for (i=0; i< nNodes; i++)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("          Node: ") + secondaryNodes [i].nodeName + string (" Port: ") + secondaryNodes [i].nodePort + string (", Status: ") + secondaryNodes [i].nodeStatus);

        if (NULL != secondaryNodes [i].validationResultPtr)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("clusterReportValidationHandler Result[") + i + "] Node: " + secondaryNodes [i].nodeName
                    + " Port: " + secondaryNodes [i].nodePort
                            + " ResLen: " + secondaryNodes [i].validationResultLen
                            + " Data:" + (char *) (secondaryNodes [i].validationResultPtr));
            if (0 != memcmp ((char *) (secondaryNodes [i].validationResultPtr), "VALIDATION RESULTS + ANY OTHER INFO", secondaryNodes [i].validationResultLen))
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, "Got invalid Validation Results");
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_DEBUG, string ("Got Correct validation result: VALIDATION RESULTS + ANY OTHER INFO"));
            }
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("NULL pointers for Validation Results number ") + i);
        }
    }

}

void heartBeatLostCB (int failureReason, ipString failedNode, UI32 port)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "... Got Heart beat Loss Call Back ...");
    WaveNs::trace (TRACE_LEVEL_INFO, string(" Failed Node: ") + (char *) failedNode + " Port: " + port + ", Failed Reason: " + failureReason);

}


void memberJoinedHandler (ipString newNode, UI32 port)
{
    WaveNs::trace (TRACE_LEVEL_INFO, "memberJoinedHandler");
}

void  clusterValidationHandler (unsigned int validationDataLen, void *pValidationData, void *pContext)
{

    if ((NULL != pValidationData) && (0 != validationDataLen))
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string (" clusterValidationHandler: Len:") + validationDataLen + string (" ptr: ") + ((char *) (pValidationData)));
        if (0 != memcmp ((char *) (pValidationData), "Verification Data", validationDataLen))
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "Got invalid Validation data");
        }
    }

    char *result  =   new char[200];
    memset (result, 0, 200);
    memcpy (result, "VALIDATION RESULTS + ANY OTHER INFO", strlen ("VALIDATION RESULTS + ANY OTHER INFO") + 1);

    WaveNs::trace (TRACE_LEVEL_INFO, string ("validation Results: Len: ") + (strlen ("VALIDATION RESULTS + ANY OTHER INFO") + 1) + string (" Result: ") + ((char *) result));

    reportValidationResults (pContext, WAVE_MESSAGE_SUCCESS, strlen ("VALIDATION RESULTS + ANY OTHER INFO") + 1, result);
    delete [] result;
    return;
}

void clusterCollectValidationHandler (void *pContext)
{
//    cout << endl << " ##### Collect Validation Data" << endl;
    char     *pVerification   = (char *) new UI8[200];

    memcpy (pVerification, "Verification Data", strlen("Verification Data") + 1);
    WaveNs::trace (TRACE_LEVEL_INFO, string ("clusterCollectValidationHandler: Validation Data = ") + ((char *) pVerification));

    reportValidationDetails (pContext, WAVE_MESSAGE_SUCCESS, strlen("Verification Data") + 1, pVerification);
}

void clusterReportValidationHandler (unsigned int nNodes, NodeValidationResult_t validationResult [])
{
    UI32    i;

    WaveNs::trace (TRACE_LEVEL_INFO, string ("clusterReportValidationHandler: nNodes:") + nNodes );

    for (i = 0 ; i < nNodes; i++)
    {
        if (NULL != validationResult [i].validationResultPtr)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("clusterReportValidationHandler Result[") + i + "] Node: " + validationResult [i].nodeName
            + " Port: " + validationResult [i].nodePort
            + " ResLen: " + validationResult [i].validationResultLen
            + " Data:" + (char *) (validationResult [i].validationResultPtr));
            if (0 != memcmp ((char *) (validationResult [i].validationResultPtr), "VALIDATION RESULTS + ANY OTHER INFO", validationResult [i].validationResultLen))
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, "Got invalid Validation Results");
            }
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("NULL pointers for Validation Results number ") + i);
        }
    }

}

void addNodeResultCallBack (void *pContext, int status)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("addNodeCallBack: context: ") +  (unsigned long int) pContext + string (", status: ") + status);
    delete [] ((char *) pContext);
}

void joinNodeResultCallBack (void *pContext, int status)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("joinNodeCallBack: context: ") + (unsigned long int) pContext + string (", status: ") + status);
    delete [] ((char *) pContext);
}


void removeNodeResultCallBack (void *pContext, int status)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("removeNodeResultCallBack: context: ") + (unsigned long int) pContext + string (", status: ") + status);
}

void removeClusterResultCallBack (void *pContext, int status)
{
    WaveNs::trace (TRACE_LEVEL_INFO, string ("removeClusterResultCallBack: context: ") + (unsigned long int) pContext + string (", status: ") +status);
}



ClusterDebug::ClusterDebug  ()
{
}

ClusterDebug::~ClusterDebug  ()
{
}

void ClusterDebug::initialize ()
{
    //Debug CLIs
    addDebugFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteShowStatDebug),         "ss"    );
    addDebugFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteRegisterCallBackDebug), "regcb" );
    addDebugFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteConfigHeartBeatDebug),  "hbconfig" );
    addDebugFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteGetRoleDebug),          "getrole" );
    addDebugFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteGetClusterConfigDebug), "gethacfg" );

    //Cluster CLIs
    addClusterFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteCreateClusterDebug),    "cc"    );
    addClusterFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteAddNodeDebug),          "addnode" );
    addClusterFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteRemoveNodeDebug),       "rmnode" );
    addClusterFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteJoinNodeDebug),         "joinnode" );
    addClusterFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteRemoveClusterDebug),    "rmcluster" );
    addClusterFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteGetClusterInfoDebug), "getClusterDbgInfo");
    addClusterFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteMakeThisNodePrincipal), "setThisNodeAsPrincipal");
    addClusterFunction ((ShellCmdFunction) (&ClusterDebug::shellExecuteNodifySecondaryNodeFailure), "secFailed");
}


UI32 ClusterDebug::shellExecuteCreateClusterDebug (UI32 argc, vector<string> argv)
{
    node_t    nodes [32];
    UI32      i;
    UI32      nodeNum         = 0;
    UI32      nSecondaryNodes = 1;
    char     *pVerification;

    if ((argc < 3) || (1 != argc % 2))
    {
        cout << "cc secondaryIpAddr1 secondaryPort1 secondaryIpAddr2 secondaryPort2..." << endl;
        return SHELL_OK;
    }

    nSecondaryNodes =   (argc - 1)/2;
    i = 1;
    while (i < argc)
    {
        memcpy (&nodes[nodeNum].nodeName, argv [i].c_str (), strlen(argv [i].c_str ()) + 1);
        i++;
        nodes[nodeNum++].nodePort = atoi (argv [i].c_str ());
        i++;
    }


    pVerification   = (char *) new UI8[200];
    memcpy (pVerification, "Verification Data", strlen("Verification Data") + 1);

#if 0
    cout << "nSecondaryNodes:" << nSecondaryNodes << endl;

    for (i = 0; i < nSecondaryNodes; i++)
    {
        cout << "node Name:" << nodes [i].nodeName << "  Node port: "<< nodes [i].nodePort << endl;
    }
#endif
    createCluster (-1, nSecondaryNodes, nodes, createClusterReportResult, pVerification);

    return SHELL_OK;
}


UI32 ClusterDebug::shellExecuteShowStatDebug (UI32 argc, vector<string> argv)
{

    (LocalClusterConfigObjectManager::getInstance ())->showStatistic();
    //(CentralClusterConfigObjectManager::getInstance ())->m_cluster.showStatistic();
    return SHELL_OK;
}



UI32 ClusterDebug::shellExecuteAddNodeDebug (UI32 argc, vector<string> argv)
{
    char                               tempBuffer[128];
    string                             nodeName;
    SI32                               nodePort;
    UI32                               i;
    UI32                               noNewNode = 0;
    ClusterObjectManagerAddNodeMessage message;
    ResourceId                         status = WAVE_MESSAGE_SUCCESS;

    
    if (argc < 3)
    {
        cout << "addnode nodeIpAddr nodePort" << endl;
        return SHELL_OK;
    }

    if (((argc + 1) % 2) != 0)
    {
        cout << "Please enter correct arguments" << endl;
        return SHELL_OK;
    }

    i = 1;

    while (i < argc)
    {
        memcpy (tempBuffer, argv [i].c_str (), strlen(argv [i].c_str ()) + 1);
        nodeName = tempBuffer; 
        i++;
        nodePort = atoi (argv [i].c_str ());
        i++;
        noNewNode++;
        message.setSecondaryNodeNameAndPort(nodeName, nodePort);
    }
  
    message.setPrimaryNodeName (FrameworkToolKit::getThisLocationIpAddress ());
    message.setNSecondaryNodes(noNewNode);

    status = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "Could not add node to Wave Cluster.  Status : " + FrameworkToolKit::localize (status));
    }

    return SHELL_OK;
}

//
//Name:
//shellExecuteJoinNodeDebug
//
//Description: 
//Posts a synchronous request for the prinicpal
//node to start the rejoin of the specified nodes
//Input/s:
//argc: argument count should be and odd number >=3
//argv: a vector with string 'getClusterDbgInfo'
//
//Output/s:
//None
//
//Returns:
//SHELL_OK: always returns success
//
UI32 ClusterDebug::shellExecuteJoinNodeDebug (UI32 argc, vector<string> argv)
{

    if (argc < 3)
    {
        cout << "rejoinnode nodeIpAddr nodePort" << endl;
        return SHELL_OK;
    }

    if (((argc + 1) % 2) != 0)
    {
        cout << "Please enter correct arguments" << endl;
        return SHELL_OK;
    }

    UI32 i = 1;
    UI32 noNewNode = 0;
    ClusterObjectManagerRejoinNodeMessage message;

    //In the loop read IP and port from the vector of each 
    //rejoining node
    while (i < argc)
    {
        char   tempBuffer[128];
        memcpy (tempBuffer, argv [i].c_str (), strlen(argv [i].c_str ()) + 1);
        string nodeIpAddress = tempBuffer; 
        i++;
        UI32 nodePort = atoi (argv [i].c_str ());
        i++;
        noNewNode++;
        message.setNodeIpAddress(nodeIpAddress);
    message.setNodePort(nodePort);
    }

    //Dispatched to the FW to forward it to the Global Cluster Service
    ResourceId status = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "Could not join node to Wave Cluster.  Status : " + FrameworkToolKit::localize (status));
    }

    WaveNs::trace (TRACE_LEVEL_INFO,  string ("shellExecuteJoinNodeDebug: ") + status);
    return SHELL_OK;
}

UI32 ClusterDebug::shellExecuteRemoveNodeDebug (UI32 argc, vector<string> argv)
{
    char                               tempBuffer[128];
    string                             nodeName;
    SI32                               nodePort;
    UI32                               i;
    UI32                               noNewNode = 0;
    ClusterObjectManagerDeleteNodeMessage message;
    ResourceId                         status = WAVE_MESSAGE_SUCCESS;

    if (argc < 3)
    {
        cout << "rmnode nodeIpAddr nodePort" << endl;
        return SHELL_OK;
    }

    if (((argc + 1) % 2) != 0)
    {
        cout << "Please enter correct arguments" << endl;
        return SHELL_OK;
    }

    i = 1;

    while (i < argc)
    {
        memcpy (tempBuffer, argv [i].c_str (), strlen(argv [i].c_str ()) + 1);
        nodeName = tempBuffer;
        i++;
        nodePort = atoi (argv [i].c_str ());
        i++;
        noNewNode++;
        message.setSecondaryNodeNameAndPort(nodeName, nodePort);
    }

    message.setNNodesToDelete(noNewNode);

    status = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "Could not Delete node to Wave Cluster.  Status : " + FrameworkToolKit::localize (status));
    }

    return SHELL_OK;

}

UI32 ClusterDebug::shellExecuteRemoveClusterDebug (UI32 argc, vector<string> argv)
{
    ClusterObjectManagerDeleteClusterMessage message;

    ResourceId                               status   = ShellPrism::shellSendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "Could not delete Wave Cluster.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, "Successfully deleted Wave Cluster.");
    }

    return SHELL_OK;
}

UI32 ClusterDebug::shellExecuteConfigHeartBeatDebug (UI32 argc, vector<string> argv)
{
    int status;
    if (argc < 3)
    {
        cout << "hbconfig heartBeatInterval nLostHeartBeatsToNotify" << endl;
        return SHELL_OK;
    }

    status =  setHeartBeatConfig (atoi (argv[1].c_str ()), atoi (argv[2].c_str ()));
    WaveNs::trace (TRACE_LEVEL_INFO, string ("shellExecuteConfigHeartBeatDebug: ") + status);
    return SHELL_OK;
}

UI32 ClusterDebug::shellExecuteGetRoleDebug (UI32 argc, vector<string> argv)
{
    int     status;
    int     nodeRole;

    status =  getSwitchHaRole (&nodeRole);

    cout << "shellExecuteGetRoleDebug: " << status << ", nodeRole:" << nodeRole << "(";
    if (CLUSTER_DMM_STAND_ALONE_NODE == nodeRole)
    {
        cout << "STAND_ALONE_NODE" ;
    }

    if (CLUSTER_DMM_PRIMARY_NODE == nodeRole)
    {
        cout << "PRIMARY_NODE" ;
    }

    if (CLUSTER_DMM_SECONDARY_NODE == nodeRole)
    {
        cout << "SECONDARY_NODE" ;
    }
    cout << ")" << endl;
    return SHELL_OK;
}


UI32 ClusterDebug::shellExecuteGetClusterConfigDebug (UI32 argc, vector<string> argv)
{
    int             index = 0;
    int             status = 0;
    haNodeStatus_t  primaryNode ;
    int             nSecondaryNodes = 0;
    haNodeStatus_t  secondaryNodes [32];

    memset(&primaryNode, 0, sizeof(haNodeStatus_t));
    memset(secondaryNodes, 0, sizeof(haNodeStatus_t)*32);

    status =  getSwitchHaConfig (&primaryNode, &nSecondaryNodes, (haNodeStatus_t *)  &secondaryNodes);

    WaveNs::trace (TRACE_LEVEL_INFO, string ("shellExecuteGetClusterConfigDebug: ") + status);
    cout << "    primaryNode: " << primaryNode.nodeName << " Port: " << primaryNode.nodePort << ", status: ";
    if (APP_CLUSTER_NODE_FAILED == primaryNode.nodeStatus)
    {
        cout << "NODE_FAILED";
    }
    else
    {
        cout << "NODE_ACTIVE";
    }

    cout << ", nSecondaryNodes: " << nSecondaryNodes << endl;

    for (index = 0 ; index < nSecondaryNodes; index++)
    {
        cout << "    secondaryNode: " << secondaryNodes[index].nodeName << " Port: " << secondaryNodes[index].nodePort <<", status: ";

        if (APP_CLUSTER_NODE_FAILED == secondaryNodes[index].nodeStatus)
        {
            cout << "NODE_FAILED" << endl;
        }
        else
        {
            cout << "NODE_ACTIVE" << endl;
        }

    }


    return SHELL_OK;
}



//
//Name:
//shellExecuteGetClusterInfoDebug
//
//Description: 
//Posts a synchronous request for the prinicpal
//node to obtain primary and secondary node 
// information
//Input/s:
//argc: argument count should be 1 in this case
// argv: a vector with string 'getClusterDbgInfo'
//
//Output/s:
//None
//
//Returns:
//SHELL_OK: always returns success
//

UI32 ClusterDebug::shellExecuteGetClusterInfoDebug(UI32 argc, vector<string> argv)
{



  //Check if the number of arguments are correct
  if(argc!=1) {
      cout << "getClusterDbgInfo" << endl ;
      return SHELL_OK;
  }

  //Send the message synchronously
  ClusterConfigObjectManagerGetClusterInfoMessage msg;
  WaveMessageStatus status = ShellPrism::shellSendSynchronously(&msg);

  //Formatted output if Success
  if(WAVE_MESSAGE_SUCCESS == status) 
    {
       cout <<"Successful response Received:"<< endl ;
      if(0==msg.isClusterCreated())
      {
          cout <<"Cluster Not Created: "<< endl;

      } 
      else

      {

          //Output Principal Node Information
          cout.width(30);cout << left << "Principal Node" << endl;
          cout.width(30);cout << left << "==============" << endl;
          cout.width(30);cout << left << "IP Address";cout << ": " << msg.getPrimaryNodeIpAddress() << endl;
          cout.width(30);cout << left << "Port";cout << ": " << msg.getPrimaryNodePort() << endl;
          cout.width(30);cout << left << "LocationId";cout << ": "<< msg.getPrimaryNodeLocationId() << endl;

          ResourceId nodeStatus = msg.getPrimaryNodeGenericStatus();
          cout.width(30);cout << left << "GenericStatus";cout << ": "<< FrameworkToolKit::localize(nodeStatus)  << endl;

          nodeStatus = msg.getPrimaryNodeSpecificStatus();     
          cout.width(30);cout << left << "SpecificStatus";cout << ": "<< FrameworkToolKit::localize (nodeStatus) << endl;
          UI32 numberOfSecondaryNodes = msg.getNSecondaryNodes(); 
          cout.width(30);cout << left << "Number of Secondary Nodes";cout << ": "<<numberOfSecondaryNodes << endl << endl;


          //Output for Secondary Node Information
          cout.width(30);cout << left << "Secondary Nodes" << endl;
          cout.width(30);cout << left << "===============" << endl;
          cout.width(15); cout << left << "IPAddress";
          cout.width(10); cout << left << "Port";
          cout.width(15); cout << left << "LocationId";
          cout.width(50); cout << left << "GenericStatus";
          cout.width(50); cout << left << "SpecificStatus";

          cout<<endl;
          for(UI32 i=0;i<numberOfSecondaryNodes;++i) 
          {
              string secondaryNodeIpAddress;
              UI32 secondaryNodePort = 0;
              LocationId secondaryNodeLocationId = 0;
              ResourceId secondaryNodeGenericStatus = 0;
              ResourceId secondaryNodeSpecificStatus = 0;
              msg.getSecondaryNodeDebugInfo(i,
                                            secondaryNodeIpAddress,
                                            secondaryNodePort,
                                            secondaryNodeLocationId,
                                            secondaryNodeGenericStatus,
                                            secondaryNodeSpecificStatus);

              cout.width(15); cout << left << secondaryNodeIpAddress;
              cout.width(10); cout << left << secondaryNodePort;
              cout.width(15); cout << left << secondaryNodeLocationId;
              cout.width(50); cout << left << FrameworkToolKit::localize(secondaryNodeGenericStatus);
              cout.width(50); cout << left << FrameworkToolKit::localize(secondaryNodeSpecificStatus);
              cout << endl;
         }
      }

  }
  else 
  {
      cout << " Get Cluster Info failure" << endl;
  }

  return SHELL_OK;
}

UI32 ClusterDebug::shellExecuteMakeThisNodePrincipal(UI32 argc, vector<string> argv)
{
  if(argc!=4) {
      cout<<"setThisNodeAsPrincipal <failedPrincipalIPAddrss> <failedPrincipalPort> <failedPrincipalLocationId>"<< endl;
      return SHELL_OK;
  }

  string failedIpAddress = argv[1];
  UI32 failedPort = atoi(argv[2].c_str());
  LocationId failedLocation = atoi(argv[3].c_str());

  FrameworkObjectManagerNewPrincipalEstablishedMessage message(failedIpAddress, failedPort, failedLocation);

  WaveMessageStatus status = ShellPrism::shellSendSynchronously(&message);

  if(status!= WAVE_MESSAGE_SUCCESS) 
  {
      cout<<"Failure in setting node to principal"<<endl;
  }
  else {
     ResourceId  processingStatus = message.getCompletionStatus();

     if(processingStatus != WAVE_MESSAGE_SUCCESS) 
     {
        cout<< "Failure in Processing the message" <<endl;
     }
  }

  return SHELL_OK;

}

UI32 ClusterDebug::shellExecuteNodifySecondaryNodeFailure(UI32 argc, vector<string> argv)
{
  if(argc!=4) {
      cout<<"secFailed <failedIPAddrss> <failedPort> <failedLocationId>"<< endl;
      return SHELL_OK;
  }

  string failedIpAddress = argv[1];
  UI32 failedPort = atoi(argv[2].c_str());
  LocationId failedLocation = atoi(argv[3].c_str());

  FrameworkObjectManagerSecondaryNodeFailureNotificationMessage message(failedIpAddress, failedPort, failedLocation);

  WaveMessageStatus status = ShellPrism::shellSendSynchronously(&message);

  if(status!= WAVE_MESSAGE_SUCCESS) 
  {
      cout<<"Failure in setting node to principal"<<endl;
  }
  else {
     ResourceId  processingStatus = message.getCompletionStatus();

     if(processingStatus != WAVE_MESSAGE_SUCCESS) 
     {
        cout<< "Failure in Processing the message" <<endl;
     }
  }

  return SHELL_OK;

}

UI32 ClusterDebug::shellExecuteRegisterCallBackDebug (UI32 argc, vector<string> argv)
{
    AppDebug::shellExecuteRegisterCallBackDebug (argc, argv);
    registerHeartbeatLossHandler            (0, &heartBeatLostCB);
    registerNodeJoinedHandler               (0, &memberJoinedHandler);
    registerClusterValidationHandler        (0, &clusterValidationHandler);
    registerClusterCollectValidationHandler (0, &clusterCollectValidationHandler);
    registerClusterReportValidationHandler  (0, &clusterReportValidationHandler);

    registerHeartbeatLossHandler            (1, &heartBeatLostCB);
    registerNodeJoinedHandler               (1, &memberJoinedHandler);
    registerClusterValidationHandler        (1, &clusterValidationHandler);
    registerClusterCollectValidationHandler (1, &clusterCollectValidationHandler);
    registerClusterReportValidationHandler  (1, &clusterReportValidationHandler);
    return SHELL_OK;
}



}
