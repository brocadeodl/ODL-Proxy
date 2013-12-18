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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorForWorker.h"

namespace WaveNs
{

WaveSourceGeneratorForWorker::WaveSourceGeneratorForWorker (const string &name, const string &nameSpace)
    : WaveSourceGeneratorBase (name, nameSpace)
{
}

WaveSourceGeneratorForWorker::~WaveSourceGeneratorForWorker ()
{
}

string WaveSourceGeneratorForWorker::generateClassName () const
{
    return (m_name + "Worker");
}

string WaveSourceGeneratorForWorker::generateHIncludes () const
{
    string hIncludes = "#include \"Framework/ObjectModel/WaveWorker.h\"\n"
                       "\n";

    return (hIncludes);
}

string WaveSourceGeneratorForWorker::generateHClassBegin () const
{
    string classBegin = "class " + generateClassName () + " : public WaveWorker\n"
                        "{\n";

    return (classBegin);
}

string WaveSourceGeneratorForWorker::generateHClassMemberFunctionsPublic () const
{
    string memberFunctionsPublic = "    public :\n"
                                   "                 " + generateClassName () + " (WaveObjectManager *pWaveObjectManager);\n"
                                   "        virtual ~" + generateClassName () + " ();\n";

    return (memberFunctionsPublic);
}

string WaveSourceGeneratorForWorker::generateCppMemberFunctions () const
{
    string memberFunctions;

    memberFunctions += generateClassName () + "::" + generateClassName () + " (WaveObjectManager *pWaveObjectManager)\n"
                       "    : WaveWorker (pWaveObjectManager)\n"
                       "{\n"
                       "}\n"
                       "\n";

    memberFunctions += generateClassName () + "::~" + generateClassName () + " ()\n"
                       "{\n"
                       "}\n"
                       "\n";

    return (memberFunctions);
}

}
