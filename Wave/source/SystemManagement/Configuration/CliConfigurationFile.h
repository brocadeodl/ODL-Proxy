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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef CLICONFIGURATIONFILE_H
#define CLICONFIGURATIONFILE_H

#include "Framework/Types/Types.h"
#include "SystemManagement/Configuration/ConfigurationFile.h"
#include "SystemManagement/Configuration/CliNode.h"
#include "Framework/Types/UI32Range.h"
#include <string>
#include <vector>
#include <utility>

using namespace std;

namespace WaveNs
{

class CliConfigurationFile : public ConfigurationFile
{
    private :
    protected :
    public :
                      CliConfigurationFile                  ();
                      CliConfigurationFile                  (const string &cliConfigurationFilePath);
        virtual      ~CliConfigurationFile                  ();

        virtual void  load                                  ();
        virtual void  reload                                ();
        virtual void  reload                                (const string &cliConfigurationFilePath);
        virtual bool  validate                              () const;
        virtual void  print                                 () const;

        static  UI32  debugLoad                             (UI32 argc, vector<string> argv);
    
                void  getChildrenCliNodeNamesForCommandLine (const string &commandLine, vector<string> &childrenCliNodeNames);
                void  splitConfigForNonBulking              (vector<pair<string, bool> > &configFiles);
        // Now the data members

    private :
        CliNode                         m_cliNodeTree;
        vector<string>                  m_cliLines;
        vector<pair<UI32Range, bool> >  m_configFileBulkRanges; // The bool is set to true if Bulking is enabled

    protected :
    public :
};

}

#endif // CLICONFIGURATIONFILE_H
