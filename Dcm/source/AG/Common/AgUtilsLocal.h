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
 *   Author : ranantha                                                     *
 **************************************************************************/
#ifndef AG_UTILSLOCAL_H 
#define AG_UTILSLOCAL_H 

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include <vector>
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{
class AgNPortMapLocalManagedObject;

class AgUtilsLocal: public WaveObjectManagerToolKit
{
    private:
    public:

    AgUtilsLocal(void);

    static ResourceId validate_pgcreate(UI32 pgid);

    static ResourceId validate_staticmapadd(string nPort, vector<string> fPorts);
    static ResourceId validate_staticmapdel(string nPort, vector<string> fPorts);

    static ResourceId validate_mapset(string nPort, vector<string> fPorts);

    static ResourceId validate_pgadd(UI32 pgid, vector<string> nports);
    static ResourceId validate_pgdel(UI32 pgid, vector<string> nports);
    static ResourceId validate_pgaddmodes(UI32 pgid, vector<string> Modes); 
    static ResourceId validate_pgdelmodes(UI32 pgid, vector<string> Modes); 
    static ResourceId validate_pgrename(UI32 pgid, string name);
    static ResourceId validate_pgremove(UI32 pgid);

    static ResourceId validate_policy(vector<string> policy, int flag);

    static int getNumNports(void);

};



}

#endif  //AG_UTILSLOCAL_H

