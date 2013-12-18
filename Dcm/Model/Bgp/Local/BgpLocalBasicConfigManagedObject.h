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
 *   Copyright (C) 2013-2018 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/


#ifndef BGPLOCALBASICCONFIGMANAGEDOBJECT_H
#define BGPLOCALBASICCONFIGMANAGEDOBJECT_H


#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Bgp/Local/BgpLocalAfIpv4UcastManagedObject.h"
#include "Bgp/Local/BgpLocalNeighborIpxAddrManagedObject.h"
#include "Bgp/Local/BgpLocalNeighborPeerGrpManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"



using namespace WaveNs;


namespace DcmNs{

class BgpLocalObjectManager;
class BgpLocalNeighborPeerGrpManagedObject;
class BgpLocalNeighborIpxAddrManagedObject;
class BgpLocalAfIpv4UcastManagedObject;


class BgpLocalBasicConfigManagedObject : virtual public DcmLocalManagedObject
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalBasicConfigManagedObject                        (BgpLocalObjectManager *pBgpLocalObjectManager);
                                                            BgpLocalBasicConfigManagedObject                        (BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const string &vrfName,
                                                                                                                const UI32 &localAs,
                                                                                                                const bool &alwaysCompareMedFlag,
                                                                                                                const bool &asPathIgnoreFlag,
                                                                                                                const bool &capabilityAs4Flag,
                                                                                                                const UI32 &choiceClusterId,
                                                                                                                const UI32 &clusterId,
                                                                                                                const IpV4Address &clusterIdIpAddress,
                                                                                                                const bool &compareMedEmptyAsPathFlag,
                                                                                                                const bool &compareRouterIdFlag,
                                                                                                                const UI32 &defaultLocalPreference,
                                                                                                                const UI32 &extRoutedistance,
                                                                                                                const UI32 &intRouteDistance,
                                                                                                                const UI32 &lclRouteDistance,
                                                                                                                const bool &enforceFirstAsFlag,
                                                                                                                const bool &fastExternalFallOverFlag,
                                                                                                                const bool &installIgpCostFlag,
                                                                                                                const bool &logDampeningDebug,
                                                                                                                const UI32 &maxAsLimit,
                                                                                                                const bool &medMissingAsWorstFlag,
                                                                                                                const UI32UC &routerBgpModeTimersKeepAliveTime,
                                                                                                                const UI32UC &routerBgpModeTimerHoldTime,
                                                                                                                const vector<WaveManagedObjectPointer<BgpLocalNeighborPeerGrpManagedObject> > &routerBgpNeighborPeerGrp,
                                                                                                                const vector<WaveManagedObjectPointer<BgpLocalNeighborIpxAddrManagedObject> > &routerBgpNeighborIpxAddr,
                                                                                                                const WaveManagedObjectPointer<BgpLocalAfIpv4UcastManagedObject> &routerBgpAddressIpv4Uc);
                static string                               getClassName                                            ();
                virtual void                                setupKeys                                               ();
                virtual void                                validateForCreateAtThisNode                             (ValidateForUpdateAtThisNodeContext *pContext);
                virtual void                                isAssociatedManagedObjectToBeCreated                    (IsAssociatedManagedObjectToBeCreatedContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForCreate                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForUpdate                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForDelete                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForCreate                (GetHardwareConfigurationDetailsForCreateContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForUpdate                (GetHardwareConfigurationDetailsForUpdateContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForDelete                (GetHardwareConfigurationDetailsForDeleteContext *pContext);
                virtual void                                setHardwareConfigurationDetails                         (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap);
                virtual void                                setPostbootHardwareConfigurationDetails                 (map<UI32, vector<string> > &backendAttributesMap, map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap);
                virtual void                                getHardwareConfigurationDetailsForPostboot              (GetHardwareConfigurationDetailsForPostbootContext *pContext);
                virtual                                     ~BgpLocalBasicConfigManagedObject                       ();
                void                                        setvrfName                                              (const string &vrfName);
                string                                      getvrfName                                              ();
                void                                        setlocalAs                                              (const UI32 &localAs);
                UI32                                        getlocalAs                                              ();
                void                                        setalwaysCompareMedFlag                                 (const bool &alwaysCompareMedFlag);
                bool                                        getalwaysCompareMedFlag                                 ();
                void                                        setasPathIgnoreFlag                                     (const bool &asPathIgnoreFlag);
                bool                                        getasPathIgnoreFlag                                     ();
                void                                        setcapabilityAs4Flag                                    (const bool &capabilityAs4Flag);
                bool                                        getcapabilityAs4Flag                                    ();
                void                                        setchoiceClusterId                                      (const UI32 &choiceClusterId);
                UI32                                        getchoiceClusterId                                      ();
                void                                        setclusterId                                            (const UI32 &clusterId);
                UI32                                        getclusterId                                            ();
                void                                        setclusterIdIpAddress                                   (const IpV4Address &clusterIdIpAddress);
                IpV4Address                                 getclusterIdIpAddress                                   ();
                void                                        setcompareMedEmptyAsPathFlag                            (const bool &compareMedEmptyAsPathFlag);
                bool                                        getcompareMedEmptyAsPathFlag                            ();
                void                                        setcompareRouterIdFlag                                  (const bool &compareRouterIdFlag);
                bool                                        getcompareRouterIdFlag                                  ();
                void                                        setdefaultLocalPreference                               (const UI32 &defaultLocalPreference);
                UI32                                        getdefaultLocalPreference                               ();
                void                                        setextRoutedistance                                     (const UI32 &extRoutedistance);
                UI32                                        getextRoutedistance                                     ();
                void                                        setintRouteDistance                                     (const UI32 &intRouteDistance);
                UI32                                        getintRouteDistance                                     ();
                void                                        setlclRouteDistance                                     (const UI32 &lclRouteDistance);
                UI32                                        getlclRouteDistance                                     ();
                void                                        setenforceFirstAsFlag                                   (const bool &enforceFirstAsFlag);
                bool                                        getenforceFirstAsFlag                                   ();
                void                                        setfastExternalFallOverFlag                             (const bool &fastExternalFallOverFlag);
                bool                                        getfastExternalFallOverFlag                             ();
                void                                        setinstallIgpCostFlag                                   (const bool &installIgpCostFlag);
                bool                                        getinstallIgpCostFlag                                   ();
                void                                        setlogDampeningDebug                                    (const bool &logDampeningDebug);
                bool                                        getlogDampeningDebug                                    ();
                void                                        setmaxAsLimit                                           (const UI32 &maxAsLimit);
                UI32                                        getmaxAsLimit                                           ();
                void                                        setmedMissingAsWorstFlag                                (const bool &medMissingAsWorstFlag);
                bool                                        getmedMissingAsWorstFlag                                ();
                void                                        setrouterBgpModeTimersKeepAliveTime                     (const UI32UC &routerBgpModeTimersKeepAliveTime);
                UI32UC                                      getrouterBgpModeTimersKeepAliveTime                     ();
                void                                        setrouterBgpModeTimerHoldTime                           (const UI32UC &routerBgpModeTimerHoldTime);
                UI32UC                                      getrouterBgpModeTimerHoldTime                           ();
                void                                        setrouterBgpNeighborPeerGrp                             (const vector<WaveManagedObjectPointer<BgpLocalNeighborPeerGrpManagedObject> > &routerBgpNeighborPeerGrp);
                vector<WaveManagedObjectPointer<BgpLocalNeighborPeerGrpManagedObject> > getrouterBgpNeighborPeerGrp                             ();
                void                                        setrouterBgpNeighborIpxAddr                             (const vector<WaveManagedObjectPointer<BgpLocalNeighborIpxAddrManagedObject> > &routerBgpNeighborIpxAddr);
                vector<WaveManagedObjectPointer<BgpLocalNeighborIpxAddrManagedObject> > getrouterBgpNeighborIpxAddr                             ();
                void                                        setrouterBgpAddressIpv4Uc                               (const WaveManagedObjectPointer<BgpLocalAfIpv4UcastManagedObject> &routerBgpAddressIpv4Uc);
                WaveManagedObjectPointer<BgpLocalAfIpv4UcastManagedObject> getrouterBgpAddressIpv4Uc                               ();
                virtual string                                      customConfigurationDisplay                      (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext);
    private   :
    protected :
    public    :
        string                                          m_vrfName;
        UI32                                            m_localAs;
        bool                                            m_alwaysCompareMedFlag;
        bool                                            m_asPathIgnoreFlag;
        bool                                            m_capabilityAs4Flag;
        UI32                                            m_choiceClusterId;
        UI32                                            m_clusterId;
        IpV4Address                                     m_clusterIdIpAddress;
        bool                                            m_compareMedEmptyAsPathFlag;
        bool                                            m_compareRouterIdFlag;
        UI32                                            m_defaultLocalPreference;
        UI32                                            m_extRoutedistance;
        UI32                                            m_intRouteDistance;
        UI32                                            m_lclRouteDistance;
        bool                                            m_enforceFirstAsFlag;
        bool                                            m_fastExternalFallOverFlag;
        bool                                            m_installIgpCostFlag;
        bool                                            m_logDampeningDebug;
        UI32                                            m_maxAsLimit;
        bool                                            m_medMissingAsWorstFlag;
        UI32UC                                          m_routerBgpModeTimersKeepAliveTime;
        UI32UC                                          m_routerBgpModeTimerHoldTime;
        vector<WaveManagedObjectPointer<BgpLocalNeighborPeerGrpManagedObject> > m_routerBgpNeighborPeerGrp;
        vector<WaveManagedObjectPointer<BgpLocalNeighborIpxAddrManagedObject> > m_routerBgpNeighborIpxAddr;
        WaveManagedObjectPointer<BgpLocalAfIpv4UcastManagedObject> m_routerBgpAddressIpv4Uc;
};
 
}
#endif
