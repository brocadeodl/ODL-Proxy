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
 *   Copyright (C) 2008-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef DCMCLIENTMANAGEMENTINTERFACEMESSAGE_H
#define DCMCLIENTMANAGEMENTINTERFACEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class DcmClientManagementInterfaceMessage : public ManagementInterfaceMessage
{
    private :
    protected :
                            DcmClientManagementInterfaceMessage   (const string &serviceName, const UI32 &operationId);

        virtual       void  setupAttributesForSerialization (); 

    public :
        virtual            ~DcmClientManagementInterfaceMessage (); 

        virtual       void  loadInputFromCStructure       (const void *pInput);
        virtual       void *getOutputForCStructure        () = 0;

    // Now the data members

    private :

    protected :
    public :
};

}

#endif // DCMMANAGEMENTINTERFACEMESSAGE_H

