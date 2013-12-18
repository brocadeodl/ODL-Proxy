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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVESERVERPAGEDIRECTORYENTRY_H
#define WAVESERVERPAGEDIRECTORYENTRY_H

#include "Framework/Types/Types.h"
#include <map>

using namespace std;

namespace WaveNs
{

class WaveServerPage;

class WaveServerPageDirectoryEntry
{
	private :
	protected :
	public :
		                              WaveServerPageDirectoryEntry     (const string &nodeName, WaveServerPage *pWaveServerPage);
		                             ~WaveServerPageDirectoryEntry     ();

        bool                          isAKnownImmediateSibling         (const string &immediateSiblingName);
        WaveServerPageDirectoryEntry *getImmediateSibling              (const string &immediateSiblingName);
        bool                          isAKnownSibling                  (const string &siblingName);
        WaveServerPageDirectoryEntry *getSibling                       (const string &siblingName);
		void                          addImmediateSibling              (const string &immediateSiblingName, WaveServerPage *pWaveServerPage = NULL);
        void                          addSibling                       (const string &siblingName, WaveServerPage *pWaveServerPage = NULL);
        void                          print                            (const string &prefix = "") const;
        WaveServerPage               *getWaveServerPageForRelativePath (const string &path);
        WaveServerPage               *getPWaveServerPage               ();
        void                          setPWaveServerPage               (WaveServerPage *pWaveServerPage);

	// Now the data members

	private :
		string                                       m_nodeName;
		WaveServerPage								*m_pWaveServerPage;
		map<string, WaveServerPageDirectoryEntry *>  m_siblingsMap;

	protected :
	public :
};

#endif // WAVESERVERPAGEDIRECTORYENTRY_H

}
