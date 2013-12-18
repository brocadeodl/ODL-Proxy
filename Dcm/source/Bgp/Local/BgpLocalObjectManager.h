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


#ifndef BGPLOCALOBJECTMANAGER_H
#define BGPLOCALOBJECTMANAGER_H



#include "Framework/ObjectModel/WaveLocalObjectManager.h"


using namespace WaveNs;


namespace DcmNs{

class BgpLocalBasicConfigWorker;
class BgpLocalIpv4Worker;

class BgpLocalObjectManager : public WaveLocalObjectManager
{
    private   :
                PrismMessage                                *createMessageInstance                                  (const UI32 &operationCode);
                WaveManagedObject                           *createManagedObjectInstance                            (const string &managedClassName);
                                                            BgpLocalObjectManager                                   ();
    protected :
    public    :
                virtual                                     ~BgpLocalObjectManager                                  ();
                static BgpLocalObjectManager                *getInstance                                            ();
                static PrismServiceId                       getPrismServiceId                                       ();
                static string                               getClassName                                            ();
                virtual void                                setPostbootPhases                                       ();
				ResourceId                                  postbootValidate                            (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);
    private   :
    protected :
    public    :
        BgpLocalBasicConfigWorker                       *m_pBgpLocalBasicConfigWorker;
        BgpLocalIpv4Worker                              *m_pBgpLocalIpv4Worker;
};
 
}
#endif
