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

#ifndef CLUSTERMESSAGES_H
#define CLUSTERMESSAGES_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Cluster/NodeManagedObject.h"
#include "Cluster/ClusterTypes.h"
#include "App/AppInterface.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include <string>

namespace WaveNs
{

class ClusterObjectManagerCreateClusterMessage : public ManagementInterfaceMessage
{
    private :
    protected :
    public :
        virtual                            ~ClusterObjectManagerCreateClusterMessage    ();
                                            ClusterObjectManagerCreateClusterMessage    ();
                                            ClusterObjectManagerCreateClusterMessage    (const string &serviceName, const UI32 &operationId);
                                            ClusterObjectManagerCreateClusterMessage    (const string &serviceName, const UI32 &operationId, const vector<string> &secondaryNodeNames, const vector<SI32> &secondaryNodePorts);
                                            ClusterObjectManagerCreateClusterMessage    (string primaryNodeName, UI32 nSecondaryNodes, string secondariesNodeNames []);
                                            ClusterObjectManagerCreateClusterMessage    (const vector<string> &secondaryNodeNames, const vector<SI32> &secondaryNodePorts);
                    void                    setupAttributesForSerialization             ();
                    void                    setPrimaryNodeName                          (const string &primaryNodeName);
                    void                    setNSecondaryNodes                          (const UI32 nSecondaryNodes);
                    void                    setSecondaryNodeNameAndPort                 (const string &nodeName, const SI32 &nodePort);
                    string                  getPrimaryNodeName                          ();
                    UI32                    getNSecondaryNodes                          ();
                    string                  getSecondaryNodeName                        (const UI32 index);
                    SI32                    getSecondaryNodePort                        (const UI32 index);
                    haNodeStatus_t         *getNodeStatusAddr                           (UI32 index);
                    void                    setNodeStaus                                (const string &nodeName, UI32 nodePort, UI32 Status);
                    string                  getStatusNodeName                           (UI32 index);
                    SI32                    getStatusNodePort                           (UI32 index);
                    UI32                    getStatusNodeStatus                         (UI32 index);
                    UI32                    getNodeStatus                               (const string &nodeName, const UI32& nodePort);

                    void                    setIsAddNodeForSpecialCaseFlag              (const bool &isAddNodeForSpecialCase);
                    bool                    getIsAddNodeForSpecialCaseFlag              () const;

                    void                    getFilenamesToSync                          ( vector<string> &filenamesToSync ) ;
                    void                    setFilenamesToSync                          ( const vector<string> &filenamesToSync );
                    void                    addFilenameToSync                           ( const string &filenameToSync );

    // Now the data members

    private :
        string                          m_primaryNodeName;
        UI32                            m_nSecondaryNodes;
        vector <string>                 m_secondaryNodeNames;
        vector <SI32>                   m_secondaryNodePorts;
        vector <string>                 m_statusNodeNames;
        vector <SI32>                   m_statusNodePorts;
        vector <UI32>                   m_statusNodeStatus;
        bool                            m_isAddNodeForSpecialCase;
        vector <string>                 m_filenamesToSync;

    protected :
    public :
};

class ClusterObjectManagerDeleteClusterMessage : public PrismMessage
{
    private :
    protected :
    public :
         virtual                                    ~ClusterObjectManagerDeleteClusterMessage   ();
                                                     ClusterObjectManagerDeleteClusterMessage   ();
                     void                            setupAttributesForSerialization            ();
                     void                            setIsRebootRequired                        (const bool &isRebootRequired);
                     bool                            getIsRebootRequired                        () const;

    // Now the data members

    private :
                    bool                             m_isRebootRequired;
    protected :
    public :
};

class ClusterObjectManagerAddNodeMessage : public ClusterObjectManagerCreateClusterMessage
{
    private :
                  void              setupAttributesForSerialization     ();
    protected :
    public :
        virtual                    ~ClusterObjectManagerAddNodeMessage  ();
                                    ClusterObjectManagerAddNodeMessage  ();
                                    ClusterObjectManagerAddNodeMessage  (const vector<string> &secondaryNodeNames, const vector<SI32> &secondaryNodePorts);

