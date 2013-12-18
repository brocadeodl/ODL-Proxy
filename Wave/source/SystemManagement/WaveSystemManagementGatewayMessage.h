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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef WAVESYSTEMMANAGEMENTGATEWAYMESSAGE_H
#define WAVESYSTEMMANAGEMENTGATEWAYMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class WaveSystemManagementGatewayMessage : public ManagementInterfaceMessage
{
    private :
    protected :
        virtual void setupAttributesForSerialization        ();
    public :
                        WaveSystemManagementGatewayMessage  ();
        virtual        ~WaveSystemManagementGatewayMessage  ();

        string          getWriteInputArguments              ();
        void            setWriteInputArguments              (string writeInputArguments);

        string          getWriteSubMode                     ();
        void            setWriteSubMode                     (string writeSubMode);

        void 			setCommandErrorString				(const string &clientStatus);
        string 			getCommandErrorString				() const;

        bool			getIsWyserEaCompatibilityMode() const;
        void 			setIsWyserEaCompatibilityMode(const bool &isWyserEaCompatibilityMode);


    // Now the data members

    private :
        string  m_writeInputArguments;
        string  m_writeSubMode;
        string  m_errorString;
        bool 	m_isWyserEaCompatibilityMode;
    protected :
    public :
};

}

#endif // WAVESYSTEMMANAGEMENTREBUILDYANGINTERFACESMESSAGE_H 
