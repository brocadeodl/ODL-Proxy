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


#include "Framework/ObjectModel/WaveManagedObject.h"

#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/DatabaseQueryCursor.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"

#include "Framework/ObjectRelationalMapping/OrmRepository.h"

#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerExecuteCursorCommandMessage.h"

namespace WaveNs
{

static PrismMutex s_databaseCursorCounterMutex;


DatabaseQueryCursor::DatabaseQueryCursor(WaveObjectManager* callingOm, const string &className, const string& queryString, UI32 pageSize, UI32 startOffset)
: m_CallingOm (callingOm),
  m_className (className)
{
	m_cursorName = DatabaseQueryCursor::getUniqueCursorName();
	m_queryString = queryString;
	m_pageSize = pageSize;
	m_startOffset = startOffset;
	m_cursorState = CURSOR_NOT_DECLARED;

	if (WAVE_MESSAGE_SUCCESS == declareCursor())
	{
		m_cursorState = CURSOR_DECLARED;
	}
}

DatabaseQueryCursor::~DatabaseQueryCursor()
{
	if (CURSOR_DECLARED == m_cursorState)
	{
		closeCursor();
	}
}

void DatabaseQueryCursor::setPageSize(UI32 pageSize)
{
	m_pageSize = pageSize;
}
		
UI32 DatabaseQueryCursor::getPageSize() const
{
	return (m_pageSize);
}


void DatabaseQueryCursor::setStartOffset(UI32 startOffset)
{
	m_startOffset = startOffset;
}

UI32 DatabaseQueryCursor::getStartOffset() const
{
	return (m_startOffset);
}

#if 0
void DatabaseQueryCursor::setCursorName(string cursorName)
{
	m_cursorName = cursorName;
}
#endif

string DatabaseQueryCursor::getCursorName() const
{
	return (m_cursorName);
}

string DatabaseQueryCursor::getUniqueCursorName()
{
	static unsigned int count = 0;
	unsigned int index=0;

	s_databaseCursorCounterMutex.lock();
	index = ++count;	
	s_databaseCursorCounterMutex.unlock();

	return (string("cursor") + index);
	
}


int DatabaseQueryCursor::declareCursor()
{
	string dbCommand;

	string declareCommand = string("DECLARE ") + m_cursorName + " NO SCROLL CURSOR " + " WITH HOLD FOR "  + m_queryString + string(" ; "); 
	dbCommand = " BEGIN; " + declareCommand + string(" COMMIT ");

	DatabaseObjectManagerExecuteCursorCfgCommandMessage  cursorCfgCommandMessage(CURSOR_DECLARE, dbCommand, m_className);
	
	ResourceId	completionStatus = WAVE_MESSAGE_SUCCESS;
	
	WaveMessageStatus status = m_CallingOm->sendSynchronously (&cursorCfgCommandMessage);

	if (WAVE_MESSAGE_SUCCESS != status)
	{
	    trace (TRACE_LEVEL_ERROR, "DatabaseQueryCursor::declareCursor : Could not send a message to Database Service.  Status : " + FrameworkToolKit::localize (status));

	}
	else
	{
	    completionStatus = cursorCfgCommandMessage.getCompletionStatus ();

	    if (WAVE_MESSAGE_SUCCESS != completionStatus)
	    {
	        trace (TRACE_LEVEL_ERROR, "DatabaseQueryCursor::declareCursor : Message errored out.  CompletionStatus : " + FrameworkToolKit::localize (completionStatus));

	    }
	    else
	    {
		//SUCCESS case
			m_cursorState = CURSOR_DECLARED;
	    }
	}
	return status;
}

int DatabaseQueryCursor::closeCursor()
{
	string closeCommand = string(" CLOSE  ") + m_cursorName;
	
	DatabaseObjectManagerExecuteCursorCfgCommandMessage 	
	cursorCfgCommandMessage(CURSOR_CLOSE, closeCommand, m_className);
	ResourceId	completionStatus = WAVE_MESSAGE_SUCCESS;
	
	WaveMessageStatus status = m_CallingOm->sendSynchronously (&cursorCfgCommandMessage);

	if (WAVE_MESSAGE_SUCCESS != status)
	{
	    trace (TRACE_LEVEL_ERROR, "DatabaseQueryCursor::closeCursor : Could not send a message to Database Service.  Status : " + FrameworkToolKit::localize (status));

	}
	else
	{
	    completionStatus = cursorCfgCommandMessage.getCompletionStatus ();

	    if (WAVE_MESSAGE_SUCCESS != completionStatus)
	    {
	        trace (TRACE_LEVEL_ERROR, "DatabaseQueryCursor::closeCursor : Message errored out.  CompletionStatus : " + FrameworkToolKit::localize (completionStatus));

	    }
	    else
	    {
		//SUCCESS case
		m_cursorState = CURSOR_CLOSED;
	    }
	}
	return status;
}

void DatabaseQueryCursor::fetchCursor()
{
	//Will support in next iteration. It's nice to have feature.

}

string DatabaseQueryCursor::getSqlOfFetchCursor()
{
	string sqlString = string(" FETCH ") + m_pageSize + string(" FROM ") + 
	m_cursorName + string(";");

	return sqlString;

}

int DatabaseQueryCursor::moveCursor(unsigned int count)
{

	string moveCommand = string(" MOVE  FORWARD ") + count +string(" FROM ") + m_cursorName + string( ";");
	
	DatabaseObjectManagerExecuteCursorCfgCommandMessage cursorCfgCommandMessage(CURSOR_MOVE, moveCommand, m_className);
	ResourceId	completionStatus = WAVE_MESSAGE_SUCCESS;

	WaveMessageStatus	status = m_CallingOm->sendSynchronously (&cursorCfgCommandMessage);

	if (WAVE_MESSAGE_SUCCESS != status)
	{
	    trace (TRACE_LEVEL_ERROR, "DatabaseQueryCursor::moveCursor : Could not send a message to Database Service.  Status : " + FrameworkToolKit::localize (status));

	}
	else
	{
	    completionStatus = cursorCfgCommandMessage.getCompletionStatus ();

	    if (WAVE_MESSAGE_SUCCESS != completionStatus)
	    {
	        trace (TRACE_LEVEL_ERROR, "DatabaseQueryCursor::moveCursor : Message errored out.  CompletionStatus : " + FrameworkToolKit::localize (completionStatus));

	    }
	    else
	    {
		//SUCCESS case
	        trace (TRACE_LEVEL_INFO, "DatabaseQueryCursor::moveCursor : SUCESSS");
		m_startOffset = count;
	    }
	}
	return status;
}

}
