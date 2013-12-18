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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Bhavin                                                     *
 **************************************************************************/


#ifndef NSMLOCALDESTINATIONMIRRORPORTMESSAGE_H
#define NSMLOCALDESTINATIONMIRRORPORTMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ClientInterface/Span/SpanMessageDef.h"
#include "ClientInterface/Span/SpanClientLocalSpanLocalConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmLocaldestinationMirrorportMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
	        NsmLocaldestinationMirrorportMessage ();
	        NsmLocaldestinationMirrorportMessage (const string &DestifName, const UI32 &opCode);
	        virtual    ~NsmLocaldestinationMirrorportMessage ();
	        void setDestifName(const string &destifName);
	        string getDestifName()  const;
	        void setOpcode(const UI32 &opCode);
	        UI32 getOpcode()  const;
			
	    public:
	    protected:
		private:
		    string  m_destifName;
		    UI32 	m_opCode;
	};
}

#endif                                            //NSMLOCALDESTINATIONMIRRORPORTMESSAGE_H

