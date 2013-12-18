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


#ifndef BGPLOCALNEIGHBORATTRIBUTESCOMMONMANAGEDOBJECTBASE_H
#define BGPLOCALNEIGHBORATTRIBUTESCOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/Attributes.h"



using namespace WaveNs;


namespace DcmNs{


class BgpLocalNeighborAttributesCommonManagedObjectBase : virtual public CommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalNeighborAttributesCommonManagedObjectBase       (PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~BgpLocalNeighborAttributesCommonManagedObjectBase      ();
                void                                        setremoteAs                                             (const UI32 &remoteAs);
                UI32                                        getremoteAs                                             ();
                void                                        setdescription                                          (const string &description);
                string                                      getdescription                                          ();
                void                                        setshutdownFlag                                         (const bool &shutdownFlag);
                bool                                        getshutdownFlag                                         ();
                void                                        setshutdownGenerateRibOutFlag                           (const bool &shutdownGenerateRibOutFlag);
                bool                                        getshutdownGenerateRibOutFlag                           ();
                void                                        setlocalAs                                              (const UI32 &localAs);
                UI32                                        getlocalAs                                              ();
                void                                        setlocalAsNoPrependFlag                                 (const bool &localAsNoPrependFlag);
                bool                                        getlocalAsNoPrependFlag                                 ();
                void                                        setchoiceNextHopSelfType                                (const UI32 &choiceNextHopSelfType);
                UI32                                        getchoiceNextHopSelfType                                ();
                void                                        setnextHopSelfStatusFlag                                (const bool &nextHopSelfStatusFlag);
                bool                                        getnextHopSelfStatusFlag                                ();
                void                                        setnextHopSelfAlwaysFlag                                (const bool &nextHopSelfAlwaysFlag);
                bool                                        getnextHopSelfAlwaysFlag                                ();
                void                                        setadvertisementInterval                                (const UI32 &advertisementInterval);
                UI32                                        getadvertisementInterval                                ();
                void                                        setchoiceEbgpMultiHopType                               (const UI32 &choiceEbgpMultiHopType);
                UI32                                        getchoiceEbgpMultiHopType                               ();
                void                                        setebgpMultiHopFlag                                     (const bool &ebgpMultiHopFlag);
                bool                                        getebgpMultiHopFlag                                     ();
                void                                        setebgpMultiHopCount                                    (const UI32 &ebgpMultiHopCount);
                UI32                                        getebgpMultiHopCount                                    ();
                void                                        setchoiceMaxasLimit                                     (const UI32 &choiceMaxasLimit);
                UI32                                        getchoiceMaxasLimit                                     ();
                void                                        setasPathNum                                            (const UI32 &asPathNum);
                UI32                                        getasPathNum                                            ();
                void                                        setmaxasDisableFlag                                     (const bool &maxasDisableFlag);
                bool                                        getmaxasDisableFlag                                     ();
                void                                        setbgpPassword                                          (const string &bgpPassword);
                string                                      getbgpPassword                                          ();
                void                                        setenforceFirstAsFlag                                   (const bool &enforceFirstAsFlag);
                bool                                        getenforceFirstAsFlag                                   ();
                void                                        setenforceFirstAsDisableFlag                            (const bool &enforceFirstAsDisableFlag);
                bool                                        getenforceFirstAsDisableFlag                            ();
                void                                        setas4EnableFlag                                        (const bool &as4EnableFlag);
                bool                                        getas4EnableFlag                                        ();
                void                                        setas4DisableFlag                                       (const bool &as4DisableFlag);
                bool                                        getas4DisableFlag                                       ();
                void                                        setNeikeepAliveTime                                     (const UI32UC &NeikeepAliveTime);
                UI32UC                                      getNeikeepAliveTime                                     ();
                void                                        setNeiHoldTime                                          (const UI32UC &NeiHoldTime);
                UI32UC                                      getNeiHoldTime                                          ();
                void                                        setchoiceUpdateSource                                   (const UI32 &choiceUpdateSource);
                UI32                                        getchoiceUpdateSource                                   ();
                void                                        setsipIpv4Address                                       (const IpV4Address &sipIpv4Address);
                IpV4Address                                 getsipIpv4Address                                       ();
                void                                        setethInterfaceType                                     (const UI32 &ethInterfaceType);
                UI32                                        getethInterfaceType                                     ();
                void                                        setethInterface                                         (const string &ethInterface);
                string                                      getethInterface                                         ();
                void                                        setloopInterface                                        (const string &loopInterface);
                string                                      getloopInterface                                        ();
                void                                        setvirtualInterface                                     (const UI32 &virtualInterface);
                UI32                                        getvirtualInterface                                     ();
                void                                        setremovePrivateAsFlag                                  (const bool &removePrivateAsFlag);
                bool                                        getremovePrivateAsFlag                                  ();
                void                                        setasOverRideFlag                                       (const bool &asOverRideFlag);
                bool                                        getasOverRideFlag                                       ();
                void                                        setsoftReconfigurationInboundFlag                       (const bool &softReconfigurationInboundFlag);
                bool                                        getsoftReconfigurationInboundFlag                       ();
    private   :
    protected :
    public    :
        UI32                                            m_remoteAs;
        string                                          m_description;
        bool                                            m_shutdownFlag;
        bool                                            m_shutdownGenerateRibOutFlag;
        UI32                                            m_localAs;
        bool                                            m_localAsNoPrependFlag;
        UI32                                            m_choiceNextHopSelfType;
        bool                                            m_nextHopSelfStatusFlag;
        bool                                            m_nextHopSelfAlwaysFlag;
        UI32                                            m_advertisementInterval;
        UI32                                            m_choiceEbgpMultiHopType;
        bool                                            m_ebgpMultiHopFlag;
        UI32                                            m_ebgpMultiHopCount;
        UI32                                            m_choiceMaxasLimit;
        UI32                                            m_asPathNum;
        bool                                            m_maxasDisableFlag;
        string                                          m_bgpPassword;
        bool                                            m_enforceFirstAsFlag;
        bool                                            m_enforceFirstAsDisableFlag;
        bool                                            m_as4EnableFlag;
        bool                                            m_as4DisableFlag;
        UI32UC                                          m_NeikeepAliveTime;
        UI32UC                                          m_NeiHoldTime;
        UI32                                            m_choiceUpdateSource;
        IpV4Address                                     m_sipIpv4Address;
        UI32                                            m_ethInterfaceType;
        string                                          m_ethInterface;
        string                                          m_loopInterface;
        UI32                                            m_virtualInterface;
        bool                                            m_removePrivateAsFlag;
        bool                                            m_asOverRideFlag;
        bool                                            m_softReconfigurationInboundFlag;
};
 
}
#endif
