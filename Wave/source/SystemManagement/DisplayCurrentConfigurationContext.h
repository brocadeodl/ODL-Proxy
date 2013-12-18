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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DISPLAYCURRENTCONFIGURATIONCONTEXT_H
#define DISPLAYCURRENTCONFIGURATIONCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"

namespace WaveNs
{

class YangDisplayConfigurationContext;
class YangElement;
class WaveManagedObjectQueryContext;

class DisplayCurrentConfigurationContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                                 DisplayCurrentConfigurationContext  (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                                 ~DisplayCurrentConfigurationContext  ();

                YangDisplayConfigurationContext *getPYangDisplayConfigurationContext ();
                void                             setPYangDisplayConfigurationContext (YangDisplayConfigurationContext *pYangDisplayConfigurationContext);

                vector<YangElement *>            getYangElements                     ();
                void                             setYangElements                     (vector<YangElement *> &yangElements);

                UI32                             getCurrentYangElementIndex          () const;

                void                             advanceToNextYangElement            ();

                bool                             isAtTheLastYangElement              ();

                YangElement                     *getPYangElement                     ();
                void                             setPYangElement                     (YangElement *pYangElement);

                WaveManagedObjectQueryContext   *getPWaveManagedObjectQueryContext   ();
                void                             setPWaveManagedObjectQueryContext   (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext);

    // Now the data members

    private :
        YangDisplayConfigurationContext *m_pYangDisplayConfigurationContext;
        vector<YangElement *>            m_yangElements;
        UI32                             m_currentYangElementIndex;
        YangElement                     *m_pYangElement;
        WaveManagedObjectQueryContext   *m_pWaveManagedObjectQueryContext;

    protected :
    public :
};

}

#endif // DISPLAYCURRENTCONFIGURATIONCONTEXT_H
