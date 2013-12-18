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

/* **
 * UdldIntfConfigMessage.h
 *
 * Aug 7, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef __UDLDINTFCONFIGMESSAGE_H__
#define __UDLDINTFCONFIGMESSAGE_H__

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    typedef enum {
        UDLD_UNKNOWN_CMD,
        UDLD_ENABLE_CMD,
        UDLD_DISABLE_CMD
    } UdldIfCommand;


    class UdldIntfConfigMessage : public PrismMessage
    {
    public:
        UdldIntfConfigMessage ();
        UdldIntfConfigMessage (const UI32 opCode);
        virtual ~UdldIntfConfigMessage ();

        UdldIfCommand getCommandCode () const;
        void setCommandCode (const UdldIfCommand commandCode);
        InterfaceType getIfType () const;
        void setIfType (const InterfaceType ifType);
        string getIfName () const;
        void setIfName (const string &ifName);
 
    protected:
        virtual void setupAttributesForSerialization ();

    private:
        UdldIfCommand   m_commandCode;
        InterfaceType   m_ifType;
        string          m_ifName;

    };
}

#endif