    // Now the data members

    private :
    protected :
    public :
};


//Class Name:
//ClusterObjectManagerRejoinNodeMessage
//
//Class Description:
//This message is used by JMP or the CLI command  in the
//cluster subshell then to the framework and the this is dispatched
//by the framework to the Principal Node to initiate the rejoin
// of the nodes in the message

class ClusterObjectManagerRejoinNodeMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual                    ~ClusterObjectManagerRejoinNodeMessage    ();
                                    ClusterObjectManagerRejoinNodeMessage    ();
		//Method for putting the attributes for serialization in a map
                void                setupAttributesForSerialization          ();

		//Mutator and accessor methods
                void                setNodeIpAddress                         (const string &nodeIp);
                string  const &     getNodeIpAddress                         (const UI32 index) const;
                void                setNodePort                              (const UI32 &nodePort);
                UI32                getNodePort                              (const UI32 index) const;
		UI32                getNNodes                                () const;
                bool                getIsReplaceRejoin                       () const;
                void                setIsReplaceRejoin                       (const bool &isReplaceRejoin);
                void                setNodeLocationId                        (const LocationId &locationId);
                LocationId          getNodeLocationId                        (const UI32 index) const;
                void                setNodeStatus                            (const string &nodeIp, SI32 nodePort, ResourceId status);
                ResourceId          getNodeStatus                            (const string &nodeIp, SI32 nodePort);

    private :

        vector <string>                 m_nodeIpAddresses;
        vector <UI32>                   m_nodePorts;
        bool                            m_isReplaceRejoin;
        vector <LocationId>             m_locationIds;
        map<string, ResourceId>         m_nodesStatus;

    protected :
    public :
};

class ClusterObjectManagerJoinNodeMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual                    ~ClusterObjectManagerJoinNodeMessage ();
                                    ClusterObjectManagerJoinNodeMessage ();
                void                setNodeName                         (const string &nodeName);
                string              getNodeName                         ();

                void                setNodePort                         (const SI32 &nodePort);
                SI32                getNodePort                         ();


    // Now the data members

    private :
        string                  m_nodeName;
        SI32                    m_nodePort;

    protected :
    public :
};

class ClusterObjectManagerDeleteNodeMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual        ~ClusterObjectManagerDeleteNodeMessage   ();
                        ClusterObjectManagerDeleteNodeMessage   ();
                void    setupAttributesForSerialization         ();
                void    setSecondaryNodeNameAndPort             (const string &nodeName, const SI32 &nodePort);
                void    setNNodesToDelete                       (const UI32 nNodes);
                string  getNodeName                             (UI32 index);
                SI32    getNodePort                             (UI32 index);
                UI32    getNNodesToDelete                       ();
                void    setNodeStatus                            (const string &nodeName, const SI32 nodePort, const UI32 Status);
                UI32    getNodeStatus                           (string &nodeName, SI32 &nodePort) const;
                void    setIsDisconnected                       (bool isDisconnected);
                bool    getIsDisconnected                       ();

    // Now the data members

    private :
        vector <string> m_nodeName;
        vector <SI32>   m_nodePort;
        vector <UI32>   m_nodeStatus;
        vector <string> m_statusNodeNames;
        vector <SI32>   m_statusNodePorts;
        UI32            m_nNodes;
        bool            m_isDisconnected;

   protected :
    public :
};

#if 0
class LocalClusterConfigObjectManagerRegisterHeartBeatLossCBMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual                    ~LocalClusterConfigObjectManagerRegisterHeartBeatLossCBMessage   ();
                                    LocalClusterConfigObjectManagerRegisterHeartBeatLossCBMessage   ();
               void                 setHeartBeatLostCallBack                                        (heartBeatCallBack_t heartBeatLostCallBack);
               heartBeatCallBack_t  getHeartBeatLostCallBack                                        ();


    // Now the data members

    private :
        heartBeatCallBack_t                             m_heartBeatLostCallBack;
    protected :
    public :
};

