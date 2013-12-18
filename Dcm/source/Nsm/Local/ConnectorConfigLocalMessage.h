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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Venkat                                                       *
 **************************************************************************/

#ifndef CONNECTORCONFIGLOCALMESSAGE_H
#define CONNECTORCONFIGLOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{
	class ConnectorConfigLocalMessage : public PrismMessage
	{
	    private :
    	    void setupAttributesForSerialization ();

	    protected :
    	public :
        	ConnectorConfigLocalMessage ();
	        virtual ~ConnectorConfigLocalMessage ();
	        UI32 getCmdCode();
    	    void setCmdCode(const UI32 &cmdCode);
        	string getName();
	        void setName(const string &name);
            void setBulkConfigsVector (const vector<string> &configs);
            vector<string> getBulkConfigsVector () const;
            void addConfigToBulkConfigsvector (const string &config);
	    private:


    	// Now the data members


	    private :
    	    UI32       m_cmdCode;
        	string     m_name;
            vector<string> m_bulkConfigs;

    	protected :
	    public :
	};
}
#endif //CONNECTORCONFIGLOCALMESSAGE_H
