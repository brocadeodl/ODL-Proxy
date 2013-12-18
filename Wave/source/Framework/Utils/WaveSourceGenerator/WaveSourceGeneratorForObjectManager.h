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

#ifndef WAVESOURCEGENERATORFOROBJECTMANAGER_H
#define WAVESOURCEGENERATORFOROBJECTMANAGER_H

#include "Framework/Utils/WaveSourceGenerator/WaveSourceGeneratorBase.h"

namespace WaveNs
{

class WaveSourceGeneratorForObjectManager : public WaveSourceGeneratorBase
{
    private :
    protected :
    public :
                        WaveSourceGeneratorForObjectManager  (const string &name, const string &nameSpace, const string &m_objectManagerName);
        virtual        ~WaveSourceGeneratorForObjectManager  ();

        virtual string  generateClassName                    () const;
        virtual string  generateHIncludes                    () const;
        virtual string  generateHClassBegin                  () const;
        virtual string  generateHClassMemberFunctionsPrivate () const;
        virtual string  generateHClassMemberFunctionsPublic  () const;
        virtual string  generateCppMemberFunctions           () const;

    // Now the Data Members

    private :
        string m_objectManagerName;

    protected :
    public :
};

}

#endif // WAVESOURCEGENERATORFOROBJECTMANAGER_H
