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

#ifndef DATABASEOBJECTMANAGERBOOTWORKER_H
#define DATABASEOBJECTMANAGERBOOTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

class WaveAsynchronousContextForBootPhases;

namespace WaveNs
{

class DatabaseObjectManagerBootWorker : public WaveWorker
{
    private :
        void  boot                      (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void  bootValidateStep          (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void  bootDatabaseStep          (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void  bootConnectToDatabaseStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void  bootReconnectToDatabaseOnInstallIfRequiredStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);

    protected :
    public :
                 DatabaseObjectManagerBootWorker (WaveObjectManager *pWaveObjectManager);
        virtual ~DatabaseObjectManagerBootWorker ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // DATABASEOBJECTMANAGERBOOTWORKER_H
