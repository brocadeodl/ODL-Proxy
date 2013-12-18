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
 * ManagedObjectReference.h
 *
 *  Created on: Mar 21, 2012
 *      Author: rmadapur
 */

#ifndef MANAGEDOBJECTREFERENCE_H_
#define MANAGEDOBJECTREFERENCE_H_
#include <string>
#include "discover/vsphere/Debug.h"
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include "discover/vsphere/vim25/WSObject.h"
using namespace std;
namespace DcmNs {

    /**
     * Class used to refer to the server side ManagedObject.
     */
    class ManagedObjectReference: public WSObject {
        public:
            ManagedObjectReference();
            ManagedObjectReference(const ManagedObjectReference &ref);
            void operator=(
                    const ManagedObjectReference &ref);
            virtual ~ManagedObjectReference();
            string getType() const;
            string getVal() const;
            void setType(string type);
            void setVal(string val);
            void deSerialize(xmlNodePtr node);
            string serialize(string tagName, string type);

            /**
             * The name of the Managed Object type this ManagedObjectReference refers to
             */
            string val;
            /**
             * The specific instance of Managed Object this ManagedObjectReference refers to.
             */
            string type;
    };

} /* namespace DcmNs */
#endif /* MANAGEDOBJECTREFERENCE_H_ */
