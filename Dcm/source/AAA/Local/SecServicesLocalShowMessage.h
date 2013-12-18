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
 *   Author : Balaji. M                                                    *
 **************************************************************************/

#ifndef SECSERVICESLOCALSHOWMESSAGE_H
#define SECSERVICESLOCALSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include "AAA/Local/AAALocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class TelnetServerLocalShowMessage : public PrismMessage
    {
        /* Member functions */
        public:
	        TelnetServerLocalShowMessage ();
	        TelnetServerLocalShowMessage (const bool &disableFlag);
        	virtual ~TelnetServerLocalShowMessage ();
            bool getDisableFlag();
            void setDisableFlag(const bool &disableFlag);

        protected:
	        virtual void setupAttributesForSerialization();

        /* Data members*/
        private:
        	bool m_disableFlag;
    };

    class SSHServerLocalShowMessage : public PrismMessage
    {
        /* Member functions */
        public:
	        SSHServerLocalShowMessage ();
	        SSHServerLocalShowMessage (const bool &disableFlag);
        	virtual ~SSHServerLocalShowMessage ();
            bool getDisableFlag();
            void setDisableFlag(const bool &disableFlag);

        protected:
	        virtual void setupAttributesForSerialization();

        /* Data members*/
        private:
        	bool m_disableFlag;
    };

    class SSHServerListLocalShowMessage : public PrismMessage
    {
        /* Member functions */
        public:
	        SSHServerListLocalShowMessage ();
	        SSHServerListLocalShowMessage (const SSHLocalKeyExchangeProtocol &protocol);
        	virtual ~SSHServerListLocalShowMessage ();
            SSHLocalKeyExchangeProtocol getKeyExchangeProtocol();
            void setKeyExchangeProtocol(const SSHLocalKeyExchangeProtocol &protocol);

        protected:
	        virtual void setupAttributesForSerialization();

        /* Data members*/
        private:
            SSHLocalKeyExchangeProtocol m_protocol;
    };
}

#endif
