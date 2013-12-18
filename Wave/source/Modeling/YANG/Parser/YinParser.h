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

#ifndef YINPARSER_H
#define YINPARSER_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class YangElement;

class YinParser
{
    private :
    protected :
    public :
                                      YinParser                ();
                                     ~YinParser                ();
        static YangElement           *parseFile                (const string &yinFileName);
        static vector<YangElement *>  parseFiles               (const vector<string> &yinFileNames);
        static vector<YangElement *>  parseDiretory            (const string &directoryPath, vector<string> &yinFileNames);
        static vector<YangElement *>  parseDirectoriesAndFiles (const vector<string> &directoryPaths, const vector<string> &yinFileNames, vector<string> &outputYinFileNames);
        static ResourceId             debugParse               (UI32 argc, vector<string> argv);

        static YangElement           *parseYangUserInterface   (const string &serializedYin);

    // now the data members

    private :
    protected :
    public :
};

}

#endif // YINPARSER_H
