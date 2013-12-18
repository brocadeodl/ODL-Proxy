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

#ifndef YANGELEMENTFACTORY_H
#define YANGELEMENTFACTORY_H

#include <string>
#include <map>

using namespace std;

namespace WaveNs
{

class YangElement;

typedef YangElement * (*YangElementInstantiator) ();

class YangElementFactory
{
    private :
    protected :
    public :
        static bool         isAKnownYangName           (const string &yangName);
        static void         addYangElementInstantiator (const string &yangName, YangElementInstantiator yangElementInstantiator);
        static YangElement *createYangElement          (const string &yangName);

        static void         initialize                 ();

    // Now the data members

    private :
        static map<string, YangElementInstantiator> m_yangElementInstantiatorMap;

    protected :
    public :
};

}

#endif // YANGELEMENTFACTORY_H
