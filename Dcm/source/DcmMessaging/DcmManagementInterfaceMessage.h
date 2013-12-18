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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DCMMANAGEMENTINTERFACEMESSAGE_H
#define DCMMANAGEMENTINTERFACEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class DcmManagementInterfaceMessage : public ManagementInterfaceMessage
{
    private :
    protected :
                     DcmManagementInterfaceMessage   (const UI32 &operationId);

        virtual void setupAttributesForSerialization ();

    public :
        virtual            ~DcmManagementInterfaceMessage ();

        virtual const void *getCStructureForInputs        ()                                                    = 0;
        virtual       void  loadOutputsFromCStructure     (const void *pOutputCStructure)                       = 0;

                      SI32  getClientStatus               () const;
                      void  setClientStatus               (const SI32 &clientStatus);
        virtual       void  updateForCompletionStatusDuringSurrogacy ();

    // Now the data members

    private :
        SI32 m_clientStatus;

    protected :
    public :
};

}

#endif // DCMMANAGEMENTINTERFACEMESSAGE_H
