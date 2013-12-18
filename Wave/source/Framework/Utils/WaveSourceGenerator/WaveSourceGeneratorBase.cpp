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

#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorBase.h"
#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorForObjectManager.h"
#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorForManagedObject.h"
#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorForWorker.h"
#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorForLinearSequencerContext.h"
#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorForMessage.h"
#include "Shell/ShellDebug.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"

#include <ctype.h>

namespace WaveNs
{

WaveSourceGeneratorBase::WaveSourceGeneratorBase (const string &name, const string &nameSpace)
    : m_name     (name),
     m_nameSpace (nameSpace)
{
}

WaveSourceGeneratorBase::~WaveSourceGeneratorBase ()
{
}

void WaveSourceGeneratorBase::registerDebugShellEntries ()
{
    addDebugFunction ((ShellCmdFunction) (&WaveSourceGeneratorBase::generateHSource),   "sgenh");
    addDebugFunction ((ShellCmdFunction) (&WaveSourceGeneratorBase::generateCppSource), "sgencpp");
    addDebugFunction ((ShellCmdFunction) (&WaveSourceGeneratorBase::generateHSource),   "sgenh");
    addDebugFunction ((ShellCmdFunction) (&WaveSourceGeneratorBase::generateSource),    "sgen");

}

void WaveSourceGeneratorBase::toUpper (string &inputString)
{
    UI32 sizeOfInput = inputString.size ();
    UI32 i           = 0;

    for (i = 0; i < sizeOfInput; i++)
    {
        inputString[i] = toupper (inputString[i]);
    }
}

string WaveSourceGeneratorBase::generateClassName () const
{
    return (m_name);
}

string WaveSourceGeneratorBase::generateHHeaderMacroName () const
{
    string className = generateClassName ();

    toUpper (className);

    return (className + "_H");
}

string WaveSourceGeneratorBase::generateHCopyRightNotice () const
{
    string copyRightNotice = "/***************************************************************************\n"
                             " *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *\n"
                             " *   All rights reserved.                                                  *\n"
                             " *   Author : Vidyasagara Reddy Guntaka                                    *\n"
                             " ***************************************************************************/\n"
                             "\n";

    return (copyRightNotice);
}

string WaveSourceGeneratorBase::generateHHeaderMacroDirectiveBegin () const
{
    string headerMacroDirectiveBegin = "#ifndef " + generateHHeaderMacroName () + "\n"
                                       "#define " + generateHHeaderMacroName () + "\n"
                                       "\n";

    return (headerMacroDirectiveBegin);
}

string WaveSourceGeneratorBase::generateHIncludes () const
{
    string includes;

    return (includes);
}

string WaveSourceGeneratorBase::generateHUsingNameSpaceDirectives () const
{
    string usingNameSpaceDirectives;

    if ("WaveNs" != m_nameSpace)
    {
        usingNameSpaceDirectives += "using namespace WaveNs;\n"
                                    "\n";
    }

    return (usingNameSpaceDirectives);
}

string WaveSourceGeneratorBase::generateHNameSpaceDirectiveBegin () const
{
    string nameSpaceDirectiveBegin = "namespace " + m_nameSpace + "\n"
                                     "{\n"
                                     "\n";

    return (nameSpaceDirectiveBegin);
}

string WaveSourceGeneratorBase::generateHClassBegin () const
{
    string classBegin = "class " + generateClassName () + "\n"
                        "{\n";

    return (classBegin);
}

string WaveSourceGeneratorBase::generateHClassBody () const
{
    string classBody;

    classBody += generateHClassMemberFunctionsBegin ();
    classBody += generateHClassMemberFunctionsBody  ();
    classBody += generateHClassMemberFunctionsEnd   ();
    classBody += generateHClassMemberFieldsBegin    ();
    classBody += generateHClassMemberFieldsBody     ();
    classBody += generateHClassMemberFieldsEnd      ();

    return (classBody);
}

string WaveSourceGeneratorBase::generateHClassMemberFieldsBegin () const
{
    string memberFieldsBegin = "    // Now the Data Members\n"
                               "\n";

    return (memberFieldsBegin);
}

string WaveSourceGeneratorBase::generateHClassMemberFieldsBody () const
{
    string memberFieldsBody;

    memberFieldsBody += generateHClassMemberFieldsPrivate   ();
    memberFieldsBody += generateHClassMemberFieldsProtected ();
    memberFieldsBody += generateHClassMemberFieldsPublic    ();

    return (memberFieldsBody);
}

string WaveSourceGeneratorBase::generateHClassMemberFieldsPrivate () const
{
    string memberFieldsPrivate = "    private :\n";

    return (memberFieldsPrivate);
}

string WaveSourceGeneratorBase::generateHClassMemberFieldsProtected () const
{
    string memberFieldsProtected = "    protected :\n";

    return (memberFieldsProtected);
}

string WaveSourceGeneratorBase::generateHClassMemberFieldsPublic () const
{
    string memberFieldsPublic = "    public :\n";

    return (memberFieldsPublic);
}

string WaveSourceGeneratorBase::generateHClassMemberFieldsEnd () const
{
    string memberFieldsEnd;

    return (memberFieldsEnd);
}

string WaveSourceGeneratorBase::generateHClassMemberFunctionsBegin () const
{
    string memberFunctionsBegin;

    return (memberFunctionsBegin);
}

string WaveSourceGeneratorBase::generateHClassMemberFunctionsBody () const
{
    string memberFunctionsBody;

    memberFunctionsBody += generateHClassMemberFunctionsPrivate   ();
    memberFunctionsBody += generateHClassMemberFunctionsProtected ();
    memberFunctionsBody += generateHClassMemberFunctionsPublic    ();

    return (memberFunctionsBody);
}

string WaveSourceGeneratorBase::generateHClassMemberFunctionsPrivate () const
{
    string memberFunctionsPrivate = "    private :\n";

    return (memberFunctionsPrivate);
}

string WaveSourceGeneratorBase::generateHClassMemberFunctionsProtected () const
{
    string memberFunctionsProtected = "    protected :\n";

    return (memberFunctionsProtected);
}

string WaveSourceGeneratorBase::generateHClassMemberFunctionsPublic () const
{
    string memberFunctionsPublic = "    public :\n"
                                   "          " + generateClassName () + " ();\n"
                                   "         ~" + generateClassName () + " ();\n";

    return (memberFunctionsPublic);
}

string WaveSourceGeneratorBase::generateHClassMemberFunctionsEnd () const
{
    string memberFunctionsEnd = "\n";

    return (memberFunctionsEnd);
}

string WaveSourceGeneratorBase::generateHClassEnd () const
{
    string classEnd = "};\n"
                      "\n";

    return (classEnd);
}

string WaveSourceGeneratorBase::generateHNameSpaceDirectiveEnd () const
{
    string nameSpaceDirectiveEnd = "}\n"
                                   "\n";

    return (nameSpaceDirectiveEnd);
}

string WaveSourceGeneratorBase::generateHHeaderMacroDirectiveEnd () const
{
    string headerMacroDirectiveEnd = "#endif // " + generateHHeaderMacroName () + "\n";

    return (headerMacroDirectiveEnd);
}

string WaveSourceGeneratorBase::generateCppCopyRightNotice () const
{
    return (generateHCopyRightNotice ());
}

string WaveSourceGeneratorBase::generateCppIncludes () const
{
    string cppIncludes = "#include \"" + generateClassName () + ".h\"\n"
                         "\n";

    return (cppIncludes);
}

string WaveSourceGeneratorBase::generateCppUsingNameSpaceDirectives () const
{
    string cppUsingNameSpaceDirectives;

    return (cppUsingNameSpaceDirectives);
}

string WaveSourceGeneratorBase::generateCppNameSpaceDirectiveBegin () const
{
    return (generateHNameSpaceDirectiveBegin ());
}

string WaveSourceGeneratorBase::generateCppMemberFunctions () const
{
    string memberFunctions;

    memberFunctions += generateClassName () + "::" + generateClassName () + " ()\n"
                       "{\n"
                       "}\n"
                       "\n";

    memberFunctions += generateClassName () + "::~" + generateClassName () + " ()\n"
                       "{\n"
                       "}\n"
                       "\n";

    return (memberFunctions);
}

string WaveSourceGeneratorBase::generateCppNameSpaceDirectiveEnd () const
{
    return (generateHNameSpaceDirectiveEnd ());
}

string WaveSourceGeneratorBase::generateHSource () const
{
    string hSource;

    hSource += generateHCopyRightNotice           ();
    hSource += generateHHeaderMacroDirectiveBegin ();
    hSource += generateHIncludes                  ();
    hSource += generateHUsingNameSpaceDirectives  ();
    hSource += generateHNameSpaceDirectiveBegin   ();
    hSource += generateHClassBegin                ();
    hSource += generateHClassBody                 ();
    hSource += generateHClassEnd                  ();
    hSource += generateHNameSpaceDirectiveEnd     ();
    hSource += generateHHeaderMacroDirectiveEnd   ();

    return (hSource);
}

string WaveSourceGeneratorBase::generateCppSource () const
{
    string cppSource;

    cppSource += generateCppCopyRightNotice          ();
    cppSource += generateCppIncludes                 ();
    cppSource += generateCppUsingNameSpaceDirectives ();
    cppSource += generateCppNameSpaceDirectiveBegin  ();
    cppSource += generateCppMemberFunctions          ();
    cppSource += generateCppNameSpaceDirectiveEnd    ();

    return (cppSource);
}

ResourceId WaveSourceGeneratorBase::generateHSource (UI32 argc, vector<string> argv)
{
    WaveSourceGeneratorBase waveSourceGeneratorBase (argv[1], argv[2]);
    string                  hSource                 = waveSourceGeneratorBase.generateHSource ();

    cout << hSource;

    return (0);
}

ResourceId WaveSourceGeneratorBase::generateCppSource (UI32 argc, vector<string> argv)
{
    WaveSourceGeneratorBase waveSourceGeneratorBase (argv[1], argv[2]);
    string                  cppSource               = waveSourceGeneratorBase.generateCppSource ();

    cout << cppSource;

    return (0);
}

ResourceId WaveSourceGeneratorBase::generateSource (UI32 argc, vector<string> argv)
{
    WaveSourceGeneratorBase *pWaveSourceGeneratorBase = NULL;
    string                   source;

    if (argv[1] == "-om")
    {
        pWaveSourceGeneratorBase = new WaveSourceGeneratorForObjectManager (argv[3], argv[4], argv[5]);
    }
    else if (argv[1] == "-mo")
    {
        pWaveSourceGeneratorBase = new WaveSourceGeneratorForManagedObject (argv[3], argv[4]);
    }
    else if (argv[1] == "-wo")
    {
        pWaveSourceGeneratorBase = new WaveSourceGeneratorForWorker (argv[3], argv[4]);
    }
    else if (argv[1] == "-lsc")
    {
        pWaveSourceGeneratorBase = new WaveSourceGeneratorForLinearSequencerContext (argv[3], argv[4]);
    }
    else if (argv[1] == "-cl")
    {
        pWaveSourceGeneratorBase = new WaveSourceGeneratorBase (argv[3], argv[4]);
    }
    else if (argv[1] == "-msg")
    {
        pWaveSourceGeneratorBase = new WaveSourceGeneratorForMessage (argv[3], argv[4]);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveSourceGeneratorBase::generateCppSource : Unknown Object Type given to generate source.");
        return (0);
    }

    prismAssert (NULL != pWaveSourceGeneratorBase, __FILE__, __LINE__);

    if (argv[2] == "-h")
    {
        source = pWaveSourceGeneratorBase->generateHSource ();
    }
    else if (argv[2] == "-cpp")
    {
        source = pWaveSourceGeneratorBase->generateCppSource ();
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveSourceGeneratorBase::generateCppSource : Unknown source Type given to generate source.");
        delete pWaveSourceGeneratorBase;
        pWaveSourceGeneratorBase = NULL;
        return (0);
    }
    cout << source;

    delete pWaveSourceGeneratorBase;
    pWaveSourceGeneratorBase = NULL;
    return (0);
}

}
