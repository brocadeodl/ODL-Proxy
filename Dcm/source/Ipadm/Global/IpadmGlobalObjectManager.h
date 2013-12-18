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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/ 
#ifndef IPADMGLOBALOBJECTMANAGER_H
#define IPADMGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Boot/BootTypes.h"

#include "Ipadm/Global/IpadmDnsConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "Ipadm/Local/IpadmLocalManagedObject.h"
#include "Ipadm/Global/IpadmGlobalManagedObject.h"
#include "Ipadm/Global/IpadmMessagingContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "Ipadm/Local/IpadmLocalDnsCfgMessage.h"

using namespace WaveNs;

namespace DcmNs
{
class IpadmLocalObjectManager;
class IpadmGlobalDNManagedObject;
class IpadmGlobalNSManagedObject;
class VcsNodeLocalManagedObject;
class IpadmLocalDnsCfgMessage;

class IpadmGlobalObjectManager : public WaveObjectManager
{
    private:
                IpadmGlobalObjectManager         ();
               ~IpadmGlobalObjectManager         ();
        	virtual  PrismMessage *createMessageInstance (const UI32 &operationCode);
        	virtual  WaveManagedObject    *createManagedObjectInstance (const string &managedClassName);
        	vector<WaveManagedObject *> *DoSynchronousQuery  (const string className);
		void install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        void    failover                         (FailoverAsynchronousContext *pFailoverAsynchronousContext);
 
                
    protected:

    public:
                static   IpadmGlobalObjectManager *getInstance       ();
                static   string                           getClassName      ();
                static   PrismServiceId                   getPrismServiceId ();

        void    IpadmClusterConfigMsgHandler (IpadmDnsConfigMessage *pIpadmDnsConfigMessage);
        void    cfgMessageHandler(IpadmMessagingContext *pIpadmMessagingContext);
        void    cfgMessageCallback (FrameworkStatus frameworkStatus, IpadmLocalDnsCfgMessage *pMessage, IpadmMessagingContext *pIpadmMessagingContext);

    // Datamember
    private :
    protected :
    public :
};

}

#endif
