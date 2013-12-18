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

/* **
 * UidToolKit.h
 *
 * Jul 09, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef UIDTOOLKIT_H_
#define UIDTOOLKIT_H_

#include <string>
#include "Framework/Types/Types.h"
#include "UidService/Global/UidServiceTypes.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs {

class UidServiceMessage;


class UidToolKit {
public:
    static ResourceId alloc(UI32 &o_uid, const UI32 idType, const string &name);

    //static ResourceId free(const UI32 idType, const UI32 id);
    static ResourceId free(const UI32 idType, const string &name);
    static ResourceId freeAll(const UI32 idType);
    static ResourceId freeAll(const string &entityName);
    static ResourceId freeAll();

    static ResourceId remove(const UI32 idType, const string &name);
    static ResourceId removeAll(const UI32 idType);
    static ResourceId removeAll(const string &name);
    static ResourceId removeAll();

    static ResourceId set(const UI32 idType, const UI32 id, const string& name);

    static ResourceId invoke(UidServiceMessage &msg);
    static ResourceId invoke(const UI32 opcode, const UI32 idType, const UI32 id, const string &name);

private:
    UidToolKit();

};

}

#endif /* UIDTOOLKIT_H_ */
