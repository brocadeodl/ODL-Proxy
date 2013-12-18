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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Modeling/YANG/ObjectModel/YangRpc.h"

namespace WaveNs
{

YangRpc::YangRpc ()
    : YangElement (getYangName (), "")
{
}

YangRpc::~YangRpc()
{
}

string YangRpc::getYangName ()
{
    return ("rpc");
}

YangElement *YangRpc::createInstance ()
{
    return (new YangRpc ());
}

void YangRpc::getAllCliTargetNodeNamesForData (vector<string> &allCliTargetNodeNamesForData) const
{
}

void YangRpc::computeCliTargetNodeNameMapForData (map<string, vector<YangElement *> > &cliTargetNodeNameMapForData) const
{
}

void YangRpc::computeWillBeShownInConfigurationTree ()
{
}

bool YangRpc::considerForComputingUsageCountForGroupingsForProgrammingLanguages () const
{
    return (false);
}

bool YangRpc::considerForHFileForCLanguage () const
{
    return (true);
}

}