class LocalClusterConfigObjectManagerRegisterMemberJoinedCBMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual                         ~LocalClusterConfigObjectManagerRegisterMemberJoinedCBMessage   ();
                                         LocalClusterConfigObjectManagerRegisterMemberJoinedCBMessage   ();
                void                     setMemberJoinedCallBack                                       (memberJoinedCallBack_t memberJoinedCallBack);
                memberJoinedCallBack_t   getMemberJoinedCallBack                                       ();

    // Now the data members

    private :
        memberJoinedCallBack_t          m_memberJoinedCallBack;
    protected :
    public :
};
#endif

class LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual        ~LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage ();
                        LocalClusterConfigObjectManagerReportPrimaryNodeChangedMessage ();
                void    setPrimaryNodeName                                             (const string &primaryNodeName);
                void    setPrimaryNodePort                                             (const SI32 &primaryNodePort);
                void    setThisNodeName                                                (const string &thisNodeName);
                void    setThisNodePort                                                (const SI32 &thisNodePort);
                void    setNSecondaryNodes                                             (const UI32 nSecondaryNodes);
                void    setSecondaryNodeNameAndPort                                    (const string &nodeName, const SI32 &nodePort);
                //void    setSecondaryNodePort                                           (const UI32 index, const SI32 &nodePort);
                string  getPrimaryNodeName                                             ();
                SI32    getPrimaryNodePort                                             ();
                string  getThisNodeName                                                () const;
                SI32    getThisNodePort                                                () const;
                UI32    getNSecondaryNodes                                             ();
                string  getSecondaryNodeName                                           (const UI32 &index);
                SI32    getSecondaryNodePort                                           (const UI32 &index);

    // Now the data members

    private :
        string          m_primaryNodeName;
        SI32            m_primaryNodePort;
        string          m_thisNodeName;
        SI32            m_thisNodePort;
        UI32            m_nSecondaryNodes;
        vector <string> m_secondaryNodeNames;
        vector <SI32>   m_secondaryNodePorts;

    protected :
    public :
};

class LocalClusterConfigObjectManagerReportRemovedNodeFromClusterMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual        ~LocalClusterConfigObjectManagerReportRemovedNodeFromClusterMessage ();
                        LocalClusterConfigObjectManagerReportRemovedNodeFromClusterMessage ();


    // Now the data members

    private :
    protected :
    public :
};

#if 0
class LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual        ~LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage   ();
                        LocalClusterConfigObjectManagerReportCenteralHeartBeatMessage   ();
        virtual void    setupAttributesForSerialization ();
                void    setHeartBeatNum                                                 (const UI32 heartBeatNum);
                void    setNodeName                                                     (const string nodeName);
                UI32    getHeartBeatNum                                                 ();
                string  getNodeName                                                     ();

    // Now the data members

    private :
        UI32                        m_heartBeatNum;
        string                      m_nodeName;
    protected :
    public :

};

class CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual        ~CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage   ();
                        CenteralClusterConfigObjectManagerReportLocalHeartBeatMessage   ();
        virtual void    setupAttributesForSerialization                                 ();
                void    setHeartBeatNum                                                 (const UI32 heartBeatNum);
                void    setNodeName                                                     (const string nodeName);
                UI32    getHeartBeatNum                                                 ();
                string  getNodeName                                                     ();

    // Now the data members

    private :
        UI32                        m_heartBeatNum;
        string                      m_nodeName;
    protected :
    public :

};



