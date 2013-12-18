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

/**
 *   @file VcsModeLinearSequencerContext.h
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Description:    This file defines the header for the VcsModeLinearSequencerContext.
 *   Author:         Brian Adaniya
 *   Date:           12/13/2010
 */

#ifndef VCSMODELINEARSEQUENCERCONTEXT_H
#define VCSMODELINEARSEQUENCERCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsModeLinearSequencerContext : public PrismLinearSequencerContext
{
    private:
    protected:
    public:
                            VcsModeLinearSequencerContext   (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual            ~VcsModeLinearSequencerContext   ();

                bool        getVcsMode                      () const;
                void        setVcsMode                      (bool vcsMode);

                bool        getVcsClusterMode               () const;
                void        setVcsClusterMode               (bool vcsClusterMode);

    // Now the data members
    private:
                bool        m_vcsMode;
                bool        m_vcsClusterMode;

    protected:
    public:
};

}

#endif // VCSMODELINEARSEQUENCERCONTEXT_H

