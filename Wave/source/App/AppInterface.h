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

#ifndef APPINTERFACE_H
#define APPINTERFACE_H
#include "WaveCStatus.h"

#define                             MAX_IP_STRING_LENGTH        32
typedef char                        ipString  [MAX_IP_STRING_LENGTH];

typedef struct
{
    /* The length of the command sent. */
    unsigned int    cmdLen;
    /* Address of the data sent. */
    void           *cmdPtr;
} commandData_t;


typedef struct
{
    /* If address of the node */
    ipString        nodeName;
    unsigned int    nodePort;
    /* Command Status */
    unsigned int    status;

    unsigned int     nCommands;
    commandData_t   *commandData;

    unsigned int     nResults;
    commandData_t   *resultData;
} remoteCmdResult_t;

typedef struct
{
    ipString        nodeName;
    unsigned int    nodePort;
    int             nodeStatus;
} haNodeStatus_t;

typedef struct
{
    ipString        nodeName;
    unsigned int    nodePort;
    int             nodeStatus;
    void           *validationResultPtr;
    unsigned int    validationResultLen;
} NodeValidationResult_t;

typedef struct
{
    ipString        nodeName;
    unsigned int    nodePort;

} node_t;

typedef enum
{
    APP_CLUSTER_HEARTBEAT_LOST  = 1,
    APP_CLUSTER_CLUSTER_DELETED
} heartbeatFailure;

/* Node participation state. */
typedef enum
{
    APP_CLUSTER_NODE_ACTIVE         = 0,
    APP_CLUSTER_NODE_FAILED

} AppClusterNodeStatus;

/* Node HA state.*/
typedef enum
{
    CLUSTER_DMM_STAND_ALONE_NODE = 0,
    CLUSTER_DMM_PRIMARY_NODE,
    CLUSTER_DMM_SECONDARY_NODE
} AppClusterRole;

typedef int  (*applicationMainFunction)           (int argc, char *argv[], int applicationId);
typedef void (*heartBeatCallBack_t)               (int failureReason, ipString failedNode, unsigned int port);
typedef void (*memberJoinedCallBack_t)            (ipString newNode, unsigned int port);
typedef void (*duplicateCmdHandler_t)             (void *pContext, unsigned int cmdLen, void *command, unsigned int cmdLen2, void *command2);
typedef void (*duplicateCmdsHandler_t)            (void *pContext, unsigned int nCommands, commandData_t commandList []);
typedef void (*duplicateCmdResultCallBack_t)      (int status, void * pContext, int nNodes, remoteCmdResult_t nodeResults[]);

typedef void (*clusterCollectValidationHandler_t) (void *pContext);
typedef void (*clusterValidationHandler_t)        (unsigned int validationDataLen, void *pValidationData, void *pContext);
typedef void (*clusterReportValidationHandler_t)  (unsigned int nNodes, NodeValidationResult_t validationResult []);

typedef void (*createClusterCallBack_t) (void *pContext, int status, int nNodes, NodeValidationResult_t secondaryNodes []);
typedef void (*addNodeCallBack_t)       (void *pContext, int status);
typedef void (*joinNodeCallBack_t)      (void *pContext, int status);
typedef void (*removeNodeCallBack_t)    (void *pContext, int status);
typedef void (*removeClusterCallBack_t) (void *pContext, int status);

/* typedef void (*getRoleResultCallBack_t) (void *pContext, int status, int role); */

