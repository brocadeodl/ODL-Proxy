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

#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorForObjectManager.h"

namespace WaveNs
{

WaveSourceGeneratorForObjectManager::WaveSourceGeneratorForObjectManager (const string &name, const string &nameSpace, const string &objectManagerName)
    : WaveSourceGeneratorBase (name, nameSpace),
      m_objectManagerName     (objectManagerName)
{
}

WaveSourceGeneratorForObjectManager::~WaveSourceGeneratorForObjectManager ()
{
}

string WaveSourceGeneratorForObjectManager::generateClassName () const
{
    return (m_name + "ObjectManager");
}

string WaveSourceGeneratorForObjectManager::generateHIncludes () const
{
    string hIncludes = "#include \"Framework/ObjectModel/WaveObjectManager.h\"\n"
                       "\n";

    return (hIncludes);
}

string WaveSourceGeneratorForObjectManager::generateHClassBegin () const
{
    string classBegin = "class " + generateClassName () + " : public WaveObjectManager\n"
                        "{\n";

    return (classBegin);
}

string WaveSourceGeneratorForObjectManager::generateHClassMemberFunctionsPrivate () const
{
    string memberFunctionsPublic = "    private :\n"
                                   "                 " + generateClassName () + " ();\n"
                                   "\n";

    return (memberFunctionsPublic);
}

string WaveSourceGeneratorForObjectManager::generateHClassMemberFunctionsPublic () const
{
    string memberFunctionsPublic = "    public :\n"
                                   "        virtual ~" + generateClassName () + " ();\n"
                                   "\n"
                                   "        static " + generateClassName () + " *getInstance ();\n"
                                   "        static PrismServiceId getPrismServiceId ();\n";

    return (memberFunctionsPublic);
}

string WaveSourceGeneratorForObjectManager::generateCppMemberFunctions () const
{
    string memberFunctions;

    memberFunctions += generateClassName () + "::" + generateClassName () + " ()\n"
                       "    : WaveObjectManager (\"" + m_objectManagerName + "\")\n"
                       "{\n"
                       "}\n"
                       "\n";

    memberFunctions += generateClassName () + "::~" + generateClassName () + " ()\n"
                       "{\n"
                       "}\n"
                       "\n";

    memberFunctions += generateClassName () + " *" + generateClassName () + ":: getInstance ()\n"
                       "{\n"
                       "    static " + generateClassName () + " *p" + generateClassName () + " = new " + generateClassName () + " ();\n"
                       "\n"
                       "    WaveNs::prismAssert (NULL != p" + generateClassName () + ", __FILE__, __LINE__);\n"
                       "\n"
                       "    return (p" + generateClassName () + ");\n"
                       "}\n"
                       "\n";

    memberFunctions += "PrismServiceId " + generateClassName () + ":: getPrismServiceId ()\n"
                       "{\n"
                       "    return ((getInstance ())->getServiceId ());\n"
                       "}\n"
                       "\n";

    return (memberFunctions);
}

}
