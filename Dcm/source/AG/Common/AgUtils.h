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
#ifndef AG_UTILS_H 
#define AG_UTILS_H 

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include <vector>
#include "AG/Common/AgPluginTypes.h"
#include "agd/public.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{
class AgNPortMapLocalManagedObject;

class AgUtils: public WaveObjectManagerToolKit
{
    private:
    public:

    AgUtils(void);

    static bool isAgModeEnabled(void);
    static vector<string> bitmap_to_vectorstring(const BitMap_t &bitmap, agInterfaceType interfaceType);
    static bool string_to_bitmap(vector<string> in_string, BitMap_t return_bit, agInterfaceType interfaceType); 
    static vector<string> add_fports_to_nport(vector<string> in_string, const string &nport_string);
    static vector<string> remove_fports_from_nport(vector<string> in_string, const string &nport_string);





    static int translateInterfacePort(string &ifname, agInterfaceType interfaceType, int &switchPort, int &mappedId);
    static int getSwitchPortFromInterfaceId(string &ifname, agInterfaceType interfaceType, int &switchPort);
    static int getInterfaceIdFromSwitchPort(int &mappedId, agInterfaceType interfaceType, int &switchPort, string &ifname);
    static bool isFCInterfaceValid(string &ifname);
    static bool isFcoeInterfaceValid(string &ifname);
    static bool isInterfaceNportLocked(string &ifname);
    static bool isInterfaceStringValid(string &ifname);
    static int  getLocalMappedId(void);
    static int  getMappedIdFromFcoeIfName(string ifname);
    static int  getPortNumFromFcoeIfName(string ifname);
    static int  stringToWwn(const char *s, wwn_t *w);
    static void removeCaseString(string str, vector<string> &str_vec);
    static bool wwnPresent (string wwn, vector<string> wwns);






};



}

#endif  //AG_UTILS_H

