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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef APPMGLOBALOBJECTMANAGER_H
#define APPMGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalConfWorker;

    class APPMGlobalObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
        protected:
        public:
            APPMGlobalObjectManager ();
			virtual    ~APPMGlobalObjectManager ();
			static APPMGlobalObjectManager *getInstance();
			static string getClassName();
			static PrismServiceId getPrismServiceId();
			bool isVlanPartOfActiveProfile(UI32 vlanId, bool &isChangeVlanList);
			bool isFcoeEnabledInDefaultProfile(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = NULL,
					PrismLinearSequencerContext *pPrismLinearSequencerContext = NULL);
			void clusterCreateCollectValidationData(WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
 			void clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

// Now the data members

        private:
            APPMGlobalConfWorker  *m_pAPPMGlobalConfWorker;

        protected:
        public:
    };

    inline  string  APPMGlobalObjectManager::getClassName()
    {
        return ("APPMGlobal");
    }

}
#endif                                            //APPMGLOBALOBJECTMANAGER_H
