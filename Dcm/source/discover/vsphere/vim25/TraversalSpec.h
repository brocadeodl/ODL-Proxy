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
 * TraversalSpec.h
 *
 *  Created on: Mar 23, 2012
 *      Author: rmadapur
 */

#ifndef TRAVERSALSPEC_H_
#define TRAVERSALSPEC_H_
#include "discover/vsphere/vim25/SelectionSpec.h"
#include <vector>
#include <string>
using namespace std;

namespace DcmNs {

    class TraversalSpec: public SelectionSpec {
        public:
            string type;
            string path;
            bool skip;
            vector<SelectionSpec> selectSet;
            TraversalSpec();
            virtual ~TraversalSpec();
            string serialize(string tagName, string _type);
    };

} /* namespace DcmNs */
#endif /* TRAVERSALSPEC_H_ */
