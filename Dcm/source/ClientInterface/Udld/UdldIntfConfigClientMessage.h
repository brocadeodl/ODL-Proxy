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
 * UdldIntfConfigClientMessage.h
 *
 * Aug 09, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef __UDLDINTFCONFIGCLIENTMESSAGE_H__
#define __UDLDINTFCONFIGCLIENTMESSAGE_H__

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs {

class UdldIntfConfigClientMessage : public DcmManagementInterfaceMessage
{

    public:
        UdldIntfConfigClientMessage ();
        virtual    ~UdldIntfConfigClientMessage ();

        virtual const void *getCStructureForInputs ();
        virtual void  loadOutputsFromCStructure (const void *out);

        UI32 getCommandCode () const;
        void setCommandCode (const UI32 commandCode);
        InterfaceType getIfType () const;
        string getIfName () const;
        void setInterfaceInfo (const InterfaceType ifType, const string &ifName);
        bool getUdldDisable () const;
        void setUdldDisable (bool udldDisable);

    protected:
        virtual void setupAttributesForSerialization ();

    private:
        UI32    m_commandCode;
        InterfaceType m_ifType;
        string  m_ifName;
        bool    m_udldDisable;

};

}

#endif
