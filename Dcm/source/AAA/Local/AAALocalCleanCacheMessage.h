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
 *   Author : kghanta                                                      *
 **************************************************************************/

#ifndef AAALOCALCLEANCACHEMESSAGE_H
#define AAALOCALCLEANCACHEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalCleanCacheMessage : public PrismMessage
{
	protected:
	virtual void setupAttributesForSerialization();
	public:
	AAALocalCleanCacheMessage ();
	AAALocalCleanCacheMessage (const string &caller, const int &defaultPasswdSync, const vector<string> &path);
	virtual    ~AAALocalCleanCacheMessage ();
	string getCaller() const;
	int getDefaultPasswdSync() const;
	void setPath(const vector<string> &path);
	vector<string> getPath() const;

	/* Data Members */
	private:
	string m_callerOp;
	int m_defaultPasswdSync;
	vector<string> m_path;
};

}
#endif
