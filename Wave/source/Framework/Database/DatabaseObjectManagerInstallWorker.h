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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Utils/PrismAsynchronousContext.h"

namespace WaveNs
{

class PrismAsynchronousContext;
class PrismLinearSequencerContext;

class DatabaseObjectManagerInstallWorker : public WaveWorker
{
    private :
        void  install                        (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void  installValidateStep            (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void  installDatabaseStep            (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void  installBootDatabaseStep        (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void  installShutdownDatabaseStep    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void  installCreatePrismDatabaseStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);

    protected :
    public :
                 DatabaseObjectManagerInstallWorker (WaveObjectManager *pWaveObjectManager);
        virtual ~DatabaseObjectManagerInstallWorker ();

    // Now the data members

    private :
        static bool m_fullInstallRequried;

    protected :
    public :
};

}
