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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef SOFTWAREMANAGEMENTVERSIONWORKER_H
#define SOFTWAREMANAGEMENTVERSIONWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class SoftwareManagementObjectManager;
class SoftwareManagementGetVersionMessage;

class SoftwareManagementVersionWorker : public WaveWorker
{
    private :
        virtual PrismMessage *createMessageInstance                      (const UI32 &operationCode);

                void          softwareManagementGetVersionMessageHandler (SoftwareManagementGetVersionMessage *psoftwareManagementGetVersionMessage);
                void          getVersionStep                             (PrismLinearSequencerContext *pPrismLinearSequencerContext);

        static  void          debugGetVersion                            (UI32 argc, vector<string> argv);

    protected :
    public :
                 SoftwareManagementVersionWorker (SoftwareManagementObjectManager *pSoftwareManagementObjectManager);
        virtual ~SoftwareManagementVersionWorker ();

    // now the data members

    private :
    protected :
    public :
};

}

#endif // SOFTWAREMANAGEMENTVERSIONWORKER_H