class LocalClusterConfigObjectManagerSetHeartBeatConfigMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual        ~LocalClusterConfigObjectManagerSetHeartBeatConfigMessage ();
                        LocalClusterConfigObjectManagerSetHeartBeatConfigMessage ();
                void    setHeartBeatInterval                            (const UI32 heartBeatInterval);
                UI32    getHeartBeatInterval                            ();
                void    setNLostHeartBeatsToNotify                      (const UI32 nLostHeartBeatToNotify);
                UI32    getNLostHeartBeatsToNotify                      ();
    // Now the data members

    private :
        UI32                        m_heartBeatInterval;
        UI32                        m_nLostHeartBeatToNotify;
    protected :
    public :
};

#endif

class LocalClusterConfigObjectManagerGetNodeRoleMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual        ~LocalClusterConfigObjectManagerGetNodeRoleMessage ();
                        LocalClusterConfigObjectManagerGetNodeRoleMessage ();
            void    setNodeRole                                                 (const UI32 role);
            UI32    getNodeRole                                                 ();
    // Now the data members

    private :
        UI32                        m_nodeRole;
    protected :
    public :
};

typedef struct
{
    string          nodeName;
    UI32            nodePort;
    SI32            nodeStatus;
} nodeStatus_t;

class LocalClusterConfigObjectManagerGetClusterConfigMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual            ~LocalClusterConfigObjectManagerGetClusterConfigMessage ();
                            LocalClusterConfigObjectManagerGetClusterConfigMessage ();

                string      getPrimaryNodeName                                ();
                UI32        getPrimaryNodeStatus                              ();
                UI32        getPrimaryNodePort                                ();
                UI32        getNSecondaryNodes                                ();
                string      getSecondaryNodeName                              (UI32 index);
                UI32        getSecondaryNodeStatus                            (UI32 index);
                UI32        getSecondaryNodePort                              (UI32 index);

                void        setPrimaryNodeName                                (string nodeName);
                void        setPrimaryNodePort                                (SI32 nodePort);
                void        setPrimaryNodeStatus                              (UI32 status);
                void        setNSecondaryNodes                                (UI32 nSecondaryNodes);
                void        setSecondaryNodeStatus                            (string nodeName, SI32 nodePort, SI32 status);

                // Now the data members

    private :
        string                  m_primaryNodeName;
        UI32                    m_primaryNodeStatus;
        UI32                    m_primaryNodePort;
        UI32                    m_nSecondaryNodes;
        vector <nodeStatus_t>   m_SecondaryNodeStatus;

    protected :
    public :
};


class HeartBeatLostMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual         ~HeartBeatLostMessage   ();
                HeartBeatLostMessage   (/*PrismServiceId id, */string dstIpAddress, UI32 dstPort/* , PrismElement *pStartHeartBeatSender*/);
                string   getDstIpAddress        ();
                void     setDstIpAddress        (string dstIpAddress);
                UI32     getDstPort             ();
                void     setDstPort             (UI32 dstPort);
    // Now the data members

    private :
//        PrismElement  *m_pStartHeartBeatSender;
        string         m_dstIpAddress;
        UI32           m_dstPort;

    protected :
    public :
};

class HeartBeatResumedMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual         ~HeartBeatResumedMessage ();
        HeartBeatResumedMessage (/*PrismServiceId id, */string dstIpAddress, UI32 dstPort /*, PrismElement *pStartHeartBeatSender*/);
        string   getDstIpAddress         ();
        void     setDstIpAddress         (string dstIpAddress);
        UI32     getDstPort              ();
        void     setDstPort              (UI32 dstPort);
    // Now the data members

    private :
//        PrismElement  *m_pStartHeartBeatSender;
        string         m_dstIpAddress;
        UI32           m_dstPort;

    protected :
    public :
};

//Class Name:
//ClusterConfigObjectManagerGetClusterInfoMessage
//
//Class Description:
//When the CLI command getClusterDbgInfo is invoked from the
//cluster subshell then an object of this class is synchronously
// to the framework. this is dispatched by the framework to the
// Principal Node where the information regarding the Principal
// and Secondary nodes is filled. Then teh Principal sends this
// message back to the calling node's CLI.

