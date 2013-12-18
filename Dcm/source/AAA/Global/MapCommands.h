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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kghanta                                                     *
 **************************************************************************/

#ifndef MAPCOMMANDS_H
#define MAPCOMMANDS_H

#include "DcmObjectModel/DcmManagedObject.h"

#include "AAAGlobalTypes.h"
using namespace WaveNs;
#include <string>
namespace DcmNs
{
    class MapCommands 
    {

		protected:

		public:
			static void setDataRuleCmd(const int &cmdKey, const vector<string> &cmd);
			static vector<string> getDataRuleCmd(int &cmdKey)  ;
			static void setCommandRuleCmd(const int &cmdKey, const vector<string> &cmd);
			static vector<string> getCommandRuleCmd(int &cmdKey);
			//static void setShowCmd(const int &cmdKey, const string &cmd);
			//static string getShowCmd(int &cmdKey);
			static map< int, vector<string> > getCommandRuleMap();
			static void setupCmdMaps();
			static vector<string> getDefaultCmds();
			static void setupDefaultCmds();
			static void unsetDefaultCmds();
// Now the data members
		private:
			static map< int, vector<string> > cmdRuleCmds;
			static map< int, vector<string> > dataRuleCmds;
			//static map<int, string> showCmds;
			static vector<string> defaultCmds;
	};
}
#endif                                            //MAPCOMMANDS_H
