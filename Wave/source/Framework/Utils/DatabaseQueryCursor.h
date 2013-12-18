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

#ifndef DATABASEQUERYCURSOR_H
#define DATABASEQUERYCURSOR_H

#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Attributes/Attributes.h"

#include <string>
#include <vector>
#include <map>

using namespace std;
namespace WaveNs
{

class DatabaseQueryCursor
{
   public :

	DatabaseQueryCursor(WaveObjectManager* callingOm, const string &className, const string& queryString, UI32 pageSize = 23, UI32 startOffset = 0);
	~DatabaseQueryCursor();
	
	void setPageSize(UI32 pageSize);
	UI32 getPageSize() const;
	
	void setStartOffset(UI32 startOffset);
	UI32 getStartOffset() const;
	
//	void setCursorName(string cursorName); //This function may be removed
	string getCursorName() const;

	static string getUniqueCursorName();

	int declareCursor();
	int closeCursor();
	void fetchCursor();
	string getSqlOfFetchCursor();
	int moveCursor(unsigned int count = 23);


   private :
	string			m_cursorName;
	string			m_queryString;
	UI32				m_startOffset;
	UI32				m_pageSize;
	UI8				m_cursorState;
	WaveObjectManager* m_CallingOm;
	string             m_className;
	enum e_cursorStates
	{
		CURSOR_NOT_DECLARED,
		CURSOR_DECLARED,
		CURSOR_CLOSED
	};
};
}

#endif //DATABASEQUERYCURSOR_H
