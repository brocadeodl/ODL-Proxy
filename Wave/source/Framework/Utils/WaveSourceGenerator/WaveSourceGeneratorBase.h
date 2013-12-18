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

#ifndef WAVESOURCEGENERATORBASE_H
#define WAVESOURCEGENERATORBASE_H

#include "Framework/Types/Types.h"

#include <string>

using namespace std;

namespace WaveNs
{

class WaveSourceGeneratorBase
{
    private :
        static void toUpper (string &inputString);

    protected :
                       WaveSourceGeneratorBase  (const string &name, const string &nameSpace);

        virtual string generateClassName                      () const;
        virtual string generateHHeaderMacroName               () const;

        virtual string generateHCopyRightNotice               () const;
        virtual string generateHHeaderMacroDirectiveBegin     () const;
        virtual string generateHIncludes                      () const;
        virtual string generateHUsingNameSpaceDirectives      () const;
        virtual string generateHNameSpaceDirectiveBegin       () const;
        virtual string generateHClassBegin                    () const;
        virtual string generateHClassBody                     () const;
        virtual string generateHClassMemberFunctionsBegin     () const;
        virtual string generateHClassMemberFunctionsBody      () const;
        virtual string generateHClassMemberFunctionsPrivate   () const;
        virtual string generateHClassMemberFunctionsProtected () const;
        virtual string generateHClassMemberFunctionsPublic    () const;
        virtual string generateHClassMemberFunctionsEnd       () const;
        virtual string generateHClassMemberFieldsBegin        () const;
        virtual string generateHClassMemberFieldsBody         () const;
        virtual string generateHClassMemberFieldsPrivate      () const;
        virtual string generateHClassMemberFieldsProtected    () const;
        virtual string generateHClassMemberFieldsPublic       () const;
        virtual string generateHClassMemberFieldsEnd          () const;
        virtual string generateHClassEnd                      () const;
        virtual string generateHNameSpaceDirectiveEnd         () const;
        virtual string generateHHeaderMacroDirectiveEnd       () const;

        virtual string generateCppCopyRightNotice             () const;
        virtual string generateCppIncludes                    () const;
        virtual string generateCppUsingNameSpaceDirectives    () const;
        virtual string generateCppNameSpaceDirectiveBegin     () const;
        virtual string generateCppMemberFunctions             () const;
        virtual string generateCppNameSpaceDirectiveEnd       () const;

    public :
        virtual           ~WaveSourceGeneratorBase   ();

        static ResourceId  generateHSource           (UI32 argc, vector<string> argv);
        static ResourceId  generateCppSource         (UI32 argc, vector<string> argv );
        static ResourceId  generateSource            (UI32 argc, vector<string> argv );

        static void        registerDebugShellEntries ();

        virtual string     generateHSource           () const;
        virtual string     generateCppSource         () const;

    // Now the data members

    private :
    protected :
        string m_name;
        string m_nameSpace;

    public :
};

}

#endif // WAVESOURCEGENERATORBASE_H
