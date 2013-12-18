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
 * UpdateSet.h
 *
 *  Created on: Apr 9, 2012
 *      Author: rmadapur
 */

#ifndef UPDATESET_H_
#define UPDATESET_H_
#include "discover/vsphere/vim25/WSObject.h"
#include <string>
#include <vector>
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/ListUtil.h"
#include "discover/vsphere/vim25/Event.h"
using namespace std;
namespace DcmNs {

    class PropertyChangeOp: public WSObject {

        public:
            string val;
            PropertyChangeOp();
            virtual ~PropertyChangeOp();
            void deSerialize(xmlNodePtr node);
    };
    class ObjectUpdateKind: public WSObject {

        public:
            string val;
            ObjectUpdateKind();
            virtual ~ObjectUpdateKind();
            void deSerialize(xmlNodePtr node);
    };

    class PropertyChange: public WSObject {

        public:
            string name;
            PropertyChangeOp op;
            string valType;
            void *val;
            PropertyChange();
            virtual ~PropertyChange();
            void deSerialize(xmlNodePtr node);
    };

    class ObjectUpdate: public WSObject {

        public:
            ObjectUpdateKind kind;
            ManagedObjectReference obj;
            vector<PropertyChange *> * changeSet;

            ObjectUpdate();
            virtual ~ObjectUpdate();
            void deSerialize(xmlNodePtr node);
    };

    class PropertyFilterUpdate: public WSObject {

        public:
            ManagedObjectReference filter;

            vector<ObjectUpdate *> *objectSet;
            PropertyFilterUpdate();
            virtual ~PropertyFilterUpdate();
            void deSerialize(xmlNodePtr node);
    };

    class UpdateSet: public WSObject {

        public:
            string version;
            vector<PropertyFilterUpdate*> *filterSet;
            bool truncated;
            UpdateSet();
            virtual ~UpdateSet();
            void deSerialize(xmlNodePtr node);
    };

} /* namespace DcmNs */
#endif /* UPDATESET_H_ */
