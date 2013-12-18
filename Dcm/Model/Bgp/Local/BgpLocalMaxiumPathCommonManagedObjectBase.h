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


#ifndef BGPLOCALMAXIUMPATHCOMMONMANAGEDOBJECTBASE_H
#define BGPLOCALMAXIUMPATHCOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Attributes/Attributes.h"



using namespace WaveNs;


namespace DcmNs{


class BgpLocalMaxiumPathCommonManagedObjectBase : virtual public CommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalMaxiumPathCommonManagedObjectBase               (PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~BgpLocalMaxiumPathCommonManagedObjectBase              ();
                void                                        setloadSharingValue                                     (const UI32 &loadSharingValue);
                UI32                                        getloadSharingValue                                     ();
                void                                        setebgpLoadSharingValue                                 (const UI32 &ebgpLoadSharingValue);
                UI32                                        getebgpLoadSharingValue                                 ();
                void                                        setibgpLoadSharingValue                                 (const UI32 &ibgpLoadSharingValue);
                UI32                                        getibgpLoadSharingValue                                 ();
                void                                        setuseLoadSharingFlag                                   (const bool &useLoadSharingFlag);
                bool                                        getuseLoadSharingFlag                                   ();
    private   :
    protected :
    public    :
        UI32                                            m_loadSharingValue;
        UI32                                            m_ebgpLoadSharingValue;
        UI32                                            m_ibgpLoadSharingValue;
        bool                                            m_useLoadSharingFlag;
};
 
}
#endif
