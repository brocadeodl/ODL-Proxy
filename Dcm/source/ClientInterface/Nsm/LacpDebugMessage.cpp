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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "ClientInterface/Nsm/LacpDebugMessage.h"

namespace DcmNs
{
    void  LacpDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32((SI32*)&cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&txOrRx,"txOrRx"));
        addSerializableAttribute (new AttributeString(&ifName ,"ifName"));
        addSerializableAttribute (new AttributeUI32(&traceLevel ,"traceLevel"));
        addSerializableAttribute (new AttributeBool(&negate ,"negate"));

    }

    const void *LacpDebugMessage::getCStructureForInputs ()
    {
        LacpDebug_InputMsg_t *pInput = new LacpDebug_InputMsg_t;

        strncpy (pInput->if_name, ifName.c_str(), MAX_STR);
		pInput->if_name[MAX_STR - 1] = '\0';
        pInput->trace_level = traceLevel;
        pInput->cmdCode     = cmdCode;
        pInput->tx_or_rx = txOrRx;
        pInput->negate = negate;
        return (pInput);
    }
}
