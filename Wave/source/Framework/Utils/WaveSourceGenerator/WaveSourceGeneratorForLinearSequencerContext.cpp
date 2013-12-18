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

#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorForLinearSequencerContext.h"

namespace WaveNs
{

WaveSourceGeneratorForLinearSequencerContext::WaveSourceGeneratorForLinearSequencerContext (const string &name, const string &nameSpace)
    : WaveSourceGeneratorBase (name, nameSpace)
{
}

WaveSourceGeneratorForLinearSequencerContext::~WaveSourceGeneratorForLinearSequencerContext ()
{
}

string WaveSourceGeneratorForLinearSequencerContext::generateClassName () const
{
    return (m_name + "Context");
}

string WaveSourceGeneratorForLinearSequencerContext::generateHIncludes () const
{
    string hIncludes = "#include \"Framework/Utils/PrismLinearSequencerContext.h\"\n"
                       "\n";

    return (hIncludes);
}

string WaveSourceGeneratorForLinearSequencerContext::generateHClassBegin () const
{
    string classBegin = "class " + generateClassName () + " : public PrismLinearSequencerContext\n"
                        "{\n";

    return (classBegin);
}

string WaveSourceGeneratorForLinearSequencerContext::generateHClassMemberFunctionsPublic () const
{
    string memberFunctionsPublic = "    public :\n"
                                   "                 " + generateClassName () + " (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);\n"
                                   "        virtual ~" + generateClassName () + " ();\n";

    return (memberFunctionsPublic);
}

string WaveSourceGeneratorForLinearSequencerContext::generateCppMemberFunctions () const
{
    string memberFunctions;

    memberFunctions += generateClassName () + "::" + generateClassName () + " (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)\n"
                       "    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps)\n"
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
