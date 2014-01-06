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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSNODELOCALEVENTTESTOBJECTMANAGER_H
#define VCSNODELOCALEVENTTESTOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{
class VcsNodeDomainValidEvent;
class VcsNodeDomainCapabilitiesAvailableEvent;
class VcsNodeDomainInvalidEvent;
class VcsNodeDomainReachableEvent;
class VcsNodeDomainUnreachableEvent;

class VcsNodeLocalEventTestObjectManager : public WaveObjectManager
{
    private :
                      VcsNodeLocalEventTestObjectManager ();

        void          listenForEvents                    (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void          domainValidEventHandler                         (const VcsNodeDomainValidEvent *&pEvent);
        void          domainCapabilitiesAvailableEventHandler         (const VcsNodeDomainCapabilitiesAvailableEvent *&pEvent);
        void          domainInvalidEventHandler                       (const VcsNodeDomainInvalidEvent *&pEvent);
        void          domainReachableEventHandler                       (const VcsNodeDomainReachableEvent *&pEvent);
        void          domainUnreachableEventHandler                       (const VcsNodeDomainUnreachableEvent *&pEvent);

    protected :
    public :
        virtual                                    ~VcsNodeLocalEventTestObjectManager                            ();
        static  VcsNodeLocalEventTestObjectManager* getInstance                                                   ();
        static  PrismServiceId                      getPrismServiceId                                             ();

    // Now the data members

     private :
     protected :
     public :
};

}

#endif // VCSNODELOCALEVENTTESTOBJECTMANAGER_H