#ifdef __cplusplus
extern "C"
{
#endif
    /*
        Create a cluster with n nodes. The switch where this function is executed will become the primary node.
        This is async function. createClusterResultCB will be called to report creation status.

        Parameters:
            nSecondaryNodes:        number of  secondary nodes in the cluster.
            secondaryNodes:         array of the IP addresses of the secondary nodes in string format.
            createClusterResultCB:  Callback function to report creation status.
                typedef void (*createClusterCallBack_t) (void *pContext, int status, int nNodes, haNodeStatus_t secondaryNodes []);
                    pContext:       user context provided to createClusterWithCallBack.
                    Status:         WAVE_MESSAGE_SUCCESS if cluster iscreated successfully. Otherwise appropriate error code.
                    nNodes:         Number of node that supposed to b ein the cluster.
                    secondaryNodes: list of all the nodes that supposed to be added to the cluster and the status for each of them.
                                    Only node with status WAVE_MESSAGE_SUCCESS are added to the cluster.

            pContext:               User context. will be provided to the user in the callback function.
            pValidationData:        Address of data to be trnasfer to the secondary node to be used to
                                    validate version and the compatability between the node in the cluster.
                                    This data will be provided to validation function that should be registered
                                    before creating the cluster using "registerClusterValidationHandler".
                                    If no validation function is registered in the secondary node create
                                    cluster will fail
            validationDataLen:      Length of the validation data.

        Returns:
            WAVE_MESSAGE_SUCCESS: cluster created successfully. Otherwise appropriate error code.
            APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS:
            APP_CLUSTER_ALREADY_CREATED:
            APP_CLUSTER_ALREADY_PART_OF_ANOTHER_CLUSTER:
            APP_CLUSTER_INVALID_SECONDARY_ADDRESS:


    */
    int  createClusterWithCallBack  (int appId, int nSecondaryNodes, ipString secondaryNodes [], createClusterCallBack_t  createClusterResultCB, void *pContext);
    int createCluster               (int appId, int nSecondaryNodes, node_t secondaryNodes [], createClusterCallBack_t  createClusterResultCB, void *pContext);

    /*
        Add node to an already created Cluster. This function should be called from the primay switch only.
        This is async function. addNodeCallBack_t will be called to report the addation status.

        Parameters:

            node:                   The IP addresses of the node to be added to the cluster in string format.
            addNodeResultCB:        Call back function to report addation status.
            pContext:               User context. will be provided to the user in the callback function.
            pValidationData:        Address of data to be trnasfer to the secondary node to be used to
                                    validate version and the compatability between the node in the cluster.
                                    This data will be provided to validation function that should be registered
                                    before creating the cluster using "registerClusterValidationHandler".
                                    If no validation function is registered in the secondary node add
                                    command will fail.
            validationDataLen:      Length of the validation data.

        Returns:
            WAVE_MESSAGE_SUCCESS: node added successfully. Otherwise appropriate error code.
            APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS:
            APP_CLUSTER_NOT_PRIMARY_NODE:
            APP_CLUSTER_NO_CLUSTER_CREATED:
            APP_CLUSTER_NODE_ALREADY_IN_CLUSTER:

    */

    int  addNodeToClusterWithCallBack    (int appId, ipString ipAddr, addNodeCallBack_t  addNodeResultCB, void *pContext, void *pValidationData, unsigned int validationDataLen);
    int  addNodeToCluster                (int appId, node_t node, addNodeCallBack_t  addNodeResultCB, void *pContext, void *pValidationData, unsigned int validationDataLen);
    /*
    Get a node to join a Cluster that it was part of it before. This function should be called from the primay switch only.
    This is async function. joinNodeCallBack_t will be called to report the status.

    Parameters:

        node:                   The IP addresses of the node to join the cluster (in string format).
        addNodeResultCB:        Call back function to report the status.
        pContext:               User context. will be provided to the user in the callback function.
        pValidationData:        Address of data to be trnasfer to the secondary node to be used to
                                validate version and the compatability between the node in the cluster.
                                This data will be provided to validation function that should be registered
                                before creating the cluster using "registerClusterValidationHandler".
                                If no validation function is registered in the secondary node jon command
                                will fail.
        validationDataLen:      Length of the validation data.

    Returns:
            WAVE_MESSAGE_SUCCESS: Node joined cluster successfully. Otherwise appropriate error code.
            APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS:
            APP_CLUSTER_NOT_PRIMARY_NODE:
            APP_CLUSTER_NO_CLUSTER_CREATED:
            APP_CLUSTER_NODE_ALREADY_IN_CLUSTER:

    */
    int  joinNodeToClusterWithCallBack               (int appId, node_t node, joinNodeCallBack_t  joinNodeResultCB, void *pContext, void *pValidationData, unsigned int validationDataLen);
    /*
        Remove node from a Cluster. This function should be called from the primay switch only.
        This is async function. removeClusterResultCB will be called to report the remove status.

        Parameters:

        node:                   The IP addresses of the node to be removed from the cluster in string format.
        removeClusterResultCB:  Callback function to report remove status.
        pContext:               User context. will be provided to the user in the callback function.

        Returns:
            WAVE_MESSAGE_SUCCESS: cluster created successfully. Otherwise appropriate error code.
            APP_CLUSTER_MANAGMENT_OPERATION_IN_PROGRESS:
            APP_CLUSTER_NOT_PRIMARY_NODE:
            APP_CLUSTER_NO_CLUSTER_CREATED:
            APP_CLUSTER_CAN_NOT_DEL_PRIMAY_NODE:
            APP_CLUSTER_NODE_NOT_IN_CLUSTER:

    */

    int  removeNodeFromClusterWithCallBack           (int appId, ipString node, removeNodeCallBack_t  removeNodeResultCB, void *pContext);
    int  removeNodeFromCluster                       (int appId, node_t node, removeNodeCallBack_t  removeNodeResultCB, void *pContext);

    /*
        Remove Cluster. This function should be called from the primay switch only.
        This is async function. removeClusterResultCB will be called to report the remove status.

        Parameters:

        removeClusterResultCB:  Callback function to report remove status.
        pContext:               User context. will be provided to the user in the callback function.

        Returns:
            WAVE_MESSAGE_SUCCESS: cluster created successfully. Otherwise appropriate error code.

    */

    int  removeClusterWithCallBack                   (int appId, removeClusterCallBack_t  removeClusterResultCB, void *pContext);

    /*
        This fucntion duplicates command to all other nodes in the cluster. This function can be called from
        the primay or secondary nodes. This is an async function. duplicateCmdResultCB will be called to
        report the status.
        Data sent between switches should not have any pointers. command should point to the data to be sent

        If you have to have pointer in the data only one pointer can be supported.
        you can send the data using command and send the pointer data using command2 and
        reconstruct your data on the other node yourself.
        command and command2 should be allocated and freed by the application when the are needed.

        Parameters:
            cmdLen:                The length of the command sent
            command:               Address of the data sent.
            cmdLen2:               Length of another command in the same request if needed otherwise
                                   it should be set to 0.
            command2:              Address if the of the other command if needed otherwise it should be set to NULL.
            duplicateCmdResultCB:  Callback function to report command result.
            typedef void (*duplicateCmdResultCallBack_t)    (int status, void * pContext, int nNodes, remoteCmdResult_t nodeResults[]);
                status:      Status for duplicating the command.
                pContext:    User context provided in duplicateCmdToClusterNodesWithCallBack.
                nNodes:      Number of nodes the commands got duplicated to.
                nodeResults: The result of duplicate command from all the nodes and the data sent and the result.
                             If the send data got changed this struct will have the modified data.

            pContext:              User context. will be provided to the user in the callback function.

        Returns:
            WAVE_MESSAGE_SUCCESS: cluster created successfully. Otherwise appropriate error code.

    */

    int  duplicateCmdsToClusterNodesWithCallBack (int appId, unsigned int nCommands, commandData_t commandList [], duplicateCmdResultCallBack_t duplicateCmdResultCB, void *pContext);
//    int  duplicateCmdToClusterNodesWithCallBack      (unsigned int appId, int cmdLen, void *command,  unsigned int cmdLen2, void *command2, duplicateCmdResultCallBack_t  duplicateCmdResultCB, void *pContext);


    /*
        This function can be used to send async results to another node. This function can be called
        from the primay or secondary nodes. This is an async function.

        Parameters:
            resultLen:  The length of the result to send back
            resultPtr:  Address of the result data to sent back.
            pContext:   This should be the same context provided in duplciate command callback function.
                        This is the function registered using registerDuplicateCmdHandler.

        Returns:
            WAVE_MESSAGE_SUCCESS: command sent successfully. Otherwise appropriate error code is returned.

    */

    int sendRemoteCommandsReply (int appId, void *pContext, unsigned int nResults, commandData_t resultList []);

//    int sendRemoteCommandReply (int appId, void *pContext, unsigned int resultLen, void *resultPtr);
    /*
        This function register a callback function that will be called if more than n heartbeats are lost.
        n can be modified using setHeartBeatConfig.
        For primary node, this function is called if the heartbeats to any seconday node are lost.
        For secondary node this function is called if the heartbeats to the primary node are lost.

        Parameters:
            heartBeatCB : Callback to report heartbeat failure.
            void (*heartBeatCallBack_t)             (int failureReason, ipString failedNode);
                failureReason:      Reason of failure.
                failedNode:         IP address of the failed node.

        Returns:
            WAVE_MESSAGE_SUCCESS: Duplicate command done successfully.
            APP_CLUSTER_NO_DUPLICATE_HANDLER_REGISTERED:.No Duplicate function registered.
    */

    int registerHeartbeatLossHandler                (int appId, heartBeatCallBack_t heartBeatLossCallBack);
    //int  registerHeartBeatCallBack                   (heartBeatCallBack_t    heartBeatCB);

    /*
        This function register a callback function that will be called if a new node automatically joined a cluster.
        automatic cluster join is not supported yet.

        Parameters:
            memberJoinedCB : Callback to report node joining a cluster.
            void (*memberJoinedCallBack_t)          (ipString newNode)
                newNode: Joined node Ip address.

        Returns:
            WAVE_MESSAGE_SUCCESS: Registeration successed.
    */

    int registerNodeJoinedHandler                   (int appId, memberJoinedCallBack_t nodeJoinedCallBack);
//    int  registerMemberJoinedCallBack                (memberJoinedCallBack_t memberJoinedCB);

    /*
        This function register a callback function that will be called to handle any command duplicated from
        another node in the cluster.
        If this function is not register all duplicateCmdToClusterNodesWithCallBack will fail.

        Parameters:
            duplicateCmdHandler: duplicate command handling function to process any duplicate command
                                  received from another node in the cluster.
            void (*duplicateCmdHandler_t)           (unsigned int cmdLen, void *command, unsigned int cmdLen2, void *command2, unsigned int *resultLen, void **result)
            calling to duplicateCmdHandler_t is sync call. The result pointer should be allocated by
            duplicateCmdHandler_t if it is required to send result back. prism will free this memory if it is not NULL


        Returns:
            WAVE_MESSAGE_SUCCESS: Registeration successed.
    */
    int  registerDuplicateCmdsHandler                (int appId, duplicateCmdsHandler_t  duplicateCmdHandler);

//   int  registerDuplicateCmdHandler                 (int appId, duplicateCmdHandler_t  duplicateCmdHandler);

    /*
        This function register a callback function that will be called to validate that this node can
        participate in a given cluster.
        If this function is not register all create cluster or add node for this node will fail.

        Parameters:
            clusterValidationHandler: Validation function to make sure that all node in the cluster run
                                      the same software version and have the same license and any other data
                                      need to be validated.
        void  (*clusterValidationHandler_t)      (unsigned int validationDataLen, void *pValidationData, void *pContext);
        validationDataLen: Length of the data that will be used to validate if the node can join the cluster or not.
        pValidationData:   Pointer to the data.
        pContext:          This should be provided back in the call to reportValidationResult.

        This call back return WAVE_MESSAGE_SUCCESS if the node can be added to the cluster.

        Returns:
             WAVE_MESSAGE_SUCCESS: Registeration successed.
    */

    int registerClusterValidationHandler            (int appId, clusterValidationHandler_t  clusterValidationHandler);
    int registerClusterCollectValidationHandler     (int appId, clusterCollectValidationHandler_t  clusterCollectValidationHandler);
    int registerClusterReportValidationHandler      (int appId, clusterReportValidationHandler_t  clusterReportValidationHandler);

    /*
        This function report the status of validating the creating of a cluster. This is the reply
        for calling clusterValidationHandler.
        pContext:           Is the context provided while calling clusterValidationHandler.
        validationStatus:   Indicate if this node can join the cluster or not.

    */

    int reportValidationDetails (void *pContext, unsigned int validationStatus, unsigned int resultLen, void *resultPtr);
    int reportValidationResults (void *pContext, unsigned int validationStatus, unsigned int resultLen, void *resultPtr);


    /*
        This function register the main for the application that will use prism. This function
        should be call before PrismInit is called. Prism will create a seperate thread and call
        "applicationMain" from that thread.

    Parameters:
        applicationMain: Main entry for the application using Prism.
            int  (*applicationMainFunction)         (int argc, char *argv[]);
                argc:   Number of arguments.
                argv:   Parameter list.

        Returns:
    WAVE_MESSAGE_SUCCESS: Registeration successed.
    */
    int prismRegisterAppWithArg  (char * applicationName, applicationMainFunction applicationMain, unsigned int argc, char *argv[]);

    int prismRegisterApp  (char * applicationName, applicationMainFunction applicationMain);

    /*
        This function is used to configure the heartbeat rate and the number of heartbeat to lose before notifying
        the application. If this fucntion is not called the defualt values will be used. These values are
        heartbeat rate = 10000 msec. 3 heartbeat loss before considering the remote node as unavailable and
        notifying the application.

        This call effects the parameters on the local node only.  In a cluster, this function must be called on
        all of the nodes individually if you would like the parameters to take effect on all of the nodes.

        Parameters:
            heartBeatInterval:          Interval between heartbeats in milliseconds. Default is 10000 msec.
            nLostHeartBeatsToNotify:    Number of lost heartbeat before considering the other node dead and
                                        notify the application. default is 3.

        Returns:
            WAVE_MESSAGE_SUCCESS: successed.
    */

    int  setHeartBeatConfig                          (int heartBeatInterval, int nLostHeartBeatsToNotify);

    /*
        Get node role.

        Parameters:
            nodeRole:        Pointer to report the node role as defined by AppClusterRole

        Returns:
             WAVE_MESSAGE_SUCCESS.
    */

    int  getSwitchHaRole                             (int *nodeRole);

    /*
        Get the cluster configuration for the node that this fucntion is called from.

        Parameters:
            primaryNode:        Pointer to report the primary node and its status (ACTIVE, DOWN,...).
            nSecondaryNodes:    It specify the number of nodes that can be reported in secondaryNodes.
                                On return it is used to report number of secondary node in the cluster.
                                nSecondaryNodes should be greater than 0.
            secondaryNodes:     pointer to report all nodes in the cluster. The number of
                                secondary nodes reported is provided by nSecondaryNodes.

        Returns:
            WAVE_MESSAGE_SUCCESS: successed.
    */

    int  getSwitchHaConfig                           (haNodeStatus_t *primaryNode, int *nSecondaryNodes, haNodeStatus_t *secondaryNodes);

    /*
        Get the ip of the local node.

        Parameters:
            localIpAddr:    Pointer to ipString to store the ip address.

        Returns:
            WAVE_MESSAGE_SUCCESS: successed.
    */
    int  getLocalIpAddr                              (ipString localIpAddr);

    int duplicateMultiCastCmds (int appId, unsigned int nCommands, commandData_t commandList [], unsigned int nNodes, node_t nodeList [], duplicateCmdResultCallBack_t duplicateCmdResultCB, void *pContext);
    //int duplicateOneWayCmdsToClusterNodes (int appId, int nCommands, commandData_t commandList []);

#ifdef __cplusplus
}
#endif


#endif /* APPINTERFACE_H */

