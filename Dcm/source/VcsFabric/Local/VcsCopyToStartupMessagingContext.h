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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef VCSCOPYTOSTARTUPMESSAGINGCONTEXT_H
#define VCSCOPYTOSTARTUPMESSAGINGCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Attributes/Attributes.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsCopyToStartupMessagingContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
            VcsCopyToStartupMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);

           ~VcsCopyToStartupMessagingContext ();

            UI32            getContextInfo                                                                      () const;
            void            setContextInfo                                                                      (const UI32 contextInfo);

            ResourceId      getStatusForCopyRunningToStartupOnActive                                            () const;
            void            setStatusForCopyRunningToStartupOnActive                                            (const ResourceId &status);

            ResourceId      getStatusForCopyRunningToStartupOnStandby                                           () const;
            void            setStatusForCopyRunningToStartupOnStandby                                           (const ResourceId &status);

    // Now the data members

    private :
        UI32                    m_contextInfo;
        ResourceId              m_statusForCopyRunningToStartupOnActive;
        ResourceId              m_statusForCopyRunningToStartupOnStandby;

    protected :
    public :
};

}

#endif // VCSCOPYTOSTARTUPMESSAGINGCONTEXT_H
