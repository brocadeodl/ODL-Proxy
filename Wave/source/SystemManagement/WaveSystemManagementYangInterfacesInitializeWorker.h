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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef WAVESYSTEMMANAGEMENTYANGINTERFACESINITIALIZEWORKER_H
#define WAVESYSTEMMANAGEMENTYANGINTERFACESINITIALIZEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Modeling/YANG/ObjectModel/YangModuleCollection.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"

namespace WaveNs
{

class PrismLinearSequencerContext;
class WaveAsynchronousContextForBootPhases;
class WaveSystemManagementRebuildYangInterfacesMessage;

class WaveSystemManagementYangInterfacesInitializeWorker: public WaveWorker
{
    private :
        virtual PrismMessage   *createMessageInstance (const UI32 &operationCode);
        
                void            initialize                              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void            initializeSystemManagementInterfaceStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);

                void            rebuildSystemManagementInterfaceStep    (PrismLinearSequencerContext *pPrismLinearSequencerContext);

                void            buildSystemManagementInterface          (YangModuleCollection *yangModuleCollection, YangUserInterface *yangUserInterface, string &yinForYangUserInterface);

        virtual void            boot                                    (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    protected :
    public :
                                WaveSystemManagementYangInterfacesInitializeWorker  (WaveObjectManager *pWaveObjectManager);
        virtual                ~WaveSystemManagementYangInterfacesInitializeWorker  ();
                
                 void           rebuildYangInterfacesMessageHandler                 (WaveSystemManagementRebuildYangInterfacesMessage *pWaveSystemManagementRebuildYangInterfacesMessage);

        YangModuleCollection   *getYangModuleCollection ();
        YangUserInterface      *getYangUserInterface ();
        YangUserInterface      *getClonedYangUserInterface ();
        string                 &getYinForYangUserInterface ();

    // Now the Data Members
    private :
        YangModuleCollection    *m_pYangModuleCollection;
        YangUserInterface       *m_pYangUserInterface;
        string                   m_yinForYangUserInterface;
        bool                     m_alreadyInitialized;
    protected :
    public :
};

}

#endif // WAVESYSTEMMANAGEMENTYANGINTERFACESINITIALIZEWORKER_H
