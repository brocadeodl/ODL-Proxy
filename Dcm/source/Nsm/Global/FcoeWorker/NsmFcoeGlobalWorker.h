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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef NSMFCOEGLOBALWORKER_H
#define NSMFCOEGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/FcoeWorker/FcoeLocalFcoeportAttrMessage.h"
#include "Nsm/Global/FcoeWorker/FcoeportAttrGlobalManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Fcoe/Global/FcoeMapManagedObject.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"
#include "Fcoe/Common/FcoeCommonWorker.h"


using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class FcoeGlobalFcoeportAttrMessage;

    class NsmFcoeGlobalWorker: public FcoeCommonWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void FcoeGlobalFcoeportAttrMessageHandler(FcoeGlobalFcoeportAttrMessage *pFcoeGlobalFcoeportAttrMessage);
            
            void FcoeGlobalFcoeportAttrUpdateConfig (FcoeLinearSequencerContext *ctx_p);
            void FcoeGlobalFcoeportAttrSendToLocal (FcoeLinearSequencerContext *ctx_p);

            FcoeportAttrGlobalManagedObject* FcoeportGetChildMoByParentMo(PoIntfManagedObject *if_mo_p,
                                                                          FcoeLinearSequencerContext *ctx_p);

            PoIntfManagedObject* GetPoIntfMoById (string &if_name,
                                                  FcoeLinearSequencerContext *ctx_p = NULL);

            ObjectId FcoeMapGetObjectIdByName (string &map_name,
                                               FcoeLinearSequencerContext *ctx_p = NULL);

            FcoeMapManagedObject* FcoeMapGetMoByName (string &map_name,
                                                      FcoeLinearSequencerContext *ctx_p = NULL);

            void NsmGlobalFcoeportAttrSendToClusterCallback (PrismAsynchronousContext *ctx_p);

        protected:
        public:
            NsmFcoeGlobalWorker ( NsmGlobalObjectManager *pFcoeGlobalObjectManager);
            virtual    ~NsmFcoeGlobalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //NSMFCOEGLOBALWORKER_H
