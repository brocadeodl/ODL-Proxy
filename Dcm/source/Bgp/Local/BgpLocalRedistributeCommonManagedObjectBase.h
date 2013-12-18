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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/


#ifndef BGPLOCALREDISTRIBUTECOMMONMANAGEDOBJECTBASE_H
#define BGPLOCALREDISTRIBUTECOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "IpPolicy/Framework/IpPolicyLocalManagedObjects.h"
#include "Framework/Attributes/Attributes.h"



using namespace WaveNs;


namespace DcmNs{


class BgpLocalRedistributeCommonManagedObjectBase : virtual public CommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalRedistributeCommonManagedObjectBase             (PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~BgpLocalRedistributeCommonManagedObjectBase            ();
                void                                        setredistributeConnectedFlag                            (const bool &redistributeConnectedFlag);
                bool                                        getredistributeConnectedFlag                            ();
                void                                        setunicastMetric                                        (const UI32 &unicastMetric);
                UI32                                        getunicastMetric                                        ();
                void                                        setredistributeRouteMapName                             (const string &redistributeRouteMapName);
                string                                      getredistributeRouteMapName                             ();
                void                                        setredistributeOspfFlag                                 (const bool &redistributeOspfFlag);
                bool                                        getredistributeOspfFlag                                 ();
                void                                        setospfMatchInternal                                    (const bool &ospfMatchInternal);
                bool                                        getospfMatchInternal                                    ();
                void                                        setospfMatchExternalOne                                 (const bool &ospfMatchExternalOne);
                bool                                        getospfMatchExternalOne                                 ();
                void                                        setospfMatchExternalTwo                                 (const bool &ospfMatchExternalTwo);
                bool                                        getospfMatchExternalTwo                                 ();
                void                                        setospfMetric                                           (const UI32 &ospfMetric);
                UI32                                        getospfMetric                                           ();
                void                                        setospfRouteMapName                                     (const string &ospfRouteMapName);
                string                                      getospfRouteMapName                                     ();
                void                                        setredistributeStaticFlag                               (const bool &redistributeStaticFlag);
                bool                                        getredistributeStaticFlag                               ();
                void                                        setunicastStaticMetric                                  (const UI32 &unicastStaticMetric);
                UI32                                        getunicastStaticMetric                                  ();
                void                                        setstaticRouteMapName                                   (const string &staticRouteMapName);
                string                                      getstaticRouteMapName                                   ();
    private   :
    protected :
    public    :
        bool                                            m_redistributeConnectedFlag;
        UI32                                            m_unicastMetric;
        string                                          m_redistributeRouteMapName;
        bool                                            m_redistributeOspfFlag;
        bool                                            m_ospfMatchInternal;
        bool                                            m_ospfMatchExternalOne;
        bool                                            m_ospfMatchExternalTwo;
        UI32                                            m_ospfMetric;
        string                                          m_ospfRouteMapName;
        bool                                            m_redistributeStaticFlag;
        UI32                                            m_unicastStaticMetric;
        string                                          m_staticRouteMapName;
};
 
}
#endif
