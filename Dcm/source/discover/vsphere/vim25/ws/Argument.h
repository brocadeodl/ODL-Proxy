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
 * Argument.h
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#ifndef ARGUMENT_H_
#define ARGUMENT_H_
#include "discover/vsphere/vim25/WSObject.h"
#include <string>
using namespace std;

namespace DcmNs {

    class Argument {
        public:
            Argument();
            virtual ~Argument();
            Argument(string name, string type, void *value);
            string getName() const;
            string getType() const;
            void* getVal() const;
            void setName(string name);
            void setType(string type);
            void setVal(void *val);
            string name;
            string type;
            void *val;
    };

} /* namespace DcmNs */
#endif /* ARGUMENT_H_ */
