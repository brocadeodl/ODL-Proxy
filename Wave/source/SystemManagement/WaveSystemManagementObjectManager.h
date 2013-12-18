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
 *   Author : Vidyasagara Reddy Guntaka, Jayanth Venkataraman              *
 ***************************************************************************/

#ifndef WAVESYSYEMMANAGEMENTOBJECTMANAGER_H
#define WAVESYSYEMMANAGEMENTOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Modeling/YANG/ObjectModel/YangModuleCollection.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"

namespace WaveNs
{

class WaveSystemManagementDisplayConfigurationWorker;
class WaveSystemManagementGetYangUserInterfaceWorker;
class WaveSystemManagementYangInterfacesInitializeWorker; 
class WaveSystemManagementGetPossibleCompletionsWorker;
class WaveSystemManagementGatewayWorker;

class WaveSystemManagementObjectManager : public WaveLocalObjectManager
{
    private :
             WaveSystemManagementObjectManager       ();


    protected :
    public :
        virtual                                   ~WaveSystemManagementObjectManager    ();
        static  WaveSystemManagementObjectManager *getInstance                          ();
        static  PrismServiceId                     getPrismServiceId                    ();
        static  string                             getClassName                         ();

        static  YangModuleCollection              *getYangModuleCollection              ();

        static  YangUserInterface                 *getYangUserInterface                 ();
        static  YangUserInterface                 *getClonedYangUserInterface           ();
        static  void                               getYinForYangUserInterface           (string &yinForYangUserInterface);


    // Now the data members

    private :
        WaveSystemManagementDisplayConfigurationWorker     *m_pWaveSystemManagementDisplayConfigurationWorker;
        WaveSystemManagementGetYangUserInterfaceWorker     *m_pWaveSystemManagementGetYangUserInterfaceWorker;
        WaveSystemManagementYangInterfacesInitializeWorker *m_pWaveSystemManagementYangInterfacesInitializeWorker;
        WaveSystemManagementGetPossibleCompletionsWorker   *m_pWaveSystemManagementGetPossibleCompletionsWorker;
        WaveSystemManagementGatewayWorker                  *m_pWaveSystemManagementGatewayWorker;

    protected :
    public :
};

}

#endif // WAVESYSYEMMANAGEMENTOBJECTMANAGER_H
