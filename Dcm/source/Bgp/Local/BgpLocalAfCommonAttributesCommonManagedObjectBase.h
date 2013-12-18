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


#ifndef BGPLOCALAFCOMMONATTRIBUTESCOMMONMANAGEDOBJECTBASE_H
#define BGPLOCALAFCOMMONATTRIBUTESCOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "IpPolicy/Framework/IpPolicyLocalManagedObjects.h"
#include "Framework/Attributes/Attributes.h"

#include "Bgp/Local/BgpLocalAfBasicAttributesCommonManagedObjectBase.h"


using namespace WaveNs;


namespace DcmNs{


class BgpLocalAfCommonAttributesCommonManagedObjectBase:  virtual public BgpLocalAfBasicAttributesCommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalAfCommonAttributesCommonManagedObjectBase       (PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~BgpLocalAfCommonAttributesCommonManagedObjectBase      ();
                void                                        setclientToClientReflectionFlag                         (const bool &clientToClientReflectionFlag);
                bool                                        getclientToClientReflectionFlag                         ();
                void                                        setchoiceDampening                                      (const UI32 &choiceDampening);
                UI32                                        getchoiceDampening                                      ();
                void                                        setdampeningFlag                                        (const bool &dampeningFlag);
                bool                                        getdampeningFlag                                        ();
                void                                        setdampeningHalfTime                                    (const UI32 &dampeningHalfTime);
                UI32                                        getdampeningHalfTime                                    ();
                void                                        setdampeningReuseValue                                  (const UI32 &dampeningReuseValue);
                UI32                                        getdampeningReuseValue                                  ();
                void                                        setdampeningStartSuppressTime                           (const UI32 &dampeningStartSuppressTime);
                UI32                                        getdampeningStartSuppressTime                           ();
                void                                        setdampeningMaxSuppressTime                             (const UI32 &dampeningMaxSuppressTime);
                UI32                                        getdampeningMaxSuppressTime                             ();
                void                                        setdampeningRouteMapName                                (const string &dampeningRouteMapName);
                string                                      getdampeningRouteMapName                                ();
                void                                        setdefaultMetric                                        (const UI32 &defaultMetric);
                UI32                                        getdefaultMetric                                        ();
                void                                        setnextHopEnableDefault                                 (const bool &nextHopEnableDefault);
                bool                                        getnextHopEnableDefault                                 ();
                void                                        settableMapRouteMapName                                 (const string &tableMapRouteMapName);
                string                                      gettableMapRouteMapName                                 ();
                void                                        setupdateTime                                           (const UI32 &updateTime);
                UI32                                        getupdateTime                                           ();
    private   :
    protected :
    public    :
        bool                                            m_clientToClientReflectionFlag;
        UI32                                            m_choiceDampening;
        bool                                            m_dampeningFlag;
        UI32                                            m_dampeningHalfTime;
        UI32                                            m_dampeningReuseValue;
        UI32                                            m_dampeningStartSuppressTime;
        UI32                                            m_dampeningMaxSuppressTime;
        string                                          m_dampeningRouteMapName;
        UI32                                            m_defaultMetric;
        bool                                            m_nextHopEnableDefault;
        string                                          m_tableMapRouteMapName;
        UI32                                            m_updateTime;
};
 
}
#endif
