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

/*
 * HistoryCollector.h
 *
 *  Created on: Jul 4, 2012
 *      Author: rmadapur
 */

#ifndef HISTORYCOLLECTOR_H_
#define HISTORYCOLLECTOR_H_

#include "discover/vsphere/vim25/mo/ManagedObject.h"
#include "discover/vsphere/vim25/ServiceContent.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/ws/vimStub.h"
namespace DcmNs {

    class HistoryCollector : public ManagedObject{
        public:
            HistoryCollector(vimStub *_stub,ManagedObjectReference *mor);
            virtual ~HistoryCollector();

            int destroyCollector();

            int resetCollector();

            int rewindCollector();

            int setCollectorPageSize(int maxCount);



    };

} /* namespace DcmNs */
#endif /* HISTORYCOLLECTOR_H_ */