class ClusterConfigObjectManagerGetClusterInfoMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual            ~ClusterConfigObjectManagerGetClusterInfoMessage ();
                            ClusterConfigObjectManagerGetClusterInfoMessage ();

		//Is the Cluster Created
		bool        isClusterCreated                                () const;
		void        setClusterCreated                               (const bool & clusterCreated);

		//Methods related to primary node
                string      getPrimaryNodeIpAddress                         () const;
	        UI32        getPrimaryNodePort                              () const;
                LocationId  getPrimaryNodeLocationId                        () const;
		ResourceId        getPrimaryNodeGenericStatus                     () const;
		ResourceId        getPrimaryNodeSpecificStatus                    () const;


                void        setPrimaryNodeIpAddress                         (const string & IpAddress);
                void        setPrimaryNodePort                              (const UI32 & nodePort);
                void        setPrimaryNodeLocationId                        (const LocationId & nodeLocationId);
		void        setPrimaryNodeGenericStatus                     (const ResourceId & status);
		void        setPrimaryNodeSpecificStatus                    (const ResourceId & status);


		//Methods related to secondary nodes
		UI32        getNSecondaryNodes                              () const;
                void        getSecondaryNodeDebugInfo                       (const UI32 & index,
									     string & secondaryNodeIpAddress,
									     UI32 & secondaryNodePort,
									     LocationId & secondaryNodeLocationId,
									     ResourceId & secondaryNodeGenericStatus,
									     ResourceId & secondaryNodeSpecificStatus) const;

                void        setNSecondaryNodes                              (const UI32 & nSecondaryNodes);
		void        setSecondaryNodeDebugInfo                       (const string & secondaryNodeIpAddress,
									     const UI32 & secondaryNodePort,
									     const LocationId & secondaryNodeLocationId,
									     const ResourceId & secondaryNodeGenericStatus,
									     const ResourceId & secondaryNodeSpecificStatus);
    virtual     void        setupAttributesForSerialization                 ();

    private :

	//IsClusterCreated
	bool                    m_ClusterCreated;
	//Primary Node Information
        string                  m_primaryNodeIpAddress;
        UI32                    m_primaryNodePort;
        LocationId              m_primaryNodeLocationId;
	ResourceId              m_primaryNodeGenericStatus;
	ResourceId              m_primaryNodeSpecificStatus;
        UI32                    m_nSecondaryNodes;

	//Secondary Node Information stored in vectors
	vector <string> m_secondaryNodeIPAddresses;
	vector <UI32>   m_secondaryNodePorts;
	vector <LocationId>   m_secondaryNodeLocationIds;
	//Whether the node is active.If it is in the cluster
	//it will be active unless there is an error.
	vector <ResourceId>   m_secondaryNodeGenericStatus;
	vector <ResourceId>   m_secondaryNodeSpecificStatus;
    protected :
    public :
};

class CentralClusterConfigUpdateHardwareSynchronizationStateMessage : public PrismMessage
{
    private :
                void                setupAttributesForSerialization                                 ();

    protected :
    public :
                                    CentralClusterConfigUpdateHardwareSynchronizationStateMessage   ();
                                    CentralClusterConfigUpdateHardwareSynchronizationStateMessage   (ResourceId hardwareSynchronizationState, const vector<LocationId> &locationIds);
        virtual                    ~CentralClusterConfigUpdateHardwareSynchronizationStateMessage   ();

                ResourceId          getHardwareSynchronizationState                                 () const;
                void                setHardwareSynchronizationState                                 (ResourceId hardwareSynchronizationState);

                vector<LocationId>  getLocationIds                                                  () const;
                void                setLocationIds                                                  (const vector<LocationId> &locationIds);

    // Now the data members

    private :
                ResourceId          m_hardwareSynchronizationState;
                vector<LocationId>  m_locationIds;

    protected :
    public :
};

}

#endif // CLUSTERMESSAGES_H
