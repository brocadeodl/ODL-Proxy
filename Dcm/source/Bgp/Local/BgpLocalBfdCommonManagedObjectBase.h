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

#if 0
#ifndef BGPLOCALBFDCOMMONMANAGEDOBJECTBASE_H
#define BGPLOCALBFDCOMMONMANAGEDOBJECTBASE_H


#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Attributes/Attributes.h"



using namespace WaveNs;


namespace DcmNs{


class BgpLocalBfdCommonManagedObjectBase : virtual public CommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalBfdCommonManagedObjectBase                      (PrismPersistableObject *pPrismPersistableObject );
                static string                               getClassName                                            ();
                virtual                                     ~BgpLocalBfdCommonManagedObjectBase                     ();
                void                                        setbfdHoldInterval                                      (const UI32 &bfdHoldInterval);
                UI32                                        getbfdHoldInterval                                      ();
                void                                        setbfdMintx                                             (const UI32 &bfdMintx);
                UI32                                        getbfdMintx                                             ();
                void                                        setbfdMinrx                                             (const UI32 &bfdMinrx);
                UI32                                        getbfdMinrx                                             ();
                void                                        setbfdMultiplier                                        (const UI32 &bfdMultiplier);
                UI32                                        getbfdMultiplier                                        ();
    private   :
    protected :
    public    :
        UI32                                            m_bfdHoldInterval;
        UI32                                            m_bfdMintx;
        UI32                                            m_bfdMinrx;
        UI32                                            m_bfdMultiplier;
};
 
}
#endif
