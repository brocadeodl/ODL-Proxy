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


#ifndef BGPLOCALAFNEIGHBORATTRIBUTESCOMMONMANAGEDOBJECTBASE_H
#define BGPLOCALAFNEIGHBORATTRIBUTESCOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Attributes/Attributes.h"



using namespace WaveNs;


namespace DcmNs{


class BgpLocalAfNeighborAttributesCommonManagedObjectBase : virtual public CommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalAfNeighborAttributesCommonManagedObjectBase     (PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~BgpLocalAfNeighborAttributesCommonManagedObjectBase    ();
                void                                        setactivateFlag                                         (const bool &activateFlag);
                bool                                        getactivateFlag                                         ();
                void                                        setfilterListDirectionInAclName                         (const string &filterListDirectionInAclName);
                string                                      getfilterListDirectionInAclName                         ();
                void                                        setfilterListdirectionInAclNameFlag                     (const bool &filterListdirectionInAclNameFlag);
                bool                                        getfilterListdirectionInAclNameFlag                     ();
                void                                        setfilterListDirectionOutAclName                        (const string &filterListDirectionOutAclName);
                string                                      getfilterListDirectionOutAclName                        ();
                void                                        setfilterListdirectionOutAclNameFlag                    (const bool &filterListdirectionOutAclNameFlag);
                bool                                        getfilterListdirectionOutAclNameFlag                    ();
                void                                        setmaxiumPrefixLimit                                    (const UI32 &maxiumPrefixLimit);
                UI32                                        getmaxiumPrefixLimit                                    ();
                void                                        setmaxiumPrefixthreshold                                (const UI32 &maxiumPrefixthreshold);
                UI32                                        getmaxiumPrefixthreshold                                ();
                void                                        setmaxiumPrefixTeardownFlag                             (const bool &maxiumPrefixTeardownFlag);
                bool                                        getmaxiumPrefixTeardownFlag                             ();
                void                                        setdefaultOriginateStatus                               (const bool &defaultOriginateStatus);
                bool                                        getdefaultOriginateStatus                               ();
                void                                        setdefaultOriginateRouteMapName                         (const string &defaultOriginateRouteMapName);
                string                                      getdefaultOriginateRouteMapName                         ();
                void                                        setprefixListInName                                     (const string &prefixListInName);
                string                                      getprefixListInName                                     ();
                void                                        setprefixListInFlag                                     (const bool &prefixListInFlag);
                bool                                        getprefixListInFlag                                     ();
                void                                        setprefixListOutName                                    (const string &prefixListOutName);
                string                                      getprefixListOutName                                    ();
                void                                        setprefixListOutFlag                                    (const bool &prefixListOutFlag);
                bool                                        getprefixListOutFlag                                    ();
                void                                        setrouteMapNameIn                                       (const string &routeMapNameIn);
                string                                      getrouteMapNameIn                                       ();
                void                                        setrouteMapNameOut                                      (const string &routeMapNameOut);
                string                                      getrouteMapNameOut                                      ();
                void                                        setrouteReflectorClientFlag                             (const bool &routeReflectorClientFlag);
                bool                                        getrouteReflectorClientFlag                             ();
                void                                        setsendCommunityBoth                                    (const bool &sendCommunityBoth);
                bool                                        getsendCommunityBoth                                    ();
                void                                        setsendCommunityExtended                                (const bool &sendCommunityExtended);
                bool                                        getsendCommunityExtended                                ();
                void                                        setsendCommunityStandard                                (const bool &sendCommunityStandard);
                bool                                        getsendCommunityStandard                                ();
                void                                        setsendCommunityStatus                                  (const bool &sendCommunityStatus);
                bool                                        getsendCommunityStatus                                  ();
                void                                        setunSuppressRouteMapName                               (const string &unSuppressRouteMapName);
                string                                      getunSuppressRouteMapName                               ();
                void                                        setweight                                               (const UI32 &weight);
                UI32                                        getweight                                               ();
    private   :
    protected :
    public    :
        bool                                            m_activateFlag;
        string                                          m_filterListDirectionInAclName;
        bool                                            m_filterListdirectionInAclNameFlag;
        string                                          m_filterListDirectionOutAclName;
        bool                                            m_filterListdirectionOutAclNameFlag;
        UI32                                            m_maxiumPrefixLimit;
        UI32                                            m_maxiumPrefixthreshold;
        bool                                            m_maxiumPrefixTeardownFlag;
        bool                                            m_defaultOriginateStatus;
        string                                          m_defaultOriginateRouteMapName;
        string                                          m_prefixListInName;
        bool                                            m_prefixListInFlag;
        string                                          m_prefixListOutName;
        bool                                            m_prefixListOutFlag;
        string                                          m_routeMapNameIn;
        string                                          m_routeMapNameOut;
        bool                                            m_routeReflectorClientFlag;
        bool                                            m_sendCommunityBoth;
        bool                                            m_sendCommunityExtended;
        bool                                            m_sendCommunityStandard;
        bool                                            m_sendCommunityStatus;
        string                                          m_unSuppressRouteMapName;
        UI32                                            m_weight;
};
 
}
#endif
